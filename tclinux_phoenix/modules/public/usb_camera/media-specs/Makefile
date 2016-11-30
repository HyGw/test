SHELL=/bin/bash

V4L_SGMLS = \
	v4l/biblio.xml \
	v4l/common.xml \
	v4l/compat.xml \
	v4l/controls.xml \
	v4l/dev-capture.xml \
	v4l/dev-codec.xml \
	v4l/dev-effect.xml \
	v4l/dev-osd.xml \
	v4l/dev-output.xml \
	v4l/dev-overlay.xml \
	v4l/dev-radio.xml \
	v4l/dev-raw-vbi.xml \
	v4l/dev-rds.xml \
	v4l/dev-sliced-vbi.xml \
	v4l/dev-teletext.xml \
	v4l/driver.xml \
	v4l/libv4l.xml \
	v4l/remote_controllers.xml \
	v4l/fdl-appendix.xml \
	v4l/func-close.xml \
	v4l/func-ioctl.xml \
	v4l/func-mmap.xml \
	v4l/func-munmap.xml \
	v4l/func-open.xml \
	v4l/func-poll.xml \
	v4l/func-read.xml \
	v4l/func-select.xml \
	v4l/func-write.xml \
	v4l/io.xml \
	v4l/pixfmt-grey.xml \
	v4l/pixfmt-nv12.xml \
	v4l/pixfmt-nv16.xml \
	v4l/pixfmt-packed-rgb.xml \
	v4l/pixfmt-packed-yuv.xml \
	v4l/pixfmt-sbggr16.xml \
	v4l/pixfmt-sbggr8.xml \
	v4l/pixfmt-sgbrg8.xml \
	v4l/pixfmt-sgrbg8.xml \
	v4l/pixfmt-uyvy.xml \
	v4l/pixfmt-vyuy.xml \
	v4l/pixfmt-y16.xml \
	v4l/pixfmt-y41p.xml \
	v4l/pixfmt-yuv410.xml \
	v4l/pixfmt-yuv411p.xml \
	v4l/pixfmt-yuv420.xml \
	v4l/pixfmt-yuv422p.xml \
	v4l/pixfmt-yuyv.xml \
	v4l/pixfmt-yvyu.xml \
	v4l/pixfmt.xml \
	v4l/vidioc-cropcap.xml \
	v4l/vidioc-dbg-g-register.xml \
	v4l/vidioc-encoder-cmd.xml \
	v4l/vidioc-enum-fmt.xml \
	v4l/vidioc-enum-frameintervals.xml \
	v4l/vidioc-enum-framesizes.xml \
	v4l/vidioc-enumaudio.xml \
	v4l/vidioc-enumaudioout.xml \
	v4l/vidioc-enuminput.xml \
	v4l/vidioc-enumoutput.xml \
	v4l/vidioc-enum-dv-presets.xml \
	v4l/vidioc-g-dv-preset.xml \
	v4l/vidioc-query-dv-preset.xml \
	v4l/vidioc-g-dv-timings.xml \
	v4l/vidioc-enumstd.xml \
	v4l/vidioc-g-audio.xml \
	v4l/vidioc-g-audioout.xml \
	v4l/vidioc-dbg-g-chip-ident.xml \
	v4l/vidioc-g-crop.xml \
	v4l/vidioc-g-ctrl.xml \
	v4l/vidioc-g-enc-index.xml \
	v4l/vidioc-g-ext-ctrls.xml \
	v4l/vidioc-g-fbuf.xml \
	v4l/vidioc-g-fmt.xml \
	v4l/vidioc-g-frequency.xml \
	v4l/vidioc-g-input.xml \
	v4l/vidioc-g-jpegcomp.xml \
	v4l/vidioc-g-modulator.xml \
	v4l/vidioc-g-output.xml \
	v4l/vidioc-g-parm.xml \
	v4l/vidioc-g-priority.xml \
	v4l/vidioc-g-sliced-vbi-cap.xml \
	v4l/vidioc-g-std.xml \
	v4l/vidioc-g-tuner.xml \
	v4l/vidioc-log-status.xml \
	v4l/vidioc-overlay.xml \
	v4l/vidioc-qbuf.xml \
	v4l/vidioc-querybuf.xml \
	v4l/vidioc-querycap.xml \
	v4l/vidioc-queryctrl.xml \
	v4l/vidioc-querystd.xml \
	v4l/vidioc-reqbufs.xml \
	v4l/vidioc-s-hw-freq-seek.xml \
	v4l/vidioc-streamon.xml \
	v4l/capture.c.xml \
	v4l/keytable.c.xml \
	v4l/v4l2grab.c.xml \
	v4l/videodev2.h.xml \
	v4l/v4l2.xml

V4L_PDFPICS = \
	v4l/crop.pdf \
	v4l/fieldseq_bt.pdf \
	v4l/fieldseq_tb.pdf \
	v4l/vbi_525.pdf \
	v4l/vbi_625.pdf \
	v4l/vbi_hsync.pdf

V4L_GIFPICS = \
	v4l/crop.gif \
	v4l/fieldseq_bt.gif \
	v4l/fieldseq_tb.gif \
	v4l/vbi_525.gif \
	v4l/vbi_625.gif \
	v4l/vbi_hsync.gif

DVB_SGMLS = \
	dvb/intro.xml \
	dvb/frontend.xml \
	dvb/dvbproperty.xml \
	dvb/demux.xml \
	dvb/video.xml \
	dvb/audio.xml \
	dvb/ca.xml \
	dvb/net.xml \
	dvb/kdapi.xml \
	dvb/examples.xml \
	dvb/frontend.h.xml \
	dvb/dvbapi.xml

DVB_PDFPICS = \
	dvb/dvbstb.pdf

DVB_GIFPICS = \
	dvb/dvbstb.png


SGMLS = $(V4L_SGMLS) $(DVB_SGMLS) media.xml media-entities.tmpl media-indices.tmpl

PDFPICS = $(V4L_PDFPICS) $(DVB_PDFPICS)

GIFPICS = $(V4L_GIFPICS) $(DVB_GIFPICS)

# Some Jade warnings for authors.
WARN =
#WARN = -w all -w duplicate -w net

# Stylesheet customization.
	# docbook/db2 syntax
CUSTOM_HTML = --dsl custom.dsl\#html
CUSTOM_PRINT = --dsl custom.dsl\#print
	# xmlto syntax
CUSTOM_HTML_XMLTO = -m custom.xsl
CUSTOM_PRINT_XMLTO = -m custom.xsl

all: capture html-single

prep: Makefile checks $(SGMLS) $(GIFPICS)

html: html-build.stamp

html-single: html-single-build.stamp

pdf: pdf-build.stamp

man: man-build.stamp

man_install: man-build.stamp
	install -d /usr/local/man/man2
	install -b man/* /usr/local/man/man2

coffeebreak: all html pdf

FUNCS = \
	close \
	ioctl \
	mmap \
	munmap \
	open \
	poll \
	read \
	select \
	write \

IOCTLS = \
	VIDIOC_CROPCAP \
	VIDIOC_DBG_G_CHIP_IDENT \
	VIDIOC_DBG_G_REGISTER \
	VIDIOC_DBG_S_REGISTER \
	VIDIOC_DQBUF \
	VIDIOC_ENCODER_CMD \
	VIDIOC_ENUMAUDIO \
	VIDIOC_ENUMAUDOUT \
	VIDIOC_ENUMINPUT \
	VIDIOC_ENUMOUTPUT \
	VIDIOC_ENUMSTD \
	VIDIOC_ENUM_DV_PRESETS \
	VIDIOC_ENUM_FMT \
	VIDIOC_ENUM_FRAMEINTERVALS \
	VIDIOC_ENUM_FRAMESIZES \
	VIDIOC_G_AUDIO \
	VIDIOC_G_AUDOUT \
	VIDIOC_G_CROP \
	VIDIOC_G_CTRL \
	VIDIOC_G_DV_PRESET \
	VIDIOC_G_DV_TIMINGS \
	VIDIOC_G_ENC_INDEX \
	VIDIOC_G_EXT_CTRLS \
	VIDIOC_G_FBUF \
	VIDIOC_G_FMT \
	VIDIOC_G_FREQUENCY \
	VIDIOC_G_INPUT \
	VIDIOC_G_JPEGCOMP \
	VIDIOC_G_MPEGCOMP \
	VIDIOC_G_MODULATOR \
	VIDIOC_G_OUTPUT \
	VIDIOC_G_PARM \
	VIDIOC_G_PRIORITY \
	VIDIOC_G_SLICED_VBI_CAP \
	VIDIOC_G_STD \
	VIDIOC_G_TUNER \
	VIDIOC_LOG_STATUS \
	VIDIOC_OVERLAY \
	VIDIOC_QBUF \
	VIDIOC_QUERYBUF \
	VIDIOC_QUERYCAP \
	VIDIOC_QUERYCTRL \
	VIDIOC_QUERYMENU \
	VIDIOC_QUERYSTD \
	VIDIOC_QUERY_DV_PRESET \
	VIDIOC_REQBUFS \
	VIDIOC_STREAMOFF \
	VIDIOC_STREAMON \
	VIDIOC_S_AUDIO \
	VIDIOC_S_AUDOUT \
	VIDIOC_S_CROP \
	VIDIOC_S_CTRL \
	VIDIOC_S_DV_PRESET \
	VIDIOC_S_DV_TIMINGS \
	VIDIOC_S_EXT_CTRLS \
	VIDIOC_S_FBUF \
	VIDIOC_S_FMT \
	VIDIOC_S_FREQUENCY \
	VIDIOC_S_HW_FREQ_SEEK \
	VIDIOC_S_INPUT \
	VIDIOC_S_JPEGCOMP \
	VIDIOC_S_MPEGCOMP \
	VIDIOC_S_MODULATOR \
	VIDIOC_S_OUTPUT \
	VIDIOC_S_PARM \
	VIDIOC_S_PRIORITY \
	VIDIOC_S_STD \
	VIDIOC_S_TUNER \
	VIDIOC_TRY_ENCODER_CMD \
	VIDIOC_TRY_EXT_CTRLS \
	VIDIOC_TRY_FMT \

TYPES = \
	v4l2_std_id \

ENUMS = \
	v4l2_buf_type \
	v4l2_colorspace \
	v4l2_ctrl_type \
	v4l2_exposure_auto_type \
	v4l2_field \
	v4l2_frmivaltypes \
	v4l2_frmsizetypes \
	v4l2_memory \
	v4l2_mpeg_audio_ac3_bitrate \
	v4l2_mpeg_audio_crc \
	v4l2_mpeg_audio_emphasis \
	v4l2_mpeg_audio_encoding \
	v4l2_mpeg_audio_l1_bitrate \
	v4l2_mpeg_audio_l2_bitrate \
	v4l2_mpeg_audio_l3_bitrate \
	v4l2_mpeg_audio_mode \
	v4l2_mpeg_audio_mode_extension \
	v4l2_mpeg_audio_sampling_freq \
	v4l2_mpeg_cx2341x_video_chroma_spatial_filter_type \
	v4l2_mpeg_cx2341x_video_luma_spatial_filter_type \
	v4l2_mpeg_cx2341x_video_median_filter_type \
	v4l2_mpeg_cx2341x_video_spatial_filter_mode \
	v4l2_mpeg_cx2341x_video_temporal_filter_mode \
	v4l2_mpeg_stream_type \
	v4l2_mpeg_stream_vbi_fmt \
	v4l2_mpeg_video_aspect \
	v4l2_mpeg_video_bitrate_mode \
	v4l2_mpeg_video_encoding \
	v4l2_power_line_frequency \
	v4l2_priority \
	v4l2_tuner_type \
	v4l2_preemphasis \

STRUCTS = \
	v4l2_audio \
	v4l2_audioout \
	v4l2_bt_timings \
	v4l2_buffer \
	v4l2_capability \
	v4l2_captureparm \
	v4l2_clip \
	v4l2_control \
	v4l2_crop \
	v4l2_cropcap \
	v4l2_dbg_chip_ident \
	v4l2_dbg_match \
	v4l2_dbg_register \
	v4l2_dv_enum_preset \
	v4l2_dv_preset \
	v4l2_dv_timings \
	v4l2_enc_idx \
	v4l2_enc_idx_entry \
	v4l2_encoder_cmd \
	v4l2_ext_control \
	v4l2_ext_controls \
	v4l2_fmtdesc \
	v4l2_format \
	v4l2_fract \
	v4l2_framebuffer \
	v4l2_frequency \
	v4l2_frmival_stepwise \
	v4l2_frmivalenum \
	v4l2_frmsize_discrete \
	v4l2_frmsize_stepwise \
	v4l2_frmsizeenum \
	v4l2_hw_freq_seek \
	v4l2_input \
	v4l2_jpegcompression \
	v4l2_modulator \
	v4l2_mpeg_vbi_fmt_ivtv \
	v4l2_output \
	v4l2_outputparm \
	v4l2_pix_format \
	v4l2_queryctrl \
	v4l2_querymenu \
	v4l2_rect \
	v4l2_requestbuffers \
	v4l2_sliced_vbi_cap \
	v4l2_sliced_vbi_data \
	v4l2_sliced_vbi_format \
	v4l2_standard \
	v4l2_streamparm \
	v4l2_timecode \
	v4l2_tuner \
	v4l2_vbi_format \
	v4l2_window \

ERRORS = \
	EACCES \
	EAGAIN \
	EBADF \
	EBUSY \
	EFAULT \
	EIO \
	EINTR \
	EINVAL \
	ENFILE \
	ENOMEM \
	ENOSPC \
	ENOTTY \
	ENXIO \
	EMFILE \
	EPERM \
	ERANGE \

ESCAPE = \
	-e "s/&/\\&amp;/g" \
	-e "s/</\\&lt;/g" \
	-e "s/>/\\&gt;/g"

FILENAME = \
	-e s,"^[^\/]*/",, \
	-e s/"\\.xml"// \
	-e s/"\\.tmpl"// \
	-e s/\\\./-/g \
	-e s/"^func-"// \
	-e s/"^pixfmt-"// \
	-e s/"^vidioc-"//

# Generate references to these structs in videodev2.h.xml.
DOCUMENTED = \
	-e "s/\(enum *\)v4l2_mpeg_cx2341x_video_\([a-z]*_spatial_filter_type\)/\1<link linkend=\"\2\">v4l2_mpeg_cx2341x_video_\2<\/link>/g" \
	-e "s/\(\(enum\|struct\) *\)\(v4l2_[a-zA-Z0-9_]*\)/\1<link linkend=\"\3\">\3<\/link>/g" \
	-e "s/\(V4L2_PIX_FMT_[A-Z0-9_]\+\) /<link linkend=\"\1\">\1<\/link> /g" \
	-e ":a;s/\(linkend=\".*\)_\(.*\">\)/\1-\2/;ta" \
	-e "s/\(linkend\=\"\)v4l2\-mpeg\-vbi\-ITV0/\1v4l2-mpeg-vbi-itv0-1/g"

DVB_DOCUMENTED = \
	-e "s,\(define \)\([A-Z0-9_]\+\)\(\s\+_IO\),\1\<link linkend=\"\2\">\2\<\/link\>\3,g" \
	-e "s/\(linkend\=\"\)FE_SET_PROPERTY/\1FE_GET_PROPERTY/g"

.PHONY: links

$(SGMLS): links
$(PDFPICS): links
$(GIFPICS): links

links::
	-rm v4l dvb
	ln -sf ../linux/Documentation/DocBook/media.tmpl media.xml
	ln -sf ../linux/Documentation/DocBook/v4l .
	ln -sf ../linux/Documentation/DocBook/dvb .

v4l/libv4l-fmt.xml: ../v4l2-apps/libv4l/libv4lconvert/*.c Makefile
	cat ../v4l2-apps/libv4l/libv4lconvert/*.c| \
	perl -ne 'if (m/(V4L2_PIX_FMT_[^\s\;\\)\,:]+)/) { printf "<link linkend=\"$$1\"><constant>$$1</constant></link>,\n"; };' \
	|sort|uniq| \
	sed  -e ":a;s/\(linkend=\".*\)_\(.*\">\)/\1-\2/;ta" > $@

v4l/capture.c.xml: examples/capture-example.c Makefile
	echo "<programlisting>" > $@
	expand --tabs=8 < $< | \
	  sed $(ESCAPE) $(DOCUMENTED) | \
	  sed 's/i\.e\./&ie;/' >> $@
	echo "</programlisting>" >> $@

v4l/v4l2grab.c.xml: examples/v4l2grab.c Makefile
	echo "<programlisting>" > $@
	expand --tabs=8 < $< | \
	  sed $(ESCAPE) $(DOCUMENTED) | \
	  sed 's/i\.e\./&ie;/' >> $@
	echo "</programlisting>" >> $@

v4l/keytable.c.xml: examples/keytable.c Makefile
	echo "<programlisting>" > $@
	expand --tabs=8 < $< | \
	  sed $(ESCAPE) $(DOCUMENTED) | \
	  sed 's/i\.e\./&ie;/' >> $@
	echo "</programlisting>" >> $@

v4l/videodev2.h.xml: ../linux/include/linux/videodev2.h Makefile
	echo "<programlisting>" > $@
	expand --tabs=8 < $< | \
	  sed $(ESCAPE) $(DOCUMENTED) | \
	  sed 's/i\.e\./&ie;/' >> $@
	echo "</programlisting>" >> $@

dvb/frontend.h.xml: ../linux/include/linux/dvb/frontend.h Makefile
	echo "<programlisting>" > $@
	expand --tabs=8 < $< | \
	  sed $(ESCAPE) $(DVB_DOCUMENTED) | \
	  sed 's/i\.e\./&ie;/' >> $@
	echo "</programlisting>" >> $@

checks: $(SGMLS)
	# Warn about <constant>FOO-BAR</constant> which should be
	# &FOO-BAR; or <constant>FOO_BAR</constant>.
	(! grep '>[A-Z]+-[A-Z-]+<' $(V4L_SGMLS))
	# Don't end a refpurpose blob with a period.
	(! grep '\.</refpurpose>' $(V4L_SGMLS))
	# Catch typos in error code names.
	errs=`grep '<errorcode>[^<]\+</errorcode>' $(V4L_SGMLS) | \
	      sed 's/.*<errorcode>//;s/<\/errorcode>.*//' | sort -u` ; \
	for err in $$errs; do \
	  if ! echo $(ERRORS) | grep -q $$err ; then \
	    echo Unknown error code $$err ; exit 1 ; \
	  fi ; \
	done
	# Use the respective entities instead.
	for abbrev in "e\\.g\\." "i\\.e\\." "i2c" ; do \
	  grep "[^a-z&]$$abbrev[^a-z.;-]" $(V4L_SGMLS) | \
	  grep -v -E "(ENTITY|videodev2\.h)" && exit 1 ; \
	done ; \
	exit 0

# We're generating these entities to avoid typos, to enforce consistent
# naming, an to be warned about missing items (Jade screams bloody murder
# if references are dangling).
media-entities.tmpl: ../linux/Documentation/DocBook/media-entities.tmpl
	ln -sf ../linux/Documentation/DocBook/$@ $@

../linux/Documentation/DocBook/media-entities.tmpl: Makefile $(SGMLS)
	echo "<!-- Generated file! Do not edit. -->" >$@
	echo -e "\n<!-- Functions -->" >>$@
	for ident in $(FUNCS) ; do \
	  entity=`echo $$ident | tr _ -` ; \
	  echo "<!ENTITY func-$$entity \"<link" \
	    "linkend='func-$$entity'><function>$$ident()</function></link>\">" \
	  >>$@ ; \
	done
	echo -e "\n<!-- Ioctls -->" >>$@
	for ident in $(IOCTLS) ; do \
	  entity=`echo $$ident | tr _ -` ; \
	  id=`grep "<refname>$$ident" v4l/vidioc-*.xml |sed -r s,"^[^\/]*/",, | sed 's/\.xml.*//'` ; \
	  echo "<!ENTITY $$entity \"<link" \
	    "linkend='$$id'><constant>$$ident</constant></link>\">" \
	  >>$@ ; \
	done
	echo -e "\n<!-- Types -->" >>$@
	for ident in $(TYPES) ; do \
	  entity=`echo $$ident | tr _ -` ; \
	  echo "<!ENTITY $$entity \"<link" \
	    "linkend='$$entity'>$$ident</link>\">" >>$@ ; \
	done
	echo -e "\n<!-- Enums -->" >>$@
	for ident in $(ENUMS) ; do \
	  entity=`echo $$ident | sed -e "s/v4l2_mpeg_cx2341x_video_\([a-z]*_spatial_filter_type\)/\1/" | tr _ -` ; \
	  echo "<!ENTITY $$entity \"enum&nbsp;<link" \
	    "linkend='$$entity'>$$ident</link>\">" >>$@ ; \
	done
	echo -e "\n<!-- Structures -->" >>$@
	for ident in $(STRUCTS) ; do \
	  entity=`echo $$ident | tr _ -` ; \
	  echo "<!ENTITY $$entity \"struct&nbsp;<link" \
	    "linkend='$$entity'>$$ident</link>\">" >>$@ ; \
	done
	echo -e "\n<!-- Error Codes -->" >>$@
	for ident in $(ERRORS) ; do \
	  echo "<!ENTITY $$ident \"<errorcode>$$ident</errorcode>" \
	    "error code\">" >>$@ ; \
	done
	echo -e "\n<!-- Subsections -->" >>$@
	for file in $(SGMLS) ; do \
	  entity=`echo "$$file" | sed $(FILENAME) -e s/"^([^-]*)"/sub\1/` ; \
	  if ! echo "$$file" | \
	    grep -q -E -e '^(func|vidioc|pixfmt)-' ; then \
	    echo "<!ENTITY sub-$$entity SYSTEM \"$$file\">" >>$@ ; \
	  fi ; \
	done
	echo -e "\n<!-- Function Reference -->" >>$@
	for file in $(SGMLS) ; do \
	  if echo "$$file" | \
	    grep -q -E -e '(func|vidioc|pixfmt)-' ; then \
	    entity=`echo "$$file" |sed $(FILENAME)` ; \
	    echo "<!ENTITY $$entity SYSTEM \"$$file\">" >>$@ ; \
	  fi ; \
	done

# Jade can auto-generate a list-of-tables, which includes all structs,
# but we only want data types, all types, and sorted please.
media-indices.tmpl: ../linux/Documentation/DocBook/media-indices.tmpl
	ln -sf ../linux/Documentation/DocBook/$@ $@

../linux/Documentation/DocBook/media-indices.tmpl: Makefile $(SGMLS)
	echo "<!-- Generated file! Do not edit. -->" >$@
	echo -e "\n<index><title>List of Types</title>" >>$@
	for ident in $(TYPES) ; do \
	  id=`echo $$ident | tr _ -` ; \
	  echo "<indexentry><primaryie><link" \
	    "linkend='$$id'>$$ident</link></primaryie></indexentry>" >>$@ ; \
	done
	for ident in $(ENUMS) ; do \
	  id=`echo $$ident | sed -e "s/v4l2_mpeg_cx2341x_video_\([a-z]*_spatial_filter_type\)/\1/" | tr _ -`; \
	  echo "<indexentry><primaryie>enum&nbsp;<link" \
	    "linkend='$$id'>$$ident</link></primaryie></indexentry>" >>$@ ; \
	done
	for ident in $(STRUCTS) ; do \
	  id=`echo $$ident | tr _ -` ; \
	  echo "<indexentry><primaryie>struct&nbsp;<link" \
	    "linkend='$$id'>$$ident</link></primaryie></indexentry>" >>$@ ; \
	done
	echo "</index>" >>$@

# HTML version.
html-build.stamp: Makefile checks $(SGMLS) $(GIFPICS)
	rm -rf media
	if which xmlto >/dev/null ; then \
	  xmlto xhtml $(WARN) $(CUSTOM_HTML_XMLTO) -o media media.xml ; \
	elif which docbook2html >/dev/null ; then \
	  export DCL="--dcl `find /usr/share/sgml -name xml.dcl |head -1`"; \
	  docbook2html $(WARN) $$DCL $(CUSTOM_HTML) --output media media.xml ; \
	else \
	  export DCL="--dcl `find /usr/share/sgml -name xml.dcl |head -1`"; \
	  db2html $(WARN) $$DCL $(CUSTOM_HTML) --output media media.xml ; \
	fi
	cp $(GIFPICS) media/
	cd media ; \
	test -e index.html || ln -s book1.htm index.html ; \
	test -e capture-example.html || \
	  ln -s `grep -l getopt_long *.htm` capture-example.html
	chmod a+rX -R media
	touch html-build.stamp

# For online version. When you have a dial-up connection a single file
# is more convenient than clicking through dozens of pages.
html-single-build.stamp: Makefile checks $(SGMLS) $(GIFPICS)
	rm -rf media-single
	if which xmlto >/dev/null ; then \
	  xmlto html-nochunks $(WARN) $(CUSTOM_HTML_XMLTO) -o media-single media.xml ; \
	elif which docbook2html >/dev/null ; then \
	  export DCL="--dcl `find /usr/share/sgml -name xml.dcl |head -1`"; \
	  docbook2html $$DCL $(WARN) $(CUSTOM_HTML) --nochunks \
	    --output media-single media.xml ; \
	else \
	  export DCL="--dcl `find /usr/share/sgml -name xml.dcl |head -1`"; \
	  db2html $$DCL $(WARN) $(CUSTOM_HTML) --nochunks \
	    --output media-single media.xml ; \
	fi
	cp $(GIFPICS) media-single/
	chmod a+rX -R media-single
	touch html-single-build.stamp

# For printing.
pdf-build.stamp: Makefile checks $(SGMLS) $(PDFPICS)
	if which db2pdf >/dev/null ; then \
	  export DCL="--dcl `find /usr/share/sgml -name xml.dcl|head -1`"; \
	  db2pdf $$DCL $(WARN) $(CUSTOM_PRINT) media.xml ; \
	elif which xmlto >/dev/null ; then \
	  xmlto pdf $(WARN) $(CUSTOM_HTML_XMLTO) -o media media.xml ; \
	else \
	  export DCL="--dcl `find /usr/share/sgml -name xml.dcl |head -1`"; \
	  docbook2pdf $$DCL $(WARN) $(CUSTOM_PRINT) media.xml ; \
	fi
	touch pdf-build.stamp


# For man
man-build.stamp: Makefile checks $(SGMLS) $(PDFPICS)
	xmlto man $(WARN) $(CUSTOM_HTML_XMLTO) -o man media.xml

# Example.
capture: examples/capture-example.c
	gcc $^ -o $@

distclean clean:
	rm -f *.stamp
	rm -f videodev2.h.xml
	rm -f capture.c.xml
	rm -f v4l2grab.c.xml
	rm -f keytable.c.xml
	rm -f media.aux media.log media.out
	rm -f capture
	rm -f media-indices.tmpl media-entities.tmpl
	rm -rf media media-single media.pdf
	rm -rf media.xml v4l dvb
