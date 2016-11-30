/************************************************************************
 *
 *	Copyright (C) 2003 Trendchip Technologies, Corp.
 *	All Rights Reserved.
 *
 * Trendchip Confidential; Need to Know only.
 * Protected as an unpublished work.
 *
 * The computer program listings, specifications and documentation
 * herein are the property of Trendchip Technologies, Co. and shall
 * not be reproduced, copied, disclosed, or used in whole or in part
 * for any reason without the prior express written permission of
 * Trendchip Technologeis, Co.
 *
 *************************************************************************/
#include "Get_AssignedSSIDInfo_func.h"
#include <openssl/aes.h>
#include <openssl/md5.h>

int AES_ECB_PKCS5Padding
(char *LOID, char *source, char *dest, const int dstlen)
{
#define OFFOF(p, n) ((typeof(p)) ((void *) (p) + (n)))
	int text_len = 0, blk_num = 0, alg_len = 0;
	int pad = 0, i = 0, enc_len = 0, enc_buf_idx = 0;
	uint8_t *alg_s = NULL, *enc_s = NULL;
	MD5_CTX c;
	AES_KEY aes_key;
	unsigned char md5PWD[16] = {0};
	char aes_256key[36] = {0};

	/* MD5 LOID and format it as 256BIT. */
	MD5_Init(&c);
	MD5_Update(&c, LOID, strlen(LOID));
	MD5_Final(md5PWD, &c);
	snprintf(aes_256key, sizeof(aes_256key) - 1
		, "%02X%02X%02X%02X%02X%02X%02X%02X"
		  "%02X%02X%02X%02X%02X%02X%02X%02X"
		, md5PWD[0], md5PWD[1], md5PWD[2], md5PWD[3]
		, md5PWD[4], md5PWD[5], md5PWD[6], md5PWD[7]
		, md5PWD[8], md5PWD[9], md5PWD[10], md5PWD[11]
		, md5PWD[12], md5PWD[13], md5PWD[14], md5PWD[15]);

	/* AES/ECB/PKCS5Padding */
	AES_set_encrypt_key(aes_256key, 256, &aes_key);

	/* calc block num, should add a '\0' at last */
	text_len = strlen(source);
	blk_num = (text_len / AES_BLOCK_SIZE) + 1;

	/*
	 * copy text to align buffer
	 * and pad by PKCS5Padding
	 *
	 * pad by algin_len - text_len
	 * if text_len % AES_BLOCK_SIZE == 0, should add a 
	 * block at last and pad by 0x16
	 *
	 * if diff 1, pad { ... , ...., 0x01 }
	 * if diff 2, pad { ...., 0x02, 0x02 }
	 * if diff 3, pad { 0x03, 0x03, 0x03 }
	 */
	alg_len = blk_num * AES_BLOCK_SIZE;
	alg_s = (typeof(alg_s)) malloc(alg_len);
	if ( !alg_s )
		return -1;
	memcpy(alg_s, source, text_len); 

	pad = AES_BLOCK_SIZE - text_len % AES_BLOCK_SIZE;
	for (i = text_len; i < alg_len; i++)
		alg_s[i] = pad;

	/*
	 * encrypt every block
	 */
	enc_len = alg_len;
	enc_s = (typeof(enc_s)) malloc(enc_len);
	if ( !enc_s )
	{
		free(alg_s);
		return -1;
	}

	for (i = 0; i < blk_num; i++)
		AES_ecb_encrypt(OFFOF(alg_s, i * AES_BLOCK_SIZE)
		, OFFOF(enc_s, i * AES_BLOCK_SIZE)
		, &aes_key, AES_ENCRYPT);

	for (i = 0; i < enc_len; i++)
	{
		enc_buf_idx += snprintf(dest + enc_buf_idx
		, dstlen - 1
		,"%02X", enc_s[i]);
	}

	free(alg_s);
	free(enc_s);
}

json_t* Get_AssignedSSIDInfo(json_t* req_root,  json_t* req_para)
{
	json_t *cursor = NULL, *reply_root = NULL, *reply_para = NULL;
	char *cSSIDIndex = NULL, nodeName[48] = {0}, cur_ssid_name[33] = {0}, cur_authmode[32] = {0}, maxBitRate[5] = {0};
	char ssidEnable[10]= {0}, channel[10]= {0};
	unsigned int cur_ssid_idx = 0;
	int ret = -1;
	char loidname[256] = {0}, linkSta[32] = {0}, wlan_key[128] = {0};
	char key_idx[8] = {0}, key_attr[12] = {0}, aes_encrypt_val[512] = {0};

	if(!TCSUPPORT_WLAN_VAL){
		mobile_printf(MOBILE_INFO,"%s:wlan is not support!...\n",__FUNCTION__);
		return ParameterBuildJSON(req_root,req_para,CMD_UNRECOGNIZE);
	}

	mobile_printf(MOBILE_DEBUG, "%s, line = %d\n",__FUNCTION__,__LINE__);

	cSSIDIndex = ParameterSearchValue(req_para, "SSIDIndex");

	if(cSSIDIndex != NULL)
	{
		cur_ssid_idx = atoi(cSSIDIndex) - 1;
		if( (cur_ssid_idx > 7) || (cur_ssid_idx < 0) )
		{
			ret = -1;
		}
		else{
			ret = 0;

			sprintf(nodeName, "WLan_Entry%d", cur_ssid_idx);
			
			tcapi_get(nodeName, "SSID", cur_ssid_name);			
			tcapi_get(nodeName, "AuthMode", cur_authmode);
			if(strcmp(cur_authmode, "OPEN") == 0){
				strcpy(cur_authmode,"1");
			}
			if(strstr(cur_authmode, "WEP") != NULL){
				strcpy(cur_authmode,"2");
				tcapi_get(nodeName, "DefaultKeyID", key_idx);
				snprintf(key_attr, sizeof(key_attr) - 1,
					"Key%sStr", key_idx);
				tcapi_get(nodeName, key_attr, wlan_key);
			}
			else if(strcmp(cur_authmode, "WPAPSK") == 0){
				strcpy(cur_authmode,"3");
				tcapi_get(nodeName, "WPAPSK", wlan_key);
			}
			else if(strcmp(cur_authmode, "WPA2PSK") == 0){
				strcpy(cur_authmode,"4");
				tcapi_get(nodeName, "WPAPSK", wlan_key);
			}
			else if( strcmp(cur_authmode, "WPAPSKWPA2PSK") == 0 ) {
				strcpy(cur_authmode, "5");
				tcapi_get(nodeName, "WPAPSK", wlan_key);
			}
			tcapi_get(nodeName, "EnableSSID", ssidEnable);			
			tcapi_get("WLan_Common", "Channel", channel);			
			tcapi_get("WLan_Common", "TxPowerLevel", maxBitRate);
			if(strcmp(maxBitRate, "1") == 0){
				strcpy(maxBitRate,"100");
			}
			if(strstr(maxBitRate, "2") != NULL){
				strcpy(maxBitRate,"80");
			}
			else if(strcmp(maxBitRate, "3") == 0){
				strcpy(maxBitRate,"60");
			}
			else if(strcmp(maxBitRate, "4") == 0){
				strcpy(maxBitRate,"30");
			}
			else if ( strcmp(maxBitRate, "5") == 0 ) {
				strcpy(maxBitRate, "20");
			}

			/* cal aes password */
			tcapi_get("XPON_LinkCfg", "LinkSta", linkSta);
			if ( 0 == strcmp(linkSta,"2") ) /* EPON*/
				tcapi_get("EPON_LOIDAuth", "LOID0", loidname);
			else
				tcapi_get("GPON_LOIDAuth", "LOID", loidname);
			if ( 0 != loidname[0]
				&& 0 != wlan_key[0] )
			{
				bzero(aes_encrypt_val, sizeof(aes_encrypt_val));
				AES_ECB_PKCS5Padding(loidname, wlan_key
					, aes_encrypt_val, sizeof(aes_encrypt_val));
			}
		}	
	}
	else
	{
		ret = -1;
	}

	//build parameter
	if(ret == -1){
		if(!(reply_para = ParameterBuildJSONReplyPara(req_para, 1))){
			mobile_printf(MOBILE_INFO,"%s:ParameterBuildJSONReplyPara Err!...\n",__FUNCTION__);
			goto err;
		}
		if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_FAIL_REASON, "The Assigned WLan Is Not Exist")){
			mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
			goto err;
		}
	}
	else{
		if(!(reply_para = ParameterBuildJSONReplyPara(req_para, 0))){//ok
			mobile_printf(MOBILE_INFO,"%s:ParameterBuildJSONReplyPara Err!...\n",__FUNCTION__);
			goto err;
		}
		if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_SSID , cur_ssid_name)){
			mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
			goto err;
		}
		if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_PWD , aes_encrypt_val)){
			mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
			goto err;
		}
		if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_ENCRYPT , cur_authmode)){
			mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
			goto err;
		}
		if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_POWERLEVEL , maxBitRate)){
			mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
			goto err;
		}
		if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_CHANNEL , channel)){
			mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
			goto err;
		}
		if(JSON_OK != ParameterAddNewJSONChild(reply_para, PARA_ENABLE , ssidEnable)){
			mobile_printf(MOBILE_INFO,"%s:ParameterAddNewJSONChild Err!...\n",__FUNCTION__);
			goto err;
		}
	}
	//build head
	if(!(reply_root = ParameterBuildJSONReplyHead(req_root,PARA_RESULT_SUCCESS))){
		mobile_printf(MOBILE_INFO,"%s:ParameterBuildJSONReplyHead Err!...\n",__FUNCTION__);
		goto err;
	}
	if(ParameterBuildJSONAddPara2Head(reply_root, reply_para)){
		mobile_printf(MOBILE_INFO,"%s:ParameterBuildJSONAddPara2Head Err!...\n",__FUNCTION__);
		goto err;
	}	
		
#if PARAMETER_REPLAY_DBG
	char* value_str = NULL; 
	json_tree_to_string(reply_root,&value_str);
	mobile_printf(MOBILE_INFO, "%s: reply json pkt=%s!...\n", __FUNCTION__,value_str);
	free(value_str);	
#endif	

	return reply_root;
	
err:
	if(reply_root) ParameterFreeJSON(&reply_root);
	if(reply_para) ParameterFreeJSON(&reply_para);
	return NULL;
}

