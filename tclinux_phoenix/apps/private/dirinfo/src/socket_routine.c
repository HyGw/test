#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "function.h"
#include <netinet/in.h>
#include <json.h>
#include "dirfile.h"
#include "threadpool.h"
#include <iconv.h>
#include <libtcapi.h> 


list_ctl_t glist_ctl;
#define PORT    3458
#define MOBILE_ENTRY "mobile_Entry"
#define NASDBG		 "nasdbg"


bool charsetchange(char *srcchr, char *deschr, int *inlen, int *outlen, char *src, char *des) {
	bool flag = true;
	
	iconv_t  cd = iconv_open(deschr, srcchr);
	if(cd < 0) {
		print(DIR_ERR, "%s====>%s/%d iconv_open fail\n", __FILE__, __FUNCTION__, __LINE__);
		flag = false;
		goto charsetchangeexit;
	}
    if((iconv(cd, &src, inlen, &des, outlen)<0) || (*inlen!=0)) {
		print(DIR_ERR, "%s====>%s/%d iconv fail!\n", __FILE__, __FUNCTION__, __LINE__);
		flag = false;
		goto charsetchangefail;
	}
charsetchangefail:
	iconv_close(cd);
charsetchangeexit:
	return flag;
}

int init_nas(void) {
	int res = 0;
	char buffer[255] = {0};
	glist_ctl.flags = 0;
	res = pthread_mutex_init(&glist_ctl.sema, NULL);
	res += pthread_cond_init(&glist_ctl.con, NULL);
	if(!tcapi_get(MOBILE_ENTRY,  NASDBG,  buffer)){
		ggLevel = atoi(buffer);
		if(ggLevel < DIR_ERR || ggLevel > DIR_WRN) ggLevel = DIR_ERR;
	}
	return res;
}

void sock_routine(pwork_data_t work) {
	int c_fd = work->fd;
	struct timeval timeout;
	char buf[1024] = {0}, *recCmdType = NULL, *ret = NULL, tmpbuf[1024] = {0};
	int i = 0,res = 0, rest = 0;;
	json_t *root = NULL;
	char *charset = NULL;
	char *tmpret = NULL;

	timeout.tv_sec = 1;
	timeout.tv_usec = 0;
	res += setsockopt(c_fd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
	res += setsockopt(c_fd, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(timeout));
    
	if((res<0) || read(c_fd, buf, sizeof(buf)) < 0) {
			print(DIR_ERR, "%s====>read fail! %d\n", __FUNCTION__, __LINE__);
			shutdown(c_fd, SHUT_RDWR);
			close(c_fd);
			return;
	}
	print(DIR_WRN, "%s====> read buf = %s\n", __FUNCTION__, buf);
	if(!(recCmdType= strstr(buf, "\"CmdType\":"))) {
		print(DIR_ERR, "%s===>read format error! %d\n", __FUNCTION__, __LINE__);
		shutdown(c_fd, SHUT_RDWR);
		close(c_fd);
		return;
	}
	recCmdType += strlen("\"CmdType\":");
	print(DIR_WRN, "%s====> 1 read buf = %s\n", __FUNCTION__, buf);
	if(JSON_OK != json_parse_document(&root, buf)) {
		print(DIR_ERR, "%s====>%s/%d json parse er!\n", __FILE__, __FUNCTION__, __LINE__);
		shutdown(c_fd, SHUT_RDWR);
		close(c_fd);
		return;
	}
	print(DIR_WRN, "%s====> 2 read buf = %s\n", __FUNCTION__, buf);
	charset = json_find_value(root, "Charset");
	if(charset && strcmp(charset, UTF8)) {     //need change charset
	    int inlen = strlen(buf);
		int outlen = sizeof(tmpbuf);
		//char *pbuf = buf, *ptmpbuf = tmpbuf;
		
		if(!charsetchange(charset, UTF8, &inlen, &outlen, buf, tmpbuf)) {
			print(DIR_ERR, "%s====>%s/%d charsetchange err!\n", __FILE__, __FUNCTION__, __LINE__);
			shutdown(c_fd, SHUT_RDWR);
			close(c_fd);
			return;
		}	
		strcpy(buf, tmpbuf);
	}
	print(DIR_WRN, "%s====> 3 read buf = %s\n", __FUNCTION__, buf);
	for(;cmd[i].cmd != NULL; i++) {
		if(!strncmp(recCmdType, cmd[i].cmd, strlen(cmd[i].cmd))) {
			ret = cmd[i].func(buf);
			break;
		}
	}
	if(!ret) {
		print(DIR_ERR, "%s====> unsupport cmd! needn't return %d\n", __FUNCTION__, __LINE__);
		shutdown(c_fd, SHUT_RDWR);
		json_free_value(&root);
		close(c_fd);
		return;
	}
	if(charset && strcmp(charset, UTF8)) {     //need change charset
	    int inlen = strlen(ret);
		int outlen = inlen*2;
		tmpret = malloc(outlen);

		if(!tmpret) {
			print(DIR_ERR, "%s====>%s/%d alloc mem err!\n", __FILE__, __FUNCTION__, __LINE__);
			shutdown(c_fd, SHUT_RDWR);
			close(c_fd);
			return;

		}
		if(!charsetchange(UTF8, charset, &inlen, &outlen, ret, tmpret)) {
			print(DIR_ERR, "%s====>%s/%d charsetchange err!\n", __FILE__, __FUNCTION__, __LINE__);
			shutdown(c_fd, SHUT_RDWR);
			close(c_fd);
			free(tmpret);
			return;
		}	
		free(ret);
		ret = tmpret;
	}
	print(DIR_WRN, "%s====>write before!\n", __FUNCTION__);
	print(DIR_INF, "%s===>send %s\n", __FUNCTION__, (strlen(ret)>1900) ? ret+strlen(ret)-1900: ret);
	rest = strlen(ret);
	while((res=write(c_fd, ret+strlen(ret)-rest, rest)) > 0) {
		rest -= res;
	}
	print(DIR_WRN, "%s====>write after! write real send %d of total %d \n", __FUNCTION__, res, strlen(ret));
	free(ret);
	json_free_value(&root);
	shutdown(c_fd, SHUT_RDWR);
	close(c_fd);

}
int main() {

	int res = 0, len = sizeof(struct sockaddr_in);
	int fd = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in s_addr, c_addr;
	int bReuse = 1;

	init_nas();
	s_addr.sin_family = AF_INET;
	s_addr.sin_port   = htons(PORT);
	s_addr.sin_addr.s_addr = INADDR_ANY;
	if(fd < 0) {
		print(DIR_ERR, "%s====>socket fail! %d\n", __FUNCTION__, __LINE__);
		return -1;
	}
	if(setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &bReuse, sizeof(bReuse))!=0) {
		print(DIR_ERR, "%s====>setsockopt fail! %d\n", __FUNCTION__, __LINE__);
	}
	res += bind(fd, (struct sockaddr*)&s_addr, len); 
	res += listen(fd, 5);
	if(res != 0) {
		print(DIR_ERR, "%s====>bind fail! %d\n", __FUNCTION__, __LINE__);
		close(fd);
		return -1;
	}
	threadpool_init(&gPool_sock, 3, threadpoolsock);
	while(1) {
		
		int c_fd; 
		pwork_data_t work = malloc(sizeof(work_data_t));
		if(!work) {
			print(DIR_ERR, "%s====>%s/%d alloc mem err!\n", __FILE__, __FUNCTION__, __LINE__);
			continue;
		}
		if((c_fd = accept(fd, (struct sockaddr*)&c_addr, &len)) < 0) {
			print(DIR_ERR, "%s====>accept fail! %d\n", __FUNCTION__, __LINE__);
			continue;
		}
		work->fd = c_fd;
		threadpool_add(&gPool_sock, sock_routine, work);
	}
	
	close(fd);
}
