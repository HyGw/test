#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/time.h>

#include "bundleState.h"

char *ip = "127.0.0.1";
unsigned int port = 3456;

int bundle_socket(char *sndbuf, char **rcvbuf) {
	int fd;
	struct timeval timeout = {2, 0};
	struct sockaddr_in clientaddr;
	int err = 0;
	int len;
	int nlen;
	char *str = NULL;
	
	if(sndbuf == NULL) {
		printf("paramter == NULL\n");
		goto fail;
	}
	fd = socket(AF_INET, SOCK_STREAM, 0);
	if(fd < 0) {
		printf("socket fail!\n");
		goto fail;
	}
	clientaddr.sin_family = AF_INET;
	clientaddr.sin_port   = port;
	clientaddr.sin_addr.s_addr = inet_addr(ip);

	if(connect(fd, (struct sockaddr*) &clientaddr, sizeof(clientaddr)) < 0) {
        printf("connect fail!\n");
		goto connectfail;
	}
	err = setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, (const char *)&timeout, sizeof(timeout));
	if(err < 0) {
		printf("setsockopt fail!\n");
		goto connectfail;
	}
//	str = strstr(sndbuf, "\"type\":\"Event\"");
	len = strlen(sndbuf);
    nlen = htonl(len);
    if(send(fd, &nlen, sizeof(nlen), 0) < 0) {
  		printf("send fail!\n");
  		goto connectfail;
     }
     if(send(fd, sndbuf, len, 0) < 0) {
  		printf("send fail!\n");
  		goto connectfail;
     }
	// if(str == NULL) {
	if(recv(fd, &nlen, sizeof(nlen), 0) != sizeof(nlen)) {
		printf("rec fail!\n");
		goto connectfail;

	}
    len = ntohl(nlen);
	
	if((*rcvbuf = malloc(sizeof(char)*(len+1))) == NULL) {
		printf("malloc fail!\n");
		goto connectfail;
	}
	memset(*rcvbuf, 0, len+1);
	int res = read_loop(fd, *rcvbuf, len);
	if(res < 0) {
		printf("read_timeout fali!\n");
		goto recvfail;
	}
	// }
    close(fd);
	return 0;
recvfail:
	free(*rcvbuf);
	
connectfail:
	close(fd);

fail:
	return -1;
}

int read_loop(int fd, char *buf, int len) {
	int rcvlen = 0;
	int tmp = 0;
	
	while((len - rcvlen) > 0) {
		tmp = recv(fd, buf+rcvlen, len-rcvlen, 0);
		if(tmp <= 0)
			break;
		rcvlen += tmp;
	}
	
	if(rcvlen != len) {
		return -1;
	}

	return rcvlen;
	
}


/*1 get InternetGatewayDevice.SoftwareModules get R*/
int getSoftModulesPara(int type, char *retbuf){
	char buf[256] = {0};
	char *rectmp = NULL; 
	int fd = 0;
	int flag = 0;
	
	if (type < SF_EXECENV||type >SF_EXECUTEUNIT)
		goto fail;
	
	sprintf(buf, "{\"type\":\"bundle parameter\",\"option\":\"GET_SF\",\"node\":\"%d\"}",type);
	
	fd = bundle_socket(buf, &rectmp);
	if (fd <0){
		/*socket error*/
		goto fail;
	}
	if(strstr(rectmp, "\"Result\":-1") != NULL) 
		goto fail;

	
	strcpy(retbuf, rectmp);
	if (rectmp != NULL)
		free(rectmp);
	return 0;

fail:
	return -1;
}


/*2  get R InternetGatewayDevice.SoftwareModules.ExecEnv*/
int getExecEnv(int type, char *retbuf){
	char buf[256] = {0};
	char *rectmp = NULL; 
	int fd = 0;
	int flag = 0;

	if (type < EXENV_ENABLE||type >EXENV_PARENTEXECENV)
		goto fail;
	sprintf(buf, "{\"type\":\"bundle parameter\",\"option\":\"GET_EXENV\",\"node\":\"%d\"}",type);

	fd = bundle_socket(buf, &rectmp);

	if (fd <0){
		/*socket error*/
		if (type == EXENV_ENABLE){
			sprintf(retbuf, "%s", "0");
			return 0;
		}
		else if(type == EXENV_STATUS){
			sprintf(retbuf, "%s", "Disabled");
			return 0;
		}
		goto fail;
	}
	
	if(strstr(rectmp, "\"Result\":-1") != NULL) 
		goto fail;
	
	strcpy(retbuf, rectmp);
	if (rectmp != NULL)
		free(rectmp);
	return 0;

fail:
	return -1;

}

int setExecEnvEnable(int setbuf){
	if(setbuf == 0){
		system("killall -9 jamvm");
	}
	else if(setbuf==1){
		system("cd /usr/osgi/felix-framework");
		system("../bin/jamvm -Xms32M -Xmx64M -jar bin/felix.jar &");
	}
	else 
		return -1;
	return 0;
}
/*2  set W InternetGatewayDevice.SoftwareModules.ExecEnv */
int setExecEnv(int type, int setbuf){
	char buf[256] = {0};
	char *rectmp = NULL; 
	int fd = 0;
	int flag = 0;

	if(type == EXENV_ENABLE){
		/*setbuf = enable , 0-close, 1-open*/
		return(setExecEnvEnable(setbuf));
	}
	else if(type == EXENV_RESET){
		/*setbuf = reset , 0-false, 1-true*/
		sprintf(buf, "{\"type\":\"bundle parameter\",\"option\":\"SET_EXENV\",\"node\":\"%d\",\"reset\":\"%d\"}", type, setbuf);
	}
	else
		return -1;

	fd = bundle_socket(buf, &rectmp);
	if (fd <0){
		/*socket error*/
		goto fail;
	}
	if(strstr(rectmp, "\"Result\":-1") != NULL) 
		goto fail;

	if (rectmp != NULL)
		free(rectmp);
	return 0;

fail:
	return -1;

}

/*3 get R InternetGatewayDevice.SoftwareModules.DeploymentUnit.{i} */
int getDeploymentUnit(int type, int num, char *retbuf){
	char buf[256] = {0};
	char *rectmp = NULL; 
	int fd = 0;
	int flag = 0;

	if (type < DE_UUID||type >DE_EXECUTIONENVREF)
		goto fail;
	sprintf(buf, "{\"type\":\"bundle parameter\",\"option\":\"GET_DE\",\"node\":\"%d\",\"num\":\"%d\"}",type, num);
	
	fd = bundle_socket(buf, &rectmp);
	if (fd <0){
		/*socket error*/
		goto fail;
	}
	if(strstr(rectmp, "\"Result\":-1") != NULL) 
		goto fail;
	
	strcpy(retbuf, rectmp);
	if (rectmp != NULL)
		free(rectmp);
	return 0;

fail:
	return -1;

}
/*3 set R InternetGatewayDevice.SoftwareModules.DeploymentUnit.{i} */
int setDeploymentUnit(int type, int num, char *setbuf){
	char buf[256] = {0};
	char *rectmp = NULL; 
	int fd = 0;
	int flag = 0;

	if(type == DE_UUID){
		sprintf(buf, "{\"type\":\"bundle parameter\",\"option\":\"SET_DE\",\"node\":\"%d\",\"num\":\"%d\",\"UUID\":\"%s\"}",type, num, setbuf);
		}
	else{
			flag =1;
	}
	if (flag == 1){
		/*the type error*/
		goto fail;
	}

	fd = bundle_socket(buf, &rectmp);
	if (fd <0){
		/*socket error*/
		goto fail;
	}
	if(strstr(rectmp, "\"Result\":-1") != NULL) 
		goto fail;

	if (rectmp != NULL)
		free(rectmp);
	return 0;

fail:
	return -1;

}


/*4 get R InternetGatewayDevice.SoftwareModules.Execution.{i} */
int getExecutionUnit(int type, int num, char *retbuf){
	char buf[256] = {0};
	char *rectmp = NULL; 
	int fd = 0;
	int flag = 0;

	if (type < EXU_EUID||type >EXU_EXECUTIONENVREF)
			goto fail;
	sprintf(buf, "{\"type\":\"bundle parameter\",\"option\":\"GET_EXU\",\"node\":\"%d\",\"num\":\"%d\"}",type, num);

	fd = bundle_socket(buf, &rectmp);
	if (fd <0){
		/*socket error*/
		goto fail;
	}

	if(strstr(rectmp, "\"Result\":-1") != NULL) 
		goto fail;

	strcpy(retbuf, rectmp);
	if (rectmp != NULL)
		free(rectmp);
	return 0;

fail:
	return -1;

}
/*4 set W InternetGatewayDevice.SoftwareModules.Execution.{i}*/
int setExecutionUnit (int type, int num, char *setbuf){
	char buf[256] = {0};
	char *rectmp = NULL; 
	int fd = 0;
	int flag = 0;

	switch(type){
		case EXU_REQUESTEDSTATE:
			/*setbuf = executionfaultcode , 0-close, 1-open*/	
			sprintf(buf, "{\"type\":\"bundle parameter\",\"option\":\"SET_EXU\",\"node\":\"%d\",\"num\":\"%d\",\"state\":\"%s\"}", type, num, setbuf);
			break;
		case EXU_AUTOSTART:
			/*setbuf = reset , 0-false, 1-true*/
			sprintf(buf, "{\"type\":\"bundle parameter\",\"option\":\"SET_EXU\",\"node\":\"%d\",\"num\":\"%d\",\"autostart\":\"%s\"}", type, num, setbuf);
			break;
		default:
			flag =1;
			break;
	}
	if (flag == 1){
		/*the type error*/
		goto fail;
	}

	fd = bundle_socket(buf, &rectmp);
	if (fd <0){
		/*socket error*/
		goto fail;
	}
	if(strstr(rectmp, "\"Result\":-1") != NULL) 
		goto fail;

	if (rectmp != NULL)
		free(rectmp);
	return 0;

fail:
	return -1;


}





