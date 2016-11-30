#include "dirfile.h"
#include "list.h"
#include <sys/stat.h>
#include <dirent.h>
#include <stdbool.h>
#include <unistd.h>
#include <pthread.h>
#include "threadpool.h"
#include <assert.h>

static struct list_head gPIC = LIST_HEAD_INIT(gPIC);
static struct list_head gAUD = LIST_HEAD_INIT(gAUD);
static struct list_head gVED = LIST_HEAD_INIT(gVED);
static pthread_mutex_t gPic_lock = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t gAud_lock = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t gVed_lock = PTHREAD_MUTEX_INITIALIZER;

pdirlist_t getDirbyName(list_head_t *head, char *name) {
	pdirlist_t pdirlist = NULL;

	list_for_each_entry(pdirlist, head, list) {
		if(!strcmp(pdirlist->name, name)) {
			return pdirlist;
		}
	}
	return NULL;
}

pdirlist_t createNewNode(char *filename) {
	pdirlist_t ptmdirlist = NULL;

	print(DIR_INF,"%s=====> name %s\n", __FUNCTION__, filename);
	ptmdirlist = malloc(sizeof(dirlist_t));
	if(!ptmdirlist) {
		print(DIR_ERR,"%s=====> malloc fail %d\n", __FUNCTION__, __LINE__);
		return NULL;
	}
	ptmdirlist->name = malloc(strlen(filename)+1);
	if(!ptmdirlist->name) {
		print(DIR_ERR,"%s=====> malloc fail %d\n", __FUNCTION__, __LINE__);
		free(ptmdirlist);
		return NULL;
	}
	strcpy(ptmdirlist->name, filename);
	INIT_LIST_HEAD(&ptmdirlist->list);
	INIT_LIST_HEAD(&ptmdirlist->child);
    return ptmdirlist;
}


pdirlist_t addDirFile(list_head_t *head, char *dirname, char *filename) {
	pdirlist_t pdirlist = NULL, pdirnext = NULL;

    print(DIR_ERR, "%s====> head=%p, dirname=%s, filename=%s\n", __FUNCTION__, head, dirname, filename);
	list_for_each_entry_safe(pdirlist, pdirnext, head, list) {
		print(DIR_INF, "%s====>addr = %p\n", __FUNCTION__, pdirlist);
		if(!strcmp(pdirlist->name, dirname)) {
			pdirlist_t ptmdirlist = NULL;
			list_for_each_entry(ptmdirlist, &pdirlist->child, child) {
				if(!strcmp(ptmdirlist->name, filename)) {
					return ptmdirlist;
				}
			}
			ptmdirlist = createNewNode(filename);
			if(ptmdirlist) {
			    list_add(&ptmdirlist->child, &pdirlist->child);
				return ptmdirlist;
			}
			return NULL;
		}
	}
	pdirlist = createNewNode(dirname);
	print(DIR_INF, "%s====>addr1 = %p\n", __FUNCTION__, pdirlist);
	if(pdirlist) {
		pdirlist_t ptmdirlist = NULL;
		ptmdirlist = createNewNode(filename);
		if(ptmdirlist) {
			list_add(&pdirlist->list, head);
			list_add(&ptmdirlist->child, &pdirlist->child);
			return ptmdirlist;
		}
		free(pdirlist->name);
		free(pdirlist);
	}
	return NULL;
}

bool delDirbyName(list_head_t *head, char *dirname) {
	pdirlist_t pdirlist = NULL, pdirlistnext = NULL;

	list_for_each_entry_safe(pdirlist, pdirlistnext, head, list) {
		if(!strncmp(pdirlist->name, dirname, strlen(dirname))) {
			pdirlist_t pchildlist = NULL, pchildnext = NULL;
			list_for_each_entry_safe(pchildlist, pchildnext, &pdirlist->child, child) {
				list_del(&pchildlist->child);
				free(pchildlist->name);
				free(pchildlist);
			}
			list_del(&pdirlist->list);
			free(pdirlist->name);
			free(pdirlist);
		} 
	}
	return true;
}

bool delDir(char *dirname) {
	bool res = false;
	pthread_mutex_lock(&gPic_lock);
	res = delDirbyName(&gPIC, dirname);
	pthread_mutex_unlock(&gPic_lock);
	if(res == false) {
		print(DIR_ERR, "%s====>delDirbyName fail! %d\n", __FUNCTION__, __LINE__);
		goto deldirexit;
	}
	pthread_mutex_lock(&gVed_lock);
	res = delDirbyName(&gVED, dirname);
	pthread_mutex_unlock(&gVed_lock);
	if(res == false) {
		print(DIR_ERR, "%s====>delDirbyName fail! %d\n", __FUNCTION__, __LINE__);
		goto deldirexit;
	}
	pthread_mutex_lock(&gAud_lock);
	res = delDirbyName(&gAUD, dirname);
	pthread_mutex_unlock(&gAud_lock);
	if(res == false) {
		print(DIR_ERR, "%s====>delDirbyName fail! %d\n", __FUNCTION__, __LINE__);
		goto deldirexit;
	}
deldirexit:
	return res;
}
bool delDirFile(list_head_t *head, char *dirname, char *filename) {
	pdirlist_t pdirlist = NULL, pdirlistnext = NULL;
	int flag = 0;

	list_for_each_entry_safe(pdirlist, pdirlistnext, head, list) {
		if(!strcmp(pdirlist->name, dirname)) {
			pdirlist_t pchildlist = NULL, pchildnext = NULL;
			list_for_each_entry_safe(pchildlist, pchildnext, &pdirlist->child, child) {
				if(!strcmp(pchildlist->name, filename)) {
					list_del(&pchildlist->child);
					free(pchildlist->name);
					free(pchildlist);
					flag = 1;
					break;
				}
			}
			if(list_empty(&pdirlist->child)) {
				list_del(&pdirlist->list);
				free(pdirlist->name);
				free(pdirlist);
			}
			if(flag) {
				return true;
			}	
		}
	}
	return true;
}
bool __rename(list_head_t *head, char *des, char *src) {
	bool flag = false;
	struct stat fstat;
	pdirlist_t pdirlist = NULL, pdirnext = NULL;
	char buf[200] = {0};

	if(stat(src, &fstat) < 0) {
		print(DIR_ERR, "%s====>%s/%d stat err!\n", __FILE__, __FUNCTION__, __LINE__);
		return false;
	}
	if(S_ISDIR(fstat.st_mode)) {
		list_for_each_entry_safe(pdirlist, pdirnext, head, list) {
			if(!strncmp(pdirlist->name, src, strlen(src))) {
				sprintf(buf, "%s%s", des, pdirlist->name+strlen(src));
				print(DIR_INF, "%s====>%s/%d buf= %s\n", __FILE__, __FUNCTION__, __LINE__, buf);
				if(pdirlist->name) free(pdirlist->name);
				pdirlist->name = malloc(strlen(buf)+1);
				if(!pdirlist->name) {
					print(DIR_ERR, "%s====>%s/%d alloc mem fail!\n", __FILE__, __FUNCTION__, __LINE__);
					return false;
				}
				strcpy(pdirlist->name, buf);
			}
		}
	}else {
	    char *tmp = strrchr(src, '/');
		if(!tmp || !(++tmp)) {
			print(DIR_ERR, "%s====>%s/%d src name err!\n", __FILE__, __FUNCTION__, __LINE__);
			return false;
		}
		tmp[-1] = '\0';
		flag = delDirFile(head, src, tmp);
		tmp = strrchr(des, '/');
		if(!tmp || !(++tmp)) {
			print(DIR_ERR, "%s====>%s/%d src name err!\n", __FILE__, __FUNCTION__, __LINE__);
			return false;
		}
		tmp[-1] = '\0';
		if(!(flag && addDirFile(head, des, tmp))) {
			print(DIR_ERR, "%s====>%s/%d del or add file err!\n", __FILE__, __FUNCTION__, __LINE__);
			return false;
		}
	}
	return true;
}
bool delFiles(char *dirname, char *name) {
	int filetype = fileType(name);
	bool res = true;
	switch(filetype) {
		case PIC:
			pthread_mutex_lock(&gPic_lock);
			res = delDirFile(&gPIC, dirname, name);
			pthread_mutex_unlock(&gPic_lock);
			break;
		case VED:
			pthread_mutex_lock(&gVed_lock);
			res = delDirFile(&gVED, dirname, name);
			pthread_mutex_unlock(&gVed_lock);
			break;
		case AUD:
			pthread_mutex_lock(&gAud_lock);
			res = delDirFile(&gAUD, dirname, name);
			pthread_mutex_unlock(&gAud_lock);
			break;
		default:
			;
			
	}
	return res;
}

bool delFileDir(char *dir) {
	struct stat fsstat;
	bool flag = true;
	char *tmp = NULL;
    char dirname[250] = {0};

	strcpy(dirname, dir);
	if(stat(dirname, &fsstat)) {
		print(DIR_ERR, "%s===>%s/%d stat fail!\n", __FILE__, __FUNCTION__, __LINE__);
		flag = false;
		goto delFileDirexit;
	}
	if(S_ISDIR(fsstat.st_mode)) {
		flag = delDir(dirname);
	}
	else {
     	tmp = strrchr(dirname, '/');
		if(!tmp || !(++tmp)) {
			print(DIR_ERR, "%s====>%s/%d file addr err!\n", __FILE__, __FUNCTION__, __LINE__);
			flag = false;
			goto delFileDirexit;
		}
		*(tmp-1) = '\0';
		flag = delFiles(dirname, tmp);
	}

delFileDirexit:
	return flag;
}

bool  removelist(char *name) {
	pdirlist_t pdirlist = NULL, pdirnext = NULL;

	if(name == NULL)
	{
		print(DIR_ERR, "%s====>name  null\n", __FUNCTION__);
		return false;
	}
	print(DIR_WRN, "%s====>clear gPic_lock\n", __FUNCTION__);
	pthread_mutex_lock(&gPic_lock);
	delDirbyName(&gPIC, name);	
	pthread_mutex_unlock(&gPic_lock);
	print(DIR_WRN, "%s====>clear gVed_lock\n", __FUNCTION__);
	pthread_mutex_lock(&gVed_lock);
	delDirbyName(&gVED, name);
	pthread_mutex_unlock(&gVed_lock);
	print(DIR_WRN, "%s====>clear gAud_lock\n", __FUNCTION__);
	pthread_mutex_lock(&gAud_lock);
	delDirbyName(&gAUD, name);
	pthread_mutex_unlock(&gAud_lock);
	return true;
}
bool movecopy(list_head_t *head, char *des, char *src, int flag) {
	char *tmpname = strrchr(src, '/');
	struct stat statfile;
	char ldirname[100] = {0};
	pdirlist_t pdirlist = NULL, pdirnext = NULL;

	if(!tmpname) {
		return false;
	}
	print(DIR_WRN, "%s====>tmpname %s src = %s\n", __FUNCTION__, tmpname, src);
	if(stat(src, &statfile)) {
		print(DIR_ERR, "%s====> stat err %d\n", __FUNCTION__, __LINE__);
		return false;
	}
	if(S_ISDIR(statfile.st_mode)) {
		sprintf(ldirname, "%s%s", des, tmpname);
		list_for_each_entry_safe(pdirlist, pdirnext, head, list) {
			if(!strncmp(pdirlist->name, ldirname, strlen(ldirname))) {
				delDirbyName(head, pdirlist->name);
			}
			if(!strncmp(pdirlist->name, src, strlen(src))) {
				char tmpdirname[100] = {0};
				sprintf(tmpdirname, "%s%s", ldirname, pdirlist->name+strlen(src));
				if(flag == 0) {
					free(pdirlist->name);
					pdirlist->name = malloc(strlen(tmpdirname)+1);
					if(!pdirlist->name) {
						print(DIR_ERR, "%s====> malloc fail %d\n", __FUNCTION__, __LINE__);
						return false;
					}
					strcpy(pdirlist->name, tmpdirname);
				}else {
				    pdirlist_t pchildlist = NULL, pchildnext = NULL;
					list_for_each_entry_safe(pchildlist, pchildnext, &pdirlist->child, child) {
						addDirFile(head, tmpdirname, pchildlist->name);
					}
				}
			}
		}	
		
	}
	else {
		char filedirname[100] = {0};
		strncpy(filedirname, src, tmpname-src);
		list_for_each_entry_safe(pdirlist, pdirnext, head, list) {
			if(!strcmp(pdirlist->name, filedirname)) {
				pdirlist_t pchildlist = NULL, pchildnext = NULL;
				list_for_each_entry_safe(pchildlist, pchildnext, &pdirlist->child, child) {
					if(!strcmp(pchildlist->name, tmpname+1)) {
						if(flag == 0) {
							list_del(&pchildlist->child);
							free(pchildlist->name);
							free(pchildlist);
							if(list_empty(&pdirlist->child)) {
								list_del(&pdirlist->list);
								free(pdirlist->name);
								free(pdirlist);
							}
						}
						addDirFile(head, des, tmpname+1);
					}
				}
			}
		}
	}
	return true;

}

//cover the orign dir, file
bool moveDir(char *des, char *src) {
   bool res = false;
   pthread_mutex_lock(&gPic_lock);
   res = movecopy(&gPIC, des, src, 0);
   pthread_mutex_unlock(&gPic_lock);
   if(res == false) {
		print(DIR_ERR, "%s====>movecopy fail! %d\n", __FUNCTION__, __LINE__);
		goto moveexit;
   }
   pthread_mutex_lock(&gVed_lock);
   res = movecopy(&gVED, des, src, 0);
   pthread_mutex_unlock(&gVed_lock);
   if(res == false) {
		print(DIR_ERR, "%s====>movecopy fail! %d\n", __FUNCTION__, __LINE__);
		goto moveexit;
   }
   pthread_mutex_lock(&gAud_lock);
   res = movecopy(&gAUD, des, src, 0);
   pthread_mutex_unlock(&gAud_lock);
   if(res == false) {
		print(DIR_ERR, "%s===>movecopy fail! %d\n", __FUNCTION__, __LINE__);
		goto  moveexit;
   }
moveexit:
	return res;	
	
}

bool copyDir(char *des, char *src) {
	bool res = false;
	pthread_mutex_lock(&gPic_lock);
	res = movecopy(&gPIC, des, src, 1);
	pthread_mutex_unlock(&gPic_lock);
	if(res == false) {
		print(DIR_ERR, "%s====>movecopy fail! %d\n", __FUNCTION__, __LINE__);
		goto copyexit;
	}
	pthread_mutex_lock(&gAud_lock);
	res = movecopy(&gAUD, des, src, 1);
	pthread_mutex_unlock(&gAud_lock);
	if(res == false) {
		print(DIR_ERR, "%s====>movecopy fail! %d\n", __FUNCTION__, __LINE__);
		goto copyexit;
	}
	pthread_mutex_lock(&gVed_lock);
	res = movecopy(&gVED, des, src, 1);
	pthread_mutex_unlock(&gVed_lock);
	if(res == false) {
		print(DIR_ERR, "%s====>movecopy fail! %d\n", __FUNCTION__, __LINE__);
		goto copyexit;
	}
copyexit:
	return res;
}

void createlist(pwork_data_t work) {
    char *root = work->name;
	if(!access(root, F_OK)) {
	    char *name = NULL;
		DIR *dir = opendir(root);
	    struct dirent *file = NULL;

		print(DIR_INF, "%s====>root =%s\n", __FUNCTION__,  root);
#if 0
		if(!name) {
				print(DIR_ERR,"%s====>malloc fail %d\n", __FUNCTION__, __LINE__);
				return;
		}
#endif
		if(!dir) {
				print(DIR_ERR, "%s====>opendir fail %d %s err = %d\n", __FUNCTION__, __LINE__, root, errno);
				return;
		}
		while(file=readdir(dir)) {
			print(DIR_INF, "%s====> readdir %s\n", __FUNCTION__, file->d_name);
			#if 1
			if(!strcmp(file->d_name, ".") || !strcmp(file->d_name, ".."))
				continue;
			if(file->d_type & DT_DIR) {
				name = malloc(strlen(root)+strlen(file->d_name)+2);
				if(!name) {
					print(DIR_ERR,"%s====>malloc fail %d\n", __FUNCTION__, __LINE__);
					closedir(dir);
					return;
		        }
				sprintf(name,"%s/%s", root, file->d_name);
				pwork_data_t twork = malloc(sizeof(work_data_t));
				if(!twork) {
					print(DIR_ERR, "%s===>%s/%d alloc mem fail!\n", __FILE__, __FUNCTION__, __LINE__);
					free(name);
					closedir(dir);
					return;
				}
				strcpy(twork->name, name);
				threadpool_add(&gPool, createlist, twork);
				free(name);
			} else {
				int tmptype =fileType(file->d_name);
				if(PIC == tmptype) {
					pthread_mutex_lock(&gPic_lock);
					addDirFile(&gPIC, root, file->d_name);
					pthread_mutex_unlock(&gPic_lock);
				}
				else if(VED == tmptype) {
					pthread_mutex_lock(&gVed_lock);
					addDirFile(&gVED, root, file->d_name);
					pthread_mutex_unlock(&gVed_lock);
				}
				else if(AUD == tmptype) {
					pthread_mutex_lock(&gAud_lock);
					addDirFile(&gAUD, root, file->d_name);
					pthread_mutex_unlock(&gAud_lock);
				}

			} 
			#endif
		}
		closedir(dir);
	}
}

#if 0
void createTotalList(char *root, int filetype) {
	if(filetype == PIC) {
	     createlist(&gPIC, root, PIC);
	}else if(filetype == VED) {
		createlist(&gVED, root, VED);
	}else if(filetype == AUD) {
		createlist(&gAUD, root, AUD);
	}else {
		print(DIR_ERR, "%s====> filetype error %d\n", __FUNCTION__, __LINE__);
	}
}
#endif
void printList(list_head_t *head) {
	pdirlist_t pdirlist = NULL, pdirnext = NULL;

	print(DIR_INF, "%s====>head =%p head-next =%p, head->next->next=%p\n", __FUNCTION__, head, head->next, head->next->next);
	list_for_each_entry_safe(pdirlist, pdirnext, head, list) {
		print(DIR_WRN, "%s====>node %s entry\n", __FUNCTION__, pdirlist->name);
		pdirlist_t pchildlist = NULL, pchildnext = NULL;
		list_for_each_entry_safe(pchildlist, pchildnext, &pdirlist->child, child) {
			print(DIR_WRN, "%s  ", pchildlist->name);
		}
		print(DIR_WRN, "\n%s====>node %s exit\n", __FUNCTION__, pdirlist->name);
	}
}

void printTotalList(int filetype) {
	if(filetype == PIC) {
		printList(&gPIC);
	}else if(filetype == VED) {
		printList(&gVED);
	}
	else if(filetype == AUD) {
		printList(&gAUD);
	}
	else {
		print(DIR_ERR, "%s====> filetype error %d\n", __FUNCTION__, __LINE__);
	}
}

static int _getDirNum(list_head_t *head) {
	int num = 0;
	pdirlist_t pdirlist = NULL, pdirnext = NULL;
	pdirlist_t pchildlist = NULL, pchildnext = NULL;

	list_for_each_entry_safe(pdirlist, pdirnext, head, list) {
		list_for_each_entry_safe(pchildlist, pchildnext, &pdirlist->child, child) {
			num++;
		}
	}
	return num;
}
int getDirNum(int filetype) {
int num = 0;
	switch(filetype) {
		case PIC:
			num = _getDirNum(&gPIC);
			break;
		case AUD:
			num = _getDirNum(&gAUD);
			break;
		case VED:
			num = _getDirNum(&gVED);
			break;
		default:
			num = -1;
			break;
	}

	return num;
}

void addFileInfo(char *dir, char *file, int *num, char **rest, int *restlen) {
	char *res = *rest;
    int reslen = *restlen;
    char *tmpres = NULL;
	char filebuf[250] = {0};
	
	print(DIR_INF, "%s====>%s/%d list=%p, len = %d\n", __FILE__, __FUNCTION__, __LINE__, *rest, *restlen);
	assert(*restlen >= 0);
	assert(dir != NULL);
	assert(file != NULL);
	assert(*num >= 0);
	if(*num+2*strlen(file)+strlen(dir)+200 > reslen) {
		tmpres = realloc(res, reslen+1000);
		if(tmpres == NULL) {
			free(res);
			*rest = NULL;
			print(DIR_ERR, "%s====> realloc fail! %d\n", __FUNCTION__, __LINE__);
			return;
		}
		*rest = tmpres;
		res = tmpres;
		*restlen += 1000;
	}
	print(DIR_WRN, "%s====>%s/%d 1 list=%p, len = %d file=%s dir=%s\n", __FILE__, __FUNCTION__, __LINE__, *rest, *restlen, file, dir);
	*num += sprintf(res+*num, "{\"FileName\":\"%s\",", file);
	*num += sprintf(res+*num, "\"PhysicalFolderName\":\"%s/%s\",", dir, file);
	print(DIR_WRN, "%s====>%s/%d 1 list=%p, len = %d\n", __FILE__, __FUNCTION__, __LINE__, *rest, *restlen);
	sprintf(filebuf, "%s/%s", dir, file);
	struct stat fstat;
	if(stat(filebuf, &fstat)) {
		print(DIR_ERR, "%s=====>stat fail %d\n", __FUNCTION__, __LINE__);
		free(res);
		*rest = NULL;
		return;
	}
	print(DIR_WRN, "%s====>%s/%d 2 list=%p, len = %d\n", __FILE__, __FUNCTION__, __LINE__, *rest, *restlen);
	*num += sprintf(res+*num, "\"IsFolder\":\"%d\",", S_ISDIR(fstat.st_mode)?1:0);
	*num += sprintf(res+*num, "\"FileSize\":\"%d\",", fstat.st_size);
	struct tm *local_time = NULL;
	local_time = localtime(&fstat.st_mtime);
	if(!local_time) {
		print(DIR_ERR, "%s====>localtime error! %d\n", __FUNCTION__, __LINE__);
		free(res);
		*rest = NULL;
		return;
	}
	print(DIR_WRN, "%s====>%s/%d 3 list=%p, len = %d\n", __FILE__, __FUNCTION__, __LINE__, *rest, *restlen);
	*num += sprintf(res+*num, "\"ModifiedTime\":\"%04d/%02d/%02d %02d:%02d\"},", local_time->tm_year+1900,
		          local_time->tm_mon+1, local_time->tm_mday, local_time->tm_hour, local_time->tm_min);
	print(DIR_WRN, "%s====>%s/%d 3 list=%p, len = %d\n", __FILE__, __FUNCTION__, __LINE__, *rest, *num);
	
}

char *_getList(list_head_t *head, int start, int end) {
	int index = 1, num = 0;
	pdirlist_t pdirlist = NULL, pdirnext = NULL;
	pdirlist_t pchildlist = NULL, pchildnext = NULL;
	char *res = NULL;
	int reslen = 0;
	char filebuf[250] = {0};
 
	res= malloc(1000);
	if(!res) {
		print(DIR_ERR, "%s====> malloc fail %d\n", __FUNCTION__, __LINE__);
		goto _getListexit;
	}
	reslen = 1000;
	list_for_each_entry_safe(pdirlist, pdirnext, head, list) {
		list_for_each_entry_safe(pchildlist, pchildnext, &pdirlist->child, child) {
			if(index++ < start) {
				continue;
			}
			addFileInfo(pdirlist->name, pchildlist->name, &num, &res, &reslen);
			if(!res) {
				print(DIR_ERR, "%s===>%s/%d addFileInfo error!\n", __FILE__, __FUNCTION__, __LINE__);
				goto _getListexit;
			}
			if((end>0) && (index>end)) {
				break;
			}
		}
		if((end>0) && (index>end)) {
			break;
		}
	}
	print(DIR_WRN, "%s===>%s/%d num = %d %p\n", __FILE__, __FUNCTION__, __LINE__, num, res);
	if(num>0) {
         res[num-1] = '\0';
	}else {
		 res[0] = '\0';
	}
	print(DIR_WRN, "%s===>%s/%d msg = %s\n", __FILE__, __FUNCTION__, __LINE__, res);

_getListexit:
	return res;

}

char *getList(int filetype, int start, int end) {
	char *res = NULL;
	
	switch(filetype) {
		case PIC:
			res = _getList(&gPIC, start, end);
			break;
		case AUD:
			res = _getList(&gAUD, start, end);
			break;
		case VED:
			res = _getList(&gVED, start, end);
			break;
		default:
			res = NULL;
	}

	return res;
}

void addFileLogic(char *dir, char *name) {
	int filetype = fileType(name);

	if(filetype == PIC) {
		pthread_mutex_lock(&gPic_lock);
		addDirFile(&gPIC, dir, name);
		pthread_mutex_unlock(&gPic_lock);
	}else if(filetype == VED) {
		pthread_mutex_lock(&gVed_lock);
		addDirFile(&gVED, dir, name);
		pthread_mutex_unlock(&gVed_lock);
	}else if(filetype == AUD) {
		pthread_mutex_lock(&gAud_lock);
		addDirFile(&gAUD, dir, name);
		pthread_mutex_unlock(&gAud_lock);
	}else {
		;
	}
}

bool renamelogic(char *des, char *src) {
	char dess[200] = {0}, srcc[200] = {0};
	bool res = false;

	strcpy(dess, des);
	strcpy(srcc, src);
	pthread_mutex_lock(&gPic_lock);
    res = __rename(&gPIC, dess, srcc);
    pthread_mutex_unlock(&gPic_lock);
	if(!res) {
		print(DIR_ERR, "%s====>%s/%d rename fail!\n", __FILE__, __FUNCTION__, __LINE__);
		return false;
	}
	strcpy(dess, des);
	strcpy(srcc, src);
	pthread_mutex_lock(&gAud_lock);
    res = __rename(&gAUD, dess, srcc);
    pthread_mutex_unlock(&gAud_lock);
	if(!res) {
		print(DIR_ERR, "%s====>%s/%d rename fail!\n", __FILE__, __FUNCTION__, __LINE__);
		return false;
	}
	strcpy(dess, des);
	strcpy(srcc, src);
	pthread_mutex_lock(&gVed_lock);
    res = __rename(&gVED, dess, srcc);
    pthread_mutex_unlock(&gVed_lock);
	if(!res) {
		print(DIR_ERR, "%s====>%s/%d rename fail!\n", __FILE__, __FUNCTION__, __LINE__);
		return false;
	}
	return true;
}


