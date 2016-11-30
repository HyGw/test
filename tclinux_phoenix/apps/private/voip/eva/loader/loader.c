/*********************************************************************
* LEGAL DISCLAIMER
*
* (Header of MediaTek Software/Firmware Release or Documentation)
*
* BY OPENING OR USING THIS FILE, BUYER HEREBY UNEQUIVOCALLY 
* ACKNOWLEDGES AND AGREES THAT THE SOFTWARE/FIRMWARE AND ITS 
* DOCUMENTATIONS ("MEDIATEK SOFTWARE") RECEIVED FROM MEDIATEK AND/OR 
* ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON AN "AS-IS" BASIS ONLY. 
* MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES, EXPRESS OR 
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF 
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR 
* NONINFRINGEMENT. NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY 
* WHATSOEVER WITH RESPECT TO THE SOFTWARE OF ANY THIRD PARTY WHICH 
* MAY BE USED BY, INCORPORATED IN, OR SUPPLIED WITH THE MEDIATEK 
* SOFTWARE, AND BUYER AGREES TO LOOK ONLY TO SUCH THIRD PARTY FOR ANY
* WARRANTY CLAIM RELATING THERETO. MEDIATEK SHALL ALSO NOT BE 
* RESPONSIBLE FOR ANY MEDIATEK SOFTWARE RELEASES MADE TO BUYER'S 
* SPECIFICATION OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
*
* BUYER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND 
* CUMULATIVE LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED 
* HEREUNDER WILL BE, AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE 
* MEDIATEK SOFTWARE AT ISSUE, OR REFUND ANY SOFTWARE LICENSE FEES OR 
* SERVICE CHARGE PAID BY BUYER TO MEDIATEK FOR SUCH MEDIATEK SOFTWARE
* AT ISSUE.
*
* THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN 
* ACCORDANCE WITH THE LAWS OF THE STATE OF CALIFORNIA, USA, EXCLUDING
* ITS CONFLICT OF LAWS PRINCIPLES.
**********************************************************************
*/

/*
 *	File:	loader.c
 *	 
 *	Author: Peter Jiang
 *	 		
 *	Last Revision Date: 2015.5.9
 *	 
 *	Description:
 *	load voip drivers according to real slic type (zarlink, silicon..)
 *
 */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <linux/ioctl.h>


#include "loader.h"



#define SPI_CTRL_IOCTL_MAGIC		0x91

int fd = -1;

typedef struct _spi_cmd_struct {
	unsigned char slic_type_id; // 0 zarlink,   1 silicon lab
	unsigned char interface_type_id; // 0 spi,  1 zsi, 2 isi  
	unsigned char slic_id; // 0 the first slic, 1the second slic
	unsigned char ctrl;
	unsigned char cmd;
	unsigned char cmdLen;
	unsigned char data[32]; // get slic type from ioctl

}spi_cmd_struct;



enum {
	SPI_CTRL_IOCTL_CHECK		= _IOW(SPI_CTRL_IOCTL_MAGIC,  1, spi_cmd_struct),	
	SPI_CTRL_IOCTL_WRITE		= _IOW(SPI_CTRL_IOCTL_MAGIC,  2, spi_cmd_struct),
	SPI_CTRL_IOCTL_READ		    = _IOW(SPI_CTRL_IOCTL_MAGIC,  3, spi_cmd_struct),
	SPI_CTRL_IOCTL_RESET		= _IOW(SPI_CTRL_IOCTL_MAGIC,  4, spi_cmd_struct),
};


//#undef printf
//#define printf tcdbg_printf

int get_slic_type_data(slic_class_e slic_type_id, interface_class_e interface_type_id, int slic_id, char *slic_type)
{
	char type[SLIC_TYPE_LEN_MAX] = {0};
	int len = SLIC_TYPE_LEN_MAX;
	int ret = -1;
	int i = 0;
	spi_cmd_struct cmd = {0};

	cmd.slic_type_id = slic_type_id;
	cmd.interface_type_id = interface_type_id;
	cmd.slic_id = slic_id;

	if (fd <= 0){
		printf("fd is not init\n");
        return LOADER_FAILED;
    }

	ret = ioctl(fd, SPI_CTRL_IOCTL_CHECK, (int)&cmd);
	strncpy (slic_type, cmd.data, len);
}

int get_slic_type (OUT char *slic_type, OUT char *slic_type_1, OUT int *pslicNum, OUT int *pinterface_type, OUT int *pslic_class)
{
    int ret = 0;
    int load_result = LOADER_FAILED; 
    char cmd[128] = {0};

    snprintf (cmd, sizeof(cmd), "%s %s", INSMOD, M_SYS_MOD);
    ret = system (cmd);
    ABNORMAL_CHECK (ret);

    snprintf (cmd, sizeof(cmd), "%s %s", INSMOD, M_DSPCORE);
    ret |= system (cmd);
    ABNORMAL_CHECK (ret);

    snprintf (cmd, sizeof(cmd), "%s %s", INSMOD, M_PCM1);
    ret |= system (cmd);
    ABNORMAL_CHECK (ret);
	
    snprintf (cmd, sizeof(cmd), "%s %s", INSMOD, M_PCM2);
    ret |= system (cmd);
    ABNORMAL_CHECK (ret);

    snprintf (cmd, sizeof(cmd), "%s %s", INSMOD, M_LEC_LOADER);
    ret |= system (cmd);
    ABNORMAL_CHECK (ret);

    snprintf (cmd, sizeof(cmd), "%s %s", INSMOD, M_SPI);
    ret |= system (cmd);
    ABNORMAL_CHECK (ret);

    fd = (int)open("/dev/spi", O_RDWR);
	
    if (fd <= 0) {
        printf ("open /dev/spi failed\n");
        return -1;
    }

    printf ("START CHECK ZARLIN ZSI\n");
    snprintf (cmd, sizeof(cmd), "%s %s type=ZSI", INSMOD, M_SLIC3);
    ret |= system (cmd);
    ABNORMAL_CHECK (ret);

    get_slic_type_data(SLIC_CLASS_ZARLINK,ZSI_TYPE,0,slic_type);
    ret = check_slic_type(slic_type);
    if(ret == SLIC_CHECK_SUCCESS)
    {
        *pslicNum = 1;
        *pinterface_type = ZSI_TYPE;
        *pslic_class = SLIC_CLASS_ZARLINK;
        get_slic_type_data(SLIC_CLASS_ZARLINK,ZSI_TYPE,1,slic_type_1);
        ret = check_slic_type(slic_type_1);
        if(ret == SLIC_CHECK_SUCCESS)
        {
            *pslicNum = 2;
            load_result = LOADER_SUCCESS;
            goto loader_end;


        }
        else
        {
            snprintf (cmd, sizeof(cmd), "%s %s", RMMOD, M_PCM2);
            ret |= system (cmd);
            ABNORMAL_CHECK (ret);

        }
        load_result = LOADER_SUCCESS;
        goto loader_end;


    }


    printf ("START CHECK SILICON ISI\n");
    snprintf (cmd, sizeof(cmd), "%s %s", RMMOD, M_SLIC3);
    ret |= system (cmd);
    ABNORMAL_CHECK (ret);

    snprintf (cmd, sizeof(cmd), "%s %s type=ISI", INSMOD, M_SLIC3_SILICON);
    ret |= system (cmd);
    ABNORMAL_CHECK (ret);

    get_slic_type_data(SLIC_CLASS_SILICON,ISI_TYPE,0,slic_type);
    ret = check_slic_type(slic_type);
    if(ret == SLIC_CHECK_SUCCESS)
    {
        *pslicNum = 1;
        *pinterface_type = ISI_TYPE;
        *pslic_class = SLIC_CLASS_SILICON;
        get_slic_type_data(SLIC_CLASS_SILICON,ISI_TYPE,1,slic_type_1);
        ret = check_slic_type(slic_type_1);
        if(ret == SLIC_CHECK_SUCCESS)
        {
            *pslicNum = 2;
            load_result = LOADER_SUCCESS;
            goto loader_end;

        }
        else
        {
            snprintf (cmd, sizeof(cmd), "%s %s", RMMOD, M_PCM2);
            ret |= system (cmd);
            ABNORMAL_CHECK (ret);
        }
        load_result = LOADER_SUCCESS;
        goto loader_end;
    }

	
    printf ("START CHECK ZARLIN SPI\n");
    snprintf (cmd, sizeof(cmd), "%s %s", RMMOD, M_SLIC3_SILICON);
    ret |= system (cmd);
    ABNORMAL_CHECK (ret);
	
    snprintf (cmd, sizeof(cmd), "%s %s", INSMOD, M_SLIC3);
    ret |= system (cmd);
    ABNORMAL_CHECK (ret);
	
    get_slic_type_data(SLIC_CLASS_ZARLINK,SPI_TYPE,0,slic_type);
    ret = check_slic_type(slic_type);
    if(ret == SLIC_CHECK_SUCCESS)
    {
        *pslicNum = 1;
        *pinterface_type = SPI_TYPE;
        *pslic_class = SLIC_CLASS_ZARLINK;
        get_slic_type_data(SLIC_CLASS_ZARLINK,SPI_TYPE,1,slic_type_1);
        ret = check_slic_type(slic_type_1);
        if(ret == SLIC_CHECK_SUCCESS)
        {
            *pslicNum = 2;
        }

        snprintf (cmd, sizeof(cmd), "%s %s", RMMOD, M_PCM2);
        ret |= system (cmd);
        ABNORMAL_CHECK (ret);
		
        load_result = LOADER_SUCCESS;
        goto loader_end;
    }

    printf ("START CHECK SILICON SPI\n");
    snprintf (cmd, sizeof(cmd), "%s %s", RMMOD, M_SLIC3);
    ret |= system (cmd);
    ABNORMAL_CHECK (ret);

    snprintf (cmd, sizeof(cmd), "%s %s", INSMOD, M_SLIC3_SILICON);
    ret |= system (cmd);
    ABNORMAL_CHECK (ret);

    get_slic_type_data(SLIC_CLASS_SILICON,SPI_TYPE,0,slic_type);
    ret = check_slic_type(slic_type);
    if(ret == SLIC_CHECK_SUCCESS)
    {
        *pslicNum = 1;
        *pinterface_type = SPI_TYPE;
        *pslic_class = SLIC_CLASS_SILICON;
        get_slic_type_data(SLIC_CLASS_SILICON,SPI_TYPE,1,slic_type_1);
        ret = check_slic_type(slic_type_1);
        if(ret == SLIC_CHECK_SUCCESS)
        {
            *pslicNum = 2;
        }

        snprintf (cmd, sizeof(cmd), "%s %s", RMMOD, M_PCM2);
        ret |= system (cmd);
        ABNORMAL_CHECK (ret);
		
        load_result = LOADER_SUCCESS;
        goto loader_end;
    }


 loader_end:
    if (fd)
        close(fd);
    return load_result;   
}

int load_voip(IN char* slic_type, IN char* slic_type_1, IN int slicNum)
{
    int ret = 0;

    if((slicNum == 1) || (slicNum == 2 && (strstr (slic_type, slic_type_1)))){ 
        if (strstr (slic_type, SLIC_TYPE_ZL88601))
            ret = load_zl88601 (slicNum);
        else if (strstr (slic_type, SLIC_TYPE_LE89156))
            ret = load_le89156 (slicNum);
        else if (strstr (slic_type, SLIC_TYPE_LE89316))
            ret = load_le89316 (slicNum);
        else if (strstr (slic_type, SLIC_TYPE_LE9641))
            ret = load_le9641 (slicNum);
        else if (strstr (slic_type, SLIC_TYPE_SI32176))
            ret = load_si32176 (slicNum);
        else if (strstr (slic_type, SLIC_TYPE_SI32260))
            ret = load_si32260 (slicNum);
        else if (strstr (slic_type, SLIC_TYPE_LE89116))
            ret = load_le89116 (slicNum);
        else if (strstr (slic_type, SLIC_TYPE_LE9642))
            ret = load_le9642 (slicNum);
        else if (strstr (slic_type, SLIC_TYPE_LE9662))
            ret = load_le9662 (slicNum);
        else if (strstr (slic_type, SLIC_TYPE_SI32266))
            ret = load_si32266 (slicNum);	  
		else if (strstr (slic_type, SLIC_TYPE_SI32183))
            ret = load_si32183 (slicNum);
		else if (strstr (slic_type, SLIC_TYPE_SI32185))
            ret = load_si32185 (slicNum);
        else
        {
            printf ("unknown slic type %s,slic num:%d\n", slic_type,slicNum);
            ret = LOADER_FAILED;
        }
    }
    else {
        if (strstr(slic_type, SLIC_TYPE_LE89116) && strstr(slic_type, SLIC_TYPE_LE89316))
            ret = load_le89116_le89316 (slicNum);
        else{
            printf ("no support slic_type %s,slic_type_1 %s,slic num:%d\n", slic_type,slic_type_1,slicNum);
            ret = LOADER_FAILED;
        }
    }
		
    return ret;
}


int check_slic_type(IN char *slic_type)
{
    int ret = SLIC_CHECK_FAILED;

    if (strstr (slic_type, SLIC_TYPE_ZL88601))
        ret = SLIC_CHECK_SUCCESS;
    else if (strstr (slic_type, SLIC_TYPE_LE89156))
        ret = SLIC_CHECK_SUCCESS;
    else if (strstr (slic_type, SLIC_TYPE_LE89316))
        ret = SLIC_CHECK_SUCCESS;
    else if (strstr (slic_type, SLIC_TYPE_LE9641))
        ret = SLIC_CHECK_SUCCESS;
    else if (strstr (slic_type, SLIC_TYPE_SI32176))
        ret = SLIC_CHECK_SUCCESS;
    else if (strstr (slic_type, SLIC_TYPE_SI32260))
        ret = SLIC_CHECK_SUCCESS;
    else if (strstr (slic_type, SLIC_TYPE_LE89116))
        ret = SLIC_CHECK_SUCCESS;
    else if (strstr (slic_type, SLIC_TYPE_LE9642))
        ret = SLIC_CHECK_SUCCESS;
    else if (strstr (slic_type, SLIC_TYPE_LE9662))
        ret = SLIC_CHECK_SUCCESS;
    else if (strstr (slic_type, SLIC_TYPE_SI32266))
        ret = SLIC_CHECK_SUCCESS;
    else if (strstr (slic_type, SLIC_TYPE_SI32183))
        ret = SLIC_CHECK_SUCCESS;
    else if (strstr (slic_type, SLIC_TYPE_SI32185))
        ret = SLIC_CHECK_SUCCESS;
    else if (strstr (slic_type, SLIC_TYPE_SI32285))
        ret = SLIC_CHECK_SUCCESS;
    else if (strstr (slic_type, SLIC_TYPE_SI32287))
        ret = SLIC_CHECK_SUCCESS;
    else
    {
        ret = SLIC_CHECK_FAILED;
    }

    return ret;
}


static int load_common ()
{
    int ret = 0;
    char cmd[128] = {0};

    printf ("Loading common\n");

    snprintf (cmd, sizeof(cmd), "%s %s", INSMOD, M_KSOCKET);
    ret = system (cmd);
    ABNORMAL_CHECK (ret);
    snprintf (cmd, sizeof(cmd), "%s %s", INSMOD, M_ORTP);
    ret |= system (cmd);
    ABNORMAL_CHECK (ret);
    snprintf (cmd, sizeof(cmd), "%s %s", INSMOD, M_ACODEC_X);
    ret |= system (cmd);
    ABNORMAL_CHECK (ret);
    snprintf (cmd, sizeof(cmd), "%s %s", INSMOD, M_FOIP);
    ret |= system (cmd);
    ABNORMAL_CHECK (ret);
    snprintf (cmd, sizeof(cmd), "%s %s", INSMOD, M_OVDSP);
    ret |= system (cmd);
    ABNORMAL_CHECK (ret);
    snprintf (cmd, sizeof(cmd), "%s %s", INSMOD, M_PCMDUMP);
    ret |= system (cmd);
    ABNORMAL_CHECK (ret);

    if (ret == 0)
    	return LOADER_SUCCESS;
    else
        return LOADER_FAILED;
}

int load_zl88601 (IN int slicNum)
{
    int ret = 0;
    char cmd[128] = {0};

    printf ("Loading zl88601,slic num:%d\n",slicNum);

    if(slicNum == 1)
        snprintf (cmd, sizeof(cmd), "%s %s slic1=zl88601 slic2=zl88601", INSMOD, M_FXS3);
    else if(slicNum == 2)
        snprintf (cmd, sizeof(cmd), "%s %s slic1=zl88601 slic2=zl88601 slic3=zl88601_1 slic4=zl88601_1", INSMOD, M_FXS3);
    ret |= system (cmd);
    ABNORMAL_CHECK (ret);

    ret |= load_common ();    
    ABNORMAL_CHECK (ret);

    if (ret == 0)
    	return LOADER_SUCCESS;
    else
        return LOADER_FAILED;

}

int load_le89156 (IN int slicNum)
{
    int ret = 0;
    char cmd[128] = {0};
    printf ("Loading le89156,slic num:%d\n",slicNum);

    if(slicNum == 1)
    	snprintf (cmd, sizeof(cmd), "%s %s slic1=le89156", INSMOD, M_FXS3);
    else if(slicNum == 2)
        snprintf (cmd, sizeof(cmd), "%s %s slic1=le89156 slic2=le89156_2", INSMOD, M_FXS3);		
    ret |= system (cmd);
    ABNORMAL_CHECK (ret);

    ret |= load_common ();    
    ABNORMAL_CHECK (ret);

    if (ret == 0)
    	return LOADER_SUCCESS;
    else
        return LOADER_FAILED;

}

int load_le89316 (IN int slicNum)
{
    int ret = 0;
    char cmd[128] = {0};

    printf ("Loading le89316,slic num:%d\n",slicNum);

    if(slicNum == 1)
        snprintf (cmd, sizeof(cmd), "%s %s slic1=le89316 slic2=le89316", INSMOD, M_FXS3);
    ret |= system (cmd);
    ABNORMAL_CHECK (ret);

    ret |= load_common ();    
    ABNORMAL_CHECK (ret);

    if (ret == 0)
    	return LOADER_SUCCESS;
    else
        return LOADER_FAILED;

}

int load_le9641 (IN int slicNum)
{
    int ret = 0;
    char cmd[128] = {0};

    printf ("Loading le9641,slic num:%d\n",slicNum);

    if(slicNum == 1)
    	snprintf (cmd, sizeof(cmd), "%s %s slic1=le9641", INSMOD, M_FXS3);
    else if(slicNum == 2)
        snprintf (cmd, sizeof(cmd), "%s %s slic1=le9641 slic2=le9641_1", INSMOD, M_FXS3);	
    ret |= system (cmd);
    ABNORMAL_CHECK (ret);

    ret |= load_common ();    
    ABNORMAL_CHECK (ret);

    if (ret == 0)
    	return LOADER_SUCCESS;
    else
        return LOADER_FAILED;

}

int load_le89116 (IN int slicNum)
{
    int ret = 0;
    char cmd[128] = {0};

    printf ("Loading le89116,slic num:%d\n",slicNum);

    if(slicNum == 1)
    	snprintf (cmd, sizeof(cmd), "%s %s slic1=le89116", INSMOD, M_FXS3);
    else if(slicNum == 2)
        snprintf (cmd, sizeof(cmd), "%s %s slic1=le89116 slic2=le89116_1", INSMOD, M_FXS3);	
    ret |= system (cmd);
    ABNORMAL_CHECK (ret);

    ret |= load_common ();    
    ABNORMAL_CHECK (ret);

    if (ret == 0)
    	return LOADER_SUCCESS;
    else
        return LOADER_FAILED;

}

int load_le9642 (IN int slicNum)
{
    int ret = 0;
    char cmd[128] = {0};

    printf ("Loading le9642,slic num:%d\n",slicNum);

    if(slicNum == 1)
        snprintf (cmd, sizeof(cmd), "%s %s slic1=le9642 slic2=le9642", INSMOD, M_FXS3);
    else if(slicNum == 2)
        snprintf (cmd, sizeof(cmd), "%s %s slic1=le9642 slic2=le9642 slic3=le9642_1 slic4=le9642_1", INSMOD, M_FXS3);
    ret |= system (cmd);
    ABNORMAL_CHECK (ret);

    ret |= load_common ();    
    ABNORMAL_CHECK (ret);

    if (ret == 0)
    	return LOADER_SUCCESS;
    else
        return LOADER_FAILED;

}

int load_le9662 (IN int slicNum)
{
    int ret = 0;
    char cmd[128] = {0};

    printf ("Loading le9662,slic num:%d\n",slicNum);

    if(slicNum == 1)
        snprintf (cmd, sizeof(cmd), "%s %s slic1=le9662 slic2=le9662", INSMOD, M_FXS3);
    else if(slicNum == 2)
        snprintf (cmd, sizeof(cmd), "%s %s slic1=le9662 slic2=le9662 slic3=le9662_1 slic4=le9662_1", INSMOD, M_FXS3);
    ret |= system (cmd);
    ABNORMAL_CHECK (ret);

    ret |= load_common ();    
    ABNORMAL_CHECK (ret);

    if (ret == 0)
    	return LOADER_SUCCESS;
    else
        return LOADER_FAILED;

}

int load_le89116_le89316 (IN int slicNum)
{
    int ret = 0;
    char cmd[128] = {0};

    printf ("Loading le89116 and le89316,slic num:%d\n",slicNum);

    snprintf (cmd, sizeof(cmd), "%s %s slic1=le89116 slic2=le89316 slic3=le89316", INSMOD, M_FXS3);	
    ret |= system (cmd);
    ABNORMAL_CHECK (ret);

    ret |= load_common ();    
    ABNORMAL_CHECK (ret);

    if (ret == 0)
    	return LOADER_SUCCESS;
    else
        return LOADER_FAILED;

}


int load_si32176 (IN int slicNum)
{
    int ret = 0;
    char cmd[128] = {0};
    printf ("Loading si32176,slic num:%d\n",slicNum);

    if(slicNum == 1)
        snprintf (cmd, sizeof(cmd), "%s %s slic1=si32176", INSMOD, M_FXS3_SILICON);
    else if(slicNum == 2)
        snprintf (cmd, sizeof(cmd), "%s %s slic1=si32176 slic2=si32176_1", INSMOD, M_FXS3_SILICON);
    ret |= system (cmd);
    ABNORMAL_CHECK (ret);

    ret |= load_common ();    
    ABNORMAL_CHECK (ret);

    if (ret == 0)
    	return LOADER_SUCCESS;
    else
        return LOADER_FAILED;

}

int load_si32260 (IN int slicNum)
{
    int ret = 0;
    char cmd[128] = {0};
    printf ("Loading si32260,slic num:%d\n",slicNum);

    if(slicNum == 1)
        snprintf (cmd, sizeof(cmd), "%s %s slic1=si32260 slic2=si32260", INSMOD, M_FXS3_SILICON);
    else if(slicNum == 2)
        snprintf (cmd, sizeof(cmd), "%s %s slic1=si32260 slic2=si32260 slic3=si32260_1 slic4=si32260_1", INSMOD, M_FXS3_SILICON);
    ret |= system (cmd);
    ABNORMAL_CHECK (ret);

    ret |= load_common ();    
    ABNORMAL_CHECK (ret);

    if (ret == 0)
    	return LOADER_SUCCESS;
    else
        return LOADER_FAILED;

}

int load_si32266 (IN int slicNum)
{
    int ret = 0;
    char cmd[128] = {0};
    printf ("Loading si32266,slic num:%d\n",slicNum);

    if(slicNum == 1)
        snprintf (cmd, sizeof(cmd), "%s %s slic1=si32266 slic2=si32266", INSMOD, M_FXS3_SILICON);
    else if(slicNum == 2)
        snprintf (cmd, sizeof(cmd), "%s %s slic1=si32266 slic2=si32266 slic3=si32266_1 slic4=si32266_1", INSMOD, M_FXS3_SILICON);
    ret |= system (cmd);
    ABNORMAL_CHECK (ret);

    ret |= load_common ();    
    ABNORMAL_CHECK (ret);

    if (ret == 0)
    	return LOADER_SUCCESS;
    else
        return LOADER_FAILED;

}

int load_si32185 (IN int slicNum)
{
    int ret = 0;
    char cmd[128] = {0};
    printf ("Loading si32185,slic num:%d\n",slicNum);

    if(slicNum == 1)
        snprintf (cmd, sizeof(cmd), "%s %s slic1=si32185 ", INSMOD, M_FXS3_SILICON);
    else if(slicNum == 2)
        snprintf (cmd, sizeof(cmd), "%s %s slic1=si32185 slic2=si32185_1 ", INSMOD, M_FXS3_SILICON);
    ret |= system (cmd);
    ABNORMAL_CHECK (ret);

    ret |= load_common ();    
    ABNORMAL_CHECK (ret);

    if (ret == 0)
    	return LOADER_SUCCESS;
    else
        return LOADER_FAILED;

}

int load_si32183 (IN int slicNum)
{
    int ret = 0;
    char cmd[128] = {0};
    printf ("Loading si32183,slic num:%d\n",slicNum);

    if(slicNum == 1)
        snprintf (cmd, sizeof(cmd), "%s %s slic1=si32183", INSMOD, M_FXS3_SILICON);
    else if(slicNum == 2)
        snprintf (cmd, sizeof(cmd), "%s %s slic1=si32183 slic2=si32183_1 ", INSMOD, M_FXS3_SILICON);
    ret |= system (cmd);
    ABNORMAL_CHECK (ret);

    ret |= load_common ();    
    ABNORMAL_CHECK (ret);

    if (ret == 0)
    	return LOADER_SUCCESS;
    else
        return LOADER_FAILED;

}

int load_si32285 (IN int slicNum)
{
    int ret = 0;
    char cmd[128] = {0};
    printf ("Loading si32285,slic num:%d\n",slicNum);

    if(slicNum == 1)
        snprintf (cmd, sizeof(cmd), "%s %s slic1=si32285 slic2=si32285", INSMOD, M_FXS3_SILICON);
    else if(slicNum == 2)
        snprintf (cmd, sizeof(cmd), "%s %s slic1=si32285 slic2=si32285 slic3=si32285_1 slic4=si32285_1", INSMOD, M_FXS3_SILICON);
    ret |= system (cmd);
    ABNORMAL_CHECK (ret);

    ret |= load_common ();    
    ABNORMAL_CHECK (ret);

    if (ret == 0)
    	return LOADER_SUCCESS;
    else
        return LOADER_FAILED;

}

int load_si32287 (IN int slicNum)
{
    int ret = 0;
    char cmd[128] = {0};
    printf ("Loading si32287,slic num:%d\n",slicNum);

    if(slicNum == 1)
        snprintf (cmd, sizeof(cmd), "%s %s slic1=si32287 slic2=si32287", INSMOD, M_FXS3_SILICON);
    else if(slicNum == 2)
        snprintf (cmd, sizeof(cmd), "%s %s slic1=si32287 slic2=si32287 slic3=si32287_1 slic4=si32287_1", INSMOD, M_FXS3_SILICON);
    ret |= system (cmd);
    ABNORMAL_CHECK (ret);

    ret |= load_common ();    
    ABNORMAL_CHECK (ret);

    if (ret == 0)
    	return LOADER_SUCCESS;
    else
        return LOADER_FAILED;

}




int voiploader_check_unload(IN int interface_type, IN int slic_class, IN int slicNum)
{
	char cmd_check[128] = {0};

	if(slic_class == SLIC_CLASS_ZARLINK){
		snprintf (cmd_check, sizeof(cmd_check), "%s %s", RMMOD, M_SLIC3);
		system (cmd_check);
	}
	else{
		snprintf (cmd_check, sizeof(cmd_check), "%s %s", RMMOD, M_SLIC3_SILICON);
		system (cmd_check);
	}

	snprintf (cmd_check, sizeof(cmd_check), "%s %s", RMMOD, M_SPI);
	system (cmd_check);

	snprintf (cmd_check, sizeof(cmd_check), "%s %s", RMMOD, M_LEC_LOADER);
	system (cmd_check);

	if(slicNum == 2 && (interface_type == ZSI_TYPE || interface_type == ISI_TYPE)){
		snprintf (cmd_check, sizeof(cmd_check), "%s %s", RMMOD, M_PCM2);
		system (cmd_check);
	}

	snprintf (cmd_check, sizeof(cmd_check), "%s %s", RMMOD, M_PCM1);
	system (cmd_check);

	snprintf (cmd_check, sizeof(cmd_check), "%s %s", RMMOD, M_DSPCORE);
	system (cmd_check);

	snprintf (cmd_check, sizeof(cmd_check), "%s %s", RMMOD, M_SYS_MOD);
	system (cmd_check);				
		
}

int main (int argc, char *argv[])
{
	int ret = 0;
	int slicNum = 0;
	interface_class_e interface_type = SPI_TYPE;
	slic_class_e slic_class = SLIC_CLASS_ZARLINK;
	loader_check_e loader_check = LOADER_CHECK_NO;
	char slic_type[SLIC_TYPE_LEN_MAX] = {0};
	char slic_type_1[SLIC_TYPE_LEN_MAX] = {0};
	char cmd[128] = {0};
	char slic_info[64] = {0};
	FILE *fp_check = NULL;

	if(argc > 1){
		if (strstr ("check", argv[1])){
			printf ("voip_loader check start \n");
			loader_check = LOADER_CHECK_YES;
			fp_check = fopen (LOADER_CHECK_FILE, "w+");
			if (fp_check == NULL) 
			{
				printf ("Can not open file %s\n", LOADER_CHECK_FILE);
				return LOADER_FAILED;
			}
		}
		else{
			printf ("voip_loader start\n");
		}
	}
	else{
		printf ("voip_loader start\n");
	}
	
	ret = get_slic_type (slic_type, slic_type_1, &slicNum, &interface_type, &slic_class);
 
	if (LOADER_SUCCESS == ret){
		if(slicNum == 1){
			printf ("[SLIC] %s\n", slic_type);
			if(loader_check == LOADER_CHECK_YES){
				snprintf(slic_info, sizeof(slic_info), "slicNum:1 %s", slic_type);
				if(fp_check != NULL){
					fputs(slic_info, fp_check);
					fflush (fp_check);
					fclose(fp_check);
				}
				voiploader_check_unload(interface_type,slic_class,slicNum);
				return ret;
			}
		}
		else if(slicNum == 2){
			printf ("[SLIC1] %s  [SLIC2] %s\n", slic_type,slic_type_1);
			if(loader_check == LOADER_CHECK_YES){
				snprintf(slic_info, sizeof(slic_info), "slicNum:2 %s %s", slic_type, slic_type_1);
				if(fp_check != NULL){
					fputs(slic_info, fp_check);
					fflush (fp_check);
					fclose(fp_check);
				}
				voiploader_check_unload(interface_type,slic_class,slicNum);
				return ret;
			}
		}	
		ret = load_voip (slic_type, slic_type_1, slicNum);
	}
    else{
        snprintf (cmd, sizeof(cmd), "%s %s", RMMOD, M_SLIC3_SILICON);
        system (cmd);
		
        snprintf (cmd, sizeof(cmd), "%s %s", RMMOD, M_SPI);
        system (cmd);
	
        snprintf (cmd, sizeof(cmd), "%s %s", RMMOD, M_LEC_LOADER);
        system (cmd);
	
        snprintf (cmd, sizeof(cmd), "%s %s", RMMOD, M_PCM2);
        system (cmd);

        snprintf (cmd, sizeof(cmd), "%s %s", RMMOD, M_PCM1);
        system (cmd);
	
        snprintf (cmd, sizeof(cmd), "%s %s", RMMOD, M_DSPCORE);
        system (cmd);
	
        snprintf (cmd, sizeof(cmd), "%s %s", RMMOD, M_SYS_MOD);
        system (cmd);
   }
	
    printf ("voip loader end\n");
    return ret;
}

