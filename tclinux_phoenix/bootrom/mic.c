#include <stdio.h>
#include <string.h>
#include <stdlib.h>
//#include "mic.h"

typedef unsigned long		uint32;
typedef unsigned char		uint8;

#define MAX_MULTI_LED_NUM 16

typedef struct {
	uint32 addr;
	uint8 len;
	uint8* val;
} map_t;

uint8 item1[] = {0x00, 0x02, 0x00, 0x00};

uint8 item2[] = {0x00, 0x10, 0x00, 0x00};

uint8 item3[] = {0xbf, 0xc0, 0x80, 0x00};
uint8 item3_spi_r[] = {0xbf, 0xc3, 0x00, 0x00};
uint8 item3_spi_b[] = {0xbf, 0xc0, 0xa0, 0x00};

uint8 item4[] = {0x06};

uint8 item5[] = {
	0x93, 0x17, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x13, 0x00, 0x00, 0x00};

uint8 item6[] = {0x86};

uint8 item7[] = {0xFF};

uint8 item8[] = {0x00};

uint32 SDRAM_CONF = 0;
char VENDOR_NAME[80];
char PRODUCT_NAME[80];
#if defined(TCSUPPORT_CT_BOOTLOADER_UPGRADE)
uint8 FUNC_BYTE[4]={0x00,0x00,0x00,0x00};
#else
uint8 FUNC_BYTE[]={0x00};
#endif

uint8 ETHER_ADDR[6];
char SNMP_SYSOBJID[80];
uint8 MODEL[2] = {0x93, 0x17};

uint8 SDRAM_CONF_array[4];
uint8 SNMP_SYSOBJID_array[22];
uint8 COUNTRY_array[] = {0xFF};
uint8 ether_gpio[] = {0x9};
uint8 power_gpio[2] = {0xFF, 0xFF};
uint8 dsl_gpio[] = {0x6};
uint8 internet_gpio[] = {0x7};
uint8 multi_upgrade_gpio[16] = {0x3, 0x5, 0x6, 0x7, 0x8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
#if defined(TCSUPPORT_CT_BOOTLOADER_UPGRADE)
char USER_NAME[16] = {0}; 
char PASS_WORD[16] = {0};
#endif
uint8 restore_flag[] = {0};
uint8 id_75xx[] = {0};
uint8 onu_type[] = {0};

char SN[18] = {0};



int map_size;
map_t map[] = {
	{0x3f00, 4,  SDRAM_CONF_array},
	{0x3f04, 32, (uint8 *) VENDOR_NAME},
	{0x3f24, 32, (uint8 *) PRODUCT_NAME},
	{0x3f44, 4,  FUNC_BYTE},
	{0x3f48, 6,  ETHER_ADDR},
	{0x3f4e, 22, SNMP_SYSOBJID_array},
	{0x3f64, 1,  COUNTRY_array},
	{0x3f65, 1,  ether_gpio},
	{0x3f66, 2,  power_gpio},
	{0x3f68, 1,  restore_flag},
#if defined(TCSUPPORT_CT_BOOTLOADER_UPGRADE)
	{0x3f69, 16,  USER_NAME}, 
	{0x3f79, 16,  PASS_WORD}, 
#endif
	{0x3f89, 1,  dsl_gpio},
	{0x3f8a, 1,  internet_gpio},
	{0x3f8b, 16,  multi_upgrade_gpio},
	{0x3f9b,1, id_75xx},
	{0x3f9c, 1, onu_type},
	{0x3f9d, 18,  SN},
};

/*
map_t map[] = {
	{0x3f88, 4,  item1},
	{0x3f8c, 4,  item2},
	{0x3fd0, 4,  item3},
	{0x3fd6, 1,  item4},
	{0x3fd8, 30, item5},
	{0x3ff6, 1,  item6},
	{0x3ffe, 1,  COUNTRY_array},
	{0x3fff, 1,  item8},
	// user-defined value
	{0x3f6c, 4,  SDRAM_CONF_array},
	{0x3f90, 32, (uint8 *) VENDOR_NAME},
	{0x3fb0, 32, (uint8 *) PRODUCT_NAME},
	{0x3ff7, 1,  FUNC_BYTE},
	{0x3ff8, 6,  ETHER_ADDR},
	{0x3f72, 22, SNMP_SYSOBJID_array},
};
*/
// 16KB bootbase buffer
//uint8 *bootbase;
//uint8 bootbase_parallel[16384];
#define SPI_OFFSET 0x2000
#define LINUX_OFFSET 0xC000

#define SPI_BB_SIZE 24*1024
#define PARALLEL_BB_SIZE 16*1024
#define LINUX_BB_SIZE 64*1024
#define LINUX_BB_LARGE_SIZE 64*1024*2

long bootbase_size;
//uint8 bootbase[24576];//IAN_SPI Flash Support
uint8 bootbase[LINUX_BB_SIZE*2];

char mi_filename[80];
char bb_filename[80];
char bo_filename[80];

void miParse(void);
void getBootbase(void);
void initBootbase(void);
void writeBootbase(void);
char *ltrim(char *string);
char *trim(char *string);
char *trim(char *string);
void extendBootbase(void);

void main(int argc, char **argv)
{
	uint8 index = 0;
	uint8 bridge_flag = 0;
	uint8 linux_flag = 0, linux_large_flag = 0;
	uint8 i;
	uint32 offset;

	memset(mi_filename, 0, sizeof(mi_filename));
	memset(bb_filename, 0, sizeof(bb_filename));
	memset(bo_filename, 0, sizeof(bo_filename));


	if (argc >= 2) {

		if( strcmp(argv[1], "-l") == 0 ){	/* Linux platform */
			index = 1;
			linux_flag = 1;
		}
		if( strcmp(argv[1], "-ll") == 0 ){	/* Linux platform  large size tcboot*/
			index = 1;
			linux_flag = 1;
			linux_large_flag = 1;
		}
		if( strcmp(argv[1], "-s") == 0 )	/* spi flash */
			index = 1;

		if( strcmp(argv[1], "-sb") == 0 )	/* spi flash */
		{
			bridge_flag = 1;
			index = 1;
		}
		if (argc >= 2+index) 
			sprintf(mi_filename, "%s", argv[1+index]);
		if (argc >= 3+index) 
			sprintf(bb_filename, "%s", argv[2+index]);
		if (argc >= 4+index) 
			sprintf(bo_filename, "%s", argv[3+index]);
	}

	strcpy(mi_filename, "mi.conf");

	if( linux_flag ){
		strcpy(bb_filename, "boot.bin");
		strcpy(bo_filename, "tcboot.bin"); 
	}
	else{
		strcpy(bb_filename, "bbtc.ram");
		strcpy(bo_filename, "bbtc.bin"); 
	}

	map_size = sizeof(map);

	if( index ){
		if( linux_flag ){
			bootbase_size = linux_large_flag ? LINUX_BB_LARGE_SIZE : LINUX_BB_SIZE;
			offset = LINUX_OFFSET;
		}
		else{
			bootbase_size = SPI_BB_SIZE;

			if( bridge_flag )
				memcpy(item3, item3_spi_b, 4);
			else
				memcpy(item3, item3_spi_r, 4);

			offset = SPI_OFFSET;
		}
		for (i = 0; i <(map_size/sizeof(map_t)); i++) {
//			printf("i=%d addr=%lx map_len=%d\r\n", i, map[i].addr, map[i].len);
			map[i].addr += offset;
//			printf("i=%d addr=%lx map_len=%d\r\n\r\n", i, map[i].addr, map[i].len);
		}
	}
	else{
		bootbase_size = PARALLEL_BB_SIZE;
		
	}

	printf("\r\n");
	printf("Manufacturing Information File: %s\r\n", mi_filename);
	printf("Bootbase Input File           : %s\r\n", bb_filename);
	printf("Bootbase Output File          : %s\r\n", bo_filename);
	printf("Flash type                    : %s\r\n", index?"SPI":"Parallel");
	printf("\r\n");

	miParse();
	
	getBootbase();
	if(linux_large_flag)
		extendBootbase();
	initBootbase();
	writeBootbase();
}

void extendBootbase(void)
{
	FILE *in;
	char buffer[100];
	char name[30];
	uint32 boot_img_start, boot_img_end, addr;
	int errno, c, i;
	char *buf;
	
	//Move boot.img to 64k offset
	if (!(in = fopen("system.map", "rb"))) {
		printf("Mic can not open <system.map> for reading\n");
		exit(EXIT_FAILURE);
	}

	while (fgets(buffer, 100, in) != NULL) {
		if(buffer == NULL) 
			break;
		sscanf(buffer, "%x %c %s", &addr, &c, name);
		if (strcmp(name, "__boot_start") == 0) {
			boot_img_start = addr - 0xbfc00000;
		} else if (strcmp(name, "__boot_end") == 0) {
			boot_img_end = addr - 0xbfc00000;
		} 
	}
	fclose(in);

	printf("boot.img:start:%x,end:%x\r\n", boot_img_start, boot_img_end);

	if (!(buf = malloc(LINUX_BB_SIZE + 4))) {
		printf("malloc failed\n");
		exit(EXIT_FAILURE);
	}

	memcpy(buf, bootbase+boot_img_start, boot_img_end - boot_img_start);
	memset(bootbase+boot_img_start, 0, boot_img_end - boot_img_start);
	memcpy(bootbase+64*1024, buf, boot_img_end - boot_img_start);

	free(buf);
	
}


void miParse(void)
{
	char line[1024];
	FILE *mic_conf_fp;
	uint8 i;
	uint8 j;
	uint8 k;
	char addr_str[3];
	uint8 addr;
	
	mic_conf_fp = fopen(mi_filename, "r+t");
	if (mic_conf_fp == NULL) {
		printf("open manufacturing information config file error\r\n");
		exit(1);
	}

	while (fgets(line, 1024, mic_conf_fp) != NULL) {
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

		//printf("key=%s value=%s\r\n", key, value);

		if (strcmp(key, "sdram_conf") == 0) {
			sscanf(value, "%lx", &SDRAM_CONF);
		} else if (strcmp(key, "vendor_name") == 0) {
			if (strlen(value) >= 32) {
				printf("ERR: vendor_name len must be less than 32\r\n");
			} else {
				strcpy(VENDOR_NAME, value);
			}
		} else if (strcmp(key, "product_name") == 0) {
			if (strlen(value) >= 32) {
				printf("ERR: product_name len must be less than 32\r\n");
			} else {
				strcpy(PRODUCT_NAME, value);
			}
		} else if (strcmp(key, "ether_addr") == 0) {
			if (strlen(value) != 12) {
				printf("ERR: ether_addr len must be 6 byte length\r\n");
			} else {
				for (i = 0, j = 0; i < 12; i += 2) {
					strncpy(addr_str, &value[i], 2);
					addr_str[2] = '\0';
					sscanf(addr_str, "%x", &addr);
					ETHER_ADDR[j++] = addr;
				}
			}
		} else if (strcmp(key, "snmp_sysobjid") == 0) {
			if (strlen(value) == 0) {
				printf("ERR: snmp_sysobjid cannot be empty\r\n");
			} else {
				strcpy(SNMP_SYSOBJID, value);
			}
		} else if (strcmp(key, "country_code") == 0) {
			if (strlen(value) != 2) {
				printf("ERR: country_code len must be 1 byte length\r\n");
			} else {
				strncpy(addr_str, &value[0], 2);
				addr_str[2] = '\0';
				sscanf(addr_str, "%x", &addr);
				COUNTRY_array[0] = addr;
				//printf("country_code=%x\r\n", COUNTRY_array[0]);
			}
		} else if (strcmp(key, "function_byte") == 0) {
			if (strlen(value) != 2) {
				printf("ERR: function_byte len must be 1 byte length\r\n");
			} else {
				strncpy(addr_str, &value[0], 2);
				addr_str[2] = '\0';
				sscanf(addr_str, "%x", &addr);
				FUNC_BYTE[0] = addr;
				//printf("country_code=%x\r\n", COUNTRY_array[0]);
			}
		} else if (strcmp(key, "model") == 0) {
			if (strlen(value) != 4) {
				printf("ERR: model len must be 2 byte length\r\n");
			} else {
				for (i = 0, j = 0; i < 4; i += 2) {
					strncpy(addr_str, &value[i], 2);
					addr_str[2] = '\0';
					sscanf(addr_str, "%x", &addr);
					MODEL[j++] = addr;
				}
				//printf("MODEL=%x %x\r\n", MODEL[0], MODEL[1]);
			}
		}
		else if (strcmp(key, "ether_gpio") == 0) {
			if (strlen(value) != 2) {
				printf("ERR: ether_gpio len must be 1 byte length\r\n");
			} else {
				strncpy(addr_str, &value[0], 2);
				addr_str[2] = '\0';
				sscanf(addr_str, "%x", &addr);
				ether_gpio[0] = addr;
				//printf("ether_gpio=%x\r\n", ether_gpio[0]);
			}
		}
		else if (strcmp(key, "power_gpio") == 0) {
			if (strlen(value) != 4) {
				printf("ERR: power_gpio len must be 2 byte length\r\n");
			} else {
				for (i = 0, j = 0; i < 4; i += 2) {
					strncpy(addr_str, &value[i], 2);
					addr_str[2] = '\0';
					sscanf(addr_str, "%x", &addr);
					power_gpio[j++] = addr;
				}
				//printf("power gpio=%x %x\r\n", power_gpio[0], power_gpio[1]);
			}
		}
#if defined(TCSUPPORT_CT_BOOTLOADER_UPGRADE)
		else if (strcmp(key, "username") == 0) 
		{
			if (strlen(value) >= 16) 
			{
				printf("ERR: user_name len must be less than 16\r\n");
			} 
			else 
			{
				strcpy(USER_NAME, value);
			}
		}
		else if (strcmp(key, "password") == 0) 
		{
			if (strlen(value) >= 16) 
			{
				printf("ERR: pass_word len must be less than 16\r\n");
			} 
			else 
			{
				strcpy(PASS_WORD, value);
			}
		}
#endif
		else if (strcmp(key, "dsl_gpio") == 0) {
			if (strlen(value) != 2) {
				printf("ERR: dsl_gpio len must be 1 byte length\r\n");
			} else {
				strncpy(addr_str, &value[0], 2);
				addr_str[2] = '\0';
				sscanf(addr_str, "%x", &addr);
				dsl_gpio[0] = addr;
				//printf("dsl_gpio=%x\r\n", dsl_gpio[0]);
			}
		}
		else if (strcmp(key, "internet_gpio") == 0) {
			if (strlen(value) != 2) {
				printf("ERR: internet_gpio len must be 1 byte length\r\n");
			} else {
				strncpy(addr_str, &value[0], 2);
				addr_str[2] = '\0';
				sscanf(addr_str, "%x", &addr);
				internet_gpio[0] = addr;
				//printf("internet gpio=%x\r\n", internet_gpio[0]);
			}
		}
		else if (strcmp(key, "multi_upgrade_gpio") == 0) {
			if (strlen(value) != (MAX_MULTI_LED_NUM*2)) {
				printf("ERR: multi_upgrade_gpio len must be 16 byte length\r\n");
			} else {
				for (i = 0, j = 0; i < (MAX_MULTI_LED_NUM*2); i += 2) {
					strncpy(addr_str, &value[i], 2);
					addr_str[2] = '\0';
					sscanf(addr_str, "%x", &addr);
					multi_upgrade_gpio[j++] = addr;
				}
			}
		}
		else if(strcmp(key, "75xx_id") == 0){
			if(strlen(value) != 1){
				printf("ERR: 75xx_id len must be 1 byte length\r\n");
			}
			else{
				strncpy(addr_str, &value[0], 2);
				addr_str[2] = '\0';
				sscanf(addr_str, "%d", &addr);
				id_75xx[0] = addr;
				printf("75xx_id=%x\r\n", id_75xx[0]);
			}
		}
		else if(strcmp(key, "onu_type") == 0){
			if(strlen(value) != 1){
				printf("ERR: onu_type len must be 1 byte length\r\n");
			}
			else{
				strncpy(addr_str, &value[0], 2);
				addr_str[2] = '\0';
				sscanf(addr_str, "%d", &addr);
				onu_type[0] = addr;
				printf("onu_type=%x\r\n", onu_type[0]);
			}
		}
		else if (strcmp(key, "SN") == 0) {
			if (strlen(value) != 18) {
				printf("ERR: SN len must be 18 byte length\r\n");
			} else {
				strcpy(SN, value);
			}
		}
	}

	printf("SDRAM Config           : %08lx\r\n", SDRAM_CONF);
	printf("Vendor Name            : %s\r\n", VENDOR_NAME);
	printf("Product Name           : %s\r\n", PRODUCT_NAME);
	printf("MAC Address            : ");
	for (k = 0; k < 6; k++)
		printf("%02x ", ETHER_ADDR[k]);
	printf("\r\n");
	printf("SNMP sysObjectId       : %s\r\n", SNMP_SYSOBJID);
	printf("Country Code           : %02x\r\n", COUNTRY_array[0]);
	printf("Func byte              : %02x\r\n", FUNC_BYTE[0]);
	printf("Model No               : %02x %02x\r\n", MODEL[0], MODEL[1]);
	printf("Ethernet GPIO          : %02x\r\n", ether_gpio[0]);
	printf("Power GPIO	            : %02x %02x\r\n", power_gpio[0], power_gpio[1]);
#if defined(TCSUPPORT_CT_BOOTLOADER_UPGRADE)
	printf("Userr Name	       : %s\r\n", USER_NAME);
	printf("Pass Word	       : %s\r\n", PASS_WORD);
#endif
	printf("DSL GPIO               : %02x\r\n", dsl_gpio[0]);
	printf("Internet GPIO          : %02x\r\n", internet_gpio[0]);
	printf("Multi Upgrade GPIO     : ");
	for(k = 0; k < MAX_MULTI_LED_NUM; k++){
		printf("%02x ", multi_upgrade_gpio[k]);
	}
	printf("\r\n");
	printf("75xx id:%d \n",id_75xx[0]);
	printf("ONU type              :%d \n",onu_type[0]);	
	printf("SN	       : %s\r\n", SN);
	fclose(mic_conf_fp);
}

void getBootbase(void)
{
	FILE *bb_fp;
	int ch;
	int i;
	
	bb_fp = fopen(bb_filename, "rb");
	if (bb_fp == NULL) {
		printf("open bootbase input file error\r\n");
		exit(1);
	}

	memset(bootbase, 0, bootbase_size);

	i = 0; 
	while (!feof(bb_fp)) {
		ch = fgetc(bb_fp);
		if (i > bootbase_size) {
			printf("ERR: bootbase size too large %d %d\r\n", i, bootbase_size);
			exit(1);
		}
		bootbase[i++] = ch;
	}
	//printf("i=%d\r\n", i);

	fclose(bb_fp);
}

void initBootbase(void)
{
	uint8 i;
	uint32 addr;
	uint8 len;

	// sdram
	SDRAM_CONF_array[0] = (uint8) ((SDRAM_CONF & 0xff000000) >> 24);
	SDRAM_CONF_array[1] = (uint8) ((SDRAM_CONF & 0x00ff0000) >> 16);
	SDRAM_CONF_array[2] = (uint8) ((SDRAM_CONF & 0x0000ff00) >> 8);
	SDRAM_CONF_array[3] = (uint8) ((SDRAM_CONF & 0x000000ff) >> 0);

	// snmp sysobjid
	if (strlen(SNMP_SYSOBJID) != 0) {
		uint8 len = 0;
		uint8 index = 1;
		char *token = strtok(SNMP_SYSOBJID, ".");

		while (token != NULL )
		{
			//printf( " %s\n", token );

			uint32 objid = atol(token);
			//printf("objid=%d\r\n", objid);

			SNMP_SYSOBJID_array[index++] = (uint8) ((objid & 0xff000000) >> 24);
			SNMP_SYSOBJID_array[index++] = (uint8) ((objid & 0x00ff0000) >> 16);
			SNMP_SYSOBJID_array[index++] = (uint8) ((objid & 0x0000ff00) >> 8);
			SNMP_SYSOBJID_array[index++] = (uint8) ((objid & 0x000000ff) >> 0);

			token = strtok(NULL, ".");
			len++;
		}
		SNMP_SYSOBJID_array[0] = len;
		//printf("len=%d\r\n", len);
	}

	for (i = 0; i < 2; i++)
		item5[i] = MODEL[i];
	for (i = 0; i < (map_size/sizeof(map_t)); i++) {
		//printf("i=%d addr=%lx map_len=%d\r\n", i, map[i].addr, map[i].len);
	
		addr = map[i].addr;
		len = 0;
		while (len < map[i].len)
			bootbase[addr++] = map[i].val[len++];
	}
}

void writeBootbase(void)
{
	FILE *bo_fp;
	int i;
	
	bo_fp = fopen(bo_filename, "wb");
	if (bo_fp == NULL) {
		printf("open bootbase output file error\r\n");
		exit(1);
	}

	for (i = 0; i < bootbase_size; i++)
		fputc(bootbase[i], bo_fp);

	fclose(bo_fp);
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
