#!/usr/bin/perl
use FileHandle;
use File::Find;

my %instdir = ();

# Take a Makefile line of the form:
# obj-XXXXX = some_directory/ some_module.o
#
# All directories are processed by including the referenced Makefile and
# removing the directory.  The modules are added to the list of modules
# to install.  Prints the edited line to OUT.
# Arguments: directory Makefile is in, the objects, original line(s) from
# Makefile (with newlines intact).
sub check_line($$$)
{
	my $dir = shift;
	my $objs = shift;
	my $orig = shift;
	my $count = 0;

	foreach(split /\s+/, $objs) {
		if (m|/$|) { # Ends in /, means it's a directory
			# Delete this directory from original Makefile line
			$orig =~ s/$_[ \t]*//;
			$_ .= 'Makefile';
			# print STDERR "open new makefile $dir/$_\n";
			open_makefile("$dir/$_");
			next;
		}

		# It's a file, add it to list of files to install
		s/\.o$/.ko/;
		my $idir = $dir;
		$idir =~ s|^../linux/drivers/media/||;
		$instdir{$idir}{$_} = 1;
		$count++;
	}
	# Removing any tailling whitespace, just to be neat
	$orig =~ s/[ \t]+$//mg;

	# Print out original line, less directories, if there is anything
	# still there
	print OUT $orig if($count);
}

# Uses the string being assigned from a line of the form:
# EXTRA_CFLAGS += -Idrivers/media/something -Dwhatever
#
# All the -Idrivers/media/something options get deleted.  All the source
# files are linked into the v4l directory and built from there, so and -I
# option for them is unnecessary.  The line is printed to OUT if there is
# anything left after that.
sub remove_includes($$)
{
	my $flags = shift;
	my $orig = shift;
	my $count = 0;

	foreach(split /\s+/, $flags) {
		if (m|^-Idrivers/media|) {
			# Remove any -I flags from original Makefile line
			$orig =~ s/$_[ \t]*//;
			next;
		}
		$count++;	# Something wasn't deleted
	}
	$orig =~ s/[ \t]+$//mg;

	# Print out original line if there is anything we didn't delete
	print OUT $orig if($count);
}

sub open_makefile($) {
	my $file = shift;
	my $in = new FileHandle;
	my $orig;

	# only open a given Makefile once
	return if exists $makefiles{$file};
	$makefiles{$file} = 1;

	$file =~ m|^(.*)/[^/]*$|;
	my $dir = $1;

	# print STDERR "opening $file (dir=$dir)\n";
	$file =~ m|.*/(linux/.*)$|;
	print OUT "# Including $1\n";
	open $in, $file;

	while (<$in>) {
		# print STDERR "Line: $_";
		# Skip comment lines
		if (/^\s*#/) {
			print OUT $_;
			next;
		}
		m/^\s*-?include/ and die "Can't handle includes! In $file";

		# Handle line continuations
		if (/\\\n$/) {
			$_ .= <$in>;
			redo;
		}

		# $orig is what we will print, $_ is what we will parse
		$orig = $_;
		# Eat line continuations in string we will parse
		s/\s*\\\n\s*/ /g;
		# Eat comments
		s/#.*$//;

		if (/^\s*obj-.*:?=\s*(\S.*?)\s*$/) {
			# print STDERR "obj matched '$1'\n";
			check_line($dir, $1, $orig);	# will print line for us
			next;
		}
		if (/^\s*EXTRA_CFLAGS\s+\+?=\s*(\S.*?)\s*$/) {
			# print STDERR "cflags matched '$1'\n";
			remove_includes($1, $orig);	# will print line for us
			next;
		}
		print OUT $orig;
	}
	close $in;
}

my %obsolete;
sub getobsolete()
{
	open OBSOLETE, '<obsolete.txt' or die "Can't open obsolete.txt: $!";
	while (<OBSOLETE>) {
		next if (/^\s*#/ || /^\s*$/);
		chomp;
		if (m|^(.*)/([^/]*)$|) {
			$obsolete{$1}{"$2.ko"} = 1;
		} else {
			print "Unable to parse obsolete.txt:$.\n$_\n";
		}
	}

	close OBSOLETE;
}

sub removeobsolete()
{
	while ( my ($dir, $files) = each(%obsolete) ) {
		print OUT "\t\@echo -e \"\\nRemoving obsolete files from \$(KDIR26)/$dir:\"\n";
		print OUT "\t\@files='", join(' ', keys %$files), "'; ";

		print OUT "for i in \$\$files;do if [ -e \$(DESTDIR)\$(KDIR26)/$dir/\$\$i ]; then ";
		print OUT "echo -n \"\$\$i \";";
		print OUT " rm \$(DESTDIR)\$(KDIR26)/$dir/\$\$i; fi; done; ";

		print OUT "for i in \$\$files;do if [ -e \$(DESTDIR)\$(KDIR26)/$dir/\$\$i.gz ]; then ";
		print OUT "echo -n \"\$\$i.gz \";";
		print OUT " rm \$(DESTDIR)\$(KDIR26)/$dir/\$\$i.gz; fi; done; echo;\n\n";
	}
}

#
# Special hack for Ubuntu with their non-standard dir
#
sub removeubuntu()
{
	my $dest = "/lib/modules/\$(KERNELRELEASE)/ubuntu/media";
	my $filelist;

	while ( my ($dir, $files) = each(%instdir) ) {
		$filelist .= ' '. join(' ', keys %$files);
	}
	while ( my ($dir, $files) = each(%obsolete) ) {
		$filelist .= ' ' . join(' ', keys %$files);
	}
	$filelist =~ s/\s+$//;

	print OUT "\t\@if [ -d $dest ]; then ";
	print OUT "printf \"\\nHmm... distro kernel with a non-standard place for module backports detected.\\n";
	print OUT "Please always prefer to use vanilla upstream kernel with V4L/DVB\\n";
	print OUT "I'll try to remove old/obsolete LUM files from $dest:\\n\"; ";
	print OUT "files='", $filelist, "'; ";

	print OUT "for i in \$\$files;do find \"$dest\" \-name \"\$\$i\" \-exec echo \'{}\' \';\' ;";
	print OUT " find \"$dest\" \-name \"\$\$i\" \-exec rm \'{}\' \';\' ;";
	print OUT " done;";
	print OUT " fi\n";
}

sub parse_dir()
{
	my $file = $File::Find::name;

	return if (!($file =~ /Makefile$/));
	open_makefile($file);
}

##############################################################################

getobsolete();

open OUT, '>Makefile.media' or die 'Unable to write Makefile.media';
open_makefile('../linux/drivers/media/Makefile');

find({wanted => \&parse_dir, no_chdir => 1}, '../linux/drivers/staging');

# Creating Install rule
print OUT "media-install::\n";

removeobsolete();
removeubuntu();

print OUT "\t\@echo \"Installing kernel modules under \$(DESTDIR)\$(KDIR26)/:\"\n";

while (my ($dir, $files) = each %instdir) {
	print OUT "\t\@n=0;for i in ", join(' ', keys %$files), ";do ";
	print OUT "if [ -e \"\$\$i\" ]; then ";
	print OUT "if [ \$\$n -eq 0 ]; then ";
	print OUT "echo -n \"\t$dir/: \"; ";
	print OUT "install -d \$(DESTDIR)\$(KDIR26)/$dir; fi; ";
	print OUT "n=\$\$\(\(\$\$n+1\)\); ";
	print OUT "if [  \$\$n -eq 4 ]; then echo; echo -n \"\t\t\"; n=1; fi; ";
	print OUT "echo -n \"\$\$i \"; ";
	print OUT "install -m 644 -c \$\$i \$(DESTDIR)\$(KDIR26)/$dir; fi; done; ";
	print OUT "if [  \$\$n -ne 0 ]; then echo; ";
	print OUT "strip --strip-debug \$(DESTDIR)\$(KDIR26)/$dir/*.ko; ";
	print OUT "fi;\n\n";
}
print OUT "\t@echo\n";
print OUT "\t/sbin/depmod -a \$(KERNELRELEASE) \$(if \$(DESTDIR),-b \$(DESTDIR))\n\n";

# Creating Remove rule
print OUT "media-rminstall::\n";

removeobsolete();
removeubuntu();

while ( my ($dir, $files) = each(%instdir) ) {
	print OUT "\t\@echo -e \"\\nRemoving old \$(KDIR26)/$dir files:\"\n";
	print OUT "\t\@files='", join(' ', keys %$files), "'; ";

	print OUT "for i in \$\$files;do if [ -e \$(DESTDIR)\$(KDIR26)/$dir/\$\$i ]; then ";
	print OUT "echo -n \"\$\$i \";";
	print OUT " rm \$(DESTDIR)\$(KDIR26)/$dir/\$\$i; fi; done; ";

	print OUT "for i in \$\$files;do if [ -e \$(DESTDIR)\$(KDIR26)/$dir/\$\$i.gz ]; then ";
	print OUT "echo -n \"\$\$i.gz \";";
	print OUT " rm \$(DESTDIR)\$(KDIR26)/$dir/\$\$i.gz; fi; done; echo;\n\n";
}

my $mediadeps = join(" \\\n", map("\t../linux/drivers/media/$_/Makefile", keys %instdir));
$mediadeps =~ s,\.\./linux/drivers/media/\.\.,..,g;

# Print dependencies of Makefile.media
print OUT "Makefile.media: ../linux/drivers/media/Makefile \\\n";
print OUT "\tobsolete.txt \\\n";
print OUT $mediadeps;
print OUT "\n";
close OUT;
