#if defined(TCSUPPORT_WEBSERVER_SSL)
#include "boa.h"
#define SSL_KEYF "/usr/etc/key.pem"
#define SSL_CERTF "/usr/etc/cert.pem"
#define SSL_FILETYPE_PEM 1

int server_ssl;				/*ssl socket */
sslKeys_t *ssl_key = NULL;
int ssl_server_port = 443;		/*The port that the server should listen on*/
int ssl_pending_requests = 0; 
#ifdef INET6
struct sockaddr_in6 ssl_server_sockaddr;		/* boa ssl socket address */
#else
struct sockaddr_in ssl_server_sockaddr;		/* boa ssl socket address */
#endif
static int sock_opt = 1;

int  boa_sslInit(void)
{
	tcdbg_printf( "\nEnabling SSL security system");
	if ((server_ssl = socket(SERVER_AF, SOCK_STREAM, IPPROTO_TCP)) == -1)
	{
		tcdbg_printf("\nCouldn't create socket for ssl");
		DIE("Couldn't create socket for ssl");
	}
	/* server socket is nonblocking */
	if (fcntl(server_ssl, F_SETFL, NOBLOCK) == -1)
	{
		tcdbg_printf("\nCouldn't fcnt");
		DIE("Couldn't fcntl");
	}
	if ((setsockopt(server_ssl, SOL_SOCKET, SO_REUSEADDR, (void *) &sock_opt,sizeof(sock_opt))) == -1)
	{
		tcdbg_printf("\nCouldn't sockopt"); 
		DIE("Couldn't sockopt");
	}
	/* internet socket */
#ifdef INET6
	ssl_server_sockaddr.sin6_family = AF_INET6;
	memcpy(&ssl_server_sockaddr.sin6_addr,&in6addr_any,sizeof(in6addr_any));
	ssl_server_sockaddr.sin6_port = htons(ssl_server_port);
#else
	ssl_server_sockaddr.sin_family = AF_INET;
	ssl_server_sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	ssl_server_sockaddr.sin_port = htons(ssl_server_port);
#endif
	if (bind(server_ssl, (struct sockaddr *) &ssl_server_sockaddr,sizeof(ssl_server_sockaddr)) == -1)
	{
		tcdbg_printf("\nCouldn't bind ssl to port"); 
		DIE("Couldn't bind ssl to port");
	}

	/* listen: large number just in case your kernel is nicely tweaked */
	if (listen(server_ssl, backlog) == -1)
	{
		tcdbg_printf("\nCouldn't listen"); 	
		DIE("Couldn't listen");
	}

	if(matrixSslOpen()!=0)
	{
		tcdbg_printf("\nCouldn't Init SSL Library!");
		DIE("\nCouldn't Init SSL Library!");
	}
	if(matrixSslReadKeys(&ssl_key,SSL_CERTF,SSL_KEYF,NULL,NULL)!=0)
	{
		tcdbg_printf("\nCouldn't read SSL keys!");
		DIE("\nCouldn't read SSL key!");
	}	
	/*load and check that the key files are appropriate.*/
	tcdbg_printf("\nSSL security system enabled");
	return 1;
}

void  boa_sslUninit()
{
	matrixSslFreeKeys(ssl_key);
	matrixSslClose();
}

void boa_sslWait (int fd,int mode)
{
	fd_set tmp_write_fdset,tmp_read_fdset;
	struct timeval tmp_timeout={1,0};
	int ret=0;

	while (1)
	{
		FD_ZERO(&tmp_read_fdset);
		FD_ZERO(&tmp_write_fdset);
		if(mode == 1)
		{
			FD_SET (fd, &tmp_write_fdset);
			ret=select(fd + 1, NULL, &tmp_write_fdset, NULL, &tmp_timeout);
		}
		else
		{
			FD_SET (fd, &tmp_read_fdset);
			ret=select(fd + 1, &tmp_read_fdset, NULL, NULL, &tmp_timeout);
		}
            	if (ret <0) 
		{
            		if (errno == EINTR)
			{
              		  	continue;   
            		}
        	}
		if(ret==0)
		{
			break;
		}
		if (FD_ISSET(fd, &tmp_read_fdset))
		{
			break;
		}
		if (FD_ISSET(fd, &tmp_write_fdset))
		{
			break;
		}
	}
}

int boa_sslRead(sslConn_t *cp, char *buf, int len, int *status)
{
	int bytes, rc, remaining;
	unsigned char	error, alertLevel, alertDescription, performRead;
	*status = 0;

	if (cp->ssl == NULL || len <= 0) {
		return -1;
	}
	if (cp->inbuf.buf) {
		if (cp->inbuf.start < cp->inbuf.end) {
			remaining = (int)(cp->inbuf.end - cp->inbuf.start);
			bytes = (int)min(len, remaining);
			memcpy(buf, cp->inbuf.start, bytes);
			cp->inbuf.start += bytes;
			return bytes;
		}
	}

	if (cp->insock.buf < cp->insock.start) {
		if (cp->insock.start == cp->insock.end) {
			cp->insock.start = cp->insock.end = cp->insock.buf;
		} else {
			memmove(cp->insock.buf, cp->insock.start, cp->insock.end - cp->insock.start);
			cp->insock.end -= (cp->insock.start - cp->insock.buf);
			cp->insock.start = cp->insock.buf;
		}
	}
	performRead = 0;
readMore:
	if (cp->insock.end == cp->insock.start || performRead) 
	{
		performRead = 1;
		boa_sslWait (cp->fd,0);
		bytes = read(cp->fd, (char *)cp->insock.end, (int)((cp->insock.buf + cp->insock.size) - cp->insock.end));
		if (bytes <0)
		 {
			*status = getSocketError();
			return -1;
		}
		if (bytes == 0)
		 {
			*status = SSLSOCKET_EOF;
			return 0;
		}
		cp->insock.end += bytes;
	}
	cp->inbuf.start = cp->inbuf.end = cp->inbuf.buf;


decodeMore:
	error = 0;
	alertLevel = 0;
	alertDescription = 0;
	rc = matrixSslDecode(cp->ssl, &cp->insock, &cp->inbuf, &error, &alertLevel, &alertDescription);
	switch (rc) {

	case SSL_SUCCESS:
		return 0;

	case SSL_PROCESS_DATA:
		rc = (int)(cp->inbuf.end - cp->inbuf.start);
		rc = min(rc, len);
		memcpy(buf, cp->inbuf.start, rc);
		cp->inbuf.start += rc;
		return rc;

	case SSL_SEND_RESPONSE:
		do
		{
			boa_sslWait(cp->fd,1);
			bytes = write(cp->fd, (char *)cp->inbuf.start, (int)(cp->inbuf.end - cp->inbuf.start));
			if(bytes <=0 )
			{
				*status = getSocketError();
				goto readError;	
			}
			cp->inbuf.start += bytes;

		}while(cp->inbuf.start < cp->inbuf.end);

		cp->inbuf.start = cp->inbuf.end = cp->inbuf.buf;
		return 0;

	case SSL_ERROR:
		if (cp->inbuf.start < cp->inbuf.end) 
		{
			bytes = send(cp->fd, (char *)cp->inbuf.start, (int)(cp->inbuf.end - cp->inbuf.start), 0);
		}
		goto readError;

	case SSL_ALERT:
		if (alertDescription == SSL_ALERT_CLOSE_NOTIFY) {
			*status = SSLSOCKET_CLOSE_NOTIFY;
			goto readZero;
		}
		goto readError;

	case SSL_PARTIAL:
		if (cp->insock.start == cp->insock.buf && cp->insock.end == (cp->insock.buf + cp->insock.size))
		 {
			if (cp->insock.size > SSL_MAX_BUF_SIZE)
			{
				goto readError;
			}
			cp->insock.size *= 2;
			cp->insock.start = cp->insock.buf = (unsigned char *)realloc(cp->insock.buf, cp->insock.size);
			if(cp->insock.buf==NULL)
			{
				goto readError;
			}	
			cp->insock.end = cp->insock.buf + (cp->insock.size / 2);
		}
		performRead = 1;
		goto readMore;

	case SSL_FULL:
		cp->inbuf.size *= 2;
		if (cp->inbuf.buf != (unsigned char*)buf) 
		{
			free(cp->inbuf.buf);
			cp->inbuf.buf = NULL;
		}
		cp->inbuf.start = cp->inbuf.end = cp->inbuf.buf = (unsigned char *)malloc(cp->inbuf.size);
		if(cp->inbuf.buf==NULL)
		{
			goto readError;
		}
		goto decodeMore;
	default:
		goto readError;
	}

readZero:
	if (cp->inbuf.buf == (unsigned char*)buf) {
		cp->inbuf.buf = NULL;
	}
	return 0;
readError:
	if (cp->inbuf.buf == (unsigned char*)buf) {
		cp->inbuf.buf = NULL;
	}
	return -1;
}

//#define BOA_SSL_MAXBUF 8192
int boa_sslAccept(sslConn_t **cpp, SOCKET fd, sslKeys_t *keys,int (*certValidator)(sslCertInfo_t *t, void *arg), int flags)
{
	sslConn_t		*conn;
	unsigned char	buf[1024];
	int		status, rc;

	conn = calloc(sizeof(sslConn_t), 1);
	conn->fd = fd;
	if (matrixSslNewSession(&conn->ssl, keys, NULL,SSL_FLAGS_SERVER | flags) < 0) 
	{
		sslFreeConnection(&conn);
		return -1;
	}
	memset(&conn->inbuf, 0x0, sizeof(sslBuf_t));
	conn->insock.size = 1024;
	conn->insock.start = conn->insock.end = conn->insock.buf = (unsigned char *)malloc(conn->insock.size);
	conn->outsock.size = 1024;
	conn->outsock.start = conn->outsock.end = conn->outsock.buf = (unsigned char *)malloc(conn->outsock.size);
	conn->inbuf.size = 1024;
	conn->inbuf.start = conn->inbuf.end = conn->inbuf.buf = (unsigned char *)malloc(conn->inbuf.size);
	if(conn->insock.buf==NULL ||conn->outsock.buf==NULL || conn->inbuf.buf ==NULL)
	{
		sslFreeConnection(&conn);
		return -1;	
	}
	*cpp = conn;

readMore:
	rc = boa_sslRead(conn, buf, sizeof(buf), &status);
	if (rc == 0)
	 {
		if (status == SSLSOCKET_EOF || status == SSLSOCKET_CLOSE_NOTIFY)
		 {
			sslFreeConnection(&conn);
			return -1;
		}
		if (matrixSslHandshakeIsComplete(conn->ssl) == 0)
		 {
			goto readMore;
		}
	}
	 else if (rc > 0)
	 {
		return -1;
	} 
	else 
	{
		sslFreeConnection(&conn);
		return -1;
	}
	*cpp = conn;
	return 0;
}

int boa_sslWrite(sslConn_t *cp, char *buf, int buflen, int *status)
{

	int len = buflen > 980 ? 980:buflen;
	int	rc;
	*status = 0;

	if (cp->outsock.buf < cp->outsock.start) 
	{
		if (cp->outsock.start == cp->outsock.end)
		{
			cp->outsock.start = cp->outsock.end = cp->outsock.buf;
		} 
		else
		{
			memmove(cp->outsock.buf, cp->outsock.start, cp->outsock.end - cp->outsock.start);
			cp->outsock.end -= (cp->outsock.start - cp->outsock.buf);
			cp->outsock.start = cp->outsock.buf;
		}
	}

	if (cp->outBufferCount > 0 && len != cp->outBufferCount)
	{
		return -1;
	}

	if (cp->outBufferCount == 0) {
retryEncode:
		rc = matrixSslEncode(cp->ssl, (unsigned char *)buf, len, &cp->outsock);
		switch (rc) {
		case SSL_ERROR:
			return -1;
		case SSL_FULL:	
			if (cp->outsock.size > SSL_MAX_BUF_SIZE)
			{
				return -1;
			}
			cp->outsock.size *= 2;
			cp->outsock.buf = (unsigned char *)realloc(cp->outsock.buf, cp->outsock.size);
			if(cp->outsock.buf == NULL)
				return -1;
			cp->outsock.end = cp->outsock.buf + (cp->outsock.end - cp->outsock.start);
			cp->outsock.start = cp->outsock.buf;
			goto retryEncode;
		default:
			break;
		}
	}

	rc = write(cp->fd, (char *)cp->outsock.start, (int)(cp->outsock.end - cp->outsock.start));
	if (rc <= 0) {
		*status = getSocketError();
		return -1;
	}
	cp->outsock.start += rc;

	if (cp->outsock.start == cp->outsock.end) {
		cp->outBufferCount = 0;
		return len;
	}
	cp->outBufferCount = len;
	return 0;
}
#endif
