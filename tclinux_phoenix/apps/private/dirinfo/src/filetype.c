#include <stdio.h>
#include "dirfile.h"

static char *pic[] = {".bmp", ".dib", ".jpg", ".jpeg", ".jpe", ".jfif",
 					  ".gif", ".tif", ".tiff", ".png", NULL};


static char *ved[] = {".avi", ".rmvb", ".rm", ".asf", ".divx", ".mpg",
					".mpeg", ".mpe", ".wmv", ".mp4", ".mkv", ".vob", NULL};

static char *aud[] = {".mp3", ".ogg", ".wav", ".ape", ".cad", ".au", 
	                 ".midi", ".mac", ".aac", NULL};
bool isPic(char *name) {
	int i = 0;
	print(DIR_INF, "%s====>name %s\n", __FUNCTION__, name);
	while(pic[i]) {
		if((strlen(name)>strlen(pic[i])) && !strcmp(name+strlen(name)-strlen(pic[i]), pic[i])) {
			return true;
		}
		i++;
	}
	return false;
}

bool isVed(char *name) {
	int i = 0;
	print(DIR_INF, "%s====>name %s\n", __FUNCTION__, name);
	while(ved[i]) {
		if((strlen(name)>strlen(ved[i])) && !strcmp(name+strlen(name)-strlen(ved[i]), ved[i])) {
			return true;
		}
		i++;
	}
	return false;
}

bool isAud(char *name) {
	int i= 0;
	print(DIR_INF, "%s====>name %s\n", __FUNCTION__, name);
	while(aud[i]) {
		if((strlen(name)>strlen(aud[i])) && !strcmp(name+strlen(name)-strlen(aud[i]), aud[i])) {
			return true;
		}
		i++;
	}
	return false;
}

int fileType(char *name) {
	print(DIR_INF, "%s====>name %s\n", __FUNCTION__, name);
	if(isPic(name)) {
		return PIC;
	}else if(isVed(name)) {
		return VED;
	}else if(isAud(name)) {
		return AUD;
	}else {
	    return OTR;
	}	
}


