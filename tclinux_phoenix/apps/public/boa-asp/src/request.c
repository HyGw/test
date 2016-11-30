/*
 *  Boa, an http server
 *  Copyright (C) 1995 Paul Phillips <paulp@go2net.com>
 *  Some changes Copyright (C) 1996,97 Larry Doolittle <ldoolitt@boa.org>
 *  Some changes Copyright (C) 1996-2002 Jon Nelson <jnelson@boa.org>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 1, or (at your option)
 *  any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 */

/* $Id: //BBN_Linux/Branch/Branch_for_Rel_CMCC_7526_20161014/tclinux_phoenix/apps/public/boa-asp/src/request.c#1 $*/

#include "boa.h"
#include <stddef.h> /* for offsetof */
#include <crypt.h>

#if defined(TCSUPPORT_CT)
#ifdef TCSUPPORT_SYSLOG
#include <syslog.h>
#endif

int total_connections;
struct status status;
char cur_username[129];
#if defined(TCSUPPORT_CT_E8GUI)
#define REQ_LONG_TIMEOUT		60
#define REQ_SHORT_TIMEOUT		10
static int REQ_TIMEOUT = REQ_SHORT_TIMEOUT;
#define REQ_USER_TIMEOUT	60*5
#define USR_NUM			3
struct timespec req_time_new,req_time_diff,pre_time_new[USR_NUM],pre_user_time_new[USR_NUM];
//time_t req_time = {0};
//time_t pre_time[USR_NUM] = {0,0,0};
//time_t pre_user_time[USR_NUM] = {0,0,0};
int bReq_TimeOut[USR_NUM] = {0,0,0};
static int iNormal_Flag = 0;
char xmlNode[32]={0};
char xmlValue[32]={0};
#endif


static int sockbufsize = SOCKETBUF_SIZE;

/* function prototypes located in this file only */
static void free_request(request ** list_head_addr, request * req);
static inline struct passwd *getrealpass(const char *user);
#if defined(TCSUPPORT_CT_E8GUI)
void initTimerStruct()
{
	int i;
	memset(&req_time_new,0,sizeof(req_time_new));
	memset(&req_time_new,0,sizeof(req_time_diff));
	for(i = 0; i < USR_NUM; i++)
	{
		memset(&req_time_new,0,sizeof(req_time_new));
		memset(&pre_user_time_new,0,sizeof(pre_user_time_new));
	}
	return;
}
#endif

#if defined(TCSUPPORT_CT_PORTAL_MANAGEMENT)
int is_portal_on(void)
{
	char tmp[4] = {0};
	tcapi_get("Portal_Entry", "Enable", tmp);
	
	if (0 == strcmp(tmp, "1"))
		return 1;
	else
		return 0;
}
#endif
#if defined(TCSUPPORT_CUC_LANDING_PAGE)
int is_pon_state_up(void)
{
	char tmp[64] = {0};

	tcapi_get("XPON_Common", "trafficStatus", tmp);
	
	if (0 == strcmp(tmp, "up"))
		return 1;
	else
		return 0;
}
int is_pon_phystate_up(void)
{
	char tmp[64] = {0};

	tcapi_get("XPON_Common", "phyStatus", tmp);
	
	if (0 == strcmp(tmp, "gpon_phy_up") || 0 == strcmp(tmp, "epon_phy_up"))
		return 1;
	else
		return 0;
}
#endif
#if defined(TCSUPPORT_CT_PHONEAPP)
int is_set_window_popup(void)
{
	char tmp[64] = {0};

	tcapi_get("System_Entry", "wd_enable", tmp);
	
	if (0 == strcmp(tmp, "1"))
		return 1;
	else
		return 0;
}
#endif
#if defined(CT_COM_DEVICEREG)
int checkDevRegisterFlag(void)
{
  char buf[5] = {0};
  int status, result, no_landingpage = 0;

  tcapi_get("deviceAccount_Entry","registerStatus",buf);
  status = atoi(buf);

  memset(buf, 0, sizeof(buf) );
  tcapi_get("deviceAccount_Entry","registerResult",buf);
  result = atoi(buf);

  memset(buf, 0, sizeof(buf) );
  if ( 0 != tcapi_get("deviceAccount_Entry", "isNOLandingPage", buf) )
  	strcpy(buf, "0");
  no_landingpage = atoi(buf);

  if ( 1 == no_landingpage )
  	return 0;

  if(status == 99 || (status == 0 && result == 2) || (status == 0 && result == 99) 
  	|| (status == 1) || (status == 2) || (status == 3) || (status == 4) || (status == 0 && result == 0))
        return 1;
  else
        return 0;
}
#endif

#if defined(TCSUPPORT_CT_SIMCARD_SEPARATION)
int checkSimCardFlag(void)
{
	char buf[5] = {0};
	
	tcapi_get("SimCard_Entry","cpeFlag",buf);
	if(strcmp(buf,"1") == 0){
		memset(buf, 0, sizeof(buf) );
		tcapi_get("SimCard_Entry","cardExist",buf);		
		if(strcmp(buf,"1") != 0){			//sim card is not exist
			return 1;
		}
	}
	
	return 0;
}
#endif

#if defined(TCSUPPORT_CT)
void specialCharacterHandle(char *oriStr)
{
    int i;
    int j = 0;
    char handle[160];
    for(i = 0;i < strlen(oriStr);i++){
    	if(oriStr[i] == '"'){
    		handle[j++]='\\';
    		handle[j++]='"';
    	}else if(oriStr[i] == '\\'){
    		handle[j++]='\\';
    		handle[j++]='\\';
    	}else if(oriStr[i] == '\$'){
    		handle[j++]='\\';
    		handle[j++]='\$';
    	}else if(oriStr[i] == '\`'){
    		handle[j++]='\\';
    		handle[j++]='\`';
    	}
    	else{
    		handle[j] = oriStr[i];
    		j++;
    	}
    }
    handle[j] = '\0';
    strcpy(oriStr,handle);
}
#endif

/*
 * Name: new_request
 * Description: Obtains a request struct off the free list, or if the
 * free list is empty, allocates memory
 *
 * Return value: pointer to initialized request
 */

request *new_request(void)
{
    request *req;

    if (request_free) {
        req = request_free;     /* first on free list */
        dequeue(&request_free, request_free); /* dequeue the head */
    } else {
        req = (request *) malloc(sizeof (request));
        if (!req) {
            log_error_time();
            perror("malloc for new request");
            return NULL;
        }
    }

    memset(req, 0, offsetof(request, buffer) + 1);
		//lee 2006-4-20
		req->cookie = NULL;
		req->authorize = NULL;
#if defined(TCSUPPORT_TRUE_LANDING_PAGE)
		req->hostname = NULL;
		req->accept_language = NULL;
#endif
    return req;
}

/*
 * Name: get_request
 *
 * Description: Polls the server socket for a request.  If one exists,
 * does some basic initialization and adds it to the ready queue;.
 */

void get_request(int server_s)
{
    int fd;                     /* socket */
    struct SOCKADDR remote_addr; /* address */
    struct SOCKADDR salocal;
    int remote_addrlen = sizeof (struct SOCKADDR);
    request *conn;              /* connection */
    size_t len;
    static int system_bufsize = 0; /* Default size of SNDBUF given by system */

		//fprintf(stderr,"get_request1\n");
    remote_addr.S_FAMILY = 0xdead;
    fd = accept(server_s, (struct sockaddr *) &remote_addr,
                &remote_addrlen);
		//fprintf(stderr,"get_request2\n");
    if (fd == -1) {
        if (errno != EAGAIN && errno != EWOULDBLOCK)
            /* abnormal error */
            WARN("accept");
        else
            /* no requests */
            pending_requests = 0;
        return;
    }
    if (fd >= FD_SETSIZE) {
        WARN("Got fd >= FD_SETSIZE.");
        close(fd);
	return;
    }
#ifdef DEBUGNONINET
    /* This shows up due to race conditions in some Linux kernels
       when the client closes the socket sometime between
       the select() and accept() syscalls.
       Code and description by Larry Doolittle <ldoolitt@boa.org>
     */
#define HEX(x) (((x)>9)?(('a'-10)+(x)):('0'+(x)))
    if (remote_addr.sin_family != AF_INET) {
        struct sockaddr *bogus = (struct sockaddr *) &remote_addr;
        char *ap, ablock[44];
        int i;
        close(fd);
        log_error_time();
        for (ap = ablock, i = 0; i < remote_addrlen && i < 14; i++) {
            *ap++ = ' ';
            *ap++ = HEX((bogus->sa_data[i] >> 4) & 0x0f);
            *ap++ = HEX(bogus->sa_data[i] & 0x0f);
        }
        *ap = '\0';
        fprintf(stderr, "non-INET connection attempt: socket %d, "
                "sa_family = %hu, sa_data[%d] = %s\n",
                fd, bogus->sa_family, remote_addrlen, ablock);
        return;
    }
#endif

/* XXX Either delete this, or document why it's needed */
/* Pointed out 3-Oct-1999 by Paul Saab <paul@mu.org> */
#ifdef REUSE_EACH_CLIENT_CONNECTION_SOCKET
    if ((setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (void *) &sock_opt,
                    sizeof (sock_opt))) == -1) {
        DIE("setsockopt: unable to set SO_REUSEADDR");
    }
#endif

    len = sizeof(salocal);

    if (getsockname(fd, (struct sockaddr *) &salocal, &len) != 0) {
        WARN("getsockname");
        close(fd);
        return;
    }
		//fprintf(stderr,"get_request3\n");
    conn = new_request();
    if (!conn) {
    	//fprintf(stderr,"get_request33\n");
        close(fd);
        return;
    }
    conn->fd = fd;
    conn->status = READ_HEADER;
    conn->header_line = conn->client_stream;
    conn->time_last = current_time;
    conn->kacount = ka_max;

    ascii_sockaddr(&salocal, conn->local_ip_addr, NI_MAXHOST);

    /* nonblocking socket */
    if (set_nonblock_fd(conn->fd) == -1)
        WARN("fcntl: unable to set new socket to non-block");

    /* set close on exec to true */
    if (fcntl(conn->fd, F_SETFD, 1) == -1)
        WARN("fctnl: unable to set close-on-exec for new socket");

    /* Increase buffer size if we have to.
     * Only ask the system the buffer size on the first request,
     * and assume all subsequent sockets have the same size.
     */
    if (system_bufsize == 0) {
        len = sizeof (system_bufsize);
        if (getsockopt
            (conn->fd, SOL_SOCKET, SO_SNDBUF, &system_bufsize, &len) == 0
            && len == sizeof (system_bufsize)) {
            /*
               fprintf(stderr, "%sgetsockopt reports SNDBUF %d\n",
               get_commonlog_time(), system_bufsize);
             */
            ;
        } else {
            WARN("getsockopt(SNDBUF)");
            system_bufsize = 1;
        }
    }
    if (system_bufsize < sockbufsize) {
        if (setsockopt
            (conn->fd, SOL_SOCKET, SO_SNDBUF, (void *) &sockbufsize,
             sizeof (sockbufsize)) == -1) {
            WARN("setsockopt: unable to set socket buffer size");
#ifdef DIE_ON_ERROR_TUNING_SNDBUF
            exit(errno);
#endif
        }
    }

    /* for log file and possible use by CGI programs */
    ascii_sockaddr(&remote_addr, conn->remote_ip_addr, NI_MAXHOST);

    /* for possible use by CGI programs */
    conn->remote_port = net_port(&remote_addr);

    status.requests++;

#ifdef USE_TCPNODELAY
    /* Thanks to Jef Poskanzer <jef@acme.com> for this tweak */
    {
        int one = 1;
        if (setsockopt(conn->fd, IPPROTO_TCP, TCP_NODELAY,
                       (void *) &one, sizeof (one)) == -1) {
            DIE("setsockopt: unable to set TCP_NODELAY");
        }

    }
#endif

#ifndef NO_RATE_LIMIT
    if (conn->fd > max_connections) {
        send_r_service_unavailable(conn);
        conn->status = DONE;
        pending_requests = 0;
    }
#endif                          /* NO_RATE_LIMIT */

    total_connections++;
    enqueue(&request_ready, conn);
    //fprintf(stderr,"get_request4\n");
}


/*
 * Name: free_request
 *
 * Description: Deallocates memory for a finished request and closes
 * down socket.
 */


static void free_request(request ** list_head_addr, request * req)
{
    int i;
    /* free_request should *never* get called by anything but
       process_requests */

    if (req->buffer_end && req->status != DEAD) {
        req->status = DONE;
        return;
    }
    /* put request on the free list */
    dequeue(list_head_addr, req); /* dequeue from ready or block list */

    if (req->logline)           /* access log */
        log_access(req);

    if (req->mmap_entry_var)
        release_mmap(req->mmap_entry_var);
    else if (req->data_mem)
        munmap(req->data_mem, req->filesize);

    if (req->data_fd)
        close(req->data_fd);

    if (req->post_data_fd)
        close(req->post_data_fd);

    if (req->response_status >= 400)
        status.errors++;

    for (i = COMMON_CGI_COUNT; i < req->cgi_env_index; ++i) {
        if (req->cgi_env[i]) {
            free(req->cgi_env[i]);
        } else {
            log_error_time();
            fprintf(stderr, "Warning: CGI Environment contains NULL value" \
                    "(index %d of %d).\n", i, req->cgi_env_index);
        }
    }

    if (req->pathname)
        free(req->pathname);
    if (req->path_info)
        free(req->path_info);
    if (req->path_translated)
        free(req->path_translated);
    if (req->script_name)
        free(req->script_name);
	//lee authorize	
		if(req->cookie)
			free(req->cookie);
		if(req->authorize)
			free(req->authorize);
									
    if ((req->keepalive == KA_ACTIVE) &&
        (req->response_status < 500) && req->kacount > 0) {
        int bytes_to_move;

        request *conn = new_request();
        if (!conn) {
            /* errors already reported */
//            enqueue(&request_free, req);
            close(req->fd);
        	//tcdbg_printf("free_request: ptr=%08lx\r\n", (unsigned long)req);
            free(req);
            total_connections--;
            return;
        }
        conn->fd = req->fd;
        conn->status = READ_HEADER;
        conn->header_line = conn->client_stream;
        conn->kacount = req->kacount - 1;

        /* close enough and we avoid a call to time(NULL) */
        conn->time_last = req->time_last;

        /* for log file and possible use by CGI programs */
        memcpy(conn->remote_ip_addr, req->remote_ip_addr, NI_MAXHOST);
        memcpy(conn->local_ip_addr, req->local_ip_addr, NI_MAXHOST);

        /* for possible use by CGI programs */
        conn->remote_port = req->remote_port;

        status.requests++;

        /* we haven't parsed beyond req->parse_pos, so... */
        bytes_to_move = req->client_stream_pos - req->parse_pos;
        if (bytes_to_move) {
            memcpy(conn->client_stream,
                   req->client_stream + req->parse_pos, bytes_to_move);
            conn->client_stream_pos = bytes_to_move;
        }
    
        enqueue(&request_block, conn);
        BOA_FD_SET(conn->fd, &block_read_fdset);

        //enqueue(&request_free, req);
    	//tcdbg_printf("free_request: ptr=%08lx\r\n", (unsigned long)req);
        free(req);
        return;
    }

    /*
     While debugging some weird errors, Jon Nelson learned that
     some versions of Netscape Navigator break the
     HTTP specification.

     Some research on the issue brought up:

     http://www.apache.org/docs/misc/known_client_problems.html

     As quoted here:

     "
     Trailing CRLF on POSTs

     This is a legacy issue. The CERN webserver required POST
     data to have an extra CRLF following it. Thus many
     clients send an extra CRLF that is not included in the
     Content-Length of the request. Apache works around this
     problem by eating any empty lines which appear before a
     request.
     "

     Boa will (for now) hack around this stupid bug in Netscape
     (and Internet Exploder)
     by reading up to 32k after the connection is all but closed.
     This should eliminate any remaining spurious crlf sent
     by the client.

     Building bugs *into* software to be compatable is
     just plain wrong
     */

    if (req->method == M_POST) {
        char *buf;

        buf = (char*)malloc(32768);
        read(req->fd, buf, 32768);
        free(buf);
    }
    close(req->fd);
    total_connections--;

//    enqueue(&request_free, req);
	//tcdbg_printf("free_request: ptr=%08lx\r\n", (unsigned long)req);
    free(req);
    return;
}

/*
 * Name: process_requests
 *
 * Description: Iterates through the ready queue, passing each request
 * to the appropriate handler for processing.  It monitors the
 * return value from handler functions, all of which return -1
 * to indicate a block, 0 on completion and 1 to remain on the
 * ready list for more procesing.
 */


void process_requests(int server_s)
{
    int retval = 0;
    request *current, *trailer;

    if (pending_requests ) {
        get_request(server_s);
#ifdef ORIGINAL_BEHAVIOR
        pending_requests = 0;
#endif
    }	
		
    current = request_ready;

    while (current) {
        time(&current_time);
        if (current->buffer_end && /* there is data in the buffer */
            current->status != DEAD && current->status != DONE) {
            retval = req_flush(current);
            /*
             * retval can be -2=error, -1=blocked, or bytes left
             */
            if (retval == -2) { /* error */
                current->status = DEAD;
                retval = 0;
            } else if (retval >= 0) {
                /* notice the >= which is different from below?
                   Here, we may just be flushing headers.
                   We don't want to return 0 because we are not DONE
                   or DEAD */

                retval = 1;
            }
        } else {
            switch (current->status) {
            case READ_HEADER:
            case ONE_CR:
            case ONE_LF:
            case TWO_CR:
            		
                retval = read_header(current);
                break;
            case BODY_READ:
                retval = read_body(current);
                break;
            case BODY_WRITE:
                retval = write_body(current);
                break;
            case WRITE:
                retval = process_get(current);
                break;
            case PIPE_READ:            		
                retval = read_from_pipe(current);                
                break;
            case PIPE_WRITE:
                retval = write_from_pipe(current);
                break;
            case DONE:
                /* a non-status that will terminate the request */
                retval = req_flush(current);
                /*
                 * retval can be -2=error, -1=blocked, or bytes left
                 */
                if (retval == -2) { /* error */
                    current->status = DEAD;
                    retval = 0;
                } else if (retval > 0) {
                    retval = 1;
                }
                break;
            case DEAD:
                retval = 0;
                current->buffer_end = 0;
                SQUASH_KA(current);
                break;
            default:
                retval = 0;
                fprintf(stderr, "Unknown status (%d), "
                        "closing!\n", current->status);
                current->status = DEAD;
                break;
            }

        }

                
        if (sigterm_flag)
            SQUASH_KA(current);

        /* we put this here instead of after the switch so that
         * if we are on the last request, and get_request is successful,
         * current->next is valid!
         */
        if (pending_requests)
        {
            get_request(server_s);
		}


        switch (retval) {
        case -1:               /* request blocked */
            trailer = current;
            current = current->next;
            block_request(trailer);
         
            break;
        case 0:                /* request complete */
            current->time_last = current_time;
            trailer = current;
            current = current->next;
            free_request(&request_ready, trailer);

            break;
        case 1:                /* more to do */
            current->time_last = current_time;
            current = current->next;
            break;
        default:
            log_error_time();
            fprintf(stderr, "Unknown retval in process.c - "
                    "Status: %d, retval: %d\n", current->status, retval);
            current = current->next;
            break;
        }
    }

}

/*
 * Name: process_logline
 *
 * Description: This is called with the first req->header_line received
 * by a request, called "logline" because it is logged to a file.
 * It is parsed to determine request type and method, then passed to
 * translate_uri for further parsing.  Also sets up CGI environment if
 * needed.
 */

int process_logline(request * req)
{
    char *stop, *stop2;
    static char *SIMPLE_HTTP_VERSION = "HTTP/0.9";
	
    #ifdef BOA_DEBUG
    struct timeval tv;
	gettimeofday(&tv,NULL);
	fprintf(stderr,"process_logline:%u,%u\n",tv.tv_usec,tv.tv_sec);
	#endif
		
    req->logline = req->client_stream;
    if (!memcmp(req->logline, "GET ", 4))
        req->method = M_GET;
    else if (!memcmp(req->logline, "HEAD ", 5))
        /* head is just get w/no body */
        req->method = M_HEAD;
    else if (!memcmp(req->logline, "POST ", 5))
        req->method = M_POST;
    else {
    	#if 0
        log_error_time();
        fprintf(stderr, "malformed request: \"%s\"\n", req->logline);
        #endif
        send_r_not_implemented(req);
        return 0;
    }

    req->http_version = SIMPLE_HTTP_VERSION;
    req->simple = 1;

    /* Guaranteed to find ' ' since we matched a method above */
    stop = req->logline + 3;
    if (*stop != ' ')
        ++stop;

    /* scan to start of non-whitespace */
    while (*(++stop) == ' ');

    stop2 = stop;

    /* scan to end of non-whitespace */
    while (*stop2 != '\0' && *stop2 != ' ')
        ++stop2;

    if (stop2 - stop > MAX_HEADER_LENGTH) {
        log_error_time();
        fprintf(stderr, "URI too long %d: \"%s\"\n", MAX_HEADER_LENGTH,
                req->logline);
        send_r_bad_request(req);
        return 0;
    }
    memcpy(req->request_uri, stop, stop2 - stop);
    req->request_uri[stop2 - stop] = '\0';
    if (*stop2 == ' ') {
        /* if found, we should get an HTTP/x.x */
        unsigned int p1, p2;

        /* scan to end of whitespace */
        ++stop2;
        while (*stop2 == ' ' && *stop2 != '\0')
            ++stop2;

        /* scan in HTTP/major.minor */
        if (sscanf(stop2, "HTTP/%u.%u", &p1, &p2) == 2) {
            /* HTTP/{0.9,1.0,1.1} */
            if (p1 == 1 && (p2 == 0 || p2 == 1)) {
                req->http_version = stop2;
                req->simple = 0;
            } else if (p1 > 1 || (p1 != 0 && p2 > 1)) {
                goto BAD_VERSION;
            }
        } else {
            goto BAD_VERSION;
        }
    }

    if (req->method == M_HEAD && req->simple) {
        send_r_bad_request(req);
        return 0;
    }
    req->cgi_env_index = COMMON_CGI_COUNT;

    return 1;

BAD_VERSION:
    log_error_time();
    fprintf(stderr, "bogus HTTP version: \"%s\"\n", stop2);
    send_r_bad_request(req);
    return 0;
}

//lee 2006-4-11
#define NIL 0
static const char *my_strpbrk (char *cs,char *ct)
{
  char *s;
				/* search for delimiter until end of string */
  for (; *cs; cs++) for (s = ct; *s; s++) if (*s == *cs) return cs;
  return NIL;			/* not found */
}

#define JNK 0177
#define PAD 0100
static void *rfc822_base64 (unsigned char *src,unsigned long srcl,unsigned long *len)
{
  char c,*s,tmp[512];
  void *ret = malloc ((size_t) (*len = 4 + ((srcl * 3) / 4)));
  char *d = (char *) ret;
  int e;
  static char decode[256] = {
   JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,
   JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,
   JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,076,JNK,JNK,JNK,077,
   064,065,066,067,070,071,072,073,074,075,JNK,JNK,JNK,PAD,JNK,JNK,
   JNK,000,001,002,003,004,005,006,007,010,011,012,013,014,015,016,
   017,020,021,022,023,024,025,026,027,030,031,JNK,JNK,JNK,JNK,JNK,
   JNK,032,033,034,035,036,037,040,041,042,043,044,045,046,047,050,
   051,052,053,054,055,056,057,060,061,062,063,JNK,JNK,JNK,JNK,JNK,
   JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,
   JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,
   JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,
   JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,
   JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,
   JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,
   JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,
   JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK
  };
  memset (ret,0,(size_t) *len);	/* initialize block */
  *len = 0;			/* in case we return an error */

				/* simple-minded decode */
  for (e = 0; srcl--; ) switch (c = decode[*src++]) {
  default:			/* valid BASE64 data character */
    switch (e++) {		/* install based on quantum position */
    case 0:
      *d = c << 2;		/* byte 1: high 6 bits */
      break;
    case 1:
      *d++ |= c >> 4;		/* byte 1: low 2 bits */
      *d = c << 4;		/* byte 2: high 4 bits */
      break;
    case 2:
      *d++ |= c >> 2;		/* byte 2: low 4 bits */
      *d = c << 6;		/* byte 3: high 2 bits */
      break;
    case 3:
      *d++ |= c;		/* byte 3: low 6 bits */
      e = 0;			/* reinitialize mechanism */
      break;
    }
    break;
  case JNK:			/* junk character */
    break;
  case PAD:			/* padding */
    switch (e++) {		/* check quantum position */
    case 3:			/* one = is good enough in quantum 3 */
				/* make sure no data characters in remainder */
      for (; srcl; --srcl) switch (decode[*src++]) {
      case JNK: case PAD:	/* ignore junk and extraneous padding */
	break;
      default:			/* valid BASE64 data character */
	/* This indicates bad MIME.  One way that it can be caused is if
	   a single-section message was BASE64 encoded and then something
	   (e.g. a mailing list processor) appended text.  The problem is
	   that in 1 out of 3 cases, there is no padding and hence no way
	   to detect the end of the data.  Consequently, prudent software
	   will always encapsulate a BASE64 segment inside a MULTIPART.
	   */
	sprintf (tmp,"Possible data truncation in rfc822_base64(): %.80s",
		 (char *) src - 1);
	s = my_strpbrk (tmp,"\015\012");
	if (s) *s = NIL;
	
	srcl = 1;		/* don't issue any more messages */
	break;
      }
      break;
    case 2:			/* expect a second = in quantum 2 */
      if (srcl && (*src == '=')) break;
    default:			/* impossible quantum position */
      free (ret);
      return NIL;
    }
    break;
  }
  *len = d - (char *) ret;	/* calculate data length */
  return ret;			/* return the string */
}


typedef struct param_s {
	char	*name;
	char	*value;
} s_param;

static void free_param(s_param **var)
{
	int i;
	#ifdef TRENDCHIP
	if(var){/*var can't be free if it point to 0*/
	#endif
	for (i = 0;var[i];i++)
	{
		if(var[i]->name)
			free(var[i]->name);
		if(var[i]->value)
			free(var[i]->value);
		free(var[i]);
	}
	free(var);
	#ifdef TRENDCHIP
	}
	#endif
}
static s_param **param_line (char *line,char separate1,char separate2)
{
    int numargs;
    char *cp, *ip, *esp, *sptr;
    s_param **result = NULL;
    int i, k, len, end = 0;

    if (line) {
	    if (strlen(line)) {
		for (numargs=1,cp=line; *cp; cp++)
		    if (*cp == separate2) numargs++;
	    } else
		numargs = 0;


	    len = (numargs+1) * sizeof(s_param *);
	    if ((result = (s_param **)malloc (len)) == NULL)
	    {
			return NULL;
			}
	    memset (result, 0, len);

	    cp = line;
	    i=0;
	    while (*cp) {
		if ((ip = (char *)strchr(cp, separate2)) != NULL) {
		    *ip = '\0';
		}else
		{
		    ip = cp + strlen(cp);
			end = 1;
		}

		if ((esp=(char *)strchr(cp, separate1)) == NULL) {
		    cp = ++ip;
		    continue;
		}

		if (!strlen(esp)) {
		    cp = ++ip;
		    continue;
		}

		if (i<numargs) {

		    /* try to find out if there's already such a variable */
		    for (k=0; k<i && (strncmp (result[k]->name,cp, esp-cp) || !(strlen (result[k]->name) == esp-cp)); k++);

		    if (k == i) {	/* No such variable yet */
			if ((result[i] = (s_param *)malloc(sizeof(s_param))) == NULL)
			{
				    return NULL;
			}
			if ((result[i]->name = (char *)malloc((esp-cp+1) * sizeof(char))) == NULL)
			 {
		    return NULL;
		  }
		  
			memset (result[i]->name, 0, esp-cp+1);
			strncpy(result[i]->name, cp, esp-cp);
			cp = ++esp;
			if ((result[i]->value = (char *)malloc((ip-esp+1) * sizeof(char))) == NULL)
			{
			    return NULL;
			 }
			  
			memset (result[i]->value, 0, ip-esp+1);
			strncpy(result[i]->value, cp, ip-esp);
			//FIXME result[i]->value = cgiDecodeString(result[i]->value);
			i++;
		    } else {	/* There is already such a name, suppose a mutiple field */
			cp = ++esp;
			len = (strlen(result[k]->value)+(ip-esp)+2) * sizeof (char);
			if ((sptr = (char *)malloc(len)) == NULL)
			{
			    return NULL;
			  }
			  
			memset (sptr, 0, len);
			sprintf (sptr, "%s\n", result[k]->value);
			strncat(sptr, cp, ip-esp);
			free(result[k]->value);
		//FIXMEresult[k]->value = cgiDecodeString (sptr);
		result[k]->value = sptr;
	    }
	}
	cp = end ? ip : ++ip;
    }
  }

  return result;
}

int check_validate(char *usr,char*pwd,request *req,char *path)
{
	int level = 1;
	char str[256];
	struct passwd *passwdbuf;
	FILE *fd;
	char *pwdencode;
#if defined(TCSUPPORT_CT_PON)
	char isSimCardExist[32] ={0};
	char simAuthRet[32] ={0};
	char webAuthRet[32] ={0};
	char username[32] ={0};
	char cmdpasswd[150] ={0};
	char tmp_psw[100] = {0};
#endif

	passwdbuf = getrealpass(usr);
	if (passwdbuf == NULL)
	{
		#ifdef BOA_DEBUG
		fprintf(stderr,"check_validate err.\n");
		#endif
		return -1;
	}
	else
	{
		pwdencode = crypt(pwd, passwdbuf->pw_passwd);
#if !defined(TCSUPPORT_CUC)
#if defined(TCSUPPORT_CT_PON)
		tcapi_get("SimCard_Entry", "cardExist",isSimCardExist);
		tcapi_get("SimCard_Entry", "simAuthRet",simAuthRet);
		tcapi_get("Account_Entry0","username",username);
		if(!strcmp(usr, username) && !strcmp(isSimCardExist, "1") && !strcmp(simAuthRet, "1"))
		{
			memset(tmp_psw, 0, sizeof(tmp_psw));
			strncpy(tmp_psw, pwd, sizeof(tmp_psw) - 1);
			specialCharacterHandle(tmp_psw);
			sprintf(cmdpasswd,"/userfs/bin/simCard loginAuth telecomadmin \"%s\"", tmp_psw);
			system(cmdpasswd);
			tcapi_get("SimCard_Entry", "webAuthRet",webAuthRet);
			if(!strcmp(webAuthRet, "1"))
			{
				tcapi_set("Account_Entry0","Logoff","0");
				strcpy(pwdencode,passwdbuf->pw_passwd);
			}
			else
			{
				strcpy(pwdencode,"");
			}
		}
#endif
#endif
		if (strcmp(pwdencode, passwdbuf->pw_passwd) == 0) 
  {
  	#ifdef BOA_DEBUG
  fprintf(stderr,"pw:%s\n",pwdencode);	
  #endif
  sprintf(str,"%d",(int)passwdbuf->pw_uid);
  strcpy(req->uid,str);
  sprintf(str,"%d",(int)passwdbuf->pw_gid);
  strcpy(req->gid,str);
  #ifdef BOA_DEBUG
  fprintf(stderr,"u:%s,g:%s\n",req->uid,req->gid);
  #endif

			#ifdef TRENDCHIP
				if(strcmp(path, "Chrome")){
			#endif
  fd = fopen(path,"r+");
	if(fd ==NULL)
	{
		#ifdef BOA_DEBUG
		fprintf(stderr,"check_validate open %s err\n",path);
		#endif
		return -1;
	}
		
	sprintf(str,"usr=%s\npwd=%s\nuid=%s\ngid=%s\nlevel=%d\nip=%s",usr,pwdencode,req->uid,req->gid,level,req->remote_ip_addr);
	fwrite(str,1,strlen(str),fd);
	fclose(fd);
	
			#ifdef TRENDCHIP
				}
			#endif
			
	strcpy(req->usr_name,usr);
	strcpy(req->pwd,pwdencode);
	sprintf(str,"%d",level);
	strcpy(req->access_level,str); 
	#ifdef BOA_DEBUG
	fprintf(stderr,"authorize:%s,%s,%s\n",req->usr_name,req->pwd,req->access_level);
	#endif
#if defined(TCSUPPORT_CT_PON)
	if(!strcmp(usr, username)){
		tcapi_set("Account_Entry0","Logoff","0");
	}
	else{
		tcapi_set("Account_Entry1","Logoff","0");
	}
#endif
  return 1;
		}
		else
		{
		  	#ifdef BOA_DEBUG
  				fprintf(stderr,"check_validate pwd err\n");
  			#endif
  			return -1;
		}
	}
	
}
		
static inline struct passwd *getrealpass(const char *user) 
{	
	struct passwd *pwp;
	pwp = getpwnam(user);
	if (pwp == NULL)
		return NULL;
	return pwp;
}

int check_session(char *path,char*ip,request *req)
{
	FILE *fd;
	char buf[256];
	s_param **sub;

#if defined(TCSUPPORT_CT_E8GUI)
	int i;
	int ret=0;
	char tmp_username[129] = {0};
	struct passwd *passwdbuf;

	for(i = 0 ; i < USR_NUM; i++){
		sprintf(xmlNode, "Account_Entry%d", i);
		ret = tcapi_get(xmlNode, "username", xmlValue);
		if(ret < 0){ //Fail to acquire username from romfile
			fprintf(stderr, "Fail to acquire username from romfile: ret = %d\r\n", ret);
			return -1;
		}
		if(!strcmp(cur_username, xmlValue)){
			tcapi_get(xmlNode, "Logoff", xmlValue);
			if((bReq_TimeOut[i] == 1) || !strcmp(xmlValue,"1")){
				unlink(path);
				if(!strcmp(xmlValue,"1"))
					tcapi_set(xmlNode, "Logoff", "0");
				return -1;
			}
			break;
		}
	}	
#endif
	
	fd = fopen(path,"r+");
	if(fd == NULL)
	{
		#ifdef BOA_DEBUG
		fprintf(stderr,"check_session open %s err\n",path);
		#endif
		goto error;
	}
	
	while(fgets(buf,256,fd))
	{
		char *ret;
		ret = strchr(buf,'\r');
		if(ret)
			ret[0] = '\0';
		ret = strchr(buf,'\n');
		if(ret)
			ret[0] ='\0';
				
		sub = param_line(buf,'=',';');
#if defined(TCSUPPORT_CT_E8GUI)
		if(!sub)
			goto error;
#endif
#if !defined(TCSUPPORT_CT_E8GUI)
		if(!strcmp("usr", sub[0]->name))
			sprintf(cur_username, "%s", sub[0]->value);
#endif
		#ifdef BOA_DEBUG
		fprintf (stderr,"check_session:%s=%s\n", sub[0]->name, sub[0]->value);
		#endif
		if(!strcmp("usr",sub[0]->name))
		{
#if defined(TCSUPPORT_CT_E8GUI)
			sprintf(tmp_username, "%s", sub[0]->value);
#endif
			strcpy(req->usr_name,sub[0]->value);
		}
		if(!strcmp("pwd",sub[0]->name))
		{
#if defined(TCSUPPORT_CT_E8GUI)
			passwdbuf = getrealpass(tmp_username);
			if (passwdbuf == NULL)
			{
				#ifdef BOA_DEBUG
				fprintf(stderr,"check_validate err.\n");
				#endif
				goto error;
			}
			else
			{
				if (strcmp(sub[0]->value, passwdbuf->pw_passwd) != 0) 
				{			
					if(strcmp(req->pathname, "/boaroot/cgi-bin/refresh.asp") == 0)
					{
						req->validsession = 1;
					}
					goto error;
				}
			}
#endif
			strcpy(req->pwd ,sub[0]->value);
		}
		if(!strcmp("level",sub[0]->name))
			strcpy(req->access_level,sub[0]->value);
		if(!strcmp("uid",sub[0]->name))
			strcpy(req->uid,sub[0]->value);
		if(!strcmp("gid",sub[0]->name))
			strcpy(req->gid,sub[0]->value);
			
		free_param(sub);		
	}
	fclose(fd);
#if defined(TCSUPPORT_CT_E8GUI)
	req->validsession = 1;
	sprintf(cur_username, "%s", tmp_username);
#endif
	#ifdef BOA_DEBUG
	fprintf(stderr,"check_session:u:%s,p:%s,l:%s,gid:%s,uid:%s\n",req->usr_name,req->pwd,req->access_level,req->gid,req->uid);
	#endif
	return atoi(req->access_level);

error:
	if(fd)
    	fclose(fd);
    return -1;		
}

#if defined(TCSUPPORT_CT_E8GUI)
int removeSessionExcept(char* path)
{
	DIR *dp=NULL;
	struct dirent *d=NULL;
	char tmppath[64] = {0};
	
	if ((dp = opendir("/var/tmp/")) == NULL){
	#ifdef BOA_DEBUG	
		tcdbg_printf("unable to open /var/tmp/\r\n");
	#endif
		return -1;
	}
	
	while ((d = readdir(dp)) != NULL) {
		if(!(d->d_type & DT_DIR))
		{
			if(!strstr(d->d_name, path+strlen("/var/tmp/")) && strstr(d->d_name, "boasid"))
			{
				sprintf(tmppath,"/var/tmp/%s",d->d_name);
				unlink(tmppath);
			}
		}
	}

	if (closedir(dp) < 0) {
		#ifdef BOA_DEBUG	
		tcdbg_printf("unable to close /var/tmp/");
		#endif
		return -1;
	}
	return 0;
}
#endif

int http_authorize(request *req)
{
	s_param **var,**auth;
  	int i;
  	unsigned long len;
	struct stat buf;
	int ret=0;
	char path[256];
	int level=-1;
	FILE *fd;
#if defined(TCSUPPORT_CT_E8GUI)
	char uid[32]={0};
	char psw[32]={0};
	char authorize[256]={0};
	char tmp_psw[100] = {0};
#endif
#if defined(TCSUPPORT_CT_PON) || defined(TCSUPPORT_CT_E8B_ADSL) || defined(TCSUPPORT_CT_WAN_PTM)
	char isSimCardExist[32] ={0};
	char simAuthRet[32] ={0};
	char webAuthRet[32] ={0};
	char username[32] ={0};
	char webpassword[32] ={0};
	char cmdpasswd[150] ={0};
	char username2[32] ={0};
#endif

#if defined(TCSUPPORT_CT_E8GUI)
	req->validsession = 0;
#endif
	//if cookie is null return
	if(req->cookie ==NULL)
	{
#if !defined(TCSUPPORT_CT_E8GUI)
		#ifdef TRENDCHIP
		/*
			krammer add for bug 1473.
			Chrome will not return the cookie when login procedure,
			so we should not check the cookie or session id when 
			first time to login. When we login successfully, Chrome
			will return the cookie with session id just like other 
			browsers, so the following procedure will be the same
			as other browsers.
		*/
			if(req->header_user_agent){
				if(strstr(req->header_user_agent, "Chrome")){
					/*
						we use "Chrome" to tell check_validate() that it 
						should not write a session file when chrome's
						login.
					*/
					strcpy(path,"Chrome");
					buf.st_size = 0;
					var=0;
					goto chrome;
				}
			}
		#endif
#endif
		#ifdef BOA_DEBUG
		fprintf(stderr,"cookie null\n");
		#endif
		return -1;
	}
	#ifdef BOA_DEBUG 
	fprintf(stderr,"cookie:%s\n",req->cookie);
	#endif
	//get sessionid in cookie
	var = param_line (req->cookie, '=',';');
#if defined(TCSUPPORT_CT_E8GUI)
	for(i = 0;var[i];i++)
	{		
		if(!strcasecmp(var[i]->name,"UID") || !strcasecmp(var[i]->name," UID"))
		{
			strncpy(uid, var[i]->value, sizeof(uid) - 1);
		}
		if(!strcasecmp(var[i]->name," PSW") || !strcasecmp(var[i]->name,"PSW"))
		{
			strncpy(psw, var[i]->value, sizeof(psw) -1);
		}
	}
#if !defined(TCSUPPORT_CUC)
#if defined(TCSUPPORT_CT_PON) || defined(TCSUPPORT_CT_E8B_ADSL) || defined(TCSUPPORT_CT_WAN_PTM)
	tcapi_get("SimCard_Entry", "cardExist",isSimCardExist);
	tcapi_get("SimCard_Entry", "simAuthRet",simAuthRet);
	tcapi_get("Account_Entry0","username",username);
	tcapi_get("Account_Entry0","web_passwd",webpassword);
	tcapi_get("Account_Entry1","username",username2);
	if(uid[0] != 0){
		strcpy(uid,username2);
	}
	if(!strcmp(isSimCardExist, "1") && !strcmp(simAuthRet, "1"))
	{
		memset(tmp_psw, 0, sizeof(tmp_psw));
		strncpy(tmp_psw, psw, sizeof(tmp_psw) - 1);
		specialCharacterHandle(tmp_psw);
		sprintf(cmdpasswd,"/userfs/bin/simCard loginAuth telecomadmin \"%s\"", tmp_psw);
		system(cmdpasswd);
		tcapi_get("SimCard_Entry", "webAuthRet",webAuthRet);
		if(!strcmp(webAuthRet, "1"))
		{
			strcpy(uid,username);
			if ( 0 != strcmp(webpassword, tmp_psw) )
				tcapi_set("Account_Entry0", "web_passwd", tmp_psw);
		}
	}
	else if(!strcmp(webpassword, psw)){
		strcpy(uid,username);
	}
#endif
#endif

#endif
	for(i = 0;var[i];i++)
	{	
		#ifdef BOA_DEBUG
		fprintf (stderr,"cookie param:%s=%s\t index:%d\n", var[i]->name, var[i]->value,i);
		if(var[i+1] != NULL)
			fprintf (stderr,"cookie param:%s=%s\t index:%d\n", var[i+1]->name, var[i+1]->value,i+1);
		#endif
		
#if defined(TCSUPPORT_CT_E8GUI)
		if(!strcasecmp(var[i]->name," SESSIONID") || !strcasecmp(var[i]->name,"SESSIONID"))
#else
		if(!strcmp(var[i]->name,"SESSIONID"))
#endif
		{
			if(var[i+1] == NULL)
			{
			snprintf(path, sizeof(path) - 1, "/var/tmp/%s",var[i]->value);
				ret = stat(path,&buf);
#if defined(TCSUPPORT_CT_E8GUI)
				if((ret)&&(strcmp(req->pathname,"/boaroot/cgi-bin/content.asp") == 0)&&(1 == iNormal_Flag))
				{
					fd = fopen(path,"w");
					fclose(fd);
					ret = stat(path,&buf);
				}
				if(ret)
				{
					#ifdef BOA_DEBUG
					fprintf(stderr,"session file not found,create_1:%s\n",path);
					#endif
					free_param(var);
					return -1;
				}
#else
				if(ret)//can't find session file
				{
					fd = fopen(path,"w");
					fclose(fd);
					#ifdef BOA_DEBUG
					fprintf(stderr,"session file not found,create_0:%s\n",path);
					#endif
					free_param(var);
					return -1;
				}
#endif
			}
			else
			{
				sprintf(path,"/var/tmp/%s",var[i]->value);
			ret = stat(path,&buf);
			if(ret)//can't find session file
			{
#if !defined(TCSUPPORT_CT_E8GUI)
				sprintf(path,"/var/tmp/%s",var[i+1]->value);
			ret = stat(path,&buf);
#endif
#if defined(TCSUPPORT_CT_E8GUI)
				if((ret)&&(strcmp(req->pathname,"/boaroot/cgi-bin/content.asp") == 0)&&(1 == iNormal_Flag))
				{
					fd = fopen(path,"w");
					fclose(fd);
					ret = stat(path,&buf);
				}
				if(ret)
				{
					#ifdef BOA_DEBUG
					fprintf(stderr,"session file not found,create_1:%s\n",path);
					#endif
					free_param(var);
					return -1;
				}
#else
					if(ret)
			{
				fd = fopen(path,"w");
				fclose(fd);
				#ifdef BOA_DEBUG
						fprintf(stderr,"session file not found,create_1:%s\n",path);
				#endif
				free_param(var);
				return -1;
			}
#endif
				}
			}

			#ifdef TRENDCHIP
			chrome:
			#endif
			if (buf.st_size ==0)
			{
				#ifdef BOA_DEBUG
				fprintf(stderr,"buf.st_size = 0\n");
				#endif		
#if !defined(TCSUPPORT_CT_E8GUI)		
				if(req->authorize == NULL)
				{
					#ifdef BOA_DEBUG
					fprintf(stderr,"authorize = NULL and free param now\n");
					#endif
					free_param(var);
					return -1;	
				}
				//check authorize info
				req->authorize = req->authorize+6;
				req->authorize = rfc822_base64(req->authorize,strlen(req->authorize),&len);
				#ifdef BOA_DEBUG
				fprintf(stderr,"base64 decode:%s\n",req->authorize);
				#endif
				auth = param_line (req->authorize, ':', ';');
#endif
#if defined(TCSUPPORT_CT_E8GUI)
				sprintf(authorize,"%s:%s",uid,psw);
				auth = param_line (authorize, ':', ';');
#endif
				sprintf(cur_username, "%s", auth[0]->name);
				#ifdef BOA_DEBUG
				fprintf (stderr,"auth:%s=%s\n", auth[0]->name, auth[0]->value);
				#endif
				level = check_validate(auth[0]->name,auth[0]->value,req,path);
				if (level== -1)
				{
					#ifdef BOA_DEBUG
					fprintf(stderr,"check_validate  %d err\n",level);
					#endif
					free_param(auth);
					free_param(var);
					
				#ifdef TCSUPPORT_SYSLOG
					openlog("TCSysLog WEB", 0, LOG_LOCAL2);
					syslog(LOG_INFO, "WEB login failed!\n");
					closelog();
				#endif
					return -1;
				}
				#ifdef TCSUPPORT_SYSLOG
				else{
					char log[128];
					snprintf(log, sizeof(log), "WEB user <%s> login\n", auth[0]->name);
					openlog("TCSysLog WEB", 0, LOG_LOCAL2);
					syslog(LOG_INFO, log);
					closelog();
				}
				#endif				
				free_param(auth);
			}
			else
			{		
#if defined(TCSUPPORT_CT_E8GUI)
				if(strcmp(uid,"") && strcmp(uid,cur_username))
				{
					FILE *fd = fopen(path,"w");
					if(fd)
						fclose(fd);
					level = check_validate(uid,psw,req,path);
					if (level== -1)
					{
						#ifdef BOA_DEBUG
						fprintf(stderr,"check_validate  %d err\n",level);
						#endif
						free_param(var);
						
					#ifdef TCSUPPORT_SYSLOG
						openlog("TCSysLog WEB", 0, LOG_LOCAL2);
						syslog(LOG_INFO, "WEB login failed!\n");
						closelog();
					#endif
						return -1;
					}
					#ifdef TCSUPPORT_SYSLOG
					else{
						char log[128];
						snprintf(log, sizeof(log), "WEB user <%s> login\n", uid);
						openlog("TCSysLog WEB", 0, LOG_LOCAL2);
						syslog(LOG_INFO, log);
						closelog();
					}
					#endif
					sprintf(cur_username, "%s", uid);
				}
#endif
				level = check_session(path,req->remote_ip_addr,req);				
				if (level== -1)
				{
					#ifdef BOA_DEBUG
					fprintf(stderr,"check_session  %d err\n",level);
					#endif
					free_param(var);
					return -1;
				}
			}
			break;
			
		} 		
		
	}		
	free_param(var);
	
#if defined(TCSUPPORT_CT_E8GUI)
	if(level != -1){
		sprintf(cur_username, "%s", uid);
		for(i = 0 ; i < USR_NUM; i++){
			sprintf(xmlNode, "Account_Entry%d", i);
			ret = tcapi_get(xmlNode, "username", xmlValue);
			if(ret < 0){ //Fail to acquire username from romfile
				fprintf(stderr, "Fail to acquire username from romfile: ret = %d\r\n", ret);
				return -1;
			}
			if(!strcmp(cur_username, xmlValue)){
				//time(&pre_time[i]);
				clock_gettime(CLOCK_MONOTONIC,&pre_time_new[i]);
				if (strcmp(req->pathname,"/boaroot/cgi-bin/refresh.asp") != 0){
					clock_gettime(CLOCK_MONOTONIC,&pre_user_time_new[i]);
					//time(&pre_user_time[i]);
				}
				bReq_TimeOut[i] = 0;
				tcapi_set(xmlNode, "Logged", "1");
				sprintf(xmlValue, "%d", i);
				tcapi_set("WebCurSet_Entry", "CurrentAccess", xmlValue);
				break;
			}
		}
		iNormal_Flag = 0;
		removeSessionExcept(path);
	}
#endif
	return level;
}

#if defined(TCSUPPORT_CT_E8GUI)
#if defined(TCSUPPORT_CT_FW_UPGRADE_16M)
/*
 * Name: uploadDownLoadUrl
 *
 * Description: check if the url is upload page or not
 * Returns 1 for found , 0 for not found
 */
int uploadDownLoadUrl(request * req)
{
        if(0 == strcmp(req->pathname,"/boaroot/cgi-bin/upgrade.asp"))
        {
                return 1;
        }
        
        return 0;
}
void releaseMemory2(request *req)
{
	unsigned int mark;
	int marklen;

	tcapi_set("System_Entry", "upgrade_status", "true");

	marklen = sizeof(mark);
	if (getsockopt(req->fd, SOL_IP, IP_SKB_MARK, &mark, &marklen) == 0)
	{
		mark = (mark & 0xf0000000) >> 28;
	#if 1
		tcdbg_printf("makr = %d\n", mark);
	#endif
		if((SSID1_PORT_MASK == mark)||(SSID2_PORT_MASK == mark)||(SSID3_PORT_MASK == mark)||(SSID4_PORT_MASK == mark))
		{
			tcdbg_printf("\r\nRelease memory: WIFI upgrade!!\r\n");
			system("sh /usr/script/before_web_download.sh 1");
		}
		else
		{
			tcdbg_printf("\r\nRelease memory: Not WIFI upgrade!!\r\n");
			system("sh /usr/script/before_web_download_remove_wifi.sh 1");
		}
	}
	else
	{
		tcdbg_printf("\r\nRelease memory: Unknown upgrade!!\r\n");
		system("/usr/script/before_firmware_update.sh");
	}

	sleep(1);	
	sync();
}
#endif
#endif

#if defined(TCSUPPORT_WEB_INTERCEPTION) 
/*
 * Name: InterceptionIpFromWan
 *
 * Description: check if the ip is turned by wan ,then interception it 
 * Returns 1 for found , 0 for not found
 */
int InterceptionIpFromWan(request * req)
{
	int j=0;
	char *index=NULL;
	char host_name[40] = "";
	char hostUrl[64] = "";
	char adslState[64];
	int upStat=0;
	int fd =0;
	int ret=0;
	char lan_ip4[32] = {0};
#ifdef INET6
	char lan_ip6[64]= {0};
#endif
	memset(hostUrl , 0 , sizeof(hostUrl));
	memset(host_name , 0 , sizeof(host_name));
	memset(xmlNode, 0x00, sizeof(xmlNode));
	strcpy(xmlNode, LAN_NODE);
	ret = tcapi_get(xmlNode, IPV4_ATTR, lan_ip4);
	if(ret < 0)
		strcpy(lan_ip4, DEF_IPV4);
#ifdef INET6
	ret = tcapi_get(xmlNode, IPV6_ATTR, lan_ip6);
	if(ret < 0)
		strcpy(lan_ip6, DEF_IPV6);
#endif
	
		/* find the host ip address */
	for(j=COMMON_CGI_COUNT; req->cgi_env[j]  != NULL; j++)
    {
		if((index = strstr(req->cgi_env[j], "HOST"))  != NULL)
		{
			strncpy(host_name, index+5, sizeof(host_name));
			break;
		}
	}

        /* if the ip is the wan ip(interception by wan) */
	if(NULL == strstr(req->local_ip_addr,host_name))
	{
		fd = open("/proc/tc3162/adsl_stats",O_RDONLY|O_NONBLOCK);
		if( fd <= 0 ){
			return;
		}
		ret = read(fd, adslState, 64);
		close(fd);
		if( ret <= 0 ){
			return;
		}
		if(strstr(adslState, "up"))
		{
			upStat = 1;
        	}
        	else
		{
			upStat = 0;
		}
        if(upStat == 0)
        {
					
#ifdef INET6
			if(strstr(req->local_ip_addr, lan_ip4)==NULL)
				snprintf(hostUrl ,sizeof(hostUrl), "http://[%s]/cgi-bin/AdslDownInfo.asp" , lan_ip6);
			else
				snprintf(hostUrl ,sizeof(hostUrl), "http://%s/cgi-bin/AdslDownInfo.asp" , lan_ip4);
#else
			snprintf(hostUrl ,sizeof(hostUrl), "http://%s/cgi-bin/AdslDownInfo.asp" ,lan_ip4);
#endif
			send_r_moved_temp(req , hostUrl , "");
			return 1;
        	}
	}
        return 0;
}
#endif

#if defined(TCSUPPORT_CT_FJ) || defined(TCSUPPORT_CT_REMOTE_UPG_WEBREDIRECT)
/*
 * Name: IsFirmwareUpgrade
 *
 * Description: check if the CPE's firmware is Upgrading by TR069
 * Returns 1 for upgrading , 0 for not
 */
int IsFirmwareUpgrade()
{
	if( 0 == access("/tmp/cwmpfirmwareupgrade", F_OK) )
		return 1;
	else
   	 	return 0;
}
#endif

#if defined(TCSUPPORT_CT_SIMCARD_SEPARATION)
/*
 * Name: IsSimcardExist
 *
 * Description: check the Simcard
 * Returns 1 for Exist , 0 for not
 */
int IsSimcardExist()
{
   char SimcardFlag[4] = {0};

   tcapi_get("SimCard_Entry","cardExist",SimcardFlag);
    if( 0 == strcmp(SimcardFlag, "0") )
    {
	return 0;
    }
    else
    {
	return 1;
    }
}
#endif

#if defined(TCSUPPORT_CT_JOYME)
/* check osgi push view upgrade
* 1:Yes, 0: No
*/
int isOSGIPushUPG()
{
	char pusview[12] = {0};

	bzero(pusview, sizeof(pusview));
	if ( 0 == tcapi_get("OSGIUpgrade_Entry"
			, "pushview", pusview)
		&& 0 == strcmp(pusview, "Yes") )
		return 1;

	return 0;
}

#endif


/*
 * Name: process_header_end
 *
 * Description: takes a request and performs some final checking before
 * init_cgi or init_get
 * Returns 0 for error or NPH, or 1 for success
 */

int process_header_end(request * req)
{

#ifdef TRENDCHIP
#ifdef PURE_BRIDGE
	int i ;
	char string[32];
#else
	int ret, i, j, break_flag = 0, maskBytes[9];
	char nodename[15], username[129], maskBuf[30];
	char bitMask[8] = {(1<<0), (1<<1), (1<<2), (1<<3), (1<<4), (1<<5), (1<<6), (1<<7)};
	struct pageset *pagesetNow, *pagesetNext;
	extern char cur_username[129];
	extern struct pageset* pageMap[8][9];
#endif
#if defined(TCSUPPORT_CT_PORTAL_MANAGEMENT) || defined(CT_COM_DEVICEREG) || defined(TCSUPPORT_CT_JOYME)
	char *index = NULL;
	char host_name[40]={0};
	char hostUrl[128];
	char url_str[128] = {0};
	char str[160] = {0};
	uint32_t yiaddr;
	uint8_t matchid;
	int flag = 0;
	FILE *fp1 = NULL;
#endif

#if defined(CT_COM_DEVICEREG) || defined(TCSUPPORT_CT_JOYME)
	char tempPathName[128] = {0};
	int openFileStatus1 = 0;
	int openFileStatus2 = 0;
#endif
#ifdef WITHVOIP
	FILE *fp;
#endif
	int adslchanging = 0;
#endif
#if defined(TCSUPPORT_CUC_LANDING_PAGE)
	char lanip[32] = {0};
#endif
#if defined(TCSUPPORT_CT_FJ) || defined(TCSUPPORT_CT_SIMCARD_SEPARATION) || defined(TCSUPPORT_CT_REMOTE_UPG_WEBREDIRECT) || defined(TCSUPPORT_CT_PHONEAPP) || defined(TCSUPPORT_CT_JOYME)
	char redirectUrl[128] = {0};
#endif
    if (!req->logline) {
        send_r_error(req);
        return 0;
    }

	/* Percent-decode request */
    if (unescape_uri(req->request_uri, &(req->query_string)) == 0) {
        log_error_doc(req);
        fputs("Problem unescaping uri\n", stderr);
        send_r_bad_request(req);
        return 0;
    }

    /* clean pathname */
    clean_pathname(req->request_uri);

    if (req->request_uri[0] != '/') {
        send_r_bad_request(req);
        return 0;
    }
#if defined(TCSUPPORT_CT_SIMCARD_SEPARATION)
	req->isSimCard_redirect = 0;
#endif
#if defined(CT_COM_DEVICEREG)
	req->iswan_redirect = 0;
	if( ( M_GET == req->method )
#if defined(TCSUPPORT_CT_FJ) || defined(TCSUPPORT_CT_REMOTE_UPG_WEBREDIRECT)
		&& ( 0 == IsFirmwareUpgrade())
#endif
                && checkDevRegisterFlag()
#if defined(TCSUPPORT_CUC_LANDING_PAGE)
		&& ( is_pon_state_up() )
#endif
	)
	{
	
		for(i=COMMON_CGI_COUNT; req->cgi_env[i] != NULL; i++)
		{
			if((index = strstr(req->cgi_env[i], "HOST")) != NULL)
			{
				strncpy(host_name, index+5, 39);
				break;
			}
		}	
#if defined(TCSUPPORT_CT_SIMCARD_SEPARATION)
		if(strstr(host_name,req->local_ip_addr) == NULL && checkSimCardFlag()){
			req->isSimCard_redirect = 1;
		}
#endif
								
		if (strstr(host_name,req->local_ip_addr) == NULL &&
			strncmp(req->local_ip_addr, host_name,8) != 0 && 
			strcmp(host_name, "220.250.64.25") != 0 &&		   /*some machine will send http message automatic*/
			NULL != req->header_user_agent)
		{		
			if(strcmp(req->request_uri, "/") != 0)
			{
				memset(tempPathName, 0, sizeof(tempPathName));
				//tcdbg_printf("req->request_uri = %s\r\n", req->request_uri);
				sprintf(tempPathName, "/boaroot%s", req->request_uri);
				openFileStatus1 = open(tempPathName, O_RDONLY);
				//tcdbg_printf("tempPathName1 = %s\r\n", tempPathName);
				//tcdbg_printf("openFileStatus1 = %d\r\n", openFileStatus1);					
				sprintf(tempPathName, "/boaroot/html%s", req->request_uri);
				openFileStatus2 = open(tempPathName, O_RDONLY);
				//tcdbg_printf("tempPathName2 = %s\r\n", tempPathName);
				//tcdbg_printf("openFileStatus2 = %d\r\n", openFileStatus2);			

				if(openFileStatus1 == -1 && openFileStatus2 == -1 && errno == ENOENT )
				{
					sprintf(req->request_uri, "/");
				}
			}
			req->iswan_redirect = 1;
		}
	}	
#endif

#if defined(TCSUPPORT_CT_JOYME)
	req->ispushview_upg = 0;
	if( M_GET == req->method
		&& (!checkDevRegisterFlag())
		&& 1 == isOSGIPushUPG() )
	{
		for(i=COMMON_CGI_COUNT; req->cgi_env[i] != NULL; i++)
		{
			if((index = strstr(req->cgi_env[i], "HOST")) != NULL)
			{
				strncpy(host_name, index+5, 39);
				break;
			}
		}

		if (strstr(host_name,req->local_ip_addr) == NULL &&
			strncmp(req->local_ip_addr, host_name,8) != 0 && 
			strcmp(host_name, "220.250.64.25") != 0 &&
			NULL != req->header_user_agent)
		{
			if(strcmp(req->request_uri, "/") != 0)
			{
				memset(tempPathName, 0, sizeof(tempPathName));
				sprintf(tempPathName, "/boaroot%s", req->request_uri);
				openFileStatus1 = open(tempPathName, O_RDONLY); 			
				sprintf(tempPathName, "/boaroot/html%s", req->request_uri);
				openFileStatus2 = open(tempPathName, O_RDONLY);
				if ( openFileStatus1 == -1 && openFileStatus2 == -1 )
				{
					sprintf(req->request_uri, "/");
				}
			}
			req->ispushview_upg = 1;
		}
	}
#endif


    if (translate_uri(req) == 0) { /* unescape, parse uri */
        SQUASH_KA(req);
        return 0;               /* failure, close down */
    }

#if defined(TCSUPPORT_CT_FJ) || defined(TCSUPPORT_CT_REMOTE_UPG_WEBREDIRECT)
    if( M_GET == req->method )
    {
        if (0 == strcmp(req->request_uri,"/cgi-bin/UpdateMsg.cgi")
#if defined(TCSUPPORT_CT_PON_GD) || defined(TCSUPPORT_CT_PON_CY_JX) || defined(TCSUPPORT_CT_FJ)
                || (0 == strcmp(req->request_uri,"/cgi-bin/RemoteUPGMsg.cgi"))
#endif
           )
        {
            return init_cgi(req);
        }
        else if (1 == IsFirmwareUpgrade()
                 /* RemoteUPGMsg.cgi ok.gif */
#if defined(TCSUPPORT_CT_PON_GD) || defined(TCSUPPORT_CT_PON_CY_JX) || defined(TCSUPPORT_CT_FJ)
                     && (0 != strcmp(req->request_uri,"/img/ok.gif"))
#endif
                )
        {
#if defined(TCSUPPORT_CT_PON_GD) || defined(TCSUPPORT_CT_PON_CY_JX) || defined(TCSUPPORT_CT_FJ)
                snprintf(redirectUrl, sizeof(redirectUrl), "http://%s/cgi-bin/RemoteUPGMsg.cgi", req->local_ip_addr);
            #else
                snprintf(redirectUrl, sizeof(redirectUrl), "http://%s/cgi-bin/UpdateMsg.cgi", req->local_ip_addr);
#endif
            send_r_moved_temp(req , redirectUrl , "");
            return 0;
        }
    }
#endif

#if defined(TCSUPPORT_CT_PORTAL_MANAGEMENT)
    if( is_portal_on() 
#if defined(CT_COM_DEVICEREG)
		&& (!checkDevRegisterFlag())
#endif
#if defined(TCSUPPORT_CUC_LANDING_PAGE)
		|| ( !is_pon_state_up() )
#endif
		&& req->method == M_GET)
    {

	for(i=COMMON_CGI_COUNT; req->cgi_env[i] != NULL; i++)
	{
		if((index = strstr(req->cgi_env[i], "HOST")) != NULL)
		{
			strncpy(host_name, index+5, 39);
			break;
		}
	}	

	//tcdbg_printf("host_name = %s, req->local_ip_addr=%s\n", host_name, req->local_ip_addr);
	if (strstr(host_name,req->local_ip_addr) == NULL &&
		strncmp(req->local_ip_addr, host_name,8) != 0 && 
	strcmp(host_name, "220.250.64.25") != 0 &&         /*some machine will send http message automatic*/
	NULL != req->header_user_agent &&
       (strstr(req->header_user_agent, "Mozilla") != NULL || 
	strstr(req->header_user_agent, "Msie") != NULL || 
	strstr(req->header_user_agent, "Gecko") != NULL ||
	strstr(req->header_user_agent, "Opera") != NULL ||
	strstr(req->header_user_agent, "Safari") != NULL ||
	strstr(req->header_user_agent, "Konqueror") != NULL ||
	strstr(req->header_user_agent, "Netscape") != NULL ||
	strstr(req->header_user_agent, "Chrome") != NULL ||
	strstr(req->header_user_agent, "IE") != NULL ||
	strstr(req->header_user_agent, "Firefox") != NULL))
	{		
#if defined(TCSUPPORT_CUC_LANDING_PAGE)
		if ( !is_pon_state_up() )
		{
			memset(nodename, 0x00, sizeof(nodename));
			strcpy(nodename, "Lan_Entry0");
			if ( tcapi_get(nodename, "IP", lanip) < 0 )
				strcpy(lanip, "192.168.1.1");
			memset(hostUrl, 0, sizeof(hostUrl));
#if defined(CT_COM_DEVICEREG)
			if(checkDevRegisterFlag())
				sprintf(hostUrl, "http://%s/cgi-bin/register.asp", lanip);
			else
#endif
			sprintf(hostUrl, "http://%s/cgi-bin/pondown.asp", lanip);
			send_r_moved_temp(req, hostUrl, "");
		}
		else
#endif

		{
		memset(str, 0, sizeof(str));	
		snprintf(str, sizeof(str),"iptables -t nat -D PRE_PORTAL -s %s -j RETURN", req->remote_ip_addr);	
		system(str);
		
		memset(str, 0, sizeof(str));	
		snprintf(str, sizeof(str),"iptables -t nat -I PRE_PORTAL 1 -s %s -j RETURN", req->remote_ip_addr);	
		system(str);
				
		fp1 = fopen("/tmp/var/tmp/ip_vendor.conf", "r");
		if (fp1 != NULL)
		{
			while (fread(&yiaddr, sizeof(uint32_t), 1, fp1) == 1)
			{	
				fread(&matchid, sizeof(uint8_t), 1, fp1);
				if (inet_addr(req->remote_ip_addr) == yiaddr)
				{	
					tcdbg_printf("find a same addr matchid = %d\n", matchid);
					switch(matchid)
					{
						/* vendor class is PC */
						case 1:
							ret = tcapi_get("Portal_Entry", "PortalUrl-Computer", url_str);
							tcdbg_printf("Fail to get current PortalUrl-Computer: ret = %d\r\n", ret);
							if (ret < 0)
							{
								tcdbg_printf("Fail to get current PortalUrl-Computer: ret = %d\r\n", ret);
								fclose(fp1);
								return -1;
							}
							break;
						/* vendor class is STB */
						case 2:
							ret = tcapi_get("Portal_Entry", "PortalUrl-STB", url_str);
							
							if (ret < 0)
							{
								tcdbg_printf("Fail to get current PortalUrl-STB: ret = %d\r\n", ret);
								fclose(fp1);
								return -1;
							}
							break;
						/* vendor class is Phone */
						case 3:
							ret = tcapi_get("Portal_Entry", "PortalUrl-Phone", url_str);
							
							if (ret < 0)
							{
								tcdbg_printf("Fail to get current PortalUrl-Phone: ret = %d\r\n", ret);
								fclose(fp1);
								return -1;
							}
							break;
						/* default is PC */
						default:
						
							ret = tcapi_get("Portal_Entry", "PortalUrl-Computer", url_str);
							tcdbg_printf("Fail to get current PortalUrl-Computer 0: ret = %d\r\n", ret);
							if (ret < 0)
							{
								tcdbg_printf("Fail to get current PortalUrl-Computer: ret = %d\r\n", ret);
								fclose(fp1);
								return -1;
							}
							break;
					}
					flag = 1;
					break;
				}
			}
			fclose(fp1);
		}
		if (flag == 0)
		{
			FILE *fp1 = fopen("/tmp/var/tmp/ip_vendor.conf", "a+");
			if (fp1 == NULL)
			{
				tcdbg_printf("file open fp1 failed \n");
				return 0;
			}
			yiaddr = inet_addr(req->remote_ip_addr);
			matchid = 0;
			fwrite(&yiaddr, sizeof(uint32_t), 1, fp1);
			fwrite(&matchid, sizeof(uint8_t), 1, fp1);
			fclose(fp1);
				
			ret = tcapi_get("Portal_Entry", "PortalUrl-Computer", url_str);
			if (ret < 0)
			{
				tcdbg_printf("Fail to get current PortalUrl-Computer: ret = %d\r\n", ret);
				return -1;
			}
		}
		memset(hostUrl, 0, sizeof(hostUrl));
		if (strstr(url_str, "http://") == NULL)
			sprintf(hostUrl, "http://%s", url_str);
		else
			sprintf(hostUrl, "%s", url_str);
			
		send_r_moved_temp(req, hostUrl, "");
		}
		
		return 0;
		}
	}
#endif
		//lee 2006-4-11:authorize
		//if(req->is_cgi)
      
	if(req->method == M_GET)
	{
		#ifdef TCSUPPORT_SYSLOG
		if (strcmp(req->pathname,"/boaroot/cgi-bin/index2.asp") == 0){
			tcapi_set("Account_Entry0", "LoginIp", req->remote_ip_addr);
		}
		#endif
#if defined(TCSUPPORT_WEB_INTERCEPTION)
        /* interception the url get request */
        if(1 == InterceptionIpFromWan(req))
        {
				return 0;
        }
#endif
#if defined(TCSUPPORT_CT_E8GUI)
		char *h_index = NULL;
		char h_name[40]={0};
		char lan_ip4[32] = {0};
		char lan_ip6[64] = {0};
		char h_temp[64] = {0};
		int  isIPv4_Flag = 1;
#if defined(TCSUPPORT_C1_CUC)
		char str_admin[4] = {0};
#endif

		if (strcmp(req->pathname,"/boaroot/cgi-bin/refresh.asp")){
				for(i=COMMON_CGI_COUNT; req->cgi_env[i] != NULL; i++)
				{
					if((h_index = strstr(req->cgi_env[i], "HOST")) != NULL)
					{
						strncpy(h_name, h_index+5, 39);
						//tcdbg_printf("host_name = %s, req->local_ip_addr=%s\n", h_name, req->local_ip_addr);
						//check host name
						memset(xmlNode, 0x00, sizeof(xmlNode));
						strcpy(xmlNode, "Lan_Entry0");
						ret = tcapi_get(xmlNode, "IP", lan_ip4);
						if(ret < 0) strcpy(lan_ip4, "192.168.1.1");
						ret = tcapi_get(xmlNode, "IP6", lan_ip6);
						if(ret < 0) strcpy(lan_ip6, "fe80::1");
						memset(xmlNode, 0x00, sizeof(xmlNode));
						strcpy(xmlNode, "WebCurSet_Entry");
						if(strstr(req->local_ip_addr, lan_ip4) != NULL){
							tcapi_set(xmlNode, "IsIPv4", "1");
							isIPv4_Flag = 1;
						}
						else {
							tcapi_set(xmlNode, "IsIPv4", "0");
							isIPv4_Flag = 0;
						}
						if((strstr(h_name, lan_ip4) != NULL) || (strstr(h_name, lan_ip6) != NULL)){
							tcapi_set(xmlNode, "InnerIP", "1");
						}	
						else{
							tcapi_set(xmlNode, "InnerIP", "0");

#if defined(TCSUPPORT_CT_PHONEAPP)
						if ( is_set_window_popup() )
						{
							snprintf(redirectUrl, sizeof(redirectUrl), "http://%s/cgi-bin/redirect.cgi", req->local_ip_addr);
							send_r_moved_temp(req , redirectUrl , "");
	//							tcapi_set("System_Entry", "wd_enable", "0");
							tcapi_set("System_Entry","wd_Stat", "1");//popup ok
							tcapi_set("System_Entry", "wd_hostname", h_name);
	//							system("iptables -t nat -F PRE_PHONEAPP");
	//							system("iptables -t nat -Z PRE_PHONEAPP");
	//							system("iptables -t nat -A PRE_PHONEAPP -j RETURN");
							return 0;
						}
#endif
						}
						break;
					}
				}
		}

		if ( 0 == strcmp(req->pathname, "/boaroot/cgi-bin/net-wanset.asp")
			|| 0 == strcmp(req->pathname, "/boaroot/cgi-bin/refresh.asp")
			|| ( NULL == strstr(req->pathname,".asp") && NULL == strstr(req->pathname,".ASP")) )
			adslchanging = 1; // wan/refresh page never timeout

		memset(xmlValue, 0x00, sizeof(xmlValue));
		tcapi_get("WebCurSet_Entry", "StatusChanging", xmlValue);
		if(!strcmp(xmlValue,"1")){
			adslchanging = 1;
		}
		memset(xmlValue, 0x00, sizeof(xmlValue));

		for(i = 0 ; i < USR_NUM; i++){
			//if(pre_time[i] == 0)
				//time(&pre_time[i]);
			if(pre_time_new[i].tv_sec == 0 || adslchanging == 1)
				clock_gettime(CLOCK_MONOTONIC,&pre_time_new[i]);
			//if(pre_user_time[i] == 0)
				//time(&pre_user_time[i]);
			if(pre_user_time_new[i].tv_sec == 0)	
				clock_gettime(CLOCK_MONOTONIC,&pre_user_time_new[i]);
			//time(&req_time);
			req_time_diff.tv_nsec = 0;
			clock_gettime(CLOCK_MONOTONIC,&req_time_new);
			//if(req_time - pre_time[i] > REQ_TIMEOUT)
			req_time_diff.tv_sec = req_time_new.tv_sec - pre_time_new[i].tv_sec;
			req_time_diff.tv_nsec += req_time_new.tv_nsec - pre_time_new[i].tv_nsec;
			while(req_time_diff.tv_nsec > 1000000000)
			{
				req_time_diff.tv_sec++;
				req_time_diff.tv_nsec -= 1000000000;
			}
			if(req_time_diff.tv_nsec < 0)
			{
				req_time_diff.tv_sec--;
				req_time_diff.tv_nsec += 1000000000;
			}
			if(req_time_diff.tv_sec > REQ_TIMEOUT)
			{
				if(adslchanging == 0)
				{
					sprintf(xmlNode, "Account_Entry%d", i);
					ret = tcapi_get(xmlNode, "Logged", xmlValue);
					if(ret < 0)
						strcpy(xmlValue, "0");
					if(!strcmp(xmlValue,"1")){
						tcapi_set(xmlNode, "Logged", "0");
						bReq_TimeOut[i] = 1;
					}
				}
			}
			//if(req_time - pre_user_time[i] > REQ_USER_TIMEOUT)
			req_time_diff.tv_sec = req_time_new.tv_sec - pre_user_time_new[i].tv_sec;
			req_time_diff.tv_nsec += req_time_new.tv_nsec - pre_user_time_new[i].tv_nsec;
			while(req_time_diff.tv_nsec > 1000000000)
			{
				req_time_diff.tv_sec++;
				req_time_diff.tv_nsec -= 1000000000;
			}
			if(req_time_diff.tv_nsec < 0)
			{
				req_time_diff.tv_sec--;
				req_time_diff.tv_nsec += 1000000000;
			}
			if(req_time_diff.tv_sec > REQ_USER_TIMEOUT)
			{
				sprintf(xmlNode, "Account_Entry%d", i);
				ret = tcapi_get(xmlNode, "Logged", xmlValue);
				if(ret < 0)
					strcpy(xmlValue, "0");
				if(!strcmp(xmlValue,"1")){
					tcapi_set(xmlNode, "Logged", "0");
					bReq_TimeOut[i] = 1;
				}
			}			
		}
		if (!strcmp(req->pathname,"/boaroot/html/telnet.asp")) {
			if (0 == isIPv4_Flag)			
				sprintf(h_temp, "http://[%s]/cgi-bin/telnet.asp", lan_ip6);
			else
				sprintf(h_temp, "http://%s/cgi-bin/telnet.asp", lan_ip4);
			send_r_moved_temp(req , h_temp, "");
			return 0;
		}
		//special trans the upgrade.asp
		if (!strcmp(req->pathname,"/boaroot/html/upgrade.asp")) {
			if (0 == isIPv4_Flag)			
				sprintf(h_temp, "http://[%s]/cgi-bin/upgrade.asp", lan_ip6);
			else
				sprintf(h_temp, "http://%s/cgi-bin/upgrade.asp", lan_ip4);
			send_r_moved_temp(req , h_temp, "");
			return 0;
		}
		if (!strcmp(req->pathname,"/boaroot/html/hwnat.asp")) {
			if (0 == isIPv4_Flag)			
				sprintf(h_temp, "http://[%s]/cgi-bin/hwnat.asp", lan_ip6);
			else
				sprintf(h_temp, "http://%s/cgi-bin/hwnat.asp", lan_ip4);
			send_r_moved_temp(req , h_temp, "");
			return 0;
		}
#if defined(TCSUPPORT_C1_CUC)
		memset(xmlNode, 0x00, sizeof(xmlNode));
		strcpy(xmlNode, "Sys_Entry");
		if (!strcmp(req->pathname,"/boaroot/html/cu.html")) {
			tcapi_get(xmlNode, "IsAdmin", str_admin);
			if (strcmp("1", str_admin)){
				tcapi_set(xmlNode, "IsAdmin", "1");
				tcapi_save();
			}
			if (0 == isIPv4_Flag)			
				sprintf(h_temp, "http://[%s]/cgi-bin/index3.asp", lan_ip6);
			else
				sprintf(h_temp, "http://%s/cgi-bin/index3.asp", lan_ip4);
			send_r_moved_temp(req , h_temp, "");
			return 0;
		}
		else if(!strcmp(req->pathname,"/boaroot/cgi-bin/index.asp")) {
			tcapi_get(xmlNode, "IsAdmin", str_admin);
			if (strcmp("0", str_admin)){
				tcapi_set(xmlNode, "IsAdmin", "0");
				tcapi_save();
			}
		}
#endif
		REQ_TIMEOUT = REQ_SHORT_TIMEOUT;
		if ((strstr(req->pathname,".asp")||strstr(req->pathname,".ASP")||(0 == strcmp(req->pathname,"/boaroot/html/romfile.cfg")))
		&& (strcmp(req->pathname,"/boaroot/cgi-bin/index2.asp") != 0)
		&& (strcmp(req->pathname,"/boaroot/cgi-bin/register.asp") != 0)
		&& (strcmp(req->pathname,"/boaroot/cgi-bin/regstatus.asp") != 0)
#if defined(TCSUPPORT_CT_PON_SC)
		&& (strcmp(req->pathname,"/boaroot/cgi-bin/selfcheck.asp") != 0)
		&& (strcmp(req->pathname,"/boaroot/cgi-bin/reset.asp") != 0)
#endif
#if defined(TCSUPPORT_CT_PON_C9)
		&& (strcmp(req->pathname, "/boaroot/cgi-bin/cwmpsetting.asp") != 0)
#endif
#if defined(TCSUPPORT_WEB_INTERCEPTION)
		&& (strcmp(req->pathname, "/boaroot/cgi-bin/AdslDownInfo.asp") != 0)//don't need to jump to authorize page
#endif
#if defined(TCSUPPORT_CUC_LANDING_PAGE)
		&& (strcmp(req->pathname,"/boaroot/cgi-bin/pondown.asp") != 0)
#endif
#if defined(TCSUPPORT_CT_ADSL_HN) || defined(TCSUPPORT_CT_PON_CZ_NX)
		&& (strcmp(req->pathname,"/boaroot/cgi-bin/routesetting.asp") != 0)
#endif
#if defined(TCSUPPORT_CT_PON_SK)
		&& (strcmp(req->pathname,"/boaroot/cgi-bin/test_version.asp") != 0)
		&& (strcmp(req->pathname,"/boaroot/cgi-bin/test_info.asp") != 0)
#endif
#if defined(TCSUPPORT_CT_PON_CQ)
		&& (strcmp(req->pathname,"/boaroot/cgi-bin/cqregister.asp") != 0)
		&& (strcmp(req->pathname,"/boaroot/cgi-bin/cqreset.asp") != 0)
#endif
#if defined(TCSUPPORT_CT_FJ)
		&& (strcmp(req->pathname,"/boaroot/cgi-bin/resetscreen.asp") != 0)
#endif
#if defined(TCSUPPORT_CT_JOYME)
		&& (strcmp(req->pathname,"/boaroot/cgi-bin/pushviewupgrade.asp") != 0)
		&& (strcmp(req->pathname,"/boaroot/cgi-bin/pushviewfinish.asp") != 0)
#endif
#if defined(TCSUPPORT_CT_PON)
		&& (strcmp(req->pathname,"/boaroot/cgi-bin/avalanch.asp") != 0)
#endif
		&& (http_authorize(req) == -1))
#else
		if (http_authorize(req) == -1)
#endif
		{
			#ifdef BOA_DEBUG
			fprintf(stderr,"send_r_unauthorized\n");
			#endif
#if defined(TCSUPPORT_CT_E8GUI)
			send_r_web_unauthorized(req);
#else
			send_r_unauthorized(req,"Member Login");
#endif
			return 0;
		}
#if defined(TCSUPPORT_CT_JOYME)
		else
		{
			if(!strcmp(req->pathname,"/boaroot/cgi-bin/upgrade.asp")||!strcmp(req->pathname,"/boaroot/cgi-bin/telnet.asp"))
			{
				memset(xmlValue, 0x00, sizeof(xmlValue));
				tcapi_get("Account_Entry0", "username", xmlValue);
				if(strcmp(req->usr_name,xmlValue))
				{				
					send_r_web_unauthorized(req);
					return 0;
				}
			}
		}
#endif
	}
	
#ifndef PURE_BRIDGE
	for(i = 0 ; i < 3; i++){
		sprintf(nodename, "Account_Entry%d", i);
		ret = tcapi_get(nodename, "username", username);
		if(ret < 0){ //Fail to acquire username from romfile
			fprintf(stderr, "Fail to acquire username from romfile: ret = %d\r\n", ret);
			return -1;
		}
		if(!strcmp(cur_username, username)){
			ret = tcapi_get(nodename, "display_mask", maskBuf);
			if(ret < 0){ //Fail to set current username
				fprintf(stderr, "Fail to get current username's dispaly mask: ret = %d\r\n", ret);
				return -1;
			}
			break;
		}
	}
	sscanf(maskBuf, "%X %X %X %X %X %X %X %X %X", &maskBytes[0], &maskBytes[1], &maskBytes[2], &maskBytes[3], &maskBytes[4], &maskBytes[5], &maskBytes[6], &maskBytes[7], &maskBytes[8]);

	for(i = 0; i < 8; i++){
		for(j = 0; j < 9; j++){
			if(pageMap[i][j]->page_name == NULL)
				continue;
			if(strstr(req->request_uri, pageMap[i][j]->page_name)){
				fprintf(stderr, "Page Found\r\n");
				break_flag = 1;
				break;
			}
			pagesetNow = pageMap[i][j];
			pagesetNext = pageMap[i][j]->next;
			while(pagesetNext){
				pagesetNow = pagesetNext;
				pagesetNext = pagesetNow->next;
				if(strstr(req->request_uri, pagesetNow->page_name)){
					fprintf(stderr, "Page Found \r\n");
					break_flag = 1;
					break;
				}
			}
			if(break_flag){
				fprintf(stderr, "Break inner loop\r\n");
				break_flag = 1;
				break;
			}
		}
		if(break_flag){
			fprintf(stderr, "Break outer loop\r\n");
			break_flag = 0;
			break;
		}
	}
	if(i < 8 && j < 9 && strcmp(req->request_uri, "/") && strcmp(req->request_uri, "/cgi-bin/index.asp")){
		if(!(maskBytes[0] & bitMask[i])){
			fprintf(stderr, "navigation-xxx.asp is blocked\r\n");
#if defined(TCSUPPORT_CT_E8GUI)
			send_r_web_unauthorized(req);
#else
			send_r_unauthorized(req,"Member Login");
#endif
			return 0;
		}
		if(j >= 1 && !(maskBytes[i+1] & bitMask[j-1])){
			fprintf(stderr, "This page is blocked\r\n");
#if defined(TCSUPPORT_CT_E8GUI)
			send_r_web_unauthorized(req);
#else
			send_r_unauthorized(req,"Member Login");
#endif
			return 0;
		}
	}
#endif
	
    if (req->method == M_POST) {
#if defined(TCSUPPORT_CUC_LANDING_PAGE)
		if (0 == strcmp(req->pathname,"/boaroot/cgi-bin/register.asp")){
			if ( !is_pon_phystate_up() )
			{
				memset(nodename, 0x00, sizeof(nodename));
				strcpy(nodename, "Lan_Entry0");
				if ( tcapi_get(nodename, "IP", lanip) < 0 )
					strcpy(lanip, "192.168.1.1");
				memset(hostUrl, 0, sizeof(hostUrl));					
				sprintf(hostUrl, "http://%s/cgi-bin/pondown.asp", lanip);
				send_r_moved_temp(req, hostUrl, "");
				return 0;
			}					
		}
#endif
#if defined(TCSUPPORT_CT_E8GUI)

#if defined(TCSUPPORT_CT_SIMCARD_SEPARATION)
	if (strcmp(req->pathname,"/boaroot/cgi-bin/index2.asp") && checkSimCardFlag()){
		tcapi_set("System_Entry", "reqMethod", "Post");
		snprintf(redirectUrl, sizeof(redirectUrl), "http://%s/cgi-bin/InsertSimcardMsg.cgi" , req->local_ip_addr);
		send_r_moved_temp(req , redirectUrl , "");
		return 0;
	}
#endif

    	REQ_TIMEOUT = REQ_LONG_TIMEOUT;
	for(i = 0 ; i < USR_NUM; i++){
		sprintf(xmlNode, "Account_Entry%d", i);
		ret = tcapi_get(xmlNode, "username", xmlValue);
		if(ret < 0){ //Fail to acquire username from romfile
			fprintf(stderr, "Fail to acquire username from romfile: ret = %d\r\n", ret);
				return -1;
		}
		if(!strcmp(cur_username, xmlValue)){
			if (strcmp(req->pathname,"/boaroot/cgi-bin/refresh.asp") != 0){
				//time(&pre_user_time[i]);
				clock_gettime(CLOCK_MONOTONIC,&pre_user_time_new[i]);		
			}
		}
	}

	if ((!strcmp(req->pathname,"/boaroot/cgi-bin/index2.asp"))
#ifdef TCSUPPORT_C1_CUC
	|| (!strcmp(req->pathname,"/boaroot/cgi-bin/index3.asp"))
#endif
	){
			iNormal_Flag = 1;
	}
#if defined(TCSUPPORT_CT_FW_UPGRADE_16M)
	if(1 == uploadDownLoadUrl(req))
        {
		releaseMemory2(req);
        }
	else
	{
		if( 0 != strcmp(req->pathname,"/boaroot/cgi-bin/net-tr069.asp") // for ca upload
			&& NULL != req->content_type
			&& NULL != strstr(req->content_type, "multipart"))
		{
			send_r_bad_request(req);
			return 0;
		}
	}
#endif
#endif
#ifdef TRENDCHIP
#ifdef PURE_BRIDGE		
		if(strcmp(req->pathname,"/boaroot/cgi-bin/tools_update.asp") == 0){
			for(i=0;i<4;i++){
				sprintf(string, "/usr/script/wan_stop.sh %d", i);
				system(string);
			}
			system("killall -9 utelnetd");
			sleep(1);
			system("rmmod tc3162l2sar.ko");
			system("sys memwl b4000040 ffffffff");
			sleep(1);
			system("rmmod tc3162_dmt.ko");
		}
#endif
#ifdef WITHVOIP
#if (VOIP == 1) /*MTK VoIP*/
#ifndef  TCSUPPORT_ECN_SIP
		system("killall -9 sipclient");
#endif
#else
	if(strcmp(req->pathname,"/boaroot/cgi-bin/tools_update.asp") == 0){
		fp = fopen("/etc/restart_voip","w");
                        if(fp != NULL){
                                fputc('s',fp);
                                fclose(fp);
                        }
                        sleep(3);
	}
#endif
#endif		
#endif        
		req->post_data_fd = create_temporary_file(1, NULL, 0);
        if (req->post_data_fd == 0)
            return(0);
#if defined(TCSUPPORT_XPON_LED_UPGRADE)
		if(strcmp(req->pathname,"/boaroot/cgi-bin/upgrade.asp") == 0)
			system("echo 3 > /proc/tc3162/led_xpon");
#endif
        return(1); /* success */
    }

    if (req->is_cgi) {
    	
        return init_cgi(req);
    }

    req->status = WRITE;
    return init_get(req);       /* get and head */
}

/*
 * Name: process_option_line
 *
 * Description: Parses the contents of req->header_line and takes
 * appropriate action.
 */

int process_option_line(request * req)
{
    char c, *value, *line = req->header_line;

    /* Start by aggressively hacking the in-place copy of the header line */

#ifdef FASCIST_LOGGING
    log_error_time();
    fprintf(stderr, "%s:%d - Parsing \"%s\"\n", __FILE__, __LINE__, line);
#endif
		
    value = strchr(line, ':');
    if (value == NULL)
        return 0;
    *value++ = '\0';            /* overwrite the : */
    to_upper(line);             /* header types are case-insensitive */
    while ((c = *value) && (c == ' ' || c == '\t'))
        value++;

    if (!memcmp(line, "IF_MODIFIED_SINCE", 18) && !req->if_modified_since)
        req->if_modified_since = value;

    else if (!memcmp(line, "CONTENT_TYPE", 13) && !req->content_type)
        req->content_type = value;

    else if (!memcmp(line, "CONTENT_LENGTH", 15) && !req->content_length)
        req->content_length = value;

    else if (!memcmp(line, "CONNECTION", 11) &&
             ka_max && req->keepalive != KA_STOPPED) {
        req->keepalive = (!strncasecmp(value, "Keep-Alive", 10) ?
                          KA_ACTIVE : KA_STOPPED);
    }
    /* #ifdef ACCEPT_ON */
    else if (!memcmp(line, "ACCEPT", 7))
        add_accept_header(req, value);
    /* #endif */

    /* Need agent and referer for logs */
    else if (!memcmp(line, "REFERER", 8)) {
        req->header_referer = value;
        if (!add_cgi_env(req, "REFERER", value, 1))
            return 0;
    } else if (!memcmp(line, "USER_AGENT", 11)) {
        req->header_user_agent = value;
        if (!add_cgi_env(req, "USER_AGENT", value, 1))
            return 0;
    }
    #if 0
    else if (!memcmp(line, "COOKIE", 6))//lee 2006-4-11
    {
    #endif
	/*
		krammer change for bug 1473.
		we use the SESSIONID in cookie to do the authentication.
		But opera brower will send cookie and cookie2 to our cpe,
		and the cookie2 will cover the SESSIONID. So opera can't 
		login our CPE because wrong SESSIONID.
		I do the following change to make cookie2 can't be stored into our CPE
	*/
    else if (!memcmp(line, "COOKIE", 6)&&memcmp(line, "COOKIE2", 7))//krammer 2008-12-4
    {
    		//fprintf(stderr,"line:%s\n",line);
    	int len = strlen(value)+1;
    	req->cookie = malloc(sizeof(char)*len);
    	memset(req->cookie,0,len);
    	memcpy(req->cookie,value,len-1);
    	if (!add_cgi_env(req, line, value, 1))
    		return 0;
    }
    else if (!memcmp(line, "AUTHORIZATION", 13))
    {
    	int len = strlen(value)+1;
    	req->authorize = malloc(sizeof(char)*len);
    	memset(req->authorize,0,len);
    	memcpy(req->authorize,value,len-1);
    	if (!add_cgi_env(req, line, value, 1))
    		return 0;
    }
#if defined(TCSUPPORT_TRUE_LANDING_PAGE)
    else if (!memcmp(line,"HOST",4))
    {
        req->hostname = value;
        if (!add_cgi_env(req, "HOST", value, 1))
            return 0;
    }
    else if(!memcmp(line,"ACCEPT_LANGUAGE",15))
    {
        req->accept_language = value;
    }
#endif
    else {
        if (!add_cgi_env(req, line, value, 1))
            return 0;
    }
    return 1;
}

/*
 * Name: add_accept_header
 * Description: Adds a mime_type to a requests accept char buffer
 *   silently ignore any that don't fit -
 *   shouldn't happen because of relative buffer sizes
 */

void add_accept_header(request * req, char *mime_type)
{
#ifdef ACCEPT_ON
    int l = strlen(req->accept);
    int l2 = strlen(mime_type);

    if ((l + l2 + 2) >= MAX_HEADER_LENGTH)
        return;

    if (req->accept[0] == '\0')
        strcpy(req->accept, mime_type);
    else {
        req->accept[l] = ',';
        req->accept[l + 1] = ' ';
        memcpy(req->accept + l + 2, mime_type, l2 + 1);
        /* the +1 is for the '\0' */
        /*
           sprintf(req->accept + l, ", %s", mime_type);
         */
    }
#endif
}

void free_requests(void)
{
    request *ptr, *next;

    ptr = request_free;
    while (ptr != NULL) {
        next = ptr->next;
        free(ptr);
        ptr = next;
    }
    request_free = NULL;
}
/* else TCSUPPORT_CT */
#else
#include "../../lib/libtcapi.h"

#ifdef TCSUPPORT_SYSLOG_ENHANCE
#include <syslog.h>
#endif

int total_connections;
struct status status;
char cur_username[129];
#if defined(TCSUPPORT_WEB_SAVE)
int flag_save = 0;
#endif

#ifdef TCSUPPORT_MODEL_NAME_CUSKIT
#define SYS_INFO_PRODUCT_NAME	"ProductName"
#define SYS_INFO_NODE_NAME "SysInfo"
#define SUB_NODE_NAME "Entry"
#endif

static int sockbufsize = SOCKETBUF_SIZE;

/* function prototypes located in this file only */
static void free_request(request ** list_head_addr, request * req);
static inline struct passwd *getrealpass(const char *user);

#if defined(TCSUPPORT_TRUE_LANDING_PAGE)
// can open this part if user have login fail times warning require
#define LOGINFAIL_NUM           3
static int checkLoginFailFlag = 0;
static int loginFailJump2url(request *req);
static int updateLoginFailStatusFlag(request *req);
static void clearLoginFailStatus(request *req);
static int isDefaultUsernamePassword(void);
#endif

static int dynamicPassUrl(char * url);
/*
 * Name: new_request
 * Description: Obtains a request struct off the free list, or if the
 * free list is empty, allocates memory
 *
 * Return value: pointer to initialized request
 */
#if  defined(TCSUPPORT_WEBSERVER_SSL)
extern int server_ssl;		
static request*  newest_request = NULL;
extern int ssl_pending_requests;
extern sslKeys_t* ssl_key;
#endif


request *new_request(void)
{
    request *req;

    if (request_free) {
        req = request_free;     /* first on free list */
        dequeue(&request_free, request_free); /* dequeue the head */
    } else {
        req = (request *) malloc(sizeof (request));
        if (!req) {
            log_error_time();
            perror("malloc for new request");
            return NULL;
        }
    }

    memset(req, 0, offsetof(request, buffer) + 1);
		//lee 2006-4-20
		req->cookie = NULL;
		req->authorize = NULL;
#if defined(TCSUPPORT_TRUE_LANDING_PAGE)
		req->hostname = NULL;
		req->accept_language = NULL;
#endif
#if defined(TCSUPPORT_WEBSERVER_SSL)
		req->ssl = NULL;
#endif
    return req;
}

/*
 * Name: get_request
 *
 * Description: Polls the server socket for a request.  If one exists,
 * does some basic initialization and adds it to the ready queue;.
 */

void get_request(int server_s)
{
    int fd;                     /* socket */
    struct SOCKADDR remote_addr; /* address */
    struct SOCKADDR salocal;
    int remote_addrlen = sizeof (struct SOCKADDR);
    request *conn;              /* connection */
    size_t len;
    static int system_bufsize = 0; /* Default size of SNDBUF given by system */

		//fprintf(stderr,"get_request1\n");
    remote_addr.S_FAMILY = 0xdead;
    fd = accept(server_s, (struct sockaddr *) &remote_addr,
                &remote_addrlen);
		//fprintf(stderr,"get_request2\n");
    if (fd == -1) {
        if (errno != EAGAIN && errno != EWOULDBLOCK)
            /* abnormal error */
            WARN("accept");
        else
            /* no requests */
            pending_requests = 0;
        return;
    }
    if (fd >= FD_SETSIZE) {
        WARN("Got fd >= FD_SETSIZE.");
        close(fd);
	return;
    }
#ifdef DEBUGNONINET
    /* This shows up due to race conditions in some Linux kernels
       when the client closes the socket sometime between
       the select() and accept() syscalls.
       Code and description by Larry Doolittle <ldoolitt@boa.org>
     */
#define HEX(x) (((x)>9)?(('a'-10)+(x)):('0'+(x)))
    if (remote_addr.sin_family != AF_INET) {
        struct sockaddr *bogus = (struct sockaddr *) &remote_addr;
        char *ap, ablock[44];
        int i;
        close(fd);
        log_error_time();
        for (ap = ablock, i = 0; i < remote_addrlen && i < 14; i++) {
            *ap++ = ' ';
            *ap++ = HEX((bogus->sa_data[i] >> 4) & 0x0f);
            *ap++ = HEX(bogus->sa_data[i] & 0x0f);
        }
        *ap = '\0';
        fprintf(stderr, "non-INET connection attempt: socket %d, "
                "sa_family = %hu, sa_data[%d] = %s\n",
                fd, bogus->sa_family, remote_addrlen, ablock);
        return;
    }
#endif

/* XXX Either delete this, or document why it's needed */
/* Pointed out 3-Oct-1999 by Paul Saab <paul@mu.org> */
#ifdef REUSE_EACH_CLIENT_CONNECTION_SOCKET
    if ((setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (void *) &sock_opt,
                    sizeof (sock_opt))) == -1) {
        DIE("setsockopt: unable to set SO_REUSEADDR");
    }
#endif

    len = sizeof(salocal);

    if (getsockname(fd, (struct sockaddr *) &salocal, &len) != 0) {
        WARN("getsockname");
        close(fd);
        return;
    }
		//fprintf(stderr,"get_request3\n");
    conn = new_request();
    if (!conn) {
    	//fprintf(stderr,"get_request33\n");
        close(fd);
        return;
    }
    conn->fd = fd;
    conn->status = READ_HEADER;
    conn->header_line = conn->client_stream;
    conn->time_last = current_time;
    conn->kacount = ka_max;

    ascii_sockaddr(&salocal, conn->local_ip_addr, NI_MAXHOST);

    /* nonblocking socket */
    if (set_nonblock_fd(conn->fd) == -1)
        WARN("fcntl: unable to set new socket to non-block");

    /* set close on exec to true */
    if (fcntl(conn->fd, F_SETFD, 1) == -1)
        WARN("fctnl: unable to set close-on-exec for new socket");

    /* Increase buffer size if we have to.
     * Only ask the system the buffer size on the first request,
     * and assume all subsequent sockets have the same size.
     */
    if (system_bufsize == 0) {
        len = sizeof (system_bufsize);
        if (getsockopt
            (conn->fd, SOL_SOCKET, SO_SNDBUF, &system_bufsize, &len) == 0
            && len == sizeof (system_bufsize)) {
            /*
               fprintf(stderr, "%sgetsockopt reports SNDBUF %d\n",
               get_commonlog_time(), system_bufsize);
             */
            ;
        } else {
            WARN("getsockopt(SNDBUF)");
            system_bufsize = 1;
        }
    }
    if (system_bufsize < sockbufsize) {
        if (setsockopt
            (conn->fd, SOL_SOCKET, SO_SNDBUF, (void *) &sockbufsize,
             sizeof (sockbufsize)) == -1) {
            WARN("setsockopt: unable to set socket buffer size");
#ifdef DIE_ON_ERROR_TUNING_SNDBUF
            exit(errno);
#endif
        }
    }

    /* for log file and possible use by CGI programs */
    ascii_sockaddr(&remote_addr, conn->remote_ip_addr, NI_MAXHOST);

    /* for possible use by CGI programs */
    conn->remote_port = net_port(&remote_addr);

    status.requests++;

#ifdef USE_TCPNODELAY
    /* Thanks to Jef Poskanzer <jef@acme.com> for this tweak */
    {
        int one = 1;
        if (setsockopt(conn->fd, IPPROTO_TCP, TCP_NODELAY,
                       (void *) &one, sizeof (one)) == -1) {
            DIE("setsockopt: unable to set TCP_NODELAY");
        }

    }
#endif

#ifndef NO_RATE_LIMIT
    if (conn->fd > max_connections) {
        send_r_service_unavailable(conn);
        conn->status = DONE;
        pending_requests = 0;
    }
#endif                          /* NO_RATE_LIMIT */

    total_connections++;
#if  defined(TCSUPPORT_WEBSERVER_SSL)
	newest_request = conn;
#endif
    enqueue(&request_ready, conn);
    //fprintf(stderr,"get_request4\n");
}


/*
 * Name: free_request
 *
 * Description: Deallocates memory for a finished request and closes
 * down socket.
 */


static void free_request(request ** list_head_addr, request * req)
{
    int i;
    /* free_request should *never* get called by anything but
       process_requests */

    if (req->buffer_end && req->status != DEAD) {
        req->status = DONE;
        return;
    }
    /* put request on the free list */
    dequeue(list_head_addr, req); /* dequeue from ready or block list */

    if (req->logline)           /* access log */
        log_access(req);

    if (req->mmap_entry_var)
        release_mmap(req->mmap_entry_var);
    else if (req->data_mem)
        munmap(req->data_mem, req->filesize);

    if (req->data_fd)
        close(req->data_fd);

    if (req->post_data_fd)
        close(req->post_data_fd);

    if (req->response_status >= 400)
        status.errors++;

    for (i = COMMON_CGI_COUNT; i < req->cgi_env_index; ++i) {
        if (req->cgi_env[i]) {
            free(req->cgi_env[i]);
        } else {
            log_error_time();
            fprintf(stderr, "Warning: CGI Environment contains NULL value" \
                    "(index %d of %d).\n", i, req->cgi_env_index);
        }
    }

#if  defined(TCSUPPORT_WEBSERVER_SSL)
	if(req->ssl)
	{
		sslFreeConnection(&req->ssl);
    		req->keepalive = KA_INACTIVE;
		req->ssl = NULL;
	}
#endif
    if (req->pathname)
        free(req->pathname);
    if (req->path_info)
        free(req->path_info);
    if (req->path_translated)
        free(req->path_translated);
    if (req->script_name)
        free(req->script_name);
	//lee authorize	
		if(req->cookie)
			free(req->cookie);
		if(req->authorize)
			free(req->authorize);
									
    if ((req->keepalive == KA_ACTIVE) &&
        (req->response_status < 500) && req->kacount > 0) {
        int bytes_to_move;

        request *conn = new_request();
        if (!conn) {
            /* errors already reported */
//            enqueue(&request_free, req);
            close(req->fd);
        	//tcdbg_printf("free_request: ptr=%08lx\r\n", (unsigned long)req);
            free(req);
            total_connections--;
            return;
        }
        conn->fd = req->fd;
        conn->status = READ_HEADER;
        conn->header_line = conn->client_stream;
        conn->kacount = req->kacount - 1;

        /* close enough and we avoid a call to time(NULL) */
        conn->time_last = req->time_last;

        /* for log file and possible use by CGI programs */
        memcpy(conn->remote_ip_addr, req->remote_ip_addr, NI_MAXHOST);
        memcpy(conn->local_ip_addr, req->local_ip_addr, NI_MAXHOST);

        /* for possible use by CGI programs */
        conn->remote_port = req->remote_port;

        status.requests++;

        /* we haven't parsed beyond req->parse_pos, so... */
        bytes_to_move = req->client_stream_pos - req->parse_pos;
        if (bytes_to_move) {
            memcpy(conn->client_stream,
                   req->client_stream + req->parse_pos, bytes_to_move);
            conn->client_stream_pos = bytes_to_move;
        }
    
        enqueue(&request_block, conn);
        BOA_FD_SET(conn->fd, &block_read_fdset);

        //enqueue(&request_free, req);
    	//tcdbg_printf("free_request: ptr=%08lx\r\n", (unsigned long)req);
        free(req);
        return;
    }

    /*
     While debugging some weird errors, Jon Nelson learned that
     some versions of Netscape Navigator break the
     HTTP specification.

     Some research on the issue brought up:

     http://www.apache.org/docs/misc/known_client_problems.html

     As quoted here:

     "
     Trailing CRLF on POSTs

     This is a legacy issue. The CERN webserver required POST
     data to have an extra CRLF following it. Thus many
     clients send an extra CRLF that is not included in the
     Content-Length of the request. Apache works around this
     problem by eating any empty lines which appear before a
     request.
     "

     Boa will (for now) hack around this stupid bug in Netscape
     (and Internet Exploder)
     by reading up to 32k after the connection is all but closed.
     This should eliminate any remaining spurious crlf sent
     by the client.

     Building bugs *into* software to be compatable is
     just plain wrong
     */

    if (req->method == M_POST) {
        char *buf;

        buf = (char*)malloc(32768);
        read(req->fd, buf, 32768);
        free(buf);
    }
    close(req->fd);
    total_connections--;

//    enqueue(&request_free, req);
	//tcdbg_printf("free_request: ptr=%08lx\r\n", (unsigned long)req);
    free(req);
    return;
}

#if  defined(TCSUPPORT_WEBSERVER_SSL)
void get_ssl_request(void)
{
	request *conn = NULL;
	get_request(server_ssl);
	conn = newest_request;
	newest_request = NULL;
	if (!conn)
		return;
	if(boa_sslAccept(&conn->ssl,conn->fd,ssl_key,NULL,0)!=0)
	{
    		close(conn->fd);
    		total_connections--;
    		dequeue(&request_ready, conn);
		free(conn);
		return;
	}
	conn->buffer_start =  conn->buffer_end = 0;
	return;
}
#endif
/*
 * Name: process_requests
 *
 * Description: Iterates through the ready queue, passing each request
 * to the appropriate handler for processing.  It monitors the
 * return value from handler functions, all of which return -1
 * to indicate a block, 0 on completion and 1 to remain on the
 * ready list for more procesing.
 */


void process_requests(int server_s)
{
    int retval = 0;
    request *current, *trailer;

    if (pending_requests ) {
        get_request(server_s);
#ifdef ORIGINAL_BEHAVIOR
        pending_requests = 0;
#endif
    }	
#if  defined(TCSUPPORT_WEBSERVER_SSL)
    if (ssl_pending_requests) {
        get_ssl_request();
	ssl_pending_requests = 0;
    }	
#endif	
    current = request_ready;

    while (current) {
        time(&current_time);
        if (current->buffer_end && /* there is data in the buffer */
            current->status != DEAD && current->status != DONE) {
            retval = req_flush(current);
            /*
             * retval can be -2=error, -1=blocked, or bytes left
             */
            if (retval == -2) { /* error */
                current->status = DEAD;
                retval = 0;
            } else if (retval >= 0) {
                /* notice the >= which is different from below?
                   Here, we may just be flushing headers.
                   We don't want to return 0 because we are not DONE
                   or DEAD */

                retval = 1;
            }
        } else {
            switch (current->status) {
            case READ_HEADER:
            case ONE_CR:
            case ONE_LF:
            case TWO_CR:
            		
                retval = read_header(current);
                break;
            case BODY_READ:
                retval = read_body(current);
                break;
            case BODY_WRITE:
                retval = write_body(current);
                break;
            case WRITE:
                retval = process_get(current);
                break;
            case PIPE_READ:            		
                retval = read_from_pipe(current);                
                break;
            case PIPE_WRITE:
                retval = write_from_pipe(current);
                break;
            case DONE:
                /* a non-status that will terminate the request */
                retval = req_flush(current);
                /*
                 * retval can be -2=error, -1=blocked, or bytes left
                 */
                if (retval == -2) { /* error */
                    current->status = DEAD;
                    retval = 0;
                } else if (retval > 0) {
                    retval = 1;
                }
                break;
            case DEAD:
                retval = 0;
                current->buffer_end = 0;
                SQUASH_KA(current);
                break;
            default:
                retval = 0;
                fprintf(stderr, "Unknown status (%d), "
                        "closing!\n", current->status);
                current->status = DEAD;
                break;
            }

        }

                
        if (sigterm_flag)
            SQUASH_KA(current);

        /* we put this here instead of after the switch so that
         * if we are on the last request, and get_request is successful,
         * current->next is valid!
         */
        if (pending_requests)
        {
            get_request(server_s);
		}


        switch (retval) {
        case -1:               /* request blocked */
            trailer = current;
            current = current->next;
            block_request(trailer);
         
            break;
        case 0:                /* request complete */
            current->time_last = current_time;
            trailer = current;
            current = current->next;
            free_request(&request_ready, trailer);

            break;
        case 1:                /* more to do */
            current->time_last = current_time;
            current = current->next;
            break;
        default:
            log_error_time();
            fprintf(stderr, "Unknown retval in process.c - "
                    "Status: %d, retval: %d\n", current->status, retval);
            current = current->next;
            break;
        }
    }

}

/*
 * Name: process_logline
 *
 * Description: This is called with the first req->header_line received
 * by a request, called "logline" because it is logged to a file.
 * It is parsed to determine request type and method, then passed to
 * translate_uri for further parsing.  Also sets up CGI environment if
 * needed.
 */

int process_logline(request * req)
{
    char *stop, *stop2;
    static char *SIMPLE_HTTP_VERSION = "HTTP/0.9";
	
    #ifdef BOA_DEBUG
    struct timeval tv;
	gettimeofday(&tv,NULL);
	fprintf(stderr,"process_logline:%u,%u\n",tv.tv_usec,tv.tv_sec);
	#endif
		
    req->logline = req->client_stream;
    if (!memcmp(req->logline, "GET ", 4))
        req->method = M_GET;
    else if (!memcmp(req->logline, "HEAD ", 5))
        /* head is just get w/no body */
        req->method = M_HEAD;
    else if (!memcmp(req->logline, "POST ", 5))
        req->method = M_POST;
    else {
    	#if 0
        log_error_time();
        fprintf(stderr, "malformed request: \"%s\"\n", req->logline);
        #endif
        send_r_not_implemented(req);
        return 0;
    }

    req->http_version = SIMPLE_HTTP_VERSION;
    req->simple = 1;

    /* Guaranteed to find ' ' since we matched a method above */
    stop = req->logline + 3;
    if (*stop != ' ')
        ++stop;

    /* scan to start of non-whitespace */
    while (*(++stop) == ' ');

    stop2 = stop;

    /* scan to end of non-whitespace */
    while (*stop2 != '\0' && *stop2 != ' ')
        ++stop2;

    if (stop2 - stop > MAX_HEADER_LENGTH) {
        log_error_time();
        fprintf(stderr, "URI too long %d: \"%s\"\n", MAX_HEADER_LENGTH,
                req->logline);
        send_r_bad_request(req);
        return 0;
    }
    memcpy(req->request_uri, stop, stop2 - stop);
    req->request_uri[stop2 - stop] = '\0';

    if (*stop2 == ' ') {
        /* if found, we should get an HTTP/x.x */
        unsigned int p1, p2;

        /* scan to end of whitespace */
        ++stop2;
        while (*stop2 == ' ' && *stop2 != '\0')
            ++stop2;

        /* scan in HTTP/major.minor */
        if (sscanf(stop2, "HTTP/%u.%u", &p1, &p2) == 2) {
            /* HTTP/{0.9,1.0,1.1} */
            if (p1 == 1 && (p2 == 0 || p2 == 1)) {
                req->http_version = stop2;
                req->simple = 0;
            } else if (p1 > 1 || (p1 != 0 && p2 > 1)) {
                goto BAD_VERSION;
            }
        } else {
            goto BAD_VERSION;
        }
    }

    if (req->method == M_HEAD && req->simple) {
        send_r_bad_request(req);
        return 0;
    }
    req->cgi_env_index = COMMON_CGI_COUNT;

    return 1;

BAD_VERSION:
    log_error_time();
    fprintf(stderr, "bogus HTTP version: \"%s\"\n", stop2);
    send_r_bad_request(req);
    return 0;
}

//lee 2006-4-11
#define NIL 0
static const char *my_strpbrk (char *cs,char *ct)
{
  char *s;
				/* search for delimiter until end of string */
  for (; *cs; cs++) for (s = ct; *s; s++) if (*s == *cs) return cs;
  return NIL;			/* not found */
}

#define JNK 0177
#define PAD 0100
static void *rfc822_base64 (unsigned char *src,unsigned long srcl,unsigned long *len)
{
  char c,*s,tmp[512];
  void *ret = malloc ((size_t) (*len = 4 + ((srcl * 3) / 4)));
  char *d = (char *) ret;
  int e;
  static char decode[256] = {
   JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,
   JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,
   JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,076,JNK,JNK,JNK,077,
   064,065,066,067,070,071,072,073,074,075,JNK,JNK,JNK,PAD,JNK,JNK,
   JNK,000,001,002,003,004,005,006,007,010,011,012,013,014,015,016,
   017,020,021,022,023,024,025,026,027,030,031,JNK,JNK,JNK,JNK,JNK,
   JNK,032,033,034,035,036,037,040,041,042,043,044,045,046,047,050,
   051,052,053,054,055,056,057,060,061,062,063,JNK,JNK,JNK,JNK,JNK,
   JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,
   JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,
   JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,
   JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,
   JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,
   JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,
   JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,
   JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK
  };
  memset (ret,0,(size_t) *len);	/* initialize block */
  *len = 0;			/* in case we return an error */

				/* simple-minded decode */
  for (e = 0; srcl--; ) switch (c = decode[*src++]) {
  default:			/* valid BASE64 data character */
    switch (e++) {		/* install based on quantum position */
    case 0:
      *d = c << 2;		/* byte 1: high 6 bits */
      break;
    case 1:
      *d++ |= c >> 4;		/* byte 1: low 2 bits */
      *d = c << 4;		/* byte 2: high 4 bits */
      break;
    case 2:
      *d++ |= c >> 2;		/* byte 2: low 4 bits */
      *d = c << 6;		/* byte 3: high 2 bits */
      break;
    case 3:
      *d++ |= c;		/* byte 3: low 6 bits */
      e = 0;			/* reinitialize mechanism */
      break;
    }
    break;
  case JNK:			/* junk character */
    break;
  case PAD:			/* padding */
    switch (e++) {		/* check quantum position */
    case 3:			/* one = is good enough in quantum 3 */
				/* make sure no data characters in remainder */
      for (; srcl; --srcl) switch (decode[*src++]) {
      case JNK: case PAD:	/* ignore junk and extraneous padding */
	break;
      default:			/* valid BASE64 data character */
	/* This indicates bad MIME.  One way that it can be caused is if
	   a single-section message was BASE64 encoded and then something
	   (e.g. a mailing list processor) appended text.  The problem is
	   that in 1 out of 3 cases, there is no padding and hence no way
	   to detect the end of the data.  Consequently, prudent software
	   will always encapsulate a BASE64 segment inside a MULTIPART.
	   */
	sprintf (tmp,"Possible data truncation in rfc822_base64(): %.80s",
		 (char *) src - 1);
	s = my_strpbrk (tmp,"\015\012");
	if (s) *s = NIL;
	
	srcl = 1;		/* don't issue any more messages */
	break;
      }
      break;
    case 2:			/* expect a second = in quantum 2 */
      if (srcl && (*src == '=')) break;
    default:			/* impossible quantum position */
      free (ret);
      return NIL;
    }
    break;
  }
  *len = d - (char *) ret;	/* calculate data length */
  return ret;			/* return the string */
}


typedef struct param_s {
	char	*name;
	char	*value;
} s_param;

static void free_param(s_param **var)
{
	int i;
	#ifdef TRENDCHIP
	if(var){/*var can't be free if it point to 0*/
	#endif
	for (i = 0;var[i];i++)
	{
		if(var[i]->name)
			free(var[i]->name);
		if(var[i]->value)
			free(var[i]->value);
		free(var[i]);
	}
	free(var);
	#ifdef TRENDCHIP
	}
	#endif
}
static s_param **param_line (char *line,char separate1,char separate2)
{
    int numargs;
    char *cp, *ip, *esp, *sptr;
    s_param **result = NULL;
    int i, k, len;

    if (line) {
	    if (strlen(line)) {
		for (numargs=1,cp=line; *cp; cp++)
		    if (*cp == separate2) numargs++;
	    } else
		numargs = 0;


	    len = (numargs+1) * sizeof(s_param *);
	    if ((result = (s_param **)malloc (len)) == NULL)
	    {
			return NULL;
			}
	    memset (result, 0, len);

	    cp = line;
	    i=0;
	    while (*cp) {
		if ((ip = (char *)strchr(cp, separate2)) != NULL) {
		    *ip = '\0';
		}else
		    ip = cp + strlen(cp);

		if ((esp=(char *)strchr(cp, separate1)) == NULL) {
		    cp = ++ip;
		    continue;
		}

		if (!strlen(esp)) {
		    cp = ++ip;
		    continue;
		}

		if (i<numargs) {

		    /* try to find out if there's already such a variable */
		    for (k=0; k<i && (strncmp (result[k]->name,cp, esp-cp) || !(strlen (result[k]->name) == esp-cp)); k++);

		    if (k == i) {	/* No such variable yet */
			if ((result[i] = (s_param *)malloc(sizeof(s_param))) == NULL)
			{
				    return NULL;
			}
			if ((result[i]->name = (char *)malloc((esp-cp+1) * sizeof(char))) == NULL)
			 {
		    return NULL;
		  }
		  
			memset (result[i]->name, 0, esp-cp+1);
			strncpy(result[i]->name, cp, esp-cp);
			cp = ++esp;
			if ((result[i]->value = (char *)malloc((ip-esp+1) * sizeof(char))) == NULL)
			{
			    return NULL;
			 }
			  
			memset (result[i]->value, 0, ip-esp+1);
			strncpy(result[i]->value, cp, ip-esp);
			//FIXME result[i]->value = cgiDecodeString(result[i]->value);
			i++;
		    } else {	/* There is already such a name, suppose a mutiple field */
			cp = ++esp;
			len = (strlen(result[k]->value)+(ip-esp)+2) * sizeof (char);
			if ((sptr = (char *)malloc(len)) == NULL)
			{
			    return NULL;
			  }
			  
			memset (sptr, 0, len);
			sprintf (sptr, "%s\n", result[k]->value);
			strncat(sptr, cp, ip-esp);
			free(result[k]->value);
		//FIXMEresult[k]->value = cgiDecodeString (sptr);
		result[k]->value = sptr;
	    }
	}
	cp = ++ip;
    }
  }

  return result;
}

int check_validate(char *usr,char*pwd,request *req,char *path)
{
	int level = 1;
	char str[256];
	struct passwd *passwdbuf;
	FILE *fd;
	char *pwdencode;

	passwdbuf = getrealpass(usr);
	if (passwdbuf == NULL)
	{
		#ifdef BOA_DEBUG
		fprintf(stderr,"check_validate err.\n");
		#endif
		return -1;
	}
	else
	{
		pwdencode = crypt(pwd, passwdbuf->pw_passwd);
		if (strcmp(pwdencode, passwdbuf->pw_passwd) == 0) 
  {
  	#ifdef BOA_DEBUG
  fprintf(stderr,"pw:%s\n",pwdencode);	
  #endif
  sprintf(str,"%d",(int)passwdbuf->pw_uid);
  strcpy(req->uid,str);
  sprintf(str,"%d",(int)passwdbuf->pw_gid);
  strcpy(req->gid,str);
  #ifdef BOA_DEBUG
  fprintf(stderr,"u:%s,g:%s\n",req->uid,req->gid);
  #endif

			#ifdef TRENDCHIP
				if(strcmp(path, "Chrome")){
			#endif
  fd = fopen(path,"r+");
	if(fd ==NULL)
	{
		#ifdef BOA_DEBUG
		fprintf(stderr,"check_validate open %s err\n",path);
		#endif
		return -1;
	}
		
	sprintf(str,"usr=%s\npwd=%s\nuid=%s\ngid=%s\nlevel=%d\nip=%s",usr,pwdencode,req->uid,req->gid,level,req->remote_ip_addr);
	fwrite(str,1,strlen(str),fd);
	fclose(fd);
	
			#ifdef TRENDCHIP
				}
			#endif
			
	strcpy(req->usr_name,usr);
	strcpy(req->pwd,pwdencode);
	sprintf(str,"%d",level);
	strcpy(req->access_level,str); 
	#ifdef BOA_DEBUG
	fprintf(stderr,"authorize:%s,%s,%s\n",req->usr_name,req->pwd,req->access_level);
	#endif
  return 1;
		}
		else
		{
		  	#ifdef BOA_DEBUG
  				fprintf(stderr,"check_validate pwd err\n");
  			#endif
  			return -1;
		}
	}
	
}
		
static inline struct passwd *getrealpass(const char *user) 
{	
	struct passwd *pwp;
	pwp = getpwnam(user);
	if (pwp == NULL)
		return NULL;
	return pwp;
}

int check_session(char *path,char*ip,request *req)
{
	FILE *fd;
	char buf[256];
	s_param **sub;
	
	fd = fopen(path,"r+");
	if(fd == NULL)
	{
		#ifdef BOA_DEBUG
		fprintf(stderr,"check_session open %s err\n",path);
		#endif
		return -1;
	}
	
	while(fgets(buf,256,fd))
	{
		char *ret;
		ret = strchr(buf,'\r');
		if(ret)
			ret[0] = '\0';
		ret = strchr(buf,'\n');
		if(ret)
			ret[0] ='\0';
				
		sub = param_line(buf,'=',';');
		if(!strcmp("usr", sub[0]->name))
			sprintf(cur_username, "%s", sub[0]->value);
		#ifdef BOA_DEBUG
		fprintf (stderr,"check_session:%s=%s\n", sub[0]->name, sub[0]->value);
		#endif
		if(!strcmp("usr",sub[0]->name))
			strcpy(req->usr_name,sub[0]->value);
		if(!strcmp("pwd",sub[0]->name))
			strcpy(req->pwd ,sub[0]->value);
		if(!strcmp("level",sub[0]->name))
			strcpy(req->access_level,sub[0]->value);
		if(!strcmp("uid",sub[0]->name))
			strcpy(req->uid,sub[0]->value);
		if(!strcmp("gid",sub[0]->name))
			strcpy(req->gid,sub[0]->value);
			
		free_param(sub);		
	}
	fclose(fd);
	#ifdef BOA_DEBUG
	fprintf(stderr,"check_session:u:%s,p:%s,l:%s,gid:%s,uid:%s\n",req->usr_name,req->pwd,req->access_level,req->gid,req->uid);
	#endif
	return atoi(req->access_level);
		
}


// add the url need to pass dynamic
static int dynamicPassUrl(char * url)
{
#if defined(TCSUPPORT_TRUE_LANDING_PAGE)
        if(1 == isDefaultUsernamePassword())
        {
                if( (NULL != strstr(url , "/cgi-bin/wzExistAccount_true.asp")) ||
                (NULL != strstr(url , "/wzExistAccount_true.htm")) ||
                (NULL != strstr(url , "/cgi-bin/wzIspUserPwd_true.asp")) ||
                (NULL != strstr(url , "/cgi-bin/wzWait3_true.asp")) ||
                (NULL != strstr(url , "/cgi-bin/wzWiFi_true.asp")) ||
                (NULL != strstr(url , "/cgi-bin/wzWait2_true.asp"))
                )
                        return 1;
        }
#endif
        return 0;
}
#if !defined(TCSUPPORT_SELF_LOGIN_WINDOW) 
int passURL(char * url)
{

        if((NULL != strstr(url , ".js")) ||
        (NULL != strstr(url , ".gif")) ||
        (NULL != strstr(url , ".cgi")) ||
#if defined(TCSUPPORT_TRUE_LANDING_PAGE)
#endif
#if defined(TCSUPPORT_WEB_INTERCEPTION)
        (NULL != strstr(url , "/cgi-bin/FirmwareUpgradeInfo.asp")) ||
        (NULL != strstr(url , "/cgi-bin/AdslDownInfo.asp")) ||
#endif
        (NULL != strstr(url , ".css")) ||
#ifdef TCSUPPORT_WEB_LOGOUT        
	(NULL != strstr(url, "/cgi-bin/login.asp")) ||
#endif    
	(NULL != strstr(url , ".ico")))
        {
                return 0;
        }
        else
        {
                // add the url need to pass dynamic
                if(1 == dynamicPassUrl(url))
                        return 0;
                return -1;
        }
}


int http_authorize(request *req)
{
	s_param **var,**auth;
  	int i;
  	unsigned long len;
	struct stat buf;
	int ret=0;
	char path[256];
	int level=-1;
	FILE *fd;

#ifdef TCSUPPORT_WEB_LOGOUT  
	char *username = NULL;
	char *userpswd = NULL;
#endif


	//if cookie is null return
	if(req->cookie ==NULL)
	{
		#ifdef TRENDCHIP
		/*
			krammer add for bug 1473.
			Chrome will not return the cookie when login procedure,
			so we should not check the cookie or session id when 
			first time to login. When we login successfully, Chrome
			will return the cookie with session id just like other 
			browsers, so the following procedure will be the same
			as other browsers.
		*/
			if(req->header_user_agent){
				if(strstr(req->header_user_agent, "Chrome")){
					/*
						we use "Chrome" to tell check_validate() that it 
						should not write a session file when chrome's
						login.
					*/
					strcpy(path,"Chrome");
					buf.st_size = 0;
					var=0;
					goto chrome;
				}
			}
		#endif
		#ifdef BOA_DEBUG
		fprintf(stderr,"cookie null\n");
		#endif
		return -1;
	}
	#ifdef BOA_DEBUG 
	fprintf(stderr,"cookie:%s\n",req->cookie);
	#endif
	//get sessionid in cookie
	var = param_line (req->cookie, '=',';');
#ifdef TCSUPPORT_WEB_LOGOUT	
	for(i = 0;var[i];i++)
	{	
		if(!strcmp(var[i]->name,"uid") || !strcmp(var[i]->name," uid"))	
		{
			username = var[i]->value;
		}
		if(!strcmp(var[i]->name,"psw") || !strcmp(var[i]->name," psw"))	
		{
			userpswd = var[i]->value;
		}
	}
#endif
	for(i = 0;var[i];i++)
	{	
		#ifdef BOA_DEBUG
		fprintf (stderr,"cookie param:%s=%s\t index:%d\n", var[i]->name, var[i]->value,i);
		if(var[i+1] != NULL)
			fprintf (stderr,"cookie param:%s=%s\t index:%d\n", var[i+1]->name, var[i+1]->value,i+1);
		#endif
		
		#ifdef TRENDCHIP /*add "mstnc" is for ie8.0.7601.17514 version*/
#if !defined(TCSUPPORT_CZ_GENERAL) 
		#ifdef TCSUPPORT_WEB_LOGOUT
		if(!strcmp(var[i]->name,"SESSIONID") || !strcmp(var[i]->name," SESSIONID") || !strcmp(var[i]->name,"mstnc"))
		#else
		if(!strcmp(var[i]->name,"SESSIONID") || !strcmp(var[i]->name,"mstnc"))
		#endif
#endif
		#else
		if(!strcmp(var[i]->name,"SESSIONID"))
		#endif
		{
			if(var[i+1] == NULL)
			{
			sprintf(path,"/var/tmp/%s",var[i]->value);
				ret = stat(path,&buf);
				if(ret)//can't find session file
				{
					fd = fopen(path,"w");
					fclose(fd);
					#ifdef BOA_DEBUG
					fprintf(stderr,"session file not found,create_0:%s\n",path);
					#endif
					free_param(var);
					return -1;
				}
			}
			else
			{
				sprintf(path,"/var/tmp/%s",var[i]->value);
                                ret = stat(path,&buf);
                                if(ret)//can't find session file
                                {
                                        sprintf(path,"/var/tmp/%s",var[i+1]->value);
                                        ret = stat(path,&buf);
                                        if(ret)
                                        {
                                                fd = fopen(path,"w");
                                                fclose(fd);
                                                #ifdef BOA_DEBUG
                                                                fprintf(stderr,"session file not found,create_1:%s\n",path);
                                                #endif
                                                free_param(var);
                                                return -1;
                                        }
				}
			}

			#ifdef TRENDCHIP
			chrome:
			#endif
#if !defined(TCSUPPORT_CZ_GENERAL) 
			if (buf.st_size ==0)
#endif
			{
#if !defined(TCSUPPORT_WEB_LOGOUT)
				#ifdef BOA_DEBUG
				fprintf(stderr,"buf.st_size = 0\n");
				#endif		
				if(req->authorize == NULL)
				{
					#ifdef BOA_DEBUG
					fprintf(stderr,"authorize = NULL and free param now\n");
					#endif
					free_param(var);
					return -1;	
				}
				//check authorize info
				req->authorize = req->authorize+6;
				req->authorize = rfc822_base64(req->authorize,strlen(req->authorize),&len);
				#ifdef BOA_DEBUG
				fprintf(stderr,"base64 decode:%s\n",req->authorize);
				#endif
				auth = param_line (req->authorize, ':', ';');
				sprintf(cur_username, "%s", auth[0]->name);
				#ifdef BOA_DEBUG
				fprintf (stderr,"auth:%s=%s\n", auth[0]->name, auth[0]->value);
				#endif
				level = check_validate(auth[0]->name,auth[0]->value,req,path);
#else
				if (NULL == username || NULL == userpswd)
				{
					return	-1;
				}	
				
				level = check_validate(username,userpswd,req,path);
#endif				
				if (level== -1)
				{
					#ifdef BOA_DEBUG
					fprintf(stderr,"check_validate  %d err\n",level);
					#endif
					free_param(auth);
					free_param(var);
#ifdef TCSUPPORT_SYSLOG_ENHANCE
					openlog("TCSysLog WEB", 0, LOG_LOCAL2);
					syslog(LOG_INFO, "WEB login failed!\n");
					closelog();
#endif
					return -1;
				}
#ifdef TCSUPPORT_SYSLOG_ENHANCE
				else{
					char log[128];
#if !defined(TCSUPPORT_WEB_LOGOUT)
					snprintf(log, sizeof(log), "WEB user <%s> login\n", auth[0]->name);
#else
					snprintf(log, sizeof(log), "WEB user <%s> login\n", username);
#endif
					openlog("TCSysLog WEB", 0, LOG_LOCAL2);
					syslog(LOG_INFO, log);
					closelog();
				}
#endif				


			}
			else
			{		
					#if defined(TCSUPPORT_WEB_LOGOUT)
					if (NULL == username || NULL == userpswd)
					{
						return	-1;
					}	
					level = check_validate(username,userpswd,req,path);	
					if (level== -1)
					{
						free_param(auth);
						free_param(var);
						return -1;
					}
					#endif
					level = check_session(path,req->remote_ip_addr,req);
					if (level== -1)
					{
						#ifdef BOA_DEBUG
						fprintf(stderr,"check_session  %d err\n",level);
						#endif
						free_param(var);
						return -1;
					}
			}
			break;
			
		} 		
		
	}		
	free_param(var);
	
	return level;
}
#endif



#ifndef PURE_BRIDGE
#if !defined(TCSUPPORT_SELF_LOGIN_WINDOW) 
/*______________________________________________________________________________
**      pageAccessCheck
**
**      descriptions:
**              check the access of the page
**      parameters:
**
**      return:
**              Success:        0
**              FAIL: -1
**_______________________________________________________________________________
*/
static int pageAccessCheck(request * req)
{
        int ret = 0, i = 0 ,  j = 0 , break_flag = 0, maskBytes[9];
        char nodename[15], username[129], maskBuf[30];
        char bitMask[8] = {(1<<0), (1<<1), (1<<2), (1<<3), (1<<4), (1<<5), (1<<6), (1<<7)};
        struct pageset *pagesetNow = NULL , *pagesetNext = NULL;
        extern char cur_username[129];
        extern struct pageset* pageMap[PAGEMAP_I_NUM][PAGEMAP_J_NUM];
        
        memset(maskBytes , 0 , sizeof(maskBytes));
        memset(nodename , 0 , sizeof(nodename));
        memset(username , 0 , sizeof(username));
        memset(maskBuf , 0 , sizeof(maskBuf));
        
        for(i = 0 ; i < 3; i++){
                sprintf(nodename, "Account_Entry%d", i);
                ret = tcapi_get(nodename, "username", username);
                if(ret < 0){ //Fail to acquire username from romfile
                        fprintf(stderr, "Fail to acquire username from romfile: ret = %d\r\n", ret);
                        return -1;
                }

                if(!strcmp(cur_username, username)){
                        ret = tcapi_get(nodename, "display_mask", maskBuf);
                        if(ret < 0){ //Fail to set current username
                                fprintf(stderr, "Fail to get current username's dispaly mask: ret = %d\r\n", ret);
                                return -1;
                        }
                        break;
                }
        }
        sscanf(maskBuf, "%X %X %X %X %X %X %X %X %X", &maskBytes[0], &maskBytes[1], &maskBytes[2], &maskBytes[3], &maskBytes[4], &maskBytes[5], &maskBytes[6], &maskBytes[7], &maskBytes[8]);

        for(i = 0; i < PAGEMAP_I_NUM; i++){
                for(j = 0; j < PAGEMAP_J_NUM; j++){
                        if(pageMap[i][j]->page_name == NULL)
                                continue;
                        if(strstr(req->request_uri, pageMap[i][j]->page_name)){
                                fprintf(stderr, "Page Found\r\n");
                                break_flag = 1;
                                break;
                        }
                        pagesetNow = pageMap[i][j];
                        pagesetNext = pageMap[i][j]->next;
                        while(pagesetNext){
                                pagesetNow = pagesetNext;
                                pagesetNext = pagesetNow->next;
                                if(strstr(req->request_uri, pagesetNow->page_name)){
                                        fprintf(stderr, "Page Found \r\n");
                                        break_flag = 1;
                                        break;
                                }
                        }
                        if(break_flag){
                                fprintf(stderr, "Break inner loop\r\n");
                                break_flag = 1;
                                break;
                        }
                }
                if(break_flag){
                        fprintf(stderr, "Break outer loop\r\n");
                        break_flag = 0;
                        break;
                }
        }
        if(i < PAGEMAP_I_NUM && j < PAGEMAP_J_NUM && strcmp(req->request_uri, "/") && strcmp(req->request_uri, "/cgi-bin/index.asp")){
#if !defined(TCSUPPORT_CD_NEW_GUI) 
                if(!(maskBytes[0] & bitMask[i])){
                        fprintf(stderr, "navigation-xxx.asp is blocked\r\n");
                        return -1;
                }
                if(j >= 1 && !(maskBytes[i+1] & bitMask[j-1])){
                        fprintf(stderr, "This page is blocked\r\n");
                        return -1;
                }        

#endif
        }
        return 0;       
} 
#endif
#endif


/*
 * Name: uploadDownLoadUrl
 *
 * Description: check if the url is upload page or not
 * Returns 1 for found , 0 for not found
 */
int uploadDownLoadUrl(request * req)
{
        if(0 == strcmp(req->pathname,"/boaroot/cgi-bin/tools_update.asp") 
        )
        {
                return 1;
        }
        
        return 0;
}

/*
 * Name: releaseMemory
 *
 * Description: release memory to upload the romfile or tclinux.bin 
 */
void releaseMemory()
{
        int i = 0;
        char string[32] = "";
        
        memset(string , 0 , sizeof(string));
        for(i=0;i<4;i++)
        {
                sprintf(string, "/usr/script/wan_stop.sh %d", i);
                system(string);
        }
        system("killall -9 utelnetd");
        sleep(1);
        system("rmmod tc3162l2sar.ko");
        system("sys memwl b4000040 ffffffff");
        sleep(1);
        system("rmmod tc3162_dmt.ko");
        return;
}

#if 0
#ifdef TCSUPPORT_FW_UPGRADE_16M
#if !defined(TCSUPPORT_CD_NEW_GUI) 
void releaseMemory2()
#endif
{
#if !defined(TCSUPPORT_CD_NEW_GUI) 
	tcapi_set("System_Entry", "upgrade_status", "true");
	system("sh /usr/script/before_web_download.sh 1");
#endif
	sleep(1);	
	sync();
}
#endif
#endif

#ifdef TCSUPPORT_FW_UPGRADE_16M
void releaseMemory2(request *req)
{
	unsigned int mark;
	int marklen;

	tcapi_set("System_Entry", "upgrade_status", "true");

	marklen = sizeof(mark);
	if (getsockopt(req->fd, SOL_IP, IP_SKB_MARK, &mark, &marklen) == 0)
	{
		mark = (mark & 0xf0000000) >> 28;
	#if 1
		tcdbg_printf("makr = %d\n", mark);
	#endif
		if((SSID1_PORT_MASK == mark)||(SSID2_PORT_MASK == mark)||(SSID3_PORT_MASK == mark)||(SSID4_PORT_MASK == mark))
		{
			tcdbg_printf("\r\nRelease memory: WIFI upgrade!!\r\n");
#if !defined(TCSUPPORT_CD_NEW_GUI) 
			system("sh /usr/script/before_web_download.sh 1");
#endif
		}
		else
		{
			tcdbg_printf("\r\nRelease memory: Not WIFI upgrade!!\r\n");
#if !defined(TCSUPPORT_CD_NEW_GUI) 
			system("sh /usr/script/before_web_download_remove_wifi.sh 1");
#endif
		}
	}
	else
	{
		tcdbg_printf("\r\nRelease memory: Unknown upgrade!!\r\n");
		system("/usr/script/before_firmware_update.sh");
	}

	sleep(1);	
	sync();
}
#endif

int voipIdleCheck(int cid)
{
	char tmpStr[64],tcapiStr[16];
	memset(tmpStr,0,sizeof(tmpStr));
	memset(tcapiStr,0,sizeof(tcapiStr));
	sprintf(tmpStr,"VoIPBasic_Entry%d",cid);
	if(!tcapi_get(tmpStr,"SIPEnable",tcapiStr)){
		if(!strcmp(tcapiStr, "Yes")){
			sprintf(tmpStr,"VoIPSysParam_Entry%d",cid);
			if(!tcapi_get(tmpStr,"SC_LINE_INFO_STATUS",tcapiStr)){
				if(!strcmp(tcapiStr, "Idle")){
					return 1;
				}
			}
		}
		else{
			return 1;
		}
	}
    return 0;
}

/*
 * Name: savingFileForVoipRestart
 *
 * Description: saving flag to /etc/restart_voip file
 */
void savingFileForVoipRestart()
{
        FILE *fp = NULL;
        fp = fopen("/etc/restart_voip","w");
        if(fp != NULL)
        {
                fputc('s',fp);
                fclose(fp);
        }
        sleep(3);
        return;
}

#if defined(TCSUPPORT_WEB_INTERCEPTION) || defined(TCSUPPORT_REDIRECT_WLAN_WEB)
/*
 * Name: InterceptionIpFromWan
 *
 * Description: check if the ip is turned by wan ,then interception it 
 * Returns 1 for found , 0 for not found
 */
int InterceptionIpFromWan(request * req)
{
        int j=0;
        char *index=NULL;       
        char host_name[40] = "";
        char hostUrl[64] = "";
#if defined(TCSUPPORT_WEB_INTERCEPTION)
	char adslState[64];
	int upStat=0;
	int fd =0;
	int ret=0;
#endif
        memset(hostUrl , 0 , sizeof(hostUrl));
        memset(host_name , 0 , sizeof(host_name));
        
        /* find the host ip address */
        for(j=COMMON_CGI_COUNT; req->cgi_env[j]  != NULL; j++)
        {
                if((index = strstr(req->cgi_env[j], "HOST"))  != NULL)
                {
                        strncpy(host_name, index+5, sizeof(host_name));
                        break;
                }
        }

        /* if the ip is the wan ip(interception by wan) */
        if(0 != strcmp(host_name, req->local_ip_addr))
        {
#if defined(TCSUPPORT_WEB_INTERCEPTION)
#if !defined(TCSUPPORT_C1_OBM) && !defined(TCSUPPORT_WIZARD_FON_OI) 

                if(0 == access("/tmp/firmwareupgrade", F_OK))
                {
                #ifdef INET6
			if(check_ip_type(req->local_ip_addr) == IP_TYPE_V6 && req->local_ip_addr[0] != '[' )//ipv6 url include the square brackets '[' and ']' 
				snprintf(hostUrl ,sizeof(hostUrl), "http://[%s]/cgi-bin/FirmwareUpgradeInfo.asp" , req->local_ip_addr);
			else
                                snprintf(hostUrl ,sizeof(hostUrl), "http://%s/cgi-bin/FirmwareUpgradeInfo.asp" , req->local_ip_addr);
	        #else		
	                snprintf(hostUrl ,sizeof(hostUrl), "http://%s/cgi-bin/FirmwareUpgradeInfo.asp" , req->local_ip_addr);
       	        #endif
			 /* In request url, the ipv6 ip address will be enclosed in the square brackets '[' and ']'. 
	 		And this square brackets should not be transferred. So we replace the "send_r_moved_temp" 
	 		function with the new one "send_r_moved_temp_new"*/						
			send_r_moved_temp_new(req , hostUrl , "");  
                        //send_r_moved_temp(req , hostUrl , "");
			return 1;
                }
		
		fd = open("/proc/tc3162/adsl_stats",O_RDONLY|O_NONBLOCK);
		if( fd <= 0 ){
			return;
		}
		ret = read(fd, adslState, 64);
		close(fd);
		if( ret <= 0 ){
			return;
		}
		if(strstr(adslState, "up"))
		{
			upStat = 1;
                }
                else
		{
			upStat = 0;
		}
                if(upStat == 0)
                {
                #ifdef INET6
			if(check_ip_type(req->local_ip_addr) == IP_TYPE_V6 && req->local_ip_addr[0] != '[' )//ipv6 url include the square brackets '[' and ']' 
				snprintf(hostUrl ,sizeof(hostUrl), "http://[%s]/cgi-bin/AdslDownInfo.asp" , req->local_ip_addr);
			else
	                        snprintf(hostUrl ,sizeof(hostUrl), "http://%s/cgi-bin/AdslDownInfo.asp" , req->local_ip_addr);
		#else		
	                snprintf(hostUrl ,sizeof(hostUrl), "http://%s/cgi-bin/AdslDownInfo.asp" , req->local_ip_addr);
       	        #endif
			 /* In request url, the ipv6 ip address will be enclosed in the square brackets '[' and ']'. 
	 		And this square brackets should not be transferred. So we replace the "send_r_moved_temp" 
	 		function with the new one "send_r_moved_temp_new"*/								
			send_r_moved_temp_new(req , hostUrl , "");  
                        //send_r_moved_temp(req , hostUrl , "");
                return 1;
                }
#endif
#endif
	
	
                //return 1;
        }
        return 0;
}
#endif
#ifdef INET6
int check_ip_type(char* ip) {
	struct sockaddr_in cvtaddr;
	struct sockaddr_in6 ipv6addr;

	if(inet_aton(ip,&cvtaddr.sin_addr)) {
		//ip address is ipv4
              	//fprintf(stderr,"\n==>check_ip_type, IPv4\n");
		return IP_TYPE_V4;
	}
	else if( inet_pton(AF_INET6, ip,(struct sockaddr *) &ipv6addr.sin6_addr) >0) {	
		//ip address is ipv6
              	//fprintf(stderr,"\n==>check_ip_type, IPv6\n");
		return IP_TYPE_V6;
	}

	return -1;
}

#endif


/*
 * Name: process_header_end
 *
 * Description: takes a request and performs some final checking before
 * init_cgi or init_get
 * Returns 0 for error or NPH, or 1 for success
 */

int process_header_end(request * req)
{
       char hostUrl[64] = "";

	   char forbidden[] = "0";
	   
	   int ret = 0;
#ifdef TCSUPPORT_MODEL_NAME_CUSKIT
	char productName[64] = {0};
	char strNodeName[32] = {0};
#endif


#ifdef WITHVOIP
	   int VoIPBusyFlag = 0; /* for ForbiddenWeb in VoIP */
#endif
        if (!req->logline) {
                send_r_error(req);
                return 0;
        }
                
        /* Percent-decode request */
        if (unescape_uri(req->request_uri, &(req->query_string)) == 0) {
                log_error_doc(req);
                fputs("Problem unescaping uri\n", stderr);
                send_r_bad_request(req);
                return 0;
        }

        /* clean pathname */
        clean_pathname(req->request_uri);
        
        if (req->request_uri[0] != '/') {
                send_r_bad_request(req);
                return 0;
        }
		
        if (translate_uri(req) == 0) { /* unescape, parse uri */
                SQUASH_KA(req);
                return 0;               /* failure, close down */
        }
        
         /* get method function */
        if(req->method == M_GET)
        {
                /* user login need add below this line */
                /* -------------------------------------------- */
#if defined(TCSUPPORT_WEB_INTERCEPTION) || defined(TCSUPPORT_REDIRECT_WLAN_WEB)
                /* interception the url get request */
                if(1 == InterceptionIpFromWan(req))
                {
                        return 0;
                }
#endif
#if !defined(TCSUPPORT_SELF_LOGIN_WINDOW) 
#if defined(TCSUPPORT_CD_NEW_GUI) || defined(TCSUPPORT_CT_C5_HEN_SFU)
                /* CD new Gui login window authorize function */
                if ((strstr(req->pathname,".asp")||strstr(req->pathname,".ASP"))
#if !defined(TCSUPPORT_CD_NEW_GUI) 
                && (strcmp(req->pathname,"/boaroot/cgi-bin/login.asp") != 0)
                && (strcmp(req->pathname,"/boaroot/cgi-bin/access_auth.asp") != 0)
                && (http_authorize(req) == -1))
#endif
                {
                        #ifdef BOA_DEBUG
                        fprintf(stderr,"send_r_unauthorized\n");
                        #endif
                        send_r_web_unauthorized(req);
                        return 0;
                }
                
                if(tcapi_set("Account_Entry0", "curUserName", cur_username) < 0)
                { 
                        return -1;
                }
#else
                 /* main trunk authorize function */
                //lee 2006-4-11:authorize
                if ( (0 != passURL(req->request_uri)) && (-1 == http_authorize(req)) )
                {
#if defined(TCSUPPORT_TRUE_LANDING_PAGE)
                        // can open this part if user have login fail times warning require
                        if(loginFailJump2url(req))
                                return 0;
#endif
                        #ifdef BOA_DEBUG
                        fprintf(stderr,"send_r_unauthorized\n");
                        #endif
#if !defined(TCSUPPORT_OI_C9) 
#ifdef TCSUPPORT_MODEL_NAME_CUSKIT
                       	sprintf(strNodeName,"%s_%s",SYS_INFO_NODE_NAME,SUB_NODE_NAME);
			ret = tcapi_get(strNodeName, SYS_INFO_PRODUCT_NAME, productName);
			if(ret < 0)
			{
			       	send_r_unauthorized(req,"Member Login");
			}
			else {
				send_r_unauthorized(req,productName);
			}
#else
			send_r_unauthorized(req,"Member Login");
#endif
#endif
                        return 0;
                }
#endif
#if defined(TCSUPPORT_TRUE_LANDING_PAGE)
                // can open this part if user have login fail times warning require
                // login success or visit the pass url
                clearLoginFailStatus(req);
#endif
#endif
        }

#ifndef PURE_BRIDGE
#if !defined(TCSUPPORT_SELF_LOGIN_WINDOW) 
		if(0!=strlen(cur_username))
        if(0 != pageAccessCheck(req))
#endif
        {
                send_r_forbidden(req);
                // the page is blocked
                return 0;
        }
#endif

         /* post method function */
        if (req->method == M_POST) {
#if defined(TCSUPPORT_TRUE_LANDING_PAGE)
                updateLoginFailStatusFlag(req);
#endif
#ifdef TRENDCHIP
#ifdef WITHVOIP

#if (VOIP == 1) /*MTK VoIP*/

                char voIPBusy[2][16];
                int tmpIdx = 0;
                if(!tcapi_get("VoIPBasic_Common","ForbiddenWeb",forbidden)){
                	for(tmpIdx = 0; tmpIdx < 2;tmpIdx++){
                    	if(!voipIdleCheck(tmpIdx)){
                    		strcpy(req->pathname, "/boaroot/cgi-bin/voip_busy.asp");
                    		VoIPBusyFlag = 1; /* if 1 pass through following work */
                    		break;
                    	}
                	}
                }
#else
                char voIPBusy[] = "0";
/*20110217 PTChen add for Forbidden Web functionality of VoIP */
/*if it match ForbiddenWeb & VoIPBusy all equal to 1, represent VoIP system is busy now. return a new web page to inform user VoIP is busy, and pass through following work*/
		if(!tcapi_get("VoIPBasic_Common","ForbiddenWeb",forbidden) && !tcapi_get("VoIPBasic_Common","VoIPBusy",voIPBusy)){
			if((0 == strcmp(voIPBusy, "1" )) && (0 == strcmp(forbidden, "1" ))){
				strcpy(req->pathname, "/boaroot/cgi-bin/voip_busy.asp");	
				VoIPBusyFlag = 1; /* if 1 pass through following work */
			}			
		}
/*20110217 PTChen add for Forbidden Web functionality of VoIP End*/
#endif /*VOIP*/
#endif /*WITHVOIP*/




#ifdef WITHVOIP
                // when upload the file to cpe
                if((1 == uploadDownLoadUrl(req)) && (VoIPBusyFlag != 1))
#else
		// when upload the file to cpe
		if(1 == uploadDownLoadUrl(req))
#endif
                {
#ifdef PURE_BRIDGE
                        releaseMemory();
#else
#ifdef TCSUPPORT_FW_UPGRADE_16M
#if 0
#if !defined(TCSUPPORT_CD_NEW_GUI)
			releaseMemory2();
#endif
#endif
			releaseMemory2(req);
#endif
#endif
#ifdef WITHVOIP
#if (VOIP == 1) /*MTK VoIP*/
			system("killall -9 sipclient");
#else
                        savingFileForVoipRestart();
#endif
#endif
                }               
#endif       
		req->post_data_fd = create_temporary_file(1, NULL, 0);
        if (req->post_data_fd == 0)
            return(0);
        return(1); /* success */
    }

    if (req->is_cgi) {
    	
        return init_cgi(req);
    }

    req->status = WRITE;
    return init_get(req);       /* get and head */
}

/*
 * Name: process_option_line
 *
 * Description: Parses the contents of req->header_line and takes
 * appropriate action.
 */

int process_option_line(request * req)
{
    char c, *value, *line = req->header_line;

    /* Start by aggressively hacking the in-place copy of the header line */

#ifdef FASCIST_LOGGING
    log_error_time();
    fprintf(stderr, "%s:%d - Parsing \"%s\"\n", __FILE__, __LINE__, line);
#endif
		
    value = strchr(line, ':');
    if (value == NULL)
        return 0;
    *value++ = '\0';            /* overwrite the : */
    to_upper(line);             /* header types are case-insensitive */
    while ((c = *value) && (c == ' ' || c == '\t'))
        value++;

    if (!memcmp(line, "IF_MODIFIED_SINCE", 18) && !req->if_modified_since)
        req->if_modified_since = value;

    else if (!memcmp(line, "CONTENT_TYPE", 13) && !req->content_type)
        req->content_type = value;

    else if (!memcmp(line, "CONTENT_LENGTH", 15) && !req->content_length)
        req->content_length = value;

    else if (!memcmp(line, "CONNECTION", 11) &&
             ka_max && req->keepalive != KA_STOPPED) {
        req->keepalive = (!strncasecmp(value, "Keep-Alive", 10) ?
                          KA_ACTIVE : KA_STOPPED);
    }
    /* #ifdef ACCEPT_ON */
    else if (!memcmp(line, "ACCEPT", 7))
        add_accept_header(req, value);
    /* #endif */

    /* Need agent and referer for logs */
    else if (!memcmp(line, "REFERER", 8)) {
        req->header_referer = value;
        if (!add_cgi_env(req, "REFERER", value, 1))
            return 0;
    } else if (!memcmp(line, "USER_AGENT", 11)) {
        req->header_user_agent = value;
        if (!add_cgi_env(req, "USER_AGENT", value, 1))
            return 0;
    }
    #if 0
    else if (!memcmp(line, "COOKIE", 6))//lee 2006-4-11
    {
    #endif
	/*
		krammer change for bug 1473.
		we use the SESSIONID in cookie to do the authentication.
		But opera brower will send cookie and cookie2 to our cpe,
		and the cookie2 will cover the SESSIONID. So opera can't 
		login our CPE because wrong SESSIONID.
		I do the following change to make cookie2 can't be stored into our CPE
	*/
    else if (!memcmp(line, "COOKIE", 6)&&memcmp(line, "COOKIE2", 7))//krammer 2008-12-4
    {
    		//fprintf(stderr,"line:%s\n",line);
    	int len = strlen(value)+1;
    	req->cookie = malloc(sizeof(char)*len);
    	memset(req->cookie,0,len);
    	memcpy(req->cookie,value,len-1);
    	if (!add_cgi_env(req, line, value, 1))
    		return 0;
    }
    else if (!memcmp(line, "AUTHORIZATION", 13))
    {
    	int len = strlen(value)+1;
    	req->authorize = malloc(sizeof(char)*len);
    	memset(req->authorize,0,len);
    	memcpy(req->authorize,value,len-1);
    	if (!add_cgi_env(req, line, value, 1))
    		return 0;
    }
#if defined(TCSUPPORT_TRUE_LANDING_PAGE)
    else if (!memcmp(line,"HOST",4))
    {
        req->hostname = value;
        if (!add_cgi_env(req, "HOST", value, 1))
            return 0;
    }
    else if(!memcmp(line,"ACCEPT_LANGUAGE",15))
    {
        req->accept_language = value;
    }
#endif
    else {
        if (!add_cgi_env(req, line, value, 1))
            return 0;
    }
    return 1;
}

/*
 * Name: add_accept_header
 * Description: Adds a mime_type to a requests accept char buffer
 *   silently ignore any that don't fit -
 *   shouldn't happen because of relative buffer sizes
 */

void add_accept_header(request * req, char *mime_type)
{
#ifdef ACCEPT_ON
    int l = strlen(req->accept);
    int l2 = strlen(mime_type);

    if ((l + l2 + 2) >= MAX_HEADER_LENGTH)
        return;

    if (req->accept[0] == '\0')
        strcpy(req->accept, mime_type);
    else {
        req->accept[l] = ',';
        req->accept[l + 1] = ' ';
        memcpy(req->accept + l + 2, mime_type, l2 + 1);
        /* the +1 is for the '\0' */
        /*
           sprintf(req->accept + l, ", %s", mime_type);
         */
    }
#endif
}

void free_requests(void)
{
    request *ptr, *next;

    ptr = request_free;
    while (ptr != NULL) {
        next = ptr->next;
        free(ptr);
        ptr = next;
    }
    request_free = NULL;
}


#if defined(TCSUPPORT_TRUE_LANDING_PAGE)
static int updateLoginFailStatusFlag(request *req)
{
        if(req->method == M_GET)
        {
                ;// do nothing
        }
        
        if(req->method == M_POST)
        {
                if(NULL == strstr(req->request_uri , "/cgi-bin/SavingAuthorize.asp") )
                {
                        return 0;
                }
        }

        checkLoginFailFlag++;
}

// can open this part if user have login fail times warning require
// if login fail times more the 3, jump to loginFail page to warning user
static int loginFailJump2url(request *req)
{
        char hostUrl[64] = "";
        memset(hostUrl , 0 , sizeof(hostUrl));

        
        if(checkLoginFailFlag  >= LOGINFAIL_NUM)
        {
                checkLoginFailFlag = 0;
        #ifdef INET6
                if(check_ip_type(req->local_ip_addr) == IP_TYPE_V6 && req->local_ip_addr[0] != '[' )//ipv6 url include the square brackets '[' and ']' 
                        snprintf(hostUrl ,sizeof(hostUrl), "http://[%s]/cgi-bin/loginFail.asp" , req->local_ip_addr);
                else{
#if defined(TCSUPPORT_TRUE_LANDING_PAGE) && defined(TCSUPPORT_C1_TRUE)
					if(req->hostname != NULL)
						snprintf(hostUrl ,sizeof(hostUrl), "http://%s/cgi-bin/loginFail.asp" , req->hostname);
					else
						 snprintf(hostUrl ,sizeof(hostUrl), "http://%s/cgi-bin/loginFail.asp" , req->local_ip_addr);
#else
                        snprintf(hostUrl ,sizeof(hostUrl), "http://%s/cgi-bin/loginFail.asp" , req->local_ip_addr);
#endif
				}
        #else           
#if defined(TCSUPPORT_TRUE_LANDING_PAGE) && defined(TCSUPPORT_C1_TRUE)
						if(req->hostname != NULL)
							snprintf(hostUrl ,sizeof(hostUrl), "http://%s/cgi-bin/loginFail.asp" , req->hostname);
						else
							snprintf(hostUrl ,sizeof(hostUrl), "http://%s/cgi-bin/loginFail.asp" , req->local_ip_addr);
#else
                snprintf(hostUrl ,sizeof(hostUrl), "http://%s/cgi-bin/loginFail.asp" , req->local_ip_addr);
#endif
        #endif
                 /* In request url, the ipv6 ip address will be enclosed in the square brackets '[' and ']'. 
                And this square brackets should not be transferred. So we replace the "send_r_moved_temp" 
                function with the new one "send_r_moved_temp_new"*/                                                             
                send_r_moved_temp_new(req , hostUrl , "");  
                //send_r_moved_temp(req , hostUrl , "");
                
                return 1;
        }
        else
        {
                updateLoginFailStatusFlag(req);
        }
        
        return 0;
}

// only visit the page need authorize then clear the login fail flag
static void clearLoginFailStatus(request *req)
{       
#if !defined(TCSUPPORT_C1_NEW_GUI) 
        if(0 == passURL(req->request_uri))
        {
                return;
        }
#endif
        checkLoginFailFlag = 0;
}

// only get the true admin (username/password)
static int isDefaultUsernamePassword(void)
{
        char username[129];
        char password[129];
        
        memset(password , 0 , sizeof(password));
        memset(username , 0 , sizeof(username));

        if( (tcapi_get("Account_Entry0", "username", username) < 0) || 
        (tcapi_get("Account_Entry0", "web_passwd", password) < 0))
                return 0;

        if( (0 == strcmp("admin", username)) && 
        (0 == strcmp("password", password)) )
        {                   
                return 1;
        }
        
        return 0;
}
#endif
#endif
