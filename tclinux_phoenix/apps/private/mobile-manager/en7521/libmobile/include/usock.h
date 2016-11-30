#ifndef USOCK_H_
#define USOCK_H_

#include <sys/socket.h>

#define USOCK_TCP		(SOCK_STREAM | (IPPROTO_TCP << 8))
#define USOCK_UDP		(SOCK_DGRAM  | (IPPROTO_UDP << 8))
#define USOCK_SCTP_STREAM	(SOCK_STREAM | (IPPROTO_SCTP << 8))
#define USOCK_SCTP_SEQPKT	(SOCK_SEQPACKET | (IPPROTO_SCTP << 8))

#define USOCK_IPV6ONLY		(1 << 4)	// IPv6-only socket
#define USOCK_IPV4ONLY		(2 << 4)	// IPv4-only socket
#define USOCK_UNIX		(3 << 4)	// UNIX socket

// flags
#define USOCK_SERVER		0x010000	// server socket
#define USOCK_NOCLOEXEC		0x020000	// don't set close-on-exec
#define USOCK_NONBLOCK		0x040000	// set non-blocking
#define USOCK_TRANSPARENT	0x080000	// set IP transparency (TPROXY)
#define USOCK_NUMERICHOST	0x100000	// host is given as numeric
#define USOCK_LITERALPORT	0x200000	// service is not given as port number
#define USOCK_BINDTODEV		0x400000	// host is a device to bind to

/**
 * Create and configure a new network socket
 *
 * type:	type and flags of the socket (see types and flags above)
 * host:	hostname / network address / interface to connect or bind to
 * service:	service / port to bind to
 *
 * returns a socket fd or -1 on error
 */
int usock(int type, const char *dev, const char *host, const char *service);
int usock_create(int type, const char *host, struct addrinfo *saddr);
int usock_connect(const char *host, struct sockaddr *sa, int sa_len, int family, int type);
int resolvename(int type, const char *host, const char *service, struct addrinfo **presult);
#endif /* USOCK_H_ */
