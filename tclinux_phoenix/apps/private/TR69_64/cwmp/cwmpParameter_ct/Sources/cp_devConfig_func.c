#include "Global_res.h"
#include "cp_devConfig.h"


int8 getPersistentDataValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char persistData[256];


	if (value==NULL || type==NULL || deviceNode==NULL)
	{
		tc_printf("\r\n getPersistentDataValue(), Pointer == NULL!");
		return -1;
	}

	memset(persistData, 0, sizeof(persistData));
	getCwmpAttrValue(Persistent_Data, persistData, sizeof(persistData));

	if (strlen(persistData) > 255){
		tc_printf("\r\n The length of vPersistentData is not correct!");
		memset(persistData, 0, sizeof(persistData));
	}

	return getTheValue(value, type, deviceNode, persistData);	
}

int8 setPersistentDataValue(void* value, struct DeviceNode*  deviceNode)
{
	if (value==NULL || deviceNode==NULL)
	{
		tc_printf("\r\n setPersistentDataValue(), Pointer == NULL!");
		return -1;		
	}

	SetCfgNodeFlag( "Cwmp", -1 );

	if(strlen((char*)value) > 255)
		return -1;
	
	setCwmpAttrValue(Persistent_Data, (char*)value);
	return 0 ;
}

int8 getPersistentDataAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute = 0;
	int8  result = 0;
	
	if (notification==NULL || acl==NULL ||deviceNode==NULL)
	{
		tc_printf("\r\n getPersistentDataAttribute(), Pointer == NULL!");
		return -1;		
	}

	attribute = tcDevConfigAttr.aPersistentData;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setPersistentDataAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result = 0;

	if (acl==NULL || deviceNode==NULL)
	{
		tc_printf("\r\n setPersistentDataAttribute(), Pointer == NULL!");
		return -1;		
	}

	attribute = &tcDevConfigAttr.aPersistentData;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	rpc.attrFlags.theSettingFlag0 |= DEVCONFIG_ATTR_FLAG;

	return result;
}

int updateConfigFile( cfgNode_ptr pCfgNode )
{
	char cmd[64];

#if defined(TCSUPPORT_CT_BACKUPROMFILEENCHANCEMENT)
	int flag = -1;
#endif
	memset(cmd, 0, sizeof(cmd));

	tc_printf("\r\n--configfile write finish, write to running romfile and reboot--\n", DOWNLOAD_CONFIG_FILE_PATH);
#if defined(TCSUPPORT_CT_BACKUPROMFILEENCHANCEMENT)
	flag = compute_checksum(DOWNLOAD_CONFIG_FILE_PATH,STORE_ROMFILE_FILE_NAME);
	if(flag < 0)
	{
		fprintf(stderr, "updateConfigFile:Fail ro compute checksum!\n");
		return -1;
	}
	sprintf(cmd, NOREBOOT_CMD, STORE_ROMFILE_FILE_NAME);
	system(cmd);	
	usleep(100000);

	sprintf(cmd, TC_FLASH_ERASE_SECTOR_CMD,(unsigned long)BACKUPROMFILE_RA_OFFSET, RESERVEAREA_NAME);
	system(cmd);
	
	memset(cmd,0,sizeof(cmd));
	sprintf(cmd, TC_FLASH_WRITE_CMD,STORE_ROMFILE_FILE_NAME,(unsigned long)BACKUPROMFILE_RA_SIZE,(unsigned long)BACKUPROMFILE_RA_OFFSET, RESERVEAREA_NAME);
	system(cmd);
	system("reboot");
#else
#if defined(TCSUPPORT_BACKUPROMFILE)
	sprintf(cmd, NOREBOOT_CMD, DOWNLOAD_CONFIG_FILE_PATH);
	system(cmd);
	usleep(100000);

	sprintf(cmd, TC_FLASH_ERASE_SECTOR_CMD,(unsigned long)BACKUPROMFILE_RA_OFFSET, RESERVEAREA_NAME);
	system(cmd);
	
	memset(cmd,0,sizeof(cmd));
	sprintf(cmd, TC_FLASH_WRITE_CMD,DOWNLOAD_CONFIG_FILE_PATH,(unsigned long)BACKUPROMFILE_RA_OFFSET,(unsigned long)BACKUPROMFILE_RA_SIZE, RESERVEAREA_NAME);
	system(cmd);
	system("reboot");
#endif
#endif
	
	return 0;
}

uint32 get_file_size(FILE *fp)
{
	uint32 fileSize = 0;
	
	if(fp == NULL)
		return 0;
	
	fseek(fp, 0, SEEK_END);//seek to the file end
	fileSize = ftell(fp);		//get the file size
	fseek(fp, 0, SEEK_SET);//seek back to the file beginning 

	return fileSize;	
}

int8 getConfigFileValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	int8 ret;
	FILE *fp;
	char tmp;
	char *spt_ptr = NULL;
	char *new_ptr = NULL;
	uint32 i = 0;
	uint32 fileSize = 0;
	uint32 buf_len_append = 0;
	int maxLen = 0;
	int leftLen = 0;
	int rightLen = 0;

	/*open romfile file and get file size*/
	fp = fopen(UPLOAD_CONFIG_FILE_PATH, "rb");
	if(fp == NULL){
		tc_printf("\r\nconfigfile read: Can't open file %s\n", UPLOAD_CONFIG_FILE_PATH);
		return -1;;
	}
	fileSize = get_file_size(fp);
	
	/*calculate and compare the replaced string ,get the max one */
	leftLen = sizeof(LEFT_REPLACE) -1;
	rightLen = sizeof(RIGHT_REPLACE) -1;
	if(leftLen - rightLen >= 0)
		maxLen = leftLen;
	else
		maxLen = rightLen;
	buf_len_append = (leftLen+rightLen) * BUF_APPEND;//buf length is appended for character replace

	/*allocate memory for buf*/
	spt_ptr = (char *)os_malloc(fileSize+buf_len_append+1);
	if(spt_ptr == NULL){
		tc_printf("\r\nOut of memory!\n");
		fclose(fp);
		return -1;
	}
	else{
		memset(spt_ptr, 0, fileSize+buf_len_append+1);
		/*parser the received data, replace '<' and '>'*/
		while((tmp = fgetc(fp)) != EOF){
			/*handle memory run out*/
			if((i+maxLen) > (fileSize+ buf_len_append)){
				tc_printf("\nMemory is out, we must do realloc to increase memory!\n");
				fileSize = fileSize << 1 ;
				new_ptr = (char *)os_malloc(fileSize+buf_len_append+1);//alloc new memory size
				if(new_ptr == NULL){
					tc_printf("\nrealloc failed, free memory!\n");
					fclose(fp);
					os_free(spt_ptr);
					spt_ptr = NULL;
					return -1;
				}
				else{
					memset(new_ptr, 0, fileSize+buf_len_append+1);
					strncpy(new_ptr, spt_ptr, i);
					tc_printf("\nfree no used memory and use new memory\n");
					os_free(spt_ptr);
					spt_ptr = new_ptr;
				}
			}
			
			if(tmp == '<'){
				strncpy(&spt_ptr[i], LEFT_REPLACE, leftLen);
				i += leftLen;
			}
			else if(tmp == '>'){
				strncpy(&spt_ptr[i], RIGHT_REPLACE, rightLen);
				i += rightLen;
			}
			else{
				spt_ptr[i] = tmp;
				i++;	
			}
		}
		spt_ptr[i] = '\0';	

		ret = getTheValue(value, type, deviceNode, spt_ptr);

		/*remeber to free  memory */
		if(spt_ptr != NULL){
			os_free(spt_ptr);
			spt_ptr = NULL;
		}	

		fclose(fp);
		return ret;
	}
}

int8   setConfigFileValue(void* value, struct DeviceNode*  deviceNode)
{
	FILE *fp;
	uint32 length = 0;
	uint32 i = 0, j = 0;
	char *buf = NULL;	
	char *romfilePtr = NULL;
	uint32 bytes = 0;
	char leftOrRightFlag= 0;
	char identicalFlag = 0;
	int leftLen = 0;
	int rightLen = 0;
	
	romfilePtr = (char *)value;
	length = strlen(value);
	if( length > 0x14000){
		tc_printf("\r\nconfig file size > 64K!! length=", length);
		return -1;
	}

	/*calculate and compare the replaced string ,get the max one */
	leftLen = sizeof(LEFT_REPLACE) -1;
	rightLen = sizeof(RIGHT_REPLACE) -1;

	/*compare the two replace string*/
	if(strcmp(LEFT_REPLACE, RIGHT_REPLACE) == 0)
		identicalFlag = 1;
	
	/*allocate memory for buf*/
	buf = (char *)os_malloc(length+1); 
	if(buf == NULL){
		tc_printf("\r\nAllocate rom-t memory error!");
		return -1;
	}
	memset(buf, 0, length+1);

	/*parser the received data, change back to '<' and '>'*/
	while(i <= length){
		if(identicalFlag == 0){//the left replace stirng is difference from the right one
			if(strncmp(&romfilePtr[i], LEFT_REPLACE, leftLen) == 0){
			buf[j] = '<';
				i += leftLen;
			}
			else if(strncmp(&romfilePtr[i], RIGHT_REPLACE, rightLen) == 0){
				buf[j] = '>';
				i += rightLen;
			}
			else{
				buf[j] = romfilePtr[i];
				i++;
			}
			j++;
		}
		else{//If the left replace string is identical with  the right one, we will use leftOrRightFlag to distinguish			
			if((strncmp(&romfilePtr[i], LEFT_REPLACE, leftLen) == 0) && (leftOrRightFlag == LEFT_FLAG)){
				buf[j] = '<';
				i += leftLen;
			        leftOrRightFlag = RIGHT_FLAG;
		        }
			else if((strncmp(&romfilePtr[i], RIGHT_REPLACE, rightLen) == 0) && (leftOrRightFlag == RIGHT_FLAG)){
		        	buf[j] = '>';
				i += rightLen;
			        leftOrRightFlag = LEFT_FLAG;
		        }
		        else{
			        buf[j] = romfilePtr[i];
			        i++;
		        }
		        j++;
	        }

	}
	buf[j] = '\0';			

	/*write to fs */
	fp = fopen(DOWNLOAD_CONFIG_FILE_PATH, "wb");
	if(fp == NULL){
		tc_printf("\r\nconfigfile write: Can't open file %s\n", DOWNLOAD_CONFIG_FILE_PATH);
		os_free(buf);
		buf = NULL;
		return -1;
	}
	
	bytes = (length+1) *sizeof(char);
	fwrite(buf, bytes, 1, fp);
	fclose(fp);

	/*remeber to free  memory */
	if(buf != NULL){
		os_free(buf);
		buf = NULL;
	}

	/*For delay commit*/
	SetCfgNodeFlag( "ConfigFile", -1 );
	
	return 0;
}

int8 getConfigFileAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcDevConfigAttr.aConfigFile;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8   setConfigFileAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcDevConfigAttr.aConfigFile;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= DEVCONFIG_ATTR_FLAG;

	return result;
}

