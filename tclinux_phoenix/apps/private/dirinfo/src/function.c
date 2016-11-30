#include "function.h"
#include <json.h>
#include <dirent.h>
#include "dirfile.h"
#include <json.h>
#include "threadpool.h"
#include <unistd.h>
#include <fcntl.h>
#include <assert.h>
#include <iconv.h>
#include <sys/statfs.h>

#undef  PIC_DIR
#define PIC_DIR    "_CT_Logic_picture_"
#undef  VID_DIR
#define VID_DIR	   "_CT_Logic_video_"
#undef  AUD_DIR
#define AUD_DIR    "_CT_Logic_audio_"
#define DEV_NUM     10
#define NTFS        "NTFS"
#define FAT32       "FAT32"


extern list_ctl_t glist_ctl;
static char gmnt_root_dir[DEV_NUM][128];
static char gmnt_dev[DEV_NUM][128];
static int  gmnt_num = 0;
static pthread_mutex_t gFor_lock = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t gChg_lock = PTHREAD_MUTEX_INITIALIZER;
//static pthread_mutex_t gVed_lock = PTHREAD_MUTEX_INITIALIZER;

bool fileaccess(char *src) {
	while((*(src+strlen(src)-1) == '/') && strlen(src)) {
		*(src+strlen(src)-1) = '\0';
	}
	if(!strlen(src) || access(src, F_OK)) {
		return false;
	}
	return true;
}
char *create_fail(char *msg, char *cmdType, char *indexNum) {
	int buflen = strlen(msg)+strlen(cmdType)+strlen(indexNum)+70;
	char *buf  = malloc(buflen);
	int index = 0;

	if(!buf) {
		print(DIR_ERR, "%s====>malloc fail! %d\n", __FUNCTION__, __LINE__);
		return NULL;
	}
	index += sprintf(buf+index, "{\"CmdType\":\"%s\",", cmdType);
	index += sprintf(buf+index, "\"SequenceId\":\"%s\",", indexNum);
	index += sprintf(buf+index, "\"Status\":\"1\",");
	index += sprintf(buf+index, "\"FailReason\":\"%s\"}", msg);

	return buf;
}

/*
	create logic tree when usb storage mount
*/
char *createMountList(char *cmd) { 
    
	json_t *root = NULL;
	char *msgId = NULL, *res =  NULL, *mnt_dir = NULL, *mnt_dev = NULL;
#if 1
	if(JSON_OK != json_parse_document(&root, cmd)) {
		print(DIR_ERR, "%s====>%s/%d json parse error!\n", __FILE__, __FUNCTION__, __LINE__);
		goto createmountexit;
	}
	mnt_dir = json_find_value(root, "MNT_DIR");
	mnt_dev = json_find_value(root, "MNT_DEV");
	if(!mnt_dir || !mnt_dev) {
		print(DIR_ERR, "%s====>%s/%d find json value fail!\n", __FILE__, __FUNCTION__, __LINE__);
		goto createmountfail;
	}

	pthread_mutex_lock(&glist_ctl.sema);
	if(!gmnt_num) {
		strcpy(gmnt_root_dir[gmnt_num], mnt_dir);
		strcpy(gmnt_dev[gmnt_num++], mnt_dev);
	}else {
		int index = 0;
		for(index = 0; index<gmnt_num; index++) {
			if(!strcmp(gmnt_root_dir[index], mnt_dir)) {
				break;
			}
		}
		if(index < gmnt_num) {
			//delFileDir(mnt_dir);
			//strcpy(gmnt_dev[index], mnt_dev);
			print(DIR_ERR, "%s====>%s/%d mount dir busy!\n", __FILE__, __FUNCTION__, __LINE__);
			pthread_mutex_unlock(&glist_ctl.sema);
			goto createmountfail;
		}else if(gmnt_num < DEV_NUM) {
			strcpy(gmnt_root_dir[gmnt_num], mnt_dir);
			strcpy(gmnt_dev[gmnt_num++], mnt_dev);
		}else {
			print(DIR_ERR, "%s====>%s/%d mount num over max!\n", __FILE__, __FUNCTION__, __LINE__);
			pthread_mutex_unlock(&glist_ctl.sema);
			goto createmountfail;
		}
	}
#endif
#if 0
	pthread_mutex_lock(&glist_ctl.sema);
	if(glist_ctl.flags == 1) {
		removelist();
		glist_ctl.flags = 0;
	}
#endif
	threadpool_init(&gPool, MAX_THREAD_NUM, threadpoolroutine);
    pwork_data_t work = malloc(sizeof(work_data_t));
	if(!work) {
		print(DIR_ERR, "%s====>%s/%d alloc mem err!\n", __FILE__, __FUNCTION__, __LINE__);
		pthread_mutex_unlock(&glist_ctl.sema);
		goto createmountfail;
	}
	strcpy(work->name, mnt_dir);
	threadpool_add(&gPool, createlist, work);
	pthread_mutex_lock(&glist_ctl.sema);
    glist_ctl.flags = 1;
	//pthread_cond_signal(&glist_ctl.con);
	pthread_mutex_unlock(&glist_ctl.sema);
	
createmountfail:
	json_free_value(&root);
createmountexit:
	return res;

}

int ltob(int a) {
	int tmp = 0;
	int i=0;

	while(i<4) {
		tmp <<= 8;
		tmp += a&0xff;
		a >>= 8;
		i++;
	}
	return tmp;
}


/*
	mnt_dev      dev_node
	name           if NULL, change dev name else  get dev name
	newname   NULL  get dev name
	                    NOT NULL change name
**/
int judgeFSType(char *mnt_dev, char *mnt_dir, char *name, char *newname) {
	int fileType = -1;
	int fd;
	char type[20] = {0};
	char tmpname[40] = {0};
	char utfname[80] = {0}, locname[200] = {0};
	char *tmpdata = NULL;
	int  tmplen = 0, tmpindex = 0, tmpuse = 0, tmpattrlen = 0, tmpattrindex = 0, nameadd = 0;
	char *putfname = utfname, *plocname = locname;
	int  utflen = 0, loclen = 0;
	int sectnum = 0, resect = 0, fatnum = 0,  fatsect=0,                 //sect large, reserve sect, number of fat
	    clustersect = 0,indexcluster = 0, nameindex = 0, res = 0;	      //root dirtecy cluster
	iconv_t cd;
	size_t volumeoff = 0, ret = 0;

	/**
		decide filesystem type
	*/
	fd = open(mnt_dev, O_RDWR);
	if(fd < 0) {
		print(DIR_ERR, "%s====>%s/%d open dev fail!\n", __FILE__, __FUNCTION__, __LINE__);
		goto judgeFSTypeexit;
	}
	lseek(fd, 3, SEEK_SET);
	if(read(fd, type, 4) != 4) {
		print(DIR_ERR, "%s====>%s/%d read dev fail!\n", __FILE__, __FUNCTION__, __LINE__);
		goto judgeFSTypefail;
	}
	if(!strcmp(type, NTFS)) {
		fileType = 1;
	}
	lseek(fd, 0x52, SEEK_SET);
	memset(type, 0, sizeof(type));
	if(read(fd, type, 5) != 5) {
		print(DIR_ERR, "%s====>%s/%d read dev fail!\n", __FILE__, __FUNCTION__, __LINE__);
		goto judgeFSTypefail;
	}
	if(!strcmp(type, FAT32)) {
		fileType = 0;
	}
	if(fileType == -1) {
		print(DIR_ERR, "%s====>%s/%d fs not support\n", __FILE__, __FUNCTION__, __LINE__);
		goto judgeFSTypefail;

	}
	if(fileType == 0) {   // FAT32
		lseek(fd, 0x0b, SEEK_SET);
		res += read(fd, &sectnum, 2);
		sectnum = ltob(sectnum);
		lseek(fd, 0x0d, SEEK_SET);
		res += read(fd, &clustersect, 1);
		clustersect = ltob(clustersect);
		lseek(fd, 0x0e, SEEK_SET);
		res += read(fd, &resect, 2);
		resect = ltob(resect);
		lseek(fd, 0x10, SEEK_SET);
		res += read(fd, &fatnum, 1);
		fatnum = ltob(fatnum);
		lseek(fd, 0x24, SEEK_SET);
		res += read(fd, &fatsect, 4);
		fatsect = ltob(fatsect);
		lseek(fd, 0x2c, SEEK_SET);
		res += read(fd, &indexcluster, 4);
		indexcluster = ltob(indexcluster);
		if(res != 14) {
			print(DIR_ERR, "%s====>%s/%d read err!\n", __FILE__, __FUNCTION__, __LINE__);
			fileType = -2;
			goto judgeFSTypefail;
		}
		nameindex = (resect+fatsect*fatnum+(indexcluster-2)*clustersect)*sectnum;
		print(DIR_INF, "%s===>%s/%d sectnum = %x, resect = %x, fatnum = %x, fatsect = %x, clustersect = %x, indexcluster = %x, nameindex = %x\n",
			  __FILE__, __FUNCTION__, __LINE__, sectnum, resect, fatnum, fatsect, clustersect, indexcluster, nameindex);
			
	}
	/*
		get dev name
	*/
	if(name) {
		if(fileType == 0) {
			lseek(fd, nameindex, SEEK_SET);
			read(fd, tmpname, 11);
			cd = iconv_open("utf-8", "GBK");
			if(cd < 0) {
				print(DIR_ERR, "%s====>%s/%d iconv_open fail!\n", __FILE__, __FUNCTION__, __LINE__);
				fileType = -3;
			    goto judgeFSTypefail;
			}
			if(1 != sscanf(tmpname, "%s", locname)) {
				strcpy(locname, "USB_disc1");
			}
			loclen = strlen(locname);
			utflen = sizeof(utfname);
			if((iconv(cd, &plocname, &loclen, &putfname, &utflen)<0) || (loclen!=0)) {
				print(DIR_ERR, "%s====>%s/%d iconv fail! loclen = %d\n", __FILE__, __FUNCTION__, __LINE__, loclen);
				fileType = -3;
				goto judgeFSTypefail;

			}
			iconv_close(cd);
			strcpy(name, utfname);
		}
		else {
			 int ffd;
			 sprintf(locname, "/bin/umount %s", mnt_dir);
			 system(locname);
			 sprintf(locname, "/userfs/bin/ntfslabel %s > /tmp/usb", mnt_dev);
			 ret = system(locname);
			 if(ret < 0) {
				print(DIR_ERR, "%s====>%s/%d ntfslabel fail!\n", __FILE__, __FUNCTION__, __LINE__);
				fileType = -4;
				goto judgeFSTypefail;
			 }
			 memset(locname, 0, sizeof(locname));
			 ffd = open("/tmp/usb", O_RDONLY);
			 if(ffd < 0) {
				print(DIR_ERR, "%s====>%s/%d open fail!\n", __FILE__, __FUNCTION__, __LINE__);
				fileType = -4;
				goto judgeFSTypefail;
			 }
			 if(read(ffd, locname, sizeof(locname))<0) {
				print(DIR_ERR, "%s====>%s/%d read fail!\n", __FILE__, __FUNCTION__, __LINE__);
				fileType = -4;
				goto judgeFSTypefail;
			 }
			 close(ffd);
			 if(strlen(locname)) {
				locname[strlen(locname)-1] = '\0';
			 }
			 if(!strlen(locname)) {
				strcpy(name, "USB_disc1");
			 }else {
				strcpy(name, locname);
	         }
			 sprintf(locname, "/userfs/bin/ntfs-3g %s %s", mnt_dev, mnt_dir);
			 ret = system(locname);
			 if(ret) {
				 print(DIR_ERR, "%s====>%s/%d mount fail!\n", __FILE__, __FUNCTION__, __LINE__);
				 fileType = -4;
				 goto judgeFSTypefail;

			 }
	    }
		
	}
	if(newname) {
		memset(tmpname, 0x20, sizeof(tmpname));
		if(fileType == 0) {
			cd = iconv_open("GBK", "utf-8");
			if(cd < 0) {
				print(DIR_ERR, "%s====>%s/%d iconv_open fail!\n", __FILE__, __FUNCTION__, __LINE__);
				fileType = -3;
			    goto judgeFSTypefail;
			}
			utflen = strlen(newname);
			strcpy(utfname, newname);
			loclen = sizeof(locname);
			if((iconv(cd, (char **)&putfname, (size_t *)&utflen, (char **)&plocname, (size_t *)&loclen)<0) || (utflen!=0)) {
				print(DIR_ERR, "%s====>%s/%d iconv fail! utflen = %d\n", __FILE__, __FUNCTION__, __LINE__, utflen);
				fileType = -3;
				goto judgeFSTypefail;

			}
			iconv_close(cd);
			if(strlen(locname) > 11) {
				fileType = -3;
				goto judgeFSTypefail;
			}
			lseek(fd, nameindex, SEEK_SET);
			memcpy(tmpname, locname, strlen(locname));
			write(fd, tmpname, 11);
		}
		else {
			sprintf(locname, "/bin/umount %s", mnt_dir);
			system(locname);
            sprintf(locname, "/userfs/bin/ntfslabel '%s' '%s'", mnt_dev, newname);
			ret = system(locname);
			if(ret < 0) {
				print(DIR_ERR, "%s====>%s/%d ntfslabel fail!\n", __FILE__, __FUNCTION__, __LINE__);
				fileType = -4;
				goto judgeFSTypefail;
			 }
			sprintf(locname, "/userfs/bin/ntfs-3g %s %s", mnt_dev, mnt_dir);
			ret = system(locname);
			if(ret < 0) {
				print(DIR_ERR, "%s====>%s/%d mount fail!\n", __FILE__, __FUNCTION__, __LINE__);
				fileType = -4;
				goto judgeFSTypefail;
			}
		}
		
	}
judgeFSTypefail:
	close(fd);
judgeFSTypeexit:
	return fileType;
	
}
void addDisInf(char *mnt_dir, char *mnt_dev, char **dirlist, int *num, int *listlen) {
	char  u8name[80] = {0};
	int fileType = -1;      // filesystem type 0 fat32    1 ntfs
	char *pdirlist = NULL;
	int totalsize = 0, freesize = 0;
	struct statfs fsstat;
	char *index = NULL;
	int usbnum = 0;
	
	assert((mnt_dir!=NULL) && (mnt_dev!=NULL) && 
		  (dirlist!=NULL) && (num!=NULL) && (listlen!=NULL));
	assert((*num>=0) && (*listlen>=0));
	print(DIR_INF, "%s====>%s/%d mnt_dir = %s, mnt_dev = %s, *num = %d, *listlen = %d\n", __FILE__, __FUNCTION__, __LINE__,
		mnt_dir, mnt_dev, *num, *listlen);
	if(*num + 200 > *listlen) {
		pdirlist = realloc(*dirlist, *listlen+1000);
		if(!pdirlist) {
			print(DIR_ERR, "%s====>%s/%d realloc fail!\n", __FILE__, __FUNCTION__, __LINE__);
			if(*dirlist)  {
				free(*dirlist);
				*dirlist = NULL;
			}
			return;
		}
		*dirlist = pdirlist; 
		*listlen += 1000;
	}
	if(statfs(mnt_dir, &fsstat) < 0) {
		print(DIR_ERR, "%s====>%s/%d stat fail!\n", __FILE__, __FUNCTION__, __LINE__);  //if one fail, not effect others
		return;
	}
	fileType = judgeFSType(mnt_dev, mnt_dir, u8name, NULL);
	if(fileType < 0) {
		print(DIR_ERR, "%s====>%s/%d judgeFSType fail!\n", __FILE__, __FUNCTION__, __LINE__);  //if one fail, not effect others
		return;
	}
	index = strrchr(mnt_dir, '_');
	if(!index || !(++index)) {
		print(DIR_ERR, "%s====>%s/%d mount name err!\n", __FILE__, __FUNCTION__, __LINE__);
		return;
	}
	usbnum = strtol(index, NULL, 10);
	*num += sprintf(*dirlist+*num, "{\"DiscName\":\"%s\",", u8name);
	*num += sprintf(*dirlist+*num, "\"ConnectInterface\":\"USB%d\",", usbnum);
	*num += sprintf(*dirlist+*num, "\"TotalSize\":\"%lld\",", (long long)fsstat.f_bsize*fsstat.f_blocks>>10);
	*num += sprintf(*dirlist+*num, "\"FreeSize\":\"%lld\",", (long long)fsstat.f_bsize*fsstat.f_bfree>>10);
	*num += sprintf(*dirlist+*num, "\"UsedSize\":\"%lld\",", (long long)fsstat.f_bsize*(fsstat.f_blocks-fsstat.f_bfree)>>10);
	*num += sprintf(*dirlist+*num, "\"MountPath\":\"%s\"},", mnt_dir);

}
char *getMountList(char * cmd) {
	json_t *root = NULL;
	char *res = NULL, *cmdType = NULL, *msgId = NULL, *dislist = NULL;
	int index = 0, num = 0, listlen = 0;	

	pthread_mutex_lock(&gChg_lock);
	if(JSON_OK != json_parse_document(&root, cmd)) {
		print(DIR_ERR, "%s====>%s/%d json parse err!\n", __FILE__, __FUNCTION__, __LINE__);
		goto getMountListexit;
	}
	cmdType  = json_find_value(root, "CmdType");
	msgId    = json_find_value(root, "SequenceId");
	
	//decide  filesystem type
#if 0
	pthread_mutex_lock(&glist_ctl.sema);
	while(glist_ctl.flags == 0) {
		pthread_cond_wait(&glist_ctl.con, &glist_ctl.sema);
	}
	pthread_mutex_unlock(&glist_ctl.sema);
#endif
    if(glist_ctl.flags == 0) {
		print(DIR_ERR, "%s====>%s/%d wait mounting!\n", __FILE__, __FUNCTION__, __LINE__);
		res = create_fail("wait mounting", cmdType, msgId);
		goto getMountListfail;		
	}
	pthread_mutex_lock(&glist_ctl.sema);
	for(index=0; index<gmnt_num; index++) {
		addDisInf(gmnt_root_dir[index], gmnt_dev[index], &dislist, &num, &listlen);
		if(!dislist) {
			print(DIR_ERR, "%s====>%s/%d addDisInf error!\n", __FILE__, __FUNCTION__, __LINE__);
			res = create_fail("addDisInf error", cmdType, msgId);
			pthread_mutex_unlock(&glist_ctl.sema);
			goto getMountListfail;
		}
	}
	pthread_mutex_unlock(&glist_ctl.sema);
	if(dislist && (num>0))
	dislist[num-1] = '\0';
	res = malloc(150 + num);
	if(!res) {
		print(DIR_ERR, "%s====>%s/%d alloc mem fail!\n", __FILE__, __FUNCTION__, __LINE__);
		res = create_fail("alloc mem fail", cmdType, msgId);
		goto getMountListfail;
	}
	index = 0;
	index += sprintf(res+index, "{\"CmdType\":\"%s\",", cmdType);
	index += sprintf(res+index, "\"SequenceId\":\"%s\",", msgId);
	index += sprintf(res+index, "\"Result\":\"0\",");
	index += sprintf(res+index, "\"DiscList\":[%s]}", (num>0) ? dislist : "");
	
getMountListfail:
	if(dislist) free(dislist);
	json_free_value(&root);
getMountListexit:
	pthread_mutex_unlock(&gChg_lock);
	return res;
	
}
char *getFileNum(char *cmd) {
	json_t *root = NULL;
	char *res = NULL, *cmdType = NULL, *mount = NULL, *msgId = NULL;
	int index = 0, filenum = 0;

	pthread_mutex_lock(&gChg_lock);
	if(JSON_OK != json_parse_document(&root, cmd)) {
		print(DIR_ERR, "%s====> json format error!\n", __FUNCTION__);
		res= NULL;
		goto filenumexitfail;
	}
	cmdType = json_find_value(root, "CmdType");
	mount   = json_find_value(root, "FolderName");
	msgId   = json_find_value(root, "SequenceId");
	if(!cmdType || !mount || !msgId) {
		print(DIR_ERR, "%s====>json_find_value error!\n", __FUNCTION__);
		res = NULL;
		goto filenumexit;
	}
	if(!strcmp(mount, "_CT_Logic_picture_")) {
		#if 0
		if(pthread_mutex_trylock(&glist_ctl.sema)) {
			res = create_fail("wait mounting...!", cmdType, msgId);
			goto filenumexit;
		}
		
		while(glist_ctl.flags == 0) { 
			pthread_cond_wait(&glist_ctl.con, &glist_ctl.sema);	
		}
		#endif
		if(glist_ctl.flags != 0) {
			pthread_mutex_lock(&glist_ctl.sema);
		    filenum = getDirNum(PIC);
		    pthread_mutex_unlock(&glist_ctl.sema);
		}else {
			res = create_fail("wait mounting...!", cmdType, msgId);
			goto filenumexit;
		}
		//pthread_mutex_unlock(&glist_ctl.sema);
	}
	else if(!strcmp(mount, "_CT_Logic_audio_")) {
		#if 0
		if(pthread_mutex_trylock(&glist_ctl.sema)) {
			res = create_fail("wait mounting...!", cmdType, msgId);
			goto filenumexit;
		}
		
		pthread_mutex_lock(&glist_ctl.sema);
		while(glist_ctl.flags == 0) { 
			pthread_cond_wait(&glist_ctl.con, &glist_ctl.sema);	
		}
		#endif
		if(glist_ctl.flags != 0) {
			pthread_mutex_lock(&glist_ctl.sema);
		    filenum = getDirNum(AUD);
			pthread_mutex_unlock(&glist_ctl.sema);
		}else {
			res = create_fail("wait mounting...!", cmdType, msgId);
			goto filenumexit;
		}
		//pthread_mutex_unlock(&glist_ctl.sema);
	}
	else if(!strcmp(mount, "_CT_Logic_video_")) {
		#if 0
		if(pthread_mutex_trylock(&glist_ctl.sema)) {
			res = create_fail("wait mounting...!", cmdType, msgId);
			goto filenumexit;
		}
		
		pthread_mutex_lock(&glist_ctl.sema);
		while(glist_ctl.flags == 0) { 
			pthread_cond_wait(&glist_ctl.con, &glist_ctl.sema);	
		}
		#endif
		if(glist_ctl.flags != 0) {
			pthread_mutex_lock(&glist_ctl.sema);
		    filenum = getDirNum(VED);
			pthread_mutex_unlock(&glist_ctl.sema);
		}else {
			res = create_fail("wait mounting...!", cmdType, msgId);
			goto filenumexit;
		}
		//pthread_mutex_unlock(&glist_ctl.sema);
	}else {
		if(0 == strcmp(mount, "")) {
			mount = gmnt_root_dir[0];
		}
		if(!fileaccess(mount)) {
			print(DIR_ERR, "%s====>%s/%d file name error!\n", __FILE__, __FUNCTION__, __LINE__);
			res = create_fail("file name error!", cmdType, msgId);
			goto filenumexit;
	    }
		DIR *dir = opendir(mount);
		if(dir == NULL) {
			print(DIR_ERR, "%s====> opendir err! %d\n", __FUNCTION__, __LINE__);
			res = create_fail("directory open fail!", cmdType, msgId);
			goto filenumexit;
		}
		struct dirent *file;
		while((file=readdir(dir))!=NULL) {
			if(!strcmp(file->d_name, ".") || !strcmp(file->d_name, ".."))
				continue;
			filenum++;
		}
		closedir(dir);
	}
	res = malloc(100*sizeof(char));
	if(!res) {
		print(DIR_ERR, "%s====> malloc fail %d\n", __FUNCTION__, __LINE__);
		goto filenumexit;
	}
	index += sprintf(res+index, "{\"CmdType\":\"%s\",", cmdType);
	index += sprintf(res+index, "\"SequenceId\":\"%s\",", msgId);
	index += sprintf(res+index, "\"Status\":\"0\",");
	index += sprintf(res+index, "\"FileNum\":\"%d\"}", filenum);
	
filenumexit:
	json_free_value(&root);
filenumexitfail:
	pthread_mutex_unlock(&gChg_lock);
	return res;
}
char *getDirList(char *cmd) {
	json_t *root = NULL;
	char *res = NULL, *cmdType = NULL, *mount = NULL, *msgId = NULL,
		 *startI = NULL, *endI = NULL, *strlist = NULL;
	int startindex = 0, endindex = 0, num = 0, listlen = 0, index = 0;

	pthread_mutex_lock(&gChg_lock);
	print(DIR_INF, "%s====>%s/%d cmd = %s\n", __FILE__, __FUNCTION__, __LINE__, cmd);
	if(JSON_OK != json_parse_document(&root, cmd)) {
		print(DIR_ERR, "%s====>json format error! %d\n", __FUNCTION__, __LINE__);
		res = NULL;
		goto listexit;
	}
	cmdType  = json_find_value(root, "CmdType");
	mount    = json_find_value(root, "FolderName");
	msgId    = json_find_value(root, "SequenceId");
	startI   = json_find_value(root, "StartIndex");
	endI     = json_find_value(root, "EndIndex");
	print(DIR_WRN, "%s====>%s/%d 1 cmd = %s\n", __FILE__, __FUNCTION__, __LINE__, cmd);
	if(!cmdType || !mount || !msgId || !startI || !endI) {
		print(DIR_ERR, "%s====>json_find_value error!\n", __FUNCTION__);
		res = NULL;
		goto listexitfail;
	}
	startindex = atoi(startI)<1 ? 1 : atoi(startI);
	if(!strcmp(endI, ".") || !strcmp(endI, "")) {
		endindex = -1;
	}else {
		endindex   = atoi(endI)<1 ? 1 : atoi(endI);
	}
	print(DIR_WRN, "%s====>%s/%d 2 cmd = %s\n", __FILE__, __FUNCTION__, __LINE__, cmd);
	if(!strcmp(mount, "_CT_Logic_picture_")) {
		#if 0
		if(pthread_mutex_trylock(&glist_ctl.sema)) {
			res = create_fail("wait mounting...!", cmdType, msgId);
			goto listexitfail;
		}
		
		pthread_mutex_lock(&glist_ctl.sema);
		while(glist_ctl.flags == 0) { 
			pthread_cond_wait(&glist_ctl.con, &glist_ctl.sema);	
		}
		#endif
		if(glist_ctl.flags != 0) {
			pthread_mutex_lock(&glist_ctl.sema);
		    strlist = getList(PIC, startindex, endindex);
			pthread_mutex_unlock(&glist_ctl.sema);
			num     = strlen(strlist);
			/*////////////////////////////////////////
			int i = 0;
			print(DIR_INF, "%s===>%s/%d  print hex\n", __FILE__, __FUNCTION__, __LINE__);
			for(i=0; (strlist&&i<strlen(strlist)); i++) {
				printf("%02x ", strlist[i]&0xff);
			}
			print(DIR_INF, "%s===>%s/%d  print hex end\n", __FILE__, __FUNCTION__, __LINE__);
			///////////////////////////////////////////*/

		}else {
            res = create_fail("wait mounting...!", cmdType, msgId);
			goto listexitfail;
		}
		//pthread_mutex_unlock(&glist_ctl.sema);
	}
	else if(!strcmp(mount, "_CT_Logic_audio_")){
		#if 0
		if(pthread_mutex_trylock(&glist_ctl.sema)) {
			res = create_fail("wait mounting...!", cmdType, msgId);
			goto listexitfail;
		}
		
		pthread_mutex_lock(&glist_ctl.sema);
		while(glist_ctl.flags == 0) { 
			pthread_cond_wait(&glist_ctl.con, &glist_ctl.sema);	
		}
		#endif
		if(glist_ctl.flags != 0) {
			 pthread_mutex_lock(&glist_ctl.sema);
		     strlist = getList(AUD, startindex, endindex);
			 pthread_mutex_unlock(&glist_ctl.sema);
			 num     = strlen(strlist);
			 /*////////////////////////////////////////
			int i = 0;
			print(DIR_INF, "%s===>%s/%d  print hex\n", __FILE__, __FUNCTION__, __LINE__);
			for(i=0; (strlist&&i<strlen(strlist)); i++) {
				printf("%02x ", strlist[i]&0xff);
			}
			print(DIR_INF, "%s===>%s/%d  print hex end\n", __FILE__, __FUNCTION__, __LINE__);
			///////////////////////////////////////////*/
		} else {
			res = create_fail("wait mounting...!", cmdType, msgId);
			goto listexitfail;
		}
		//pthread_mutex_unlock(&glist_ctl.sema);
	}else if(!strcmp(mount, "_CT_Logic_video_")) {
		#if 0
		if(pthread_mutex_trylock(&glist_ctl.sema)) {
			res = create_fail("wait mounting...!", cmdType, msgId);
			goto listexitfail;
		}
		
		pthread_mutex_lock(&glist_ctl.sema);
		while(glist_ctl.flags == 0) { 
			pthread_cond_wait(&glist_ctl.con, &glist_ctl.sema);	
		}
		#endif
		if(glist_ctl.flags != 0) {
			pthread_mutex_lock(&glist_ctl.sema);
		    strlist = getList(VED, startindex, endindex);
			pthread_mutex_unlock(&glist_ctl.sema);
			num     = strlen(strlist);
			/*////////////////////////////////////////
			int i = 0;
			print(DIR_INF, "%s===>%s/%d  print hex\n", __FILE__, __FUNCTION__, __LINE__);
			for(i=0; (strlist&&i<strlen(strlist)); i++) {
				printf("%02x ", strlist[i]&0xff);
			}
			print(DIR_INF, "%s===>%s/%d  print hex end\n", __FILE__, __FUNCTION__, __LINE__);
			///////////////////////////////////////////*/
		}else {
			res = create_fail("wait mounting...!", cmdType, msgId);
			goto listexitfail;
		}
		//pthread_mutex_unlock(&glist_ctl.sema);
		print(DIR_WRN, "%s====>%s/%d 1 strlist = %s\n", __FILE__, __FUNCTION__, __LINE__, strlist);
	}
	else {
		if(0 == strcmp(mount, "")) {
			mount = gmnt_root_dir[0];
		}
		if(!fileaccess(mount)) {
			print(DIR_ERR, "%s====>%s/%d file name error!\n", __FILE__, __FUNCTION__, __LINE__);
			res = create_fail("file name error!", cmdType, msgId);
			goto listexitfail;
	    }
		DIR *dir = opendir(mount);
		if(!dir) {
			print(DIR_ERR, "%s====>opendir fail! %d\n", __FUNCTION__, __LINE__);
			res = create_fail("open directory fail", cmdType, msgId);
			goto listexitfail;
		}
		print(DIR_WRN, "%s====>%s/%d 3 cmd = %s\n", __FILE__, __FUNCTION__, __LINE__, cmd);
		struct dirent *file = NULL;
		int index = 1;
		while(file=readdir(dir)) {
			print(DIR_WRN, "%s====>%s/%d 4 cmd = %s\n", __FILE__, __FUNCTION__, __LINE__, cmd);
			if(!strcmp(file->d_name, ".") || !strcmp(file->d_name, "..")) {
				continue;
			}
			if(index++<startindex) {
				continue;
			}
			print(DIR_INF, "%s====>%s/%d file name =%s\n",__FILE__, __FUNCTION__, __LINE__, file->d_name);
			addFileInfo(mount, file->d_name, &num, &strlist, &listlen);
			if(!strlist) {
				closedir(dir);
				goto listexitfail;
			}
			if((endindex>0) && (index>endindex)) {
				break;
			}
		}
		closedir(dir);
		if(strlist && (num>0))
		strlist[num-1]='\0';
	}
	listlen = num +100;
	res = malloc(listlen);
	if(!res) {
		print(DIR_ERR, "%s====>%s/%d malloc fail\n", __FILE__, __FUNCTION__, __LINE__);
		goto listexitfail;
	}
	index += sprintf(res+index, "{\"CmdType\":\"%s\",", cmdType);
	index += sprintf(res+index, "\"SequenceId\":\"%s\",", msgId);
	index += sprintf(res+index, "\"Status\":\"0\",");
	if(num > 0)
	{
		index += sprintf(res+index, "\"FileList\":[%s]}", strlist);
	}
	else 
	{
		index += sprintf(res+index, "\"FileList\":\"\"}");
	}
	
listexitfail:
	if(strlist) free(strlist);
	json_free_value(&root);
listexit:
	pthread_mutex_unlock(&gChg_lock);
	return res;
}

bool copymoveFile(char *cmd, char **rres,char **rcmdType, char **rsrc, char **rdes, char **rmsgId, json_t **rroot) {
	json_t *root = NULL;
	char *res = NULL, *cmdType = NULL, *src = NULL, *des = NULL, *msgId = NULL, *tmp = NULL;
	char buf[256] = {0};
	bool flag = true;

	if(JSON_OK != json_parse_document(&root, cmd)) {
		print(DIR_ERR, "%s====>%s/%d json parse error!\n", __FILE__, __FUNCTION__, __LINE__);
		flag = false;
		goto copymoveFileexit;
	}
	cmdType = json_find_value(root, "CmdType");
	msgId   = json_find_value(root, "SequenceId");
	src     = json_find_value(root, "FileName");
	des     = json_find_value(root, "NewFolderName");
	if(!fileaccess(src)) {
		print(DIR_ERR, "%s====>%s/%d file name error!\n", __FILE__, __FUNCTION__, __LINE__);
		res = create_fail("file name error!", cmdType, msgId);
		flag = false;
		goto copymoveFilefail;

	}
	fileaccess(des);
	if(!cmdType || !msgId || !src || !des) {
		print(DIR_ERR, "%s====>%s/%d find value error!\n", __FILE__, __FUNCTION__, __LINE__);
		res = create_fail("find json value fail", cmdType, msgId);
		flag = false;
		goto copymoveFilefail;
	}
	*rcmdType = cmdType;
	*rsrc     = src;
	*rdes     = des;
	*rmsgId   = msgId;
	*rroot    = root;
	tmp = strrchr(src, '/');
	if(!tmp || !(++tmp)) {
		print(DIR_ERR, "%s====>%s/%d find file error!\n", __FILE__, __FUNCTION__, __LINE__);
		res = create_fail("src addr error", cmdType, msgId);
		flag = false;
		goto copymoveFilefail;
	}
	sprintf(buf, "%s/%s", des, tmp);
	if(!access(buf, F_OK)) {
		print(DIR_ERR, "%s====>%s/%d des file exist!\n", __FILE__, __FUNCTION__, __LINE__);
		res = create_fail("des file exist", cmdType, msgId);
		flag = false;
		goto copymoveFilefail;
	}
	goto copymoveFileexit;
	
copymoveFilefail:
	json_free_value(&root);
copymoveFileexit:
	*rres = res;
	return flag;

}


/**
	
**/
char *copyFile(char *cmd) {
	json_t *root = NULL;
	char *res = NULL, *cmdType = NULL, *src = NULL, *des = NULL, *msgId = NULL, *tmp = NULL;
	char tmpcmd[512] = {0};
	int index = 0;

	pthread_mutex_lock(&gChg_lock);
	if(!copymoveFile(cmd, &res, &cmdType, &src, &des, &msgId, &root)) {
		print(DIR_ERR, "%s===>%s/%d copymoveFile fail!\n", __FILE__, __FUNCTION__, __LINE__);
		goto copyfileexit;
	}
	if(!copyDir(des, src)) {
		print(DIR_ERR, "%s====>%s/%d copy logic tree fail!\n", __FILE__, __FUNCTION__, __LINE__);
		res = create_fail("copy logic tree fail", cmdType, msgId);
		goto copyfilefail;
	}
	sprintf(tmpcmd, "cp '%s' '%s' -adr", src, des);
	system(tmpcmd);
	sync();
	res = malloc(200);
	if(!res) {
		print(DIR_ERR, "%s====>%s/%d malloc fail!\n", __FILE__, __FUNCTION__, __LINE__);
		res = create_fail("loc mem fail", cmdType, msgId);
		goto copyfilefail;
	}
	index += sprintf(res+index, "{\"CmdType\":\"%s\",", cmdType);
	index += sprintf(res+index, "\"SequenceId\":\"%s\",", msgId);
	index += sprintf(res+index, "\"Status\":\"0\"}");
	
copyfilefail:
	json_free_value(&root);
copyfileexit:
	pthread_mutex_unlock(&gChg_lock);
	return res;

}
char *moveFile(char *cmd){
	json_t *root = NULL;
	char *res = NULL, *cmdType = NULL, *src = NULL, *des = NULL, *msgId = NULL, *tmp = NULL;
	char tmpcmd[512] = {0};
	int index = 0;

	pthread_mutex_lock(&gChg_lock);
    print(DIR_INF, "%s====>%s/%d cmd = %s\n", __FILE__, __FUNCTION__, __LINE__,  cmd);
	if(!copymoveFile(cmd, &res, &cmdType, &src, &des, &msgId, &root)) {
		print(DIR_ERR, "%s===>%s/%d copymoveFile fail!\n", __FILE__, __FUNCTION__, __LINE__);
		goto moveFileexit;
	}
	print(DIR_INF, "%s====>%s/%d src = %s des = %s\n", __FILE__, __FUNCTION__, __LINE__,  src, des);
	if(!moveDir(des, src)) {
		print(DIR_ERR, "%s====>%s/%d move logic tree fail!\n", __FILE__, __FUNCTION__, __LINE__);
		res = create_fail("move logic tree fail", cmdType, msgId);
		goto moveFilefail;

	}
	sprintf(tmpcmd, "mv '%s' '%s'", src, des);
	system(tmpcmd);	
	sync();
	res = malloc(200);
	if(!res) {
		print(DIR_ERR, "%s====>%s/%d malloc fail!\n", __FILE__, __FUNCTION__, __LINE__);
		res = create_fail("loc mem fail", cmdType, msgId);
		goto moveFilefail;
	}
	index += sprintf(res+index, "{\"CmdType\":\"%s\",", cmdType);
	index += sprintf(res+index, "\"SequenceId\":\"%s\",", msgId);
	index += sprintf(res+index, "\"Status\":\"0\"}");

moveFilefail:
	json_free_value(&root);
moveFileexit:
	pthread_mutex_unlock(&gChg_lock);
	return res;
}
char *delFile(char *cmd){
	json_t *root = NULL;
	char *res = NULL, *cmdType = NULL, *src = NULL, *msgId = NULL;
	char buf[256] = {0};
	int index = 0;

    pthread_mutex_lock(&gChg_lock);
	if(JSON_OK != json_parse_document(&root, cmd)) {
		print(DIR_ERR, "%s====>%s/%d json parse error!\n", __FILE__, __FUNCTION__, __LINE__);
		goto delFileexit;
	}
	cmdType = json_find_value(root, "CmdType");
	msgId	= json_find_value(root, "SequenceId");
	src 	= json_find_value(root, "FileName");
	if(!fileaccess(src)) {
		print(DIR_ERR, "%s====>%s/%d file name error!\n", __FILE__, __FUNCTION__, __LINE__);
		res = create_fail("file name error!", cmdType, msgId);
		goto delFilefail;
	}
	if(!cmdType || !msgId || !src) {
		print(DIR_ERR, "%s====>%s/%d find value error!\n", __FILE__, __FUNCTION__, __LINE__);
		res = create_fail("find json value fail", cmdType, msgId);
		goto delFilefail;
	}
	if(!delFileDir(src)) {
		print(DIR_ERR, "%s====>%s/%d del logic tree fail!\n", __FILE__, __FUNCTION__, __LINE__);
		res = create_fail("del logic tree fail", cmdType, msgId);
		goto delFilefail;
	}
	sprintf(buf, "rm '%s' -rf", src);
	system(buf);
	sync();
	res = malloc(200);
	if(!res) {
		print(DIR_ERR, "%s====>%s/%d alloc mem fail!\n", __FILE__, __FUNCTION__, __LINE__);
		res = create_fail("alloc mem fail", cmdType, msgId);
		goto delFilefail;
	}
	index += sprintf(res+index, "{\"CmdType\":\"%s\",", cmdType);
	index += sprintf(res+index, "\"SequenceId\":\"%s\",", msgId);
	index += sprintf(res+index, "\"Status\":\"0\"}");
	
delFilefail:
	json_free_value(&root);
delFileexit:
	pthread_mutex_unlock(&gChg_lock);
	return res;

}
char *renameFile(char *cmd){
	json_t *root = NULL;
	char *res = NULL, *cmdType = NULL, *src = NULL, *des = NULL, *msgId = NULL;
	char buf[256] = {0};
	int index = 0;

	pthread_mutex_lock(&gChg_lock);
	if(JSON_OK != json_parse_document(&root, cmd)) {
		print(DIR_ERR, "%s====>%s/%d json parse error!\n", __FILE__, __FUNCTION__, __LINE__);
		goto renameFileexit;

	}
	cmdType = json_find_value(root, "CmdType");
	src     = json_find_value(root, "CurrFileName");
	des     = json_find_value(root, "NewFileName");
	msgId   = json_find_value(root, "SequenceId");
	if(!fileaccess(src)) {
		print(DIR_ERR, "%s====>%s/%d file name error!\n", __FILE__, __FUNCTION__, __LINE__);
		res = create_fail("file name error!", cmdType, msgId);
		goto renameFilefail;

	}
	fileaccess(des);
	if(!cmdType || !src || !des || !msgId) {
		print(DIR_ERR, "%s====>%s/%d find value error!\n", __FILE__, __FUNCTION__, __LINE__);
		res = create_fail("find json value fail", cmdType, msgId);
		goto renameFilefail;
	}
	if(!access(des, F_OK)) {
		print(DIR_ERR, "%s====>%s/%d des file exist", __FILE__, __FUNCTION__, __LINE__);
		res = create_fail("des file exist", cmdType, msgId);
		goto renameFilefail;
	}
	if(!renamelogic(des, src)) {
		print(DIR_ERR, "%s====>%s/%d rename logic tree error!\n", __FILE__, __FUNCTION__, __LINE__);
		res = create_fail("rename logic tree error", cmdType, msgId);
		goto renameFilefail;
	}
    sprintf(buf, "mv '%s' '%s'", src, des);
	system(buf);
	sync();
	res = malloc(200);
	if(!res) {
		print(DIR_ERR, "%s====>%s/%d alloc mem error!\n", __FILE__, __FUNCTION__, __LINE__);
		res = create_fail("alloc mem error", cmdType, msgId);
		goto renameFilefail;
	}
	index += sprintf(res+index, "{\"CmdType\":\"%s\",", cmdType);
	index += sprintf(res+index, "\"SequenceId\":\"%s\",", msgId);
	index += sprintf(res+index, "\"Status\":\"0\"}");
	
renameFilefail:
	json_free_value(&root);
renameFileexit:
	pthread_mutex_unlock(&gChg_lock);
	return res;
}
char *createFile(char *cmd){
	json_t *root = NULL;
	char *res = NULL, *cmdType = NULL, *src = NULL, *msgId = NULL;
	char buf[256] = {0}, tmpcmd[256] = {0};
	int index = 0;

	pthread_mutex_lock(&gChg_lock);
	if(JSON_OK != json_parse_document(&root, cmd)) {
		print(DIR_ERR, "%s====>%s/%d json parse err!\n", __FILE__, __FUNCTION__, __LINE__);
		goto createFileexit;
	}
	cmdType = json_find_value(root, "CmdType");
	src     = json_find_value(root, "FolderName");
	msgId   = json_find_value(root, "SequenceId");
	if(!cmdType || !src || !msgId) {
		print(DIR_ERR, "%s====>%s/%d  find value error!\n", __FILE__, __FUNCTION__, __LINE__);
		res = create_fail("find json value fail", cmdType, msgId);
		goto createFilefail;
	}
	if(!strncmp(PIC_DIR, src, strlen(PIC_DIR))) {
		sprintf(buf, "%s/%s", gmnt_root_dir[0], src+strlen(PIC_DIR));
	}else if(!strncmp(VID_DIR, src, strlen(VID_DIR))) {
		sprintf(buf, "%s/%s", gmnt_root_dir[0], src+strlen(VID_DIR));
	}else if(!strncmp(AUD_DIR, src, strlen(AUD_DIR))) {
	    sprintf(buf, "%s/%s", gmnt_root_dir[0], src+strlen(AUD_DIR));
	}
	else {
		if(0 != strncmp(src, "/tmp/mnt", strlen("/tmp/mnt")))
		{
			sprintf(buf, "%s%s", gmnt_root_dir[0], src);
		}
		else
		{
			fileaccess(src);
			strcpy(buf, src);
		}
	}
	print(DIR_INF, "%s====>%s/%d buf = %s\n", __FILE__, __FUNCTION__, __LINE__, buf);
	if(!access(buf, F_OK)) {
		print(DIR_ERR, "%s====>%s/%d file already exist!\n", __FILE__, __FUNCTION__, __LINE__);
		res = create_fail("file already exist", cmdType, msgId);
		goto createFilefail;
	}
	sprintf(tmpcmd, "mkdir '%s' -p", buf);
	system(tmpcmd);
	sync();
	res = malloc(200);
	if(!res) {
		print(DIR_ERR, "%s====>%s/%d alloc mem err!\n", __FILE__, __FUNCTION__, __LINE__);
		res = create_fail("alloc mem error", cmdType, msgId);
		goto createFilefail;
	}
	index += sprintf(res+index, "{\"CmdType\":\"%s\",", cmdType);
	index += sprintf(res+index, "\"SequenceId\":\"%s\",", msgId);
	index += sprintf(res+index, "\"Status\":\"0\"}");
	
createFilefail:
	json_free_value(&root);
createFileexit:
	pthread_mutex_unlock(&gChg_lock);
	return res;

}


/**
		unmount dev
*/
char *delMountList(char *cmd) {   
	json_t *root = NULL;
	char *res = NULL, *cmdType = NULL, *mnt = NULL, *msgId = NULL;
	int index = 0;
	int i=0, fileType = -1;
	char *dev = NULL;
	char buf[200];

	pthread_mutex_lock(&gChg_lock);
	if(JSON_OK != json_parse_document(&root, cmd)) {
		print(DIR_ERR, "%s====>%s/%d json parse err!\n", __FILE__, __FUNCTION__, __LINE__);
		goto delMountListexit;
	}
	cmdType = json_find_value(root, "CmdType");
	msgId	= json_find_value(root, "SequenceId");
	mnt     = json_find_value(root, "Mount");
	if(!cmdType || !msgId || !mnt) {
		print(DIR_ERR, "%s====>%s/%d  find value error!\n", __FILE__, __FUNCTION__, __LINE__);
		//res = create_fail("find json value fail", cmdType, msgId);
		goto delMountListfail;
	}
	fileaccess(mnt);
	for(i=0; i<gmnt_num; i++) {
		if(!strcmp(mnt, gmnt_root_dir[i])) {
			break;
		}
	}
	if(i >= gmnt_num) {
		print(DIR_ERR, "%s====>%s/%d mnt dir error!\n", __FILE__, __FUNCTION__, __LINE__);
		goto delMountListfail;
	}
	print(DIR_WRN, "%s===>%s/%d ===> 0!\n", __FILE__, __FUNCTION__, __LINE__);
	if(!removelist(mnt)) {
		print(DIR_ERR, "%s===>%s/%d deldirfile Error!\n", __FILE__, __FUNCTION__, __LINE__);
		goto delMountListfail;
	}
	print(DIR_WRN, "%s===>%s/%d ===> 1!\n", __FILE__, __FUNCTION__, __LINE__);
	sprintf(buf, "/bin/umount '%s'", mnt);
	int tmpres = system(buf);
	print(DIR_WRN, "%s===>%s/%d ===> 2 tmpres = %d!\n", __FILE__, __FUNCTION__, __LINE__, tmpres);
	res = malloc(200);
	if(!res) {
		print(DIR_ERR, "%s===>%s/%d alloc mem error!\n", __FILE__, __FUNCTION__, __LINE__);
		res= create_fail("alloc mem error!", cmdType, msgId);
		goto delMountListfail;
	}
	if(tmpres ==0) { 
		pthread_mutex_lock(&glist_ctl.sema);
		while(i<gmnt_num-1) {
			strcpy(gmnt_dev[i], gmnt_dev[i+1]);
			strcpy(gmnt_root_dir[i], gmnt_root_dir[i+1]);
		}
		gmnt_num --;
		if(gmnt_num == 0) {
			glist_ctl.flags = 0;
		}
		pthread_mutex_unlock(&glist_ctl.sema);
		index += sprintf(res+index, "{\"CmdType\":\"%s\",", cmdType);
		index += sprintf(res+index, "\"SequenceId\":\"%s\",", msgId);
		index += sprintf(res+index, "\"Result\":\"0\"}");
	}else {
		free(res);
		pthread_mutex_lock(&glist_ctl.sema);
		while(i<gmnt_num-1) {
			strcpy(gmnt_dev[i], gmnt_dev[i+1]);
			strcpy(gmnt_root_dir[i], gmnt_root_dir[i+1]);
		}
		gmnt_num --;
		if(gmnt_num == 0) {
			glist_ctl.flags = 0;
		}
		pthread_mutex_unlock(&glist_ctl.sema);
		print(DIR_ERR, "%s===>%s/%d umount error!\n", __FILE__, __FUNCTION__, __LINE__);
		res= create_fail("umount error!", cmdType, msgId);
		goto delMountListfail;
	}
	
delMountListfail:
	json_free_value(&root);
delMountListexit:
	pthread_mutex_unlock(&gChg_lock);
	return res;

}
char *renameDisc(char *cmd) {
	json_t *root = NULL;
	char *res = NULL, *cmdType = NULL, *src = NULL, *des = NULL, *mnt = NULL, *msgId = NULL;
	int index = 0;
	int i=0, fileType = -1;
	char *dev = NULL;

	pthread_mutex_lock(&gChg_lock);
	if(JSON_OK != json_parse_document(&root, cmd)) {
		print(DIR_ERR, "%s====>%s/%d json parse err!\n", __FILE__, __FUNCTION__, __LINE__);
		goto renameDiscexit;
	}
	cmdType = json_find_value(root, "CmdType");
	src 	= json_find_value(root, "CurrentDiscName");
	des     = json_find_value(root, "NewDiscName");
	msgId	= json_find_value(root, "SequenceId");
	mnt     = json_find_value(root, "Mount");
	if(!cmdType || !src || !msgId || !des || !mnt) {
		print(DIR_ERR, "%s====>%s/%d  find value error!\n", __FILE__, __FUNCTION__, __LINE__);
		res = create_fail("find json value fail", cmdType, msgId);
		goto renameDiscfail;
	}
	for(i=0; i<gmnt_num; i++) {
		if(!strcmp(mnt, gmnt_root_dir[i])) {
			break;
		}
	}
	if(i>=gmnt_num) {
		print(DIR_ERR, "%s====>%s/%d mnt dir err!\n", __FILE__, __FUNCTION__, __LINE__);
		res = create_fail("mnt dir err", cmdType, msgId);
		goto renameDiscfail;
	}
	dev = gmnt_dev[i];
	fileType = judgeFSType(dev, mnt, NULL, des);
	if(fileType < 0) {
		print(DIR_ERR, "%s====>%s/%d rename disc err!\n", __FILE__, __FUNCTION__, __LINE__);
		res = create_fail("rename disc err", cmdType, msgId);
		goto renameDiscfail;
	}
	res = malloc(100);
	if(!res) {
		print(DIR_ERR, "%s====>%s/%d alloc mem err!\n", __FILE__, __FUNCTION__, __LINE__);
		res = create_fail("alloc mem err", cmdType, msgId);
		goto renameDiscfail;
	}
	index += sprintf(res+index, "{\"CmdType\":\"%s\",", cmdType);
	index += sprintf(res+index, "\"Result\":\"0\",");
	index += sprintf(res+index, "\"SequenceId\":\"%s\"}", msgId);
renameDiscfail:
	json_free_value(&root);
renameDiscexit:
	pthread_mutex_unlock(&gChg_lock);
	return res;

}

typedef struct {
	int flags;
	char buf[100];
	int type;
	char dev[100];
	char mnt[100];

}format_ctl_t;

static format_ctl_t format_ctl;

void * format_u_rountine(void * arg) {
	format_ctl_t *pformat_ctl = arg;
	int res = 0;
	char buf[200] = {0};

	res = system(pformat_ctl->buf);
	if(res) {
		print(DIR_ERR, "%s====>%s/%d format U err!\n", __FILE__, __FUNCTION__, __LINE__);
		pformat_ctl->flags = -2;
		return;		
	}
	pformat_ctl->flags = 2;
	if(pformat_ctl->type == 0) {
		sprintf(buf, "/bin/mount -o iocharset=utf8 -t vfat '%s' '%s'", pformat_ctl->dev, pformat_ctl->mnt);
	}else{
		sprintf(buf, "/userfs/bin/ntfs-3g '%s' '%s'", pformat_ctl->dev, pformat_ctl->mnt);
	}
	
	res = system(buf);
	if(res) {
		print(DIR_ERR, "%s====>%s/%d mount U err!\n", __FILE__, __FUNCTION__, __LINE__);
		pformat_ctl->flags = -3;
		return;
	}
}

int format_u(format_ctl_t *pformat_ctl) {
	int retCode = 0;
	pthread_attr_t attr;
	pthread_t pid;
	
    pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	retCode = pthread_create(&pid, &attr, format_u_rountine, pformat_ctl);
	pthread_attr_destroy(&attr);
	if((retCode == 0) && (pthread_detach(pid) == EINVAL)) {
		return -1;
	}
	return 0;
}

char *formatDisc(char *cmd) {
	json_t *root = NULL;
	char *res = NULL, *cmdType = NULL, *pfileType = NULL, *msgId = NULL, *mnt = NULL, *name = NULL;
	int index = 0;
	int i=0, fileType = -1;
	char *dev = NULL;
	char buf[200];
	int ret = 0;
	int namelen =0;
	char gbk_disname[200], *pdisname = gbk_disname;
	int gbklen = sizeof(gbk_disname);
	iconv_t cd;

	pthread_mutex_lock(&gChg_lock);
	if(format_ctl.flags == 1) {
		print(DIR_ERR, "%s====>%s/%d formating...!\n", __FILE__, __FUNCTION__, __LINE__);
		goto formatDiscexit;
	}
	format_ctl.flags = 1;
	if(JSON_OK != json_parse_document(&root, cmd)) {
		print(DIR_ERR, "%s====>%s/%d json parse err!\n", __FILE__, __FUNCTION__, __LINE__);
		goto formatDiscexit;
	}
	cmdType     = json_find_value(root, "CmdType");
	pfileType 	= json_find_value(root, "FileType");
	msgId	    = json_find_value(root, "SequenceId");
	mnt 	    = json_find_value(root, "Mount");
	name        = json_find_value(root, "Name");
	if(!cmdType  || !msgId || !mnt) {
		print(DIR_ERR, "%s====>%s/%d  find value error!\n", __FILE__, __FUNCTION__, __LINE__);
		res = create_fail("find json value fail", cmdType, msgId);
		format_ctl.flags = -1;
		goto formatDiscfail;
	}
	for(i=0; i<gmnt_num; i++) {
		if(!strcmp(mnt, gmnt_root_dir[i])) {
			break;
		}
	}
	if(i>=gmnt_num) {
		print(DIR_ERR, "%s====>%s/%d mnt dir err!\n", __FILE__, __FUNCTION__, __LINE__);
		res = create_fail("mnt dir err", cmdType, msgId);
		format_ctl.flags = -1;
		goto formatDiscfail;
	}
	dev = gmnt_dev[i];
	fileType = judgeFSType(dev, mnt, NULL, NULL);
	if(pfileType) {
		if(!strcmp(pfileType, NTFS)) {
			fileType = 1;
		}else if(!strcmp(pfileType, FAT32)){
			fileType = 0;
		}
	}
	if(fileType < 0) {
		fileType = 1;
	}
	sprintf(buf, "/bin/umount %s", mnt);
	system(buf);
	if(!delFileDir(mnt)) {
		print(DIR_ERR, "%s====>%s/%d del logic file err!\n", __FILE__, __FUNCTION__, __LINE__);
		res = create_fail("del logic file err", cmdType, msgId);
		format_ctl.flags = -1;
		goto formatDiscfail;
	}
	if(fileType == 0) {   //fat32 format
		if(name) {
			cd = iconv_open(GBK, UTF8);
			if(cd < 0) {
				print(DIR_ERR, "%s====>%s/%d iconv_open err!\n", __FILE__, __FUNCTION__, __LINE__);
				res = create_fail("iconv_open err", cmdType, msgId);
			    goto formatDiscfail;
			}
			namelen = strlen(name);
			if((iconv(cd, &name, &namelen, &pdisname, &gbklen)< 0) || (namelen!=0)) {
				print(DIR_ERR, "%s====>%s/%d iconv err!\n", __FILE__, __FUNCTION__, __LINE__);
				res = create_fail("iconv err", cmdType, msgId);
				iconv_close(cd);
			    goto formatDiscfail;
			}
			iconv_close(cd);
		    sprintf(buf, "/userfs/bin/mkfs.vfat -n '%s' '%s'", gbk_disname, dev);
		}else {
			sprintf(buf, "/userfs/bin/mkfs.vfat '%s'", dev);
		}
		format_ctl.type = 0;
		strcpy(format_ctl.buf, buf);
		strcpy(format_ctl.dev, dev);
		strcpy(format_ctl.mnt, mnt);
		res = format_u(&format_ctl);
		if(res < 0) {
			print(DIR_ERR, "%s====>%s/%d format_u err!\n", __FILE__, __FUNCTION__, __LINE__);
			res = create_fail("format_u err", cmdType, msgId);
			goto formatDiscfail;
		}
	#if 0
		res = system(buf);
		if(res) {
			print(DIR_ERR, "%s====>%s/%d format U err!\n", __FILE__, __FUNCTION__, __LINE__);
			res = create_fail("format U err", cmdType, msgId);
			goto formatDiscfail;
		}
		sprintf(buf, "/bin/mount -o iocharset=utf8 -t vfat %s %s", dev, mnt);
		res = system(buf);
		if(res) {
			print(DIR_ERR, "%s====>%s/%d mount U err!\n", __FILE__, __FUNCTION__, __LINE__);
			res = create_fail("mount U err", cmdType, msgId);
			goto formatDiscfail;
		}
	#endif
		
	} else {              //ntfs format
		if(name) {
			sprintf(buf, "/userfs/bin/mkntfs -f -L '%s' '%s'", name, dev);
		}else {
			sprintf(buf, "/userfs/bin/mkntfs -f '%s'", dev);
		}
		format_ctl.type = 1;
		strcpy(format_ctl.buf, buf);
		strcpy(format_ctl.dev, dev);
		strcpy(format_ctl.mnt, mnt);
		res = format_u(&format_ctl);
		if(res < 0) {
			print(DIR_ERR, "%s====>%s/%d format_u err!\n", __FILE__, __FUNCTION__, __LINE__);
			res = create_fail("format_u err", cmdType, msgId);
			goto formatDiscfail;
		}
		#if 0
		res = system(buf);
		if(res) {
			print(DIR_ERR, "%s====>%s/%d format U err!\n", __FILE__, __FUNCTION__, __LINE__);
			res = create_fail("format U err", cmdType, msgId);
			goto formatDiscfail;
		}
		sprintf(buf, "/userfs/bin/ntfs-3g %s %s", dev, mnt);
		res = system(buf);
		if(res) {
			print(DIR_ERR, "%s====>%s/%d mount U err!\n", __FILE__, __FUNCTION__, __LINE__);
			res = create_fail("mount U err", cmdType, msgId);
			goto formatDiscfail;
		}
		#endif
	}
	if(format_ctl.flags == 2) {
		res = malloc(150);
		if(!res) {
			print(DIR_ERR, "%s====>%s/%d alloc mem err!\n", __FILE__, __FUNCTION__, __LINE__);
			res = create_fail("alloc mem err", cmdType, msgId);
			goto formatDiscfail;
		}
		index += sprintf(res+index, "{\"CmdType\":\"%s\",", cmdType);
		index += sprintf(res+index, "\"Result\":\"0\",");
		index += sprintf(res+index, "\"SequenceId\":\"%s\"}", msgId);
	}else if(format_ctl.flags == 1) {
		print(DIR_ERR, "%s====>%s/%d wait format!\n", __FILE__, __FUNCTION__, __LINE__);
		res = create_fail("wait format", cmdType, msgId);
		goto formatDiscfail;
	}else {
		print(DIR_ERR, "%s====>%s/%d format err!\n", __FILE__, __FUNCTION__, __LINE__);
		res = create_fail("format err", cmdType, msgId);
		goto formatDiscfail;
	}
	
formatDiscfail:
	json_free_value(&root);
formatDiscexit:
	pthread_mutex_unlock(&gChg_lock);
	return res;

}

char *updateFile(char *cmd) {
	json_t *root = NULL;
	char *res = NULL, *cmdType = NULL, *msgId = NULL, *name = NULL, *tmpchr = NULL;
	int index = 0;
	char tmpname[200];
	bool flag = false;

	pthread_mutex_lock(&gChg_lock);
	if(JSON_OK != json_parse_document(&root, cmd)) {
		print(DIR_ERR, "%s====>%s/%d json parse err!\n", __FILE__, __FUNCTION__, __LINE__);
		goto updateFileexit;
	}
	cmdType 	= json_find_value(root, "CmdType");
	msgId		= json_find_value(root, "SequenceId");
	name		= json_find_value(root, "Name");
	if(!cmdType  || !msgId || !name) {
		print(DIR_ERR, "%s====>%s/%d  find value error!\n", __FILE__, __FUNCTION__, __LINE__);
		res = create_fail("find json value fail", cmdType, msgId);
		goto updateFilefail;
	}
	strcpy(tmpname, name);
	flag = fileaccess(tmpname);
	if(!flag) {
		print(DIR_ERR, "%s====>%s/%d  file can't access!\n", __FILE__, __FUNCTION__, __LINE__);
		res = create_fail("file can't access", cmdType, msgId);
		goto updateFilefail;
	}
	tmpchr = strrchr(tmpname, '/');
	if(!tmpchr || !(++tmpchr)) {
		print(DIR_ERR, "%s====>%s/%d  update file name error!\n", __FILE__, __FUNCTION__, __LINE__);
		res = create_fail("update file name error", cmdType, msgId);
		goto updateFilefail;
	}
	tmpchr[-1] = '\0';
	addFileLogic(tmpname, tmpchr);
	res = malloc(150);
	if(!res) {
		print(DIR_ERR, "%s====>%s/%d  alloc mem err!\n", __FILE__, __FUNCTION__, __LINE__);
		res = create_fail("alloc mem err", cmdType, msgId);
		goto updateFilefail;
	}
	index += sprintf(res+index, "{\"CmdType\":\"%s\",", cmdType);
	index += sprintf(res+index, "\"SequenceId\":\"%s\",", msgId);
	index += sprintf(res+index, "\"Result\":\"0\"}");
	
updateFilefail:
	json_free_value(&root);
updateFileexit:
	pthread_mutex_unlock(&gChg_lock);
	return res;

}

char * reqformat(char *cmd) {
	json_t *root = NULL;
	char *res = NULL, *cmdType = NULL, *msgId = NULL;
	int index = 0;
	char tmpname[200];

	pthread_mutex_lock(&gChg_lock);
	if(JSON_OK != json_parse_document(&root, cmd)) {
		print(DIR_ERR, "%s====>%s/%d json parse err!\n", __FILE__, __FUNCTION__, __LINE__);
		goto reqformatexit;
	}
	cmdType 	= json_find_value(root, "CmdType");
	msgId		= json_find_value(root, "SequenceId");

	if(!cmdType  || !msgId) {
		print(DIR_ERR, "%s====>%s/%d  find value error!\n", __FILE__, __FUNCTION__, __LINE__);
		res = create_fail("find json value fail", cmdType, msgId);
		goto reqformatfail;
	}
	res = malloc(200);
	if(!res) {
		print(DIR_ERR, "%s====>%s/%d  mem alloc error!\n", __FILE__, __FUNCTION__, __LINE__);
		res = create_fail("mem alloc fail", cmdType, msgId);
		goto reqformatfail;
	}
	index += sprintf(res+index, "{\"CmdType\":\"%s\",", cmdType);
	index += sprintf(res+index, "\"SequenceId\":\"%s\",", msgId);
	index += sprintf(res+index, "\"Result\":\"%d\"}", format_ctl.flags);
reqformatfail:
	json_free_value(&root);
reqformatexit:	
	pthread_mutex_unlock(&gChg_lock);
	return res;
}

cmd_t cmd[] = {
	{CREATE_DIR_LIST, createMountList},
	{GET_DISC_INF_LOCAL, getMountList},
	{GET_FILE_NUM, getFileNum},
	{LIST, getDirList},
	{RENAME, renameFile},
	{MOVE, moveFile},
	{REMOVE, delFile},
	{COPY, copyFile},
	{CREATE_FOLDER, createFile},
	{DEL_DIR_LIST, delMountList},
	{RENAME_DISC, renameDisc},
	{FORMAT_DISC, formatDisc},
	{UPLOAD_FILE, updateFile},
	{REQ_FORMAT,  reqformat},
	{NULL, NULL},
};



