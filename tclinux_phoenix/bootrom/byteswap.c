#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <byteswap.h>

int main(int argc, char ** argv)
{
    int fd1, fd2, num;
    char out_name[64], buf[2];

	if (argc < 2) {
		printf("usage: byteswap <filename>\n");
		return -1;
	}
    fd1 = open(argv[1], O_RDONLY);
    if (fd1 < 0) {
        printf("open argv[1] failed!!\n");
		return -1;
	}
    strcpy(out_name, argv[1]);
    strcat(out_name, ".swap");
    fd2 = open(out_name, O_WRONLY|O_CREAT|O_TRUNC, 0744);
    if (fd2 < 0)
	printf("open out_name failed!!\n"); 

    while ((num=read(fd1, buf, 2)) == 2 || num == 1)
    {
        *(unsigned short *)buf = bswap_16(*(unsigned short *)buf); 
        write(fd2, buf, 2);
    }
    
    close(fd1);
    close(fd2);

    printf("byteswap complete\n");
	return 0;
}

