#include <stdio.h>
#include <string.h>
#ifdef TCSUPPORT_SYSLOG_ENHANCE
#include <syslog.h>
#endif

#include "mini-asp.h"
#include "grammar.h"
#include "http-get-utils.h"
#include "../boa.h"
#include "../../../lib/libtcapi.h"
#if defined(TCSUPPORT_CT)
#ifdef TCSUPPORT_SYSLOG
#include <syslog.h>
#endif
#endif

#if defined(TCSUPPORT_GENERAL_MULTILANGUAGE)
#include "parsers.h"
#endif

/*krammer add for bug 1321*/
#define WAN_PVC "Wan_PVC"
#define MAX_PVC_NUMBER 8
#define DEFAULT_RT_ATTR_LENGTH 4
#define DEFAULT_RT "DEFAULTROUTE"
#define TO_ASCII_OFFSET 48
#if defined(TCSUPPORT_CT_E8GUI)
#define MAX_BUFF_SIZE 1024
#endif

static void get_post_multipart(request *req);
static void get_post(request *req);
static void get_query(request *req);
static int http_header();

void init_asp_funcs(void);
#ifndef TRENDCHIP
void init_asp_constants (void);
#endif
/*transfrom the number to ASCII*/
int decode_uri(char *uri);
static char hex_to_decimal(char char1,char char2);

static void asp_Write (asp_reent* reent, const asp_text* params,  asp_text* ret);
static void request_Form (asp_reent* reent, const asp_text* params,  asp_text* ret);
static void tcWebApi_Set (asp_reent* reent, const asp_text* params,  asp_text* ret);
static void tcWebApi_Get (asp_reent* reent, const asp_text* params,  asp_text* ret);
static void tcWebApi_Unset (asp_reent* reent, const asp_text* params,  asp_text* ret);
static void tcWebApi_Commit (asp_reent* reent, const asp_text* params,  asp_text* ret);
static void tcWebApi_Save (asp_reent* reent, const asp_text* params,  asp_text* ret);
static void tcWebApi_CurrentDefaultRoute(asp_reent * reent, const asp_text * params, asp_text * ret);
#if defined(TCSUPPORT_IMPROVE_GUI_PERFM)
static void tcWebApi_constSet (asp_reent* reent, const asp_text* params,  asp_text* ret);
#endif
static void tcWebApi_staticGet (asp_reent* reent, const asp_text* params,  asp_text* ret);
static void tcWebApi_CommitWithoutSave (asp_reent* reent, const asp_text* params,  asp_text* ret);

#ifdef RA_PARENTALCONTROL
static void getClientMacAddr(char * ip_addr);
#endif/*RA_PARENTALCONTROL*/
#if defined(TCSUPPORT_WEBSERVER_SSL)
static request* temp_req = NULL;
#endif

int fd_out;
extern char cur_username[129];
#if defined(TCSUPPORT_WEB_SAVE)
extern int flag_save;
#endif

#if defined(TCSUPPORT_GENERAL_MULTILANGUAGE)
extern	int	getString(const char *pkey, char *value);
extern	int	initandparserfile(void);
extern	void	closefp(void);
extern	int	islangChanged(void);
#endif

#if defined(TCSUPPORT_CT_PON_C9)
int getMACfromNeigh(char *in_ipaddr, char *out_mac);
#endif

int asp_handler(request * req)
{
	int ret, i;
	char nodename[15], username[129];

	fprintf(stderr,"%s mothed:%d %s \n",__FUNCTION__,req->method,req->pathname);
	for(i = 0 ; i < 3; i++){
		sprintf(nodename, "Account_Entry%d", i);
		ret = tcapi_get(nodename, "username", username);
		if(ret < 0){ //Fail to acquire username from romfile
			fprintf(stderr, "Fail to acquire username from romfile: ret = %d\r\n", ret);
			return -1;
		}
		if(!strcmp(cur_username, username)){
			sprintf(nodename, "%d", i);
			ret = tcapi_set("WebCurSet_Entry", "CurrentAccess", nodename);
			if(ret < 0){ //Fail to set current username
				fprintf(stderr, "Fail to set current username: ret = %d\r\n", ret);
				return -1;
			}
			tcapi_get("WebCurSet_Entry", "CurrentAccess", username);
			break;
		}
	}
	

	g_var_post = NULL;
#if defined(TCSUPPORT_WEBSERVER_SSL)
	temp_req = req;
	dontDoAsp = 0;
#endif
	fd_out = req->fd;
	if(http_header() == -1)
	{
		close(fd_out);
		req->status = DEAD;
		req->keepalive = KA_INACTIVE;//Ian_20070326
		return 0;//Ian_20070326
	}
	if((req->method == M_POST) && req->content_length){
		if(req->content_type == NULL)
		{
			get_post(req);
		}
		/*add support to parse multipart. shnwind 2009.4.8*/
		else if(strstr(req->content_type,"multipart") == NULL){
			get_post(req);
		}else{
			get_post_multipart(req);
		}
	}else if((req->method == M_GET) && req->query_string)
		get_query(req);

//	init_asp_funcs ();
//    init_asp_constants();
#ifdef RA_PARENTALCONTROL
	/* Modify for taking CGI environment to ASP handler by richard.lai */
	do_asp (req, req->pathname); 
#else
#if defined(TCSUPPORT_CT_PON_C9)
	do_asp (req, req->pathname);
#else
	do_asp (NULL, req->pathname);
#endif
#endif/*RA_PARENTALCONTROL*/
#if defined(TCSUPPORT_WEB_SAVE)
		if(flag_save == 1){
			flag_save = 0;
			
			tcapi_save();
		}
#endif
	#ifdef TRENDCHIP
	if(dontDoAsp !=0){
		dontDoAsp=0;
	}
	#endif
	free_param_line(g_var_post);
	req->cgi_status = CGI_DONE;
	req->status = DONE;
	close(fd_out);
	fprintf(stderr,"%s END \n",__FUNCTION__);
	req->keepalive = KA_INACTIVE;//Ian_20070326
	return 0;//Ian_20070326
}

static void get_post_multipart(request *req){

	//char boundary[64];
	char boundary[256]; 
	char *c_ret, *post;
	int fd, content_len;
	struct stat buf_st;
	int i, len_bound;


	/*get boundary*/
	c_ret = strstr(req->content_type,"boundary=");
	if(c_ret != NULL){
		len_bound = strlen(c_ret);
		if(len_bound <= sizeof(boundary)){
			for(i = strlen("boundary=") ; i < len_bound ;i++){
				/*do not need "-------"*/
				if(c_ret[i] != '-'){
					break;
				}
			}
			strcpy(boundary,c_ret+i);
			//tcdbg_printf("bound = %s\n",boundary);
		}else{
			tcdbg_printf("boundary too long!!!upgrade fail\n");
			return;
		}
	}
	/*paser all item information*/
	multipart_parser(req, boundary);
	/*get all item information*/
	if(stat(MULTI_TO_NORMAL_TEMP, &buf_st) != 0){
		return ;
	}
	content_len = buf_st.st_size;
	post = (char*)malloc(content_len+1);
	fd = open(MULTI_TO_NORMAL_TEMP,O_RDONLY);
	if(fd < 0){
		free(post);
		return;
	}
	if(read(fd,post,content_len) <= 0){
		free(post);
		close(fd);
		return;
	}
	close(fd);
	g_var_post = parse_param_line (post);
	free(post);
	//move to cfg_manager
	//unlink(MULTI_TO_NORMAL_TEMP);
}


static void get_post(request *req)
{
	char *post;
	int ret,len;

	len = atoi(req->content_length);
	post = (char*)malloc(len+1);
	memset(post,0,len+1);

	lseek(req->post_data_fd, 0,SEEK_SET);
	ret = read(req->post_data_fd,post,len);
	if(ret<=0)
		goto exit;

	//fprintf(stderr,"post:%s %s\n",post,req->content_length);
	g_var_post = parse_param_line (post);
exit:
	#if 0  //lee 2006-11-27
	close(req->post_data_fd);
	req->post_data_fd = 0;
	#endif
	free(post);
}

static void get_query(request *req)
{
#if 0
	char *query;
	int len;

	len = strlen(req->query_string);
	query =(char*) malloc(len+1);
	memcpy(query,req->query_string,len);
	//fprintf(stderr,"query:%s %d\n",query,len);
	g_var_post = parse_param_line (query);
	free(query);
#endif
	g_var_post = parse_param_line (req->query_string);
}

static int http_header()
{
	char HTTP_OK[] = "HTTP/1.0 200 OK\r\n";
/*pork 20090309 added*/
#if 0
	char CONTENT_TYPE[] = "Content-type: text/html;charset=GB2312\r\n\r\n";
#else
	char CONTENT_TYPE[64];
	if(charset){
		sprintf(CONTENT_TYPE,"Content-type: text/html;charset=%s\r\n\r\n",charset);
	}
	else{
		/*default char set: ISO-8859-1*/
		sprintf(CONTENT_TYPE,"Content-type: text/html;charset=ISO-8859-1\r\n\r\n");
	}
#endif
/*pork 20090309 added over*/

	if(asp_send_response (NULL, HTTP_OK, strlen(HTTP_OK)) == -1)
		return -1;

	if(asp_send_response (NULL, CONTENT_TYPE, strlen(CONTENT_TYPE)) ==-1)
		return -1;

	return 0;
}
#ifndef TRENDCHIP
static void Request_Form(asp_reent* reent, int id, const asp_text* params,  asp_text* ret)
{
	char *val,*tmp;

	//fprintf(stderr, "(%s ID: %d ", __FUNCTION__, id);
    ret->str ="";
    ret->len = 0;
    if (params->str && params->len)
    {
    	//fwrite(params->str, params->len, 1, stderr);

    	tmp = (char*)asp_alloc(reent,params->len+1);
    	memset(tmp,0,params->len+1);
    	memcpy(tmp,params->str,params->len);
    	val = get_param(g_var_post,tmp);
    	if(val ==NULL)
    		return;

		ret->str = val;
		ret->len = strlen(val);
    }
    //else
    //	fprintf (stderr, "No Params )");
    //fprintf (stderr, "\n");

}

extern asp_reent *my_reent;
static void TcWebApi_set (asp_reent* reent, int id, const asp_text* params,  asp_text* ret)
{
	char *tmp;

    //fprintf(stderr, "(%s ID: %d ", __FUNCTION__, id);
    if (params->str && params->len)
    {
      //fwrite(params->str, params->len, 1, stderr);

      tmp = (char*)asp_alloc(reent,params->len+1);
      memset(tmp,0,params->len+1);
      memcpy(tmp,params->str,params->len);
      my_reent = reent;
      asp_TCWebApi_set(id,tmp);
    }
    //else
    //	fprintf (stderr, "No Params )");
    //fprintf (stderr, "\n");

}


static void TcWebApi_get (asp_reent* reent, int id, const asp_text* params,  asp_text* ret)
{
	char *val;

	#if 0
    fprintf(stderr, "(%s ID: %d ", __FUNCTION__, id);

    if (params->str && params->len)
      fwrite(params->str, params->len, 1, stderr);
    else
      fprintf (stderr, "No Params )");
    fprintf (stderr, "\n");
    #endif

    my_reent = reent;
    val = asp_TCWebApi_get(id);
    if(val == NULL)
    {
    	ret->str = "";
    	ret->len = 0;
    }
    else
    {
    	ret->str = val;
		ret->len = strlen(val);
    }

}

static void TcWebApi_execute (asp_reent* reent, int id, const asp_text* params,  asp_text* ret)
{

    //fprintf(stderr, "%s ID: %d \n", __FUNCTION__, id);
    my_reent = reent;
    asp_TCWebApi_execute(id);
}
#endif
void init_asp_funcs(void)
{
    #ifndef TRENDCHIP
    append_asp_func ("Request.Form", Request_Form);
    append_asp_func ("TcWebApi_set", TcWebApi_set);
    append_asp_func ("TcWebApi_get", TcWebApi_get);
    append_asp_func ("TcWebApi_execute", TcWebApi_execute);
    #else
    /*new tcWebApi*/
    append_asp_func ("asp_Write", asp_Write);
    append_asp_func ("request_Form", request_Form);
    append_asp_func ("tcWebApi_Set", tcWebApi_Set);
    append_asp_func ("tcWebApi_Get", tcWebApi_Get);
    append_asp_func ("tcWebApi_Unset", tcWebApi_Unset);
    append_asp_func ("tcWebApi_Commit", tcWebApi_Commit);
    append_asp_func ("tcWebApi_Save", tcWebApi_Save);
/*krammer add for bug 1321*/
    append_asp_func("tcWebApi_CurrentDefaultRoute", tcWebApi_CurrentDefaultRoute);
#if defined(TCSUPPORT_IMPROVE_GUI_PERFM)
    append_asp_func ("tcWebApi_constSet", tcWebApi_constSet);
#endif
    append_asp_func ("tcWebApi_staticGet", tcWebApi_staticGet);

    append_asp_func ("tcWebApi_CommitWithoutSave", tcWebApi_CommitWithoutSave);
    #endif
}

static void wait_fd (int fd)
{
	fd_set block_write_fdset,block_read_fdset;
	#ifdef TRENDCHIP
	struct timeval timeout={FAIL_SESSION_TIMEOUT,0};
	int ret=0;
	#endif

	while (1)
	{
		FD_ZERO(&block_write_fdset);
		FD_SET (fd, &block_write_fdset);
		FD_ZERO(&block_read_fdset);
		FD_SET (fd, &block_read_fdset);
	 	#ifdef TRENDCHIP
                /*
                   krammer change for bug 1094
                   we make select to have a timeout, if timeout, that means the session has some
                   thing wrong, so we break wait fd and dont do any asp until new session.
                */
            	if ((ret=select(fd + 1, &block_read_fdset,
                   &block_write_fdset, NULL,
                   &timeout)) <0 ) {
		#else
		if (select(fd + 1, &block_read_fdset,
                   &block_write_fdset, NULL,
                   0) <=0 ) {
              #endif
            /* what is the appropriate thing to do here on EBADF */
            if (errno == EINTR)
            {
                continue;   /* while(1) */
            }
            else if (errno != EBADF) {
                fprintf(stderr,"select error \n");
            }
        }
		#ifdef TRENDCHIP
		if(ret==0){/*timeout*/
			dontDoAsp=1;
			break;
		}
		#endif
		if (FD_ISSET(fd, &block_read_fdset))
			break;

		if (FD_ISSET(fd, &block_write_fdset))
			break;
	}

}


int asp_send_response (asp_reent* reent, const void* buffer, size_t len)
{

	int res=0;
	#ifdef TRENDCHIP
	 /*
           krammer add for bug 1094, this flag can block asp_send_response when
           wait_fd is timeout and change_ip flag is set.
        */
	if(dontDoAsp){
		return 0;
	}
	#endif

loop_write:

	len =len-res;
	buffer = buffer+res;

	wait_fd (fd_out);
#if defined(TCSUPPORT_WEBSERVER_SSL)
	if(temp_req->ssl == NULL)
#endif
	{
	res = write(fd_out,buffer,len);
	}
#if defined(TCSUPPORT_WEBSERVER_SSL)
	else{
		int retCode = 0; 
		if(len <=0)
			return 0; 
		res = boa_sslWrite(temp_req->ssl, buffer,len,&retCode);
		if(res<0)
		{
			dontDoAsp = 1;
		}
	}
#endif

  if(res <0 )
  {
  	return -1;
  }
  if(res<len)
  	goto loop_write;

  return 0;



}
#ifndef TRENDCHIP
void init_asp_constants (void)
{
  //sample append_asp_constant ("WAN_SETTING",					0);

}
#endif
#ifdef TRENDCHIP
/*____________________________________________________________________*
**	function name: asp_Write
**
**	description:
*     Output the string to web page.
*
**	parameters:
*     reent:not use
*     id:not use
*     params:params[0] will save the string user input
*     ret:not use
**	global:
*     none
**	return:
*     0:successful
*     -1:fail
**	call:
*     none
**	revision:
*     1.shnwind
* _______________________________________________________________*/

static void
asp_Write (asp_reent* reent, const asp_text* params,  asp_text* ret)
{

    asp_send_response (NULL,params[0].str,params[0].len);

}
/*____________________________________________________________________*
**	function name: request_Form
**
**	description:
*     get the value of item on page.
*
**	parameters:
*     reent:not use
*     id:not use
*     params:params[0] item name.
*     ret:not use
*
**	global:
*     none
**	return:
*     none
**	call:
*     none
**	revision:
*     1.shnwind
* _____________________________________________________________________*/
static void
request_Form (asp_reent* reent, const asp_text* params,  asp_text* ret)
{

    char *val,*tmp;

    /*ret->str = "");
    ret->len = 0;*/

    if (params[0].str && params[0].len)
    {
    	tmp = (char*)asp_alloc(reent,params[0].len+1);
    	memset(tmp,0,params[0].len+1);
    	memcpy(tmp,params[0].str,params[0].len);
    	val = get_param(g_var_post,tmp);

	//tcdbg_printf("request name %s value %s\n",tmp,val);
      if(val ==NULL)
    		return;
		decode_uri(val);
		ret->str = val;
		ret->len = strlen(val);
    }

}

/*[OSBNB00040562]ADD by peter.jiang@20141015, enable password encryption*/
#if defined(TCSUPPORT_CT_VOIP_CRYPT)
/*____________________________________________________________________*
**      function name: privSymmCrypt
**
**      description:
*       private symmetrical crypt.
*
**      parameters:
*     text: text input, may be encoded or decodec.
*     crypt_out: text out after crypting
*     
*     ret:crypt_out
*
**      global:
*     none
**      return:
*     none
**      call:
*     tcWebApi_set
**      revision:
* _____________________________________________________________________*/
static char*
privSymmCrypt (const char* text, char* crypt_out)
{
    char PRIVATE_KEY[2];
    int i = 0;
    int len = 0;

    if (text == NULL || strlen(text) == 0 || crypt_out == NULL) {
        tcdbg_printf ("[ASP] privSymmCrypt: invalid input\n");
        return NULL;
    }

    /*In order to make crypt_out characters printable(<0x80), private key should be <0x80,
    **and not be confict with text, e.g., 'Z'^'Z' = 0, this will truncate crypt_out,
    **set private keys as 0x12, 0x13(<0x20). 
    */
    PRIVATE_KEY[0] = 0x12;
    PRIVATE_KEY[1] = 0x13;

    len = strlen(text);
    if (len%2)
        len--;

    for (i = 0; i < len; i+=2) {
        crypt_out[i] = text[i] ^ PRIVATE_KEY[0];
        crypt_out[i+1] = text[i+1] ^ PRIVATE_KEY[1];
    }

    if (strlen(text)%2) { /*the last byte*/
        crypt_out[i] = text[i] ^ PRIVATE_KEY[0];
    }
/*
    tcdbg_printf("\n+++++++++++++++(ASP text)++++++++++++++++++\n");
    for (i = 0; i < strlen(text); i++)
        tcdbg_printf("%2x ", text[i]);
    tcdbg_printf("\n+++++++++++++(ASP crypt_out)+++++++++++++++\n");
    for (i = 0; i < strlen(crypt_out); i++)
        tcdbg_printf("%2x ", crypt_out[i]);
    tcdbg_printf("\n++++++++++++++++++++++++++++++++++++++++++\n");
*/
    return crypt_out;
}
#endif

/*____________________________________________________________________*
**	function name: tcWebApi _set
**
**	description:
*     write the value which is user input to cfg_manager.
*
**	parameters:
*     reent:not use
*     id:not use
*     params:params[0] node name.
*            params[1] attribute name.
*            params[2] value
*     ret:not use
*
**	global:
*     none
**	return:
*     none
**	call:
*     none
**	revision:
*     1.shnwind
* _____________________________________________________________________*/
static void
tcWebApi_Set (asp_reent* reent, const asp_text* params,  asp_text* ret)
{

    char *node,*attr,*value,*v_tmp;
    int r_val, i = 0;
	char *directSet[] = {"CurPage", ""};

    node = (char*)asp_alloc(reent,params[0].len+1);
    attr = (char*)asp_alloc(reent,params[1].len+1);
    value = (char*)asp_alloc(reent,params[2].len+1);
    memset(node,0,params[0].len+1);
    memset(attr,0,params[1].len+1);
    memset(value,0,params[2].len+1);
    memcpy(node,params[0].str,params[0].len);
    memcpy(attr,params[1].str,params[1].len);
    memcpy(value,params[2].str,params[2].len);
    node[params[0].len]='\0';
    attr[params[1].len]='\0';
    value[params[2].len]='\0';

    if(params[2].len == 0){
      return;
    }

    v_tmp = get_param(g_var_post,value);//get_param on web page
    //tcdbg_printf("set node = %s attr %s value %s\n",node,attr,v_tmp);
    if(v_tmp != NULL){
      decode_uri(v_tmp);

/*[OSBNB00040562]ADD by peter.jiang@20141015, enable password encryption*/
#if defined(TCSUPPORT_CT_VOIP_CRYPT)
    {
        char crypt[64] = {0};
        if (!strncmp (node, "VoIPBasic_Entry", strlen("VoIPBasic_Entry"))
          && !strncmp (attr, "SIPPassword", strlen("SIPPassword"))){
            privSymmCrypt(v_tmp, crypt);
            memcpy (v_tmp, crypt, strlen(v_tmp)+1);
        }
    }
#endif

      r_val=tcapi_set(node, attr,v_tmp);
    }
	else{//If the value is not the name of an attribute, regard it as the value to be assigned
		while(strlen(directSet[i]))
			if(!strcmp(attr, directSet[i++]))
				r_val=tcapi_set(node, attr, value);
	}
		

}
void strQuotConvertHTML(char *oriStr,char *desStr)  {
    int i;
    int j = 0;
    for(i = 0;i < strlen(oriStr);i++){
    	if(oriStr[i] == '"'){
    		strcpy(&(desStr[j]),"&quot;");
    		j+=6;
    	}
    	else{
    		desStr[j] = oriStr[i];
    		j++;
    	}
    }
    desStr[j] = '\0';
}
/*____________________________________________________________________*
**	function name: tcWebApi _get
**
**	description:
*     get the attribute value of specific node.
*
**	parameters:
*     reent:not use
*     id:not use
*     params:params[0] node name.
*            params[1] attribute name.
*            params[2] show or hide
*     ret:if params[2] == hide, use ret to return value.
*         if params[2] == show, use asp_send_response() to output string on page.
*
**	global:
*     none
**	return:
*     none
**	call:
*     none
**	revision:
*     1.shnwind
* _____________________________________________________________________*/
static void
tcWebApi_Get (asp_reent* reent, const asp_text* params,  asp_text* ret)
{
#if defined(TCSUPPORT_CT)
#if defined(TCSUPPORT_CT_E8GUI)
	static char val[MAX_BUFF_SIZE];
#else
	static char val[129];
#endif
	char *node,*attr,*show;
#if defined(TCSUPPORT_CT_E8GUI)
	static char retVal[MAX_BUFF_SIZE];
#else
	char retVal[129];
#endif
  int r_val;

    node = (char*)asp_alloc(reent,params[0].len+1);
    attr = (char*)asp_alloc(reent,params[1].len+1);
    show = (char*)asp_alloc(reent,params[2].len+1);
    memset(node,0,params[0].len+1);
    memset(attr,0,params[1].len+1);
    memset(show,0,params[2].len+1);
    memcpy(node,params[0].str,params[0].len);
    memcpy(attr,params[1].str,params[1].len);
    memcpy(show,params[2].str,params[2].len);
    node[params[0].len]='\0';
    attr[params[1].len]='\0';
    show[params[2].len]='\0';

    r_val = tcapi_get(node, attr, val);
    if(r_val < 0){
      strcpy(val,"N/A");
    }

#if defined(TCSUPPORT_CT_PON_C9)
	if( 0 == strcmp(node, "PC")
		&& 0 == strcmp(attr, "PCMAC") )
		getMACfromNeigh(((request *)reent->server_env)->remote_ip_addr, val);
#endif

/*[OSBNB00040562]ADD by peter.jiang@20141015, enable password encryption*/
#if defined(TCSUPPORT_CT_VOIP_CRYPT)
    {
        char crypt[64] = {0};
        if (!strncmp (node, "VoIPBasic_Entry", strlen("VoIPBasic_Entry"))
         && !strncmp (attr, "SIPPassword", strlen("SIPPassword"))){
            privSymmCrypt(val, crypt);
            memcpy (val, crypt, strlen(val)+1);
        }
    }
#endif

    if(!strcmp(show,"s")){//show -> s
#if defined(TCSUPPORT_CT_E8GUI)
    memset(retVal, 0, MAX_BUFF_SIZE);
#endif
    	strQuotConvertHTML(val,retVal);
      asp_send_response (NULL,retVal,strlen(retVal));
    }
    else if(!strcmp(show,"h")){//hide -> h
      if(strlen(val))
    	  ret->str = val;
        ret->len = strlen(val);
    }
#else
#if defined(TCSUPPORT_GUI_STRING_CONFIG) || defined(TCSUPPORT_GENERAL_MULTILANGUAGE)
#if defined(TCSUPPORT_GENERAL_MULTILANGUAGE)
	static char val[MAX_VALUE];
#else
	static char val[580];
#endif
#else
	static char val[129];
#endif
	char *node,*attr,*show;
#if defined(TCSUPPORT_GUI_STRING_CONFIG) || defined(TCSUPPORT_GENERAL_MULTILANGUAGE)
#if defined(TCSUPPORT_GENERAL_MULTILANGUAGE)
	static char retVal[MAX_STREAM];
#else
	char retVal[640];
#endif
#else
	//char retVal[129];
	char retVal[385];
#endif
	int r_val;

	node = (char*)asp_alloc(reent,params[0].len+1);
	attr = (char*)asp_alloc(reent,params[1].len+1);
	show = (char*)asp_alloc(reent,params[2].len+1);
	memset(node,0,params[0].len+1);
	memset(attr,0,params[1].len+1);
	memset(show,0,params[2].len+1);
	memcpy(node,params[0].str,params[0].len);
	memcpy(attr,params[1].str,params[1].len);
	memcpy(show,params[2].str,params[2].len);
	node[params[0].len]='\0';
	attr[params[1].len]='\0';
	show[params[2].len]='\0';
#ifdef RA_PARENTALCONTROL
	if(strcmp(node, "Parental") == 0 && strcmp(attr, "BrowserMAC") == 0){
		getClientMacAddr(reent->server_env->remote_ip_addr);
	} else {
#endif/*RA_PARENTALCONTROL*/

#if defined(TCSUPPORT_GENERAL_MULTILANGUAGE)
		if (!strcmp(node, "String_Entry"))
		{
			if(0 == getString(attr, val))
				strcpy(val, "N/A");
		}
		else {
#endif
		r_val = tcapi_get(node, attr, val);
		if(r_val < 0){
			strcpy(val,"N/A");
		}
#if defined(TCSUPPORT_GENERAL_MULTILANGUAGE)
		}
#endif
		if(!strcmp(show,"s")){//show -> s
#if defined(TCSUPPORT_GUI_STRING_CONFIG) || defined(TCSUPPORT_GENERAL_MULTILANGUAGE)
	if(strcmp(node,"String_Entry"))          //not from String_Entry
    	{    		
    		strQuotConvertHTML(val,retVal);
    	}
	else
	{
		memset(retVal,0,572);
		strcpy(retVal,val);
	}
#else
			strQuotConvertHTML(val,retVal);
#endif
			asp_send_response (NULL,retVal,strlen(retVal));
		}
		else if(!strcmp(show,"h")){//hide -> h
			if(strlen(val))
				ret->str = val;
			ret->len = strlen(val);
		}
#ifdef RA_PARENTALCONTROL
	}
#endif/*RA_PARENTALCONTROL*/
#endif
}
/*_____________________________________________________________________________*
**	function name: tcWebApi _unset
**
**	description:
*     delete the all attribute value of specific node.
*
**	parameters:
*     reent:not use
*     id:not use
*     params:params[0] node name.
*     ret:not use
*
**	global:
*     none
**	return:
*     0:successful
*     -1:fail
**	call:
*     none
**	revision:
*     1.shnwind
* ___________________________________________________________________________*/

static void
tcWebApi_Unset (asp_reent* reent, const asp_text* params,  asp_text* ret)
{

    char *node;
    int r_val;

    node = (char*)asp_alloc(reent,params[0].len+1);
    memset(node,0,params[0].len+1);
    memcpy(node,params[0].str,params[0].len);

    r_val=tcapi_unset(node);

}
/*___________________________________________________________________*
**	function name: tcWebApi_commit
**
**	description:
*     Write the value of specific attribute to file system.
*
**	parameters:
*     reent:not use
*     id:not use
*     params:params[0] node name.
*     ret:not use
*
**	global:
*     none
**	return:
*     0:successful
*     -1:fail
**	call:
*     none
**	revision:
*     1.shnwind
* __________________________________________________________________*/

static void
tcWebApi_Commit (asp_reent* reent, const asp_text* params,  asp_text* ret)
{
	  char *node;
    int r_val;
 #if defined(TCSUPPORT_WEB_SAVE)
    char bootType[16] = {0};
 #endif
#if defined(TCSUPPORT_CT)
#ifdef TCSUPPORT_SYSLOG	
	char loginfo_array[][2][32]=
	{
		{{"WanInfo"},{"Wan Configuration changed\n"}},
		{{"Ddns"},{"DDNS Configuration changed\n"}},
		{{""},{""}}
	};
	char logContent[32] = {0};
	int i=0;
#endif
#endif

    node = (char*)asp_alloc(reent,params[0].len+1);
    memset(node,0,params[0].len+1);
    memcpy(node,params[0].str,params[0].len);

#if defined(TCSUPPORT_WAN_ATM) || defined(TCSUPPORT_WAN_PTM) || defined(TCSUPPORT_WAN_ETHER)
//when someone uses webpage to commit wan, let CC have one more chance to do WAN_STOP or WAN_START for ATM/PTM.
	if(strstr(node,"Wan_PVC")){
		system("echo 1 > /tmp/commitByWeb");
	}
#endif

    r_val=tcapi_commit(node);
    //tcdbg_printf("commit ret = %d node %s\n",r_val,node);

#if defined(TCSUPPORT_GENERAL_MULTILANGUAGE)
	if (!strcmp(node, "LanguageSwitch_Entry"))
	{
		if (1 == islangChanged()) {
			initandparserfile();
		}
	}
#endif

#if defined(TCSUPPORT_WEB_SAVE)
    flag_save = 1;
    if(strcmp(node, SYSTEM_NODE) == 0) {
	tcapi_get(SYSTEM_NODE, REBOOT_TYPE, bootType);
	if(atoi(bootType) != NO_BOOT)
	     flag_save = 0;//If do system reboot or reset default, should not do save
    }
#endif

#ifdef TCSUPPORT_SYSLOG_ENHANCE
   	openlog("TCSysLog WEB", 0, LOG_LOCAL1);
   	syslog(LOG_INFO, "Configuration changed\n");
	closelog();
#endif
#if defined(TCSUPPORT_CT)
#ifdef TCSUPPORT_SYSLOG	
   	openlog("TCSysLog WEB", 0, LOG_LOCAL1);
	strcpy(logContent,"Configuration changed\n");
	for(i=0; strlen(loginfo_array[i][0])!=0; i++){
		if(strstr(node,loginfo_array[i][0])){
			strcpy(logContent,loginfo_array[i][1]);
			break;
		}
	}
   	syslog(LOG_INFO, logContent);
	closelog();
#endif
#endif
}

static void
tcWebApi_CommitWithoutSave (asp_reent* reent, const asp_text* params,  asp_text* ret)
{
	  char *node;
    int r_val;

    node = (char*)asp_alloc(reent,params[0].len+1);
    memset(node,0,params[0].len+1);
    memcpy(node,params[0].str,params[0].len);

    r_val=tcapi_commit(node);
    //tcdbg_printf("commit ret = %d node %s\n",r_val,node);

}

/*____________________________________________________________________*
**	function name: tcWebApi _save
**
**	description:
*     Write all system parameters to flash
*
**	parameters:
*     reent:not use
*     id:not use
*     params:not use
*     ret:not use
*
**	global:
*     none
**	return:
*     none
**	call:
*     none
**	revision:
*     1.shnwind
* _____________________________________________________________________*/

static void
tcWebApi_Save (asp_reent* reent, const asp_text* params,  asp_text* ret)
{
#if defined(TCSUPPORT_C7) || defined(TCSUPPORT_CT_PROLINE_SUPPORT) || defined(TCSUPPORT_CT_PON_SK)
	char resetflag[8];
	tcapi_get("SysInfo_Entry", "ResetFlag", resetflag);
	if(!strcmp(resetflag, "0")){
	    tcapi_set("SysInfo_Entry", "ResetFlag", "1");
	    tcapi_commit("SysInfo");
        }
#endif
#if defined(TCSUPPORT_CT_SIMCARD_SEPARATION)
	char cardExist[16] = {0};
	char simAuthRet[16] = {0};
	char CurrentAccess[16] = {0};

	//only do write sim file when adminstrator login
	if( tcapi_get("WebCurSet_Entry", "CurrentAccess",CurrentAccess) == 0 && strcmp(CurrentAccess,"0") == 0 
		&& tcapi_get("SimCard_Entry", "cardExist",cardExist) == 0 && strcmp(cardExist,"1") == 0 
		&& tcapi_get("SimCard_Entry", "simAuthRet",simAuthRet) == 0 && strcmp(simAuthRet,"1") == 0 ){
		tcapi_set("SimCard_Entry","needWriteSim","1");
	}
#endif

#if defined(TCSUPPORT_WEB_SAVE)
	flag_save = 0;
#endif
    tcapi_save();
}

static char
hex_to_decimal(char char1,char char2)
{
	return (((char1 >= 'A') ? (((char1 & 0xdf) - 'A') + 10) : (char1 - '0')) * 16) + (((char2 >= 'A') ? (((char2 & 0xdf) - 'A') + 10) : (char2 - '0')));
}

int
decode_uri(char *uri)
{
	char c, d;
  char *uri_old;
  int count = 0;

  uri_old = uri;
  while ((c = *uri_old)) {
    if(count++ == 1024)
	{/*max run 128 cycle*/
      return 0;
    }
    if (c == '%') {
        uri_old++;
        if ((c = *uri_old++) && (d = *uri_old++))
          *uri++ = hex_to_decimal(c, d);
        else
          return 0;       /* NULL in chars to be decoded */
    }else{
			*uri++ = c;
      uri_old++;
    }
  }
  *uri = '\0';
  return 1;
}

/*krammer add for bug 1321*/
/*____________________________________________________________________*
**	function name: tcWebApi_CurrentDefaultRoute
**
**	description:
*     get the current route pvc number
*
**	parameters:
*     reent:not use
*     id:not use
*     params:not use
*     ret:not use
*
**	global:
*     none
**	return:
*     current default route pvc number
**	call:tcapi_get, asp_send_response
*     none
**	revision:
*     1.krammer
* _____________________________________________________________________*/

static void
tcWebApi_CurrentDefaultRoute(asp_reent* reent, const asp_text* params,  asp_text* ret)
{
	char i, defaultRT;
	int r_val;
	char val[DEFAULT_RT_ATTR_LENGTH]={0};
	char wanPvc[16]={0};

	for(i=0; i<MAX_PVC_NUMBER; i++){
		sprintf(wanPvc,"%s%d",WAN_PVC,i);
		r_val=tcapi_get(wanPvc, DEFAULT_RT, val);
		if(r_val<0){
			continue;
		}
		if(!strcmp(val, "Yes")){
			defaultRT=i+TO_ASCII_OFFSET;
			asp_send_response (NULL,&defaultRT,1);
			return;
		}
	}
	/*can't fine default route, we use pvc 0 to be the default route*/
	defaultRT=0+TO_ASCII_OFFSET;
	asp_send_response (NULL,&defaultRT,1);
}

#endif

#if defined(TCSUPPORT_IMPROVE_GUI_PERFM)
/*____________________________________________________________________*
**      function name: tcWebApi_constSet
**
**      description:
*     write the const value which is user input to cfg_manager.
*
**      parameters:
*     reent:not use
*     id:not use
*     params:params[0] node name.
*            params[1] attribute name.
*            params[2] value
*     ret:not use
*
**      global:
*     none
**      return:
*     none
**      call:
*     none
**      revision:
*     1.shnwind
* _____________________________________________________________________*/
static void
tcWebApi_constSet (asp_reent* reent, const asp_text* params,  asp_text* ret)
{
        char *node,*attr,*value;
        int r_val;
        
        node = (char*)asp_alloc(reent,params[0].len+1);
        attr = (char*)asp_alloc(reent,params[1].len+1);
        value = (char*)asp_alloc(reent,params[2].len+1);
        memset(node,0,params[0].len+1);
        memset(attr,0,params[1].len+1);
        memset(value,0,params[2].len+1);
        memcpy(node,params[0].str,params[0].len);
        memcpy(attr,params[1].str,params[1].len);
        memcpy(value,params[2].str,params[2].len);
        node[params[0].len]='\0';
        attr[params[1].len]='\0';
        value[params[2].len]='\0';

        // can alow set NULL
	/*
        if(params[2].len == 0)
        {
                return;
        }
	*/
        
        r_val=tcapi_set(node, attr, value);
}
#endif
/*____________________________________________________________________*
**      function name: tcWebApi_staticGet
**
**      description:
*     get the attribute value of specific node.
*
**      parameters:
*     reent:not use
*     id:not use
*     params:params[0] node name.
*            params[1] attribute name.
*            params[2] show or hide
*     ret:if params[2] == hide, use ret to return value.
*         if params[2] == show, use asp_send_response() to output string on page.
*
**      global:
*     none
**      return:
*     none
**      call:
*     none
**      revision:
*     1.shnwind
* _____________________________________________________________________*/
static void
tcWebApi_staticGet (asp_reent* reent, const asp_text* params,  asp_text* ret)
{
        static char val[129];
        char *node,*attr,*show;
        char retVal[129];
        int r_val;
        
        node = (char*)asp_alloc(reent,params[0].len+1);
        attr = (char*)asp_alloc(reent,params[1].len+1);
        show = (char*)asp_alloc(reent,params[2].len+1);
        memset(node,0,params[0].len+1);
        memset(attr,0,params[1].len+1);
        memset(show,0,params[2].len+1);
        memcpy(node,params[0].str,params[0].len);
        memcpy(attr,params[1].str,params[1].len);
        memcpy(show,params[2].str,params[2].len);
        node[params[0].len]='\0';
        attr[params[1].len]='\0';
        show[params[2].len]='\0';
        r_val = tcapi_staticGet(node, attr, val);
        if(r_val < 0)
        {
                strcpy(val,"N/A");
        }
        
        //show -> s
        if(!strcmp(show,"s"))
        {
                strQuotConvertHTML(val,retVal);
                asp_send_response (NULL,retVal,strlen(retVal));
        }
        //hide -> h
        else if(!strcmp(show,"h"))
        {
                if(strlen(val))
                        ret->str = val;
                ret->len = strlen(val);
        }

}


#ifdef RA_PARENTALCONTROL
/* Add for Time Restrict by richard.lai */
/*____________________________________________________________________*
**	function name: getClientMacAddr
**
**	description:
*     Get Client Mac Address.
*
**	parameters:
*     reent:not use
*     id:not use
*     params:not use
*     ret:not use
*
**	global:
*     none
**	return:
*     none
**	call:
*     none
**	revision:
*     1.shnwind
* _____________________________________________________________________*/
#define PROC_ARP "/proc/net/arp"
static void
getClientMacAddr (char * ip_addr) {

	static char val[129];
	char retVal[129];
	FILE *fp;
	char buf[80];
	char ipaddr[16], hwtype[16], flags[16], hwaddr[20], mask[16], device[16];

	fp = fopen(PROC_ARP, "r");
	if(fp) {
		fgets(buf, 80, fp);
		while(fscanf(fp, "%s %s %s %s %s %s\n", ipaddr, hwtype, flags, hwaddr, mask, device) > 3) {
			if(strcmp(ipaddr, ip_addr) == 0) {
				strcpy(val, hwaddr);
				strQuotConvertHTML(val, retVal);
				asp_send_response (NULL, retVal, strlen(retVal));
			}
		}
		fclose(fp);
	}
}
#endif/*RA_PARENTALCONTROL*/

#if defined(TCSUPPORT_CT_PON_C9)
#define IP_NEIGH_TEMP_PATH		"/tmp/ip_neigh"
int getMACfromNeigh(char *in_ipaddr, char *out_mac)
{
	char cmdbuf[128] = {0};
	char buf[120] = {0};
	char ipaddr[64] = {0}, devs[16]={0}, devvalue[16]={0}, addype[16]={0}, hwaddr[20] = {0}, state[16] = {0};
	FILE *fp = NULL;

	if ( !in_ipaddr || !out_mac )
		return -1;

	memset(cmdbuf, 0, sizeof(cmdbuf));
	sprintf(cmdbuf, "/usr/bin/ip neigh > %s", IP_NEIGH_TEMP_PATH);
	system(cmdbuf);

	fp = fopen(IP_NEIGH_TEMP_PATH, "r");
	if ( !fp )
		return -2;

	while ( NULL != fgets(buf, sizeof(buf), fp) )
	{
		sscanf(buf, "%s %s %s %s %s %s\n", ipaddr, devs, devvalue, addype, hwaddr, state);
		if( strcmp(ipaddr, in_ipaddr) == 0)
		{
			strcpy(out_mac, hwaddr);
			if( strcmp("REACHABLE", state) == 0)
				break;
		}
	}

	fclose(fp);
	unlink(IP_NEIGH_TEMP_PATH);

	return 0;
}
#endif

