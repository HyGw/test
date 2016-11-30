/*
 *  Boa, an http server
 *  Copyright (C) 1995 Paul Phillips <paulp@go2net.com>
 *  Some changes Copyright (C) 1996,97 Larry Doolittle <ldoolitt@jlab.org>
 *  Some changes Copyright (C) 1997 Jon Nelson <jnelson@boa.org>
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

/* $Id: //BBN_Linux/Branch/Branch_for_Rel_CMCC_7526_20161014/tclinux_phoenix/apps/public/boa-asp/src/globals.h#1 $*/

#ifndef _GLOBALS_H
#define _GLOBALS_H

#if defined(TCSUPPORT_WEBSERVER_SSL)
#include "sslSocket.h"
#endif
struct mmap_entry {
    dev_t dev;
    ino_t ino;
    char *mmap;
    int use_count;
    size_t len;
};

struct alias {
    char *fakename;             /* URI path to file */
    char *realname;             /* Actual path to file */
    int type;                   /* ALIAS, SCRIPTALIAS, REDIRECT */
    int fake_len;               /* strlen of fakename */
    int real_len;               /* strlen of realname */
    struct alias *next;
};

typedef struct alias alias;

struct request {                /* pending requests */
    int fd;                     /* client's socket fd */
    int status;                 /* see #defines.h */
    time_t time_last;           /* time of last succ. op. */
    char *pathname;             /* pathname of requested file */
    int simple;                 /* simple request? */
    int keepalive;              /* keepalive status */
    int kacount;                /* keepalive count */

    int data_fd;                /* fd of data */
    unsigned long filesize;     /* filesize */
    unsigned long filepos;      /* position in file */
    char *data_mem;             /* mmapped/malloced char array */
    int method;                 /* M_GET, M_POST, etc. */

    char *logline;              /* line to log file */

    char *header_line;          /* beginning of un or incompletely processed header line */
    char *header_end;           /* last known end of header, or end of processed data */
    int parse_pos;              /* how much have we parsed */
    int client_stream_pos;      /* how much have we read... */

    int buffer_start;           /* where the buffer starts */
    int buffer_end;             /* where the buffer ends */

    char *http_version;         /* HTTP/?.? of req */
    int response_status;        /* R_NOT_FOUND etc. */

    char *if_modified_since;    /* If-Modified-Since */
    time_t last_modified;       /* Last-modified: */

    char local_ip_addr[NI_MAXHOST]; /* for virtualhost */

    /* CGI vars */

    int remote_port;            /* could be used for ident */

    char remote_ip_addr[NI_MAXHOST]; /* after inet_ntoa */

    int is_cgi;                 /* true if CGI/NPH */
    int cgi_status;
    int cgi_env_index;          /* index into array */

    /* Agent and referer for logfiles */
    char *header_user_agent;
    char *header_referer;

    int post_data_fd;           /* fd for post data tmpfile */

    char *path_info;            /* env variable */
    char *path_translated;      /* env variable */
    char *script_name;          /* env variable */
    char *query_string;         /* env variable */
    char *content_type;         /* env variable */
    char *content_length;       /* env variable */

    struct mmap_entry *mmap_entry_var;

    struct request *next;       /* next */
    struct request *prev;       /* previous */

#if defined(TCSUPPORT_WEBSERVER_SSL)
	sslConn_t* ssl;
#endif
    /* everything below this line is kept regardless */
    char buffer[BUFFER_SIZE + 1]; /* generic I/O buffer */
    char request_uri[MAX_HEADER_LENGTH + 1]; /* uri */
    char client_stream[CLIENT_STREAM_SIZE]; /* data from client - fit or be hosed */
    char *cgi_env[CGI_ENV_MAX + 4];             /* CGI environment */

#ifdef ACCEPT_ON
    char accept[MAX_ACCEPT_LENGTH]; /* Accept: fields */
#endif
	 char *cookie;//lee 2006-4-11
	 char *authorize;
	 char usr_name[128];
	 char pwd[128];
	 char access_level[128];
	 char uid[128];
	 char gid[128];
      #ifndef TRENDCHIP
	 char web_api_addr[32];
	 char val_def_addr[32];
      #endif
#if defined(TCSUPPORT_TRUE_LANDING_PAGE)
    char *hostname;             /* HOST variable */
	char *accept_language;		/* Accept-Language */
#endif
#if defined(TCSUPPORT_CT_E8GUI)
     unsigned char validsession;
#endif
#if defined(CT_COM_DEVICEREG)
	unsigned char iswan_redirect;
#endif
#if defined(TCSUPPORT_CT_SIMCARD_SEPARATION)
	unsigned char isSimCard_redirect;
#endif
#if defined(TCSUPPORT_CT_JOYME)
	unsigned char ispushview_upg;
#endif

};

typedef struct request request;

struct status {
    long requests;
    long errors;
};

extern struct status status;

extern char *optarg;            /* For getopt */
extern FILE *yyin;              /* yacc input */

extern request *request_ready;  /* first in ready list */
extern request *request_block;  /* first in blocked list */
extern request *request_free;   /* first in free list */

extern fd_set block_read_fdset; /* fds blocked on read */
extern fd_set block_write_fdset; /* fds blocked on write */

/* global server variables */

extern char *access_log_name;
extern char *error_log_name;
extern char *cgi_log_name;
extern int cgi_log_fd;
extern int use_localtime;
/*pork 20090309 added*/
extern char *charset;
/*pork 20090309 added over*/
extern int server_port;
extern uid_t server_uid;
extern gid_t server_gid;
extern char *server_admin;
extern char *server_root;
extern char *server_name;
extern char *server_ip;
extern int max_fd;
extern int devnullfd;

extern char *document_root;
extern char *user_dir;
extern char *directory_index;
extern char *default_type;
extern char *dirmaker;
extern char *mime_types;
extern char *cachedir;

extern char *tempdir;

extern char *cgi_path;
extern int single_post_limit;

extern int ka_timeout;
extern int ka_max;

#ifdef TRENDCHIP
//krammer add
extern int dontDoAsp;
#endif

extern int sighup_flag;
extern int sigchld_flag;
extern int sigalrm_flag;
extern int sigterm_flag;
extern time_t start_time;

extern int pending_requests;
extern long int max_connections;

extern int verbose_cgi_logs;

extern int backlog;
extern time_t current_time;

extern int virtualhost;

extern int total_connections;

extern sigjmp_buf env;
extern int handle_sigbus;

#endif
