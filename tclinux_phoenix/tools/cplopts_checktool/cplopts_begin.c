
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define TMP_FILE_DIR "swap_file"
#define MAX_MACRO_NUM   8
#define REMOVE_ALL       0
#define REMOVE_DEF       1
#define REMOVE_TOT       1

#define ERROR_STRING	-2
#define MATCH_PLAIN_CODE    -1
#define MATCH_ENDIF	0
#define MATCH_IF_FOUND      1
#define MATCH_IF_NOTFOUND   2
#define MATCH_ELSE	    3


#define IFDEF	1
#define ELSE	0

int main(int argc, char *argv[])
{

	char cmd[2048] = {0};
	
	memset(cmd, 0, sizeof(cmd) );
	if(argc == 4)
		sprintf(cmd, "tools/cplopts_formcheck %s %s %s", argv[1], argv[2], argv[3]);
	else if(argc == 3)
	sprintf(cmd, "tools/cplopts_formcheck %s %s", argv[1], argv[2]);
	else {
		printf("tools/cplopts_formcheck arg number error!\n");
		return 0;
	}
	printf("tools:%s!\n",cmd);		
	system(cmd);
		
	return 0;
}
