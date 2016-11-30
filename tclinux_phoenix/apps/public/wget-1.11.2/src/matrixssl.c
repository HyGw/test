/* SSL support via matrixssl library 

Adapted from openssl.c: January 2008 by Eric Paul Bishop

This file is part of GNU Wget.

GNU Wget is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

GNU Wget is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Wget; if not, write to the Free Software
Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

[NOTE: Open SSL exception removed -- the whole point of this file 
is to eliminate dependence on openssl for embedded systems,
e.g. OpenWrt (www.openwrt.org), that don't have the space for it.]

*/

#include <config.h>


#include <stdio.h>

//#include "options.h"

#include <assert.h>
#include <errno.h>
#ifdef HAVE_UNISTD_H
# include <unistd.h>
#endif
#ifdef HAVE_STRING_H
# include <string.h>
#else
# include <strings.h>
#endif

#include <stdlib.h>

#include "wget.h"
#include "utils.h"
#include "connect.h"
#include "url.h"
#include "ssl.h"

#include "matrixssl_helper.h"

static int certChecker(sslCertInfo_t *cert, void *arg);


#ifndef errno
extern int errno;
#endif

/* Application-wide SSL context.  This is common to all SSL
   connections.  */
sslKeys_t *ssl_ctx;


typedef struct 
{
	int verified;
	char* common_name;
} my_cert_info;

static my_cert_info* global_certificate = NULL;

/*
static void
init_prng (void)
{
}

static void
print_errors (void) 
{
}

static int
key_type_to_ssl_type (enum keyfile_type type)
{
}
*/



/* Create an SSL Context and set default paths etc.  Called the first
   time an HTTP download is attempted.

   Returns 1 on success, 0 otherwise.  */

bool ssl_init ()
{


	int return_value = 1;
	if (!ssl_ctx)
	{
		/* ssl keys have not been initialized. */
		
		return_value = matrixSslOpen();
		if(return_value >= 0)
		{
			return_value = matrixSslReadKeys(&ssl_ctx, NULL, NULL, NULL, NULL);

		}
	}
	return return_value >= 0 ? true : false;
}




static int
matrixssl_read (int fd, char *buf, int bufsize, void *ctx)
{
	SSL *ssl = (SSL *) ctx;
	return SSL_read(ssl, buf, bufsize);
}

static int
matrixssl_write (int fd, char *buf, int bufsize, void *ctx)
{
	SSL *ssl = (SSL *) ctx;
	return SSL_write (ssl, buf, bufsize);
}

static int
matrixssl_poll (int fd, double timeout, int wait_for, void *ctx)
{
	SSL *ssl = (SSL *) ctx;
	if (timeout == 0)
	{
		return 1;
	}
	if(ssl->insock.end == ssl->insock.start && ssl->inbuf.end == ssl->inbuf.start)
	{
		return 1;
	}
	return select_fd (fd, timeout, wait_for);
}

static int
matrixssl_peek (int fd, char *buf, int bufsize, void *ctx)
{
	SSL *ssl = (SSL *) ctx;
	return SSL_peek(ssl, buf, bufsize);
}

static void
matrixssl_close (int fd, void *ctx)
{
	SSL *ssl = (SSL *) ctx;
	if(global_certificate != NULL)
	{
		free(global_certificate->common_name);
		free(global_certificate);
		global_certificate = NULL;
	}
	SSL_free (ssl);
	#ifdef WINDOWS
		closesocket (fd);
	#else
		close (fd);
	#endif

	DEBUGP (("Closed %d/SSL 0x%0lx\n", fd, (unsigned long) ssl));
}




static const char *
matrixssl_errstr (int fd, void *arg)
{
	return "";
}





static struct transport_implementation matrixssl_transport = {
  		matrixssl_read, matrixssl_write, matrixssl_poll,
  		matrixssl_peek, matrixssl_errstr, matrixssl_close
		};





/* Perform the SSL handshake on file descriptor FD, which is assumed
   to be connected to an SSL server.  The SSL handle provided by
   OpenSSL is registered with the file descriptor FD using
   fd_register_transport, so that subsequent calls to fd_read,
   fd_write, etc., will use the corresponding SSL functions.

   Returns 1 on success, 0 on failure.  */

bool ssl_connect (int fd) 
{
	SSL *ssl;

	DEBUGP (("Initiating SSL handshake.\n"));

	assert (ssl_ctx != NULL);
	ssl = SSL_new (ssl_ctx, 0); //flags=0 for ssl client
	
	if (!ssl)
		goto error;

	
	SSL_set_fd(ssl, fd);

	if (SSL_connect (ssl, certChecker, ssl) < 0)
		goto error;




	/* Register FD with Wget's transport layer, i.e. arrange that our
	functions are used for reading, writing, and polling.  */
	fd_register_transport (fd, &matrixssl_transport, ssl);
	DEBUGP (("Handshake successful; connected socket %d to SSL handle 0x%0*lx\n",
				fd, PTR_FORMAT (ssl)));
	return true;

	error:
		DEBUGP (("SSL handshake failed.\n"));
		if (ssl)
		{
			if(global_certificate != NULL)
			{
				if(global_certificate->common_name != NULL)
				{
					free(global_certificate->common_name);
				}
				free(global_certificate);
				global_certificate = NULL;
			}
			SSL_free (ssl);
		
		}
		return false;
}


static int certChecker(sslCertInfo_t *cert, void *arg)
{
	//save certificate info for checking later, and return valid
	if(global_certificate != NULL)
	{
		if(global_certificate->common_name != NULL)
		{
			free(global_certificate->common_name);
		}
		free(global_certificate);
		global_certificate = NULL;
	}

	my_cert_info* info = (my_cert_info*)malloc(sizeof(my_cert_info));
	info->verified = cert->verified;
	info->common_name = strdup( (cert->subject).commonName );
	global_certificate = info;


	// we check information elsewhere, so always return valid
	// >= 0 is valid, < 0 is invalid

	return 1;	
}		





/******************************************************************************/




#define ASTERISK_EXCLUDES_DOT	/* mandated by rfc2818 */

/* Return 1 is STRING (case-insensitively) matches PATTERN, 0
   otherwise.  The recognized wildcard character is "*", which matches
   any character in STRING except ".".  Any number of the "*" wildcard
   may be present in the pattern.

   This is used to match of hosts as indicated in rfc2818: "Names may
   contain the wildcard character * which is considered to match any
   single domain name component or component fragment. E.g., *.a.com
   matches foo.a.com but not bar.foo.a.com. f*.com matches foo.com but
   not bar.com [or foo.bar.com]."

   If the pattern contain no wildcards, pattern_match(a, b) is
   equivalent to !strcasecmp(a, b).  */

static int
pattern_match (const char *pattern, const char *string)
{
  const char *p = pattern, *n = string;
  char c;
  for (; (c = TOLOWER (*p++)) != '\0'; n++)
    if (c == '*')
      {
	for (c = TOLOWER (*p); c == '*'; c = TOLOWER (*++p))
	  ;
	for (; *n != '\0'; n++)
	  if (TOLOWER (*n) == c && pattern_match (p, n))
	    return 1;
#ifdef ASTERISK_EXCLUDES_DOT
	  else if (*n == '.')
	    return 0;
#endif
	return c == '\0';
      }
    else
      {
	if (c != TOLOWER (*n))
	  return 0;
      }
  return *n == '\0';
}

/* Verify the validity of the certificate presented by the server.
   Also check that the "common name" of the server, as presented by
   its certificate, corresponds to HOST.  (HOST typically comes from
   the URL and is what the user thinks he's connecting to.)

   This assumes that ssl_connect has successfully finished, i.e. that
   the SSL handshake has been performed and that FD is connected to an
   SSL handle.

   If opt.check_cert is non-zero (the default), this returns 1 if the
   certificate is valid, 0 otherwise.  If opt.check_cert is 0, the
   function always returns 1, but should still be called because it
   warns the user about any problems with the certificate.  */

bool
ssl_check_certificate (int fd, const char *host)
{
	if(opt.check_cert)
	{
		//warn user that current implementation of SSL cannot
		//verify certificate signatures if --no-check-certificate not specified
		logprintf (LOG_NOTQUIET, _("WARNING: CURRENT SSL IMPLEMENTATION DOES NOT FULLY VALIDATE CERTIFICATES\n"));
	}

	const char *severity = opt.check_cert ? _("ERROR") : _("WARNING");
	int success = 1;
	SSL *ssl = (SSL *) fd_transport_context (fd);
	assert (ssl != NULL);
	
	my_cert_info* cert = global_certificate;
	if(cert == NULL)
	{
		success=0;
		goto no_cert;
	}

	char* cert_common_name = cert->common_name;
	char* cert_common_pattern = malloc(strlen(cert_common_name)+10);
	sprintf(cert_common_pattern, "*.%s", cert_common_name);

	if ( (!pattern_match (cert_common_name, host)) && (!pattern_match (cert_common_pattern, host)) )
	{
		logprintf (LOG_NOTQUIET, _("\
			%s: certificate common name `%s' doesn't match requested host name `%s'.\n"),
			severity, escnonprint (cert_common_name), escnonprint (host));
		success = 0;
	}

	// The matrixSsl signature verification algorithm requires certificates
	// to check against, and completely fails otherwise. Therefore we have to leave
	// the internal signature check out.  The openssl version of wget checks the 
	// signature but allows the certificate to be self-signed.
	//
	// Leaving this internal check out DOES expose us to "man-in-the-middle" attacks,
	// BUT it is important to realize the default version of wget, which
	// uses openssl to valiate the signature, allows the certificate 
	// to be self-signed. This leaves the default version open to the
	// same "man-in-the-middle" attack, only the attacker must generate a 
	// properly self-signed certificate. This is not hard.  Therefore, 
	// disabling this check will only extend this potential vulnerability
	// to hackers who are sufficiently sophisticated to intercept the 
	// data, but too stupid to generate a proper self-signed certificate.
	//
	// The only way to really fix the problem is to be able to store the public
	// keys of the certificate authorities somewhere, AND be able to validate
	// signatures correctly.  Having the second without the first ( as is the case
	// in the gnu openssl version of wget ) really isn't much better than having
	// neither, which is the case here.
	//
	// success = cert->verified == 1 ? 1 : 0;
	



	if (success == 1)
		DEBUGP (("X509 certificate successfully verified and matches host %s\n", 
			escnonprint (host)));
 
no_cert:

 
	if (success != 1 && opt.check_cert)
    		logprintf (LOG_NOTQUIET, _("\
			To connect to %s insecurely, use `--no-check-certificate'.\n"),
			escnonprint (host));
	
	
	/* Allow --no-check-cert to disable certificate checking. */
	//return true;
	return opt.check_cert ? (success > 0 ? true : false) : true;
}
