/*
 * Do some HTTP things.
 */
#include <linux/string.h>

#include <lwip/pbuf.h>
#include <lwip/tcp.h>

//void printtop(const char * str);

#define HTTP_LINE_END "\x0D\x0A"

/*
 * The fixed response to every GET.
 */
#if defined(TCSUPPORT_CT_BOOTLOADER_UPGRADE)
const char *get_response2 =
"HTTP/1.1 200 OK" HTTP_LINE_END
"Server: TCHTTPD/0.1" HTTP_LINE_END
"Content-Length: 868" HTTP_LINE_END
"Content-Type: text/html; charset=UTF-8" HTTP_LINE_END
HTTP_LINE_END
"<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01//EN\" \"http://www.w3.org/TR/html4/strict.dtd\">\n"//91
"<html>\n"//7
"<head>\n"//7
"<title>TC Rescue Page</title>\n"//30
"<script language=JavaScript>\n"//29_new
"function check(){\n"//18_new
"if(document.FW.username.value == '' || document.FW.passwd.value == ''){\n"//72
"alert(\"Please input the UserName and PassWord!\");\n"//50_new
"return false;\n"//14_new
"}\n"//2_new
"else\n"//5_new
"return true;\n"//13_new
"}\n"//2_new
"</script>\n"//10_new
"</head>\n"//8
"<body>\n"//7
"<FORM ENCTYPE=\"multipart/form-data\" METHOD=\"POST\" name=\"FW\">"//60
"<h1>User Login:</h1>\n"//21
"<table width=\"28%\" border=\"0\">\n"//31
"<tr>\n"//5
"<td width=\"36%\">UserName:</td>\n"//31
"<td width=\"64%\"><input name=\"username\" type=\"text\"></td>\n"//57
"</tr>\n"//6
"<tr>\n"//5
"<td>PassWord:</td>\n"//19
"<td><input type=\"password\" name=\"passwd\"></td>\n"//47
"</tr>\n"//6
"<tr>\n"//5
"<td align=\"right\"><input type=\"submit\" name=\"submit\" value=\"login\" onclick=\"return check();\"></td>\n"//99
"<td align=\"center\"><input type=\"reset\" name=\"Reset\" value=\"Reset\"></td>\n"//72
"</tr>\n"//6
"</table>\n"//9
"</FORM>\n"//8
"</body>\n"//8
"</html>\n";//8
#endif

const char *get_response = 
"HTTP/1.1 200 OK" HTTP_LINE_END
"Server: TCHTTPD/0.1" HTTP_LINE_END
"Content-Length: 691" HTTP_LINE_END
"Content-Type: text/html; charset=UTF-8" HTTP_LINE_END
HTTP_LINE_END
"<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01//EN\" \"http://www.w3.org/TR/html4/strict.dtd\">\n"//90
"<html>\n"//7
"<head>\n"//7
"<title>TC Rescue Page</title>\n"//30
"<script language=JavaScript>\n"//29_new
"function checkName(){\n"//22_new
"if(!(document.FW.FWupload.value.match(\"tcboot.bin\") || document.FW.FWupload.value.match(\"tclinux.bin\"))){\n"//106_new
"alert(\"Wrong File Name\");\n"//26_new
"return false;\n"//14_new
"}\n"//2_new
"else\n"//5_new
"return true;\n"//13_new
"}\n"//2_new
"</script>\n"//10_new
"</head>\n"//8
"<body>\n"//7
"<FORM ENCTYPE=\"multipart/form-data\" METHOD=\"POST\" name=\"FW\">"//60
"<h1>Upload firmware here:</h1>\n"//35
"<INPUT TYPE=\"FILE\" NAME=\"FWupload\" SIZE=\"20\" MAXLENGTH=\"128\" style=\"FONT-SIZE: 10pt\" contenteditable=\"false\">&nbsp;\n"//109+7
"<input type=\"submit\" name=\"Upload\" value=\"Upload\" onclick=\"return checkName();\">\n"//81  30_new
"</FORM>\n"//8
"</body>\n"//8
"</html>\n";//8

#if defined(TCSUPPORT_CT_BOOTLOADER_UPGRADE)
const char *get_response1 = 
"HTTP/1.1 200 OK" HTTP_LINE_END
"Server: TCHTTPD/0.1" HTTP_LINE_END
"Content-Length: 229" HTTP_LINE_END
"Content-Type: text/html; charset=UTF-8" HTTP_LINE_END
HTTP_LINE_END
"<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01//EN\" \"http://www.w3.org/TR/html4/strict.dtd\">\n"//90
"<html>\n"//7
"<head>\n"//7
"<title>TC Rescue Page</title>\n"//30
"</head>\n"//8
"<body>\n"//7
"<h1>Upload Successfully! System will restart in 2 minutes!</h1>\n"//64
"</body>\n"//8
"</html>\n";//8
#else
const char *get_response1 = 
"HTTP/1.1 200 OK" HTTP_LINE_END
"Server: TCHTTPD/0.1" HTTP_LINE_END
"Content-Length: 238" HTTP_LINE_END
"Content-Type: text/html; charset=UTF-8" HTTP_LINE_END
HTTP_LINE_END
"<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01//EN\" \"http://www.w3.org/TR/html4/strict.dtd\">\n"//90
"<html>\n"//7
"<head>\n"//7
"<title>TC Rescue Page</title>\n"//30
"</head>\n"//8
"<body>\n"//7
"<h1>Upload Successfully! Please Restart the System after 2 minutes!</h1>\n"//73
"</body>\n"//8
"</html>\n";//8
#endif

/*
 * Process stream data for HTTP requests.
 */
#define FOUND_NOUGHT 0
#define FOUND_G 1
#define FOUND_E 2
int process_http( struct pbuf *stream, struct tcp_pcb *outlet) {
  /* All this does is search for GET in the pbuf and if it finds it sends
   * out the fixed response.
   */

  struct pbuf *cur_pbuf;
  int sent = 0;
  int state = FOUND_NOUGHT;

#ifdef HTTP_DBG  
  prom_printf("process_http\r\n");
#endif  

  for ( cur_pbuf = stream; cur_pbuf != NULL && !sent; cur_pbuf = cur_pbuf->next) 
  {
	int i;
    /* find GET */
	for ( i = 0; i < cur_pbuf->len && !sent; i++) 
	{
		switch ( state) 
		{
			case FOUND_NOUGHT:
			if ( ((char *)cur_pbuf->payload)[i] == 'G')
			{
				state = FOUND_G;
			}
			break;

			case FOUND_G:
			if ( ((char *)cur_pbuf->payload)[i] == 'E') 
			{
				 state = FOUND_E;
			}
			else 
			{
				state = FOUND_NOUGHT;
			}
			break;

			case FOUND_E:
			if ( ((char *)cur_pbuf->payload)[i] == 'T')
			{
				sent = 1;
	  /* send the response
	   */
				#ifdef HTTP_DBG	  
	 			prom_printf("process_http: tcp_write\r\n");
				#endif
#if defined(TCSUPPORT_CT_BOOTLOADER_UPGRADE)
	 		 	if ( tcp_write( outlet, get_response2, strlen( get_response2), 1) != ERR_OK) 
#else
				if ( tcp_write( outlet, get_response, strlen( get_response), 1) != ERR_OK) 
#endif
			 	{
					#ifdef HTTP_DBG		  
	    				prom_printf( "Failed to send response");
					#endif		
	  		 	}	  
			}
			else 
			{
	  			state = FOUND_NOUGHT;
			}
			break;
		}
	}
  }

  return 0;
}

int afterUpload( struct pbuf *stream, struct tcp_pcb *outlet) {
#if defined(TCSUPPORT_CT_BOOTLOADER_UPGRADE)
	if ( tcp_write( outlet, get_response1, strlen(get_response1), 1) != ERR_OK) 
#else
	if ( tcp_write( outlet, get_response, strlen(get_response), 1) != ERR_OK) 
#endif
	{
	    prom_printf( "Failed to send response after firmware upload\r\n");
	}
	else
	{
		tcp_close(outlet);
	}
}

#if defined(TCSUPPORT_CT_BOOTLOADER_UPGRADE)
int afterAuthed( struct pbuf *stream, struct tcp_pcb *outlet) {
	if ( tcp_write( outlet, get_response, strlen(get_response), 1) != ERR_OK) {
	    prom_printf( "Failed to send response after Authed\r\n");
	}
}
int afterRefresh( struct pbuf *stream, struct tcp_pcb *outlet) {
	if ( tcp_write( outlet, get_response2, strlen(get_response2), 1) != ERR_OK) {
	    prom_printf( "Failed to send response after Refresh\r\n");
	}
}
#endif
