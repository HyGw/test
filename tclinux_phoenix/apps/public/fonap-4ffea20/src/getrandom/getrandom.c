
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <getopt.h>
#include <errno.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <sys/termios.h>
#include <sys/time.h>



/*****************************************************************************/
#define MAX_NUM 100000000

int hextoint(char ch)
{
	int ret = 0;
	if (ch >= '0' && ch <= '9')
		ret = ch - '0';
	if (ch >= 'a' && ch <= 'f')
		ret = 10 + ch - 'a';
	if (ch >= 'A'  && ch <= 'F')
		ret = 10 + ch - 'A';
	return ret;
}

int main(int argc, char *argv[])
{
	FILE* fp;
	char macstr[32];
	unsigned int seed = 1;
	unsigned int min = 0;
	unsigned int max = MAX_NUM;

	if (argc == 2)
	{
		min = atoi(argv[1]);
	}
	if (argc == 3)
	{
		min = atoi(argv[1]);
		max = atoi(argv[2]);
	}
	if (min <0 || max > MAX_NUM || min >= max )
	{
		min = 0;
		max = MAX_NUM;
	}
	fp = fopen("/etc/fon_mac","r");
	if (fp != NULL)
	{	
		fgets(macstr,32,fp);
		seed = hextoint(macstr[12])*4096+hextoint(macstr[13])*256+hextoint(macstr[15])*16+hextoint(macstr[16]);
		fclose(fp);
	}
	seed += (unsigned) time(NULL) + getpid();
	fp =  fopen("/etc/fon/fon_random_debug","r");
	if(fp != NULL)
	{
		printf("min = %u max = %u seed = %u \n",min,max,seed);
		fclose(fp);
	}
	srand(seed);
	printf("%u\n",min + rand()%(max-min) );
	return 0;
}

/*****************************************************************************/
