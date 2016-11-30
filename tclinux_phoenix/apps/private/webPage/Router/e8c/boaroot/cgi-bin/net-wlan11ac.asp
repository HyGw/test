<%
If Request_Form("wlanWEPFlag")="1" Then
	tcWebApi_set("WebCurSet_Entry","wlan_ac_id","SSID_INDEX")
	tcWebApi_set("WLan11ac_Common","BssidNum","BssidNum")
	TCWebApi_set("WLan11ac_Common","Country","Countries_Channels")
	TCWebApi_set("WLan11ac_Common","APOn","wlan_APenable")
	TCWebApi_set("WLan11ac_Common","Channel","Channel_ID")
	TCWebApi_set("WLan11ac_Common","BeaconPeriod","BeaconInterval")
  TCWebApi_set("WLan11ac_Common","RTSThreshold","RTSThreshold")
	TCWebApi_set("WLan11ac_Common","FragThreshold","FragmentThreshold")
	TCWebApi_set("WLan11ac_Common","DtimPeriod","DTIM")
	TCWebApi_set("WLan11ac_Common","WirelessMode","WirelessMode")
	If Request_Form("Is11nMode") = "1" then
		TCWebApi_set("WLan11ac_Common","HT_BW","WLANChannelBandwidth")
		If Request_Form("WLANChannelBandwidth")="1" then
			TCWebApi_set("WLan_Common","HT_EXTCHA","ExtChannFlag")
			If Request_Form("Is11acMode") = "1"  then
					TCWebApi_set("WLan11ac_Common","VHT_BW","WLan11acVHTChannelBandwidth")
					TCWebApi_set("WLan11ac_Common","VHT_SGI","WLan11acVHTGuardInterval")
					if Request_Form("RTDEVICE") = "7615"  then
						TCWebApi_set("WLan11ac_Common","VHT_Sec80_Channel","VHTSec80Channel")
						TCWebApi_set("WLan11ac_Common","ITxBfEn","WLan11acITxBfEn")
						TCWebApi_set("WLan11ac_Common","ETxBfEnCond","WLan11acETxBfEnCond")
						TCWebApi_set("WLan11ac_Common","ETxBfIncapable","WLan11acETxBfIncapable")
					end if	
			else	
					TCWebApi_set("WLan11ac_Common","VHT_BW","VHTBW")
			End if	
			If tcWebApi_get("WebCustom_Entry","isHTBW40M","h") = "Yes" then
				tcWebApi_set("WLan11ac_Common","HT_BSSCoexistence","Wlan_HTBW40M")
			End if
		End if
		TCWebApi_set("WLan11ac_Common","HT_GI","WLANGuardInterval")
		TCWebApi_set("WLan11ac_Entry","HT_MCS","WLANMCS")
	else
		TCWebApi_set("WLan11ac_Common","HT_BW","HTBW")
		TCWebApi_set("WLan11ac_Entry","HT_MCS","HTMCSAUTO")
		TCWebApi_set("WLan11ac_Entry","WMM","WMM_Selection")
		TCWebApi_set("WLan11ac_Common","VHT_BW","VHTBW")
	end IF
	If Request_Form("StationNum") <> "" then
		TCWebApi_set("WLan11ac_Entry","MaxStaNum","StationNum")
	End if
	TCWebApi_set("WLan11ac_Common","11nMode","Is11nMode")
	TCWebApi_set("WLan11ac_Common","11acMode","Is11acMode")
	
	TCWebApi_set("WLan11ac_Common","BasicRate","BasicRate_Value1")
	
	TCWebApi_set("WLan11ac_Entry","SSID","ESSID")
	TCWebApi_set("WLan11ac_Common","BeaconPeriod","BeaconInterval")
	TCWebApi_set("WLan11ac_Entry","HideSSID","ESSID_HIDE_Selection")
	If Request_Form("isPerSSIDSupport") = "1" then
	TCWebApi_set("WLan11ac_Entry","EnableSSID","ESSID_Enable_Selection")
	end if
	if Request_form("WLAN_FltActive") = "0" then
		TCWebApi_set("WLan11ac_Entry","AccessPolicy","WLAN_FltActive")
	else
		TCWebApi_set("WLan11ac_Entry","AccessPolicy","WLAN_FltAction")
	end if
	if Request_Form("isWPSSupported") = "1" then
	if Request_Form("SSID_INDEX") = "0" then
		if Request_Form("UseWPS_Selection") = "1" then
			TCWebApi_set("WLan11ac_Entry","WPSConfMode","WpsConfModeAll")
		else
			TCWebApi_set("WLan11ac_Entry","WPSConfMode","WpsConfModeNone")
		end if
		if Request_form("UseWPS_Selection") = "1" then
			TCWebApi_set("WLan11ac_Entry","WPSMode","WPSMode_Selection")
			if Request_form("WPSMode_Selection") = "0" then
				TCWebApi_set("WLan11ac_Entry","enrolleePinCode","WPSEnrolleePINCode")
			end if
		end if
	end if
	end if
	TCWebApi_set("WLan11ac_Entry","WLan_MAC0","WLANFLT_MAC1")	
	TCWebApi_set("WLan11ac_Entry","WLan_MAC1","WLANFLT_MAC2")	
	TCWebApi_set("WLan11ac_Entry","WLan_MAC2","WLANFLT_MAC3")		
	TCWebApi_set("WLan11ac_Entry","WLan_MAC3","WLANFLT_MAC4")		
	TCWebApi_set("WLan11ac_Entry","WLan_MAC4","WLANFLT_MAC5")	
	TCWebApi_set("WLan11ac_Entry","WLan_MAC5","WLANFLT_MAC6")	
	TCWebApi_set("WLan11ac_Entry","WLan_MAC6","WLANFLT_MAC7")		
	TCWebApi_set("WLan11ac_Entry","WLan_MAC7","WLANFLT_MAC8")

	TCWebApi_set("WLan11ac_Entry","AuthMode","WEP_Selection")
	TCWebApi_save()
end if

If Request_Form("CountryChange")="1" Then
	tcWebApi_set("WebCurSet_Entry","wlan_ac_id","SSID_INDEX")
	TCWebApi_set("WLan11ac_Common","Country","Countries_Channels")
	if Request_Form("Countries_Channels") = "ARGENTINA" then
		TCWebApi_set("WLan11ac_Common","CountryRegionABand","CountryRegionABand3")
	elseif Request_Form("Countries_Channels") = "ARMENIA" then	
		TCWebApi_set("WLan11ac_Common","CountryRegionABand","CountryRegionABand2")
	elseif Request_Form("Countries_Channels") = "AUSTRIA" then
		TCWebApi_set("WLan11ac_Common","CountryRegionABand","CountryRegionABand1")
	elseif Request_Form("Countries_Channels") = "AZERBAIJAN" then
		TCWebApi_set("WLan11ac_Common","CountryRegionABand","CountryRegionABand2")
	elseif Request_Form("Countries_Channels") = "BELGIUM" then
		TCWebApi_set("WLan11ac_Common","CountryRegionABand","CountryRegionABand1")
	elseif Request_Form("Countries_Channels") = "BELIZE" then 
		TCWebApi_set("WLan11ac_Common","CountryRegionABand","CountryRegionABand4")
	elseif Request_Form("Countries_Channels") = "BOLIVIA" then
		TCWebApi_set("WLan11ac_Common","CountryRegionABand","CountryRegionABand4")
	elseif Request_Form("Countries_Channels") = "BRAZIL" then
		TCWebApi_set("WLan11ac_Common","CountryRegionABand","CountryRegionABand1")
	elseif Request_Form("Countries_Channels") = "BRUNEI DARUSSALAM" then
		TCWebApi_set("WLan11ac_Common","CountryRegionABand","CountryRegionABand4")
	elseif Request_Form("Countries_Channels") = "BULGARIA" then
		TCWebApi_set("WLan11ac_Common","CountryRegionABand","CountryRegionABand1")
	elseif Request_Form("Countries_Channels") = "CHINA" then			             
		TCWebApi_set("WLan11ac_Common","CountryRegionABand","CountryRegionABand4")
	elseif Request_Form("Countries_Channels") = "CROATIA" then			             
		TCWebApi_set("WLan11ac_Common","CountryRegionABand","CountryRegionABand2")			
	elseif Request_Form("Countries_Channels") = "CYPRUS" then			             
		TCWebApi_set("WLan11ac_Common","CountryRegionABand","CountryRegionABand1")			
	elseif Request_Form("Countries_Channels") = "CZECH REPUBLIC" then
		TCWebApi_set("WLan11ac_Common","CountryRegionABand","CountryRegionABand2")
	elseif Request_Form("Countries_Channels") = "DENMARK" then			             
		TCWebApi_set("WLan11ac_Common","CountryRegionABand","CountryRegionABand1")
	elseif Request_Form("Countries_Channels") = "EGYPT" then			             
		TCWebApi_set("WLan11ac_Common","CountryRegionABand","CountryRegionABand2")			
	elseif Request_Form("Countries_Channels") = "ESTONIA" then			             
		TCWebApi_set("WLan11ac_Common","CountryRegionABand","CountryRegionABand1")			
	elseif Request_Form("Countries_Channels") = "FINLAND" then
		TCWebApi_set("WLan11ac_Common","CountryRegionABand","CountryRegionABand1")
	elseif Request_Form("Countries_Channels") = "FRANCE" then			             
		TCWebApi_set("WLan11ac_Common","CountryRegionABand","CountryRegionABand2")	
	elseif Request_Form("Countries_Channels") = "GEORGIA" then			             
		TCWebApi_set("WLan11ac_Common","CountryRegionABand","CountryRegionABand2")			
	elseif Request_Form("Countries_Channels") = "GERMANY" then			             
		TCWebApi_set("WLan11ac_Common","CountryRegionABand","CountryRegionABand1")			
	elseif Request_Form("Countries_Channels") = "GREECE" then
		TCWebApi_set("WLan11ac_Common","CountryRegionABand","CountryRegionABand1")
	elseif Request_Form("Countries_Channels") = "HUNGARY" then			             
		TCWebApi_set("WLan11ac_Common","CountryRegionABand","CountryRegionABand1")
	elseif Request_Form("Countries_Channels") = "ICELAND" then			             
		TCWebApi_set("WLan11ac_Common","CountryRegionABand","CountryRegionABand1")			
	elseif Request_Form("Countries_Channels") = "INDONESIA" then			             
		TCWebApi_set("WLan11ac_Common","CountryRegionABand","CountryRegionABand4")			
	elseif Request_Form("Countries_Channels") = "IRAN" then
		TCWebApi_set("WLan11ac_Common","CountryRegionABand","CountryRegionABand4")
		elseif Request_Form("Countries_Channels") = "IRELAND" then			             
		TCWebApi_set("WLan11ac_Common","CountryRegionABand","CountryRegionABand1")
	elseif Request_Form("Countries_Channels") = "ITALY" then			             
		TCWebApi_set("WLan11ac_Common","CountryRegionABand","CountryRegionABand1")			
	elseif Request_Form("Countries_Channels") = "JAPAN" then			             
		TCWebApi_set("WLan11ac_Common","CountryRegionABand","CountryRegionABand9")
	elseif Request_Form("Countries_Channels") = "KOREA DEMOCRATIC" then
		TCWebApi_set("WLan11ac_Common","CountryRegionABand","CountryRegionABand5")
		elseif Request_Form("Countries_Channels") = "KOREA REPUBLIC" then			             
		TCWebApi_set("WLan11ac_Common","CountryRegionABand","CountryRegionABand5")
	elseif Request_Form("Countries_Channels") = "LATVIA" then			             
		TCWebApi_set("WLan11ac_Common","CountryRegionABand","CountryRegionABand1")			
	elseif Request_Form("Countries_Channels") = "LITHUANIA" then			             
		TCWebApi_set("WLan11ac_Common","CountryRegionABand","CountryRegionABand1")			
	elseif Request_Form("Countries_Channels") = "LUXEMBOURG" then
		TCWebApi_set("WLan11ac_Common","CountryRegionABand","CountryRegionABand1")
		elseif Request_Form("Countries_Channels") = "MONACO" then			             
		TCWebApi_set("WLan11ac_Common","CountryRegionABand","CountryRegionABand2")
	elseif Request_Form("Countries_Channels") = "NETHERLANDS" then			             
		TCWebApi_set("WLan11ac_Common","CountryRegionABand","CountryRegionABand1")			
	elseif Request_Form("Countries_Channels") = "PERU" then			             
		TCWebApi_set("WLan11ac_Common","CountryRegionABand","CountryRegionABand4")			
	elseif Request_Form("Countries_Channels") = "PHILIPPINES" then
		TCWebApi_set("WLan11ac_Common","CountryRegionABand","CountryRegionABand4")
	elseif Request_Form("Countries_Channels") = "POLAND" then			             
		TCWebApi_set("WLan11ac_Common","CountryRegionABand","CountryRegionABand1")			
	elseif Request_Form("Countries_Channels") = "PORTUGAL" then
		TCWebApi_set("WLan11ac_Common","CountryRegionABand","CountryRegionABand1")
	elseif Request_Form("Countries_Channels") = "SLOVAKIA" then
		TCWebApi_set("WLan11ac_Common","CountryRegionABand","CountryRegionABand1")
		elseif Request_Form("Countries_Channels") = "SLOVENIA" then			             
		TCWebApi_set("WLan11ac_Common","CountryRegionABand","CountryRegionABand1")
	elseif Request_Form("Countries_Channels") = "SOUTH AFRICA" then			             
		TCWebApi_set("WLan11ac_Common","CountryRegionABand","CountryRegionABand1")			
	elseif Request_Form("Countries_Channels") = "SPAIN" then             
		TCWebApi_set("WLan11ac_Common","CountryRegionABand","CountryRegionABand1")			
	elseif Request_Form("Countries_Channels") = "SWEDEN" then
		TCWebApi_set("WLan11ac_Common","CountryRegionABand","CountryRegionABand1")
	elseif Request_Form("Countries_Channels") = "SWITZERLAND" then
		TCWebApi_set("WLan11ac_Common","CountryRegionABand","CountryRegionABand1")
	elseif Request_Form("Countries_Channels") = "TAIWAN" then
		TCWebApi_set("WLan11ac_Common","CountryRegionABand","CountryRegionABand3")
	elseif Request_Form("Countries_Channels") = "TRINIDAD AND TOBAGO" then
		TCWebApi_set("WLan11ac_Common","CountryRegionABand","CountryRegionABand2")
	elseif Request_Form("Countries_Channels") = "TUNISIA" then 
		TCWebApi_set("WLan11ac_Common","CountryRegionABand","CountryRegionABand2")
	elseif Request_Form("Countries_Channels") = "TURKEY" then  
		TCWebApi_set("WLan11ac_Common","CountryRegionABand","CountryRegionABand2")
	elseif Request_Form("Countries_Channels") = "UNITED KINGDOM" then
		TCWebApi_set("WLan11ac_Common","CountryRegionABand","CountryRegionABand1")	
	elseif Request_Form("Countries_Channels") = "URUGUAY" then
		TCWebApi_set("WLan11ac_Common","CountryRegionABand","CountryRegionABand5")
	elseif Request_Form("Countries_Channels") = "UZBEKISTAN" then
		TCWebApi_set("WLan11ac_Common","CountryRegionABand","CountryRegionABand1")
	elseif Request_Form("Countries_Channels") = "VENEZUELA" then
		TCWebApi_set("WLan11ac_Common","CountryRegionABand","CountryRegionABand5")
	else
		TCWebApi_set("WLan11ac_Common","CountryRegionABand","CountryRegionABand0")
	end if
	TCWebApi_save()
End If
IF Request_Form("wlanWEPFlag")="2" Then
	tcWebApi_set("WebCurSet_Entry","wlan_ac_id","SSID_INDEX")
	TCWebApi_save()
End if
If Request_Form("wlanWEPFlag")="3" Then
	TCWebApi_set("WLan11ac_Entry","AuthMode","WEP_Selection")	
	tcWebApi_set("WebCurSet_Entry","wlan_ac_id","SSID_INDEX")
	tcWebApi_set("WLan11ac_Common","BssidNum","BssidNum")
	TCWebApi_set("WLan11ac_Common","Country","Countries_Channels")
	TCWebApi_set("WLan11ac_Common","Channel","Channel_ID")
	TCWebApi_set("WLan11ac_Common","APOn","wlan_APenable")
	TCWebApi_set("WLan11ac_Common","BeaconPeriod","BeaconInterval")
  TCWebApi_set("WLan11ac_Common","RTSThreshold","RTSThreshold")
	TCWebApi_set("WLan11ac_Common","FragThreshold","FragmentThreshold")
	TCWebApi_set("WLan11ac_Common","DtimPeriod","DTIM")
	TCWebApi_set("WLan11ac_Common","WirelessMode","WirelessMode")
	If Request_Form("Is11nMode") = "1" then
		TCWebApi_set("WLan11ac_Common","HT_BW","WLANChannelBandwidth")
		if Request_Form("WLANChannelBandwidth")="1" then
			TCWebApi_set("WLan_Common","HT_EXTCHA","ExtChannFlag")
			If Request_Form("Is11acMode") = "1"  then
					TCWebApi_set("WLan11ac_Common","VHT_BW","WLan11acVHTChannelBandwidth")
					TCWebApi_set("WLan11ac_Common","VHT_SGI","WLan11acVHTGuardInterval")
					if Request_Form("RTDEVICE") = "7615"  then
						TCWebApi_set("WLan11ac_Common","VHT_Sec80_Channel","VHTSec80Channel")
						TCWebApi_set("WLan11ac_Common","ITxBfEn","WLan11acITxBfEn")
						TCWebApi_set("WLan11ac_Common","ETxBfEnCond","WLan11acETxBfEnCond")
						TCWebApi_set("WLan11ac_Common","ETxBfIncapable","WLan11acETxBfIncapable")
					end if	
			else	
					TCWebApi_set("WLan11ac_Common","VHT_BW","VHTBW")
			End if
			if tcWebApi_get("WebCustom_Entry","isHTBW40M","h") = "Yes" then
				tcWebApi_set("WLan11ac_Common","HT_BSSCoexistence","Wlan_HTBW40M")
			end if
		end if
		TCWebApi_set("WLan11ac_Common","HT_GI","WLANGuardInterval")
		TCWebApi_set("WLan11ac_Entry","HT_MCS","WLANMCS")
	else
		TCWebApi_set("WLan11ac_Common","HT_BW","HTBW")
		TCWebApi_set("WLan11ac_Entry","HT_MCS","HTMCSAUTO")
		TCWebApi_set("WLan11ac_Entry","WMM","WMM_Selection")
		TCWebApi_set("WLan11ac_Common","VHT_BW","VHTBW")

	end IF
	If Request_Form("StationNum") <> "" then
		TCWebApi_set("WLan11ac_Entry","MaxStaNum","StationNum")
	End if
	TCWebApi_set("WLan11ac_Common","11nMode","Is11nMode")	
	TCWebApi_set("WLan11ac_Common","11acMode","Is11acMode")
	TCWebApi_set("WLan11ac_Entry","SSID","ESSID")
	TCWebApi_set("WLan11ac_Common","BeaconPeriod","BeaconInterval")	
	TCWebApi_set("WLan11ac_Entry","HideSSID","ESSID_HIDE_Selection")
	If Request_Form("isPerSSIDSupport") = "1" then
	TCWebApi_set("WLan11ac_Entry","EnableSSID","ESSID_Enable_Selection")
	end if

	if Request_Form("isDot1XSupported")  = "1" then
		if Request_Form("WEP_Selection")  = "Radius-WEP64" then
			TCWebApi_set("WLan11ac_Common","session_timeout_interval","radiusSVR_ReAuthInter1")
			TCWebApi_set("WLan11ac_Entry","RADIUS_Server","radiusSVR_IP1")
			TCWebApi_set("WLan11ac_Entry","RADIUS_Port","radiusSVR_Port1")
			TCWebApi_set("WLan11ac_Entry","RADIUS_Key","radiusSVR_Key1")
			if Request_Form("isDot1XEnhanceSupported") = "1" then
				TCWebApi_set("WLan11ac_Entry","BAK_RADIUS_Server","bakradiusSVR_IP1")
				TCWebApi_set("WLan11ac_Entry","BAK_RADIUS_Port","bakradiusSVR_Port1")
				TCWebApi_set("WLan11ac_Entry","BAK_RADIUS_Key","bakradiusSVR_Key1")
			end if
		elseif Request_Form("WEP_Selection")  = "Radius-WEP128"  then
			TCWebApi_set("WLan11ac_Common","session_timeout_interval","radiusSVR_ReAuthInter2")
			TCWebApi_set("WLan11ac_Entry","RADIUS_Server","radiusSVR_IP2")
			TCWebApi_set("WLan11ac_Entry","RADIUS_Port","radiusSVR_Port2")
			TCWebApi_set("WLan11ac_Entry","RADIUS_Key","radiusSVR_Key2")
			if Request_Form("isDot1XEnhanceSupported") = "1" then
				TCWebApi_set("WLan11ac_Entry","BAK_RADIUS_Server","bakradiusSVR_IP2")
				TCWebApi_set("WLan11ac_Entry","BAK_RADIUS_Port","bakradiusSVR_Port2")
				TCWebApi_set("WLan11ac_Entry","BAK_RADIUS_Key","bakradiusSVR_Key2")
			end if
		elseif Request_Form("WEP_Selection")  = "WPA"  then
			TCWebApi_set("WLan11ac_Common","session_timeout_interval","radiusSVR_ReAuthInter3")
			TCWebApi_set("WLan11ac_Entry","RekeyInterval","WPARekeyInter3")			
			TCWebApi_set("WLan11ac_Entry","RADIUS_Server","radiusSVR_IP3")
			TCWebApi_set("WLan11ac_Entry","RADIUS_Port","radiusSVR_Port3")
			TCWebApi_set("WLan11ac_Entry","RADIUS_Key","radiusSVR_Key3")
			if Request_Form("isDot1XEnhanceSupported") = "1" then
				TCWebApi_set("WLan11ac_Entry","BAK_RADIUS_Server","bakradiusSVR_IP3")
				TCWebApi_set("WLan11ac_Entry","BAK_RADIUS_Port","bakradiusSVR_Port3")
				TCWebApi_set("WLan11ac_Entry","BAK_RADIUS_Key","bakradiusSVR_Key3")
			end if
		elseif Request_Form("WEP_Selection")  = "WPA2"  then
			TCWebApi_set("WLan11ac_Common","session_timeout_interval","radiusSVR_ReAuthInter4")
			TCWebApi_set("WLan11ac_Entry","RekeyInterval","WPARekeyInter4")
			TCWebApi_set("WLan11ac_Entry","RADIUS_Server","radiusSVR_IP4")
			TCWebApi_set("WLan11ac_Entry","RADIUS_Port","radiusSVR_Port4")
			TCWebApi_set("WLan11ac_Entry","RADIUS_Key","radiusSVR_Key4")
			if Request_Form("isDot1XEnhanceSupported") = "1" then
				TCWebApi_set("WLan11ac_Entry","BAK_RADIUS_Server","bakradiusSVR_IP4")				
				TCWebApi_set("WLan11ac_Entry","BAK_RADIUS_Port","bakradiusSVR_Port4")
				TCWebApi_set("WLan11ac_Entry","BAK_RADIUS_Key","bakradiusSVR_Key4")
			end if
		elseif Request_Form("WEP_Selection")  = "WPA1WPA2"  then
			TCWebApi_set("WLan11ac_Common","session_timeout_interval","radiusSVR_ReAuthInter5")
			TCWebApi_set("WLan11ac_Entry","RekeyInterval","WPARekeyInter5")
			TCWebApi_set("WLan11ac_Entry","RADIUS_Server","radiusSVR_IP5")
			TCWebApi_set("WLan11ac_Entry","RADIUS_Port","radiusSVR_Port5")
			TCWebApi_set("WLan11ac_Entry","RADIUS_Key","radiusSVR_Key5")
			if Request_Form("isDot1XEnhanceSupported") = "1" then
				TCWebApi_set("WLan11ac_Entry","BAK_RADIUS_Server","bakradiusSVR_IP5")
				TCWebApi_set("WLan11ac_Entry","BAK_RADIUS_Port","bakradiusSVR_Port5")
				TCWebApi_set("WLan11ac_Entry","BAK_RADIUS_Key","bakradiusSVR_Key5")
			end if
		end if	
	end if	
	if Request_form("WLAN_FltActive") = "0" then
		TCWebApi_set("WLan11ac_Entry","AccessPolicy","WLAN_FltActive")
	else
		TCWebApi_set("WLan11ac_Entry","AccessPolicy","WLAN_FltAction")
	end if
	if Request_Form("isWPSSupported") = "1" then	
	if Request_Form("SSID_INDEX") = "0" then
		if Request_Form("UseWPS_Selection") = "1" then
			TCWebApi_set("WLan11ac_Entry","WPSConfMode","WpsConfModeAll")
		else
			TCWebApi_set("WLan11ac_Entry","WPSConfMode","WpsConfModeNone")
		end if
		if Request_form("UseWPS_Selection") = "1" then
			TCWebApi_set("WLan11ac_Entry","WPSMode","WPSMode_Selection")
			if Request_form("WPSMode_Selection") = "0" then
				TCWebApi_set("WLan11ac_Entry","enrolleePinCode","WPSEnrolleePINCode")
			end if
		end if
	end if	
	end if		
	if Request_Form("isWDSSupported") = "1" then
		TCWebApi_set("WLan11ac_WDS","WdsEnable","WLAN_WDS_Active")
		if Request_form("WLAN_WDS_Active") = "1" then
			if Request_Form("WEP_Selection") = "OPEN" then
				TCWebApi_set("WLan11ac_WDS","WdsEncrypType","WDS_EncrypType_NONE")
			elseif Request_Form("WEP_Selection") = "WEP-64Bits"  then
				TCWebApi_set("WLan11ac_WDS","WdsEncrypType","WDS_EncrypType_WEP")
			elseif Request_Form("WEP_Selection") = "WEP-128Bits" then
				TCWebApi_set("WLan11ac_WDS","WdsEncrypType","WDS_EncrypType_WEP")
			elseif Request_Form("WEP_Selection") = "Radius-WEP64" then
				TCWebApi_set("WLan11ac_WDS","WdsEncrypType","WDS_EncrypType_WEP")
			elseif Request_Form("WEP_Selection") = "Radius-WEP128" then
				TCWebApi_set("WLan11ac_WDS","WdsEncrypType","WDS_EncrypType_WEP")
			elseif Request_Form("WEP_Selection") = "WPAPSK" then
				TCWebApi_set("WLan11ac_WDS","WdsEncrypType","WDS_EncrypType_Selection")
				TCWebApi_set("WLan11ac_WDS","WdsKey","WDS_Key")
			elseif Request_Form("WEP_Selection") = "WPA2PSK" then
				TCWebApi_set("WLan11ac_WDS","WdsEncrypType","WDS_EncrypType_Selection")
				TCWebApi_set("WLan11ac_WDS","WdsKey","WDS_Key")
			elseif Request_Form("WEP_Selection") = "WPAPSKWPA2PSK"  then
				TCWebApi_set("WLan11ac_WDS","WdsEncrypType","WDS_EncrypType_Selection")
				TCWebApi_set("WLan11ac_WDS","WdsKey","WDS_Key")
			elseif Request_Form("WEP_Selection") = "WPA"  then
				TCWebApi_set("WLan11ac_WDS","WdsEncrypType","WDS_EncrypType_Selection")
				TCWebApi_set("WLan11ac_WDS","WdsKey","WDS_Key")
			elseif Request_Form("WEP_Selection") = "WPA2"  then
				TCWebApi_set("WLan11ac_WDS","WdsEncrypType","WDS_EncrypType_Selection")
				TCWebApi_set("WLan11ac_WDS","WdsKey","WDS_Key")
			elseif Request_Form("WEP_Selection") = "WPA1WPA2"  then
				TCWebApi_set("WLan11ac_WDS","WdsEncrypType","WDS_EncrypType_Selection")
				TCWebApi_set("WLan11ac_WDS","WdsKey","WDS_Key")
			end if
				
			TCWebApi_set("WLan11ac_WDS","Wds_MAC0","WLANWDS_PEER_MAC1")
			TCWebApi_set("WLan11ac_WDS","Wds_MAC1","WLANWDS_PEER_MAC2")
			TCWebApi_set("WLan11ac_WDS","Wds_MAC2","WLANWDS_PEER_MAC3")
			TCWebApi_set("WLan11ac_WDS","Wds_MAC3","WLANWDS_PEER_MAC4")
		end if
	end if			
	TCWebApi_set("WLan11ac_Entry","WLan_MAC0","WLANFLT_MAC1")	
	TCWebApi_set("WLan11ac_Entry","WLan_MAC1","WLANFLT_MAC2")	
	TCWebApi_set("WLan11ac_Entry","WLan_MAC2","WLANFLT_MAC3")		
	TCWebApi_set("WLan11ac_Entry","WLan_MAC3","WLANFLT_MAC4")		
	TCWebApi_set("WLan11ac_Entry","WLan_MAC4","WLANFLT_MAC5")	
	TCWebApi_set("WLan11ac_Entry","WLan_MAC5","WLANFLT_MAC6")	
	TCWebApi_set("WLan11ac_Entry","WLan_MAC6","WLANFLT_MAC7")		
	TCWebApi_set("WLan11ac_Entry","WLan_MAC7","WLANFLT_MAC8")	
	if Request_Form("WEP_Selection") = "WEP-64Bits"  then
		TCWebApi_set("WLan11ac_Entry","WEPAuthType","WEP_TypeSelection1")
		TCWebApi_set("WLan11ac_Entry","DefaultKeyID","DefWEPKey3")
		TCWebApi_set("WLan11ac_Entry","Key1Str","WEP_Key13")	
		TCWebApi_set("WLan11ac_Entry","Key2Str","WEP_Key23")		
		TCWebApi_set("WLan11ac_Entry","Key3Str","WEP_Key33")		
		TCWebApi_set("WLan11ac_Entry","Key4Str","WEP_Key43")
	elseif Request_Form("WEP_Selection")  = "WEP-128Bits" then
		TCWebApi_set("WLan11ac_Entry","WEPAuthType","WEP_TypeSelection2")
		TCWebApi_set("WLan11ac_Entry","DefaultKeyID","DefWEPKey4")
		TCWebApi_set("WLan11ac_Entry","Key1Str","WEP_Key14")	
		TCWebApi_set("WLan11ac_Entry","Key2Str","WEP_Key24")		
		TCWebApi_set("WLan11ac_Entry","Key3Str","WEP_Key34")		
		TCWebApi_set("WLan11ac_Entry","Key4Str","WEP_Key44")
	elseif Request_Form("WEP_Selection")  = "Radius-WEP64" then
		TCWebApi_set("WLan11ac_Entry","DefaultKeyID","DefWEPKey1")
		TCWebApi_set("WLan11ac_Entry","Key1Str","WEP_Key11")	
		TCWebApi_set("WLan11ac_Entry","Key2Str","WEP_Key21")		
		TCWebApi_set("WLan11ac_Entry","Key3Str","WEP_Key31")		
		TCWebApi_set("WLan11ac_Entry","Key4Str","WEP_Key41")				
	elseif Request_Form("WEP_Selection")  = "Radius-WEP128" then
		TCWebApi_set("WLan11ac_Entry","DefaultKeyID","DefWEPKey2")
		TCWebApi_set("WLan11ac_Entry","Key1Str","WEP_Key12")	
		TCWebApi_set("WLan11ac_Entry","Key2Str","WEP_Key22")		
		TCWebApi_set("WLan11ac_Entry","Key3Str","WEP_Key32")		
		TCWebApi_set("WLan11ac_Entry","Key4Str","WEP_Key42")
	elseif Request_Form("WEP_Selection")  = "WPAPSK" then
		TCWebApi_set("WLan11ac_Entry","WPAPSK","PreSharedKey2")
		TCWebApi_set("WLan11ac_Entry","EncrypType","TKIP_Selection5")
		TCWebApi_set("WLan11ac_Entry","RekeyInterval","keyRenewalInterval2")
	elseif Request_Form("WEP_Selection")  = "WPA2PSK" then
		TCWebApi_set("WLan11ac_Entry","WPAPSK","PreSharedKey1")
		TCWebApi_set("WLan11ac_Entry","EncrypType","TKIP_Selection4")
		TCWebApi_set("WLan11ac_Entry","RekeyInterval","keyRenewalInterval1")
	elseif Request_Form("WEP_Selection")  = "WPAPSKWPA2PSK"  then
		TCWebApi_set("WLan11ac_Entry","WPAPSK","PreSharedKey3")
		TCWebApi_set("WLan11ac_Entry","EncrypType","TKIP_Selection6")
		TCWebApi_set("WLan11ac_Entry","RekeyInterval","keyRenewalInterval3")
	elseif Request_Form("WEP_Selection")  = "WPA"  then
		TCWebApi_set("WLan11ac_Entry","EncrypType","TKIP_Selection1")
	elseif Request_Form("WEP_Selection")  = "WPA2"  then
		TCWebApi_set("WLan11ac_Entry","EncrypType","TKIP_Selection2")
	elseif Request_Form("WEP_Selection")  = "WPA1WPA2"  then
		TCWebApi_set("WLan11ac_Entry","EncrypType","TKIP_Selection3")
	end if
	
	if Request_Form("isWPSSupported") = "1" then
	TCWebApi_set("WLan11ac_Entry","WPSConfStatus","WPSConfigured")
	end if			
	TCWebApi_save()
	tcWebApi_commit("WLan11ac_Entry")	
End If
if Request_Form("isWDSSupported") = "1" then
	If Request_Form("wlanWEPFlag")="4" Then
		TCWebApi_set("WLan11ac_WDS","WdsEncrypType","WDS_EncrypType_Selection")
	End If
	TCWebApi_save()
End If
if Request_Form("isWPSSupported") = "1" then
If Request_Form("WpsStart")="1" Then
	TCWebApi_set("Info_WLan11ac","WPSActiveStatus","WpsStart")
	TCWebApi_set("WLan11ac_Entry","WPSConfMode","WpsConfModeAll")
	if Request_form("WPSMode_Selection") = "0" then
		TCWebApi_set("WLan11ac_Entry","enrolleePinCode","WPSEnrolleePINCode")
	end if
	TCWebApi_set("WLan11ac_Entry","WPSMode","WPSMode_Selection")
	tcWebApi_commit("WLan11ac_Entry")
End If
If Request_Form("WpsOOB")="1" Then
	TCWebApi_set("Info_WLan11ac","WPSOOBActive","WpsOOB")
	tcWebApi_commit("WLan11ac_Entry")
end if

If Request_Form("WpsGenerate") = "1" then
	TCWebApi_set("Info_WLan11ac","WPSGenPinCode","WpsGenerate")
	TCWebApi_commit("WLan11ac_Entry")
end if
	TCWebApi_save()
end if
%>
<HTML><HEAD><TITLE>WLAN5G配置</TITLE>
<META http-equiv=Content-Language content=zh-cn >
<META http-equiv=Content-Type content="text/html; charset=gb2312" >
<LINK href="/JS/stylemain.css" type=text/css rel=stylesheet >
<SCRIPT language=javascript src="/JS/menu.js" ></SCRIPT>
<SCRIPT language=javascript src="/JS/util.js" ></SCRIPT>
<META content="MSHTML 6.00.6000.16809" name=GENERATOR >
<SCRIPT language=JavaScript type=text/javascript >
HT5GExtCh = new Array(22);
HT5GExtCh[0] = 1;
HT5GExtCh[1] = 0;
HT5GExtCh[2] = 1;
HT5GExtCh[3] = 0;
HT5GExtCh[4] = 1;
HT5GExtCh[5] = 0;
HT5GExtCh[6] = 1;
HT5GExtCh[7] = 0;
HT5GExtCh[8] = 1;
HT5GExtCh[9] = 0;
HT5GExtCh[10] = 1;
HT5GExtCh[11] = 0;
HT5GExtCh[12] = 1;
HT5GExtCh[13] = 0;
HT5GExtCh[14] = 1;
HT5GExtCh[15] = 0;
HT5GExtCh[16] = 1;
HT5GExtCh[17] = 0;
HT5GExtCh[18] = 1;
HT5GExtCh[19] = 0;
HT5GExtCh[20] = 1;
HT5GExtCh[21] = 0;

<% if tcWebApi_get("WebCustom_Entry","isWPSSupported","h") = "Yes"  then %>
var wpsenable;
<% end if %>
var wepidx;
var WEPSelectIndex;

function doCheckWepSelectIndex()
{
<% if tcWebApi_get("WebCustom_Entry","isWlanEncrypt","h") = "Yes"  then %>
		WEPSelectIndex = document.ConfigForm.WEP_Selection.selectedIndex + 1;
<% else %>
		WEPSelectIndex = document.ConfigForm.WEP_Selection.selectedIndex;
<% end if %>
	return true;	
}

<% if tcWebApi_get("WebCustom_Entry","isWPSSupported","h") = "Yes"  then %>
function doStartWPS(){
	if((document.ConfigForm.WPSMode_Selection[0].checked)&&(document.ConfigForm.isInWPSing.value==0))
	{
		var pincode = document.ConfigForm.WPSEnrolleePINCode;
		var len = pincode.value.length;
		if(doPINCodeCheck(pincode) == false)
		{
			return ;
		}
<% if tcWebApi_get("WLan11ac_Entry0","WscV2Support","h") <> "1"  then %>
<% if tcWebApi_get("WLan11ac_Entry", "WPSConfStatus", "h") = "2" then %>
		if(len <= 0)
		{
		  alert("WPS PIN code couldn't be null!");
			return;
		}
<% end if %>
<% end if %>
	}
	if(document.ConfigForm.isInWPSing.value==0){
		alert("Please Start WPS peer within 2 minutes.");
	}
	document.ConfigForm.WpsStart.value = 1;
	document.ConfigForm.submit();
}

function doResetOOB(){
	document.ConfigForm.WpsOOB.value = 1;
	document.ConfigForm.submit();
}

function doGenerate(){
	document.ConfigForm.WpsGenerate.value = "1";
	document.ConfigForm.submit();
}
<% end if %>

function strESSIDCheck(str) {
	if(str.value.match(/[^\x00-\xff]/g)){
		alert("Invalid SSID Input!");
		return true;
	}
	if(document.ConfigForm.ESSID.value.length <= 0){
		alert("SSID is empty");
		return true;
	}
	return false;
}	

function LoadFrame()
{
	doCheckSSID();
	RefreshChannPage();
	doCheckWepSelectIndex();	
<% if tcWebApi_get("WLan11ac_Common","rt_device","h") = "7615" then %>
	initWLan11ac2ndFrequencySel();	
	VHTBandwidthRefreshPage();
	if(initFlag == true)
	{
		initWLan11acTxBeamFormingSel();
		initFlag = false;
	}
<% end if %>
	if(document.ConfigForm.isDot1XSupported.value==1)
	{
		if(document.ConfigForm.isAuthenTypeSupported.value==1)
		{
			document.getElementById("WEP_Selection_div").style.display="none";
			document.getElementById("WEP_Selection_show_div").style.display="";
			doWEPChange2();
		}
		else
		{
			document.getElementById("WEP_Selection_div").style.display="";
			document.getElementById("WEP_Selection_show_div").style.display="none";
		}
	}	
	if(document.ConfigForm.WirelessMode.selectedIndex>=1){
		document.ConfigForm.Is11nMode.value=1;
		if(document.ConfigForm.WirelessMode.selectedIndex>=2){
			document.ConfigForm.Is11acMode.value=1;
		}else{
			document.ConfigForm.Is11acMode.value=0;
		}
<% if tcWebApi_get("WebCustom_Entry","isHTBW40M","h") = "Yes" then %>
		if((document.ConfigForm.WLANChannelBandwidth.selectedIndex == 1)||(document.ConfigForm.WLANChannelBandwidth.selectedIndex == 2)){
<% else %>
		if(document.ConfigForm.WLANChannelBandwidth.selectedIndex == 1){
<% end if %>
			if(InsExtChOpt(document.ConfigForm.Channel_ID.value) == 1)
				document.ConfigForm.WLANExtensionChannel.selectedIndex = 1;
				else
				document.ConfigForm.WLANExtensionChannel.selectedIndex = 0;
				document.ConfigForm.WLANExtensionChannel.disabled = true;	
		}
	}else{
		document.ConfigForm.Is11nMode.value=0;
		document.ConfigForm.Is11acMode.value=0;
	}
<% if tcWebApi_get("WebCustom_Entry","isWPSSupported","h") = "Yes"  then %>
	if(document.ConfigForm.isInWPSing.value==1){
		document.ConfigForm.ResetOOB.disabled = true;
		document.ConfigForm.BUTTON.disabled = true;
		document.ConfigForm.CancelBtn.disabled = true;
	}
	if(document.ConfigForm.wlan_VC.value==0){
		wpsenable = document.ConfigForm.UseWPS_Selection[0].checked;
	}
<% end if %>
	wepidx = WEPSelectIndex;

	if(document.ConfigForm.isWDSSupported.value==1)
	{
		if(document.ConfigForm.WLAN_WDS_Active[0].checked == true){	//if wds enable
			autoWLAN_WDS_Active();
		}else if(document.ConfigForm.WLAN_WDS_Active[1].checked == true){	//if wds disable
			autoWLAN_WDS_Deactive();
		}
	}
	if(document.ConfigForm.bharti_ssid2.value==1)
		doloadSSID2();		
		
		McsRefreshPage();
}

function doCheckSSID()
{	
	var ssid_val = document.ConfigForm.wlan_VC.value;	
	var ssid_optval = document.ConfigForm.SSID_INDEX.value;	
	if(ssid_val != ssid_optval)	
	{		
		document.ConfigForm.wlanWEPFlag.value = 2;		
		document.ConfigForm.submit();	
	}
}	

function doRegionCheck(){
	var vCountryName = document.ConfigForm.Countries_Channels.value;
	var ctlCountryRegionABand = document.ConfigForm.hCountryRegionABand;
	var ctlCountryRegionABand0 = document.ConfigForm.CountryRegionABand0;
	var ctlCountryRegionABand1 = document.ConfigForm.CountryRegionABand1;
	var ctlCountryRegionABand2 = document.ConfigForm.CountryRegionABand2;
	var ctlCountryRegionABand3 = document.ConfigForm.CountryRegionABand3;
	var ctlCountryRegionABand4 = document.ConfigForm.CountryRegionABand4;
	var ctlCountryRegionABand5 = document.ConfigForm.CountryRegionABand5;
	var ctlCountryRegionABand6 = document.ConfigForm.CountryRegionABand6;
	var ctlCountryRegionABand7 = document.ConfigForm.CountryRegionABand7;
	var ctlCountryRegionABand8 = document.ConfigForm.CountryRegionABand8;
	var ctlCountryRegionABand9 = document.ConfigForm.CountryRegionABand9;
	var ctlCountryRegionABand10 = document.ConfigForm.CountryRegionABand10;
	var ctlCountryRegionABand11 = document.ConfigForm.CountryRegionABand11;
	if(vCountryName == "ARGENTINA")
		ctlCountryRegionABand.value = ctlCountryRegionABand3.value;
	else if (vCountryName == "ARMENIA")
		ctlCountryRegionABand.value = ctlCountryRegionABand2.value;
	else if (vCountryName == "AUSTRIA")
		ctlCountryRegionABand.value = ctlCountryRegionABand1.value;
	else if (vCountryName == "AZERBAIJAN")
		ctlCountryRegionABand.value = ctlCountryRegionABand2.value;
	else if (vCountryName == "BELGIUM")
		ctlCountryRegionABand.value = ctlCountryRegionABand1.value;
	else if (vCountryName == "BELIZE")
		ctlCountryRegionABand.value = ctlCountryRegionABand4.value;
	else if (vCountryName == "BOLIVIA")
		ctlCountryRegionABand.value = ctlCountryRegionABand4.value;
	else if (vCountryName == "BRAZIL")
		ctlCountryRegionABand.value = ctlCountryRegionABand1.value;
	else if (vCountryName == "BRUNEI DARUSSALAM")
		ctlCountryRegionABand.value = ctlCountryRegionABand4.value;
	else if (vCountryName == "BULGARIA")
		ctlCountryRegionABand.value = ctlCountryRegionABand1.value;
	else if (vCountryName == "CHINA")			             
		ctlCountryRegionABand.value = ctlCountryRegionABand4.value;		
	else if (vCountryName == "CROATIA")			             
		ctlCountryRegionABand.value = ctlCountryRegionABand2.value;		
	else if (vCountryName == "CYPRUS")			             
		ctlCountryRegionABand.value = ctlCountryRegionABand1.value;		
	else if (vCountryName == "CZECH REPUBLIC")			             
		ctlCountryRegionABand.value = ctlCountryRegionABand2.value;		
	else if (vCountryName == "DENMARK")			             
		ctlCountryRegionABand.value = ctlCountryRegionABand1.value;	
	else if (vCountryName == "EGYPT")			             
		ctlCountryRegionABand.value = ctlCountryRegionABand2.value;
	else if (vCountryName == "ESTONIA")			             
		ctlCountryRegionABand.value = ctlCountryRegionABand1.value;
	else if (vCountryName == "FINLAND")			             
		ctlCountryRegionABand.value = ctlCountryRegionABand1.value;
	else if (vCountryName == "FRANCE")			             
		ctlCountryRegionABand.value = ctlCountryRegionABand2.value;
	else if (vCountryName == "GEORGIA")			             
		ctlCountryRegionABand.value = ctlCountryRegionABand2.value;
	else if (vCountryName == "GERMANY")			             
		ctlCountryRegionABand.value = ctlCountryRegionABand1.value;
	else if (vCountryName == "GREECE")			             
		ctlCountryRegionABand.value = ctlCountryRegionABand1.value;
	else if (vCountryName == "HUNGARY")			             
		ctlCountryRegionABand.value = ctlCountryRegionABand1.value;
	else if (vCountryName == "ICELAND")			             
		ctlCountryRegionABand.value = ctlCountryRegionABand1.value;
	else if (vCountryName == "INDONESIA")			             
		ctlCountryRegionABand.value = ctlCountryRegionABand4.value;
	else if (vCountryName == "IRAN")			             
		ctlCountryRegionABand.value = ctlCountryRegionABand4.value;
	else if (vCountryName == "IRELAND")			             
		ctlCountryRegionABand.value = ctlCountryRegionABand1.value;
	else if (vCountryName == "ITALY")			             
		ctlCountryRegionABand.value = ctlCountryRegionABand1.value;
	else if (vCountryName == "JAPAN")			             
		ctlCountryRegionABand.value = ctlCountryRegionABand9.value;
	else if (vCountryName == "KOREA DEMOCRATIC")			             
		ctlCountryRegionABand.value = ctlCountryRegionABand5.value;
	else if (vCountryName == "KOREA REPUBLIC")			             
		ctlCountryRegionABand.value = ctlCountryRegionABand5.value;
	else if (vCountryName == "LATVIA")			             
		ctlCountryRegionABand.value = ctlCountryRegionABand1.value;
	else if (vCountryName == "LITHUANIA")			             
		ctlCountryRegionABand.value = ctlCountryRegionABand1.value;
	else if (vCountryName == "LUXEMBOURG")			             
		ctlCountryRegionABand.value = ctlCountryRegionABand1.value;
	else if (vCountryName == "MONACO")			             
		ctlCountryRegionABand.value = ctlCountryRegionABand2.value;
	else if (vCountryName == "NETHERLANDS")			             
		ctlCountryRegionABand.value = ctlCountryRegionABand1.value;
	else if (vCountryName == "PERU")			             
		ctlCountryRegionABand.value = ctlCountryRegionABand4.value;
	else if (vCountryName == "PHILIPPINES")			             
		ctlCountryRegionABand.value = ctlCountryRegionABand4.value;
	else if (vCountryName == "POLAND")			             
		ctlCountryRegionABand.value = ctlCountryRegionABand1.value;
	else if (vCountryName == "PORTUGAL")			             
		ctlCountryRegionABand.value = ctlCountryRegionABand1.value;
	else if (vCountryName == "SLOVAKIA")			             
		ctlCountryRegionABand.value = ctlCountryRegionABand1.value;
	else if (vCountryName == "SLOVENIA")			             
		ctlCountryRegionABand.value = ctlCountryRegionABand1.value;
	else if (vCountryName == "SOUTH AFRICA")			             
		ctlCountryRegionABand.value = ctlCountryRegionABand1.value;
	else if (vCountryName == "SPAIN")			             
		ctlCountryRegionABand.value = ctlCountryRegionABand1.value;
	else if (vCountryName == "SWEDEN")			             
		ctlCountryRegionABand.value = ctlCountryRegionABand1.value;
	else if (vCountryName == "SWITZERLAND")			             
		ctlCountryRegionABand.value = ctlCountryRegionABand1.value;
	else if (vCountryName == "TAIWAN")			             
		ctlCountryRegionABand.value = ctlCountryRegionABand3.value;
	else if (vCountryName == "TRINIDAD AND TOBAGO")			             
		ctlCountryRegionABand.value = ctlCountryRegionABand2.value;
	else if (vCountryName == "TUNISIA")			             
		ctlCountryRegionABand.value = ctlCountryRegionABand2.value;
	else if (vCountryName == "TURKEY")			             
		ctlCountryRegionABand.value = ctlCountryRegionABand2.value;
	else if (vCountryName == "UNITED KINGDOM")			             
		ctlCountryRegionABand.value = ctlCountryRegionABand1.value;
	else if (vCountryName == "URUGUAY")			             
		ctlCountryRegionABand.value = ctlCountryRegionABand5.value;
	else if (vCountryName == "UZBEKISTAN")			             
		ctlCountryRegionABand.value = ctlCountryRegionABand1.value;
	else if (vCountryName == "VENEZUELA")			             
		ctlCountryRegionABand.value = ctlCountryRegionABand5.value;
	else
		ctlCountryRegionABand.value = ctlCountryRegionABand0.value;

	RefreshChannPage();
	if(document.ConfigForm.RTDEVICE.value == "7615")
	{
		initWLan11ac2ndFrequencySel();
		VHTBandwidthRefreshPage();
	}
	document.ConfigForm.CountryChange.value = 1; 
}

var bInit = 1;
function RefreshChannPage(){
	var index = 0;
	var ctlChannel_ID = document.ConfigForm.Channel_ID;
	var vChannel = ctlChannel_ID.value;
	var vCountryRegionABand = document.ConfigForm.hCountryRegionABand.value;
	if(bInit == 1){
		vChannel = "<% tcWebApi_get("WLan11ac_Common","Channel","s") %>";
		bInit = 0;
	}
	ctlChannel_ID.length = 0;
	if(vCountryRegionABand == 0){
		ctlChannel_ID.options[index++] = new Option("AUTO", "0");
		ctlChannel_ID.options[index++] = new Option("36", "36");
		ctlChannel_ID.options[index++] = new Option("40", "40");
		ctlChannel_ID.options[index++] = new Option("44", "44");
		ctlChannel_ID.options[index++] = new Option("48", "48");
		ctlChannel_ID.options[index++] = new Option("52", "52");
		ctlChannel_ID.options[index++] = new Option("56", "56");
		ctlChannel_ID.options[index++] = new Option("60", "60");
		ctlChannel_ID.options[index++] = new Option("64", "64");
		ctlChannel_ID.options[index++] = new Option("149", "149");
		ctlChannel_ID.options[index++] = new Option("153", "153");
		ctlChannel_ID.options[index++] = new Option("157", "157");
		ctlChannel_ID.options[index++] = new Option("161", "161");
		ctlChannel_ID.options[index++] = new Option("165", "165");
	}else if(vCountryRegionABand == 1){
		ctlChannel_ID.options[index++] = new Option("AUTO", "0");
		ctlChannel_ID.options[index++] = new Option("36", "36");
		ctlChannel_ID.options[index++] = new Option("40", "40");
		ctlChannel_ID.options[index++] = new Option("44", "44");
		ctlChannel_ID.options[index++] = new Option("48", "48");
		ctlChannel_ID.options[index++] = new Option("52", "52");
		ctlChannel_ID.options[index++] = new Option("56", "56");
		ctlChannel_ID.options[index++] = new Option("60", "60");
		ctlChannel_ID.options[index++] = new Option("64", "64");
		ctlChannel_ID.options[index++] = new Option("100", "100");
		ctlChannel_ID.options[index++] = new Option("104", "104");
		ctlChannel_ID.options[index++] = new Option("108", "108");
		ctlChannel_ID.options[index++] = new Option("112", "112");
		ctlChannel_ID.options[index++] = new Option("116", "116");
		ctlChannel_ID.options[index++] = new Option("120", "120");
		ctlChannel_ID.options[index++] = new Option("124", "124");
		ctlChannel_ID.options[index++] = new Option("128", "128");
		ctlChannel_ID.options[index++] = new Option("132", "132");
		ctlChannel_ID.options[index++] = new Option("136", "136");
		ctlChannel_ID.options[index++] = new Option("140", "140");
	}else if(vCountryRegionABand == 2){
		ctlChannel_ID.options[index++] = new Option("AUTO", "0");
		ctlChannel_ID.options[index++] = new Option("36", "36");
		ctlChannel_ID.options[index++] = new Option("40", "40");
		ctlChannel_ID.options[index++] = new Option("44", "44");
		ctlChannel_ID.options[index++] = new Option("48", "48");
		ctlChannel_ID.options[index++] = new Option("52", "52");
		ctlChannel_ID.options[index++] = new Option("56", "56");
		ctlChannel_ID.options[index++] = new Option("60", "60");
		ctlChannel_ID.options[index++] = new Option("64", "64");
	}else if(vCountryRegionABand == 3){
		ctlChannel_ID.options[index++] = new Option("AUTO", "0");
		ctlChannel_ID.options[index++] = new Option("52", "52");
		ctlChannel_ID.options[index++] = new Option("56", "56");
		ctlChannel_ID.options[index++] = new Option("60", "60");
		ctlChannel_ID.options[index++] = new Option("64", "64");
		ctlChannel_ID.options[index++] = new Option("149", "149");
		ctlChannel_ID.options[index++] = new Option("153", "153");
		ctlChannel_ID.options[index++] = new Option("157", "157");
		ctlChannel_ID.options[index++] = new Option("161", "161");
	}else if(vCountryRegionABand == 4){
		ctlChannel_ID.options[index++] = new Option("AUTO", "0");
		ctlChannel_ID.options[index++] = new Option("149", "149");
		ctlChannel_ID.options[index++] = new Option("153", "153");
		ctlChannel_ID.options[index++] = new Option("157", "157");
		ctlChannel_ID.options[index++] = new Option("161", "161");
		ctlChannel_ID.options[index++] = new Option("165", "165");
	}else if(vCountryRegionABand == 5){
		ctlChannel_ID.options[index++] = new Option("AUTO", "0");
		ctlChannel_ID.options[index++] = new Option("149", "149");
		ctlChannel_ID.options[index++] = new Option("153", "153");
		ctlChannel_ID.options[index++] = new Option("157", "157");
		ctlChannel_ID.options[index++] = new Option("161", "161");
	}else if(vCountryRegionABand == 6){
		ctlChannel_ID.options[index++] = new Option("AUTO", "0");
		ctlChannel_ID.options[index++] = new Option("36", "36");
		ctlChannel_ID.options[index++] = new Option("40", "40");
		ctlChannel_ID.options[index++] = new Option("44", "44");
		ctlChannel_ID.options[index++] = new Option("48", "48");
	}else if(vCountryRegionABand == 8){
		ctlChannel_ID.options[index++] = new Option("AUTO", "0");
		ctlChannel_ID.options[index++] = new Option("52", "52");
		ctlChannel_ID.options[index++] = new Option("56", "56");
		ctlChannel_ID.options[index++] = new Option("60", "60");
		ctlChannel_ID.options[index++] = new Option("64", "64");
	}else if(vCountryRegionABand == 9){
		ctlChannel_ID.options[index++] = new Option("AUTO", "0");
		ctlChannel_ID.options[index++] = new Option("36", "36");
		ctlChannel_ID.options[index++] = new Option("40", "40");
		ctlChannel_ID.options[index++] = new Option("44", "44");
		ctlChannel_ID.options[index++] = new Option("48", "48");
		ctlChannel_ID.options[index++] = new Option("52", "52");
		ctlChannel_ID.options[index++] = new Option("56", "56");
		ctlChannel_ID.options[index++] = new Option("60", "60");
		ctlChannel_ID.options[index++] = new Option("64", "64");
		ctlChannel_ID.options[index++] = new Option("100", "100");
		ctlChannel_ID.options[index++] = new Option("104", "104");
		ctlChannel_ID.options[index++] = new Option("108", "108");
		ctlChannel_ID.options[index++] = new Option("112", "112");
		ctlChannel_ID.options[index++] = new Option("116", "116");
		ctlChannel_ID.options[index++] = new Option("132", "132");
		ctlChannel_ID.options[index++] = new Option("136", "136");
		ctlChannel_ID.options[index++] = new Option("140", "140");
		ctlChannel_ID.options[index++] = new Option("149", "149");
		ctlChannel_ID.options[index++] = new Option("153", "153");
		ctlChannel_ID.options[index++] = new Option("157", "157");
		ctlChannel_ID.options[index++] = new Option("161", "161");
		ctlChannel_ID.options[index++] = new Option("165", "165");
	}else if(vCountryRegionABand == 10){
		ctlChannel_ID.options[index++] = new Option("AUTO", "0");
		ctlChannel_ID.options[index++] = new Option("36", "36");
		ctlChannel_ID.options[index++] = new Option("40", "40");
		ctlChannel_ID.options[index++] = new Option("44", "44");
		ctlChannel_ID.options[index++] = new Option("48", "48");
		ctlChannel_ID.options[index++] = new Option("149", "149");
		ctlChannel_ID.options[index++] = new Option("153", "153");
		ctlChannel_ID.options[index++] = new Option("157", "157");
		ctlChannel_ID.options[index++] = new Option("161", "161");
		ctlChannel_ID.options[index++] = new Option("165", "165");
	}else if(vCountryRegionABand == 11){
		ctlChannel_ID.options[index++] = new Option("AUTO", "0");
		ctlChannel_ID.options[index++] = new Option("36", "36");
		ctlChannel_ID.options[index++] = new Option("40", "40");
		ctlChannel_ID.options[index++] = new Option("44", "44");
		ctlChannel_ID.options[index++] = new Option("48", "48");
		ctlChannel_ID.options[index++] = new Option("52", "52");
		ctlChannel_ID.options[index++] = new Option("56", "56");
		ctlChannel_ID.options[index++] = new Option("60", "60");
		ctlChannel_ID.options[index++] = new Option("64", "64");
		ctlChannel_ID.options[index++] = new Option("100", "100");
		ctlChannel_ID.options[index++] = new Option("104", "104");
		ctlChannel_ID.options[index++] = new Option("108", "108");
		ctlChannel_ID.options[index++] = new Option("112", "112");
		ctlChannel_ID.options[index++] = new Option("116", "116");
		ctlChannel_ID.options[index++] = new Option("120", "120");
		ctlChannel_ID.options[index++] = new Option("149", "149");
		ctlChannel_ID.options[index++] = new Option("153", "153");
		ctlChannel_ID.options[index++] = new Option("157", "157");
		ctlChannel_ID.options[index++] = new Option("161", "161");
	}else{
		ctlChannel_ID.options[index++] = new Option("AUTO", "0");
		ctlChannel_ID.options[index++] = new Option("36", "36");
		ctlChannel_ID.options[index++] = new Option("40", "40");
		ctlChannel_ID.options[index++] = new Option("44", "44");
		ctlChannel_ID.options[index++] = new Option("48", "48");
		ctlChannel_ID.options[index++] = new Option("52", "52");
		ctlChannel_ID.options[index++] = new Option("56", "56");
		ctlChannel_ID.options[index++] = new Option("60", "60");
		ctlChannel_ID.options[index++] = new Option("64", "64");
		ctlChannel_ID.options[index++] = new Option("100", "100");
		ctlChannel_ID.options[index++] = new Option("104", "104");
		ctlChannel_ID.options[index++] = new Option("108", "108");
		ctlChannel_ID.options[index++] = new Option("112", "112");
		ctlChannel_ID.options[index++] = new Option("116", "116");
		ctlChannel_ID.options[index++] = new Option("120", "120");
		ctlChannel_ID.options[index++] = new Option("124", "124");
		ctlChannel_ID.options[index++] = new Option("128", "128");
		ctlChannel_ID.options[index++] = new Option("132", "132");
		ctlChannel_ID.options[index++] = new Option("136", "136");
		ctlChannel_ID.options[index++] = new Option("140", "140");
		ctlChannel_ID.options[index++] = new Option("149", "149");
		ctlChannel_ID.options[index++] = new Option("153", "153");
		ctlChannel_ID.options[index++] = new Option("157", "157");
		ctlChannel_ID.options[index++] = new Option("161", "161");
		ctlChannel_ID.options[index++] = new Option("165", "165");
	}
	ctlChannel_ID.options[0].selected=true;
	SelectValue(ctlChannel_ID, vChannel);

	if(document.ConfigForm.Is11nMode.value == "1"){
		document.getElementById("11nMode_0_div").style.display="none";
		document.getElementById("11nMode_1_div").style.display="";
	}else{
		document.getElementById("11nMode_0_div").style.display="";
		document.getElementById("11nMode_1_div").style.display="none";
	}
<% if tcWebApi_get("WebCustom_Entry","isHTBW40M","h") = "Yes" then %>
	if((document.ConfigForm.WLANChannelBandwidth.value == 1 ) || (document.ConfigForm.WLANChannelBandwidth.value == 2 ))
<% else %>
	if(document.ConfigForm.WLANChannelBandwidth.value == "1")
<% end if %>
	{
		document.getElementById("HT_BW_1_div").style.display="";
		if(document.ConfigForm.Is11acMode.value == "1"){
			document.getElementById("11acMode_1_div").style.display="";
		}else{
			document.getElementById("11acMode_1_div").style.display="none";
		}
	}else{
		document.getElementById("HT_BW_1_div").style.display="none";
		document.getElementById("11acMode_1_div").style.display="none";
	}	
	if(document.ConfigForm.isWPSSupported.value == "1")
	{
		if(document.ConfigForm.wlan_VC.value == 0 ){
			if(document.ConfigForm.UseWPS_Selection[0].checked == true)
				document.getElementById("WPSConfMode_1_div").style.display="";
			else
				document.getElementById("WPSConfMode_1_div").style.display="none";
		}
	}
	var vAuthMode = document.ConfigForm.WEP_Selection.value;
	if(document.getElementById("Radius-WEP64_div") != null) 
		document.getElementById("Radius-WEP64_div").style.display="none";
	if(document.getElementById("Radius-WEP128_div") != null)
		document.getElementById("Radius-WEP128_div").style.display="none";
	if(document.getElementById("WPA_div") != null)
		document.getElementById("WPA_div").style.display="none";
	if(document.getElementById("WPA2_div") != null)
		document.getElementById("WPA2_div").style.display="none";
	if(document.getElementById("WPA1WPA2_div") != null)
		document.getElementById("WPA1WPA2_div").style.display="none";
		
	if(vAuthMode == "Radius-WEP64"){
		if(document.getElementById("Radius-WEP64_div") != null) 
			document.getElementById("Radius-WEP64_div").style.display="";
	}
	if(vAuthMode == "Radius-WEP128"){
		if(document.getElementById("Radius-WEP128_div") != null)
			document.getElementById("Radius-WEP128_div").style.display="";
	}
	if(vAuthMode == "WPA"){
		if(document.getElementById("WPA_div") != null)
			document.getElementById("WPA_div").style.display="";
	}
	if(vAuthMode == "WPA2"){
		if(document.getElementById("WPA2_div") != null)
			document.getElementById("WPA2_div").style.display="";
	}
	if(vAuthMode == "WPA1WPA2"){
		if(document.getElementById("WPA1WPA2_div") != null)
			document.getElementById("WPA1WPA2_div").style.display="";
	}
		
	if(document.getElementById("WEP-64Bits_div") != null)
		document.getElementById("WEP-64Bits_div").style.display="none";
	if(document.getElementById("WEP-128Bits_div") != null)
		document.getElementById("WEP-128Bits_div").style.display="none";
	if(document.getElementById("WPA2PSK_div") != null)
		document.getElementById("WPA2PSK_div").style.display="none";
	if(document.getElementById("WPAPSK_div") != null)
		document.getElementById("WPAPSK_div").style.display="none";
	if(document.getElementById("WPAPSKWPA2PSK_div") != null)
		document.getElementById("WPAPSKWPA2PSK_div").style.display="none";	
	if(vAuthMode == "WEP-64Bits"){
		if(document.getElementById("WEP-64Bits_div") != null)
			document.getElementById("WEP-64Bits_div").style.display="";
	}else if(vAuthMode == "WEP-128Bits"){
		if(document.getElementById("WEP-128Bits_div") != null)
			document.getElementById("WEP-128Bits_div").style.display="";
	}else if(vAuthMode == "WPA2PSK"){
		if(document.getElementById("WPA2PSK_div") != null)
			document.getElementById("WPA2PSK_div").style.display="";
	}else if(vAuthMode == "WPAPSK"){
		if(document.getElementById("WPAPSK_div") != null)
			document.getElementById("WPAPSK_div").style.display="";
	}else if(vAuthMode == "WPAPSKWPA2PSK"){
		if(document.getElementById("WPAPSKWPA2PSK_div") != null)
			document.getElementById("WPAPSKWPA2PSK_div").style.display="";	
	}
	
	if(document.getElementById("else_div") != null)
		document.getElementById("else_div").style.display="none";
	if(vAuthMode == "OPEN")
		;
	else if(vAuthMode == "WEP-64Bits")
		;
	else if(vAuthMode == "WEP-128Bits")
		;
	else if(vAuthMode == "Radius-WEP64")
		;
	else if(vAuthMode == "Radius-WEP128")
		;
	else if(document.getElementById("else_div") != null)
		document.getElementById("else_div").style.display="";
}

var MCSInit = 1;
function McsRefreshPage(){
	var rt_device = "<%tcWebApi_get("WLan11ac_Common","rt_device","s")%>";
	var index = 0;
	var WLANMCS_ID = document.ConfigForm.WLANMCS;
	var vWLANMCS = WLANMCS_ID.value;
	var ls11acmode = document.ConfigForm.Is11acMode.value;
	if(MCSInit == 1){
		vWLANMCS = "<%tcWebApi_get("WLan11ac_Entry","HT_MCS","s")%>";
		MCSInit = 0;
	}
	WLANMCS_ID.length = 0;
	WLANMCS_ID.options[index++] = new Option("0", "0");
	WLANMCS_ID.options[index++] = new Option("1", "1");
	WLANMCS_ID.options[index++] = new Option("2", "2");
	WLANMCS_ID.options[index++] = new Option("3", "3");
	WLANMCS_ID.options[index++] = new Option("4", "4");
	WLANMCS_ID.options[index++] = new Option("5", "5");
	WLANMCS_ID.options[index++] = new Option("6", "6");
	WLANMCS_ID.options[index++] = new Option("7", "7");
	if(ls11acmode == "1"){
		WLANMCS_ID.options[index++] = new Option("8", "8");
		if (document.ConfigForm.WLANChannelBandwidth.selectedIndex != 0)
		WLANMCS_ID.options[index++] = new Option("9", "9");
	}
	else if (rt_device == "7612" || rt_device == "7615") {
		WLANMCS_ID.options[index++] = new Option("8", "8");
		WLANMCS_ID.options[index++] = new Option("9", "9");
		WLANMCS_ID.options[index++] = new Option("10", "10");
		WLANMCS_ID.options[index++] = new Option("11", "11");
		WLANMCS_ID.options[index++] = new Option("12", "12");
		WLANMCS_ID.options[index++] = new Option("13", "13");
		WLANMCS_ID.options[index++] = new Option("14", "14");
		WLANMCS_ID.options[index++] = new Option("15", "15");
	}
	WLANMCS_ID.options[index++] = new Option("AUTO", "33");
	
	WLANMCS_ID.options[0].selected=true;
	SelectValue(WLANMCS_ID, vWLANMCS);
}

<% If tcWebApi_get("WLan11ac_Common","rt_device","h") = "7615" Then %>
var VHTSec80Channel = "<% tcWebApi_get("WLan11ac_Common","VHT_Sec80_Channel","s") %>";
var initFlag = true;

function initWLan11acTxBeamFormingSel()			//init TxBeamforming sel
{
	var ITxBfEn = "<%tcWebApi_get("WLan11ac_Common","ITxBfEn","s")%>";
	var ETxBfEnCond = "<%tcWebApi_get("WLan11ac_Common","ETxBfEnCond","s")%>";
	var ETxBfIncapable = "<%tcWebApi_get("WLan11ac_Common","ETxBfIncapable","s")%>";
	if(ITxBfEn == "1" && ETxBfEnCond == "1" && ETxBfIncapable == "0")		//Both
	{
		document.ConfigForm.WLan11acTxBeamForming[0].selected = true;
	}
	else if(ITxBfEn == "0" && ETxBfEnCond == "1" && ETxBfIncapable == "0")		//Explicit
	{
		document.ConfigForm.WLan11acTxBeamForming[1].selected = true;
	}
	else if(ITxBfEn == "1" && ETxBfEnCond == "0" && ETxBfIncapable == "1")		//Implicit
	{
		document.ConfigForm.WLan11acTxBeamForming[2].selected = true;
	}
	else if(ITxBfEn == "0" && ETxBfEnCond == "0" && ETxBfIncapable == "1")		//Disable
	{
		document.ConfigForm.WLan11acTxBeamForming[3].selected = true;
	}
	else
	{
		document.ConfigForm.WLan11acTxBeamForming[3].selected = true;
	}
}

function doVHTTxBeamFormingChange()
{
	if(document.ConfigForm.WLan11acTxBeamForming[0].selected == true)			//Both
	{
		document.ConfigForm.WLan11acITxBfEn.value = "1";
		document.ConfigForm.WLan11acETxBfEnCond.value = "1";
		document.ConfigForm.WLan11acETxBfIncapable.value = "0";
	}
	else if(document.ConfigForm.WLan11acTxBeamForming[1].selected == true)		//Explicit
	{
		document.ConfigForm.WLan11acITxBfEn.value = "0";
		document.ConfigForm.WLan11acETxBfEnCond.value = "1";
		document.ConfigForm.WLan11acETxBfIncapable.value = "0";
	}
	else if(document.ConfigForm.WLan11acTxBeamForming[2].selected == true)		//Implicit
	{
		document.ConfigForm.WLan11acITxBfEn.value = "1";
		document.ConfigForm.WLan11acETxBfEnCond.value = "0";
		document.ConfigForm.WLan11acETxBfIncapable.value = "1";
	}
	else if(document.ConfigForm.WLan11acTxBeamForming[3].selected == true)		//Disable
	{
		document.ConfigForm.WLan11acITxBfEn.value = "0";
		document.ConfigForm.WLan11acETxBfEnCond.value = "0";
		document.ConfigForm.WLan11acETxBfIncapable.value = "1";
	}
	else
	{
		document.ConfigForm.WLan11acITxBfEn.value = "0";
		document.ConfigForm.WLan11acETxBfEnCond.value = "0";
		document.ConfigForm.WLan11acETxBfIncapable.value = "1";
	}
}

function setVHTbwChannel()
{
	document.ConfigForm.VHTSec80Channel.value = VHTSec80Channel;					
}

function initWLan11ac2ndFrequencySel()		//init 2ndFrequencyChannel sel options
{
	document.ConfigForm.WLan11ac2ndFrequency.length = 0;
	var opts = document.ConfigForm.Channel_ID.options;
	var selindex = document.ConfigForm.Channel_ID.selectedIndex;
	var optsval = opts[selindex].value;
	var oindex = 0;
	var fcount = new Array(5);
	fcount[0] = "36~48";
	fcount[1] = "52~64";
	fcount[2] = "100~112";
	fcount[3] = "116~128";
	fcount[4] = "149~161";
	if(optsval >= 36 && optsval <= 48)
	{
		document.ConfigForm.WLan11ac2ndFrequency[oindex++] = new Option(fcount[1],0);
		document.ConfigForm.WLan11ac2ndFrequency[oindex++] = new Option(fcount[2],1);
		document.ConfigForm.WLan11ac2ndFrequency[oindex++] = new Option(fcount[3],2);
		document.ConfigForm.WLan11ac2ndFrequency[oindex++] = new Option(fcount[4],3);
		if(VHTSec80Channel == "52")
		{
			document.ConfigForm.WLan11ac2ndFrequency[0].selected = true;
		}
		else if(VHTSec80Channel == "100")
		{
			document.ConfigForm.WLan11ac2ndFrequency[1].selected = true;
		}
		else if(VHTSec80Channel == "116")
		{
			document.ConfigForm.WLan11ac2ndFrequency[2].selected = true;
		}
		else if(VHTSec80Channel == "149")
		{
			document.ConfigForm.WLan11ac2ndFrequency[3].selected = true;
		}		
	}
	else if(optsval >= 52 && optsval <= 64)
	{
		document.ConfigForm.WLan11ac2ndFrequency[oindex++] = new Option(fcount[0],0);
		document.ConfigForm.WLan11ac2ndFrequency[oindex++] = new Option(fcount[2],1);
		document.ConfigForm.WLan11ac2ndFrequency[oindex++] = new Option(fcount[3],2);
		document.ConfigForm.WLan11ac2ndFrequency[oindex++] = new Option(fcount[4],3);
		if(VHTSec80Channel == "36")
		{
			document.ConfigForm.WLan11ac2ndFrequency[0].selected = true;
		}
		else if(VHTSec80Channel == "100")
		{
			document.ConfigForm.WLan11ac2ndFrequency[1].selected = true;
		}
		else if(VHTSec80Channel == "116")
		{
			document.ConfigForm.WLan11ac2ndFrequency[2].selected = true;
		}
		else if(VHTSec80Channel == "149")
		{
			document.ConfigForm.WLan11ac2ndFrequency[3].selected = true;
		}	
	}	
	else if(optsval >= 100 && optsval <= 112)
	{
		document.ConfigForm.WLan11ac2ndFrequency[oindex++] = new Option(fcount[0],0);
		document.ConfigForm.WLan11ac2ndFrequency[oindex++] = new Option(fcount[1],1);
		document.ConfigForm.WLan11ac2ndFrequency[oindex++] = new Option(fcount[3],2);
		document.ConfigForm.WLan11ac2ndFrequency[oindex++] = new Option(fcount[4],3);
		if(VHTSec80Channel == "36")
		{
			document.ConfigForm.WLan11ac2ndFrequency[0].selected = true;
		}
		else if(VHTSec80Channel == "52")
		{
			document.ConfigForm.WLan11ac2ndFrequency[1].selected = true;
		}
		else if(VHTSec80Channel == "116")
		{
			document.ConfigForm.WLan11ac2ndFrequency[2].selected = true;
		}
		else if(VHTSec80Channel == "149")
		{
			document.ConfigForm.WLan11ac2ndFrequency[3].selected = true;
		}		
	}
	else if(optsval >= 116 && optsval <= 128)
	{
		document.ConfigForm.WLan11ac2ndFrequency[oindex++] = new Option(fcount[0],0);
		document.ConfigForm.WLan11ac2ndFrequency[oindex++] = new Option(fcount[1],1);
		document.ConfigForm.WLan11ac2ndFrequency[oindex++] = new Option(fcount[2],2);
		document.ConfigForm.WLan11ac2ndFrequency[oindex++] = new Option(fcount[4],3);
		if(VHTSec80Channel == "36")
		{
			document.ConfigForm.WLan11ac2ndFrequency[0].selected = true;
		}
		else if(VHTSec80Channel == "52")
		{
			document.ConfigForm.WLan11ac2ndFrequency[1].selected = true;
		}
		else if(VHTSec80Channel == "100")
		{
			document.ConfigForm.WLan11ac2ndFrequency[2].selected = true;
		}
		else if(VHTSec80Channel == "149")
		{
			document.ConfigForm.WLan11ac2ndFrequency[3].selected = true;
		}	
	}
	else if(optsval >= 149 && optsval <= 161)
	{
		document.ConfigForm.WLan11ac2ndFrequency[oindex++] = new Option(fcount[0],0);
		document.ConfigForm.WLan11ac2ndFrequency[oindex++] = new Option(fcount[1],1);
		document.ConfigForm.WLan11ac2ndFrequency[oindex++] = new Option(fcount[2],2);
		document.ConfigForm.WLan11ac2ndFrequency[oindex++] = new Option(fcount[3],3);
		if(VHTSec80Channel == "36")
		{
			document.ConfigForm.WLan11ac2ndFrequency[0].selected = true;
		}
		else if(VHTSec80Channel == "52")
		{
			document.ConfigForm.WLan11ac2ndFrequency[1].selected = true;
		}
		else if(VHTSec80Channel == "100")
		{
			document.ConfigForm.WLan11ac2ndFrequency[2].selected = true;
		}
		else if(VHTSec80Channel == "116")
		{
			document.ConfigForm.WLan11ac2ndFrequency[3].selected = true;
		}		
	}
	else if(selindex != 0)
	{
		document.ConfigForm.WLan11ac2ndFrequency[oindex++] = new Option(fcount[0],0);
		document.ConfigForm.WLan11ac2ndFrequency[oindex++] = new Option(fcount[1],1);
		document.ConfigForm.WLan11ac2ndFrequency[oindex++] = new Option(fcount[2],2);
		document.ConfigForm.WLan11ac2ndFrequency[oindex++] = new Option(fcount[3],3);
		document.ConfigForm.WLan11ac2ndFrequency[oindex++] = new Option(fcount[4],4);
		if(VHTSec80Channel == "36")
		{
			document.ConfigForm.WLan11ac2ndFrequency[0].selected = true;
		}
		else if(VHTSec80Channel == "52")
		{
			document.ConfigForm.WLan11ac2ndFrequency[1].selected = true;
		}
		else if(VHTSec80Channel == "100")
		{
			document.ConfigForm.WLan11ac2ndFrequency[2].selected = true;
		}
		else if(VHTSec80Channel == "116")
		{
			document.ConfigForm.WLan11ac2ndFrequency[3].selected = true;
		}
		else if(VHTSec80Channel == "149")
		{
			document.ConfigForm.WLan11ac2ndFrequency[4].selected = true;
		}	
	}
	onVHTbwChannelChange();		
}

function onVHTbwChannelChange()
{
	var opts = document.ConfigForm.Channel_ID.options;
	var selindex = document.ConfigForm.Channel_ID.selectedIndex;
	var optsval = opts[selindex].value;
	var j = document.ConfigForm.WLan11ac2ndFrequency.selectedIndex;
	if(optsval >= 36 && optsval <= 48)
	{
		switch(j)
		{
			case 0:
				VHTSec80Channel = "52";
				break;
			case 1:
				VHTSec80Channel = "100";
				break;
			case 2:
				VHTSec80Channel = "116";
				break;
			case 3:
				VHTSec80Channel = "149";
				break;
		}
	}
	else if(optsval >= 52 && optsval <= 64)
	{
		switch(j)
		{
			case 0:
				VHTSec80Channel = "36";
				break;
			case 1:
				VHTSec80Channel = "100";
				break;
			case 2:
				VHTSec80Channel = "116";
				break;
			case 3:
				VHTSec80Channel = "149";
				break;
		}
	}		
	else if(optsval >= 100 && optsval <= 112)
	{
		switch(j)
		{
			case 0:
				VHTSec80Channel = "36";
				break;
			case 1:
				VHTSec80Channel = "52";
				break;
			case 2:
				VHTSec80Channel = "116";
				break;
			case 3:
				VHTSec80Channel = "149";
				break;
		}
	}	
	else if(optsval >= 116 && optsval <= 128)
	{				
		switch(j)
		{
			case 0:
				VHTSec80Channel = "36";
				break;
			case 1:
				VHTSec80Channel = "52";
				break;
			case 2:
				VHTSec80Channel = "100";
				break;
			case 3:
				VHTSec80Channel = "149";
				break;
		}
	}	
	else if(optsval >= 149 && optsval <= 161)
	{		
		switch(j)
		{
			case 0:
				VHTSec80Channel = "36";
				break;
			case 1:
				VHTSec80Channel = "52";
				break;
			case 2:
				VHTSec80Channel = "100";
				break;
			case 3:
				VHTSec80Channel = "116";
				break;
		}
	}
	else if(selindex != 0)
	{
		switch(j)
		{
			case 0:
				VHTSec80Channel = "36";
				break;
			case 1:
				VHTSec80Channel = "52";
				break;
			case 2:
				VHTSec80Channel = "100";
				break;
			case 3:
				VHTSec80Channel = "116";
				break;
			case 4:
				VHTSec80Channel = "149";
				break;
		}
	}	
}	

function VHTBandwidthRefreshPage()
{
	if(document.ConfigForm.WLan11acVHTChannelBandwidth.options[3].selected == true && document.ConfigForm.Channel_ID.selectedIndex != 0)
	{
		document.getElementById("WLan11ac2ndFrequencyTable").style.display = "";
	}
	else
	{
		document.getElementById("WLan11ac2ndFrequencyTable").style.display = "none";
	}
}
<% End If %>

<% if tcWebApi_get("WebCustom_Entry","isWPSSupported","h") = "Yes"  then %>
function dowpscheck(){				//modified by fred to support WPS2.0
	var wlan=document.ConfigForm;
	if(wlan.SSID_INDEX.value==0){	
		if(wlan.UseWPS_Selection[0].checked == true){
			//check if WscV2Supported
<% if tcWebApi_get("WLan11ac_Entry0","WscV2Support","h") = "1"  then %>
			if (wlan.ESSID_HIDE_Selection[1].checked == true){
				alert("SSID broadcast must be enabled if you want to use WPS, in other cases you need to disable WPS firstly!");
			  wlan.UseWPS_Selection[1].checked = true;
			  return 0;
			}
			if (WEPSelectIndex != 0 && WEPSelectIndex != 3 && WEPSelectIndex != 4 && WEPSelectIndex != 5 && WEPSelectIndex != 8 && WEPSelectIndex != 9 && WEPSelectIndex != 10)
			{
				alert("The authentication type must be one of \"OPEN,WPA,WPA2,WPAPSK,WPA2PSK,WPAPSK/WPA2PSK\" if you want to use WPS, in other cases you need to disable WPS firstly!");
				wlan.WEP_Selection.selectedIndex = wepidx;	
				if(wpsenable)
				{
					wlan.UseWPS_Selection[0].checked = true;
				}else{
					wlan.UseWPS_Selection[1].checked = true;
				}
					return 0;
			}
			if(((WEPSelectIndex == 3) && (wlan.TKIP_Selection5.value== "TKIP"))||((WEPSelectIndex==4) && (wlan.TKIP_Selection4.value== "TKIP"))|| ((WEPSelectIndex==5) && (wlan.TKIP_Selection6.value== "TKIP")))
			{
				//WPAPSK ,WPA2PSK and WPAPSKWPA2PSK with TKIP	
				alert("Encryption type must not be TKIP if you want to use WPS, in other cases you need to disable WPS firstly!");
				wlan.UseWPS_Selection[1].checked = true;
				return 0;
			}
			else if(((WEPSelectIndex == 8) && (wlan.TKIP_Selection1.value== "TKIP"))||((WEPSelectIndex==9) && (wlan.TKIP_Selection2.value== "TKIP"))|| ((WEPSelectIndex==10) && (wlan.TKIP_Selection3.value== "TKIP")))
			{	
				//WPA, WPA2 and WPAWPA2 with TKIP
				alert("Encryption type must not be TKIP if you want to use WPS, in other cases you need to disable WPS firstly!");
				wlan.UseWPS_Selection[1].checked = true;
				return 0;
			}
	
			return 1;
<% end if %>
		//do simple check if only WPS 1.0 supported, use original check code in 1.0
<% if tcWebApi_get("WLan11ac_Entry0","WscV2Support","h") <> "1"  then %>
		//WEPSelectIndex 1=WEP64,2=WEP128,3=Radius-WEP64,4=Radius-WEP128
		if(WEPSelectIndex == 1 || WEPSelectIndex == 2 || WEPSelectIndex == 6 || WEPSelectIndex == 7){
			alert("We should not use WEP when WPS function turned on!");
			if(wpsenable){
				wlan.UseWPS_Selection[0].checked = true;
			}else{
				wlan.UseWPS_Selection[1].checked = true;
			}
			return 0;
		}else{
			return 1;
		}
<% end if %>
	
		}
		else{
			return 1;
		}
	}else{
		return 1;
	}
}
<%  end if %>

<% if tcWebApi_get("WebCustom_Entry","isWPSSupported","h") = "Yes"  then %>
function doBroadcastSSIDChange(){
	//check if WscV2Supported
<% if tcWebApi_get("WLan11ac_Entry0","WscV2Support","h") = "1"  then %>
	var wlan=document.ConfigForm;
	if((wlan.SSID_INDEX.value==0) && (wlan.UseWPS_Selection[0].checked == true) && (wlan.ESSID_HIDE_Selection[1].checked == true))
	{
			var rv = confirm("WPS will be disabled!");
			if (rv == false)
			{
				wlan.ESSID_HIDE_Selection[0].checked = true;
				wlan.ESSID_HIDE_Selection[1].checked = false;
			}
			else
			{
				wlan.ESSID_HIDE_Selection[0].checked = false;
				wlan.ESSID_HIDE_Selection[1].checked = true;
				wlan.UseWPS_Selection[0].checked = false;
				wlan.UseWPS_Selection[1].checked = true;
			}
	}
  LoadFrame();
<% end if %>
	
	return 1;	
}

function doEncryptionChange(object){
	//check if WscV2Supported
<% if tcWebApi_get("WLan11ac_Entry0","WscV2Support","h") = "1"  then %>
	var wlan=document.ConfigForm;	
	if((wlan.SSID_INDEX.value==0) && (wlan.UseWPS_Selection[0].checked == true) && (object.value == "TKIP"))
	{
		var rv = confirm("WPS will be disabled!");
		if (rv == false)
		{
			object.value="AES";
		}
		else
		{
			object.value = "TKIP";
			wlan.UseWPS_Selection[0].checked = false;
			wlan.UseWPS_Selection[1].checked = true;
		}
	}
	LoadFrame();
<% end if %>

	return 1;
}
<% end if %>

function doWEPChange(){
	doCheckWepSelectIndex();
	var wlan=document.ConfigForm;	
<% if tcWebApi_get("WebCustom_Entry","isWPSSupported","h") = "Yes"  then %>
<% if tcWebApi_get("WLan11ac_Entry0","WscV2Support","h") = "1"  then %>
	if((wlan.SSID_INDEX.value==0) && (wlan.UseWPS_Selection[0].checked == true) && (WEPSelectIndex != 0 && WEPSelectIndex != 3 && WEPSelectIndex != 4 && WEPSelectIndex != 5 && WEPSelectIndex != 8 && WEPSelectIndex != 9 && WEPSelectIndex != 10))
	{
		var rv = confirm("WPS will be disabled!");
		if (rv == false)
		{
			wlan.WEP_Selection.selectedIndex = wepidx;
		}
		else
		{
			wlan.UseWPS_Selection[0].checked = false;
			wlan.UseWPS_Selection[1].checked = true;
		}			
	}
<% end if %>

	//do simple check if only WPS 1.0 supported, use original check code in 1.0
<% if tcWebApi_get("WLan_Entry0","WscV2Support","h") <> "1"  then %>
	if((wlan.SSID_INDEX.value==0) && (wlan.UseWPS_Selection[0].checked == true) &&(WEPSelectIndex == 1 || WEPSelectIndex == 2 || WEPSelectIndex == 6 || WEPSelectIndex == 7))
	{
		alert("We should not use WEP when WPS function turned on!");
		wlan.WEP_Selection.selectedIndex = wepidx;
	}
<% end if %>
<% end if %>	
	if(WEPSelectIndex == 0)
	{
		var rv = confirm("Your network will be set to OPEN without security setting, we strongly suggest you choose WPAPSK or WPA2PSK encryption!!");
		if (rv == false)
		{
			wlan.WEP_Selection.selectedIndex = wepidx;
		}
	}
	wlan.wlanWEPFlag.value = 1;
	LoadFrame();
}

function doWEPChange2(){
<% if tcWebApi_get("WebCustom_Entry","isWPSSupported","h") = "Yes"  then %>
	if(dowpscheck()){
<% end if %>
		document.ConfigForm.wlanWEPFlag.value = 1;
		if(WEPSelectIndex != 9){
<% if tcWebApi_get("WebCustom_Entry","isWlanEncrypt","h") = "Yes"  then %>
			document.ConfigForm.WEP_Selection.selectedIndex = 8;
<% else %>
			document.ConfigForm.WEP_Selection.selectedIndex = 9;
<% end if %>
			document.ConfigForm.submit();
		}
<% if tcWebApi_get("WebCustom_Entry","isWPSSupported","h") = "Yes"  then %>
	}
<% end if %>
}

function doWDSEncrypTypeChange(){
		document.ConfigForm.wlanWEPFlag.value = 4;
}

function doSSIDChange(){
	document.ConfigForm.wlanWEPFlag.value = 2;
	document.ConfigForm.submit();
}

function doExtChaLockChange() {
	if(document.ConfigForm.WirelessMode.selectedIndex >= 1){
		if(document.ConfigForm.RTDEVICE.value == "7615")
		{
			initWLan11ac2ndFrequencySel();
			VHTBandwidthRefreshPage();
		}
<% if tcWebApi_get("WebCustom_Entry","isHTBW40M","h") = "Yes" then %>
		if((document.ConfigForm.WLANChannelBandwidth.selectedIndex == 1)||(document.ConfigForm.WLANChannelBandwidth.selectedIndex == 2)){
<% else %>
		if(document.ConfigForm.WLANChannelBandwidth.selectedIndex == 1){
<% end if %>
			document.ConfigForm.wlanWEPFlag.value = 1;
			LoadFrame();
		}
	}
}

function doWirelessModeChange(){
	document.ConfigForm.wlanWEPFlag.value = 1;
	if(document.ConfigForm.WirelessMode.selectedIndex>=1){
		document.ConfigForm.Is11nMode.value=1;
		if(document.ConfigForm.WirelessMode.selectedIndex>=2){
			document.ConfigForm.Is11acMode.value=1;
		}else{
			document.ConfigForm.Is11acMode.value=0;	
		}
	}else{
		document.ConfigForm.Is11nMode.value=0;
		document.ConfigForm.Is11acMode.value=0;
	}
	LoadFrame();
}

function doVHTBandwidthChange(){
	document.ConfigForm.wlanWEPFlag.value = 1;
	LoadFrame();
}

function doChannelBandwidthChange(){
	document.ConfigForm.wlanWEPFlag.value = 1;
	LoadFrame();
}

function doExtChannChange(){
	if(document.ConfigForm.WLANExtensionChannel.selectedIndex==0){
		document.ConfigForm.ExtChannFlag.value = 0;
	}else{
		document.ConfigForm.ExtChannFlag.value = 1;
	}
}

<% if tcWebApi_get("WebCustom_Entry","isWPSSupported","h") = "Yes"  then %>
function doWPSUseChange(){
	if(dowpscheck()){
		document.ConfigForm.wlanWEPFlag.value = 1;
		RefreshChannPage();
		LoadFrame();
	}
}

function doWPSModeChange(){
	document.ConfigForm.wlanWEPFlag.value = 1;
	document.ConfigForm.submit();
}
<% end if %>

function wpapskCheck(object) {
	var keyvalue=object.value;
	var wpapsklen=object.value.length;
 	if(wpapsklen >= 8 && wpapsklen < 64){
  	if(keyvalue.match(/[^\x00-\xff]/g))
 	  {
			alert("Pre-Shared Key should be between 8 and 63 ASCII characters or 64 Hex string.");
			return true;
 	  }
	}else if(wpapsklen==64){
		for(i=0;i<64;i++){
			var c=keyvalue.charAt(i);
			if(doHexCheck(c)<0){
				alert("Pre-Shared Key Hex value error!");
				return true;
			}
		}
	}else {
    alert("Pre-Shared Key length error!");
		return true;
	}			
	return false;
}

function WDSKeyCheck(object) {
	var keyvalue=object.value;
	var wdskeylen=object.value.length;
 	if(wdskeylen < 8) {
  	alert('WDS Key length error!');
		return true;
	}else if(wdskeylen==64){
		for(i=0;i<64;i++){
			var c=keyvalue.charAt(i);
			if(doHexCheck(c)<0){
				alert('WDS Key Hex value error!');
				return true;
			}
		}
	}else if(wdskeylen > 64) {
    alert('WDS Key length error!');
		return true;
	}			
	return false;
}

function doWEPTypeChange(){

}

function doHexCheck(c)
{
  if ((c >= "0")&&(c <= "9"))
  {
    return 1;
  }
  else if ((c >= "A")&&(c <= "F"))
  {
    return 1;
  }
  else if ((c >= "a")&&(c <= "f"))
  {
    return 1;
  }

  return -1;
}

function doNonSympolCheck(c)
{
	if ((c >= "0")&&(c <= "9"))
	{
		return 1;
	}
	else if ((c >= "A")&&(c <= "Z"))
	{
		return 1;
	}
	else if ((c >= "a")&&(c <= "z"))
	{
		return 1;
	}

  return -1;
}

function doKEYcheck(object)
{
	var index = object.value.indexOf("0x");
	len=object.value.length;
	
	if(len == 0){
		return true;
	}

	if(WEPSelectIndex==1)
	{
		if(len==5)/*wep 64*/
		{
			return true;
		}
		else if(len==10)/*wep 64*/
		{
			for(i=0;i<len;i++)
			{
				var c = object.value.charAt(i);	
				if(doHexCheck(c)==-1)
				{
					alert("<%tcWebApi_get("String_Entry","WirelessJS9Text","s")%>");
						return false;
				}
			}
		}
		else
		{
			alert("Invalid Key Value");
			return false;
		}
	}
	else if(WEPSelectIndex==2)
	{
		if(len==13)/*wep 128*/
		{
			return true;
		}
		else if(len==26)/*wep 128*/
		{
			for(i=0;i<len;i++)
			{
				var c = object.value.charAt(i);	
				if(doHexCheck(c)==-1)
				{
					alert("Invalid Key Value");
					return false;
				}
			}
		}
		else
		{
			alert("Invalid Key Value");
			return false;
		}
	}
	if(document.ConfigForm.isDot1XSupported.value==1)
	{
		if(WEPSelectIndex==6)
		{
			if(len==5)/*wep 64*/
			{
				return true;
			}
			else if(len==10)/*wep 64*/
			{
				for(i=0;i<len;i++)
				{
					var c = object.value.charAt(i);	
					if(doHexCheck(c)==-1)
					{
						alert("Invalid Key Value");
						return false;
					}
				}
			}
			else
			{
				alert("Invalid Key Value");
				return false;
			}
		}
		else if(WEPSelectIndex==7)
		{
			if(len==13)/*wep 128*/
			{
				return true;
			}
			else if(len==26)/*wep 128*/
			{
				for(i=0;i<len;i++)
				{
					var c = object.value.charAt(i);	
					if(doHexCheck(c)==-1)
					{
						alert("nvalid Key Value");
						return false;
					}
				}
			}
			else
			{
				alert("nvalid Key Value");
				return false;
			}
		}
	}
	return true;
}

function doMACcheck(object)
{
  var szAddr = object.value;
  var len = szAddr.length;
  var errMsg = "Invalid MAC Address";

  if ( len == 0 )
  {
    object.value ="00:00:00:00:00:00";
    return;
  }

  if ( len == 12 )
  {
    var newAddr = "";
    var i = 0;

    for ( i = 0; i < len; i++ )
    {
      var c = szAddr.charAt(i);
      
      if ( doHexCheck(c) < 0 )
      {
      	alert("Invalid MAC Address");        
      	object.focus();
        return;
      }
      if ( (i == 2) || (i == 4) || (i == 6) || (i == 8) || (i == 10) )
        newAddr = newAddr + ":";
      newAddr = newAddr + c;
    }
    object.value = newAddr;
    return;
  }
  else if ( len == 17 )
  {
    var i = 2;
    var c0 = szAddr.charAt(0);
    var c1 = szAddr.charAt(1);

    if ( (doHexCheck(c0) < 0) || (doHexCheck(c1) < 0) )
    {
			alert("Invalid MAC Address");       	
			object.focus();
			return;
    }
    
    i = 2;
    while ( i < len )
    {
      var c0 = szAddr.charAt(i);
      var c1 = szAddr.charAt(i+1);
      var c2 = szAddr.charAt(i+2);  
      if ( (c0 != ":") || (doHexCheck(c1) < 0) || (doHexCheck(c2) < 0) )
      {
				alert("Invalid MAC Address");         	
				object.focus();
				return;
      }
      i = i + 3;
    }
    return; 
  }
  else
  {
		alert("Invalid MAC Address");         	
		object.focus();
		return;
  }
}

function checkBeacon(value) 
{
  if (!isNumeric(value)) {
    alert("Non-integer value given" + value);    
    return true;
  }
  if (value < 20 || value > 1000) {
    alert("Beacon value must be between 20 and 1000");    
    return true;  	
  }
  return false;
}

function checkRTS(value) 
{
  if (!isNumeric(value)) {
    alert("Non-integer value given" + value);    
    return true;
  }
  if (value < 1500 || value > 2347) {
    alert("RTS Threshold value must be between 1500 and 2347");    
    return true;  	
  }
  return false;
}

function checkFrag(value) 
{
  if (!isNumeric(value)) {
    alert("Non-integer value given" + value);    
    return true;
  }
  if (value < 256 || value > 2346) {
    alert("Fragmentation Threshold value must be between 256 and 2346");    
    return true;  	
  }
  if (value % 2) {
    alert("Fragmentation Threshold value must be an even number");    
    return true;  	
  }
  return false;
}

function checkDTIM(value) 
{
  if (!isNumeric(value)) {
    alert("Non-integer value given" + value);    
    return true;
  }
  if (value < 1 || value > 255) {
    alert("DTIM value must be between 1 and 255");		
    return true;
	}	
	return false;
}

<% If tcWebApi_get("WebCustom_Entry","isMaxStaNumSupported","h") = "Yes" Then %>
function checkStationNum(value, limit) 
{
	if (!isNumeric(value) || parseInt(value,10) < 0 || parseInt(value,10) > parseInt(limit,10)){
    alert("Station Number value must be between 0 and " + limit);
		return true;
	}	
	return false;
}
<% End If %>

function checkRekeyinteral(value, flag) 
{
	if (!isNumeric(value)) {
		if(flag == 1){
			alert("WPA Group Rekey Interval : Non-integer value given"); 
		}else{
			alert("Key Renewal Interval : Non-integer value given");
		}
		return true;
	}
	if (value < 10 || value > 4194303) {
		if(flag == 1){
			alert("WPA Group Rekey Interval must be between 10 and 4194303");
		}else{
			alert("Key Renewal Interval must be between 10 and 4194303");
		}	
		return true;
	}	
	return false;
}

function quotationCheck(object, limit_len) {
	var len = object.value.length;
	var c;
	var i, j = 0;
  for (i = 0; i < len; i++)
  {
	 	var c = object.value.charAt(i);
	  if (c == '"')
		{
			j += 6;
		}
		else
			j++;
  }
 	if (j > limit_len-1)
	{
    alert('too many quotation marks!');		
    return true;
	}	
	return false;
}

function ValidateChecksum(PIN)
{
	var accum = 0;
	accum += 3 * (((PIN - PIN % 10000000) / 10000000) % 10);
	accum += 1 * (((PIN - PIN % 1000000) / 1000000) % 10);
	accum += 3 * (((PIN - PIN % 100000) / 100000) % 10);
	accum += 1 * (((PIN - PIN % 10000) / 10000) % 10);
	accum += 3 * (((PIN - PIN % 1000) / 1000) % 10);
	accum += 1 * (((PIN - PIN % 100) / 100) % 10);
	accum += 3 * (((PIN - PIN % 10) / 10) % 10);
	accum += 1 * (((PIN - PIN % 1) / 1) % 10);
	if ((accum % 10) == 0)
		return true;
	else
		return false;
}

<% if tcWebApi_get("WebCustom_Entry","isWPSSupported","h") = "Yes"  then %>
<% if tcWebApi_get("WLan11ac_Entry0","WscV2Support","h") = "1"  then %>
//interface for future use
function ValidateChecksum_4(PIN)
{
	return true;
}

function ignoreSpaces(string) {
   var temp = "";
   string = '' + string;
   splitstring = string.split(" ");
   for(i = 0; i < splitstring.length; i++)
       temp += splitstring[i];
   return temp;
}

function ignoreDash(string) {
   var temp = "";
   string = '' + string;
   splitstring = string.split("-");
   for(i = 0; i < splitstring.length; i++)
       temp += splitstring[i];
   return temp;
}
<% end if %>

function doPINCodeCheck(object)
{
	var len= object.value.length;
	var ch;
<% if tcWebApi_get("WLan11ac_Entry0","WscV2Support","h") = "1"  then %>
	if(len==9)
	{
		acutal_value=0;
		ch_4= object.value.charAt(4);
		if( ch_4 != ' ' & ch_4  != '-')
		{
				alert("WPS PIN code format error!");
				return false;
		}
		object.value = ignoreSpaces(object.value);
		object.value = ignoreDash(object.value);
		
		if (ValidateChecksum(Number(object.value)) == false)
		{
			alert("WPS PIN code checksum error!");
			return false;
		}
		return true;
	}

	if(len==8)
	{
		for( i=0; i < len; i++)
		{
			ch= object.value.charAt(i);
			if( ch > '9' || ch < '0')
			{
				alert("WPS PIN code must be 8 digits!");
				return false;
			}
		}
		if (ValidateChecksum(Number(object.value)) == false){
			alert("WPS PIN code checksum error!");
			return false;		
		}
		return true;
	}

	if(len==4)
	{
		for( i=0; i < len; i++)
		{
			ch= object.value.charAt(i);
			if( ch > '9' || ch < '0')
			{
				alert("WPS PIN code must be 8 digits!");
				return false;
			}
		}
		if (ValidateChecksum_4(Number(object.value)) == false)
		{
			alert("WPS PIN code checksum error!>");
			return false;
		}
		return true;
	}

	if(len==0)
	{
		return true;
	}

	alert("WPS PIN code format error!");
	return false;
<% end if %>

<% if tcWebApi_get("WLan11ac_Entry0","WscV2Support","h") <> "1"  then %>
	if (len > 0)
	{
		if(len < 8)
		{
			alert("WPS PIN code must be 8 digits!");
			return;
		}
		for( i=0; i < len; i++)
		{
			ch= object.value.charAt(i);
			if( ch > '9' || ch < '0')
			{
				alert("WPS PIN code must be 8 digits!");
				return;
			}
		}
		if (ValidateChecksum(Number(object.value)) == false)
		{
			alert("WPS PIN code checksum error!");
		}
		return;
	}
<% end if %>	
}
<% end if %>	

function doSave(){
<% if tcWebApi_get("WebCustom_Entry","isWPSSupported","h") = "Yes"  then %>
	if(document.ConfigForm.SSID_INDEX.value==0){
    if(document.ConfigForm.WPSMode_Selection[0].checked)
    {
			var pincode = document.ConfigForm.WPSEnrolleePINCode;
			if((doPINCodeCheck(pincode) == false))
			{
			  return false;
			}
     }
	}
<% end if %>
<% if tcWebApi_get("Info_WLan11ac","isbharti","h") = "Yes" then %>
	if(0 == WEPSelectIndex)	
	{
		return false;
	}
<% end if %>
	if(checkBeacon(document.ConfigForm.BeaconInterval.value) ||
		checkRTS(document.ConfigForm.RTSThreshold.value) ||
		checkFrag(document.ConfigForm.FragmentThreshold.value) ||
		checkDTIM(document.ConfigForm.DTIM.value)
<% If tcWebApi_get("WebCustom_Entry","isMaxStaNumSupported","h") = "Yes" Then %>
		|| checkStationNum(document.ConfigForm.StationNum.value, document.ConfigForm.maxStaNum.value)
<% End If %>
	){
		return false;
	}
		
	if(WEPSelectIndex == 3){ 
		document.ConfigForm.hRekeyMethod.value = "TIME";
		if (wpapskCheck(document.ConfigForm.PreSharedKey2)){
			return false;
		}
		
		if(quotationCheck(document.ConfigForm.PreSharedKey2, 385) ){
			return false;	 
		}
		if(checkRekeyinteral(document.ConfigForm.keyRenewalInterval2.value, 0)){
			return false;
		}  			
	}

	if(WEPSelectIndex == 4){
		document.ConfigForm.hRekeyMethod.value = "TIME";
		if (wpapskCheck(document.ConfigForm.PreSharedKey1)){
			return false;
		}
		
		if(quotationCheck(document.ConfigForm.PreSharedKey1, 385) ){
			return false;	 
		}
		if(checkRekeyinteral(document.ConfigForm.keyRenewalInterval1.value, 0)){
			return false;
		}  			
	}

	if(WEPSelectIndex == 5){
		document.ConfigForm.hRekeyMethod.value = "TIME";
		if (wpapskCheck(document.ConfigForm.PreSharedKey3)){
			return false;
		}
		
		if(quotationCheck(document.ConfigForm.PreSharedKey3, 385) ){
			return false;	 
		}
		if(checkRekeyinteral(document.ConfigForm.keyRenewalInterval3.value, 0)){
			return false;
		}  			
	}
      	
	if(WEPSelectIndex == 1){ 
		document.ConfigForm.hRekeyMethod.value = "DISABLE";
		if((!doKEYcheck(document.ConfigForm.WEP_Key13))||
		(!doKEYcheck(document.ConfigForm.WEP_Key23))||
		(!doKEYcheck(document.ConfigForm.WEP_Key33))||
		(!doKEYcheck(document.ConfigForm.WEP_Key43))){
			return false;
		}
	}
	
	if(WEPSelectIndex == 2){ 
		document.ConfigForm.hRekeyMethod.value = "DISABLE";
		if((!doKEYcheck(document.ConfigForm.WEP_Key14))||
		(!doKEYcheck(document.ConfigForm.WEP_Key24))||
		(!doKEYcheck(document.ConfigForm.WEP_Key34))||
		(!doKEYcheck(document.ConfigForm.WEP_Key44))){
			return false;
		}
	}
	
	if(document.ConfigForm.isDot1XSupported.value==1)
	{	
		if(WEPSelectIndex == 6) 
		{
			if((!doKEYcheck(document.ConfigForm.WEP_Key11))||
			(!doKEYcheck(document.ConfigForm.WEP_Key21))||
			(!doKEYcheck(document.ConfigForm.WEP_Key31))||
			(!doKEYcheck(document.ConfigForm.WEP_Key41)))
			{
				return false;
			}
		}

		if(WEPSelectIndex == 7) 
		{
			if((!doKEYcheck(document.ConfigForm.WEP_Key12))||
			(!doKEYcheck(document.ConfigForm.WEP_Key22))||
			(!doKEYcheck(document.ConfigForm.WEP_Key32))||
			(!doKEYcheck(document.ConfigForm.WEP_Key42)))
			{
				return false;
			}
		}
<% if tcWebApi_get("WebCustom_Entry","isWlanEncrypt","h") = "Yes"  then %>
		var vAuthMode = document.ConfigForm.WEP_Selection.selectedIndex + 1;
<% else %>
		var vAuthMode = document.ConfigForm.WEP_Selection.selectedIndex;
<% end if %>
		if((WEPSelectIndex == 6) || (WEPSelectIndex == 7) || (WEPSelectIndex == 8) || (WEPSelectIndex == 9)
		|| (WEPSelectIndex == 10))
		{
			if(document.ConfigForm.isDot1XEnhanceSupported.value == 0)
			{
				if(vAuthMode == 6){
					radiusip = document.ConfigForm.radiusSVR_IP1.value;
				}
				else if(vAuthMode == 7){
					radiusip = document.ConfigForm.radiusSVR_IP2.value;
				}
				else if(vAuthMode == 8){
					radiusip = document.ConfigForm.radiusSVR_IP3.value;
				}
				else if(vAuthMode == 9){
					radiusip = document.ConfigForm.radiusSVR_IP4.value;
				}
				else if(vAuthMode == 10){
					radiusip = document.ConfigForm.radiusSVR_IP5.value;
				}								
				
			 if(inValidIPAddr(radiusip))
			 {
				return false;
			 }
			}
			//serverport
			if(vAuthMode == 6){
				radiusport = parseInt(document.ConfigForm.radiusSVR_Port1.value);
				radiuskey = document.ConfigForm.radiusSVR_Key1.value;
				session_timeout_interval = document.ConfigForm.radiusSVR_ReAuthInter1.value;
			}
			else if(vAuthMode == 7){
				radiusport = parseInt(document.ConfigForm.radiusSVR_Port2.value);
				radiuskey = document.ConfigForm.radiusSVR_Key2.value;
				session_timeout_interval = document.ConfigForm.radiusSVR_ReAuthInter2.value;
			}
			else if(vAuthMode == 8){
				radiusport = parseInt(document.ConfigForm.radiusSVR_Port3.value);
				radiuskey = document.ConfigForm.radiusSVR_Key3.value;
				session_timeout_interval = document.ConfigForm.radiusSVR_ReAuthInter3.value;
			}
			else if(vAuthMode == 9){
				radiusport = parseInt(document.ConfigForm.radiusSVR_Port4.value);
				radiuskey = document.ConfigForm.radiusSVR_Key4.value;
				session_timeout_interval = document.ConfigForm.radiusSVR_ReAuthInter4.value;
			}
			else if(vAuthMode == 10){
				radiusport = parseInt(document.ConfigForm.radiusSVR_Port5.value);
				radiuskey = document.ConfigForm.radiusSVR_Key5.value;
				session_timeout_interval = document.ConfigForm.radiusSVR_ReAuthInter5.value;
			}
			if(isNaN(radiusport) || radiusport < 0 || radiusport > 65535)
			{	
				alert("adius Server Port number's range: 0 ~ 65535");
				return false;
			}
			
			// radius share key
			{
				if (radiuskey.length == 0)
				{
  					alert("Radius Share secret can not be empty"); 
  					return false; 
				}
			}

			{
				if (session_timeout_interval.length == 0)
				{
  					alert("Re-auth Interval can not be empty"); 
  					return false; 
				}
			}
   	}

		if((WEPSelectIndex == 8) || (WEPSelectIndex == 9) || (WEPSelectIndex == 10))
		{
			if(vAuthMode == 8){
				WPARekeyInter = document.ConfigForm.WPARekeyInter3.value;
			}
			else if(vAuthMode == 9){
				WPARekeyInter = document.ConfigForm.WPARekeyInter4.value;
			}
			else if(vAuthMode == 10){
				WPARekeyInter = document.ConfigForm.WPARekeyInter5.value;
			}
			
			if(checkRekeyinteral(WPARekeyInter, 1)){
				return false;	
			}
   	}
	}
	
	//if wds support meantime AuthMode of MAIN SSID is wpa2psk,wpapsk WPAPSKWPA2PSK , then check wds key 
	if(document.ConfigForm.isWDSSupported.value==1)
	{
		if(document.ConfigForm.WLAN_WDS_Active[0].checked == true)//if wds enable ,to check wds_key, or not to check
		{
			if(document.ConfigForm.isDot1XSupported.value==1)
			{
				if((WEPSelectIndex == 3) || (WEPSelectIndex == 4) || (WEPSelectIndex == 5)
				|| (WEPSelectIndex == 8) || (WEPSelectIndex == 9) || (WEPSelectIndex == 10))
				{
					if (WDSKeyCheck(document.ConfigForm.WDS_Key))
					{
						return false;
					}
			
					if(quotationCheck(document.ConfigForm.WDS_Key, 385) )
					{
						return false;	 
					}  			
				}	
			}
			else
			{
				if((WEPSelectIndex == 3) || (WEPSelectIndex == 4) || (WEPSelectIndex == 5))
				{
					if (WDSKeyCheck(document.ConfigForm.WDS_Key))
					{
						return false;
					}
					if(quotationCheck(document.ConfigForm.WDS_Key, 385) )
					{
						return false;	 
					}  			
				}	
			}
		}
	}
	if(quotationCheck(document.ConfigForm.ESSID, 193)||strESSIDCheck(document.ConfigForm.ESSID)){
		return;	   
	}

	document.ConfigForm.wlanWEPFlag.value = 3;
	if(document.ConfigForm.WirelessMode.selectedIndex>=1){
		document.ConfigForm.Is11nMode.value=1;
		if(document.ConfigForm.WirelessMode.selectedIndex>=2){
			document.ConfigForm.Is11acMode.value=1;
		}else{
			document.ConfigForm.Is11acMode.value=0;
		}
<% if tcWebApi_get("WebCustom_Entry","isHTBW40M","h") = "Yes" then %>
		if(document.ConfigForm.WLANChannelBandwidth.selectedIndex == 1){
			document.ConfigForm.Wlan_HTBW40M.value = 1;
			document.ConfigForm.WLANChannelBandwidth.value = 1;
		}
		else if(document.ConfigForm.WLANChannelBandwidth.selectedIndex == 2){
			document.ConfigForm.Wlan_HTBW40M.value = 0;
			document.ConfigForm.WLANChannelBandwidth.value = 1;
		}
		else{
			document.ConfigForm.WLANChannelBandwidth.value = 0;
		}
<% end if %>	
	}else{
		document.ConfigForm.Is11nMode.value=0;
		document.ConfigForm.Is11acMode.value=0;
	}
<% if tcWebApi_get("WLan11ac_Common","rt_device","h") = "7615" then %>
	doVHTTxBeamFormingChange();
	if((document.ConfigForm.WLan11acVHTChannelBandwidth.options[3].selected == true) && (document.ConfigForm.Channel_ID.selectedIndex != 0))
	{
		setVHTbwChannel();
	}
<% end if %>
	if(!checkSelectedKEY()){
		return false;
	}
	document.ConfigForm.submit();
}

function checkSelectedKEY(){
	var group;
	var curCBX;
	var vAuthMode = document.ConfigForm.WEP_Selection.value;
	if(vAuthMode == "Radius-WEP64"){
		group = document.ConfigForm.DefWEPKey1;
	}
	else if(vAuthMode == "Radius-WEP128"){
		group = document.ConfigForm.DefWEPKey2;
	}
	else if(vAuthMode == "WEP-64Bits"){
		group = document.ConfigForm.DefWEPKey3;
	}
	else if(vAuthMode == "WEP-128Bits"){
		group = document.ConfigForm.DefWEPKey4;
	}else{
		return true;
	}

	for (var i=0; i<group.length; i++){
		if (group[i].checked)
		break;
	}
	
	if(vAuthMode == "Radius-WEP64"){
		switch (i)
		{
		case 0:
			curCBX = document.ConfigForm.WEP_Key11;
			break;
		case 1:
			curCBX = document.ConfigForm.WEP_Key21;
			break;
		case 2:
			curCBX = document.ConfigForm.WEP_Key31;
			break;
		case 3:
			curCBX = document.ConfigForm.WEP_Key41;
			break;
		default:
		  ;
		}
	}
	else if(vAuthMode == "Radius-WEP128"){
		switch (i)
		{
		case 0:
			curCBX = document.ConfigForm.WEP_Key12;
			break;
		case 1:
			curCBX = document.ConfigForm.WEP_Key22;
			break;
		case 2:
			curCBX = document.ConfigForm.WEP_Key32;
			break;
		case 3:
			curCBX = document.ConfigForm.WEP_Key42;
			break;
		default:
		  ;
		}
	}
	else if(vAuthMode == "WEP-64Bits"){
		switch (i)
		{
		case 0:
			curCBX = document.ConfigForm.WEP_Key13;
			break;
		case 1:
			curCBX = document.ConfigForm.WEP_Key23;
			break;
		case 2:
			curCBX = document.ConfigForm.WEP_Key33;
			break;
		case 3:
			curCBX = document.ConfigForm.WEP_Key43;
			break;
		default:
		  ;
		}
	}
	else if(vAuthMode == "WEP-128Bits"){
		switch (i)
		{
		case 0:
			curCBX = document.ConfigForm.WEP_Key14;
			break;
		case 1:
			curCBX = document.ConfigForm.WEP_Key24;
			break;
		case 2:
			curCBX = document.ConfigForm.WEP_Key34;
			break;
		case 3:
			curCBX = document.ConfigForm.WEP_Key44;
			break;
		default:
		  ;
		}
	}
	
	len=curCBX.value.length;
	index = curCBX.value.indexOf("0x");

	if(WEPSelectIndex==1)/*wep 64*/
	{
		if(len==5)
		{
			for(i=0;i<len;i++)
			{
				var c = curCBX.value.charAt(i);	
				if(doNonSympolCheck(c)==-1)
				{
					alert("Invalid Key Value");
					curCBX.focus();
					return false;
				}
			}
		}
		else if(len==10)
		{
			for(i=0;i<len;i++)
			{
				var c = curCBX.value.charAt(i);	
				if(doHexCheck(c)==-1)
				{
					alert("Invalid Key Value");
					curCBX.focus();
					return false;
				}
			}
		}
		else if(len==12)
		{
			if(index==0)
			{
				for(i=2;i<len;i++)
				{
					var c = curCBX.value.charAt(i);	
					if(doHexCheck(c)==-1)
					{
						alert("Invalid Key Value");
						curCBX.focus();
						return false;
					}
				}
			}
			else
			{
				alert("Invalid Key Value");
				curCBX.focus();
				return false;
			}
		}
		else
		{
			alert("Invalid Key Value");
			curCBX.focus();
			return false;
		}
	}
	else if(WEPSelectIndex==2)/*wep 128*/
	{
		if(len==13)
		{
			for(i=0;i<len;i++)
			{
				var c = curCBX.value.charAt(i);	
				if(doNonSympolCheck(c)==-1)
				{
					alert("Invalid Key Value");
					curCBX.focus();
					return false;
				}
			}
		}
		else if(len==26)
		{
			for(i=0;i<len;i++)
			{
				var c = curCBX.value.charAt(i);	
				if(doHexCheck(c)==-1)
				{
					alert("Invalid Key Value");
					curCBX.focus();
					return false;
				}
			}
		}
		else if(len==28)
		{
			if(index==0)
			{
				for(i=2;i<len;i++)
				{
					var c = curCBX.value.charAt(i);	
					if(doHexCheck(c)==-1)
					{
						alert("Invalid Key Value");
						curCBX.focus();
						return false;
					}
				}
			}
			else
			{
				alert("Invalid Key Value");
				curCBX.focus();
				return false;
			}
		}
		else
		{
			alert("Invalid Key Value");
			curCBX.focus();
			return false;
		}
	}
	return true;
}

function checkFocus(value){
	if(WEPSelectIndex == 0){
		document.ConfigForm.WEP_Selection.focus();
	}
}

function autoWLAN_WDS_Active()
{
	if(document.ConfigForm.WDS_EncrypType_Selection != null){
		document.ConfigForm.WDS_EncrypType_Selection.disabled = false;
		document.ConfigForm.WDS_Key.disabled = false;
	}
	document.ConfigForm.WLANWDS_PEER_MAC1.disabled = false;
	document.ConfigForm.WLANWDS_PEER_MAC2.disabled = false;
	document.ConfigForm.WLANWDS_PEER_MAC3.disabled = false;
	document.ConfigForm.WLANWDS_PEER_MAC4.disabled = false;
}	

function autoWLAN_WDS_Deactive()
{
	if(document.ConfigForm.WDS_EncrypType_Selection != null){
		document.ConfigForm.WDS_EncrypType_Selection.disabled = true;
		document.ConfigForm.WDS_Key.disabled = true;
	}
	document.ConfigForm.WLANWDS_PEER_MAC1.disabled = true;
	document.ConfigForm.WLANWDS_PEER_MAC2.disabled = true;
	document.ConfigForm.WLANWDS_PEER_MAC3.disabled = true;
	document.ConfigForm.WLANWDS_PEER_MAC4.disabled = true;
}

function InsExtChOpt(CurrCh)
{
	var ExtChann = document.ConfigForm.ExtChannFlag;
	if ((1*CurrCh >= 36) && (1*CurrCh <= 64))
	{
			CurrCh = 1*CurrCh;
			CurrCh /= 4;
			CurrCh -= 9;

			ExtChann.value = HT5GExtCh[CurrCh];
	}
	else if ((1*CurrCh >= 100) && (1*CurrCh <= 136))
	{
			CurrCh = 1*CurrCh;
			CurrCh /= 4;
			CurrCh -= 17;

			ExtChann.value = HT5GExtCh[CurrCh];
	}
	else if ((1*CurrCh >= 149) && (1*CurrCh <= 161))
	{
			CurrCh = 1*CurrCh;
			CurrCh -= 1;
			CurrCh /= 4;
			CurrCh -= 19;

			ExtChann.value = HT5GExtCh[CurrCh];
	}
	else
	{
			ExtChann.value = 0;
	}
	return ExtChann.value;
}

function doloadSSID2()
{
	if(document.ConfigForm.SSID_INDEX.selectedIndex == 1)
	{
		var j;
		var frm = document.ConfigForm;
		for(j = 0; j < frm.elements.length; j++)
		{	
			if(frm.elements[j].type != "hidden")		
				frm.elements[j].disabled = true;
		}
	}
	document.ConfigForm.SSID_INDEX.disabled = false;
}

function isNumeric(s)
{
  var len = s.length;
  var ch;
  if(len==0)
    return false;
  for( i=0; i< len; i++)
  {
    ch= s.charAt(i);
    if( ch > '9' || ch < '0')
    {
      return false;
    }
  }
  return true;
}

function SelectValue(o,v){
	for(var i=0; i<o.options.length; i++)
		if(o.options[i].value == v){
		o.options[i].selected=true;
		break;
	}
}
</SCRIPT></HEAD>
<BODY style="TEXT-ALIGN: center" vLink=#000000 aLink=#000000 link=#000000 leftMargin=0 topMargin=0 
	onload="DisplayLocation(getElement('Selected_Menu').value);FinishLoad();if(getElById('ConfigForm') != null)LoadFrame()" 
	onunload=DoUnload() marginheight="0" marginwidth="0" >
<TABLE height="100%" cellSpacing=0 cellPadding=0 width=808 align=center border=0 >
<TBODY>
<TR>
<TD height=1 >
<% if tcwebApi_get("WebCustom_Entry","isCYE8SFUSupported","h") = "Yes" then %>
	<TABLE height=80 cellSpacing=0 cellPadding=0 width=808 background=/img/framelogo.jpg border=0 >
<% else %>
	<TABLE height=117 cellSpacing=0 cellPadding=0 width=808 background=/img/framelogo.jpg border=0 >
<% end if %> 
	<TBODY>
	<TR>
	<TD>&nbsp;</TD>
	<TD vAlign=bottom align=right width=358 >
		<TABLE id=table8 cellSpacing=0 cellPadding=0 border=0 >
		<TBODY>
		<TR>
		<TD vAlign=bottom align=right ><SPAN class=curUserName >&nbsp;</SPAN></TD>
		<TD class=welcom vAlign=bottom align=middle width=120 >欢迎您!</TD>
		<TD vAlign=bottom width=50 ><A onclick=DoLogout() href="/cgi-bin/logout.cgi" target=_top ><SPAN class=logout >退出</SPAN></A></TD>
		</TR>
		</TBODY>
		</TABLE>
	</TD>
	</TR>
	</TBODY>
	</TABLE>
	<TABLE id=table2 height=100 cellSpacing=0 cellPadding=0 width=808 border=0 >
	<TBODY>
	<TR>
	<TD class=LocationDisplay id=LocationDisplay align=middle width=163 bgColor=#ef8218 rowSpan=3 ></TD>
	<TD width=434 bgColor=#427594 height=33 ><P align=right ><FONT face=黑体 color=#ffffff >
		<B><FONT face=黑体 color=#ffffff size=6 ><INPUT id=Selected_Menu type=hidden value="网络->WLAN5G配置" name=Selected_Menu ></FONT></B>
		<SPAN class=GatewayName >网关名称:
<SCRIPT language=javascript>
	document.write(top.gateWayName);
</SCRIPT>
		</SPAN></FONT></P>
	</TD>
	<TD width=211 bgColor=#ef8218 height=33 ><P class=GatewayType align=center >型号:
<SCRIPT language=javascript>
	document.write(top.ModelName);
</SCRIPT>
		</P>
	</TD>
	</TR>
	<TR><TD id=MenuArea_L1 vAlign=bottom bgColor=#ef8218 colSpan=2 height=43 >&nbsp;</TD></TR>
	<TR><TD id=MenuArea_L2 bgColor=#427594 colSpan=2 height=24 ></TD></TR>
	</TBODY>
	</TABLE>
<SCRIPT language=javascript>
	MakeMenu(getElById('Selected_Menu').value);
</SCRIPT>
	<TABLE id=table3 height=15 cellSpacing=0 cellPadding=0 width=808 border=0 >
	<TBODY>
	<TR>
	<TD height=15 ><IMG height=15 src="/img/panel1.gif" width=164 border=0 ></TD>
	<TD><IMG height=15 src="/img/panel2.gif" width=645 border=0 ></TD>
	</TR>
	</TBODY>
	</TABLE>
</TD>
</TR>
<TR>
<TD vAlign=top >				
	<TABLE height="100%" cellSpacing=0 cellPadding=0 border=0 >
	<TBODY>
	<TR>
	<TD width=157 bgColor=#ef8218 height=20 ><P class=Item_L1 >WLAN5G配置</P></TD>
	<TD width=7 bgColor=#ef8218 ></TD>
	<TD width=474 ></TD>
	<TD vAlign=top width=170 background=/img/panel4.gif rowSpan=5 >
		<TABLE cellSpacing=0 cellPadding=20 width="100%" border=0 height='100%' >
		<TBODY>
		<TR><TD valign='top'><A href="/cgi-bin/help_content.asp#WLAN配置" target=_blank ><IMG height=34 src="/img/help.gif" width=40 border=0 ></A></TD></TR>
<% if tcwebApi_get("WebCustom_Entry","isCYE8SFUSupported","h") = "Yes" then %>
		<TR><TD valign='bottom'><IMG src='/img/tybottom.jpg' height=76 width=112 ></TD></TR>
<% end if %>
		</TBODY>
		</TABLE>
	</TD>
	</TR>
	<TR>
	<TD vAlign=top width=157 bgColor=#e7e7e7 height=10 ></TD>
	<TD width=7 background=/img/panel3.gif ></TD>
	<TD></TD>
	</TR>
	<TR>
	<TD vAlign=top width=157 bgColor=#e7e7e7 height=30 ><P class=Item_L2 ></P></TD>
	<TD width=7 background=/img/panel3.gif ></TD>
	<TD vAlign=top >
		<FORM name=ConfigForm action="/cgi-bin/net-wlan11ac.asp" method="post" >
<INPUT TYPE="HIDDEN" NAME="wlanWEPFlag" VALUE="0" >
<INPUT TYPE="HIDDEN" NAME="CountryChange" VALUE="0" >
<INPUT TYPE="HIDDEN" NAME="WDS_EncrypType_NONE" value="NONE" >
<INPUT TYPE="HIDDEN" NAME="WDS_EncrypType_WEP" value="WEP" >
<INPUT TYPE="HIDDEN" NAME="BasicRate_Value1" VALUE="15" >
<INPUT TYPE="HIDDEN" NAME="BasicRate_Value2" VALUE="3" >
<INPUT TYPE="HIDDEN" NAME="BasicRate_Value3" VALUE="351" >
<INPUT TYPE="HIDDEN" NAME="CountryRegionABand0" value="0" >
<INPUT TYPE="HIDDEN" NAME="CountryRegionABand1" value="1" >
<INPUT TYPE="HIDDEN" NAME="CountryRegionABand2" value="2" >
<INPUT TYPE="HIDDEN" NAME="CountryRegionABand3" value="3" >
<INPUT TYPE="HIDDEN" NAME="CountryRegionABand4" value="4" >
<INPUT TYPE="HIDDEN" NAME="CountryRegionABand5" value="5" >
<INPUT TYPE="HIDDEN" NAME="CountryRegionABand6" value="6" >
<INPUT TYPE="HIDDEN" NAME="CountryRegionABand7" value="7" >
<INPUT TYPE="HIDDEN" NAME="CountryRegionABand8" value="8" >
<INPUT TYPE="HIDDEN" NAME="CountryRegionABand9" value="9" >
<INPUT TYPE="HIDDEN" NAME="CountryRegionABand10" value="10" >
<INPUT TYPE="HIDDEN" NAME="CountryRegionABand11" value="11" >	
<INPUT TYPE="HIDDEN" NAME="HTMCSAUTO" value="33" >
<INPUT TYPE="HIDDEN" NAME="HTBW" value="0" >
<INPUT TYPE="HIDDEN" NAME="VHTBW" value="0" >
<INPUT TYPE="HIDDEN" NAME="RTDEVICE" value="<% tcWebApi_get("WLan11ac_Common","rt_device","s") %>" >
<% if tcWebApi_get("WLan11ac_Common","rt_device","h") = "7615" then %>
<INPUT TYPE="HIDDEN" NAME="VHTSec80Channel" value="<% tcWebApi_get("WLan11ac_Common","VHT_Sec80_Channel","s") %>" >
<INPUT TYPE="HIDDEN" NAME="WLan11acITxBfEn" value="<% tcWebApi_get("WLan11ac_Common","ITxBfEn","s") %>" >
<INPUT TYPE="HIDDEN" NAME="WLan11acETxBfEnCond" value="<% tcWebApi_get("WLan11ac_Common","ETxBfEnCond","s") %>" >
<INPUT TYPE="HIDDEN" NAME="WLan11acETxBfIncapable" value="<% tcWebApi_get("WLan11ac_Common","ETxBfIncapable","s") %>" >
<% end if %>
<% if tcWebApi_get("WebCustom_Entry","isHTBW40M","h") = "Yes" then %>
<INPUT TYPE="HIDDEN" NAME="Wlan_HTBW40M" value="<% tcWebApi_get("WLan11ac_Common","HT_BSSCoexistence","s") %>" >
<% end if %>
<INPUT TYPE="HIDDEN" NAME="Is11nMode" value="<% tcWebApi_get("WLan11ac_Common","11nMode","s") %>" >
<INPUT TYPE="HIDDEN" NAME="Is11acMode" value="<% tcWebApi_get("WLan11ac_Common","11acMode","s") %>" >														
<INPUT TYPE="HIDDEN" NAME="CountryName" value="<% tcWebApi_get("WLan11ac_Common","Country","s") %>" >
<INPUT TYPE="HIDDEN" NAME="ExtChannFlag" value="<% tcWebApi_get("WLan11ac_Common","HT_EXTCHA","s") %>" >
<INPUT TYPE="HIDDEN" NAME="BssidNum" value="<% tcWebApi_get("WLan11ac_Common","BssidNum","s") %>" >
<INPUT TYPE="HIDDEN" NAME="hCountryRegionABand" value="<% tcWebApi_get("WLan11ac_Common","CountryRegionABand","s") %>" >
<% if tcWebApi_get("WebCustom_Entry","isWPSSupported","h") = "Yes"  then %>
<INPUT TYPE="HIDDEN" NAME="WPSConfigured" value="2" >
<INPUT TYPE="HIDDEN" NAME="WpsConfModeAll" value="7" >
<INPUT TYPE="HIDDEN" NAME="WpsConfModeNone" value="0" >
<INPUT TYPE="HIDDEN" NAME="WpsStart" value="0" >
<INPUT TYPE="HIDDEN" NAME="WpsOOB" value="0" >
<INPUT TYPE="HIDDEN" NAME="WpsGenerate" value="0" >
<INPUT TYPE="HIDDEN" NAME="isInWPSing" value="<% tcWebApi_get("Info_WLan11ac","wlanWPStimerRunning","s") %>" >
<% end if %>
<INPUT TYPE="HIDDEN" NAME="maxStaNum" value="<% tcWebApi_get("Info_WLan11ac","maxStaNumSupported","s") %>" >
<INPUT TYPE="HIDDEN" NAME="isAuthenTypeSupported" value="<% if tcWebApi_get("Info_WLan11ac","isAuthenTypeSupported","h") = "Yes" then asp_write("1") else asp_write("0") end if %>" >
<INPUT TYPE="HIDDEN" NAME="isDot1XSupported" value="<% if tcWebApi_get("Info_WLan11ac","isDot1XSupported","h") = "Yes" then asp_write("1") else asp_write("0") end if %>" >
<INPUT TYPE="HIDDEN" NAME="isDot1XEnhanceSupported" value="<% if tcWebApi_get("Info_WLan11ac","isDot1XEnhanceSupported","h") = "Yes" then asp_write("1") else asp_write("0") end if %>" >
<INPUT TYPE="HIDDEN" NAME="isWDSSupported" value="<% if tcWebApi_get("Info_WLan11ac","isWDSSupported","h") = "Yes" then asp_write("1") else asp_write("0") end if %>" >
<INPUT TYPE="HIDDEN" NAME="bharti_ssid2" value="<% if tcWebApi_get("Info_WLan11ac","isbharti","h") = "Yes" then asp_write("1") else asp_write("0") end if %>" >
<INPUT TYPE="HIDDEN" NAME="wlan_VC" value="<% tcWebApi_get("WebCurSet_Entry","wlan_ac_id","s") %>" >
<INPUT TYPE="HIDDEN" NAME="isWPSSupported" value="<% if tcWebApi_get("WebCustom_Entry","isWPSSupported","h") = "Yes" then asp_write("1") else asp_write("0") end if %>" >
<INPUT TYPE="HIDDEN" NAME="isPerSSIDSupport" value="<% if tcWebApi_get("WebCustom_Entry","isPerSSIDSupport","h") = "Yes" then asp_write("1") else asp_write("0") end if %>" >
<INPUT TYPE="HIDDEN" NAME="WscV2Support" value="<% if tcWebApi_get("WLan11ac_Entry0","WscV2Support","h") = "1" then asp_write("1") else asp_write("0") end if %>" >
<INPUT TYPE="HIDDEN" NAME="hRekeyMethod" value="DISABLE" >
		<TABLE cellSpacing=0 cellPadding=0 width="100%" border=0 >
		<TBODY>
		<TR>
		<TD width=10 >&nbsp;</TD>
		<TD>
			<TABLE cellSpacing=0 cellPadding=0 border=0 >
			<TBODY>
			<TR>
			<TD width="130"><font color="#000000">无线使能:</font></TD>
			<TD>
				<INPUT TYPE="RADIO" NAME="wlan_APenable" VALUE="1" <% if tcWebApi_get("WLan11ac_Common","APOn","h") = "1" then asp_Write("checked") end if %> ><font color="#000000">开启</font>        
				<INPUT TYPE="RADIO" NAME="wlan_APenable" VALUE="0" <% if tcWebApi_get("WLan11ac_Common","APOn","h") = "0" then asp_Write("checked") end if %> <% if tcWebApi_get("WLan11ac_Common","APOn","h") = "N/A" then asp_Write("checked") end if %> ><font color="#000000">关闭</font>
			</TD>
			</TR> 
			<TR>
            <TD><font color="#000000">信道选择:</font></TD>
            <TD>
            	<SELECT NAME="Countries_Channels" SIZE="1" onChange="doRegionCheck()" >	
<SCRIPT language=javascript >
	var countryarr = new Array(98);
	countryarr[0]="ALBANIA";
	countryarr[1]="ALGERIA";
	countryarr[2]="ARGENTINA";
	countryarr[3]="ARMENIA";
	countryarr[4]="AUSTRALIA";
	countryarr[5]="AUSTRIA";
	countryarr[6]="AZERBAIJAN";
	countryarr[7]="BAHRAIN";
	countryarr[8]="BELARUS";
	countryarr[9]="BELGIUM";
	countryarr[10]="BELIZE";
	countryarr[11]="BOLIVIA";
	countryarr[12]="BRAZIL";
	countryarr[13]="BRUNEI DARUSSALAM";
	countryarr[14]="BULGARIA";
	countryarr[15]="CANADA";
	countryarr[16]="CHILE";
	countryarr[17]="CHINA";
	countryarr[18]="COLOMBIA";
	countryarr[19]="COSTA RICA";
	countryarr[20]="CROATIA";
	countryarr[21]="CYPRUS";
	countryarr[22]="CZECH REPUBLIC";
	countryarr[23]="DENMARK";
	countryarr[24]="DOMINICAN REPUBLIC";
	countryarr[25]="ECUADOR";
	countryarr[26]="EGYPT";
	countryarr[27]="ELSALVADOR";
	countryarr[28]="FINLAND";
	countryarr[29]="FRANCE";
	countryarr[30]="GEORGIA";
	countryarr[31]="GERMANY";
	countryarr[32]="GREECE";
	countryarr[33]="GUATEMALA";
	countryarr[34]="HONDURAS";
	countryarr[35]="HONGKONG";
	countryarr[36]="HUNGARY";
	countryarr[37]="ICELAND";
	countryarr[38]="INDIA";
	countryarr[39]="INDONESIA";
	countryarr[40]="IRAN";
	countryarr[41]="IRELAND";
	countryarr[42]="ISRAEL";
	countryarr[43]="ITALY";
	countryarr[44]="JAPAN";
	countryarr[45]="KAZAKHSTAN";
	countryarr[46]="KOREA DEMOCRATIC";
	countryarr[47]="KOREA REPUBLIC";
	countryarr[48]="LATVIA";
	countryarr[49]="LEBANON";
	countryarr[50]="LIECHTENSTEIN";
	countryarr[51]="LITHUANIA";
	countryarr[52]="LUXEMBOURG";
	countryarr[53]="MACAU";
	countryarr[54]="MACEDONIA";
	countryarr[55]="MALAYSIA";
	countryarr[56]="MEXICO";
	countryarr[57]="MONACO";
	countryarr[58]="MOROCCO";
	countryarr[59]="NETHERLANDS";
	countryarr[60]="NEW ZEALAND";
	countryarr[61]="NORWAY";
	countryarr[62]="OMAN";
	countryarr[63]="PAKISTAN";
	countryarr[64]="PANAMA";
	countryarr[65]="PERU";
	countryarr[66]="PHILIPPINES";
	countryarr[67]="POLAND";
	countryarr[68]="PORTUGAL";
	countryarr[69]="PUERTO RICO";
	countryarr[70]="QATAR";
	countryarr[71]="ROMANIA";
	countryarr[72]="RUSSIA";
	countryarr[73]="SAUDI ARABIA";
	countryarr[74]="SINGAPORE";
	countryarr[75]="SLOVAKIA";
	countryarr[76]="SLOVENIA";
	countryarr[77]="SOUTH AFRICA";
	countryarr[78]="SPAIN";
	countryarr[79]="SWEDEN";
	countryarr[80]="SWITZERLAND";
	countryarr[81]="SYRIAN ARAB REPUBLIC";
	countryarr[82]="TAIWAN";
	countryarr[83]="THAILAND";
	countryarr[84]="TRINIDAD AND TOBAGO";
	countryarr[85]="TUNISIA";
	countryarr[86]="TURKEY";
	countryarr[87]="UKRAINE";
	countryarr[88]="UNITED ARAB EMIRATES";
	countryarr[89]="UNITED KINGDOM";
	countryarr[90]="UNITED STATES";
	countryarr[91]="URUGUAY";
	countryarr[92]="UZBEKISTAN";
	countryarr[93]="VENEZUELA";
	countryarr[94]="VIET NAM";
	countryarr[95]="YEMEN";
	countryarr[96]="ZIMBABWE";
	countryarr[97]="Undefined";
	
	for(i=0;i<98;i++)
	{
		if(document.ConfigForm.CountryName.value.match(countryarr[i]) != null)
		{
			document.ConfigForm.Countries_Channels[i]=new Option(countryarr[i],countryarr[i],false,true);
			document.ConfigForm.Countries_Channels[i].selected=true;
		}
		else
		{
			if(countryarr[i].match("TAIWAN") !=null){
				document.ConfigForm.Countries_Channels[i]=new Option(countryarr[i],countryarr[i],true,false);
			}
			else
			{
				document.ConfigForm.Countries_Channels[i]=new Option(countryarr[i],countryarr[i],false,false);
				document.ConfigForm.Countries_Channels[i].selected=false;
			}
		}
	}	
</SCRIPT>				
				</SELECT> 
				<SELECT NAME="Channel_ID" SIZE="1" onChange="doExtChaLockChange()"></SELECT>
			</TD>
			</TR>
			<TR>
			<TD><font color="#000000">当前信道:</font></TD> 
			<TD>
<% if tcWebApi_get("WLan11ac_Common","Channel","h") = "0" then %>
				<INPUT TYPE="TEXT" NAME="CurrentChannel" STYLE="WIDTH:62" MAXLENGTH="2" VALUE="<% If tcWebApi_get("Info_WLan11ac","CurrentChannel","h") <> "N/A" then tcWebApi_get("Info_WLan11ac","CurrentChannel","s") end if %>" disabled >
<% else %>
				<INPUT TYPE="TEXT" NAME="CurrentChannel" STYLE="WIDTH:62" MAXLENGTH="2" VALUE="<% If tcWebApi_get("WLan11ac_Common","Channel","h") <> "N/A" then tcWebApi_get("WLan11ac_Common","Channel","s") end if %>" disabled >
<% end if %>
			</TD>
			</TR>
			<TR>
			<TD><font color="#000000">信标间隔:</font></TD>
			<TD><INPUT TYPE="TEXT" NAME="BeaconInterval" STYLE="WIDTH:62" MAXLENGTH="4" VALUE="<% If tcWebApi_get("WLan11ac_Common","BeaconPeriod","h") <> "N/A" then tcWebApi_get("WLan11ac_Common","BeaconPeriod","s") else asp_Write("100") end if %>"> <font color="#000000">(范围: 20~1000)</font></TD>
			</TR> 
			<TR>
			<TD><font color="#000000">RTS/CTS界限:</font></TD>
			<TD><INPUT TYPE="TEXT" NAME="RTSThreshold" STYLE="WIDTH:62" MAXLENGTH="5" VALUE="<% If tcWebApi_get("WLan11ac_Common","RTSThreshold","h") <> "N/A" then tcWebApi_get("WLan11ac_Common","RTSThreshold","s") else asp_Write("2347") end if %>"> <font color="#000000">(范围: 1500~2347)</font></TD>
			</TR> 
			<TR>
			<TD><font color="#000000">分割界限:</font></TD>
			<TD><INPUT TYPE="TEXT" NAME="FragmentThreshold" STYLE="WIDTH:62" MAXLENGTH="5" VALUE="<% If tcWebApi_get("WLan11ac_Common","FragThreshold","h") <> "N/A" then tcWebApi_get("WLan11ac_Common","FragThreshold","s") else asp_Write("2346") end if %>"> <font color="#000000">(范围: 256~2346, 仅偶数)</font></TD>
			</TR> 
			<TR>
			<TD><font color="#000000">DTIM间隔:</font></TD>
			<TD><INPUT TYPE="TEXT" NAME="DTIM" STYLE="WIDTH:62" MAXLENGTH="5" VALUE="<% If tcWebApi_get("WLan11ac_Common","DtimPeriod","h") <> "N/A" then tcWebApi_get("WLan11ac_Common","DtimPeriod","s") else asp_Write("1") end if %>" > <font color="#000000">(范围: 1~255)</font></TD>
			</TR> 
			<TR>
			<TD><font color="#000000">模式选择:</font></TD>
			<TD>
				<SELECT NAME="WirelessMode" SIZE="1" onChange="doWirelessModeChange()">
				<OPTION value="2" <% if tcWebApi_get("WLan11ac_Common","WirelessMode","h") = "2" then asp_Write("selected") end if %> >11a only</OPTION>
				<OPTION value="8" <% if tcWebApi_get("WLan11ac_Common","WirelessMode","h") = "8" then asp_Write("selected") end if %> >11a/n mixed mode</OPTION>
				<OPTION value="14" <% if tcWebApi_get("WLan11ac_Common","WirelessMode","h") = "14" then asp_Write("selected") end if %> >11vht AC/AN/A</OPTION>
				<OPTION value="15" <% if tcWebApi_get("WLan11ac_Common","WirelessMode","h") = "15" then asp_Write("selected") end if %> <% If tcWebApi_get("WLan11ac_Common","WirelessMode","h") = "N/A" then asp_Write("checked") end if%> >11vht AC/AN</OPTION>
				</SELECT>
			</TD>
			</TR> 
<% If tcWebApi_get("WebCustom_Entry","isMaxStaNumSupported","h") = "Yes" Then %>																	
			<TR>
			<TD><font color="#000000">电台编号:</font></TD>
			<TD>
				<INPUT NAME="StationNum" TYPE="text" STYLE="WIDTH:62" value="<% if tcWebApi_get("WLan11ac_Entry","MaxStaNum","h") <> "N/A" then tcWebApi_get("WLan11ac_Entry","MaxStaNum","s") else asp_Write("0") end if %>" MAXLENGTH="2" onblur="value=isNumeric(value)?value:'0';">
				<font color="#000000">(范围: 0~</font><% tcWebApi_get("Info_WLan11ac","maxStaNumSupported","s") %><font color="#000000">, 0表示没有限制)</font>
			</TD>
			</TR> 
<% End If %>											        
			</TBODY>
			</TABLE><BR>			
			<div id="11nMode_1_div">
			<TABLE cellSpacing=0 cellPadding=0 border=0 >
			<TBODY>
  			<TR>
            <TD width="130"><font color="#000000">信道带宽:</font></TD>
            <TD>
            	<select name="WLANChannelBandwidth" STYLE="WIDTH:105" SIZE="1" onChange="doChannelBandwidthChange();" >
  				<option value="0" <% if tcWebApi_get("WLan11ac_Common","HT_BW","h") = "0" then asp_Write("selected") end if %> >20 MHz</option>
<% if tcWebApi_get("WebCustom_Entry","isHTBW40M","h") = "Yes" then %>
				<option value="1" 
<% If tcWebApi_get("WLan11ac_Common","HT_BSSCoexistence","h") = "1" Then %>
					<% if tcWebApi_get("WLan11ac_Common","HT_BW","h") <> "0" then asp_Write("selected") end if %>
<% End If %>
					<% if tcWebApi_get("WLan11ac_Common","HT_BW","h") = "N/A" then asp_Write("selected") end if %> >20/40 MHz
				</option>
				<option value="2" 
<% If tcWebApi_get("WLan11ac_Common","HT_BW","h") = "1" Then %>
					<% if tcWebApi_get("WLan11ac_Common","HT_BSSCoexistence","h") = "0" then asp_Write("selected") end if %>
<% End If %>
					>40 MHz
				</option>
<% else %>
				<option value="1" <% if tcWebApi_get("WLan11ac_Common","HT_BW","h") = "1" then asp_Write("selected") end if %><% if tcWebApi_get("WLan11ac_Common","HT_BW","h") = "N/A" then asp_Write("selected") end if %> >20/40 MHz</option>
<% end if %>
				</select>
        	</TD>
      		</TR>
	        </TBODY>
			</TABLE> 
			<div id="HT_BW_1_div">
			<TABLE cellSpacing=0 cellPadding=0 border=0 >
			<TBODY>
			<TR>
			<TD width="130"><font color="#000000">扩展信道:</font></TD>
			<TD>
				<select name="WLANExtensionChannel" SIZE="1" onChange="doExtChannChange();">
				<option value="0" <% if tcWebApi_get("WLan11ac_Common","HT_EXTCHA","h") = "0" then asp_Write("selected") end if %><% if tcWebApi_get("WLan11ac_Common","HT_EXTCHA","h") = "N/A" then asp_Write("selected") end if %> >向下扩展信道</option>
				<option value="1" <% if tcWebApi_get("WLan11ac_Common","HT_EXTCHA","h") = "1" then asp_Write("selected") end if %> >向上扩展信道</option>
				</select>
			</TD>
			</TR>
			</TBODY>
			</TABLE> 
			</div>
	        <TABLE cellSpacing=0 cellPadding=0 border=0 >
			<TBODY>  
			<TR>
			<TD width="130"><font color="#000000">保护间隔:</font></TD>
			<TD>
				<select name="WLANGuardInterval" STYLE="WIDTH:105" SIZE="1" >
				<option value="0" <% if tcWebApi_get("WLan11ac_Common","HT_GI","h") = "0" then asp_Write("selected") end if %> >800 nsec</option>
				<option value="1" <% if tcWebApi_get("WLan11ac_Common","HT_GI","h") = "1" then asp_Write("selected") end if %><% if tcWebApi_get("WLan11ac_Common","HT_GI","h") = "N/A" then asp_Write("selected") end if %> >AUTO</option>
				</select>
			</TD>
			</TR>	
			<TR>
			<TD><font color="#000000">MCS:</font></TD>
			<TD><select name="WLANMCS" STYLE="WIDTH:105" SIZE="1" ></select></TD>
			</TR>	  		            		            		            		            	        
			</TBODY>
			</TABLE>
			</div><BR>			
			<div id="11acMode_1_div">
			<TABLE cellSpacing=0 cellPadding=0 border=0 >
			<TBODY>
			<TR>
			<TD width="130"><font color="#000000">VHT带宽:</font></TD>
			<TD>
				<select name="WLan11acVHTChannelBandwidth" SIZE="1" onChange="doVHTBandwidthChange();">
				<option value="0" <% if tcWebApi_get("WLan11ac_Common","VHT_BW","h") = "0" then asp_Write("selected") end if %> >20/40 MHz</option>
				<option value="1" <% if tcWebApi_get("WLan11ac_Common","VHT_BW","h") = "1" then asp_Write("selected") end if %><% if tcWebApi_get("WLan11ac_Common","VHT_BW","h") = "N/A" then asp_Write("selected") end if %> >20/40/80 MHz</option>
<% if tcWebApi_get("WLan11ac_Common","rt_device","h") = "7615" then %>
				<option value="2" <% if tcWebApi_get("WLan11ac_Common","VHT_BW","h") = "2" then asp_Write("selected") end if %><% if tcWebApi_get("WLan11ac_Common","VHT_BW","h") = "N/A" then asp_Write("selected") end if %> >160 MHz</option>
				<option value="3" <% if tcWebApi_get("WLan11ac_Common","VHT_BW","h") = "3" then asp_Write("selected") end if %><% if tcWebApi_get("WLan11ac_Common","VHT_BW","h") = "N/A" then asp_Write("selected") end if %> >80+80 MHz</option>
<% end if %>
				</select>
			</TD>
			</TR>
			</TBODY>
			</TABLE>
<% If tcWebApi_get("WLan11ac_Common","rt_device","h") = "7615" Then %>
			<div id="WLan11ac2ndFrequencyTable" style="display:none">
			<TABLE cellSpacing=0 cellPadding=0 border=0 >
			<TBODY>
			<TR>
			<TD width="130"><font color="#000000">第二频率(信道):</font></TD>
			<TD>
				<select name="WLan11ac2ndFrequency" SIZE="1" onChange="onVHTbwChannelChange();"></select>
			</TD>
			</TR>
			<TR>
			<TD><font color="#000000">固定发送波束成形:</font></TD>
			<TD>
				<select name="WLan11acTxBeamForming" STYLE="WIDTH:109" SIZE="1">
		    	<option value="3">Both</option>
				<option value="2">Explicit TxBF</option>
				<option value="1">Implicit TxBF</option>
				<option value="0">Disable</option>
			  </select>
			</TD>
			</TR>
			</TBODY>
			</TABLE>
			</div>
<% End If %>
			<TABLE cellSpacing=0 cellPadding=0 border=0 >
			<TBODY>
			<TR>
			<TD width="130"><font color="#000000">VHT保护间隔:</font></TD>
			<TD>
				<select name="WLan11acVHTGuardInterval" STYLE="WIDTH:109" SIZE="1">
				<option value="0" <% if tcWebApi_get("WLan11ac_Common","VHT_SGI","h") = "0" then asp_Write("selected") end if %>>800 nsec</option>
				<option value="1" <% if tcWebApi_get("WLan11ac_Common","VHT_SGI","h") = "1" then asp_Write("selected") end if %><% if tcWebApi_get("WLan11ac_Common","VHT_SGI","h") = "N/A" then asp_Write("selected") end if %>>AUTO</option>
				</select>
			</TD>
			</TR>  		            		            		            		            	        
			</TBODY>
			</TABLE>
			</div><BR>		
<% if tcWebApi_get("WLan11ac_Common","BssidNum","h") = "1" then %>
			<TABLE cellSpacing=0 cellPadding=0 border=0 >
			<TBODY>
			<TR>
			<TD width="130"><font color="#000000">SSID索引:</font></TD>
			<TD>
				<select NAME="SSID_INDEX" SIZE="1" onChange="doSSIDChange()">
				<OPTION value="0" <% if TCWebApi_get("WebCurSet_Entry","wlan_ac_id","h") = "0" then asp_write("selected") end if %> >1</OPTION>
				</select>
			</TD>
			</TR>
			</TBODY>
			</TABLE>
<% elseif tcWebApi_get("WLan11ac_Common","BssidNum","h") = "2" then %>
			<TABLE cellSpacing=0 cellPadding=0 border=0 >
			<TBODY>
			<TR>
			<TD width="130"><font color="#000000">SSID索引:</font></TD>
			<TD>
				<select NAME="SSID_INDEX" SIZE="1" onChange="doSSIDChange()">
				<OPTION value="0" <% if TCWebApi_get("WebCurSet_Entry","wlan_ac_id","h") = "0" then asp_write("selected") end if %> >1</OPTION>
<% if tcWebApi_get("GUI_Entry0","ssidOneOnly","h") <> "support" then %>
<% if tcWebApi_get("WebCustom_Entry","isFonSupport","h") <> "Yes" then %>
				<OPTION value="1" <% if TCWebApi_get("WebCurSet_Entry","wlan_ac_id","h") = "1" then asp_write("selected") end if %> >2</OPTION>
<% end if %>
<% end if %>
				</select>
			</TD>
			</TR>
			</TBODY>
			</TABLE>
<% elseif tcWebApi_get("WLan11ac_Common","BssidNum","h") = "3" then %>
			<TABLE cellSpacing=0 cellPadding=0 border=0 >
			<TBODY>
			<TR>
			<TD width="130"><font color="#000000">SSID索引:</font></TD>
			<TD>
				<select NAME="SSID_INDEX" SIZE="1" onChange="doSSIDChange()">
				<OPTION value="0" <% if TCWebApi_get("WebCurSet_Entry","wlan_ac_id","h") = "0" then asp_write("selected") end if %> >1</OPTION>
<% if tcWebApi_get("GUI_Entry0","ssidOneOnly","h") <> "support" then %>
<% if tcWebApi_get("WebCustom_Entry","isFonSupport","h") <> "Yes" then %>
				<OPTION value="1" <% if TCWebApi_get("WebCurSet_Entry","wlan_ac_id","h") = "1" then asp_write("selected") end if %> >2</OPTION>
<% end if %>
				<OPTION value="2" <% if TCWebApi_get("WebCurSet_Entry","wlan_ac_id","h") = "2" then asp_write("selected") end if %> >3</OPTION>
<% end if %>
				</select>
			</TD>
			</TR>
			</TBODY>
			</TABLE>
<% elseif tcWebApi_get("WLan11ac_Common","BssidNum","h") = "4" then %>
			<TABLE cellSpacing=0 cellPadding=0 border=0 >
			<TBODY>
			<TR>
			<TD width="130"><font color="#000000">SSID索引:</font></TD>
			<TD>
				<select NAME="SSID_INDEX" SIZE="1" onChange="doSSIDChange()">
				<OPTION value="0" <% if TCWebApi_get("WebCurSet_Entry","wlan_ac_id","h") = "0" then asp_write("selected") end if %> >1</OPTION>
<% if tcWebApi_get("GUI_Entry0","ssidOneOnly","h") <> "support" then %>
<% if tcWebApi_get("WebCustom_Entry","isFonSupport","h") <> "Yes" then %>
				<OPTION value="1" <% if TCWebApi_get("WebCurSet_Entry","wlan_ac_id","h") = "1" then asp_write("selected") end if %> >2</OPTION>
<% end if %>
				<OPTION value="2" <% if TCWebApi_get("WebCurSet_Entry","wlan_ac_id","h") = "2" then asp_write("selected") end if %> >3</OPTION>
				<OPTION value="3" <% if TCWebApi_get("WebCurSet_Entry","wlan_ac_id","h") = "3" then asp_write("selected") end if %> >4</OPTION>
<% end if %>
				</select>
			</TD>
			</TR>
			</TBODY>
			</TABLE>
<% end if %>
<% if tcWebApi_get("WebCustom_Entry","isPerSSIDSupport","h") = "Yes"  then %>
			<TABLE cellSpacing=0 cellPadding=0 border=0 >
			<TBODY>
			<TR>
			<TD width="130"><font color="#000000">PerSSID使能:</font></TD>
			<TD>
				<INPUT TYPE="RADIO" NAME="ESSID_Enable_Selection" VALUE="1" <% If tcWebApi_get("WLan11ac_Entry","EnableSSID","h") = "1" then asp_Write("checked") end if %> ><font color="#000000">开启</font> 
				<INPUT TYPE="RADIO" NAME="ESSID_Enable_Selection" VALUE="0" <% If tcWebApi_get("WLan11ac_Entry","EnableSSID","h") = "0" then asp_Write("checked") end if %> <% If tcWebApi_get("WLan11ac_Entry","EnableSSID","h") = "N/A" then asp_Write("checked") end if %> ><font color="#000000">关闭</font>
			</TD>
			</TR>
			</TBODY>
			</TABLE>
<% end if %>						
			<TABLE cellSpacing=0 cellPadding=0 border=0 >
			<TBODY>						
			<TR>
			<TD width="130"><font color="#000000">SSID:</font></TD>
			<TD><INPUT TYPE="TEXT" NAME="ESSID" MAXLENGTH="32" VALUE="<% If tcWebApi_get("WLan11ac_Entry","SSID","h") <> "N/A" then tcWebApi_get("WLan11ac_Entry","SSID","s") end if %>"></TD>
			</TR>
			<TR>
			<TD><font color="#000000">广播使能:</font></TD>
			<TD>
				<INPUT TYPE="RADIO" NAME="ESSID_HIDE_Selection" VALUE="0" <% If tcWebApi_get("WLan11ac_Entry","HideSSID","h") = "0" then asp_Write("checked") end if %> ><font color="#000000">开启</font>		
				<INPUT TYPE="RADIO" NAME="ESSID_HIDE_Selection" VALUE="1" onClick="doBroadcastSSIDChange();" <% If tcWebApi_get("WLan11ac_Entry","HideSSID","h") = "1" then asp_Write("checked") end if %> <% If tcWebApi_get("WLan11ac_Entry","HideSSID","h") = "N/A" then asp_Write("checked") end if %> ><font color="#000000">关闭</font>
			</TD>
			</TR>
			</TBODY>
			</TABLE>
			<div id="11nMode_0_div">
			<TABLE cellSpacing=0 cellPadding=0 border=0 >
			<TBODY>
			<TR>
			<TD width="130"><font color="#000000">WMM使能:</font></TD>
			<TD>
				<INPUT TYPE="RADIO" NAME="WMM_Selection" VALUE="1" <% If tcWebApi_get("WLan11ac_Entry","WMM","h") = "1" then asp_Write("checked") end if %> ><font color="#000000">开启</font>		
				<INPUT TYPE="RADIO" NAME="WMM_Selection" VALUE="0" <% If tcWebApi_get("WLan11ac_Entry","WMM","h") = "0" then asp_Write("checked") end if %> <% If tcWebApi_get("WLan11ac_Entry","WMM","h") = "N/A" then asp_Write("checked") end if %> ><font color="#000000">关闭</font>
			</TD>
			</TR>
			</TBODY>
			</TABLE>
			</div>
<% if tcWebApi_get("WebCustom_Entry","isWPSSupported","h") = "Yes"  then %>
<% if tcWebApi_get("WebCurSet_Entry","wlan_ac_id","h") = "0" then %>
			<TABLE cellSpacing=0 cellPadding=0 border=0 >
			<TBODY>
			<TR>
			<TD width="130"><font color="#000000">WPS使能:</font></TD>
			<TD>
				<input name="UseWPS_Selection" VALUE="1" <% If tcWebApi_get("WLan11ac_Entry","WPSConfMode","h") <> "0" then asp_Write("checked") end if %> onClick="doWPSUseChange();" type="radio"><font color="#000000">开启</font>
				<input name="UseWPS_Selection" VALUE="0" <% If tcWebApi_get("WLan11ac_Entry","WPSConfMode","h") = "0" then asp_Write("checked") end if %> <% If tcWebApi_get("WLan11ac_Entry","WPSConfMode","h") = "N/A" then asp_Write("checked") end if %> onClick="doWPSUseChange();" type="radio"><font color="#000000">关闭</font>
			</TD>
			</TR>  	  		            		            		            		            	        
			</TBODY>
			</TABLE>
			<div id="WPSConfMode_1_div">	
			<TABLE cellSpacing=0 cellPadding=0 border=0 >
			<TBODY>
			<TR>
			<TD width="130"><font color="#000000">WPS状态:</font></TD>
			<TD>
				<% if tcWebApi_get("WLan11ac_Entry", "WPSConfStatus", "h") = "1" then asp_Write("未配置")
					elseif tcWebApi_get("WLan11ac_Entry", "WPSConfStatus", "h") = "2" then asp_Write("已配置")
					elseif tcWebApi_get("WLan11ac_Entry", "WPSConfStatus", "h") = "N/A" then asp_Write("未配置") end if %>
			</TD>
			</TR>
			<TR>
			<TD><font color="#000000">WPS模式:</font></TD>
			<TD>
				<input name="WPSMode_Selection" value="0" onClick="doWPSModeChange();" <% If tcWebApi_get("WLan11ac_Entry","WPSMode","h") = "0" then asp_Write("checked") end if %> type="radio">PIN码
				<input name="WPSMode_Selection" value="1" onClick="doWPSModeChange();" <% If tcWebApi_get("WLan11ac_Entry","WPSMode","h") = "1" then asp_Write("checked") end if %> <% If tcWebApi_get("WLan11ac_Entry","WPSMode","h") = "N/A" then asp_Write("checked") end if %>  type="radio">PBC
			</TD>
			</TR>
<% if tcWebApi_get("WLan11ac_Entry","WPSMode","h") = "0" then %>
			<TD><font color="#000000">AP self PIN code:</font></TD>
			<TD>
				<% tcWebApi_get("Info_WLan11ac", "wlanSelfPinCode", "s") %>&nbsp;&nbsp;
				<input type="button" name="pin_generate" value="Generate" onClick="doGenerate()">
			</TD>
			</TR>
			<TD><font color="#000000">enrollee PIN code:</font></TD>
			<TD><input name="WPSEnrolleePINCode" size="9" maxlength="9" value="<% If tcWebApi_get("WLan11ac_Entry","enrolleePinCode","h") = "N/A" then asp_Write("") else TCWebApi_get("WLan11ac_Entry","enrolleePinCode","s") end if %>" onblur="doPINCodeCheck(this)" type="text"></TD>
			</TR>
<% end if %>
			<TR>
			<TD></TD>
			<TD><input name="StartWPS" value="<% IF tcWebApi_get("Info_WLan11ac","wlanWPStimerRunning","h") = "0" then asp_Write("Start WPS") end if %> <% IF tcWebApi_get("Info_WLan11ac","wlanWPStimerRunning","h") = "1" then asp_Write("Stop WPS") end if %><% IF tcWebApi_get("Info_WLan11ac","wlanWPStimerRunning","h") = "N/A" then asp_Write("Start WPS") end if %>" onclick="doStartWPS();" type="button"></TD>
			</TR>
			<TR>  		
			<TD><font color="#000000">WPS进度:</font></TD>
			<TD>
				<% if tcWebApi_get("Info_WLan11ac", "wlanWPSStatus", "h") = "Idle" then asp_Write("Idle")
					elseif tcWebApi_get("Info_WLan11ac", "wlanWPSStatus", "h") = "In progress" then asp_Write("Inprogress")
					elseif tcWebApi_get("Info_WLan11ac", "wlanWPSStatus", "h") = "Configured" then asp_Write("Configured")
					elseif tcWebApi_get("Info_WLan11ac", "wlanWPSStatus", "h") = "WPS process Fail" then asp_Write("WPS process Fail")
					else tcWebApi_get("Info_WLan11ac", "wlanWPSStatus", "s") end if%>
			</TD>
			</TR>
			<TR>
			<TD></TD>
			<TD><input name="ResetOOB" value="Reset to OOB" onclick="doResetOOB();" type="button" ></TD>
			</TR>
			</TBODY>
			</TABLE>
			</div><BR>
<% end if %>
<% end if %> 
			<TABLE cellSpacing=0 cellPadding=0 border=0 >
			<TBODY>
<% if tcWebApi_get("Info_WLan11ac","isDot1XSupported","h") = "Yes" then %>							
			<TR>
			<TD width="130"><font color="#000000">安全设置:</font></TD>
			<TD>
				<SELECT NAME="WEP_Selection" SIZE="1" onChange="doWEPChange()">
<% if tcWebApi_get("Info_WLan11ac","isbharti","h") = "Yes" then %>
				<OPTION value="OPEN" <% if tcWebApi_get("WLan11ac_Entry","AuthMode","h") = "OPEN" then asp_Write("selected") elseif tcWebApi_get("WLan11ac_Entry","AuthMode","h") = "N/A" then asp_Write("selected") end if %> >OPEN</OPTION>
<% elseif tcWebApi_get("WebCustom_Entry","isWlanEncrypt","h") = "Yes"  then %>
<% else %>
				<OPTION value="OPEN" <% if tcWebApi_get("WLan11ac_Entry","AuthMode","h") = "OPEN" then asp_Write("selected") elseif tcWebApi_get("WLan11ac_Entry","AuthMode","h") = "N/A" then asp_Write("selected") end if %> >OPEN</OPTION>
<%  end if %>
				<OPTION value="WEP-64Bits" <% if tcWebApi_get("WLan11ac_Entry","AuthMode","h") = "WEP-64Bits" then asp_Write("selected") end if %> >WEP-64Bits</OPTION>
				<OPTION value="WEP-128Bits" <% if tcWebApi_get("WLan11ac_Entry","AuthMode","h") = "WEP-128Bits" then asp_Write("selected") end if %> >WEP-128Bits</OPTION>
				<OPTION value="WPAPSK" <% if tcWebApi_get("WLan11ac_Entry","AuthMode","h") = "WPAPSK" then asp_Write("selected") end if %>  >WPAPSK</OPTION>
				<OPTION value="WPA2PSK" <% if tcWebApi_get("WLan11ac_Entry","AuthMode","h") = "WPA2PSK" then asp_Write("selected") end if %> >WPA2PSK</OPTION>
				<OPTION value="WPAPSKWPA2PSK" <% if tcWebApi_get("WLan11ac_Entry","AuthMode","h") = "WPAPSKWPA2PSK" then asp_Write("selected") end if %> >WPAPSKWPA2PSK</OPTION>				
				<OPTION value="Radius-WEP64" <% if tcWebApi_get("WLan11ac_Entry","AuthMode","h") = "Radius-WEP64" then asp_Write("selected") end if %> >Radius-WEP64</OPTION>
				<OPTION value="Radius-WEP128" <% if tcWebApi_get("WLan11ac_Entry","AuthMode","h") = "Radius-WEP128" then asp_Write("selected") end if %> >Radius-WEP128</OPTION>
				<OPTION value="WPA" <% if tcWebApi_get("WLan11ac_Entry","AuthMode","h") = "WPA" then asp_Write("selected") end if %> >WPA</OPTION>
				<OPTION value="WPA2" <% if tcWebApi_get("WLan11ac_Entry","AuthMode","h") = "WPA2" then asp_Write("selected") end if %> >WPA2</OPTION>
				<OPTION value="WPA1WPA2" <% if tcWebApi_get("WLan11ac_Entry","AuthMode","h") = "WPA1WPA2" then asp_Write("selected") end if %> >WPA1WPA2</OPTION>
				</SELECT>
				<select name="WEP_Selection_show" SIZE="1" >
				<option <% if tcWebApi_get("Info_WLan11ac","isAuthenTypeSupported","h") = "Yes" then asp_Write("selected") elseif tcWebApi_get("Info_WLan11ac","isAuthenTypeSupported","h") = "N/A" then asp_Write("selected") end if %> >WPA2</option>
				</select>
			</TD>
			</TR>
<% else %>  	
			<TR>
			<TD width="130"><font color="#000000">安全设置:</font></TD>
			<TD>
				<SELECT NAME="WEP_Selection" SIZE="1" onChange="doWEPChange()">
<% if tcWebApi_get("Info_WLan11ac","isbharti","h") <> "Yes" then %>
<% if tcWebApi_get("WebCustom_Entry","isWlanEncrypt","h") = "Yes"  then %>	
<% else %>
				<OPTION value="OPEN" <% if tcWebApi_get("WLan11ac_Entry","AuthMode","h") = "OPEN" then asp_Write("selected") elseif tcWebApi_get("WLan11ac_Entry","AuthMode","h") = "N/A" then asp_Write("selected") end if %>>OPEN</OPTION>
<% end if %>	
<% end if %>
				<OPTION value="WEP-64Bits" <% if tcWebApi_get("WLan11ac_Entry","AuthMode","h") = "WEP-64Bits" then asp_Write("selected") end if %>>WEP-64Bits</OPTION>
				<OPTION value="WEP-128Bits" <% if tcWebApi_get("WLan11ac_Entry","AuthMode","h") = "WEP-128Bits" then asp_Write("selected") end if %>>WEP-128Bits</OPTION>
				<OPTION value="WPAPSK" <% if tcWebApi_get("WLan11ac_Entry","AuthMode","h") = "WPAPSK" then asp_Write("selected") end if %>>WPAPSK</OPTION>
				<OPTION value="WPA2PSK" <% if tcWebApi_get("WLan11ac_Entry","AuthMode","h") = "WPA2PSK" then asp_Write("selected") end if %>>WPA2PSK</OPTION>
				<OPTION value="WPAPSKWPA2PSK" <% if tcWebApi_get("WLan11ac_Entry","AuthMode","h") = "WPAPSKWPA2PSK" then asp_Write("selected") end if %>>WPAPSKWPA2PSK</OPTION>
				</SELECT>
			</TD>
			</TR>
<% end if %>						  	  		  		            		            		            		            	        
			</TBODY>
			</TABLE><BR>				
<% if tcWebApi_get("Info_WLan11ac","isDot1XSupported","h") = "Yes" then %>						
			<div id="Radius-WEP64_div">  	            	
			<TABLE cellSpacing=0 cellPadding=0 border=0 >
			<TBODY>
			<TR>
			<TD width="130"><font color="#000000">Radius Server IP:</font></TD>
			<TD><input name="radiusSVR_IP1" type="text" value="<% if tcWebApi_get("WLan11ac_Entry","RADIUS_Server","h") <> "N/A" then tcWebApi_get("WLan11ac_Entry","RADIUS_Server","s") end if %>" size="20" maxlength="15"></TD>
			</TR>
			<TR>
			<TD><font color="#000000">Radius Server Port:</font></TD>
			<TD><input name="radiusSVR_Port1" type="text" value="<% if tcWebApi_get("WLan11ac_Entry","RADIUS_Port","h") <> "N/A" then tcWebApi_get("WLan11ac_Entry","RADIUS_Port","s") end if %>" size="8" maxlength="5"></TD>
			</TR>
			<TR>
			<TD><font color="#000000">Radius Share secret:</font></TD>
			<TD><input name="radiusSVR_Key1" type="text" value="<% if tcWebApi_get("WLan11ac_Entry","RADIUS_Key","h") <> "N/A" then tcWebApi_get("WLan11ac_Entry","RADIUS_Key","s") end if %>" size="48" maxlength="63"></TD>
			</TR>
<% if tcWebApi_get("Info_WLan11ac","isDot1XEnhanceSupported","h") = "Yes" then %>						
			<TR>
			<TD><font color="#000000">Backup Server IP:</font></TD>
			<TD><input name="bakradiusSVR_IP1" type="text" value="<% if tcWebApi_get("WLan11ac_Entry","BAK_RADIUS_Server","h") <> "N/A" then tcWebApi_get("WLan11ac_Entry","BAK_RADIUS_Server","s") end if %>" size="20" maxlength="15"></TD>
			</TR> 
			<TR>
			<TD><font color="#000000">Backup Server Port:</font></TD>
			<TD><input name="bakradiusSVR_Port1" type="text" value="<% if tcWebApi_get("WLan11ac_Entry","BAK_RADIUS_Port","h") <> "N/A" then tcWebApi_get("WLan11ac_Entry","BAK_RADIUS_Port","s") end if %>" size="8" maxlength="5"> </TD>
			</TR>
			<TR>
			<TD><font color="#000000">Backup Share secret:</font></TD>
			<TD><input name="bakradiusSVR_Key1" type="text" value="<% if tcWebApi_get("WLan11ac_Entry","BAK_RADIUS_Key","h") <> "N/A" then tcWebApi_get("WLan11ac_Entry","BAK_RADIUS_Key","s") end if %>" size="48" maxlength="63"></TD>
			</TR> 
<% end if %>						
			<TR>
			<TD><font color="#000000">Re-auth Interval:</font></TD>
			<TD><input type="text" name="radiusSVR_ReAuthInter1" value="<% if tcWebApi_get("WLan11ac_Common","session_timeout_interval","h") <> "N/A" then tcWebApi_get("WLan11ac_Common","session_timeout_interval","s") end if %>" size="48" maxlength="63">seconds</TD>
			</TR>
			<TR>
			<TD><font color="#000000">WEP-64Bits:</font></TD>
			<TD><font color="#000000">每个键请输入(1)5个字符或(2)10个字符,从0~9, a, b, c, d, e, f.</font></TD>
			</TR>
			<TR>
			<TD><font color="#000000">WEP-128Bits:</font></TD>
			<TD><font color="#000000">每个键请输入(1)13个字符或(2)26个字符,从0~9, a, b, c, d, e, f.</font></TD>
			</TR> 
			<TR>
			<TD>
				<INPUT TYPE="RADIO" NAME="DefWEPKey1" VALUE="1" <% If tcWebApi_get("WLan11ac_Entry","DefaultKeyID","h") = "1" then asp_Write("checked") end if%> <% If tcWebApi_get("WLan11ac_Entry","DefaultKeyID","h") = "N/A" then asp_Write("checked") end if %> >
				<font color="#000000">Key</font>#1:</TD>
			<TD><INPUT TYPE="TEXT" NAME="WEP_Key11" SIZE="30" MAXLENGTH="28" VALUE="<% If tcWebApi_get("WLan11ac_Entry","Key1Str","h") <> "N/A" then tcWebApi_get("WLan11ac_Entry","Key1Str","s") end if %>" onBlur="doKEYcheck(this);" ></TD>
			</TR> 
			<TR>
			<TD>
				<INPUT TYPE="RADIO" NAME="DefWEPKey1" VALUE="2" <% If tcWebApi_get("WLan11ac_Entry","DefaultKeyID","h") = "2" then asp_Write("checked") end if %> >
				<font color="#000000">Key</font>#2:</font></TD>
			<TD><INPUT TYPE="TEXT" NAME="WEP_Key21" SIZE="30" MAXLENGTH="28" VALUE="<%If tcWebApi_get("WLan11ac_Entry","Key2Str","h") <> "N/A" then tcWebApi_get("WLan11ac_Entry","Key2Str","s") end if %>"  onBlur="doKEYcheck(this);" ></TD>
			</TR> 
			<TR>
			<TD>
				<INPUT TYPE="RADIO" NAME="DefWEPKey1" VALUE="3" <% If tcWebApi_get("WLan11ac_Entry","DefaultKeyID","h") = "3" then asp_Write("checked") end if %> >
				<font color="#000000">Key</font>#3:</TD>
			<TD><INPUT TYPE="TEXT" NAME="WEP_Key31" SIZE="30" MAXLENGTH="28" VALUE="<%If tcWebApi_get("WLan11ac_Entry","Key3Str","h") <> "N/A" then tcWebApi_get("WLan11ac_Entry","Key3Str","s") end if %>"  onBlur="doKEYcheck(this);" > </TD>
			</TR> 
			<TR>
			<TD>
				<INPUT TYPE="RADIO" NAME="DefWEPKey1" VALUE="4" <% If tcWebApi_get("WLan11ac_Entry","DefaultKeyID","h") = "4" then asp_Write("checked") end if %> >
				<font color="#000000">Key</font>#4:</TD>
			<TD><INPUT TYPE="TEXT" NAME="WEP_Key41" SIZE="30" MAXLENGTH="28" VALUE="<% If tcWebApi_get("WLan11ac_Entry","Key4Str","h") <> "N/A" then tcWebApi_get("WLan11ac_Entry","Key4Str","s") end if %>"  onBlur="doKEYcheck(this);" > </TD>
			</TR>
			</TBODY>
			</TABLE>
			</div>    		            		            		            		            	        			 	
			<div id="Radius-WEP128_div"> 	
			<TABLE cellSpacing=0 cellPadding=0 border=0 >
			<TBODY> 	
			<TR>
			<TD width="130"><font color="#000000">Radius Server IP:</font></TD>
			<TD><input name="radiusSVR_IP2" type="text" value="<% if tcWebApi_get("WLan11ac_Entry","RADIUS_Server","h") <> "N/A" then tcWebApi_get("WLan11ac_Entry","RADIUS_Server","s") end if %>" size="20" maxlength="15"></TD>
			</TR>
			<TR>
			<TD><font color="#000000">Radius Server Port:</font></TD>
			<TD><input name="radiusSVR_Port2" type="text" value="<% if tcWebApi_get("WLan11ac_Entry","RADIUS_Port","h") <> "N/A" then tcWebApi_get("WLan11ac_Entry","RADIUS_Port","s") end if %>" size="8" maxlength="5"></TD>
			</TR>
			<TR>
			<TD><font color="#000000">Radius Share secret:</font></TD>
			<TD><input name="radiusSVR_Key2" type="text" value="<% if tcWebApi_get("WLan11ac_Entry","RADIUS_Key","h") <> "N/A" then tcWebApi_get("WLan11ac_Entry","RADIUS_Key","s") end if %>" size="48" maxlength="63"></TD>
			</TR>
<% if tcWebApi_get("Info_WLan11ac","isDot1XEnhanceSupported","h") = "Yes" then %>						
			<TR>
			<TD><font color="#000000">Backup Server IP:</font></TD>
			<TD><input name="bakradiusSVR_IP2" type="text" value="<% if tcWebApi_get("WLan11ac_Entry","BAK_RADIUS_Server","h") <> "N/A" then tcWebApi_get("WLan11ac_Entry","BAK_RADIUS_Server","s") end if %>" size="20" maxlength="15"></TD>
			</TR> 
			<TR>
			<TD><font color="#000000">Backup Server Port:</font></TD>
			<TD><input name="bakradiusSVR_Port2" type="text" value="<% if tcWebApi_get("WLan11ac_Entry","BAK_RADIUS_Port","h") <> "N/A" then tcWebApi_get("WLan11ac_Entry","BAK_RADIUS_Port","s") end if %>" size="8" maxlength="5"> </TD>
			</TR>
			<TR>
			<TD><font color="#000000">Backup Share secret:</font></TD>
			<TD><input name="bakradiusSVR_Key2" type="text" value="<% if tcWebApi_get("WLan11ac_Entry","BAK_RADIUS_Key","h") <> "N/A" then tcWebApi_get("WLan11ac_Entry","BAK_RADIUS_Key","s") end if %>" size="48" maxlength="63"></TD>
			</TR> 
<% end if %>						
			<TR>
			<TD><font color="#000000">Re-auth Interval:</font></TD>
			<TD><input type="text" name="radiusSVR_ReAuthInter2" value="<% if tcWebApi_get("WLan11ac_Common","session_timeout_interval","h") <> "N/A" then tcWebApi_get("WLan11ac_Common","session_timeout_interval","s") end if %>" size="48" maxlength="63">seconds</TD>
			</TR>
			<TR>
			<TD><font color="#000000">WEP-64Bits:</font></TD>
			<TD><font color="#000000">每个键请输入(1)5个字符或(2)10个字符,从0~9, a, b, c, d, e, f.</font></TD>
			</TR>
			<TR>
			<TD><font color="#000000">WEP-128Bits:</font></TD>
			<TD><font color="#000000">每个键请输入(1)13个字符或(2)26个字符,从0~9, a, b, c, d, e, f.</font></TD>
			</TR> 
			<TR>
			<TD>
				<INPUT TYPE="RADIO" NAME="DefWEPKey2" VALUE="1" <% If tcWebApi_get("WLan11ac_Entry","DefaultKeyID","h") = "1" then asp_Write("checked") end if%> <% If tcWebApi_get("WLan11ac_Entry","DefaultKeyID","h") = "N/A" then asp_Write("checked") end if %> >
				<font color="#000000">Key</font>#1:</TD>
			<TD><INPUT TYPE="TEXT" NAME="WEP_Key12" SIZE="30" MAXLENGTH="28" VALUE="<% If tcWebApi_get("WLan11ac_Entry","Key1Str","h") <> "N/A" then tcWebApi_get("WLan11ac_Entry","Key1Str","s") end if %>" onBlur="doKEYcheck(this);" ></TD>
			</TR> 
			<TR>
			<TD>
				<INPUT TYPE="RADIO" NAME="DefWEPKey2" VALUE="2" <% If tcWebApi_get("WLan11ac_Entry","DefaultKeyID","h") = "2" then asp_Write("checked") end if %> >
				<font color="#000000">Key</font>#2:</font></TD>
			<TD><INPUT TYPE="TEXT" NAME="WEP_Key22" SIZE="30" MAXLENGTH="28" VALUE="<%If tcWebApi_get("WLan11ac_Entry","Key2Str","h") <> "N/A" then tcWebApi_get("WLan11ac_Entry","Key2Str","s") end if %>"  onBlur="doKEYcheck(this);" ></TD>
			</TR> 
			<TR>
			<TD>
				<INPUT TYPE="RADIO" NAME="DefWEPKey2" VALUE="3" <% If tcWebApi_get("WLan11ac_Entry","DefaultKeyID","h") = "3" then asp_Write("checked") end if %> >
				<font color="#000000">Key</font>#3:</TD>
			<TD><INPUT TYPE="TEXT" NAME="WEP_Key32" SIZE="30" MAXLENGTH="28" VALUE="<%If tcWebApi_get("WLan11ac_Entry","Key3Str","h") <> "N/A" then tcWebApi_get("WLan11ac_Entry","Key3Str","s") end if %>"  onBlur="doKEYcheck(this);" > </TD>
			</TR> 
			<TR>
			<TD>
				<INPUT TYPE="RADIO" NAME="DefWEPKey2" VALUE="4" <% If tcWebApi_get("WLan11ac_Entry","DefaultKeyID","h") = "4" then asp_Write("checked") end if %> >
				<font color="#000000">Key</font>#4:</TD>
			<TD><INPUT TYPE="TEXT" NAME="WEP_Key42" SIZE="30" MAXLENGTH="28" VALUE="<% If tcWebApi_get("WLan11ac_Entry","Key4Str","h") <> "N/A" then tcWebApi_get("WLan11ac_Entry","Key4Str","s") end if %>"  onBlur="doKEYcheck(this);" > </TD>
			</TR>
			</TBODY>
			</TABLE>
			</div>  		            		            		            		            	        		 	
			<div id="WPA_div">	
			<TABLE cellSpacing=0 cellPadding=0 border=0 >
			<TBODY>					
			<TR>
			<TD width="130"><font color="#000000">Radius Server IP:</font></TD>
			<TD><input name="radiusSVR_IP3" type="text" value="<% if tcWebApi_get("WLan11ac_Entry","RADIUS_Server","h") <> "N/A" then tcWebApi_get("WLan11ac_Entry","RADIUS_Server","s") end if %>" size="20" maxlength="15"></TD>
			</TR>
			<TR>
			<TD><font color="#000000">Radius Server Port:</font></TD>
			<TD><input name="radiusSVR_Port3" type="text" value="<% if tcWebApi_get("WLan11ac_Entry","RADIUS_Port","h") <> "N/A" then tcWebApi_get("WLan11ac_Entry","RADIUS_Port","s") end if %>" size="8" maxlength="5"></TD>
			</TR>
			<TR>
			<TD><font color="#000000">Radius Share secret:</font></TD>
			<TD><input name="radiusSVR_Key3" type="text" value="<% if tcWebApi_get("WLan11ac_Entry","RADIUS_Key","h") <> "N/A" then tcWebApi_get("WLan11ac_Entry","RADIUS_Key","s") end if %>" size="48" maxlength="63"></TD>
			</TR>
<% if tcWebApi_get("Info_WLan11ac","isDot1XEnhanceSupported","h") = "Yes" then %>						
			<TR>
			<TD><font color="#000000">Backup Server IP:</font></TD>
			<TD><input name="bakradiusSVR_IP3" type="text" value="<% if tcWebApi_get("WLan11ac_Entry","BAK_RADIUS_Server","h") <> "N/A" then tcWebApi_get("WLan11ac_Entry","BAK_RADIUS_Server","s") end if %>" size="20" maxlength="15"></TD>
			</TR> 
			<TR>
			<TD><font color="#000000">Backup Server Port:</font></TD>
			<TD><input name="bakradiusSVR_Port3" type="text" value="<% if tcWebApi_get("WLan11ac_Entry","BAK_RADIUS_Port","h") <> "N/A" then tcWebApi_get("WLan11ac_Entry","BAK_RADIUS_Port","s") end if %>" size="8" maxlength="5"> </TD>
			</TR>
			<TR>
			<TD><font color="#000000">Backup Share secret:</font></TD>
			<TD><input name="bakradiusSVR_Key3" type="text" value="<% if tcWebApi_get("WLan11ac_Entry","BAK_RADIUS_Key","h") <> "N/A" then tcWebApi_get("WLan11ac_Entry","BAK_RADIUS_Key","s") end if %>" size="48" maxlength="63"></TD>
			</TR> 
<% end if %>						
			<TR>
			<TD><font color="#000000">Re-auth Interval:</font></TD>
			<TD><input type="text" name="radiusSVR_ReAuthInter3" value="<% if tcWebApi_get("WLan11ac_Common","session_timeout_interval","h") <> "N/A" then tcWebApi_get("WLan11ac_Common","session_timeout_interval","s") end if %>" size="48" maxlength="63">seconds</TD>
			</TR>
			<TR>
			<TD><font color="#000000">加密方式:</font></TD>
			<TD>
				<SELECT NAME="TKIP_Selection1" onChange="doEncryptionChange(this)" SIZE="1">
				<OPTION value="AES" <% if tcWebApi_get("WLan11ac_Entry","EncrypType","h") = "AES" then asp_Write("selected") end if %> >AES</OPTION>
				<OPTION value="TKIP" <% If tcWebApi_get("WLan11ac_Entry","EncrypType","h") = "TKIP" then asp_Write("selected") end if %> >TKIP</OPTION>
				<OPTION value="TKIPAES" <% if tcWebApi_get("WLan11ac_Entry","EncrypType","h") = "TKIPAES" then asp_Write("selected") end if %> >TKIPAES</OPTION>
				</SELECT>
			</TD>
			</TR>
			<TR>
			<TD><font color="#000000">WPA组密钥更新间隔:</font></TD>
			<TD>
				<input type="text" id="WPARekeyInter3" name="WPARekeyInter3" size="7" maxlength="7" onBlur="checkRekeyinteral(this.value, 1)">
				<font color="#000000">seconds</font>
<script language="JavaScript" type="text/JavaScript">
	var rekeystr = "<% tcWebApi_get("WLan11ac_Entry","RekeyInterval","s") %>";
	if("N/A" == rekeystr || "" == rekeystr)
		document.getElementById('WPARekeyInter3').value = "3600";
	else
		document.getElementById('WPARekeyInter3').value = rekeystr;
</script>
			</TD>
			</TR>
			</TBODY>
			</TABLE>
			</div>    		            		            		            		            	        		 	
			<div id="WPA2_div">
			<TABLE cellSpacing=0 cellPadding=0 border=0 >
			<TBODY>		
			<TR>
			<TD width="130"><font color="#000000">Radius Server IP:</font></TD>
			<TD><input name="radiusSVR_IP4" type="text" value="<% if tcWebApi_get("WLan11ac_Entry","RADIUS_Server","h") <> "N/A" then tcWebApi_get("WLan11ac_Entry","RADIUS_Server","s") end if %>" size="20" maxlength="15"></TD>
			</TR>
			<TR>
			<TD><font color="#000000">Radius Server Port:</font></TD>
			<TD><input name="radiusSVR_Port4" type="text" value="<% if tcWebApi_get("WLan11ac_Entry","RADIUS_Port","h") <> "N/A" then tcWebApi_get("WLan11ac_Entry","RADIUS_Port","s") end if %>" size="8" maxlength="5"></TD>
			</TR>
			<TR>
			<TD><font color="#000000">Radius Share secret:</font></TD>
			<TD><input name="radiusSVR_Key4" type="text" value="<% if tcWebApi_get("WLan11ac_Entry","RADIUS_Key","h") <> "N/A" then tcWebApi_get("WLan11ac_Entry","RADIUS_Key","s") end if %>" size="48" maxlength="63"></TD>
			</TR>
<% if tcWebApi_get("Info_WLan11ac","isDot1XEnhanceSupported","h") = "Yes" then %>						
			<TR>
			<TD><font color="#000000">Backup Server IP:</font></TD>
			<TD><input name="bakradiusSVR_IP4" type="text" value="<% if tcWebApi_get("WLan11ac_Entry","BAK_RADIUS_Server","h") <> "N/A" then tcWebApi_get("WLan11ac_Entry","BAK_RADIUS_Server","s") end if %>" size="20" maxlength="15"></TD>
			</TR> 
			<TR>
			<TD><font color="#000000">Backup Server Port:</font></TD>
			<TD><input name="bakradiusSVR_Port4" type="text" value="<% if tcWebApi_get("WLan11ac_Entry","BAK_RADIUS_Port","h") <> "N/A" then tcWebApi_get("WLan11ac_Entry","BAK_RADIUS_Port","s") end if %>" size="8" maxlength="5"> </TD>
			</TR>
			<TR>
			<TD><font color="#000000">Backup Share secret:</font></TD>
			<TD><input name="bakradiusSVR_Key4" type="text" value="<% if tcWebApi_get("WLan11ac_Entry","BAK_RADIUS_Key","h") <> "N/A" then tcWebApi_get("WLan11ac_Entry","BAK_RADIUS_Key","s") end if %>" size="48" maxlength="63"></TD>
			</TR> 
<% end if %>						
			<TR>
			<TD><font color="#000000">Re-auth Interval:</font></TD>
			<TD><input type="text" name="radiusSVR_ReAuthInter4" value="<% if tcWebApi_get("WLan11ac_Common","session_timeout_interval","h") <> "N/A" then tcWebApi_get("WLan11ac_Common","session_timeout_interval","s") end if %>" size="48" maxlength="63">seconds</TD>
			</TR>
			<TR>
			<TD><font color="#000000">加密方式:</font></TD>
			<TD>
				<SELECT NAME="TKIP_Selection2" onChange="doEncryptionChange(this)" SIZE="1">
				<OPTION value="AES" <% if tcWebApi_get("WLan11ac_Entry","EncrypType","h") = "AES" then asp_Write("selected") end if %>>AES</OPTION>
				<OPTION value="TKIP" <% If tcWebApi_get("WLan11ac_Entry","EncrypType","h") = "TKIP" then asp_Write("selected") end if %>>TKIP</OPTION>
				<OPTION value="TKIPAES" <% if tcWebApi_get("WLan11ac_Entry","EncrypType","h") = "TKIPAES" then asp_Write("selected") end if %>>TKIPAES</OPTION>
			</SELECT>
			</TD>
			</TR>
			<TR>
			<TD><font color="#000000">WPA组密钥更新间隔:</font></TD>
			<TD>
				<input type="text" id="WPARekeyInter4" name="WPARekeyInter4" size="7" maxlength="7" onBlur="checkRekeyinteral(this.value, 1)">
				<font color="#000000">seconds</font>
<script language="JavaScript" type="text/JavaScript">
	var rekeystr = "<% tcWebApi_get("WLan11ac_Entry","RekeyInterval","s") %>";
	if("N/A" == rekeystr || "" == rekeystr)
		document.getElementById('WPARekeyInter4').value = "3600";
	else
		document.getElementById('WPARekeyInter4').value = rekeystr;
</script>
			</TD>
			</TR>
			</TBODY>
			</TABLE>
			</div>    		            		            		            		            	        	
			<div id="WPA1WPA2_div">
			<TABLE cellSpacing=0 cellPadding=0 border=0 >
			<TBODY>	
			<TR>
			<TD width="130"><font color="#000000">Radius Server IP:</font></TD>
			<TD><input name="radiusSVR_IP5" type="text" value="<% if tcWebApi_get("WLan11ac_Entry","RADIUS_Server","h") <> "N/A" then tcWebApi_get("WLan11ac_Entry","RADIUS_Server","s") end if %>" size="20" maxlength="15"></TD>
			</TR>
			<TR>
			<TD><font color="#000000">Radius Server Port:</font></TD>
			<TD><input name="radiusSVR_Port5" type="text" value="<% if tcWebApi_get("WLan11ac_Entry","RADIUS_Port","h") <> "N/A" then tcWebApi_get("WLan11ac_Entry","RADIUS_Port","s") end if %>" size="8" maxlength="5"></TD>
			</TR>
			<TR>
			<TD><font color="#000000">Radius Share secret:</font></TD>
			<TD><input name="radiusSVR_Key5" type="text" value="<% if tcWebApi_get("WLan11ac_Entry","RADIUS_Key","h") <> "N/A" then tcWebApi_get("WLan11ac_Entry","RADIUS_Key","s") end if %>" size="48" maxlength="63"></TD>
			</TR>
<% if tcWebApi_get("Info_WLan11ac","isDot1XEnhanceSupported","h") = "Yes" then %>						
			<TR>
			<TD><font color="#000000">Backup Server IP:</font></TD>
			<TD><input name="bakradiusSVR_IP5" type="text" value="<% if tcWebApi_get("WLan11ac_Entry","BAK_RADIUS_Server","h") <> "N/A" then tcWebApi_get("WLan11ac_Entry","BAK_RADIUS_Server","s") end if %>" size="20" maxlength="15"></TD>
			</TR> 
			<TR>
			<TD><font color="#000000">Backup Server Port:</font></TD>
			<TD><input name="bakradiusSVR_Port5" type="text" value="<% if tcWebApi_get("WLan11ac_Entry","BAK_RADIUS_Port","h") <> "N/A" then tcWebApi_get("WLan11ac_Entry","BAK_RADIUS_Port","s") end if %>" size="8" maxlength="5"> </TD>
			</TR>
			<TR>
			<TD><font color="#000000">Backup Share secret:</font></TD>
			<TD><input name="bakradiusSVR_Key5" type="text" value="<% if tcWebApi_get("WLan11ac_Entry","BAK_RADIUS_Key","h") <> "N/A" then tcWebApi_get("WLan11ac_Entry","BAK_RADIUS_Key","s") end if %>" size="48" maxlength="63"></TD>
			</TR> 
<% end if %>						
			<TR>
			<TD><font color="#000000">Re-auth Interval:</font></TD>
			<TD><input type="text" name="radiusSVR_ReAuthInter5" value="<% if tcWebApi_get("WLan11ac_Common","session_timeout_interval","h") <> "N/A" then tcWebApi_get("WLan11ac_Common","session_timeout_interval","s") end if %>" size="48" maxlength="63">seconds</TD>
			</TR>
			<TR>
			<TD><font color="#000000">加密方式:</font></TD>
			<TD>
				<SELECT NAME="TKIP_Selection3" onChange="doEncryptionChange(this)" SIZE="1">
				<OPTION value="AES" <% if tcWebApi_get("WLan11ac_Entry","EncrypType","h") = "AES" then asp_Write("selected") end if %>>AES</OPTION>
				<OPTION value="TKIP" <% If tcWebApi_get("WLan11ac_Entry","EncrypType","h") = "TKIP" then asp_Write("selected") end if %>>TKIP</OPTION>
				<OPTION value="TKIPAES" <% if tcWebApi_get("WLan11ac_Entry","EncrypType","h") = "TKIPAES" then asp_Write("selected") end if %>>TKIPAES</OPTION>
			</SELECT>
			</TD>
			</TR>
			<TR>
			<TD><font color="#000000">WPA组密钥更新间隔:</font></TD>
			<TD>
				<input type="text" id="WPARekeyInter5" name="WPARekeyInter5" size="7" maxlength="7" onBlur="checkRekeyinteral(this.value, 1)">
				<font color="#000000">seconds</font>
<script language="JavaScript" type="text/JavaScript">
	var rekeystr = "<% tcWebApi_get("WLan11ac_Entry","RekeyInterval","s") %>";
	if("N/A" == rekeystr || "" == rekeystr)
		document.getElementById('WPARekeyInter5').value = "3600";
	else
		document.getElementById('WPARekeyInter5').value = rekeystr;
</script>
			</TD>
			</TR>
			</TBODY>
			</TABLE>
			</div> 
<% end if %>  		            		            		            		            	        	
			<div id="WEP-64Bits_div">
			<TABLE cellSpacing=0 cellPadding=0 border=0 >
			<TBODY>		
			<TR>
			<TD width="130"><font color="#000000">WEP AuthType:</font></TD>
			<TD>
				<SELECT NAME="WEP_TypeSelection1" SIZE="1" onChange="doWEPTypeChange()">
				<OPTION value="OpenSystem" <% if tcWebApi_get("WLan11ac_Entry","WEPAuthType","h") = "OpenSystem" then asp_Write("selected") elseif tcWebApi_get("WLan11ac_Entry","WEPAuthType","h") = "N/A" then asp_Write("selected") end if %>>OPENWEP</OPTION>
				<OPTION value="SharedKey" <% if tcWebApi_get("WLan11ac_Entry","WEPAuthType","h") = "SharedKey" then asp_Write("selected") end if %>>SHAREDWEP</OPTION>
				<OPTION value="WEPAuto" <% if tcWebApi_get("WLan11ac_Entry","WEPAuthType","h") = "WEPAuto" then asp_Write("selected") end if %>>Both</OPTION>
				</SELECT>
			</TD>
			</TR>
			<TR>
			<TD><font color="#000000">WEP-64Bits:</font></TD>
			<TD><font color="#000000">每个键请输入(1)5个字符或(2)10个字符,从0~9, a, b, c, d, e, f.</font></TD>
			</TR>
			<TR>
			<TD><font color="#000000">WEP-128Bits:</font></TD>
			<TD><font color="#000000">每个键请输入(1)13个字符或(2)26个字符,从0~9, a, b, c, d, e, f.</font></TD>
			</TR> 
			<TR>
			<TD>
				<INPUT TYPE="RADIO" NAME="DefWEPKey3" VALUE="1" <% If tcWebApi_get("WLan11ac_Entry","DefaultKeyID","h") = "1" then asp_Write("checked") end if%> <% If tcWebApi_get("WLan11ac_Entry","DefaultKeyID","h") = "N/A" then asp_Write("checked") end if %> >
				<font color="#000000">Key</font>#1:</TD>
			<TD><INPUT TYPE="TEXT" NAME="WEP_Key13" SIZE="30" MAXLENGTH="28" VALUE="<% If tcWebApi_get("WLan11ac_Entry","Key1Str","h") <> "N/A" then tcWebApi_get("WLan11ac_Entry","Key1Str","s") end if %>" onBlur="doKEYcheck(this);" ></TD>
			</TR> 
			<TR>
			<TD>
				<INPUT TYPE="RADIO" NAME="DefWEPKey3" VALUE="2" <% If tcWebApi_get("WLan11ac_Entry","DefaultKeyID","h") = "2" then asp_Write("checked") end if %> >
				<font color="#000000">Key</font>#2:</font></TD>
			<TD><INPUT TYPE="TEXT" NAME="WEP_Key23" SIZE="30" MAXLENGTH="28" VALUE="<%If tcWebApi_get("WLan11ac_Entry","Key2Str","h") <> "N/A" then tcWebApi_get("WLan11ac_Entry","Key2Str","s") end if %>"  onBlur="doKEYcheck(this);" ></TD>
			</TR> 
			<TR>
			<TD>
				<INPUT TYPE="RADIO" NAME="DefWEPKey3" VALUE="3" <% If tcWebApi_get("WLan11ac_Entry","DefaultKeyID","h") = "3" then asp_Write("checked") end if %> >
				<font color="#000000">Key</font>#3:</TD>
			<TD><INPUT TYPE="TEXT" NAME="WEP_Key33" SIZE="30" MAXLENGTH="28" VALUE="<%If tcWebApi_get("WLan11ac_Entry","Key3Str","h") <> "N/A" then tcWebApi_get("WLan11ac_Entry","Key3Str","s") end if %>"  onBlur="doKEYcheck(this);" > </TD>
			</TR> 
			<TR>
			<TD>
				<INPUT TYPE="RADIO" NAME="DefWEPKey3" VALUE="4" <% If tcWebApi_get("WLan11ac_Entry","DefaultKeyID","h") = "4" then asp_Write("checked") end if %> >
				<font color="#000000">Key</font>#4:</TD>
			<TD><INPUT TYPE="TEXT" NAME="WEP_Key43" SIZE="30" MAXLENGTH="28" VALUE="<% If tcWebApi_get("WLan11ac_Entry","Key4Str","h") <> "N/A" then tcWebApi_get("WLan11ac_Entry","Key4Str","s") end if %>"  onBlur="doKEYcheck(this);" > </TD>
			</TR>
			</TBODY>
			</TABLE>
			</div>
			<div id="WEP-128Bits_div">
			<TABLE cellSpacing=0 cellPadding=0 border=0 >
			<TBODY>	
			<TR>
			<TD width="130"><font color="#000000">WEP AuthType:</font></TD>
			<TD>
				<SELECT NAME="WEP_TypeSelection2" SIZE="1" onChange="doWEPTypeChange()">
				<OPTION value="OpenSystem" <% if tcWebApi_get("WLan11ac_Entry","WEPAuthType","h") = "OpenSystem" then asp_Write("selected") elseif tcWebApi_get("WLan11ac_Entry","WEPAuthType","h") = "N/A" then asp_Write("selected") end if %>>OPENWEP</OPTION>
				<OPTION value="SharedKey" <% if tcWebApi_get("WLan11ac_Entry","WEPAuthType","h") = "SharedKey" then asp_Write("selected") end if %>>SHAREDWEP</OPTION>
				<OPTION value="WEPAuto" <% if tcWebApi_get("WLan11ac_Entry","WEPAuthType","h") = "WEPAuto" then asp_Write("selected") end if %>>Both</OPTION>
				</SELECT>
			</TD>
			</TR>
			<TR>
			<TD><font color="#000000">WEP-64Bits:</font></TD>
			<TD><font color="#000000">每个键请输入(1)5个字符或(2)10个字符,从0~9, a, b, c, d, e, f.</font></TD>
			</TR>
			<TR>
			<TD><font color="#000000">WEP-128Bits:</font></TD>
			<TD><font color="#000000">每个键请输入(1)13个字符或(2)26个字符,从0~9, a, b, c, d, e, f.</font></TD>
			</TR> 
			<TR>
			<TD>
				<INPUT TYPE="RADIO" NAME="DefWEPKey4" VALUE="1" <% If tcWebApi_get("WLan11ac_Entry","DefaultKeyID","h") = "1" then asp_Write("checked") end if%> <% If tcWebApi_get("WLan11ac_Entry","DefaultKeyID","h") = "N/A" then asp_Write("checked") end if %> >
				<font color="#000000">Key</font>#1:</TD>
			<TD><INPUT TYPE="TEXT" NAME="WEP_Key14" SIZE="30" MAXLENGTH="28" VALUE="<% If tcWebApi_get("WLan11ac_Entry","Key1Str","h") <> "N/A" then tcWebApi_get("WLan11ac_Entry","Key1Str","s") end if %>" onBlur="doKEYcheck(this);" ></TD>
			</TR> 
			<TR>
			<TD>
				<INPUT TYPE="RADIO" NAME="DefWEPKey4" VALUE="2" <% If tcWebApi_get("WLan11ac_Entry","DefaultKeyID","h") = "2" then asp_Write("checked") end if %> >
				<font color="#000000">Key</font>#2:</font></TD>
			<TD><INPUT TYPE="TEXT" NAME="WEP_Key24" SIZE="30" MAXLENGTH="28" VALUE="<%If tcWebApi_get("WLan11ac_Entry","Key2Str","h") <> "N/A" then tcWebApi_get("WLan11ac_Entry","Key2Str","s") end if %>"  onBlur="doKEYcheck(this);" ></TD>
			</TR> 
			<TR>
			<TD>
				<INPUT TYPE="RADIO" NAME="DefWEPKey4" VALUE="3" <% If tcWebApi_get("WLan11ac_Entry","DefaultKeyID","h") = "3" then asp_Write("checked") end if %> >
				<font color="#000000">Key</font>#3:</TD>
			<TD><INPUT TYPE="TEXT" NAME="WEP_Key34" SIZE="30" MAXLENGTH="28" VALUE="<%If tcWebApi_get("WLan11ac_Entry","Key3Str","h") <> "N/A" then tcWebApi_get("WLan11ac_Entry","Key3Str","s") end if %>"  onBlur="doKEYcheck(this);" > </TD>
			</TR> 
			<TR>
			<TD>
				<INPUT TYPE="RADIO" NAME="DefWEPKey4" VALUE="4" <% If tcWebApi_get("WLan11ac_Entry","DefaultKeyID","h") = "4" then asp_Write("checked") end if %> >
				<font color="#000000">Key</font>#4:</TD>
			<TD><INPUT TYPE="TEXT" NAME="WEP_Key44" SIZE="30" MAXLENGTH="28" VALUE="<% If tcWebApi_get("WLan11ac_Entry","Key4Str","h") <> "N/A" then tcWebApi_get("WLan11ac_Entry","Key4Str","s") end if %>"  onBlur="doKEYcheck(this);" > </TD>
			</TR>   
			</TBODY>
			</TABLE>
			</div>
			<div id="WPA2PSK_div">
			<TABLE cellSpacing=0 cellPadding=0 border=0 >
			<TBODY>   		        
			<TR>
			<TD width="130"><font color="#000000">加密方式:</font></TD>
			<TD>
				<SELECT NAME="TKIP_Selection4" onChange="doEncryptionChange(this)" SIZE="1">
				<OPTION value="AES" <% if tcWebApi_get("WLan11ac_Entry","EncrypType","h") = "AES" then asp_Write("selected") end if %>>AES</OPTION>
				<OPTION value="TKIP" <% If tcWebApi_get("WLan11ac_Entry","EncrypType","h") = "TKIP" then asp_Write("selected") end if%> >TKIP</OPTION>
				<OPTION value="TKIPAES" <% if tcWebApi_get("WLan11ac_Entry","EncrypType","h") = "TKIPAES" then asp_Write("selected") end if %>>TKIPAES</OPTION>
				</SELECT>
			</TD>
			</TR>  	
			<TR>
			<TD><font color="#000000">预共享密钥:</font></TD>
			<TD><INPUT TYPE="TEXT" NAME="PreSharedKey1" STYLE="WIDTH:140" SIZE="48" MAXLENGTH="64" VALUE="<% If tcWebApi_get("WLan11ac_Entry","WPAPSK","h") <> "N/A" then tcWebApi_get("WLan11ac_Entry","WPAPSK","s") end if %>" onBlur="wpapskCheck(this)"><font color="#000000">(8~63个字符或64个十六进制字符串)</font></TD>
			</TR>
			<TR>
			<TD><font color="#000000">密钥更新周期:</font></TD>
			<TD>
				<INPUT TYPE="TEXT" id="keyRenewalInterval1" NAME="keyRenewalInterval1" SIZE="7" MAXLENGTH="7" onBlur="checkRekeyinteral(this.value, 0)">
				<font color="#000000">秒 (0 ~ 4194303)</font>
<script language="JavaScript" type="text/JavaScript">
	var rekeystr = "<% tcWebApi_get("WLan11ac_Entry","RekeyInterval","s") %>";
	if("N/A" == rekeystr || "" == rekeystr)
		document.getElementById('keyRenewalInterval1').value = "3600";
	else
		document.getElementById('keyRenewalInterval1').value = rekeystr;
</script>
			</TD>
			</TR>
			</TBODY>
			</TABLE>
			</div>
			<div id="WPAPSK_div">
			<TABLE cellSpacing=0 cellPadding=0 border=0 >
			<TBODY>
			<TR>
			<TD width="130"><font color="#000000">加密方式:</font></TD>
			<TD>
				<SELECT NAME="TKIP_Selection5" onChange="doEncryptionChange(this)" SIZE="1">
				<OPTION value="AES" <% if tcWebApi_get("WLan11ac_Entry","EncrypType","h") = "AES" then asp_Write("selected") end if %>>AES</OPTION>
				<OPTION value="TKIP" <% If tcWebApi_get("WLan11ac_Entry","EncrypType","h") = "TKIP" then asp_Write("selected") end if%> >TKIP</OPTION>
				<OPTION value="TKIPAES" <% if tcWebApi_get("WLan11ac_Entry","EncrypType","h") = "TKIPAES" then asp_Write("selected") end if %>>TKIPAES</OPTION>
				</SELECT>
			</TD>
			</TR>  	
			<TR>
			<TD><font color="#000000">预共享密钥:</font></TD>
			<TD><INPUT TYPE="TEXT" NAME="PreSharedKey2" STYLE="WIDTH:140" SIZE="48" MAXLENGTH="64" VALUE="<% If tcWebApi_get("WLan11ac_Entry","WPAPSK","h") <> "N/A" then tcWebApi_get("WLan11ac_Entry","WPAPSK","s") end if %>" onBlur="wpapskCheck(this)"><font color="#000000">(8~63个字符或64个十六进制字符串)</font></TD>
			</TR>
			<TR>
			<TD><font color="#000000">密钥更新周期:</font></TD>
			<TD>
				<INPUT TYPE="TEXT" id="keyRenewalInterval2" NAME="keyRenewalInterval2" SIZE="7" MAXLENGTH="7" onBlur="checkRekeyinteral(this.value, 0)">
				<font color="#000000">秒 (0 ~ 4194303)</font>
<script language="JavaScript" type="text/JavaScript">
	var rekeystr = "<% tcWebApi_get("WLan11ac_Entry","RekeyInterval","s") %>";
	if("N/A" == rekeystr || "" == rekeystr)
		document.getElementById('keyRenewalInterval2').value = "3600";
	else
		document.getElementById('keyRenewalInterval2').value = rekeystr;
</script>
			</TD>
			</TR>  
			</TBODY>
			</TABLE>
			</div>
			<div id="WPAPSKWPA2PSK_div">
			<TABLE cellSpacing=0 cellPadding=0 border=0 >
			<TBODY>
			<TR>
			<TD width="130"><font color="#000000">加密方式:</font></TD>
			<TD>
				<SELECT NAME="TKIP_Selection6" onChange="doEncryptionChange(this)" SIZE="1">
				<OPTION value="AES" <% if tcWebApi_get("WLan11ac_Entry","EncrypType","h") = "AES" then asp_Write("selected") end if %>>AES</OPTION>
				<OPTION value="TKIP" <% If tcWebApi_get("WLan11ac_Entry","EncrypType","h") = "TKIP" then asp_Write("selected") end if%> >TKIP</OPTION>
				<OPTION value="TKIPAES" <% if tcWebApi_get("WLan11ac_Entry","EncrypType","h") = "TKIPAES" then asp_Write("selected") end if %>>TKIPAES</OPTION>
				</SELECT>
			</TD>
			</TR>  	
			<TR>
			<TD><font color="#000000">预共享密钥:</font></TD>
			<TD><INPUT TYPE="TEXT" NAME="PreSharedKey3" STYLE="WIDTH:140" SIZE="48" MAXLENGTH="64" VALUE="<% If tcWebApi_get("WLan11ac_Entry","WPAPSK","h") <> "N/A" then tcWebApi_get("WLan11ac_Entry","WPAPSK","s") end if %>" onBlur="wpapskCheck(this)"><font color="#000000">(8~63个字符或64个十六进制字符串)</font></TD>
			</TR>
			<TR>
			<TD><font color="#000000">密钥更新周期:</font></TD>
			<TD>
				<INPUT TYPE="TEXT" id="keyRenewalInterval3" NAME="keyRenewalInterval3" SIZE="7" MAXLENGTH="7" onBlur="checkRekeyinteral(this.value, 0)">
				<font color="#000000">秒 (0 ~ 4194303)</font>
<script language="JavaScript" type="text/JavaScript">
	var rekeystr = "<% tcWebApi_get("WLan11ac_Entry","RekeyInterval","s") %>";
	if("N/A" == rekeystr || "" == rekeystr)
		document.getElementById('keyRenewalInterval3').value = "3600";
	else
		document.getElementById('keyRenewalInterval3').value = rekeystr;
</script>
			</TD>
			</TR>  
			</TBODY>
			</TABLE>
			</div><BR>
			<TABLE cellSpacing=0 cellPadding=0 border=0 >
			<TBODY>
<% If tcWebApi_get("Info_WLan11ac","isWDSSupported","h") = "Yes" Then %>
			<TR>
			<TD width="130"><font color="#000000">WDS使能:</font></TD>
			<TD>
				<input type="RADIO" name="WLAN_WDS_Active" value="1" onClick="autoWLAN_WDS_Active()" <%If tcWebApi_get("WLan11ac_WDS","WdsEnable","h") <> "0" then asp_Write("checked") end if%> ><font color="#000000">开启</font> 
				<input type="RADIO" name="WLAN_WDS_Active" value="0" onClick="autoWLAN_WDS_Deactive()" <%If tcWebApi_get("WLan11ac_WDS","WdsEnable","h") = "0" then asp_Write("checked") end if%> <%If tcWebApi_get("WLan11ac_WDS","WdsEnable","h") = "N/A" then asp_Write("checked") end if%> ><font color="#000000">关闭</font>
			</TD>
			</TR>
			</TBODY>
			</TABLE>

			<div id="else_div">
			<TABLE cellSpacing=0 cellPadding=0 border=0 >
			<TBODY>
			<TR>
			<TD width="130"><font color="#000000">WDS加密方式:</font></TD>
			<TD>
				<SELECT NAME="WDS_EncrypType_Selection" SIZE="1" onChange="doWDSEncrypTypeChange()">
				<OPTION value="TKIP" <% If tcWebApi_get("WLan11ac_WDS","WdsEncrypType","h") = "TKIP" then asp_Write("selected") end if %> >TKIP</OPTION>
				<OPTION value="AES" <% if tcWebApi_get("WLan11ac_WDS","WdsEncrypType","h") = "AES" then asp_Write("selected") end if %> >TKIPAES</OPTION>
				</SELECT>
			</TD>
			</TR>
			<TR>
			<TD><font color="#000000">WDS密钥:</font></TD>
			<TD><INPUT TYPE="TEXT" NAME="WDS_Key" STYLE="WIDTH:140" SIZE="48" MAXLENGTH="64" VALUE="<% If tcWebApi_get("WLan11ac_WDS","WdsKey","h") <> "N/A" then tcWebApi_get("WLan11ac_WDS","WdsKey","s") end if %>" onBlur="WDSKeyCheck(this)"><font color="#000000">(8~63个字符或64个十六进制字符串)</font></TD>
			</TR>
			</TBODY>
			</TABLE>
			</div>

			<TABLE cellSpacing=0 cellPadding=0 border=0 >
			<TBODY>			
			<TR>
			<TD width="130"><font color="#000000">WDS Peer MAC #1:</font></TD>
			<TD><INPUT TYPE="TEXT" NAME="WLANWDS_PEER_MAC1" STYLE="WIDTH:140" SIZE="20" MAXLENGTH="20" VALUE="<% if tcWebApi_get("WLan11ac_WDS","Wds_MAC0","h") <> "N/A" then tcWebApi_get("WLan11ac_WDS","Wds_MAC0","s") end if %>" onBlur="doMACcheck(this)"></TD>
			</TR>
			<TR>
			<TD><font color="#000000">WDS Peer MAC #2:</font></TD>
			<TD><INPUT TYPE="TEXT" NAME="WLANWDS_PEER_MAC2" STYLE="WIDTH:140" SIZE="20" MAXLENGTH="20" VALUE="<% if tcWebApi_get("WLan11ac_WDS","Wds_MAC1","h") <> "N/A" then tcWebApi_get("WLan11ac_WDS","Wds_MAC1","s") end if %>" onBlur="doMACcheck(this)"></TD>
			</TR>
			<TR>
			<TD><font color="#000000">WDS Peer MAC #3:</font></TD>
			<TD><INPUT TYPE="TEXT" NAME="WLANWDS_PEER_MAC3" STYLE="WIDTH:140" SIZE="20" MAXLENGTH="20" VALUE="<%if tcWebApi_get("WLan11ac_WDS","Wds_MAC2","h") <> "N/A" then tcWebApi_get("WLan11ac_WDS","Wds_MAC2","s") end if %>" onBlur="doMACcheck(this)"></TD>
			</TR>
			<TR>
			<TD><font color="#000000">WDS Peer MAC #4:</font></TD>
			<TD><INPUT TYPE="TEXT" NAME="WLANWDS_PEER_MAC4" STYLE="WIDTH:140" SIZE="20" MAXLENGTH="20" VALUE="<%if tcWebApi_get("WLan11ac_WDS","Wds_MAC3","h") <> "N/A" then tcWebApi_get("WLan11ac_WDS","Wds_MAC3","s") end if %>" onBlur="doMACcheck(this)"></TD>
			</TR>
<% End If %> 		            		            		            		            	        
			</TBODY>
			</TABLE><BR>	
			<TABLE cellSpacing=0 cellPadding=0 border=0 >
			<TBODY>
			<TR>
			<TD width="130"><font color="#000000">Active:</font></TD>
			<TD>
				<INPUT TYPE="RADIO" NAME="WLAN_FltActive" VALUE="1" <% If tcWebApi_get("WLan11ac_Entry","AccessPolicy","h") <> "0" then asp_Write("checked") end if %> ><font color="#000000">开启</font>
				<INPUT TYPE="RADIO" NAME="WLAN_FltActive" VALUE="0" <% If tcWebApi_get("WLan11ac_Entry","AccessPolicy","h") = "0" then asp_Write("checked") end if %> <% If tcWebApi_get("WLan11ac_Entry","AccessPolicy","h") = "N/A" then asp_Write("checked") end if %> ><font color="#000000">关闭</font>
			</TD>
			</TR>
			<TR>
			<TD><font color="#000000">Action:</font></TD>
			<TD>
				<SELECT NAME="WLAN_FltAction" SIZE="1">
				<OPTION value="1" <% if tcWebApi_get("WLan11ac_Entry","AccessPolicy","h") = "1" then asp_Write("selected") end if %>>Allow</OPTION>
				<OPTION value="2" <% if tcWebApi_get("WLan11ac_Entry","AccessPolicy","h") = "2" then asp_Write("selected") end if %>>Deny</OPTION>
				</SELECT>
			</TD>
			</TR>
			<TR>
			<TD><font color="#000000">Mac地址#1:</font></TD>
			<TD><INPUT TYPE="TEXT" NAME="WLANFLT_MAC1" STYLE="WIDTH:140" SIZE="20" MAXLENGTH="20" VALUE="<% if tcWebApi_get("WLan11ac_Entry","WLan_MAC0","h") <> "N/A" then tcWebApi_get("WLan11ac_Entry","WLan_MAC0","s") end if %>" onBlur="doMACcheck(this)"></TD>
			</TR>
			<TR>
			<TD><font color="#000000">Mac地址#2:</font></TD>
			<TD><INPUT TYPE="TEXT" NAME="WLANFLT_MAC2" STYLE="WIDTH:140" SIZE="20" MAXLENGTH="20" VALUE="<% if tcWebApi_get("WLan11ac_Entry","WLan_MAC1","h") <> "N/A" then tcWebApi_get("WLan11ac_Entry","WLan_MAC1","s") end if %>" onBlur="doMACcheck(this)"></TD>
			</TR>
			<TR>
			<TD><font color="#000000">Mac地址#3:</font></TD>
			<TD><INPUT TYPE="TEXT" NAME="WLANFLT_MAC3" STYLE="WIDTH:140" SIZE="20" MAXLENGTH="20" VALUE="<% if tcWebApi_get("WLan11ac_Entry","WLan_MAC2","h") <> "N/A" then tcWebApi_get("WLan11ac_Entry","WLan_MAC2","s") end if %>" onBlur="doMACcheck(this)"></TD>
			</TR>
			<TR>
			<TD><font color="#000000">Mac地址#4:</font></TD>
			<TD><INPUT TYPE="TEXT" NAME="WLANFLT_MAC4" STYLE="WIDTH:140" SIZE="20" MAXLENGTH="20" VALUE="<% if tcWebApi_get("WLan11ac_Entry","WLan_MAC3","h") <> "N/A" then tcWebApi_get("WLan11ac_Entry","WLan_MAC3","s") end if %>" onBlur="doMACcheck(this)"></TD>
			</TR>
			<TR>
			<TD><font color="#000000">Mac地址#5:</font></TD>
			<TD><INPUT TYPE="TEXT" NAME="WLANFLT_MAC5" STYLE="WIDTH:140" SIZE="20" MAXLENGTH="20" VALUE="<% if tcWebApi_get("WLan11ac_Entry","WLan_MAC4","h") <> "N/A" then tcWebApi_get("WLan11ac_Entry","WLan_MAC4","s") end if %>" onBlur="doMACcheck(this)"></TD>
			</TR>  
			<TR>
			<TD><font color="#000000">Mac地址#6:</font></TD>
			<TD><INPUT TYPE="TEXT" NAME="WLANFLT_MAC6" STYLE="WIDTH:140" SIZE="20" MAXLENGTH="20" VALUE="<% if tcWebApi_get("WLan11ac_Entry","WLan_MAC5","h") <> "N/A" then tcWebApi_get("WLan11ac_Entry","WLan_MAC5","s") end if %>" onBlur="doMACcheck(this)"></TD>
			</TR>  
			<TR>
			<TD><font color="#000000">Mac地址#7:</font></TD>
			<TD><INPUT TYPE="TEXT" NAME="WLANFLT_MAC7" STYLE="WIDTH:140" SIZE="20" MAXLENGTH="20" VALUE="<% if tcWebApi_get("WLan11ac_Entry","WLan_MAC6","h") <> "N/A" then tcWebApi_get("WLan11ac_Entry","WLan_MAC6","s") end if %>" onBlur="doMACcheck(this)"></TD>
			</TR>  
			<TR>
			<TD><font color="#000000">Mac地址#8:</font></TD>
			<TD><INPUT TYPE="TEXT" NAME="WLANFLT_MAC8" STYLE="WIDTH:140" SIZE="20" MAXLENGTH="20" VALUE="<% if tcWebApi_get("WLan11ac_Entry","WLan_MAC7","h") <> "N/A" then tcWebApi_get("WLan11ac_Entry","WLan_MAC7","s") end if %>" onBlur="doMACcheck(this)"></TD>
			</TR>    		            		            		            		            	        
			</TBODY>
			</TABLE>	  		            		              	 
		</TD>
		</TR>
		</TBODY>
		</TABLE>
		</FORM>
	</TD>
	</TR>
	<TR>
	<TD vAlign=top width=157 bgColor=#e7e7e7 height=10 ></TD>
	<TD width=7 background=/img/panel3.gif ></TD>
	<TD></TD>
	</TR>
	<TR>
	<TD vAlign=top width=157 bgColor=#e7e7e7 ></TD>
	<TD width=7 background=/img/panel3.gif ></TD>
	<TD></TD>
	</TR>
	</TBODY>
	</TABLE>
</TD>
</TR>
<TR>
<TD height=1 >
<TABLE id=table7 height=35 cellSpacing=0 cellPadding=0 width=808 border=0 >
<TBODY>
<TR>
<TD width=162 bgColor=#ef8218 ></TD>
<TD width=278 bgColor=#427594 ></TD>
<TD width=196 bgColor=#427594 ><P align=center><IMG id=btnOK name="BUTTON" onclick=doSave() height=23 src="/img/ok.gif" width=80 border=0 >&nbsp;&nbsp;<IMG id=btnCancel onclick="RefreshPage()" height=23 src="/img/cancel.gif" width=80 border=0 ></P></TD>
<TD width=170 bgColor=#313031 ></TD>
</TR>
</TBODY>
</TABLE>
</TD>
</TR>
</TBODY>
</TABLE>
</BODY></HTML>
