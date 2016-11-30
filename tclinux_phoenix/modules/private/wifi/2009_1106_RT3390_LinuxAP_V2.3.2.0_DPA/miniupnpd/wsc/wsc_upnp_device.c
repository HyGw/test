///////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2000-2003 Ralink Corporation 
// All rights reserved. 
//
// Redistribution and use in source and binary forms, with or without 
// modification, are permitted provided that the following conditions are met: 
//
// * Redistributions of source code must retain the above copyright notice, 
// this list of conditions and the following disclaimer. 
// * Redistributions in binary form must reproduce the above copyright notice, 
// this list of conditions and the following disclaimer in the documentation 
// and/or other materials provided with the distribution. 
// * Neither name of Intel Corporation nor the names of its contributors 
// may be used to endorse or promote products derived from this software 
// without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR 
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL INTEL OR 
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, 
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR 
// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY 
// OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS 
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
///////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "upnphttp.h"
#include "upnpevents.h"
#include "upnpglobalvars.h"
#include "wsc_msg.h"
#include "wsc_common.h"
#include "wsc_ioctl.h"
#include "wsc_upnp.h"
#include "wsc_upnp_device.h"

#include "upnpreplyparse.h"/* for struct NameValueParserData */


//#include "sample_util.h"
//#include "ThreadPool.h"
//#include "service_table.h"
//#include "upnpapi.h"
//#include "ssdplib.h"

#define DO_NOTHING_IN_MAIN_LOOP	3
#define UUID_STR_LEN            36	
#define WSC_UPNP_UUID_STR_LEN	(5 + UUID_STR_LEN + 1)	// The UUID string get from the driver by ioctl and the strlen is 36 plus 1 '\0', 
						        // and we need extra 5 bytes for prefix string "uuid:"

//extern char HostDescURL[WSC_UPNP_DESC_URL_LEN];
//extern unsigned char HostMacAddr[MAC_ADDR_LEN];
//extern unsigned int HostIPAddr;
//extern int WscUPnPOpMode;

struct upnpCPNode *WscCPList = NULL;

struct upnpDevice wscLocalDevice;	// The data structure used the save the local Wsc UPnP Device.
int wscDevHandle = -1;	// Device handle of "wscLocalDevice" supplied by UPnP SDK.

/* The amount of time (in seconds) before advertisements will expire */
int defAdvrExpires = 100;


/* 
	Structure for storing Wsc Device Service identifiers and state table 
*/
#define WSC_ACTION_COUNTS		13
#define WSC_ACTION_MAXCOUNT		WSC_ACTION_COUNTS

typedef int (*upnp_action) (IXML_Document *request, uint32 ipAddr, IXML_Document **out, char **errorString);

struct WscDevActionList {
  char *actionNames[WSC_ACTION_COUNTS];
  upnp_action actionFuncs[WSC_ACTION_COUNTS];
};

struct WscDevActionList wscDevActTable;


//static char *wscStateVarName[] = {"WLANEvent", "APStatus", "STAStatus"};
static char wscStateVarCont[WSC_STATE_VAR_COUNT][WSC_STATE_VAR_MAX_STR_LEN];

static char *wscStateVarDefVal[] = {"", "0", "0"};

char wscAckMsg[]= {0x10, 0x4a, 0x00, 0x01, 0x10, 0x10, 0x22, 0x00, 0x01, 0x0d, 0x10, 0x1a, 0x00, 0x10, 0x00, 0x00,
				   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x39, 
				   0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
				   0x00, 0x00};


typedef enum _WSC_EVENT_STATUS_CODE{
	WSC_EVENT_STATUS_CONFIG_CHANGED = 0x00000001,
	WSC_EVENT_STATUS_AUTH_THRESHOLD_REACHED = 0x00000010,
}WSC_EVENT_STATUS_CODE;

typedef enum _WSC_EVENT_WLANEVENTTYPE_CODE{
	WSC_EVENT_WLANEVENTTYPE_PROBE = 1,
	WSC_EVENT_WLANEVENTTYPE_EAP = 2,
}WSC_EVENT_WLANEVENTTYPE_CODE;


int senderID = 1;
char CfgMacStr[18]={0};

#ifdef ENABLE_WSC_SERVICE
/************************************************************************
* Function : addToAction											
*																	
* Parameters:														
*	IN int response: flag to tell if the ActionDoc is for response 
*					or request
*	INOUT IXML_Document **ActionDoc: request or response document
*	IN char *ActionName: Name of the action request or response
*	IN char *ServType: Service type
*	IN char * ArgName: Name of the argument
*	IN char * ArgValue: Value of the argument
*
* Description:		
*	This function adds the argument in the action request or response. 
* This function creates the action request or response if it is a first
* argument else it will add the argument in the document
*
* Returns: int
*	returns 0 if successful else returns appropriate error
***************************************************************************/
int
addToAction( IN int response,
             INOUT IXML_Document * ActionDoc,
             IN const char *ActionName,
             IN const char *ServType,
             IN const char *ArgName,
             IN const char *ArgValue )
{
    int rc = 0;
	int buffer_size = 2000;

    if( ActionName == NULL || ServType == NULL )
	{
        return -1;
    }

	*ActionDoc = malloc(buffer_size);
	
	if (*ActionDoc == NULL)
	{
		printf("malloc failed in addToAction()\n");
        return -1;
	}

	memset(*ActionDoc, 0x00, buffer_size);
	
    if(response)
	{
		if (ArgName != NULL)
		{
		        sprintf( *ActionDoc,
		                 "<u:%sResponse xmlns:u=\"%s\"><%s>%s</%s></u:%sResponse>",
		                 ActionName, ServType, ArgName, ArgValue, ArgName, ActionName);
		}
		else
		{
		        sprintf( *ActionDoc,
		                 "<u:%sResponse xmlns:u=\"%s\"></u:%sResponse>",
		                 ActionName, ServType, ActionName);
		}
    } 
	else
	{
        sprintf( *ActionDoc, "<u:%s xmlns:u=\"%s\"></u:%s>",
                 ActionName, ServType, ActionName );
    }
    return rc;
}

/************************************************************************
* Function : UpnpAddToActionResponse									
*																	
* Parameters:
*	INOUT IXML_Document **ActionResponse: action response document	
*	IN char * ActionName: Name of the action request or response
*	IN char * ServType: Service type
*	IN int ArgName :Name of argument to be added in the action response
*	IN char * ArgValue : value of the argument
*
* Description:		
*	This function adds the argument in the action response. Its a wrapper 
* function that calls addToAction function to add the argument in the 
* action response.
*
* Returns: int
*	returns 0 if successful 
*	else returns appropriate error
***************************************************************************/
int
UpnpAddToActionResponse( INOUT IXML_Document * ActionResponse,
                         IN const char *ActionName,
                         IN const char *ServType,
                         IN const char *ArgName,
                         IN const char *ArgValue )
{
    return addToAction( 1, ActionResponse, ActionName, ServType, ArgName,
                        ArgValue );
}

/************************************************************************
* Function : UpnpAddToAction									
*																	
* Parameters:
*	INOUT IXML_Document **ActionDoc: action request document	
*	IN char * ActionName: Name of the action request or response
*	IN char * ServType: Service type
*	IN int ArgName :Name of argument to be added in the action response
*	IN char * ArgValue : value of the argument
*
* Description:		
*	This function adds the argument in the action request. Its a wrapper 
* function that calls addToAction function to add the argument in the 
* action request.
*
* Returns: int
*	returns 0 if successful 
*	else returns appropriate error
***************************************************************************/
int
UpnpAddToAction( INOUT IXML_Document * ActionDoc,
                 const char *ActionName,
                 const char *ServType,
                 const char *ArgName,
                 const char *ArgValue )
{

    return addToAction( 0, ActionDoc, ActionName, ServType, ArgName,
                        ArgValue );
}
#endif /* ENABLE_WSC_SERVICE */


/******************************************************************************
 * wscU2KMsgCreate
 *
 * Description: 
 *       Allocate the memory and copy the content to the buffer.
 *
 * Parameters:
 *    char **dstPtr - pointer used for refer the allocated U2Kmsg.
 *    char *srcPtr  - the message need to copy into the U2KMsg.
 *    int 	msgLen  - length of the message "srcPtr".
 *    int	EAPType - the EAP message type. 
 *    				 			1=Identity, 0xFE=reserved, used by WSC
 * Return Value:
 *    Total length of created U2KMsg
 *    	zero 	- if failure
 *    	others  - if success 
 *****************************************************************************/
int 
wscU2KMsgCreate(
	INOUT char **dstPtr,
	IN char *srcPtr,
	IN int msgLen,
	IN int EAPType)
{
	int totalLen;
	char *pPos = NULL, *pMsgPtr = NULL;
	RTMP_WSC_U2KMSG_HDR *pU2KMsgHdr; 
	IEEE8021X_FRAME *p1xFrameHdr;
	EAP_FRAME	*pEAPFrameHdr;
		
	/* Allocate the msg buffer and fill the content */
	totalLen = sizeof(RTMP_WSC_U2KMSG_HDR) + msgLen;
	if ((pMsgPtr = malloc(totalLen)) != NULL)
	{
		memset(pMsgPtr , 0, totalLen);
		pU2KMsgHdr = (RTMP_WSC_U2KMSG_HDR *)pMsgPtr;
		
		// create the IEEE8021X_FRAME header
		p1xFrameHdr = &pU2KMsgHdr->IEEE8021XHdr;
		p1xFrameHdr->Version = IEEE8021X_FRAME_VERSION;
		p1xFrameHdr->Length = htons(sizeof(EAP_FRAME) + msgLen);
		p1xFrameHdr->Type = IEEE8021X_FRAME_TYPE_EAP;

		// create the EAP header
		pEAPFrameHdr = &pU2KMsgHdr->EAPHdr;
		pEAPFrameHdr->Code = EAP_FRAME_CODE_RESPONSE;
		pEAPFrameHdr->Id = 1;  // The Id field is useless here.
		pEAPFrameHdr->Type = EAPType;
		pEAPFrameHdr->Length = htons(sizeof(EAP_FRAME) + msgLen);

		//copy the msg payload
		pPos = (char *)(pMsgPtr + sizeof(RTMP_WSC_U2KMSG_HDR));
		memcpy(pPos, srcPtr, msgLen);
		*dstPtr = pMsgPtr;
		DBGPRINTF(RT_DBG_INFO, "create U2KMsg success!MsgLen = %d, headerLen=%d! totalLen=%d!\n", msgLen, sizeof(RTMP_WSC_U2KMSG_HDR), totalLen);
	} else {
		DBGPRINTF(RT_DBG_INFO, "malloc allocation(size=%d) failed in wscU2KMsgCreate()!\n", totalLen);
		totalLen = 0;
	}

	return totalLen;
}


/*---------------------------action handler---------------------------*/

/******************************************************************************
 * WscDevGetAPSettings
 *
 * Description: 
 *       Wsc Service action callback used to get the AP's settings.
 *
 * Parameters:
 *    
 *    IXML_Document * in -  action request document
 *    IXML_Document **out - action result document
 *    char **errorString - errorString (in case action was unsuccessful)
 *  
 * Return:
 *    0 		- if success
 *    others 				- if failure
 *****************************************************************************/
int
WscDevGetAPSettings(
	IN struct upnphttp * h,
	IN uint32 ipAddr,
    IXML_Document * out,
    OUT char **errorString )
{
	// TODO: Need to complete it, currently do nothing and return directly.
	
	//create a response	
	if(UpnpAddToActionResponse(out, "GetAPSettings", WscServiceTypeStr, NULL, NULL) != 0)
	{
		(*out) = NULL;
		(*errorString) = "Internal Error";
		
		return -1;
	}
	
	return 0;

}


/******************************************************************************
 * WscDevGetSTASettings
 *
 * Description: 
 *      Wsc Service action callback used to get the STA's settings.
 *
 * Parameters:
 *   
 *    IXML_Document * in -  action request document
 *    IXML_Document **out - action result document
 *    char **errorString - errorString (in case action was unsuccessful)
 *  
 * Return:
 *    0 		- if success
 *    others 				- if failure
 *****************************************************************************/
int
WscDevGetSTASettings(
	IN struct upnphttp * h,
	IN uint32 ipAddr,
	OUT IXML_Document * out,
	OUT char **errorString)
{
	// TODO: Need to complete it, currently do nothing and return directly.
	
	//create a response	
	if(UpnpAddToActionResponse(out, "GetSTASettings", WscServiceTypeStr, NULL, NULL) != 0)
	{
		(*out) = NULL;
		(*errorString) = "Internal Error";
		
		return -1;
	}
	
	return 0;

}

/******************************************************************************
 * WscDevSetAPSettings
 *
 * Description: 
 *       Wsc Service action callback used to set the AP's settings.
 *
 * Parameters:
 *   
 *    IXML_Document * in -  action request document
 *    IXML_Document **out - action result document
 *    char **errorString - errorString (in case action was unsuccessful)
 *  
 * Return:
 *    0 		- if success
 *    others 				- if failure
 *****************************************************************************/
int
WscDevSetAPSettings(
	IN struct upnphttp * h,
	IN uint32 ipAddr,
	OUT IXML_Document * out,
	OUT char **errorString)
{
	// TODO: Need to complete it, currently do nothing and return directly.
	
	//create a response	
	if(UpnpAddToActionResponse(out, "SetAPSettings", WscServiceTypeStr, NULL, NULL) != 0)
	{
		(*out) = NULL;
		(*errorString) = "Internal Error";
		
		return -1;
	}
	
	return 0;

}


/******************************************************************************
 * WscDevDelAPSettings
 *
 * Description: 
 *       Wsc Service action callback used to delete the AP's settings.
 *
 * Parameters:
 *   
 *    IXML_Document * in -  action request document
 *    IXML_Document **out - action result document
 *    char **errorString - errorString (in case action was unsuccessful)
 *  
 * Return:
 *    0 		- if success
 *    others 				- if failure
 *****************************************************************************/
int
WscDevDelAPSettings(
	IN struct upnphttp * h,
	IN uint32 ipAddr,
	OUT IXML_Document * out,
	OUT char **errorString)
{
	// TODO: Need to complete it, currently do nothing and return directly.
	
	//create a response	
	if(UpnpAddToActionResponse(out, "DelAPSettings", WscServiceTypeStr, NULL, NULL) != 0)
	{
		(*out) = NULL;
		(*errorString) = "Internal Error";
		
		return -1;
	}
	
	return 0;

}


/******************************************************************************
 * WscDevSetSTASettings
 *
 * Description: 
 *       Wsc Service action callback used to set the STA's settings.
 *
 * Parameters:
 *  
 *    IXML_Document * in -  action request document
 *    IXML_Document **out - action result document
 *    char **errorString - errorString (in case action was unsuccessful)
 *  
 * Return:
 *    0 		- if success
 *    others 				- if failure
 *****************************************************************************/
int
WscDevSetSTASettings(
	IN struct upnphttp * h,
	IN uint32 ipAddr,
	OUT IXML_Document * out,
	OUT char **errorString)
{

	// TODO: Need to complete it, currently do nothing and return directly.
	
	//create a response	
	if(UpnpAddToActionResponse(out, "SetSTASettings", WscServiceTypeStr, NULL, NULL) != 0)
	{
		(*out) = NULL;
		(*errorString) = "Internal Error";
		
		return -1;
	}
	
	return 0;

}


/******************************************************************************
 * WscDevRebootAP
 *
 * Description: 
 *       Wsc Service action callback used to reboot the AP.
 *
 * Parameters:
 *   
 *    IXML_Document * in -  action request document
 *    IXML_Document **out - action result document
 *    char **errorString - errorString (in case action was unsuccessful)
 *  
 * Return:
 *    0 		- if success
 *    others 				- if failure
 *****************************************************************************/
int
WscDevRebootAP(
	IN struct upnphttp * h,
	IN uint32 ipAddr,
	OUT IXML_Document * out,
	OUT char **errorString)
{
	// TODO: Need to complete it, currently do nothing and return directly.
	
	//create a response	
	if(UpnpAddToActionResponse(out, "RebootAP", WscServiceTypeStr, NULL, NULL) != 0)
	{
		(*out) = NULL;
		(*errorString) = "Internal Error";
		
		return -1;
	}
	
	return 0;

}


/******************************************************************************
 * WscDevResetAP
 *
 * Description: 
 *       Wsc Service action callback used to reset the AP device to factory default config.
 *
 * Parameters:
 *   
 *    IXML_Document * in -  action request document
 *    IXML_Document **out - action result document
 *    char **errorString - errorString (in case action was unsuccessful)
 *  
 * Return:
 *    0 		- if success
 *    others 				- if failure
 *****************************************************************************/
int 
WscDevResetAP(
	IN struct upnphttp * h,
	IN uint32 ipAddr,
	OUT IXML_Document * out,
	OUT char **errorString)
{
	// TODO: Need to complete it, currently do nothing and return directly.
	
	//create a response	
	if(UpnpAddToActionResponse(out, "ResetAP", WscServiceTypeStr, NULL, NULL) != 0)
	{
		(*out) = NULL;
		(*errorString) = "Internal Error";
		
		return -1;
	}
	
	return 0;
}


/******************************************************************************
 * WscDevRebootSTA
 *
 * Description: 
 *       Wsc Service action callback used to reboot the STA.
 *
 * Parameters:
 *   
 *    IXML_Document * in -  action request document
 *    IXML_Document **out - action result document
 *    char **errorString - errorString (in case action was unsuccessful)
 *  
 * Return:
 *    0 		- if success
 *    others 				- if failure
 *****************************************************************************/
int
WscDevRebootSTA(
	IN struct upnphttp * h,
	IN uint32 ipAddr,
	OUT IXML_Document * out,
	OUT char **errorString)
{
	// TODO: Need to complete it, currently do nothing and return directly.
	
	//create a response	
	if(UpnpAddToActionResponse(out, "RebootSTA", WscServiceTypeStr, NULL, NULL) != 0)
	{
		(*out) = NULL;
		(*errorString) = "Internal Error";
		
		return -1;
	}
	
	return 0;
}


/******************************************************************************
 * WscDevResetSTA
 *
 * Description: 
 *       Wsc Service action callback used to reset the STA to factory default value.
 *
 * Parameters:
 *   
 *    IXML_Document * in -  action request document
 *    IXML_Document **out - action result document
 *    char **errorString - errorString (in case action was unsuccessful)
 *  
 * Return:
 *    0 		- if success
 *    others 				- if failure
 *****************************************************************************/
int
WscDevResetSTA(
	IN struct upnphttp * h,
	IN uint32 ipAddr,
	OUT IXML_Document * out,
	OUT char **errorString)
{
	
	// TODO: Need to complete it, currently do nothing and return directly.

	//create a response	
	if(UpnpAddToActionResponse(out, "ResetSTA", WscServiceTypeStr, NULL, NULL) != 0)
	{
		( *out ) = NULL;
		( *errorString ) = "Internal Error";
		
		return -1;
	}
	return 0;

}


/******************************************************************************
 * WscDevSetSelectedRegistrar
 *
 * Description: 
 *       This action callback used to receive a SetSelectedRegistar message send by
 *       contorl Point(Registrar). 
 *
 * Parameters:
 *   
 *    IXML_Document * in -  action request document
 *    IXML_Document **out - action result document
 *    char **errorString - errorString (in case action was unsuccessful)
 *  
 * Return:
 *    0 - if success
 *    others - if failure
 *****************************************************************************/
int WscDevSetSelectedRegistrar(
	IN struct upnphttp * h,
	IN uint32 ipAddr,
	OUT IXML_Document * out,
	OUT char **errorString)
{
	char *inStr = NULL;
	unsigned char *decodeStr = NULL;
	int decodeLen, retVal = -1;
	int inStrLen=0;	

	(*out) = NULL;
	(*errorString) = NULL;

	inStr = WSCGetValueFromNameValueList((char *)(h->req_buf + h->req_contentoff), "NewMessage", &inStrLen);
	ASSERT(inStrLen > 0);
	if (!inStr)
	{
		(*errorString) = "Invalid SetSelectedRegistrar mesg parameter";
		SoapError(h, 402, "Invalid Args");		
		return -1;
	}
	decodeLen = ILibBase64Decode((unsigned char *)inStr, inStrLen, &decodeStr);
	if (decodeLen == 0 || decodeStr == NULL)
	{
		goto done;
	}
#ifdef RT_DEBUG
	wsc_hexdump("WscDevSetSelectedRegistrar", (char *)decodeStr, decodeLen);
#endif
	/* Now send ioctl to wireless driver to set the ProbeReponse bit field. */
	if (ioctl_sock >= 0)
		retVal = wsc_set_oid(RT_OID_WSC_SET_SELECTED_REGISTRAR, (char *)decodeStr, decodeLen);

	if (retVal != 0)
		goto done;
	
	/*
		Send UPnP repsone to remote UPnP device controller 
	*/
	retVal = UpnpAddToActionResponse(out, "SetSelectedRegistrar", WscServiceTypeStr, NULL, NULL);
	if (retVal != 0)
	{
		retVal = -1;
	}
	else
	{
		BuildSendAndCloseSoapResp(h, *out, strlen(*out));
		if (*out != NULL)
		{
			free(*out);
		}
	}
done:
	if (inStr)
	{
//		free(inStr);
	}
	if (decodeStr)
		free(decodeStr);

	if (retVal != 0)
		(*errorString) = "Internal Error";
	
	return 0;
	
}


/******************************************************************************
 * WscDevPutWLANResponse
 *
 * Description: 
 *      When Device in Proxy Mode, the Registrar will use this Action response 
 *      the M2/M2D, M4, M6, and M8 messages.
 *
 * Parameters:
 *   
 *    IXML_Document * in -  action request document
 *    IXML_Document **out - action result document
 *    char **errorString - errorString (in case action was unsuccessful)
 *  
 * Return:
 *    0 - if success
 *    -1 - if failure
 *****************************************************************************/
int
WscDevPutWLANResponse(
	IN struct upnphttp * h,
	IN uint32 ipAddr,
	OUT IXML_Document * out,
	OUT char **errorString)
{
	char *inStr = NULL;
	unsigned char *decodeStr = NULL;
	int decodeLen, retVal = -1;
	
	char *pWscU2KMsg = NULL;
	RTMP_WSC_U2KMSG_HDR *msgHdr = NULL;
	int wscU2KMsgLen;
	int inStrLen=0;	

	(*out) = NULL;
	(*errorString) = NULL;

	inStr = WSCGetValueFromNameValueList((char *)(h->req_buf + h->req_contentoff), "NewMessage", &inStrLen);
	ASSERT(inStrLen > 0);
	if (!inStr)
	{
		(*errorString) = "Invalid PutWLANResponse mesg";
		SoapError(h, 402, "Invalid Args");		
		return retVal;
	}
	decodeLen = ILibBase64Decode((unsigned char *)inStr, inStrLen, &decodeStr);
	if (decodeLen == 0 || decodeStr == NULL)
	{
		goto done;
	}
	
	/* Prepare the msg buffers */
	wscU2KMsgLen = wscU2KMsgCreate(&pWscU2KMsg, (char *)decodeStr, decodeLen, EAP_FRAME_TYPE_WSC);
	if (wscU2KMsgLen == 0)
		goto done;

	// Fill the sessionID, Addr1, and Adde2 to the U2KMsg buffer header.
	msgHdr = (RTMP_WSC_U2KMSG_HDR *)pWscU2KMsg;
	msgHdr->envID = 0;
	memcpy(msgHdr->Addr1, HostMacAddr, MAC_ADDR_LEN);
	memcpy(msgHdr->Addr2, &ipAddr, sizeof(unsigned int));
	
	DBGPRINTF(RT_DBG_INFO, "(%s):Ready to send pWscU2KMsg(len=%d) to kernel by ioctl(%d)!\n", __FUNCTION__, 
				wscU2KMsgLen, ioctl_sock);

	wsc_hexdump("U2KMsg", pWscU2KMsg, wscU2KMsgLen);
	
	// Now send the msg to kernel space.
	if (ioctl_sock >= 0)
		retVal = wsc_set_oid(RT_OID_WSC_EAPMSG, (char *)pWscU2KMsg, wscU2KMsgLen);

	// Waiting for the response from the kernel space.
	DBGPRINTF(RT_DBG_INFO, "ioctl retval=%d! senderID=%d!\n", retVal, senderID);
	if(retVal == 0)
	{
		retVal = UpnpAddToActionResponse(out, "PutWLANResponse", WscServiceTypeStr, NULL, NULL);
	}
	BuildSendAndCloseSoapResp(h, *out, strlen(*out));

	if (*out != NULL)
	{
		free(*out);
	}
done:

	if (inStr)
	{
//		free(inStr);
	}
	if (decodeStr)
		free(decodeStr);
	if (pWscU2KMsg)
		free(pWscU2KMsg);
	
	if (retVal == 0)
		return 0;
	else {
		(*errorString) = "Internal Error";
		return -1;
	}
}


/******************************************************************************
 * WscDevPutMessageResp
 *
 * Description: 
 *       This action used by Registrar send WSC_MSG(M2,M4,M6, M8) to Enrollee.
 *
 * Parameters:
 *    
 *    IXML_Document * in - document of action request
 *    IXML_Document **out - action result
 *    char **errorString - errorString (in case action was unsuccessful)
 *
 * Return:
 *    0 - if success
 *    -1 - if failure
 *****************************************************************************/
int 
WscDevPutMessageResp(
	IN WscEnvelope *msgEnvelope)
{
	int retVal= -1;
	char *pWscU2KMsg = NULL;
	char *out = NULL; 
	struct upnphttp * h = msgEnvelope->h;


	DBGPRINTF(RT_DBG_INFO, "Got msg from netlink! envID=0x%x!\n", msgEnvelope->envID);

	( out ) = malloc(2500);

	if ((msgEnvelope->flag != WSC_ENVELOPE_SUCCESS) || 
		(msgEnvelope->pMsgPtr == NULL) || 
		(msgEnvelope->msgLen == 0))
	{
		goto done;
	}

	// Response the msg from state machine back to the remote UPnP control device.
	if (msgEnvelope->pMsgPtr)
	{
		unsigned char *encodeStr = NULL;
		RTMP_WSC_MSG_HDR *rtmpHdr = NULL;		
		int len;

		DBGPRINTF(RT_DBG_INFO, "(%s):Ready to parse the K2U msg(len=%d)!\n", __FUNCTION__, msgEnvelope->msgLen);
		wsc_hexdump("WscDevPutMessage-K2UMsg", msgEnvelope->pMsgPtr, msgEnvelope->msgLen);
		
		rtmpHdr = (RTMP_WSC_MSG_HDR *)(msgEnvelope->pMsgPtr);
	
		if(rtmpHdr->msgType == WSC_OPCODE_UPNP_CTRL || rtmpHdr->msgType == WSC_OPCODE_UPNP_MGMT)
		{
			DBGPRINTF(RT_DBG_INFO, "Receive a UPnP Ctrl/Mgmt Msg!\n");
			goto done;
		}
		
		if(rtmpHdr->msgSubType == WSC_UPNP_DATA_SUB_ACK)
		{
			retVal = UpnpAddToActionResponse(&out, "PutMessage", WscServiceTypeStr, NULL, NULL);
		}
		else 
		{
			len = ILibBase64Encode((unsigned char *)(msgEnvelope->pMsgPtr + sizeof(RTMP_WSC_MSG_HDR)), 
									rtmpHdr->msgLen, &encodeStr);
			if (len >0)
				retVal = UpnpAddToActionResponse(&out, "PutMessage", WscServiceTypeStr, 
													"NewOutMessage", (char *)encodeStr);
			if (encodeStr != NULL)
				free(encodeStr);
		}
		BuildSendAndCloseSoapResp(h, out, strlen(out));

		if (out != NULL)
		{
			free(out);
		}
	} 

done:
	if (pWscU2KMsg)
		free(pWscU2KMsg);
	wscEnvelopeFree(msgEnvelope);
	if (retVal == 0)
	{
		return retVal;
	}
	else
	{
		return -1;
	}
}



/******************************************************************************
 * WscDevPutMessage
 *
 * Description: 
 *       This action used by Registrar request WSC_MSG(M2,M4,M6, M8) from WIFI driver to Enrollee.
 *
 * Parameters:
 *    
 *    IXML_Document * in - document of action request
 *    IXML_Document **out - action result
 *    char **errorString - errorString (in case action was unsuccessful)
 *
 * Return:
 *    0 - if success
 *    -1 - if failure
 *****************************************************************************/
int 
WscDevPutMessage(
	IN struct upnphttp * h,
	IN uint32 ipAddr,
	OUT IXML_Document * out,
	OUT char **errorString)
{
	char *inStr = NULL;
	int inStrLen=0;
	unsigned char *decodeStr = NULL;
	int decodeLen, retVal = -1;
	
	char *pWscU2KMsg = NULL;
	RTMP_WSC_U2KMSG_HDR *msgHdr = NULL;
	WscEnvelope *msgEnvelope = NULL;
	int wscU2KMsgLen;
	int msgQIdx = -1;

	(*out) = NULL;
	(*errorString) = NULL;

	inStr = WSCGetValueFromNameValueList((char *)(h->req_buf + h->req_contentoff), "NewInMessage", &inStrLen);
	ASSERT(inStrLen > 0);
	if (!inStr)
	{
		(*errorString) = "Invalid PutMessage mesg";
		SoapError(h, 402, "Invalid Args");		
		return retVal;
	}
	decodeLen = ILibBase64Decode((unsigned char *)inStr, inStrLen, &decodeStr);
	if (decodeLen == 0 || decodeStr == NULL)
	{
		goto done;
	}

	/* Prepare the msg buffers */
	wscU2KMsgLen = wscU2KMsgCreate(&pWscU2KMsg, (char *)decodeStr, decodeLen, EAP_FRAME_TYPE_WSC);
	if (wscU2KMsgLen == 0)
		goto done;

	/* Prepare the msg envelope */
	if ((msgEnvelope = wscEnvelopeCreate()) == NULL)
		goto done;
	
	/* Lock the msgQ and check if we can get a valid mailbox to insert our request! */
	if(wscMsgQInsert(msgEnvelope, &msgQIdx)!= WSC_SYS_SUCCESS)
	{
		goto done;
	}

	/* log the socket ID and callback in the msgEnvelope */
	msgEnvelope->h = h;
	msgEnvelope->DevCallBack = WscDevPutMessageResp;
	
	// Fill the sessionID, Addr1, and Adde2 to the U2KMsg buffer header.
	msgHdr = (RTMP_WSC_U2KMSG_HDR *)pWscU2KMsg;
	msgHdr->envID = msgEnvelope->envID;
	memcpy(msgHdr->Addr1, HostMacAddr, MAC_ADDR_LEN);
	memcpy(msgHdr->Addr2, &ipAddr, sizeof(unsigned int));
	
	DBGPRINTF(RT_DBG_INFO, "(%s):Ready to send pWscU2KMsg(len=%d) to kernel by ioctl(%d)!\n", __FUNCTION__, 
				wscU2KMsgLen, ioctl_sock);
	wsc_hexdump("U2KMsg", pWscU2KMsg, wscU2KMsgLen);
	
	// Now send the msg to kernel space.
	if (ioctl_sock >= 0)
		retVal = wsc_set_oid(RT_OID_WSC_EAPMSG, (char *)pWscU2KMsg, wscU2KMsgLen);

	if (retVal == 0)
	{
		/* let main loop do nothing about the current http exchange */
		h->state = 3;

		// Waiting for the response from the kernel space.
		DBGPRINTF(RT_DBG_INFO, "%s():ioctl to kernel success, waiting for condition!\n", __FUNCTION__);

		if (inStr)
//			free(inStr);// I have no right to free this pointer, because the buffer is not alloc by me.
		if (decodeStr)
			free(decodeStr);
		if (pWscU2KMsg)
			free(pWscU2KMsg);
		return retVal;
	}
	else
	{
		DBGPRINTF(RT_DBG_INFO, "%s():ioctl to kernel failed, retVal=%d, goto done!\n", __FUNCTION__, retVal);
		wscMsgQRemove(Q_TYPE_PASSIVE, msgQIdx);
		goto done;
	}

done:
	if (inStr)
	{
//		free(inStr);// I have no right to free this pointer, because the buffer is not alloc by me.
	}
	if (decodeStr)
		free(decodeStr);
	if (pWscU2KMsg)
		free(pWscU2KMsg);

	wscEnvelopeFree(msgEnvelope);

	if (retVal == 0)
	{
		return retVal;
	}
	else
	{
		(*errorString) = "Internal Error";
		return -1;
	}
}


/******************************************************************************
 * WscDevGetDeviceInfoResp
 *
 * Description: 
 *       This action callback used to send AP's M1 message to the Control Point(Registrar).
 *
 * Parameters:
 *
 *    WscEnvelope *msgEnvelope  - response from wifi driver of action request
 *    
 *    
 *    
 *
 * Return:
 *    0 - if success
 *    -1 - if failure
 *****************************************************************************/
int
WscDevGetDeviceInfoResp(
	IN WscEnvelope *msgEnvelope)
{
	int retVal= -1;
	char *pWscU2KMsg = NULL;
	char *out = NULL; 
	struct upnphttp * h = msgEnvelope->h;

	DBGPRINTF(RT_DBG_INFO, "(%s):Got msg from netlink! envID=0x%x, flag=%d!\n", 
				__FUNCTION__, msgEnvelope->envID, msgEnvelope->flag);

	( out ) = malloc(2500);

	if ((msgEnvelope->flag != WSC_ENVELOPE_SUCCESS) || 
		(msgEnvelope->pMsgPtr == NULL) || 
		(msgEnvelope->msgLen == 0))
	{
		goto done;
	}

	// Response the msg from state machine back to the remote UPnP control device.
	if (msgEnvelope->pMsgPtr)
	{
		unsigned char *encodeStr = NULL;
		RTMP_WSC_MSG_HDR *rtmpHdr = NULL;		
		int len;

		rtmpHdr = (RTMP_WSC_MSG_HDR *)(msgEnvelope->pMsgPtr);
		DBGPRINTF(RT_DBG_INFO, "(%s):Ready to parse the K2U msg(TotalLen=%d, headerLen=%d)!\n", __FUNCTION__, 
						msgEnvelope->msgLen, sizeof(RTMP_WSC_MSG_HDR));
		DBGPRINTF(RT_DBG_INFO, "\tMsgType=%d!\n" 
								"\tMsgSubType=%d!\n"
								"\tipAddress=0x%x!\n"
								"\tMsgLen=%d!\n",
								rtmpHdr->msgType, rtmpHdr->msgSubType, rtmpHdr->ipAddr, rtmpHdr->msgLen);
		
		if(rtmpHdr->msgType == WSC_OPCODE_UPNP_CTRL || rtmpHdr->msgType == WSC_OPCODE_UPNP_MGMT)
		{
			DBGPRINTF(RT_DBG_INFO, "Receive a UPnP Ctrl/Mgmt Msg!\n");
			goto done;
		}
		wsc_hexdump("WscDevGetDeviceInfoResp-K2UMsg", msgEnvelope->pMsgPtr, msgEnvelope->msgLen);
		
		if(rtmpHdr->msgSubType == WSC_UPNP_DATA_SUB_ACK)
		{
			retVal = UpnpAddToActionResponse(&out, "GetDeviceInfo", WscServiceTypeStr, NULL, NULL);
		}
		else
		{
			len = ILibBase64Encode((unsigned char *)(msgEnvelope->pMsgPtr + sizeof(RTMP_WSC_MSG_HDR)), 
									rtmpHdr->msgLen, &encodeStr);

			if (len > 0)
			{
				retVal = UpnpAddToActionResponse(&out, "GetDeviceInfo", WscServiceTypeStr, 
												"NewDeviceInfo", (char *)encodeStr);
			}

			if (encodeStr != NULL)
				free(encodeStr);
		}

		BuildSendAndCloseSoapResp(h, out, strlen(out));

		if (out != NULL)
		{
			free(out);
		}
	} 

done:
	if (pWscU2KMsg)
		free(pWscU2KMsg);
	
	wscEnvelopeFree(msgEnvelope);
	
	if (retVal == 0)
	{
		return retVal;
	}
	else
	{
		return -1;
	}

}

	
/******************************************************************************
 * WscDevGetDeviceInfo
 *
 * Description: 
 *       This action callback used to send AP's M1 message to the Control Point(Registrar).
 *
 * Parameters:
 *
 *    IXML_Document * in  - document of action request
 *    uint32 ipAddr       - ipAddr,
 *    IXML_Document **out - action result
 *    char **errorString  - errorString (in case action was unsuccessful)
 *
 * Return:
 *    0 - if success
 *    -1 - if failure
 *****************************************************************************/
int
WscDevGetDeviceInfo(
	IN struct upnphttp * h,
	IN uint32 ipAddr,
	OUT IXML_Document * out,
	OUT char **errorString)
{
	char UPnPIdentity[] = {"WFA-SimpleConfig-Registrar"};
	int retVal= -1;
	char *pWscU2KMsg = NULL;
	WscEnvelope *msgEnvelope = NULL;
	RTMP_WSC_U2KMSG_HDR *msgHdr = NULL;
	int wscU2KMsgLen;
	int msgQIdx = -1;
	
	
	DBGPRINTF(RT_DBG_INFO, "Receive a GetDeviceInfo msg from Remote Upnp Control Point!\n");
	
	( *out ) = NULL;
	( *errorString ) = NULL;

	/* Prepare the msg buffers */
	wscU2KMsgLen = wscU2KMsgCreate(&pWscU2KMsg, UPnPIdentity, strlen(UPnPIdentity), EAP_FRAME_TYPE_IDENTITY);
	if (wscU2KMsgLen == 0)
		goto done;

	/* Prepare the msg envelope */
	if ((msgEnvelope = wscEnvelopeCreate()) == NULL)
		goto done;
	
	/* Lock the msgQ and check if we can get a valid mailbox to insert our request! */
	if (wscMsgQInsert(msgEnvelope, &msgQIdx) != WSC_SYS_SUCCESS)
	{
		goto done;
	}

	/* log the http info and callback at the msgEnvelope */
	msgEnvelope->h = h;
	msgEnvelope->DevCallBack = WscDevGetDeviceInfoResp;
	
	// Fill the sessionID, Addr1, and Adde2 to the U2KMsg buffer header.
	msgHdr = (RTMP_WSC_U2KMSG_HDR *)pWscU2KMsg;
	msgHdr->envID = msgEnvelope->envID;
	memcpy(msgHdr->Addr1, HostMacAddr, MAC_ADDR_LEN);
	memcpy(msgHdr->Addr2, &ipAddr, sizeof(int));
	
	// Now send the msg to kernel space.
	DBGPRINTF(RT_DBG_INFO, "(%s):Ready to send pWscU2KMsg(len=%d) to kernel by ioctl(%d)!\n", __FUNCTION__, 
				wscU2KMsgLen, ioctl_sock);
	wsc_hexdump("U2KMsg", pWscU2KMsg, wscU2KMsgLen);

	if (ioctl_sock >= 0)
		retVal = wsc_set_oid(RT_OID_WSC_EAPMSG, pWscU2KMsg, wscU2KMsgLen);
	else
		retVal = -1;
	
	if (retVal == 0)
	{
		/* let main loop do nothing about the current http exchange */
		h->state = DO_NOTHING_IN_MAIN_LOOP;

		// Waiting for the response from the kernel space.
		DBGPRINTF(RT_DBG_INFO, "%s():ioctl to kernel success, waiting for response!\n", __FUNCTION__);

		if (pWscU2KMsg)
			free(pWscU2KMsg);
		return retVal;
	}
	else
	{
		DBGPRINTF(RT_DBG_ERROR, "%s():ioctl to kernel failed, retVal=%d, goto done!\n", __FUNCTION__, retVal);
		wscMsgQRemove(Q_TYPE_PASSIVE, msgQIdx);
		goto done;
	}

	/* moved to WscDevGetDeviceInfoResp() */

done:
	if (pWscU2KMsg)
		free(pWscU2KMsg);
	
	wscEnvelopeFree(msgEnvelope);
	
	if (retVal == 0)
		return retVal;
	else
	{
		(*errorString) = "Internal Error";
		return -1;
	}
}
/*---------------------------end of action handler---------------------------*/



/******************************************************************************
 * WscDevHandleActionReq
 *
 * Description: 
 *       Called during an action request callback.  If the request is for the
 *       Wsc device and match the ServiceID, then perform the action and respond.
 *
 * Parameters:
 *   ca_event -- The "Upnp_Action_Request" structure
 *
 * Return:
 *    
 *****************************************************************************/
static int
WscDevHandleActionReq(IN struct upnphttp * h)
{
	return 0;
}


/******************************************************************************
 * WscDevCPNodeInsert
 *
 * Description: 
 *       Depends on the UDN String, service identifier and Subscription ID, find 
 *		 the IP address of the remote Control Point. And insert the Control Point
 *       into the "WscCPList".  
 *
 * NOTE: 
 *		 Since this function blocks on the mutex WscDevMutex, to avoid a hang this 
 *		 function should not be called within any other function that currently has 
 *		 this mutex locked.
 *
 *		 Besides, this function use the internal data structure of libupnp-1.3.1 
 *		 SDK, so if you wanna change the libupnp to other versions, please make sure
 *		 the data structure refered in this function was not changed!
 *
 * Parameters:
 *   UpnpDevice_Handle  - UpnpDevice handle of WscLocalDevice assigned by UPnP SDK.
 *   char *UDN			- The UUID string of the Subscription requested.
 *   char *servId 		- The service Identifier of the Subscription requested.
 *   char *sid 			- The Subscription ID of the ControlPoint.
 * 
 * Return:
 *   TRUE  - If success
 *   FALSE - If failure
 *****************************************************************************/
static int 
WscDevCPNodeInsert(
	IN int device_handle,
	IN const char *sid)
{
//	struct Handle_Info *handle_info;
	int found = 0;
	struct upnpCPNode *CPNode, *newCPNode;
	
	HandleLock();

	CPNode = WscCPList;
	while(CPNode)
	{
		if (strcmp(CPNode->device.SID, sid) == 0)
		{
			found = 1;
			break;
		}
		CPNode = CPNode->next;
	}

	if (found)
	{
		strncpy(CPNode->device.SID, sid, NAME_SIZE);
	}
	else
	{
		// It's a new subscriber, insert it.
		if ((newCPNode = malloc(sizeof(struct upnpCPNode))) != NULL)
		{
			memset(newCPNode, 0, sizeof(struct upnpCPNode));
			strncpy(newCPNode->device.SID, sid, NAME_SIZE);
			newCPNode->next = NULL;

			if(WscCPList)
			{
				newCPNode->next = WscCPList->next;
				WscCPList->next = newCPNode;
			}
			else
			{
				WscCPList = newCPNode;
			}
		}
		else 
		{
			goto Fail;
		}
	}

	HandleUnlock();
	DBGPRINTF(RT_DBG_INFO, "Insert ControlPoint success!\n");
		
	return TRUE;

Fail:
	
	HandleUnlock();
	DBGPRINTF(RT_DBG_ERROR, "Insert ControlPoint failed!\n");	
	return FALSE;
}



/******************************************************************************
 * WscDevCPNodeSearch
 *
 * Description: 
 *   Search for specific Control Point Node by ip address in WscCPList
 *
 * Parameters:
 *   unsigned int ipAddr - ip address of the contorl point we want to seach.
 *   char *sid           - used to copy the SID string
 *
 * Return:
 *   1 - if found
 *   0 - if not found
 *****************************************************************************/
static int 
WscDevCPNodeSearch(
	IN unsigned int ipAddr,
	OUT char **strSID)
{
	struct upnpCPNode *CPNode;
	int found = 0;
	
	
	CPNode = WscCPList;
	while (CPNode)
	{
		if(CPNode->device.ipAddr == ipAddr)
		{
			*strSID = strdup(CPNode->device.SID);
			found = 1;
			break;
		}
		CPNode = CPNode->next;
	}

	return found;
}


/******************************************************************************
 * WscDevCPListRemoveAll
 *
 * Description: 
 *   Remove all Control Point Node in WscCPList
 *
 * Parameters:
 *   None
 *
 * Return:
 *   TRUE
 *****************************************************************************/
static int WscDevCPListRemoveAll(void)
{
	struct upnpCPNode *CPNode;

	while((CPNode = WscCPList))
	{
		WscCPList = CPNode->next;
		free(CPNode);
	}
	
	return TRUE;
}

/******************************************************************************
 * WscDevCPNodeRemove
 *
 * Description: 
 *       Remove the ControlPoint Node in WscCPList depends on the subscription ID.
 *
 * Parameters:
 *   char *SID - The subscription ID of the ControlPoint will be deleted
 * 
 * Return:
 *   TRUE  - If success
 *   FALSE - If failure
 *****************************************************************************/
int WscDevCPNodeRemove(IN char *SID)
{
	struct upnpCPNode *CPNode, *prevNode = NULL;
	
	CPNode = prevNode = WscCPList;

	if(strcmp(WscCPList->device.SID, SID) == 0)
	{
		WscCPList = WscCPList->next;
		free(CPNode);
	} 
	else
	{
		while((CPNode = CPNode->next))
		{
			if(strcmp(CPNode->device.SID, SID) == 0)
			{
				prevNode->next = CPNode->next;
				free(CPNode);
				break;
			}
			prevNode = CPNode;
		}
	}

	return TRUE;
}


/******************************************************************************
 * dumpDevCPNodeList
 *
 * Description: 
 *       Dump the WscCPList. Used for debug.
 *
 * Parameters:
 *   	void
 *
 *****************************************************************************/
void dumpDevCPNodeList(void)
{
	struct upnpCPNode *CPNode;
	int i=0;
	
	printf("Dump The UPnP Subscribed ControlPoint:\n");

	CPNode = WscCPList;	
	while(CPNode)
	{
		i++;
		printf("ControlPoint Node[%d]:\n", i);
		printf("\t ->sid=%s\n", CPNode->device.SID);
		printf("\t ->SubURL=%s\n", CPNode->device.SubURL);
		printf("\t ->ipAddr=0x%x!\n", CPNode->device.ipAddr);
		printf("\t ->SubTimeOut=%d\n", CPNode->device.SubTimeOut);
		CPNode = CPNode->next;
	}
	
	printf("\n-----DumpFinished!\n");

}


/******************************************************************************
 * WscDevStateVarUpdate
 *
 * Description: 
 *       Update the Wsc Device Service State variable, and notify all subscribed 
 *       Control Points of the updated state.  Note that since this function
 *       blocks on the mutex WscDevMutex, to avoid a hang this function should 
 *       not be called within any other function that currently has this mutex 
 *       locked.
 *
 * Parameters:
 *   variable -- The variable number (WSC_EVENT_WLANEVENT, WSC_EVENT_APSTATUS,
 *                   WSC_EVENT_STASTATUS)
 *   value -- The string representation of the new value
 *
 *****************************************************************************/
static int
WscDevStateVarUpdate(
	IN unsigned int variable,
	IN char *value,
	IN char *SubsId)
{
	struct subscriber* sub = NULL;
	int sub_found = 0;
	
	if((variable >= WSC_STATE_VAR_MAXVARS) || (strlen(value) >= WSC_STATE_VAR_MAX_STR_LEN))
		return (0);

	strcpy(wscLocalDevice.services.StateVarVal[variable], value);

	if (SubsId == NULL)
	{
		upnp_event_var_change_notify(EWSC);
	}
	else 
	{
		sub_found = WscDevSubscriberSearch(SubsId, &sub);

		if ((sub_found) && (sub->service == EWSC))
		{
			if (sub != NULL)
			{
				if ((sub->service == EWSC) && (sub->notify == NULL))
				{
					upnp_event_create_notify(sub);
				}				
			}
		}
		else if (!sub_found)
		{
			DBGPRINTF(RT_DBG_ERROR, "\n%s() : ERROR!!! : The subscriber can not be found in subscriberlist.\n", __FUNCTION__); 
		}
	}

    return (1);
}


/*
	The format of UPnP WLANEvent message send to Remote Registrar.
	  WLANEvent:
		=>This variable represents the concatenation of the WLANEventType, WLANEventMac and the 
		  802.11 WSC message received from the Enrollee & forwarded by the proxy over UPnP.
		=>the format of this variable
			->represented as base64(WLANEventType || WLANEventMAC || Enrollee's message).
	  WLANEventType:
		=>This variable represents the type of WLANEvent frame that was received by the proxy on 
		  the 802.11 network.
		=>the options for this variable
			->1: 802.11 WCN-NET Probe Frame
			->2: 802.11 WCN-NET 802.1X/EAP Frame
	  WLANEventMAC:
		=>This variable represents the MAC address of the WLAN Enrollee that generated the 802.11 
	 	  frame that was received by the proxy.
	 	=>Depends on the WFAWLANConfig:1  Service Template Version 1.01, the format is
 			->"xx:xx:xx:xx:xx:xx", case-independent, 17 char
*/	
int WscEventCtrlMsgRecv(
	IN char *pBuf,
	IN int  bufLen)
{

	DBGPRINTF(RT_DBG_INFO, "Receive a Control Message!\n");
	return 0;

}


//receive a WSC message and send it to remote UPnP Contorl Point
int WscEventDataMsgRecv(
	IN char *pBuf,
	IN int  bufLen)
{
	RTMP_WSC_MSG_HDR *pHdr = NULL;
	unsigned char *encodeStr = NULL, *pWscMsg = NULL, *pUPnPMsg = NULL;
	int encodeLen = 0, UPnPMsgLen = 0;
	int retVal;
	uint32 wscLen;
	char *strSID = NULL;
	unsigned char includeMAC = 0;
	char curMacStr[18];
	
	if ((WscUPnPOpMode & WSC_UPNP_OPMODE_DEV) == 0)
		return -1;
		
	pHdr = (RTMP_WSC_MSG_HDR *)pBuf;
		
#if 1
	// Find the SID.
	if(WscDevCPNodeSearch(pHdr->ipAddr, &strSID) == 0)
	{
		DBGPRINTF(RT_DBG_INFO, "%s(): Didn't find the SID by ip(0x%x)!\n", __FUNCTION__, pHdr->ipAddr);
	}
	else
	{
		DBGPRINTF(RT_DBG_INFO, "%s(): The SID(%s) by ip(0x%x)!\n", __FUNCTION__, strSID, pHdr->ipAddr);
	}
#endif	
	DBGPRINTF(RT_DBG_INFO, "%s:Receive a Data event, msgSubType=%d!\n", __FUNCTION__, pHdr->msgSubType);

	memset(curMacStr, 0 , sizeof(curMacStr));
	if ((pHdr->msgSubType & WSC_UPNP_DATA_SUB_INCLUDE_MAC) == WSC_UPNP_DATA_SUB_INCLUDE_MAC)
	{
		if (pHdr->msgLen < 6){
			DBGPRINTF(RT_DBG_ERROR, "pHdr->msgSubType didn't have enoguh length!\n", pHdr->msgLen);
			return -1;
		}
		includeMAC = 1;
		pHdr->msgSubType &= 0x00ff;
		pWscMsg = (unsigned char *)(pBuf + sizeof(RTMP_WSC_MSG_HDR));
		snprintf(curMacStr, 18, "%02x:%02x:%02x:%02x:%02x:%02x", pWscMsg[0], pWscMsg[1], pWscMsg[2], pWscMsg[3],pWscMsg[4],pWscMsg[5]);	
	}
	else
	{
		memcpy(&curMacStr[0], CfgMacStr, 17);
	}

	
	if (pHdr->msgSubType == WSC_UPNP_DATA_SUB_NORMAL || 
		pHdr->msgSubType == WSC_UPNP_DATA_SUB_TO_ALL ||
		pHdr->msgSubType == WSC_UPNP_DATA_SUB_TO_ONE)
	{

		DBGPRINTF(RT_DBG_INFO, "(%s):Ready to parse the K2U msg(len=%d)!\n", __FUNCTION__, bufLen);

		pWscMsg = (unsigned char *)(pBuf + sizeof(RTMP_WSC_MSG_HDR));
		//Get the WscData Length
		wscLen = pHdr->msgLen;
		
		if (includeMAC)
		{
			wscLen -= MAC_ADDR_LEN;
			pWscMsg += MAC_ADDR_LEN;
		}
		
		DBGPRINTF(RT_DBG_INFO, "(%s): pWscMsg Len=%d!\n", __FUNCTION__, wscLen);
			
			
		UPnPMsgLen = wscLen + 18;
		pUPnPMsg = malloc(UPnPMsgLen);
		if(pUPnPMsg)
		{
			memset(pUPnPMsg, 0, UPnPMsgLen);
			pUPnPMsg[0] = WSC_EVENT_WLANEVENTTYPE_EAP;
			memcpy(&pUPnPMsg[1], &curMacStr[0], 17);

			//Copy the WscMsg to pUPnPMsg buffer
			memcpy(&pUPnPMsg[18], pWscMsg, wscLen);
			wsc_hexdump("UPnP WLANEVENT Msg", (char *)pUPnPMsg, UPnPMsgLen);

			//encode the message use base64
			encodeLen = ILibBase64Encode(pUPnPMsg, UPnPMsgLen, &encodeStr);
			
			//Send event out
			if (encodeLen > 0){
				DBGPRINTF(RT_DBG_INFO, "EAP->Msg=%s!\n", encodeStr);
				retVal = WscDevStateVarUpdate(WSC_EVENT_WLANEVENT, (char *)encodeStr, strSID);
			}
			if (encodeStr != NULL)
				free(encodeStr);
			free(pUPnPMsg);
		}
	}
	
	if (strSID)
		free(strSID);
	
	return 0;
	
}


/*
	Format of iwcustom msg WSC RegistrarSelected message:
	1. The Registrar ID which send M2 to the Enrollee(4 bytes):
								
			  4
		+-----------+
		|RegistrarID|
*/
static int WscEventMgmt_RegSelect(	
	IN char *pBuf,
	IN int  bufLen)
{
	char *pWscMsg = NULL;
	int registrarID = 0;
		
	if ((WscUPnPOpMode & WSC_UPNP_OPMODE_DEV) == 0)
		return -1;
	
	DBGPRINTF(RT_DBG_INFO, "(%s):Ready to parse the K2U msg(len=%d)!\n", __FUNCTION__, bufLen);
	wsc_hexdump("WscEventMgmt_RegSelect-K2UMsg", pBuf, bufLen);

	pWscMsg = (char *)(pBuf + sizeof(RTMP_WSC_MSG_HDR));

	registrarID = *((int *)pWscMsg);
	DBGPRINTF(RT_DBG_INFO, "The registrarID=%d!\n", registrarID);
	
	return 0;

}

/*
	Format of iwcustom msg WSC clientJoin message:
	1. SSID which station want to probe(32 bytes):
			<SSID string>
		*If the length if SSID string is small than 32 bytes, fill 0x0 for remaining bytes.
	2. Station MAC address(6 bytes):
	3. Status:
		Value 1 means change APStatus as 1. 
		Value 2 means change STAStatus as 1.
		Value 3 means trigger msg.
								
			32         6       1 
		+----------+--------+------+
		|SSIDString| SrcMAC |Status|
*/
static int WscEventMgmt_ConfigReq(
	IN char *pBuf,
	IN int  bufLen)
{
	unsigned char *encodeStr = NULL, *pWscMsg = NULL, *pUPnPMsg = NULL;
	int encodeLen = 0, UPnPMsgLen = 0;
	int retVal;
	unsigned char Status;
	char triggerMac[18];
		
	
	DBGPRINTF(RT_DBG_INFO, "(%s):Ready to parse the K2U msg(len=%d)!\n", __FUNCTION__, bufLen);
	wsc_hexdump("WscEventMgmt_ConfigReq-K2UMsg", pBuf, bufLen);

	pWscMsg = (unsigned char *)(pBuf + sizeof(RTMP_WSC_MSG_HDR));

	memset(&triggerMac[0], 0, 18);
	memcpy(&triggerMac[0], pWscMsg, 18);
	//Skip the SSID field
	pWscMsg += 32;
			
#if 0
	// Get the SrcMAC and copy to the WLANEVENTMAC, in format "xx:xx:xx:xx:xx:xx", case-independent, 17 char.
	memset(CfgMacStr, 0 , sizeof(CfgMacStr));
	snprintf(CfgMacStr, 18, "%02x:%02x:%02x:%02x:%02x:%02x", pWscMsg[0], pWscMsg[1], pWscMsg[2],
															pWscMsg[3],pWscMsg[4],pWscMsg[5]);
#endif
	//Skip the SrcMAC field
	pWscMsg += 6;

	//Change APStatus and STAStatus
	Status = *(pWscMsg);
	DBGPRINTF(RT_DBG_INFO, "(%s): Status =%d!\n", __FUNCTION__, Status);

	if ((WscUPnPOpMode & WSC_UPNP_OPMODE_DEV) && (strlen(triggerMac) == 0))
	{
		strcpy(wscLocalDevice.services.StateVarVal[WSC_EVENT_APSTATUS], "0");
		strcpy(wscLocalDevice.services.StateVarVal[WSC_EVENT_STASTATUS], "0");

		/* not needed right now */
#if 0
		if(Status == 3)
		{
			/* This "ConfigReq" is the trigger msg, we should send BYEBYE to all external registrar for 
				reset the cache status of those Vista devices. Then re-send Notify out.
			*/
	    	retVal = AdvertiseAndReply(-1, wscDevHandle, 0, (struct sockaddr_in *)NULL,(char *)NULL, 
									(char *)NULL, (char *)NULL, defAdvrExpires);
		    retVal = AdvertiseAndReply(1, wscDevHandle, 0, (struct sockaddr_in *)NULL,(char *)NULL, 
									(char *)NULL, (char *)NULL, defAdvrExpires);
		}
#endif
		//Prepare the message.
		UPnPMsgLen = 18 + sizeof(wscAckMsg);
		pUPnPMsg = malloc(UPnPMsgLen);
		
		if(pUPnPMsg)
		{
			memset(pUPnPMsg, 0, UPnPMsgLen);
			pUPnPMsg[0] = WSC_EVENT_WLANEVENTTYPE_EAP;
			memcpy(&pUPnPMsg[1], CfgMacStr, 17);

			//jump to the WscProbeReqData and copy to pUPnPMsg buffer
			pWscMsg++;	
			memcpy(&pUPnPMsg[18], wscAckMsg, sizeof(wscAckMsg));
			wsc_hexdump("UPnP WLANEVENT Msg", (char *)pUPnPMsg, UPnPMsgLen);
					
			//encode the message use base64
			encodeLen = ILibBase64Encode(pUPnPMsg, UPnPMsgLen, &encodeStr);
			if(encodeLen > 0)
				DBGPRINTF(RT_DBG_INFO, "ConfigReq->Msg=%s!\n", encodeStr);
			strcpy(wscLocalDevice.services.StateVarVal[WSC_EVENT_WLANEVENT], (const char *)encodeStr);
			
			//Send event out
			if (encodeLen > 0)
				retVal = WscDevStateVarUpdate(WSC_EVENT_WLANEVENT, (char *)encodeStr, NULL);

			if (encodeStr != NULL)
				free(encodeStr);
			free(pUPnPMsg);
		}
	}

	if ((WscUPnPOpMode & WSC_UPNP_OPMODE_CP) && (strlen(triggerMac) > 0))
	{
		
	}

	DBGPRINTF(RT_DBG_INFO, "<-----End(%s)\n", __FUNCTION__);
	return 0;
}

/*
	Format of iwcustom msg WSC ProbeReq message:
	1. SSID which station want to probe(32 bytes):
			<SSID string>
		*If the length if SSID string is small than 32 bytes, fill 0x0 for remaining bytes.
	2. Station MAC address(6 bytes):
	3. element ID(OID)(1 byte):
			val=0xDD
	4. Length of "WscProbeReqData" in the probeReq packet(1 byte):
	5. "WscProbeReqData" info in ProbeReq:
								
			32        6      1          1          variable length
		+----------+--------+---+-----------------+----------------------+
		|SSIDString| SrcMAC |eID|LenOfWscProbeData|    WscProbeReqData   |

*/
static int WscEventMgmt_ProbreReq(
	IN char *pBuf,
	IN int  bufLen)
{
	unsigned char *encodeStr = NULL, *pWscMsg = NULL, *pUPnPMsg = NULL;
	char srcMacStr[18];
	int encodeLen = 0, UPnPMsgLen = 0;
	int retVal;
	unsigned char wscLen;
		
	if ((WscUPnPOpMode & WSC_UPNP_OPMODE_DEV) == 0)
		return -1;
	
	DBGPRINTF(RT_DBG_INFO, "(%s):Ready to parse the K2U msg(len=%d)!\n", __FUNCTION__, bufLen);
	wsc_hexdump("WscMgmtEvent_ProbreReq-K2UMsg", pBuf, bufLen);

	pWscMsg = (unsigned char *)(pBuf + sizeof(RTMP_WSC_MSG_HDR));
	//Skip the SSID field
	pWscMsg += 32;
			
	/* Get the SrcMAC and copy to the WLANEVENTMAC, 
		depends on the WFAWLANConfig:1  Service Template Version 1.01, 
		the MAC format is "xx:xx:xx:xx:xx:xx", case-independent, 17 char.
	*/
	memset(srcMacStr, 0 , sizeof(srcMacStr));
	snprintf(srcMacStr, 17, "%02x:%02x:%02x:%02x:%02x:%02x", pWscMsg[0], pWscMsg[1], pWscMsg[2],pWscMsg[3],pWscMsg[4],pWscMsg[5]);
	DBGPRINTF(RT_DBG_INFO, "ProbeReq->Mac=%s!\n", srcMacStr);

	//Skip the SrcMAC field & eID field
	pWscMsg += (6 + 1);

	//Get the WscProbeData Length
	wscLen = *(pWscMsg);
	DBGPRINTF(RT_DBG_INFO, "(%s): pWscHdr Len=%d!\n", __FUNCTION__, wscLen);
		
	UPnPMsgLen = wscLen + 18;
	pUPnPMsg = malloc(UPnPMsgLen);
	
	if(pUPnPMsg)
	{
		memset(pUPnPMsg, 0, UPnPMsgLen);
		pUPnPMsg[0] = WSC_EVENT_WLANEVENTTYPE_PROBE;
		memcpy(&pUPnPMsg[1], srcMacStr, 17);

		//jump to the WscProbeReqData and copy to pUPnPMsg buffer
		pWscMsg++;	
		memcpy(&pUPnPMsg[18], pWscMsg, wscLen);
		wsc_hexdump("UPnP WLANEVENT Msg", (char *)pUPnPMsg, UPnPMsgLen);
				
		//encode the message use base64
		encodeLen = ILibBase64Encode(pUPnPMsg, UPnPMsgLen, &encodeStr);
			
		//Send event out
		if (encodeLen > 0){
			DBGPRINTF(RT_DBG_INFO, "ProbeReq->Msg=%s!\n", encodeStr);
			retVal = WscDevStateVarUpdate(WSC_EVENT_WLANEVENT, (char *)encodeStr, NULL);
		}
		if (encodeStr != NULL)
			free(encodeStr);
		free(pUPnPMsg);
	}
	
	return 0;
	
}


int WscEventMgmtMsgRecv(
	IN char *pBuf,
	IN int  bufLen)
{
	RTMP_WSC_MSG_HDR *pHdr = NULL;	
	
	pHdr = (RTMP_WSC_MSG_HDR *)pBuf;
	if (!pHdr)
		return -1;

	if (pHdr->msgType != WSC_OPCODE_UPNP_MGMT)
		return -1;

	DBGPRINTF(RT_DBG_INFO, "%s:Receive a MGMT event, msgSubType=%d\n", __FUNCTION__, pHdr->msgSubType);
	switch(pHdr->msgSubType)
	{
		case WSC_UPNP_MGMT_SUB_PROBE_REQ:
			WscEventMgmt_ProbreReq(pBuf, bufLen);
			break;
		case WSC_UPNP_MGMT_SUB_CONFIG_REQ:
			WscEventMgmt_ConfigReq(pBuf, bufLen);
			break;
		case WSC_UPNP_MGMT_SUB_REG_SELECT:
			WscEventMgmt_RegSelect(pBuf, bufLen);
			break;
		default:
			DBGPRINTF(RT_DBG_ERROR, "Un-Supported WSC Mgmt event type(%d)!\n", pHdr->msgSubType);
			break;
	}

	return 0;
}


/******************************************************************************
 * WscDevHandleSubscriptionReq
 *
 * Description: 
 *       Called during a subscription request callback.  If the subscription 
 *       request is for the Wsc device and match the serviceId, then accept it.
 *
 * Parameters:
 *   sr_event -- The "Upnp_Subscription_Request" structure
 *
 * Return:
 *   TRUE 
 *****************************************************************************/
int
WscDevHandleSubscriptionReq(IN struct upnphttp * h)
{
	/*  Insert this Control Point into WscCPList*/
	WscDevCPNodeInsert(wscDevHandle, h->req_SID);
	dumpDevCPNodeList();

	return TRUE;
}


/******************************************************************************
 * WscDevServiceHandler
 *
 * Description: 
 *       The callback handler registered with the SDK while registering
 *       root device.  Dispatches the request to the appropriate procedure
 *       based on the value of EventType. The four requests handled by the 
 *       device are: 
 *             1) Event Subscription requests.  
 *             2) Get Variable requests. 
 *             3) Action requests.
 *
 * Parameters:
 *
 *   EventType - The type of callback event
 *   Event     - Data structure containing event data
 *   Cookie    - Optional data specified during callback registration
 *
 * Return:
 *   Zero 
 *****************************************************************************/
int WscDevServiceHandler(
	struct upnphttp * h)
{

	switch (h->req_command) 
	{
		case ESubscribe:
			WscDevHandleSubscriptionReq(h);
			break;

		case EUnSubscribe:
			WscDevCPNodeRemove((char *)(h->req_SID));
			break;

		case EGet:
		case EPost:
			/* do nothing */
			WscDevHandleActionReq(h);
			break;

		default:
			DBGPRINTF(RT_DBG_ERROR, "Error in WscDevServiceHandler: unknown event type %d\n", h->req_command);
	}

	return (0);
}


/******************************************************************************
 * WscLocalDevServTbInit
 *
 * Description: 
 *     Initializes the service table for the Wsc UPnP service.
 *     Note that knowledge of the service description is assumed. 
 *
 * Paramters:
 *     None
 *
 * Return:
 *      always TRUE.
 *****************************************************************************/
static int WscLocalDevServTbInit(void)
{
	unsigned int i = 0;

	for (i = 0; i < WSC_STATE_VAR_MAXVARS; i++)
	{
		wscLocalDevice.services.StateVarVal[i] = wscStateVarCont[i];
		strncpy(wscLocalDevice.services.StateVarVal[i], wscStateVarDefVal[i], WSC_STATE_VAR_MAX_STR_LEN-1);
	}

	/* replaced by soapMethods[] in upnpsoap.c */
		
	return TRUE;
}


/******************************************************************************
 * WscUPnPDevStop
 *
 * Description: 
 *     Stops the device. Uninitializes the sdk. 
 *
 * Parameters:
 *     None
 * Return:
 *     TRUE 
 *****************************************************************************/
int WscUPnPDevStop(void)
{
//	UpnpUnRegisterRootDevice(wscDevHandle);
	WscDevCPListRemoveAll();
	
	return 0;
}

/******************************************************************************
 * WscUPnPDevStart
 *
 * Description: 
 *      Registers the UPnP device, and sends out advertisements.
 *
 * Parameters:
 *
 *   char *ipAddr 		 - ip address to initialize the Device Service.
 *   unsigned short port - port number to initialize the Device Service.
 *   char *descDoc  	 - name of description document.
 *                   		If NULL, use default description file name. 
 *   char *webRootDir 	 - path of web directory.
 *                   		If NULL, use default PATH.
 * Return:
 *    success - WSC_SYS_SUCCESS
 *    failed  - WSC_SYS_ERROR
 *****************************************************************************/
int WscUPnPDevStart(
	IN char *ipAddr,
	IN unsigned short port,
	IN char *descDoc,
	IN char *webRootDir)
{
	int ret, strLen = 0;
	char descDocUrl[WSC_UPNP_DESC_URL_LEN] = {0};
	char udnStr[WSC_UPNP_UUID_STR_LEN]={0};

	if (descDoc == NULL)
		descDoc = DEFAULT_DESC_FILE_NAME;

	if (webRootDir == NULL)
		webRootDir = DEFAULT_WEB_ROOT_DIR;

	memset(CfgMacStr, 0 , sizeof(CfgMacStr));
	snprintf(CfgMacStr, 18, "%02x:%02x:%02x:%02x:%02x:%02x", HostMacAddr[0], HostMacAddr[1], 
				HostMacAddr[2],HostMacAddr[3],HostMacAddr[4],HostMacAddr[5]);
//peter : 0523	
//	memset(&HostDescURL[0], 0, WSC_UPNP_DESC_URL_LEN);

#ifdef USE_XML_TEMPLATE
	memset(udnStr, 0, WSC_UPNP_UUID_STR_LEN);
	ret = wsc_get_oid(RT_OID_WSC_UUID, &udnStr[5], &strLen);
	if (ret == 0)
	{
		memcpy(&udnStr[0], "uuid:", 5);
		DBGPRINTF(RT_DBG_PKT, "UUID Str=%s!\n", udnStr);

		/* We replace the uuid string generated by init() of miniupnpd. */
		strncpy(wps_uuidvalue+5, &udnStr[5], strLen);
		ASSERT((*(wps_uuidvalue+5+strLen-1)) == '\0');
		DBGPRINTF(RT_DBG_PKT, "wps_uuidvalue[]=%s!\n", wps_uuidvalue);
	}
	else
	{
		DBGPRINTF(RT_DBG_ERROR,  "Get UUID string failed -- ret=%d\n", ret);
		goto failed;
	}
#if 0//peter : 0523
	memset(descDocUrl, 0, WSC_UPNP_DESC_URL_LEN);
	snprintf(descDocUrl, WSC_UPNP_DESC_URL_LEN, "http://%s:%d/WFADeviceDesc.xml", ipAddr, port);
	strcpy(&HostDescURL[0], &descDocUrl[0]);
	DBGPRINTF(RT_DBG_PKT, "Registering the RootDevice\n\t with descDocUrl: %s\n", descDocUrl);
#endif
#else
//peter : 0523
//	snprintf(descDocUrl, WSC_UPNP_DESC_URL_LEN, "http://%s:%d/%s", ipAddr, port, descDoc);
//	strcpy(&HostDescURL[0], &descDocUrl[0]);
#endif /* USE_XML_TEMPLATE */
//peter : 0523
//	DBGPRINTF(RT_DBG_PKT, "HostDescURL Prepared\n");

	memset(&wscLocalDevice, 0, sizeof(wscLocalDevice));
	WscLocalDevServTbInit();	
	DBGPRINTF(RT_DBG_PKT, "WscLocalDevServTbInit Initialized\n");

	//Init the ControlPoint List.
	WscCPList = NULL;

	return 0;

failed:
	
	return WSC_SYS_ERROR;
}

