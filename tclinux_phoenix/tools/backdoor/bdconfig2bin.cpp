#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <errno.h>


#define STORE32_LE(A) (((int)(A) & 0xff000000) >> 24 | \
					((int)(A) & 0x00ff0000) >> 8 | \
					((int)(A) & 0x0000ff00) << 8 | \
					((int)(A) & 0x000000ff) << 24)

//#define STORE32_LE(X)	SWAP32(X)

#define BD_MAGIC_NUM	0x54321253

typedef struct {
	int	magic_num;
	int ddr_config_length;
	int eth_config_length;
	int reserved[5];
} header_t;


void usage()
{
	fprintf(stderr, "Usage: bdconf2bin bd.conf bd.bin\n");
//	exit(EXIT_FAILURE);
}

char *ltrim(char *string) 
{
	/* trims leading blanks */
	char *j;

	j = string+strlen(string);
	while ((string[0] == ' ') && (string < j)) string++;
	return(string);
}


char *rtrim(char *string)
{
	/* trims trailing blanks */
	int j;

	j = strlen(string);
	while ((string[j-1] == ' ') && (j != 0)) j--;
	string[j]='\0';
	return(string);
}

char *trim(char *string)
{
  /* trims leading and trailing blanks */
  return(rtrim(ltrim(string)));
}


void confParse(char *config_fname, char *bin_fname)
{
	char line[1024], wbuf[64];
	FILE *conf_fp, *bin_fp, *tmp_fp;
	int uvalue = 0, i, readcnt;
	header_t bin_header;
	int *p;
	
	conf_fp = fopen(config_fname, "r+t");
	if (conf_fp == NULL) {
		printf("open manufacturing information config file error\r\n");
		exit(1);
	}

	bin_fp = fopen(bin_fname, "wb");
	if (bin_fp == NULL) {
		printf("open bin file error\r\n");
		exit(1);
	}

	tmp_fp = fopen("temp.bin", "wb");
	if (tmp_fp == NULL) {
		printf("open temp file error\r\n");
		exit(1);
	}

	memset((void*)(&bin_header), 0, sizeof(header_t));
	bin_header.magic_num = BD_MAGIC_NUM;
	

	while (fgets(line, 1024, conf_fp) != NULL) {
		if (line[strlen(line)-1] == '\n')
			line[strlen(line)-1] = '\0';
		if ((strlen(line) == 0) || (line[0] == ';'))
			continue;

		if (strchr(line, '=') == NULL) {
			printf("format error line=%s len=%d\n", line, strlen(line));
			continue;
		}

		char *pos = strchr(line, '=');
		char *key = line;
		char *value = pos + 1;

		*pos = '\0';

		key = trim(key);
		if( key[strlen(key)-1] == 0x0d )
			key[strlen(key)-1] = 0x00;
		value = trim(value);
		if( value[strlen(value)-1] == 0x0d )
			value[strlen(value)-1] = 0x00;

		printf("key=%s value=%s\r\n", key, value);

		if (strstr(key, "ether") != NULL) {	//eth config
			bin_header.eth_config_length += 4;
		}
		else if(strstr(key, "ddr") != NULL) {	//ddr config
			bin_header.ddr_config_length += 4;
		}
		
		sscanf(value, "%lx", &uvalue);
		uvalue = STORE32_LE(uvalue);
		*(int*)(wbuf) = uvalue;
		
		fwrite(wbuf, 4, 1, tmp_fp);

	}
	fclose(tmp_fp);

	p = (int*)(&bin_header);
	for(i = 0; i < sizeof(header_t); i+=4){
		uvalue = STORE32_LE(*p);
		*(int*)(wbuf) = uvalue;
		fwrite(wbuf, 4, 1, bin_fp);
		p++;
	}

	tmp_fp = fopen("temp.bin", "rb");
	if (tmp_fp == NULL) {
		printf("open temp file error\r\n");
		exit(1);
	}

	while(readcnt = fread(wbuf, 4, 1, tmp_fp)){
		fwrite(wbuf, 4, 1, bin_fp);
	}
	

	printf("read %d ether config parameters, %d ddr config parameters\r\n", bin_header.eth_config_length/12, bin_header.ddr_config_length/8);
	fclose(tmp_fp);
	fclose(bin_fp);
	fclose(conf_fp);
}


int main(int argc, char **argv) 
{
	if (argc != 3)
	{
		usage();
		return 0;
	}

	confParse(argv[1], argv[2]);

	return 0;
				
}
