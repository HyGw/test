<%
If Request_Form("isIPv6Supported") <> "1" Then
	tcWebApi_set("Wan_PVC","IPVERSION","IPVERSION_IPv4")
End If

If request_Form("wanVCFlag") = "3" Then
	If tcWebApi_get("Wan_Common", "TransMode", "h") = "PON" Then
		tcWebApi_set("WebCurSet_Entry","wan_pvc","wan_VC")
		tcWebApi_set("WebCurSet_Entry","nat_pvc","wan_VC")
		tcWebApi_set("Wan_PVC","Active","wan_VCStatus")
   elseif tcWebApi_get("Wan_Common","TransMode","h") = "ATM" then	
	  tcWebApi_set("WebCurSet_Entry","wan_pvc","wan_VC")
	  tcWebApi_set("WebCurSet_Entry","nat_pvc","wan_VC")
	  tcWebApi_set("Wan_PVC","Active","wan_VCStatus")
	  tcWebApi_set("Wan_PVC","VPI","Alwan_VPI")
	  tcWebApi_set("Wan_PVC","VCI","Alwan_VCI")
	  tcWebApi_set("Wan_PVC","QOS","Alwan_QoS")
	  tcWebApi_set("Wan_PVC","PCR","wan_PCR")
	  tcWebApi_set("Wan_PVC","SCR","wan_SCR")
	  tcWebApi_set("Wan_PVC","MBS","wan_MBS")
  	  if tcWebApi_get("Wan_Common","CDVT_Enable","h") = "1" then
	  	tcWebApi_set("Wan_PVC","CDVT","wan_CDVT")
  	  end if
   else
	  tcWebApi_set("WebCurSet_Entry","wan_pvc","ptm_VC")
	  tcWebApi_set("WebCurSet_Entry","nat_pvc","ptm_VC")
	  tcWebApi_set("Wan_PVC","Active","wan_TransStatus")
   end if

	  tcWebApi_set("Wan_PVC","ISP","wanTypeRadio")
	  tcWebApi_set("Wan_PVC","IPVERSION","ipVerRadio")
	  tcWebApi_set("Wan_PVC","IPADDR6","wan_IPv6Addr")
	  tcWebApi_set("Wan_PVC","PREFIX6","wan_IPv6Prefix")
	  tcWebApi_set("Wan_PVC","DEFGATEWAY6","wan_IPv6DefGw")
	  
	  if Request_Form("wan_8021q") = "1" then
	  if Request_Form("disp_wan_8021q") = "1" then
		tcWebApi_set("Wan_PVC","dot1q","wan_dot1q")
		if Request_Form("isTPIDSupported") = "Yes" then
		tcWebApi_set("Wan_PVC","TPID","wan_tpid")	
		end if
	   	if Request_Form("wan_dot1q") = "Yes" then
		  tcWebApi_set("Wan_PVC","VLANID","wan_vid")
		  if Request_Form("isdot1pSupport") = "Yes" then
		  tcWebApi_set("Wan_PVC","DOT1P","wan_dot1p")
		  end if
		  	if Request_Form("isWanTagChk") = "Yes" then
		  		tcWebApi_set("Wan_PVC", "TAG", "TAGSEL")
			end if	
		end if
	  end if	  
	  end if
			  
	  if tcWebApi_get("WebCustom_Entry","isMultiVLanSupported","h") = "Yes" then
		tcWebApi_set("Wan_PVC","MVLAN","wan_mvlan")
	  end if
			  
    if request_Form("wanTypeRadio") = "0" then

	  if tcWebApi_get("Wan_Common","TransMode","h") = "ATM" then
		if request_Form("hidEncapFlag") = "1" then 
			tcWebApi_set("Wan_PVC","ENCAP","hidEncap")
		elseif request_Form("hidEncapFlag") = "0" then 
			tcWebApi_set("Wan_PVC","ENCAP","wan_Encap0")
		end if
	 end if
		
		if request_Form("wan_Encap0") = "1483 Bridged IP LLC" then
	    	tcWebApi_set("Wan_PVC","BridgeInterface","wan_BridgeInterface0")
	  	elseif request_Form("wan_Encap0") = "1483 Bridged IP VC-Mux" then
	    	tcWebApi_set("Wan_PVC","BridgeInterface","wan_BridgeInterface0")
		elseif tcWebApi_get("Wan_Common","TransMode","h") = "PTM" then
			tcWebApi_set("Wan_PVC","BridgeInterface","wan_BridgeInterface0")
		elseif tcWebApi_get("Wan_Common","TransMode","h") = "Ethernet" then
			tcWebApi_set("Wan_PVC","BridgeInterface","wan_BridgeInterface0")
		ElseIf tcWebApi_get("Wan_Common","TransMode","h") = "PON" then
			tcWebApi_set("Wan_PVC","BridgeInterface","wan_BridgeInterface0")
		end if
			
	    tcWebApi_set("Wan_PVC","NATENABLE","wan_NAT0")
	    tcWebApi_set("Wan_PVC","DEFAULTROUTE","WAN_DefaultRoute0")
	    tcWebApi_set("Wan_PVC","MTU","wan_TCPMTU0")
	    tcWebApi_set("Wan_PVC","RIPVERSION","wan_RIP0")
	    tcWebApi_set("Wan_PVC","DIRECTION","wan_RIP_Dir0")
	    tcWebApi_set("Wan_PVC","IGMPproxy","wan_IGMP0")
		
		If Request_Form("isIPv6Supported") = "1" Then           
		  tcWebApi_set("Wan_PVC","IPVERSION","ipVerRadio") 	  	  
		  tcWebApi_set("Wan_PVC","EnableDynIPv6","DynIPv6EnableRadio")		  
		  if request_Form("DynIPv6EnableRadio") = "1" then
			tcWebApi_set("Wan_PVC","MLDproxy","wan_MLD0")	
		  else 
			 tcWebApi_set("Wan_PVC","MLDproxy","wan_MLD0")
		  end if 
		  if request_Form("IPv6PrivacyAddrsSupportedFlag") = "Yes" then
			if request_Form("DynIPv6EnableRadio") = "0" then
				tcWebApi_set("Wan_PVC","IPv6Extension","privacyaddrsradio0")		
			end if  
		  end if

		  if tcwebApi_get("WebCustom_Entry","isRipngSupported","h") = "Yes" then
					TCWebApi_set("Wan_PVC","RIPNGENABLE","ripngEnableRadio0")
					If Request_Form("ripngEnableRadio0") = "1" Then 
						TCWebApi_set("Wan_PVC","RIPNGDIRECTION","wan_Ripng_Dir0")
					End if
		  end if

		  tcWebApi_set("Wan_PVC","PPPv6PD","PPPIPv6PDRadio0")
		  
		  If Request_Form("isDSLITESupported") = "1" Then
		  If Request_Form("ipVerRadio") = "IPv6" Then
					tcWebApi_set("Wan_PVC","DSLITEEnable","DSLITEEnableRadio0")
					tcWebApi_set("Wan_PVC","DSLITEMode","DSLITEModeRadio0")
					If Request_Form("DSLITEModeRadio0") = "1" Then				
						tcWebApi_set("Wan_PVC","DSLITEAddr","DSLITEAddr0")
					End if			
			End if
			End if		  
	  	End if
	   	If Request_Form("is8021xsupport") = "1" Then
	    tcWebApi_set("Wan_PVC","IPOE_DOT1X_STATUS","wan_status")
        tcWebApi_set("Wan_PVC","EAPIDENTITY","wan_eapIdentity")	
        tcWebApi_set("Wan_PVC","BIDIRECTIONALAUTHENTICATION","wan_HiddenBiDirectionalAuth")   
        tcWebApi_set("Wan_PVC","CERTIFICATE","wan_certificate")
        tcWebApi_set("Wan_PVC","TRUSTEDCA","wan_CA")
End if

    elseif request_Form("wanTypeRadio") = "1" Then
	  if tcWebApi_get("Wan_Common","TransMode","h") = "ATM" then
	    if request_Form("hidEncapFlag") = "1" then 
			tcWebApi_set("Wan_PVC","ENCAP","hidEncap")
		elseif request_Form("hidEncapFlag") = "0" then 
			tcWebApi_set("Wan_PVC","ENCAP","wan_Encap1")
		end if
	  end if
		
		if request_Form("wan_Encap1") = "1483 Bridged IP LLC" then
	    	tcWebApi_set("Wan_PVC","BridgeInterface","wan_BridgeInterface1")
	  	elseif request_Form("wan_Encap1") = "1483 Bridged IP VC-Mux" then
	    	tcWebApi_set("Wan_PVC","BridgeInterface","wan_BridgeInterface1")
		elseif tcWebApi_get("Wan_Common","TransMode","h") = "PTM" then
			tcWebApi_set("Wan_PVC","BridgeInterface","wan_BridgeInterface1")
		elseif tcWebApi_get("Wan_Common","TransMode","h") = "Ethernet" then
			tcWebApi_set("Wan_PVC","BridgeInterface","wan_BridgeInterface1")
		elseif tcWebApi_get("Wan_Common","TransMode","h") = "PON" then
			tcWebApi_set("Wan_PVC","BridgeInterface","wan_BridgeInterface1")
		end if
		
	    tcWebApi_set("Wan_PVC","IPADDR","wan_StaticIPaddr1")
	    tcWebApi_set("Wan_PVC","NETMASK","wan_StaticIPSubMask1")
	    tcWebApi_set("Wan_PVC","GATEWAY","wan_StaticIpGateway1")
		
		if tcwebApi_get("WebCustom_Entry","isWanDNSEachPVC","h") = "Yes" then
		tcWebApi_set("Wan_PVC","PRIMARYDNS","PrimaryDNS1")
		tcWebApi_set("Wan_PVC","SECONDARYDNS","SecondaryDNS1")
	    end if
		
		tcWebApi_set("Wan_PVC","NATENABLE","wan_NAT1")
	    tcWebApi_set("Wan_PVC","DEFAULTROUTE","WAN_DefaultRoute1")
	    tcWebApi_set("Wan_PVC","MTU","wan_TCPMTU1")
	    tcWebApi_set("Wan_PVC","RIPVERSION","wan_RIP1")
	    tcWebApi_set("Wan_PVC","DIRECTION","wan_RIP_Dir1")
	    tcWebApi_set("Wan_PVC","IGMPproxy","wan_IGMP1")
		
		if Request_Form("isIPv6Supported") = "1" Then 
		
				tcWebApi_set("Wan_PVC","IPADDR6","wan_IPv6Addr")			
	  			tcWebApi_set("Wan_PVC","PREFIX6","wan_IPv6Prefix")
	  			tcWebApi_set("Wan_PVC","DEFGATEWAY6","wan_IPv6DefGw")
				tcWebApi_set("Wan_PVC","DNSIPv61st","wan_IPv6DNS1")
				tcWebApi_set("Wan_PVC","DNSIPv62nd","wan_IPv6DNS2")
				tcWebApi_set("Wan_PVC","MLDproxy","wan_MLD1")

				if tcwebApi_get("WebCustom_Entry","isRipngSupported","h") = "Yes" then
					TCWebApi_set("Wan_PVC","RIPNGENABLE","ripngEnableRadio1")
					If Request_Form("ripngEnableRadio1") = "1" Then 
						TCWebApi_set("Wan_PVC","RIPNGDIRECTION","wan_Ripng_Dir1")
					End if
				end if
				
				
				If Request_Form("DHCP6SMode_Flag") = "0" Then 
					tcWebApi_set("Dhcp6s_Entry","DNSserver","wan_IPv6DNS1")
					tcWebApi_set("Dhcp6s_Entry","SecDNSserver","wan_IPv6DNS2")
					If request_Form("wanSaveFlag") = "1" Then
						tcWebApi_commit("Dhcp6s_Entry")
					End If			
				End If
				If Request_Form("isDSLITESupported") = "1" Then
				If Request_Form("ipVerRadio") = "IPv6" Then
					tcWebApi_set("Wan_PVC","DSLITEEnable","DSLITEEnableRadio1")
					tcWebApi_set("Wan_PVC","DSLITEMode","DSLITE_MANUAL_MODE")				
					tcWebApi_set("Wan_PVC","DSLITEAddr","DSLITEAddr1")
				End if
				End if
		end If
	If Request_Form("is8021xsupport") = "1" Then
	    tcWebApi_set("Wan_PVC","IPOE_DOT1X_STATUS","wan_status")
        tcWebApi_set("Wan_PVC","EAPIDENTITY","wan_eapIdentity")	
        tcWebApi_set("Wan_PVC","BIDIRECTIONALAUTHENTICATION","wan_HiddenBiDirectionalAuth")   
        tcWebApi_set("Wan_PVC","CERTIFICATE","wan_certificate")
        tcWebApi_set("Wan_PVC","TRUSTEDCA","wan_CA")
End if

  	elseif request_Form("wanTypeRadio") = "2" Then
	  
	  if tcWebApi_get("Wan_Common","TransMode","h") = "ATM" then
		if request_Form("hidEncapFlag") = "1" then 
			tcWebApi_set("Wan_PVC","ENCAP","hidEncap")
		elseif request_Form("hidEncapFlag") = "0" then 
			tcWebApi_set("Wan_PVC","ENCAP","wan_Encap2")
		end if
	  end if
		
		if request_Form("wan_Encap2") = "PPPoE LLC" then
	    	tcWebApi_set("Wan_PVC","BridgeInterface","wan_BridgeInterface2")
	  	elseif request_Form("wan_Encap2") = "PPPoE VC-Mux" then
	    	tcWebApi_set("Wan_PVC","BridgeInterface","wan_BridgeInterface2")
		elseif tcWebApi_get("Wan_Common","TransMode","h") = "PTM" then
			tcWebApi_set("Wan_PVC","BridgeInterface","wan_BridgeInterface2")
		elseif tcWebApi_get("Wan_Common","TransMode","h") = "Ethernet" then
			tcWebApi_set("Wan_PVC","BridgeInterface","wan_BridgeInterface2")
		elseif tcWebApi_get("Wan_Common","TransMode","h") = "PON" then
			tcWebApi_set("Wan_PVC","BridgeInterface","wan_BridgeInterface2")
		end if
		
 	    tcWebApi_set("Wan_PVC","USERNAME","wan_PPPUsername")
		if request_Form("TTNETGuiSupport") = "1" then
			tcWebApi_set("Wan_PVC", "USERNAME", "username1")
		end if
		If tcWebApi_get("WebCustom_Entry","isCZOTESupported","h") <> "Yes" Then
	    tcWebApi_set("Wan_PVC","PASSWORD","wan_PPPPassword")
		else
			if Request_Form("wan_PPPPassword") <> "******" Then
		    tcWebApi_set("Wan_PVC","PASSWORD","wan_PPPPassword")
		  end if
		end if
		if request_Form("isPPPAuthen") = "Yes" then
		tcWebApi_set("Wan_PVC", "AUTHEN", "PPPAuthen")
		end if
	    tcWebApi_set("Wan_PVC","CONNECTION","wan_ConnectSelect")
	    tcWebApi_set("Wan_PVC","CLOSEIFIDLE","wan_IdleTimeT")
	    tcWebApi_set("Wan_PVC","MSS","wan_TCPMSS")
		if request_Form("ipv6SupportValue")="0" then
	   		 tcWebApi_set("Wan_PVC","MTU","wan_TCPMTU2")
		end if
		
		If Request_Form("ipVerRadio") <> "IPv6" Then
	    tcWebApi_set("Wan_PVC","PPPGETIP","wan_PPPGetIP")
	    tcWebApi_set("Wan_PVC","IPADDR","wan_StaticIPaddr2")
	    tcWebApi_set("Wan_PVC","NETMASK","wan_StaticIPSubMask2")
	    tcWebApi_set("Wan_PVC","GATEWAY","wan_StaticIpGateway2")
	    tcWebApi_set("Wan_PVC","NATENABLE","wan_NAT2")	    
	    tcWebApi_set("Wan_PVC","RIPVERSION","wan_RIP2")
	    tcWebApi_set("Wan_PVC","DIRECTION","wan_RIP_Dir2")
	    tcWebApi_set("Wan_PVC","IGMPproxy","wan_IGMP2")
		End if
		tcWebApi_set("Wan_PVC","DEFAULTROUTE","WAN_DefaultRoute2") 
		
		If Request_Form("isIPv6Supported") = "1" Then 			
				tcWebApi_set("Wan_PVC","PPPv6Mode","PPPDHCPv6Mode_Flag")
				tcWebApi_set("Wan_PVC","PPPv6PD","PPPIPv6PDRadio2")	
				if request_Form("IPv6PrivacyAddrsSupportedFlag") = "Yes" then
					if request_Form("PPPDHCPv6Mode_Flag") = "0" then 
				 		 tcWebApi_set("Wan_PVC","IPv6Extension","privacyaddrsradio2")		
					end if  
		  		end if			
				tcWebApi_set("Wan_PVC","MLDproxy","wan_MLD2")

				if tcwebApi_get("WebCustom_Entry","isRipngSupported","h") = "Yes" then
					TCWebApi_set("Wan_PVC","RIPNGENABLE","ripngEnableRadio2")
					If Request_Form("ripngEnableRadio2") = "1" Then 
						TCWebApi_set("Wan_PVC","RIPNGDIRECTION","wan_Ripng_Dir2")
					End if
				end if

				If Request_Form("isDSLITESupported") = "1" Then
				If Request_Form("ipVerRadio") = "IPv6" Then
					tcWebApi_set("Wan_PVC","DSLITEEnable","DSLITEEnableRadio2")
					tcWebApi_set("Wan_PVC","DSLITEMode","DSLITEModeRadio2")
					If Request_Form("DSLITEModeRadio2") = "1" Then				
						tcWebApi_set("Wan_PVC","DSLITEAddr","DSLITEAddr2")
					End if
				End if
				End if			
		End if
	
	  elseif request_Form("wanTypeRadio") = "3" Then
	   if tcWebApi_get("Wan_Common","TransMode","h") = "ATM" then	
	    tcWebApi_set("Wan_PVC","ENCAP","wan_Encap3")
	   end if
	  end if
	  

	If request_Form("wanSaveFlag") = "1" Then
		tcWebApi_commit("Wan_PVC")
	End If
	

ElseIf Request_Form("wanVCFlag")="1" Then
  	if request_Form("wanTransFlag") = "1" Then
		tcWebApi_set("Wan_Common","TransMode","wan_TransMode")
		tcWebApi_set("WebCurSet_Entry","wan_pvc","ptm_VC")

	elseif request_Form("wanBarrierFlag") = "1" Then
 		tcWebApi_set("WebCurSet_Entry","wan_pvc","ptm_VC")
	
	elseif request_Form("service_num_flag") = "1" Then
 		tcWebApi_set("WebCurSet_Entry","wan_pvc_ext","service_num")
 		tcWebApi_set("WebCurSet_Entry","nat_pvc_ext","service_num")

  	else
		tcWebApi_set("WebCurSet_Entry","wan_pvc","wan_VC")
	
		If Request_Form("isIPv6Supported") = "1" Then 
			If Request_Form("IPVersion_Flag") = "1" Then
				tcWebApi_set("Wan_PVC","IPVERSION","ipVerRadio")
			End If		
		End If
	
		If Request_Form("vciCheckFlag")="1" Then
			tcWebApi_set("Wan_PVC","ISP","wanTypeRadio")
		End If
		If Request_Form("wanEncapFlag")="1" Then
	  		if tcWebApi_get("Wan_Common","TransMode","h") = "ATM" then
				tcWebApi_set("Wan_PVC","ENCAP","wan_Encap")
	  		end if
		End If
  	end if

ElseIf Request_Form("wanVCFlag")="2" Then
	tcWebApi_unset("Wan_PVC")
	tcWebApi_set("WebCurSet_Entry","wan_pvc","wan_VC")
if  tcwebApi_get("WebCustom_Entry","isCZOTESupported","h")<>"Yes" then
	tcWebApi_set("Wan_PVC","ISP","DefaultWan_ISP")
	tcWebApi_set("Wan_PVC","Active","DefaultWan_Active")
	If tcWebApi_get("WebCustom_Entry", "haveXPON", "h") <> "Yes" then
	tcWebApi_set("Wan_PVC","VPI","DefaultWan_VPI")
	tcWebApi_set("Wan_PVC","VCI","DefaultWan_VCI")
	tcWebApi_set("Wan_PVC","QOS","DefaultWan_QOS")
	tcWebApi_set("Wan_PVC","PCR","DefaultWan_PCR")
	tcWebApi_set("Wan_PVC","SCR","DefaultWan_SCR")
	tcWebApi_set("Wan_PVC","MBS","DefaultWan_MBS")
	End if
  if tcWebApi_get("Wan_Common","CDVT_Enable","h") = "1" then
	tcWebApi_set("Wan_PVC","CDVT","DefaultWan_CDVT")
  end if
	tcWebApi_set("Wan_PVC","IPVERSION","DefaultWan_IPVERSION")
	If tcWebApi_get("WebCustom_Entry", "haveXPON", "h") <> "Yes" then
	tcWebApi_set("Wan_PVC","ENCAP","DefaultWan_ENCAP")
	End If
	tcWebApi_set("Wan_PVC","MLDproxy","DefaultWan_MLDproxy")
end if	
	tcWebApi_commit("Wan_PVC")
	
	tcWebApi_set("WebCurSet_Entry","nat_pvc","wan_VC")
  	tcWebApi_unset("VirServer_PVC")

	tcWebApi_set("WebCurSet_Entry","nat_pvc","wan_VC")
	tcWebApi_set("Dmz_PVC","Active","DefaultDmz_Active")
	tcWebApi_set("Dmz_PVC","DMZ_IP","DefaultDmz_HostIP")
	tcWebApi_commit("Dmz_PVC")

End If
%>

<html><head>
<meta http-equiv=Content-Script-Type content=text/javascript>
<meta http-equiv=Content-Style-Type content=text/css>
<meta http-equiv=Content-Type content="text/html; charset=iso-8859-1">
<link rel="stylesheet" href="/style.css" type="text/css">
<style>
body{margin-right: -15px !important;>margin-right: 0px !important;margin-right: 0px}
</style>
<script language="JavaScript" src="/val.js"></script>
<script language="JavaScript" src="/wanfunc.js"></script>
<script language="JavaScript" src="/mac.js"></script>
<script language="JavaScript" src="OutVariant.asp"></script>
<script language="JavaScript" type='text/javascript' src="/ip_new.js"></script>
</head><script language="JavaScript">
function isInteger(value)
{   	
	if (/^\d+$/.test(value)) 
	{
	   return true;
	} 
	else 
	{
	    return false;
	}
}

function doBarrierChange(){
	var pvc;

	pvc = document.Alpha_WAN.ptm_Barrier.selectedIndex;
	pvc += 8;
	document.Alpha_WAN.ptm_VC.value = pvc;
	document.Alpha_WAN.wanVCFlag.value = 1;
	//document.Alpha_WAN.wanBarrierFlag.value = 1;
	document.Alpha_WAN.submit();
	return;
}

function doServiceChange(){
	document.Alpha_WAN.wanVCFlag.value = 1;
	document.Alpha_WAN.service_num_flag.value = 1;
	document.Alpha_WAN.submit();
	return;
}

function onClickServiceSummary() {
	window.open("/cgi-bin/home_servlist.cgi","Serv_List","toolbar=no,menubar=no,height=305, width=620,location=0,left=200,top=400");
	return false;
}

function doTransChange() {
	var pvc, hasAtm=0, hasPtm=0, hasWan0=0;

	pvc = document.Alpha_WAN.wan_TransMode.selectedIndex;

<%if tcWebApi_get("WebCustom_Entry","haveAtm","h") = "Yes" then%>
	hasAtm = 1;
<%end if%>
<%if tcWebApi_get("WebCustom_Entry","havePtm","h") = "Yes" then%>
	hasPtm = 1;
<%end if%>
<%if tcWebApi_get("WebCustom_Entry","haveWan0","h") = "Yes" then%>
	hasWan0 = 1;
<%end if%>

if(hasAtm==1 && hasPtm==1 && hasWan0==1){
//AtmPtmEther
	if(pvc==1) //ptm
		pvc = 8;
	else if(pvc==2) //ether
		pvc = 10;
}

if(hasAtm==1 && hasPtm==1 && hasWan0==0){
//AtmPtm, no Ether
	if(pvc==1) //ptm
		pvc = 8;
}

if(hasAtm==1 && hasPtm==0 && hasWan0==1){
//AtmEther, no Ptm
	if(pvc==1) //ether
		pvc = 10;
}

if(hasAtm==0 && hasPtm==1 && hasWan0==1){
//PtmEther, no Atm
	if(pvc==0) //ptm
		pvc = 8;
	else if(pvc==1) //ether
		pvc = 10;
}

if(hasAtm==0 && hasPtm==1 && hasWan0==0){
//Ptm, no AtmEther
	pvc = 8;
}

if(hasAtm==0 && hasPtm==0 && hasWan0==1){
//Ether, no AtmPtm
	pvc = 10;
}

	document.Alpha_WAN.ptm_VC.value = pvc;
	document.Alpha_WAN.wanVCFlag.value = 1;
	document.Alpha_WAN.wanTransFlag.value = 1;
	document.Alpha_WAN.wanBarrierFlag.value = 1;
	document.Alpha_WAN.submit();
	return;
}

function splitPPPUsername()
{
	var str = document.Alpha_WAN.username1.value;
	var str_array = str.split("@");
	document.Alpha_WAN.wan_PPPUsername.value = str_array[0];
	if (str_array[1] == "ttnet")
	{
		document.Alpha_WAN.wan_PPPDomain.selectedIndex = 0;
	}else{
		document.Alpha_WAN.wan_PPPDomain.selectedIndex = 1;
		document.Alpha_WAN.wan_CompanyName.value = "@" + str_array[1];		
	}
}
function doDomainChange(domainIndex)
{
	if(domainIndex == 0)
	{
		document.Alpha_WAN.wan_CompanyName.style.display = "none";
		document.Alpha_WAN.wan_PPPDomain.style.width = "70px";
		document.getElementById("IFrame1").style.display = "none";
		document.getElementById("PPPUsername_ErrorMsg_TR").style.display = "none";
	}
	else
	{
		document.Alpha_WAN.wan_CompanyName.style.display = "";
		document.Alpha_WAN.wan_PPPDomain.style.width = "119px";
		document.getElementById("IFrame1").style.display = "";
		if(document.Alpha_WAN.wan_CompanyName.value == "")
		{
			document.Alpha_WAN.wan_CompanyName.value = "Input service name";
			document.Alpha_WAN.wan_CompanyName.style.color = "#c0c0c0";
		}
		else
		{
			document.Alpha_WAN.wan_CompanyName.style.color = "#000000";
		}
	}
}
function setTtnetCompanyDefalutText(company)
{
	var companyvalue = company.value;
	if(company.value =="Input service name")
	{
		company.style.color = "#000000";
		company.value = "@";
		var range = company.createTextRange();
		range.moveStart('character',company.value.length);
		range.moveEnd('character',0);
		range.select();
	}
}
function TtnetCompanyCheck(company)
{
	var companyvalue = company.value;
	if(companyvalue=="@" || companyvalue=="" || companyvalue=="Input service name")
	{
		alert("Please don't forget to set a service name.");
		return true;
	}
	if(companyvalue.lastIndexOf("@")!=0 && companyvalue!="")
	{
		alert("Service name must be written as \"@Service name\".Please input @ and try again.");
		return true;
	}
	if(companyvalue.indexOf("ttnet")==-1 && companyvalue.indexOf("meb")==-1)
	{
		document.getElementById("PPPUsername_ErrorMsg_TR").style.display = "";
		return true;
	}else{
			document.getElementById("PPPUsername_ErrorMsg_TR").style.display = "none";
		}
	return false;
}
function DomainRangeCheck(val)
{
	var re;
	re = /[^0-9a-z@]/;
	if( re.test(val) )
	return true;
	else
	return false;
}
function WANChkIdleTimeT() {
    var form=document.Alpha_WAN;
    if (form.wan_ConnectSelect[1].checked)
        form.wan_IdleTimeT.disabled = false;
    else
    	form.wan_IdleTimeT.disabled = true;
}
function QosCheck() {
	 var form=document.Alpha_WAN;
	switch(form.Alwan_QoS.selectedIndex) {
<%if tcWebApi_get("Wan_Common","CDVT_Enable","h") = "1" then%>
		case 0:
			form.wan_PCR.disabled=false;
			form.wan_SCR.disabled = true;
			form.wan_MBS.disabled=true;
			form.wan_CDVT.disabled=true;
			break;
		case 1:
			form.wan_PCR.disabled=false;
			form.wan_SCR.disabled = true;
			form.wan_MBS.disabled=true;
			form.wan_CDVT.disabled=false;
			break;
		case 2:
		case 3:		
			form.wan_PCR.disabled=false;
			form.wan_SCR.disabled = false;
			form.wan_MBS.disabled=false;
			form.wan_CDVT.disabled=false;
			break;
<%else%>
		case 0:
		case 1:
			form.wan_PCR.disabled=false;
			form.wan_SCR.disabled = true;
			form.wan_MBS.disabled=true;
			break;
		case 2:
		case 3:		
			form.wan_PCR.disabled=false;
			form.wan_SCR.disabled = false;
			form.wan_MBS.disabled=false;
			break;
<%end if%>
		}
 return;
}
function onClickPVCSummary() {
	window.open("/cgi-bin/home_pvclist.cgi","PVC_List","toolbar=no,menubar=no,height=305, width=620,location=0,left=200,top=400");
    return false;
}

function doStatusChange() {
    var form = document.Alpha_WAN;

    if (form.wan_status.options[form.wan_status.selectedIndex].text == "Enable") {

        if (form.wanTypeRadio[0].checked || form.wanTypeRadio[1].checked) {
            if ((document.Alpha_WAN.wan_certificate.value == "")||(document.Alpha_WAN.wan_certificate.value == "N/A")) {
                //adding selected certificate name to hidden control
                var x = window.frames['Iframe2'].document.forms['Alpha_WAN'].elements['wan_cert']
                document.Alpha_WAN.wan_certificate.value = x.options[0].text;
            }
            if ((document.Alpha_WAN.wan_CA.value == "")||(document.Alpha_WAN.wan_CA.value == "N/A")) {
                //adding selected CA file name to hidden control
                var x = window.frames['Iframe3'].document.forms['Alpha_WAN'].elements['wan_trusted_ca']
                document.Alpha_WAN.wan_CA.value = x.options[0].text;
            }
            if ((document.Alpha_WAN.wan_HiddenBiDirectionalAuth.value == "") || (document.Alpha_WAN.wan_HiddenBiDirectionalAuth.value == "N/A")) {
                document.Alpha_WAN.wan_HiddenBiDirectionalAuth.value = "Yes";
            }

        }
    }
}

function doauthenticationChange() {
    if (document.Alpha_WAN.wan_authentication.checked) {
        document.Alpha_WAN.wan_HiddenBiDirectionalAuth.value = "Yes";
        document.Alpha_WAN.wan_authentication.value = "Yes";
        var x = window.frames['Iframe3'].document.forms['Alpha_WAN'].elements['wan_trusted_ca']
        x.disabled = false;
    }
    else {
        document.Alpha_WAN.wan_authentication.value = "No";
        document.Alpha_WAN.wan_HiddenBiDirectionalAuth.value = "No";
        var x = window.frames['Iframe3'].document.forms['Alpha_WAN'].elements['wan_trusted_ca']
        x.disabled = true;
    }
   // document.Alpha_WAN.submit();
}
function doVCChange() {
	document.Alpha_WAN.wanVCFlag.value = 1;
	document.Alpha_WAN.submit();
	return;
}

//jrchen add
function doIPVersionChangeIPv4(){
	//document.Alpha_WAN.wanVCFlag.value = 1;
	//document.Alpha_WAN.IPVersion_Flag.value = 1;
//	document.Alpha_WAN.PPPDHCPv6Enable_Flag.value = 0;
	//document.Alpha_WAN.submit();
	with (document.Alpha_WAN){
		setDisplay('div_isp0dsl', 0);
		setDisplay('div_isp1dsl', 0);
		setDisplay('div_isp2dsl', 0);
		if(ipVerRadio[0].checked){//ipv4
			if(wanTypeRadio[0].checked == true){
				setDisplay('div_ipv4nat_0', 1);
				setDisplay('div_ipv4nat_1', 1);
				setDisplay('div_ipv4IP', 1);
			}
			else{
				setDisplay('div_ipv4nat_0', 0);
				setDisplay('div_ipv4nat_1', 0);
				setDisplay('div_ipv4IP', 0);
			}
			
			if(wanTypeRadio[1].checked == true){
				setDisplay('div_ipv4static', 1);//isp =1
				if(document.forms[0].UserMode.value == 0)
					setDisplay('div_ipv4igp', 1); //isp =1
			}
			else{
				setDisplay('div_ipv4static', 0);//isp =1
				if(document.forms[0].UserMode.value == 0)
					setDisplay('div_ipv4igp', 0); //isp =1
			}
			if(wanTypeRadio[2].checked == true){
				setDisplay('div_ipv4getip', 1);//isp =2
				setDisplay('connondemand_info', 1);
				pppStaticCheck();
			}
			else{
				setDisplay('div_ipv4getip', 0);//isp =2
			}
			
			setDisplay('div_ipv6_staticip', 0); //
			setDisplay('div_ipv6pdm', 0);
			setDisplay('div_ipv6dhcp', 0);
		}
		else if(ipVerRadio[2].checked == true){//ipv6
			setDisplay('div_ipv4nat_0', 0);
			setDisplay('div_ipv4nat_1', 0);
			setDisplay('div_ipv4IP', 0);
			setDisplay('div_ipv4static', 0);//isp =1
			if(document.forms[0].UserMode.value == 0)
				setDisplay('div_ipv4igp', 0); //isp =1	
			setDisplay('div_ipv4getip', 0);//isp =2
			
			if(wanTypeRadio[0].checked == true){
				setDisplay('div_ipv6dhcp', 1);
				if(isDSLITESupported.value == "1")
					setDisplay('div_isp0dsl', 1);
			}
			else{
				setDisplay('div_ipv6dhcp', 0);
			}
			if(wanTypeRadio[1].checked == true){	
				setDisplay('div_ipv6_staticip', 1); //
				if(isDSLITESupported.value == "1")
					setDisplay('div_isp1dsl', 1);
			}
			else{
				setDisplay('div_ipv6_staticip', 0);
			}
			if(wanTypeRadio[2].checked == true){
				setDisplay('div_ipv6pdm', 1);
				if(isDSLITESupported.value == "1")
					setDisplay('div_isp2dsl', 1);
				if(wan_ConnectSelect[1].checked == true)
					wan_ConnectSelect[0].checked = true;
				setDisplay('connondemand_info', 0);
			}
			else{
				setDisplay('div_ipv6pdm', 1);
			}
		}
		else{//ipv4/ipv6
			if(wanTypeRadio[0].checked == true){
				setDisplay('div_ipv4nat_0', 1);
				setDisplay('div_ipv4nat_1', 1);
				setDisplay('div_ipv4IP', 1);
				setDisplay('div_ipv6dhcp', 1);
			}
			else{
				setDisplay('div_ipv4nat_0', 0);
				setDisplay('div_ipv4nat_1', 0);
				setDisplay('div_ipv4IP', 0);
				setDisplay('div_ipv6dhcp', 0);
			}
			
			if(wanTypeRadio[1].checked == true){
				setDisplay('div_ipv4static', 1);//isp =1
				if(document.forms[0].UserMode.value == 0)
					setDisplay('div_ipv4igp', 1); //isp =1
				setDisplay('div_ipv6_staticip', 1); //
			}
			else{
				setDisplay('div_ipv4static', 0);//isp =1
				if(document.forms[0].UserMode.value == 0)
					setDisplay('div_ipv4igp', 0); //isp =1
				setDisplay('div_ipv6_staticip', 0); //
			}
			if(wanTypeRadio[2].checked == true){
				setDisplay('div_ipv4getip', 1);//isp =2
				setDisplay('div_ipv6pdm', 1);
				if(wan_ConnectSelect[1].checked == true)
					wan_ConnectSelect[0].checked = true;
				setDisplay('connondemand_info', 0);
				pppStaticCheck();
			}
			else{
				setDisplay('div_ipv4getip', 0);//isp =2
				setDisplay('div_ipv6pdm', 0);
			}
		}
	}
	
    return;
}

function doIPVersionChangeIPv6(){
	//document.Alpha_WAN.wanVCFlag.value = 1;
	//document.Alpha_WAN.IPVersion_Flag.value = 1;
	//document.Alpha_WAN.PPPDHCPv6Enable_Flag.value = 1;
	//document.Alpha_WAN.submit();
    return;
}

function doConTypeChange(object) 
{
    /*if(! check_vci()){
    	document.Alpha_WAN.vciCheckFlag.value = 1;
    	document.Alpha_WAN.wanVCFlag.value = 1;
    	alert('The previous settings are reset.');
    }
	var encapLength = document.Alpha_WAN.wan_Encap.options.length; //identify bridge mode with other mode
	var flag1 = 0;
	var flag2 = 0;
	if(object.value == "0" || object.value == "1")
	{
		if(encapLength > 2)
		{
			var encap = document.Alpha_WAN.wan_Encap[2].text;
			if(encap == "PPPoA LLC" || encap == "PPPoA VC-Mux")//from PPPoA/PPPoE
				flag1 = 1;
		}else if(encapLength = 2)//from bridge mode
			flag1 = 1;
		if(flag1 == 1)
		{
			document.Alpha_WAN.hidEncapFlag.value = "1";
			document.Alpha_WAN.hidEncap.value = "1483 Bridged IP LLC";
		}
	}else if(object.value == "2")
	{
		if(encapLength > 2)
		{
			var encap = document.Alpha_WAN.wan_Encap[2].text;
			if(encap == "1483 Routed IP LLC(IPoA)" || encap == "1483 Routed IP VC-Mux")//from dynamic/static ip address
				flag2 = 1;
		}else if(encapLength = 2) //from bridge mode
			flag2 = 1;
		if(flag2 == 1)
		{
			document.Alpha_WAN.hidEncapFlag.value = "1";
			document.Alpha_WAN.hidEncap.value = "PPPoE LLC";
		}
	}*/
    //document.Alpha_WAN.submit();
	with (document.Alpha_WAN){
		if(wanTypeRadio[0].checked == true){
 			if (document.Alpha_WAN.is8021xsupport.value == "1") {
	           		setDisplay('div_802_1x', 1);
	        	}
	        	else {
	            		setDisplay('div_802_1x', 0);
	        	}
			setDisplay('div_isp0', 1);
			setDisplay('div_isp1', 0);
			setDisplay('div_isp2', 0);
			setDisplay('div_isp3', 0);
<%if tcWebApi_get("WebCustom_Entry","isCZGeneralSupported","h") = "Yes" then%>
			setDisplay('div_isp0encap', 0);
			setDisplay('div_isp1encap', 0);
<%end if%>
			//setDisplay('div_ispna', 0);
		}
		else if(wanTypeRadio[1].checked == true){
			if (document.Alpha_WAN.is8021xsupport.value == "1") {
		    		setDisplay('div_802_1x', 1);
			}
			else {
		    		setDisplay('div_802_1x', 0);
			}
			setDisplay('div_isp0', 0);
			setDisplay('div_isp1', 1);
			setDisplay('div_isp2', 0);
			setDisplay('div_isp3', 0);
<%if tcWebApi_get("WebCustom_Entry","isCZGeneralSupported","h") = "Yes" then%>
			setDisplay('div_isp0encap', 0);
			setDisplay('div_isp1encap', 0);
<%end if%>
			//setDisplay('div_ispna', 0);
		}
		else if(wanTypeRadio[2].checked == true){
			setDisplay('div_802_1x', 0);
			setDisplay('div_isp0', 0);
			setDisplay('div_isp1', 0);
			setDisplay('div_isp2', 1);
			setDisplay('div_isp3', 0);
			//setDisplay('div_ispna', 0);
			pppStaticCheck();
		}
		else if(wanTypeRadio[3].checked == true){
			setDisplay('div_802_1x', 0);
			setDisplay('div_isp0', 0);
			setDisplay('div_isp1', 0);
			setDisplay('div_isp2', 0);
			setDisplay('div_isp3', 1);
			//setDisplay('div_ispna', 0);
		}
		doIPVersionChangeIPv4();
	}
    return;
}

function doEncapChange() {
    /*if(! check_vci()){
    	document.Alpha_WAN.vciCheckFlag.value = 1;
    	document.Alpha_WAN.wanEncapFlag.value = 1;
    	document.Alpha_WAN.wanVCFlag.value = 1;
   		alert('The previous settings are reset.');
		}
    document.Alpha_WAN.submit();*/
	with (document.Alpha_WAN){
		if(wanTypeRadio[0].checked == true){
			if((0 == wan_Encap0.selectedIndex) || (1 == wan_Encap0.selectedIndex)){
<%if tcWebApi_get("WebCustom_Entry","isCZGeneralSupported","h") = "Yes" then%>
				setDisplay('div_isp0encap', 0);
<%else%>
				setDisplay('div_isp0encap', 1);
<%end if%>
			}
			else{
				setDisplay('div_isp0encap', 0);
			}	
		}
		else if(wanTypeRadio[1].checked == true){
			if((0 == wan_Encap1.selectedIndex) || (1 == wan_Encap1.selectedIndex)){
<%if tcWebApi_get("WebCustom_Entry","isCZGeneralSupported","h") = "Yes" then%>
				setDisplay('div_isp1encap', 0);
<%else%>
				setDisplay('div_isp1encap', 1);
<%end if%>
			}
			else{
				setDisplay('div_isp1encap', 0);
			}
		}
		else if(wanTypeRadio[2].checked == true){
			if((0 == wan_Encap2.selectedIndex) || (1 == wan_Encap2.selectedIndex)){
				setDisplay('div_isp2encap', 1);
			}
			else{
				setDisplay('div_isp2encap', 0);
			}
		}
	}
    return;
}
function DynamicCheck() {
    if (document.Alpha_WAN.wan_VC.selectedIndex > 0) {
        document.Alpha_WAN.wanTypeRadio[0].disabled=true;
    }
    return;
}

function idleTimeCheck() {
    var number = document.Alpha_WAN.wan_IdleTimeT.value.match("^[0-9]{1,5}$");

    if (document.Alpha_WAN.wan_ConnectSelect[1].checked) {
		if (number == null) {
			alert("Invalid Connection of Demand Idle Time");
			return true;
		}
	}
	return false;
}

function check_vci()
{
	var value;
	var value1;
	var pvc;
	var form=document.Alpha_WAN;
	pvc = form.wan_VC.selectedIndex;
	value =form.Alwan_VCI.value;
	value1=form.Alwan_VPI.value;
	if(!isInteger(value))
	{
		alert('VCI must be a Non-Negative Interger');
		return false;
	}
	if(!isInteger(value1))
	{
		alert('VPI must be a Non-Negative Interger');
		return false;
	}
	if(parseInt(value) > 65535 || parseInt(value) < 32)
	{
		alert('VCI must be in the range 32~65535');
		return false;
	}
<%if tcWebApi_get("WebCustom_Entry","sharepvc","h") = "Yes" then%>
	return true;
<%end if%>
	if(pvc == 0)
	{
		if((value == form.VCI1.value && value1 == form.VPI1.value) || (value == form.VCI2.value && value1 == form.VPI2.value) || (value == form.VCI3.value && value1 == form.VPI3.value) || (value == form.VCI4.value && value1 == form.VPI4.value) || (value == form.VCI5.value && value1 == form.VPI5.value) || (value == form.VCI6.value && value1 == form.VPI6.value) || (value == form.VCI7.value && value1 == form.VPI7.value))
		{
			alert('Invalid VPI:' + value1 +' and VCI:' + value +'. Already used by another PVC');
			return false;		
		}
		return true;
	}	
	if(pvc == 1)
	{
		if((value == form.VCI0.value && value1 == form.VPI0.value) || (value == form.VCI2.value && value1 == form.VPI2.value) || (value == form.VCI3.value && value1 == form.VPI3.value) || (value == form.VCI4.value && value1 == form.VPI4.value) || (value == form.VCI5.value && value1 == form.VPI5.value) || (value == form.VCI6.value && value1 == form.VPI6.value) || (value == form.VCI7.value && value1 == form.VPI7.value))
		{
			alert('Invalid VPI:' + value1 +' and VCI:' + value +'. Already used by another PVC');
			return false;		
		}
		return true;
	}
	if(pvc == 2)
	{
		if((value == form.VCI0.value && value1 == form.VPI0.value) || (value == form.VCI1.value && value1 == form.VPI1.value) || (value == form.VCI3.value && value1 == form.VPI3.value) || (value == form.VCI4.value && value1 == form.VPI4.value) || (value == form.VCI5.value && value1 == form.VPI5.value) || (value == form.VCI6.value && value1 == form.VPI6.value) || (value == form.VCI7.value && value1 == form.VPI7.value))
		{
			alert('Invalid VPI:' + value1 +' and VCI:' + value +'. Already used by another PVC');
			return false;		
		}
		return true;
	}
	if(pvc == 3)
	{
		if((value == form.VCI0.value && value1 == form.VPI0.value) || (value == form.VCI1.value && value1 == form.VPI1.value) || (value == form.VCI2.value && value1 == form.VPI2.value) || (value == form.VCI4.value && value1 == form.VPI4.value) || (value == form.VCI5.value && value1 == form.VPI5.value) || (value == form.VCI6.value && value1 == form.VPI6.value) || (value == form.VCI7.value && value1 == form.VPI7.value))
		{
			alert('Invalid VPI:' + value1 +' and VCI:' + value +'. Already used by another PVC');
			return false;		
		}
		return true;
	}	
	if(pvc == 4)
	{
		if((value == form.VCI0.value && value1 == form.VPI0.value) || (value == form.VCI1.value && value1 == form.VPI1.value) || (value == form.VCI2.value && value1 == form.VPI2.value) || (value == form.VCI3.value && value1 == form.VPI3.value) || (value == form.VCI5.value && value1 == form.VPI5.value) || (value == form.VCI6.value && value1 == form.VPI6.value) || (value == form.VCI7.value && value1 == form.VPI7.value))
		{
			alert('Invalid VPI:' + value1 +' and VCI:' + value +'. Already used by another PVC');
			return false;		
		}
		return true;
	}
	if(pvc == 5)
	{
		if((value == form.VCI0.value && value1 == form.VPI0.value) || (value == form.VCI1.value && value1 == form.VPI1.value) || (value == form.VCI2.value && value1 == form.VPI2.value) || (value == form.VCI3.value && value1 == form.VPI3.value) || (value == form.VCI4.value && value1 == form.VPI4.value) || (value == form.VCI6.value && value1 == form.VPI6.value) || (value == form.VCI7.value && value1 == form.VPI7.value))
		{
			alert('Invalid VPI:' + value1 +' and VCI:' + value +'. Already used by another PVC');
			return false;		
		}
		return true;
	}	
	if(pvc == 6)
	{
		if((value == form.VCI0.value && value1 == form.VPI0.value) || (value == form.VCI1.value && value1 == form.VPI1.value) || (value == form.VCI2.value && value1 == form.VPI2.value) || (value == form.VCI3.value && value1 == form.VPI3.value) || (value == form.VCI4.value && value1 == form.VPI4.value) || (value == form.VCI5.value && value1 == form.VPI5.value) || (value == form.VCI7.value && value1 == form.VPI7.value))
		{
			alert('Invalid VPI:' + value1 +' and VCI:' + value +'. Already used by another PVC');
			return false;		
		}
		return true;
	}	
	if(pvc == 7)
	{
		if((value == form.VCI0.value && value1 == form.VPI0.value) || (value == form.VCI1.value && value1 == form.VPI1.value) || (value == form.VCI2.value && value1 == form.VPI2.value) || (value == form.VCI3.value && value1 == form.VPI3.value) || (value == form.VCI4.value && value1 == form.VPI4.value) || (value == form.VCI5.value && value1 == form.VPI5.value) || (value == form.VCI6.value && value1 == form.VPI6.value))
		{
			alert('Invalid VPI:' + value1 +' and VCI:' + value +'. Already used by another PVC');
			return false;		
		}
		return true;
	}	
	return false;
}

function doDisablePPPv6()
{
	var form=document.Alpha_WAN;
	form.PPPDHCPv6Enable_Flag.value = 0;
	form.wanSaveFlag.value = 0;
	//form.submit();
	
}


function doEnablePPPv6()
{
	var form=document.Alpha_WAN;
	form.PPPDHCPv6Enable_Flag.value = 1;
	form.wanSaveFlag.value = 0;
	//form.submit();
}

function doPPPv6ModeSLAAC()
{
	var form=document.Alpha_WAN;
	form.PPPDHCPv6Mode_Flag.value = 0;
	form.wanSaveFlag.value = 0;
	//form.submit();
}

function doPPPv6ModeDHCP()
{
	var form=document.Alpha_WAN;
	form.PPPDHCPv6Mode_Flag.value = 1;
	form.wanSaveFlag.value = 0;
	//form.submit();
}

function doPPPv6PDDisable()
{
	var form=document.Alpha_WAN;
	form.IPv6PD_Flag.value = 0;
	form.wanSaveFlag.value = 0;
	//form.submit();
}

function doPPPv6PDEnable()
{
	var form=document.Alpha_WAN;
	form.IPv6PD_Flag.value = 1;
	form.wanSaveFlag.value = 0;
	//form.submit();
}

function  doPPPv6ModeStatic()
{
	var form=document.Alpha_WAN;
	form.PPPDHCPv6Mode_Flag.value = 2;
	form.wanSaveFlag.value = 0;
	//form.submit();
}

function doDSLiteEnable(index)
{
	var form=document.Alpha_WAN;	
	form.wanSaveFlag.value = 0;
	//form.submit();
	with (document.Alpha_WAN) {
		if (0 == index){
			setDisplay('div_dslite0_0', 1);
			setDisplay('div_dslite0_1', 1);
		}
		else if (1 == index){
			setDisplay('div_dslite1_0', 1);
			setDisplay('div_dslite1_1', 1);
		}
		else if (2 == index){
			setDisplay('div_dslite2_0', 1);
			setDisplay('div_dslite2_1', 1);
		}
	}
}

function doDSLiteDisable(index)
{
	var form=document.Alpha_WAN;	
	form.wanSaveFlag.value = 0;
	//form.submit();
	with (document.Alpha_WAN) {
		if (0 == index){
			setDisplay('div_dslite0_0', 0);
			setDisplay('div_dslite0_1', 0);
		}
		else if (1 == index){
			setDisplay('div_dslite1_0', 0);
			setDisplay('div_dslite1_1', 0);
		}
		else if (2 == index){
			setDisplay('div_dslite2_0', 0);
			setDisplay('div_dslite2_1', 0);
		}
	}
}

function doDefaultRouteNo()
{
	var form=document.Alpha_WAN;
	value = form.isDSLITESupported.value;
	if(value == 1){
		if(form.wanTypeRadio[0].checked){
			if(form.DSLITEEnableRadio0[0].checked){
				form.DSLITEModeRadio0[0].disabled = true;
				form.DSLITEModeRadio0[1].disabled = true;
				form.DSLITEAddr0.disabled = true;
			}
			form.DSLITEEnableRadio0[0].disabled = true;
			form.DSLITEEnableRadio0[1].disabled = true;
		}
		else if(form.wanTypeRadio[1].checked){
			if(form.DSLITEEnableRadio1[0].checked){
				form.DSLITEModeRadio1[0].disabled = true;
				form.DSLITEModeRadio1[1].disabled = true;
				form.DSLITEAddr1.disabled = true;
			}
			form.DSLITEEnableRadio1[0].disabled = true;
			form.DSLITEEnableRadio1[1].disabled = true;
		}
		else if(form.wanTypeRadio[2].checked){
			if(form.DSLITEEnableRadio2[0].checked){
				form.DSLITEModeRadio2[0].disabled = true;
				form.DSLITEModeRadio2[1].disabled = true;
				form.DSLITEAddr2.disabled = true;
			}
			form.DSLITEEnableRadio2[0].disabled = true;
			form.DSLITEEnableRadio2[1].disabled = true;
		}
	}
	
}

function doDefaultRouteYes()
{
	var form=document.Alpha_WAN;
	value = form.isDSLITESupported.value;
	if(value == 1){
		if(form.wanTypeRadio[0].checked){
			if(form.DSLITEEnableRadio0[0].checked){
				form.DSLITEModeRadio0[0].disabled = false;
				form.DSLITEModeRadio0[1].disabled = false;
				form.DSLITEAddr0.disabled = false;
			}
			form.DSLITEEnableRadio0[0].disabled = false;
			form.DSLITEEnableRadio0[1].disabled = false;
		}
		else if(form.wanTypeRadio[1].checked){
			if(form.DSLITEEnableRadio1[0].checked){
				form.DSLITEModeRadio1[0].disabled = false;
				form.DSLITEModeRadio1[1].disabled = false;
				form.DSLITEAddr1.disabled = false;
			}
			form.DSLITEEnableRadio1[0].disabled = false;
			form.DSLITEEnableRadio1[1].disabled = false;
		}
		else if(form.wanTypeRadio[2].checked){
			if(form.DSLITEEnableRadio2[0].checked){
				form.DSLITEModeRadio2[0].disabled = false;
				form.DSLITEModeRadio2[1].disabled = false;
				form.DSLITEAddr2.disabled = false;
			}
			form.DSLITEEnableRadio2[0].disabled = false;
			form.DSLITEEnableRadio2[1].disabled = false;
		}
	}
}


function doDSLiteModeAuto()
{
	var form=document.Alpha_WAN;
	if(form.wanTypeRadio[0].checked){
		form.DSLITEAddr0.disabled = true;
	}
	else if(form.wanTypeRadio[1].checked){
		form.DSLITEAddr1.disabled = true;
	}
	else if(form.wanTypeRadio[2].checked){
		form.DSLITEAddr2.disabled = true;
	}
}

function doDSLiteModeManual()
{
	var form=document.Alpha_WAN;
	if(form.wanTypeRadio[0].checked){
		form.DSLITEAddr0.disabled = false;
	}
	else if(form.wanTypeRadio[1].checked){
		form.DSLITEAddr1.disabled = false;
	}
	else if(form.wanTypeRadio[2].checked){
		form.DSLITEAddr2.disabled = false;
	}
}

function check_wan_vid()
{
	var form=document.Alpha_WAN;
	if((form.wan_8021q.value==1) && (form.disp_wan_8021q.value==1) && (form.wan_dot1q[0].checked)){
		value = form.wan_vid.value;
	
		if(!isNumeric(value)){
			alert('VLANID must be a non-negative integer');
			return false;			
		}

		if(parseInt(value) > 4095  || parseInt(value) < 0)
		{
			alert('VLANID must be in the range 0~4095');
			return false;
		}
	}
	return true;
}

<% if tcWebApi_get("WebCustom_Entry", "isMultiVLanSupported", "h") = "Yes" then %>
function isNumericPlus(s)
{
  var len= s.length;
  var ch;
  if(len==0)
    return false;
    
  i=0;
  ch = s.charAt(i);

  if(ch == '-'){
    if(len == 1)
      return false;
    else
      i++;
  }
	
  for(; i< len; i++)
  {
    ch= s.charAt(i);
    if( ch > '9' || ch < '0')
    {
      return false;
    }
  }
  return true;
}

function check_wan_mvlan()
{
	var form=document.Alpha_WAN;
	value = form.wan_mvlan.value;
	
	if(!isNumericPlus(value)){
		alert('Multi VLan must be in the range -1~4095');
		return false;			
	}

	if(parseInt(value) > 4095  || parseInt(value) < -1)
	{
		alert('Multi VLan must be in the range -1~4095');
		return false;
	}
	return true;
}
<%end if%>
<% if tcWebApi_get("WebCustom_Entry", "isTPIDSupported", "h") = "Yes" then %>
function check_wan_tpid()
{
	var form=document.Alpha_WAN;
	var tpid = form.wan_tpid.value;
	var tmp = tpid.toUpperCase();
	form.wan_tpid.value = tmp;
	if((form.wan_8021q.value==1) && (form.disp_wan_8021q.value==1) && (form.wan_dot1q[0].checked)){
		var findpos = tpid.search("^([0-9A-Fa-f]{1}){4}$");
		if(findpos != 0){
			alert('Tpid must be hex');
			return false;			
		}
	}
	return true;
}
<%end if%>
function check_wan_dot1p()
{
	var form=document.Alpha_WAN;
	if((form.wan_8021q.value==1) && (form.disp_wan_8021q.value==1) && (form.wan_dot1q[0].checked)){
		value = form.wan_dot1p.value;
	
		if(!isNumeric(value)){
			alert('8021.P must be a non-negative integer');
			return false;			
		}

		if(parseInt(value) > 7  || parseInt(value) < 0)
		{
			alert('802.1P must be in the range 0~7');
			return false;
		}
	}
	return true;
}

function uiSave() {
	var value;
	var form=document.Alpha_WAN;
	var pvc, barrier, hasAtm=0, hasPtm=0, hasWan0=0, hasPon = 0;
	var vpi,vci;
	var IPAddrValue,maskValue;

  	<%if tcwebApi_get("WebCustom_Entry", "isCZGeneralSupported", "h")="Yes" Then%>
		if(!confirm("Are you sure you want to save these configurations?"))
			return;
	<%end if%>	
<%if tcWebApi_get("Wan_Common","TransMode","h") = "ATM" then%>
	vpi = form.Alwan_VPI.value;
	vci = form.Alwan_VCI.value;

	if(isNaN(vpi) || isNaN(vci))
	{
		alert("VPI or VCI must be number");
		return;
	}
<%end if%>

<%if tcWebApi_get("WebCustom_Entry","haveAtm","h") = "Yes" then%>
	hasAtm = 1;
<%end if%>
<%if tcWebApi_get("WebCustom_Entry","havePtm","h") = "Yes" then%>
	hasPtm = 1;
<%end if%>
<%if tcWebApi_get("WebCustom_Entry","haveWan0","h") = "Yes" then%>
	hasWan0 = 1;
<%end if%>
<%if tcWebApi_get("WebCustom_Entry","haveXPON","h") = "Yes" then%>
	hasPon = 1;
<%end if%>
if (hasPon==1)
	pvc = 8;
else{
if(hasAtm==1 && hasPtm==1 && hasWan0==1){
//AtmPtmEther
	pvc=form.wan_TransMode.selectedIndex;
	if(pvc==1){ //ptm
	//	barrier = document.Alpha_WAN.ptm_Barrier.selectedIndex;
	//	if(barrier==0)
			pvc = 8;
	//	else
	//		pvc = 9;
	}
	else if(pvc==2) //ether
		pvc = 10;
}

if(hasAtm==1 && hasPtm==1 && hasWan0==0){
//AtmPtm, no Ether
	pvc=form.wan_TransMode.selectedIndex;
	if(pvc==1){ //ptm
	//	barrier = document.Alpha_WAN.ptm_Barrier.selectedIndex;
	//	if(barrier==0)
			pvc = 8;
	//	else
	//		pvc = 9;
	}
}

if(hasAtm==1 && hasPtm==0 && hasWan0==1){
//AtmEther, no Ptm
	pvc=form.wan_TransMode.selectedIndex;
	if(pvc==1) //ether
		pvc = 10;
}

if(hasAtm==0 && hasPtm==1 && hasWan0==1){
//PtmEther, no Atm
	pvc=form.wan_TransMode.selectedIndex;
	if(pvc==0){ //ptm
	//	barrier = document.Alpha_WAN.ptm_Barrier.selectedIndex;
	//	if(barrier==0)
			pvc = 8;
	//	else
	//		pvc = 9;
	}
	else if(pvc==1) //ether
		pvc = 10;
}

if(hasAtm==0 && hasPtm==1 && hasWan0==0){
//Ptm, no AtmEther
	//barrier = document.Alpha_WAN.ptm_Barrier.selectedIndex;
	//if(barrier==0)
		pvc = 8;
	//else
	//	pvc = 9;
}

if(hasAtm==0 && hasPtm==0 && hasWan0==1){
//Ether, no AtmPtm
	pvc = 10;
}
}
<%if tcWebApi_get("Wan_Common","TransMode","h") = "ATM" then%>
	if(!check_vci())
		return;
	   	
	if (!pvcDoValidatePageWAN() || !QoSDoValidatePageWAN())
		return;
<%else%>
	//ptm,ether
	form.ptm_VC.value = pvc;

<%end if%>
	if(!check_wan_vid())
	   return;

<% if tcWebApi_get("WebCustom_Entry", "isMultiVLanSupported", "h") = "Yes" then %>
	if(!check_wan_mvlan())
	   return;
<%end if%>

<% if tcWebApi_get("WebCustom_Entry", "isTPIDSupported", "h") = "Yes" then %>
	if(!check_wan_tpid())
	   return;
<%end if%>

<% if tcWebApi_get("WebCustom_Entry", "isdot1pSupport", "h") = "Yes" then %>
	if(!check_wan_dot1p())
	   return;

<%end if%>
<%if tcWebApi_get("Wan_Common","CDVT_Enable","h") = "1" then%>
	switch(document.Alpha_WAN.Alwan_QoS.selectedIndex){
		case 1:
		case 2:
		case 3:
                value = document.Alpha_WAN.wan_CDVT.value
                if(!isNumeric(value))
                {
                	alert("CDVT must be Interger");
                	return false;
                }
                else
                {
                	cdvt = parseInt(value);
					if(cdvt > 65535 || cdvt < 0)
					{
						alert("CDVT Range: 0 <= CDVT <= 65535");
						return false;
					}
                }
	 break;

     default:
	 break;
	}
<%end if%>

	if(form.wanTypeRadio[0].checked)
	{
		if(!isNumeric(form.wan_TCPMTU0.value))
		{
			alert("TCP MTU must be digits!");
			return false;
		}
		MTU = parseInt(form.wan_TCPMTU0.value);
	    if((MTU > 1500 || MTU < 576) && MTU != 0)
	    {
	    	alert("The range of TCP MTU: 576 ~ 1500 or 0 as default value");
	    	return false;
	    }
		
		//jrchen add for IPv6 check
		value = form.isIPv6Supported.value;
		if(value == 1){			
			if(form.ipVerRadio[1].checked || form.ipVerRadio[2].checked){
				//jrchen add
				//check IPv6 Address format	
				form.DynIPv6Enable_flag.value = form.DynIPv6EnableRadio.value;
				value = form.isDSLITESupported.value;
				if(value == 1){
					if(form.ipVerRadio[2].checked && form.DSLITEEnableRadio0[0].checked){							
						if(form.DSLITEModeRadio0[1].checked){
							value = form.DSLITEAddr0.value;		
							if(inValidIPv6Addr(value))
							return false;
						}
					}
				}	   
			}
		}
	}
	
	if(form.wanTypeRadio[1].checked)
	{
		//jrchen add for IPv6 check
		value = form.isIPv6Supported.value;
		if(value == 1){
			if(form.ipVerRadio[0].checked || form.ipVerRadio[1].checked){
				value = form.wan_StaticIPaddr1.value;
				if (inValidIPAddr(value))
					return;
							
				value = form.wan_StaticIPSubMask1.value;
				if (inValidSubnetMask(value))
					return;
						
				value = form.wan_StaticIpGateway1.value;
				if (inValidIPAddr(value))
					return;
				<%if tcwebApi_get("WebCustom_Entry","isWanDNSEachPVC","h") = "Yes" then%>
				value = form.PrimaryDNS1.value;
				if (inValidIPAddr(value))
					return;
			    
				value = form.SecondaryDNS1.value;
				if (inValidIPAddr(value))
					return;
				<%end if%>	
				IPAddrValue =  form.wan_StaticIPaddr1.value;
				maskValue = form.wan_StaticIPSubMask1.value;
				if(inValidStaticIPSubNet(IPAddrValue,maskValue))
					return;
			}
		}
		else{
				value = form.wan_StaticIPaddr1.value;
				if (inValidIPAddr(value))
					return;
							
				value = form.wan_StaticIPSubMask1.value;
				if (inValidSubnetMask(value))
					return;
						
				value = form.wan_StaticIpGateway1.value;
				if (inValidIPAddr(value))
					return;
				<%if tcwebApi_get("WebCustom_Entry","isWanDNSEachPVC","h") = "Yes" then%>
				value = form.PrimaryDNS1.value;
				if (inValidIPAddr(value))
					return;

				value = form.SecondaryDNS1.value;
				if (inValidIPAddr(value))
					return;
				<%end if%>		
				IPAddrValue =  form.wan_StaticIPaddr1.value;
				maskValue = form.wan_StaticIPSubMask1.value;
				if(inValidStaticIPSubNet(IPAddrValue,maskValue))
					return;
				
		}
		if(!isNumeric(form.wan_TCPMTU1.value))
		{
			alert("TCP MTU must be digits!");
			return false;
		}
		MTU = parseInt(form.wan_TCPMTU1.value);
	    if((MTU > 1500 || MTU < 100) && MTU != 0)
	    {
	    	alert("The range of TCP MTU: 100 ~ 1500 or 0 as default value");
	    	return false;
	    }		
		
		//jrchen add for IPv6 check
		value = form.isIPv6Supported.value;
		if(value == 1){			
			if(form.ipVerRadio[1].checked || form.ipVerRadio[2].checked){
				//jrchen add
				//check IPv6 Address format	
				value = form.wan_IPv6Addr.value;
				if(inValidIPv6Addr(value))
					return false;
		
				//check IPv6 Prefix
				value = form.wan_IPv6Prefix.value;
				if(inValidIPv6Prefix(value))
					return false;
 		
				//check IPV6 Gateway Address format
				value = form.wan_IPv6DefGw.value;		
				if(inValidIPv6Addr(value))
				return false;
				
				value = form.wan_IPv6DNS1.value;
				<%if tcwebApi_get("WebCustom_Entry","isCZGeneralSupported","h")="Yes" then%>
				if(value.length!=0 && inValidIPv6Addr(value))
				<%else%>
				if(inValidIPv6Addr(value))
				<%end if%>
				return false;
				
				
				value = form.wan_IPv6DNS2.value;
				<%if tcwebApi_get("WebCustom_Entry","isCZGeneralSupported","h")="Yes" then%>
				if(value.length!=0 && inValidIPv6Addr(value))
				<%else%>
				if(inValidIPv6Addr(value))
				<%end if%>
				return false;
				value = form.isDSLITESupported.value;
				if(value == 1){
				if(form.ipVerRadio[2].checked && form.DSLITEEnableRadio1[0].checked){
					value = form.DSLITEAddr1.value;		
					if(inValidIPv6Addr(value))
					return false;
				}
				}
							
				
					   
			}
		}
	}
	else if(form.wanTypeRadio[2].checked)
	{
		if(idleTimeCheck())
			return ;
	if(form.TTNETGuiSupport.value ==1 ){
		if(form.wan_PPPDomain.selectedIndex == 0)
		{
			form.username1.value = form.wan_PPPUsername.value + "@ttnet";	
		}
		if(form.wan_PPPDomain.selectedIndex == 1)
		{
			if(TtnetCompanyCheck(form.wan_CompanyName))
				return;
			if(DomainRangeCheck(document.Alpha_WAN.wan_CompanyName.value) == true)
			{
				document.getElementById("PPPUsername_ErrorMsg_TR").style.display = "";
				return;
			}else{
				document.getElementById("PPPUsername_ErrorMsg_TR").style.display = "none";
				}
			form.username1.value = form.wan_PPPUsername.value + form.wan_CompanyName.value;
		}
	}
		if(isValidNameEx(form.wan_PPPUsername.value) == false ){
			alert("username invalid!");
			return;
		}
		if(isValidNameEx(form.wan_PPPPassword.value) == false ){
			alert("password invalid!");
			return;
		}
		if(form.wan_PPPUsername.value.length <= 0 || form.wan_PPPPassword.value.length <= 0){
  	  alert("please input username and password");
  	  return;
    }
		if(!isNumeric(form.wan_TCPMSS.value)){
			alert("TCP MSS must be digits!");
			return false;			
		}
		MSS = parseInt(form.wan_TCPMSS.value);
		if((MSS > 1452 || MSS < 100) && MSS != 0)
		{
			alert("The range of TCP MSS: 100 ~ 1452 or 0 as default value");
			return false;
		}
		if(form.ipv6SupportValue.value =="0")
		{
			if(!isNumeric(form.wan_TCPMTU2.value))
			{
				alert("TCP MTU must be digits!");
				return false;
			}
			MTU = parseInt(form.wan_TCPMTU2.value);
			if((MTU > 1492 || MTU < 100) && MTU!=0)
			{
				alert("The range of TCP MTU: 100~1492 or 0 as default value");
				return false;
			}
			if(( MSS!=0 && MTU!=0 && MSS > MTU-40 ) || (MSS==0 && MTU!=0 &&MTU!=1492))
			{
				alert("The range of TCP MSS must less than TCP MTU-40(tcp and ip header)");
				return false;
			}
		}
		value = form.isIPv6Supported.value;
		if(value == 1){
			if(form.ipVerRadio[0].checked || form.ipVerRadio[1].checked){
				if(form.wan_PPPGetIP[0].checked)
				{
					value = form.wan_StaticIPaddr2.value;
					if (inValidIPAddr(value))
						return;
					value = form.wan_StaticIPSubMask2.value;
					if (inValidSubnetMask(value))
						return;
					value = form.wan_StaticIpGateway2.value;
					if (inValidIPAddr(value))
						return;
					IPAddrValue =  form.wan_StaticIPaddr2.value;
					maskValue = form.wan_StaticIPSubMask2.value;
					if(inValidStaticIPSubNet(IPAddrValue,maskValue))
						return;
					
				}
			}
		}
		else{
			if(form.wan_PPPGetIP[0].checked)
			{
				value = form.wan_StaticIPaddr2.value;
				if (inValidIPAddr(value))
					return;
				value = form.wan_StaticIPSubMask2.value;
				if (inValidSubnetMask(value))
					return;
				value = form.wan_StaticIpGateway2.value;
				if (inValidIPAddr(value))
					return;				
				IPAddrValue =  form.wan_StaticIPaddr2.value;
				maskValue = form.wan_StaticIPSubMask2.value;
				if(inValidStaticIPSubNet(IPAddrValue,maskValue))
					return;
			}
		}
		
		//jrchen add for IPv6 check
		value = form.isIPv6Supported.value;
		if(value == 1){
			if(form.ipVerRadio[1].checked || form.ipVerRadio[2].checked)
			{			
			  if(!PPPv6Enable())
			  	return false;
				value = form.isDSLITESupported.value;
				if(value == 1){
				if(form.ipVerRadio[2].checked && form.DSLITEEnableRadio2[0].checked){
					if(form.DSLITEModeRadio2[1].checked){
						value = form.DSLITEAddr2.value;		
						if(inValidIPv6Addr(value))
						return false;
					}
				}
				}
			}
		}
}
//EAP Identity validation
//if (form.wanTypeRadio[0].checked || form.wanTypeRadio[1].checked) {
//    if (form.wan_status.options[1].selected) {
//        var x = form.wan_eapIdentity.value
//        var reg = /^([A-Za-z0-9_\-\.])+\@([A-Za-z0-9_\-\.])+\.([A-Za-z]{2,4})$/;
//
//        if (reg.test(x) == false) {
//            alert("Not a valid Identity. Please enter valid e-mail address for EAP Identity field");
//            return false;
//        }
//    }
//}
	form.wanSaveFlag.value = 1;
	form.wanVCFlag.value = "3";
	form.submit();
}
function inValidStaticIPSubNet(staticIP,staticMask)
{
<%if tcwebApi_get("WebCustom_Entry","isWANIPNetWorkCheck","h") = "Yes" then%>
	var cmpIP,cmpMask;			
	var form=document.Alpha_WAN;
	
	cmpIP =  form.viewLanIPAddr.value;
	cmpMask = form.viewLanNetMask.value;
	if(invalidSubNet(staticIP,staticMask,cmpIP,cmpMask))
	{
		alert("Static IP and LAN IP in the same Subset");
		return true;
	}
	
	if(form.viewLanAliasIPAddr.value == "0.0.0.0"||form.viewLanAliasIPAddr.value == "N/A")
	{
			return false;
	}
	else
	{
		cmpIP =  form.viewLanAliasIPAddr.value;
		cmpMask = form.viewLanAliasNetMask.value;
		if(invalidSubNet(staticIP,staticMask,cmpIP,cmpMask))
		{
			alert("Static IP and LAN Alias IP in the same Subset");
			return true;
		}
	}								
<% end if%>
	
	return false;
}
				
<%if tcwebApi_get("WebCustom_Entry","isWANIPNetWorkCheck","h") = "Yes" then%>
function invalidSubNet(staticIP,staticMask,cmpIP,cmpMask)
{
	var digitsFirstIP,digitsFirstMask,digitsSecondIP,digitsSecondMask;
	var digitsFirstIPtemp,digitsFirstMasktemp,digitsSecondIPtemp,digitsSecondMasktemp;
	var i;

	digitsFirstIPtemp=staticIP.match("^[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}$");
	digitsFirstMasktemp=staticMask.match("^[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}$");
	digitsSecondIPtemp=cmpIP.match("^[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}$");
	digitsSecondMasktemp=cmpMask.match("^[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}$");	
	digitsFirstIP = digitsFirstIPtemp[0].split(".");
	digitsFirstMask = digitsFirstMasktemp[0].split(".");
	digitsSecondIP = digitsSecondIPtemp[0].split(".");
	digitsSecondMask = digitsSecondMasktemp[0].split(".");
	
	for(i=0;i<4;i++)
	{	

		if(((Number(digitsFirstIP[i]))&(Number(digitsFirstMask[i]))) != ((Number(digitsSecondIP[i]))&(Number(digitsSecondMask[i]))))
			return false;
	}
 return true;
}

<% end if%>

function PPPv6Enable()
{
	var form=document.Alpha_WAN;
	//form.PPPIPv6EnableRadio[1].checked 
	if( form.PPPDHCPv6Enable_Flag.value == 1 )
	{
		value = processModeVar();
		if(!value)
			return false;						   
	}
	return true;
}


function doDisableDynIPv6()
{
	var form=document.Alpha_WAN;
	if( form.DynIPv6EnableRadio[0].ckecked)
	{
		form.DynIPv6Enable_flag.value = 0;
	}
}

function doEnableDynIPv6()
{
	var form=document.Alpha_WAN;
	if( form.DynIPv6EnableRadio[1].ckecked)
	{
		form.DynIPv6Enable_flag.value = 1;
	}
}

function processModeVar()
{
	var value;
	var form=document.Alpha_WAN;
	if(form.PPPIPv6ModeRadio[2].checked)
	{
		value = processIPV6format();
		if(!value)
			return false;
	}
	return true;
}

function processIPV6format()
{
	var value;
	var form=document.Alpha_WAN;
	
	value = form.wan_IPv6Addr.value;
	if(inValidIPv6Addr(value))
		return false;
	
	value = form.wan_IPv6Prefix.value;
	if(inValidIPv6Prefix(value))
		return false;
 		
	value = form.wan_IPv6DefGw.value;		
	if(inValidIPv6Addr(value))
		return false;
		
	value = form.wan_IPv6DNS1.value;		
	if(inValidIPv6Addr(value))
		return false;
		
	value = form.wan_IPv6DNS2.value;		
	if(inValidIPv6Addr(value))
		return false;
		
	return true;
}
function valDoValidateNum(Num) {
    var number = Num.value.match("^[0-9]{1,5}$");
    if (number == null) {
        alert('Invalid number format!');
        Num.value = 0;
    }
}

function pppStaticCheck() {
	var form=document.Alpha_WAN;
	value = form.isIPv6Supported.value;
	if(value == 1){
		if(form.ipVerRadio[0].checked || form.ipVerRadio[1].checked){
			var value = document.Alpha_WAN.wan_PPPGetIP[1].checked;
			var form=document.Alpha_WAN;
			if (value) {
				form.wan_StaticIPaddr2.disabled = true;
				form.wan_StaticIPaddr2.value = "0.0.0.0";
				form.wan_StaticIPSubMask2.disabled = true;
				form.wan_StaticIPSubMask2.value = "0.0.0.0";
				form.wan_StaticIpGateway2.disabled = true;
				form.wan_StaticIpGateway2.value = "0.0.0.0";	
			} else {
				form.wan_StaticIPaddr2.disabled = false;
				form.wan_StaticIPSubMask2.disabled = true;
				form.wan_StaticIPSubMask2.value = "255.255.255.255";
				form.wan_StaticIpGateway2.disabled = false;
			}
		}
	}
	else{
			var value = document.Alpha_WAN.wan_PPPGetIP[1].checked;
			var form=document.Alpha_WAN;
			if (value) {
				form.wan_StaticIPaddr2.disabled = true;
				form.wan_StaticIPaddr2.value = "0.0.0.0";
				form.wan_StaticIPSubMask2.disabled = true;
				form.wan_StaticIPSubMask2.value = "0.0.0.0";
				form.wan_StaticIpGateway2.disabled = true;
				form.wan_StaticIpGateway2.value = "0.0.0.0";	
			} else {
				form.wan_StaticIPaddr2.disabled = false;
				form.wan_StaticIPSubMask2.disabled = true;
				form.wan_StaticIPSubMask2.value = "255.255.255.255";
				form.wan_StaticIpGateway2.disabled = false;
			}
	}
}

// Check if a name valid
function isValidNameEx(name) {
   var i = 0;	
   
   for ( i = 0; i < name.length; i++ ) {
      if ( isNameUnsafeEx(name.charAt(i)) == true ){
		return false;
	}
   }

   return true;
}

function isNameUnsafeEx(compareChar)
{	
   if ( compareChar.charCodeAt(0) > 32
        && compareChar.charCodeAt(0) < 127)
      return false; // found no unsafe chars, return false
   else
      return true;
}

function quotationCheck(object) {
	var len = object.value.length;
	var c;
	var i;
    for (i = 0; i < len; i++)
    {
	 	 var c = object.value.charAt(i);
      
	 	 if (c == '"' || c == '$' || c == '\\' || c == '`')
		 {
				alert('Can\'t input "quotation marks" or "dollar sign" or "backslash" or "acute accent" character!!');      	  								    	    	   		
		 		return true;
		 }
    }
    
	return false;
}

function wanVidOper(onOff){
	var value;
		 		
	if(onOff == 0)
		value = true;
	else
		value = false;

	document.Alpha_WAN.wan_vid.disabled=value;
	
	if (document.Alpha_WAN.isdot1pSupport.value == "Yes")
		document.Alpha_WAN.wan_dot1p.disabled=value;
	
	if (document.Alpha_WAN.isWanTagChk.value == "Yes")
		document.Alpha_WAN.TAGSEL.disabled = value;
		
	if (document.Alpha_WAN.isTPIDSupported.value == "Yes")
		document.Alpha_WAN.wan_tpid.disabled=value;		
		
}

function wan8021QCheck() {
	var form=document.Alpha_WAN;
	if((form.wan_8021q.value == 1) && (form.disp_wan_8021q.value==1)){
		if(form.wan_dot1q[1].checked){
			wanVidOper(0);//disabled vlan id controls
		}
	}
}

function doLoad() {
	var value = document.Alpha_WAN.wanTypeRadio[2].checked;
<%if tcWebApi_get("Wan_Common","TransMode","h") = "ATM" then%>
	QosCheck();
<%end if%>
	if (value)
		pppStaticCheck();
		
	if(document.Alpha_WAN.wan_PPPDomain != null && document.Alpha_WAN.wan_CompanyName != null){
		splitPPPUsername();
		var	companylistindex = document.Alpha_WAN.wan_PPPDomain.selectedIndex;
		if(companylistindex == 0)
		{
			document.Alpha_WAN.wan_CompanyName.style.display = "none";
			document.Alpha_WAN.wan_PPPDomain.style.width = "70px";
			document.getElementById("IFrame1").style.display = "none";
		}
	else
	{
		document.Alpha_WAN.wan_CompanyName.style.display = "";
		document.Alpha_WAN.wan_PPPDomain.style.width = "119px";
		document.getElementById("IFrame1").style.display = "";
	}
	}
	document.Alpha_WAN.hidEncapFlag.value = "0";
	doConTypeChange();
<%if tcWebApi_get("Wan_Common","TransMode","h") = "ATM" then%>
	doEncapChange();
<%end if%>
	value = document.Alpha_WAN.is8021xsupport.value;
	if (value == 1) {
	    doauthenticationChange();
	    doStatusChange();
	}

	value = document.Alpha_WAN.isDSLITESupported.value;
	if(value == 1){
	if(document.Alpha_WAN.ipVerRadio[2].checked && (document.Alpha_WAN.wanTypeRadio[0].checked || document.Alpha_WAN.wanTypeRadio[2].checked)){
		if(document.Alpha_WAN.WAN_DefaultRoute0[1].checked || (document.Alpha_WAN.WAN_DefaultRoute2[1].checked)){
			doDefaultRouteNo();
		}
		else{
			if((document.Alpha_WAN.DSLITEEnableRadio0[0].checked) || (document.Alpha_WAN.DSLITEEnableRadio2[0].checked)){
				if((document.Alpha_WAN.DSLITEModeRadio0[0].checked) || (document.Alpha_WAN.DSLITEModeRadio2[0].checked)){
					doDSLiteModeAuto();
				}
				else if((document.Alpha_WAN.DSLITEModeRadio0[1].checked) || (document.Alpha_WAN.DSLITEModeRadio2[1].checked)){
					doDSLiteModeManual();
				}
			}
		}
	}
	}
	if(document.Alpha_WAN.wan_PPPPassword != null)
		document.Alpha_WAN.wan_PPPPassword.value = pppPwd;
		
	<%if tcWebApi_get("WebCustom_Entry", "isFonV2Support", "h")="Yes" then %>
	var fon_pvc = "<% tcwebApi_get("FonNet_Entry","fon_pvcs","s") %>";
	var pvcs = new Array();
	pvcs = fon_pvc.split(" ");
	for(i = 0; i < pvcs.length; i++)
	{
		if(document.Alpha_WAN.wan_VC.selectedIndex == pvcs[i])
		{
			for(j = 0; j < document.Alpha_WAN.elements.length; j++)
	{			
				document.Alpha_WAN.elements[j].disabled = true;
			}
			document.Alpha_WAN.wan_VC.disabled = false;
			document.Alpha_WAN.wanVCFlag.disabled = false;
		}
	}
	<%end if%>
}

function doDelete() {
	document.Alpha_WAN.wanVCFlag.value = 2;
    document.Alpha_WAN.submit();
}
function doPrivacyaddrsShow0() 
{
	if(document.Alpha_WAN.DynIPv6EnableRadio[0].checked)
		setDisplay('div_privacyaddrs0',0);
	else
		setDisplay('div_privacyaddrs0',1);
}
function doPrivacyaddrsShow2()
{
	if(document.Alpha_WAN.PPPIPv6ModeRadio[0].checked)
		setDisplay('div_privacyaddrs2',0);
	else
		setDisplay('div_privacyaddrs2',1);
}
function ripngEnableChanged0() 
{
	if(document.Alpha_WAN.ripngEnableRadio0[0].checked)
		setDisplay('div_ripng_direction0', 1);
	else
		setDisplay('div_ripng_direction0', 0);
}
function ripngEnableChanged1() 
{
	if(document.Alpha_WAN.ripngEnableRadio1[0].checked)
		setDisplay('div_ripng_direction1', 1);
	else
		setDisplay('div_ripng_direction1', 0);
}

function ripngEnableChanged2() 
{
	if(document.Alpha_WAN.ripngEnableRadio2[0].checked)
		setDisplay('div_ripng_direction2', 1);
	else
		setDisplay('div_ripng_direction2', 0);
}

</script>

<body onLoad="doLoad()">
<FORM METHOD="POST" ACTION="/cgi-bin/home_wan.asp" name="Alpha_WAN">
<div align=center>
<table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
	<%if tcWebApi_get("WebCustom_Entry", "haveXPON", "h") <> "Yes" then%>
	<INPUT TYPE="HIDDEN" NAME="VCI0" VALUE='<%if tcWebApi_get("Wan_PVC0","Active","h") = "Yes" then tcWebApi_get("Wan_PVC0","VCI","s") else asp_write("0") end if%>'>
	<INPUT TYPE="HIDDEN" NAME="VCI1" VALUE='<%if tcWebApi_get("Wan_PVC1","Active","h") = "Yes" then tcWebApi_get("Wan_PVC1","VCI","s") else asp_write("0") end if%>'>
	<INPUT TYPE="HIDDEN" NAME="VCI2" VALUE='<%if tcWebApi_get("Wan_PVC2","Active","h") = "Yes" then tcWebApi_get("Wan_PVC2","VCI","s") else asp_write("0") end if%>'>
	<INPUT TYPE="HIDDEN" NAME="VCI3" VALUE='<%if tcWebApi_get("Wan_PVC3","Active","h") = "Yes" then tcWebApi_get("Wan_PVC3","VCI","s") else asp_write("0") end if%>'>
	<INPUT TYPE="HIDDEN" NAME="VCI4" VALUE='<%if tcWebApi_get("Wan_PVC4","Active","h") = "Yes" then tcWebApi_get("Wan_PVC4","VCI","s") else asp_write("0") end if%>'>
	<INPUT TYPE="HIDDEN" NAME="VCI5" VALUE='<%if tcWebApi_get("Wan_PVC5","Active","h") = "Yes" then tcWebApi_get("Wan_PVC5","VCI","s") else asp_write("0") end if%>'>
	<INPUT TYPE="HIDDEN" NAME="VCI6" VALUE='<%if tcWebApi_get("Wan_PVC6","Active","h") = "Yes" then tcWebApi_get("Wan_PVC6","VCI","s") else asp_write("0") end if%>'>
	<INPUT TYPE="HIDDEN" NAME="VCI7" VALUE='<%if tcWebApi_get("Wan_PVC7","Active","h") = "Yes" then tcWebApi_get("Wan_PVC7","VCI","s") else asp_write("0") end if%>'>

	<INPUT TYPE="HIDDEN" NAME="VPI0" VALUE='<%if tcWebApi_get("Wan_PVC0","Active","h") = "Yes" then tcWebApi_get("Wan_PVC0","VPI","s") else asp_write("0") end if%>'>
	<INPUT TYPE="HIDDEN" NAME="VPI1" VALUE='<%if tcWebApi_get("Wan_PVC1","Active","h") = "Yes" then tcWebApi_get("Wan_PVC1","VPI","s") else asp_write("0") end if%>'>
	<INPUT TYPE="HIDDEN" NAME="VPI2" VALUE='<%if tcWebApi_get("Wan_PVC2","Active","h") = "Yes" then tcWebApi_get("Wan_PVC2","VPI","s") else asp_write("0") end if%>'>
	<INPUT TYPE="HIDDEN" NAME="VPI3" VALUE='<%if tcWebApi_get("Wan_PVC3","Active","h") = "Yes" then tcWebApi_get("Wan_PVC3","VPI","s") else asp_write("0") end if%>'>
	<INPUT TYPE="HIDDEN" NAME="VPI4" VALUE='<%if tcWebApi_get("Wan_PVC4","Active","h") = "Yes" then tcWebApi_get("Wan_PVC4","VPI","s") else asp_write("0") end if%>'>
	<INPUT TYPE="HIDDEN" NAME="VPI5" VALUE='<%if tcWebApi_get("Wan_PVC5","Active","h") = "Yes" then tcWebApi_get("Wan_PVC5","VPI","s") else asp_write("0") end if%>'>
	<INPUT TYPE="HIDDEN" NAME="VPI6" VALUE='<%if tcWebApi_get("Wan_PVC6","Active","h") = "Yes" then tcWebApi_get("Wan_PVC6","VPI","s") else asp_write("0") end if%>'>
	<INPUT TYPE="HIDDEN" NAME="VPI7" VALUE='<%if tcWebApi_get("Wan_PVC7","Active","h") = "Yes" then tcWebApi_get("Wan_PVC7","VPI","s") else asp_write("0") end if%>'>
	<%end if%>	
	<INPUT TYPE="HIDDEN" NAME="hidEncapFlag" VALUE="0">
	<INPUT TYPE="HIDDEN" NAME="hidEncap" VALUE="0">
<!-- jrchen add -->
<INPUT type="HIDDEN" NAME="isIPv6Supported" value="<% if tcWebApi_get("Info_Ether","isIPv6Supported","h") = "Yes" then asp_write("1") else asp_write("0") end if %>">
<!-- jrchen add end-->

	<INPUT type="HIDDEN" NAME="DynIPv6Enable_flag" value="<% tcWebApi_get("Wan_PVC","EnableDynIPv6","s")  %>">

	<INPUT TYPE="HIDDEN" NAME="PPPDHCPv6Enable_Flag" VALUE="<% tcWebApi_get("Wan_PVC","PPPv6Enable","s") %>" >
	<INPUT TYPE="HIDDEN" NAME="PPPDHCPv6Mode_Flag" VALUE='<%if tcWebApi_get("Wan_PVC","PPPv6Mode","h") <> "N/A" then tcWebApi_get("Wan_PVC","PPPv6Mode","s") else asp_write("0") end if%>' >
	<INPUT TYPE="HIDDEN" NAME="IPv6PD_Flag" VALUE="<% tcWebApi_get("Wan_PVC","PPPv6PD","s") %>" >
	
	<INPUT TYPE="HIDDEN" NAME="DHCP6SMode_Flag" VALUE="<% tcWebApi_get("Dhcp6s_Entry","Mode","s") %>" >
	<INPUT TYPE="HIDDEN" NAME="IPVERSION_IPv4" VALUE="IPv4" >
	<INPUT TYPE="HIDDEN" NAME="wanTransFlag" VALUE="0">
	<INPUT TYPE="HIDDEN" NAME="wanBarrierFlag" VALUE="0">
	<INPUT TYPE="HIDDEN" NAME="ptm_VC" VALUE="8">
	<INPUT TYPE="HIDDEN" NAME="wanVCFlag" VALUE="0">
	<INPUT TYPE="HIDDEN" NAME="service_num_flag" VALUE="0">
	<INPUT TYPE="HIDDEN" NAME="wanSaveFlag" VALUE="0">
	<INPUT TYPE="HIDDEN" NAME="vciCheckFlag" VALUE="0">
	<INPUT TYPE="HIDDEN" NAME="wanEncapFlag" VALUE="0">
	<INPUT TYPE="HIDDEN" NAME="DSLITE_MANUAL_MODE" VALUE="1" >
	<INPUT TYPE="HIDDEN" NAME="IPVersion_Flag" VALUE="0" >
	<INPUT type="HIDDEN" NAME="is8021xsupport" value="<% if tcWebApi_get("Info_Ether","is8021xsupport","h") = "Yes" then asp_write("1") else asp_write("0") end if %>">
	<INPUT type="HIDDEN" NAME="isDSLITESupported" value="<% if tcWebApi_get("Info_Ether","isDSLITESupported","h") = "Yes" then asp_write("1") else asp_write("0") end if %>">
	<INPUT type="HIDDEN" NAME="wan_8021q" value="<% if tcWebApi_get("Info_dot1q","wan_8021q","h") = "1" then asp_write("1") else asp_write("0") end if %>" >
	<INPUT type="HIDDEN" NAME="disp_wan_8021q" value="<% if tcWebApi_get("Info_dot1q","disp_wan_8021q","h") = "1" then asp_write("1") else asp_write("0") end if %>" >

	<INPUT TYPE="HIDDEN" NAME="DefaultWan_Active" VALUE="<% tcWebApi_get("DefaultWan_Entry","Active","s") %>" >
	<%if tcWebApi_get("WebCustom_Entry", "haveXPON", "h") <> "Yes" then%>
	<INPUT TYPE="HIDDEN" NAME="DefaultWan_VPI" VALUE="<% tcWebApi_get("DefaultWan_Entry","VPI","s") %>" >
	<INPUT TYPE="HIDDEN" NAME="DefaultWan_VCI" VALUE="<% tcWebApi_get("DefaultWan_Entry","VCI","s") %>" >
	<INPUT TYPE="HIDDEN" NAME="DefaultWan_QOS" VALUE="<% tcWebApi_get("DefaultWan_Entry","QOS","s") %>" >
	<INPUT TYPE="HIDDEN" NAME="DefaultWan_PCR" VALUE="<% tcWebApi_get("DefaultWan_Entry","PCR","s") %>" >
	<INPUT TYPE="HIDDEN" NAME="DefaultWan_SCR" VALUE="<% tcWebApi_get("DefaultWan_Entry","SCR","s") %>" >
	<INPUT TYPE="HIDDEN" NAME="DefaultWan_MBS" VALUE="<% tcWebApi_get("DefaultWan_Entry","MBS","s") %>" >
	<%end if%>
	<%if tcwebApi_get("WebCustom_Entry","isWANIPNetWorkCheck","h") = "Yes" then%>
	<INPUT TYPE="HIDDEN" NAME="viewLanIPAddr" VALUE="<% tcWebApi_get("Lan_Entry","IP","s") %>">
	<INPUT TYPE="HIDDEN" NAME="viewLanNetMask" VALUE="<% tcWebApi_get("Lan_Entry","netmask","s") %>">
	<INPUT TYPE="HIDDEN" NAME="viewLanAliasIPAddr" VALUE="<% tcWebApi_get("LanAlias_Entry","IP","s") %>">
	<INPUT TYPE="HIDDEN" NAME="viewLanAliasNetMask" VALUE="<% tcWebApi_get("LanAlias_Entry","netmask","s") %>">
	<%end if%>
	
<%if tcWebApi_get("Wan_Common","CDVT_Enable","h") = "1" then%>
	<INPUT TYPE="HIDDEN" NAME="DefaultWan_CDVT" VALUE="<% tcWebApi_get("DefaultWan_Entry","CDVT","s") %>" >
<%end if%>
	<INPUT TYPE="HIDDEN" NAME="DefaultWan_ISP" VALUE="<% tcWebApi_get("DefaultWan_Entry","ISP","s") %>" >
	<%if tcWebApi_get("WebCustom_Entry", "haveXPON", "h") <> "Yes" then%>
	<INPUT TYPE="HIDDEN" NAME="DefaultWan_ENCAP" VALUE="<% tcWebApi_get("DefaultWan_Entry","ENCAP","s") %>" >
	<%End If%>
	<INPUT TYPE="HIDDEN" NAME="DefaultWan_IPVERSION" VALUE="<% tcWebApi_get("DefaultWan_Entry","IPVERSION","s") %>" >
	<INPUT TYPE="HIDDEN" NAME="DefaultWan_MLDproxy" VALUE="<% tcWebApi_get("DefaultWan_Entry","MLDproxy","s") %>" >
	<INPUT TYPE="hidden" NAME="ipv6SupportValue" VALUE="<% if tcWebApi_get("Wan_PVC","IPVERSION","h") = "IPv6" then asp_write("1") else asp_write("0") end if%>">		
	<input type="hidden" name="UserMode" value="<% if tcWebApi_Get("WebCurSet_Entry","UserMode","h")<>"1" then asp_write("0") else asp_write("1") end if%>">
	<INPUT TYPE="HIDDEN" NAME="wan_certificate" value="<% tcWebApi_get("Wan_PVC","CERTIFICATE","s") %>">
	<INPUT TYPE="HIDDEN" NAME="wan_CA" value="<% tcWebApi_get("Wan_PVC","TRUSTEDCA","s") %>">
	<INPUT TYPE="HIDDEN" NAME="wan_HiddenBiDirectionalAuth" value="<%tcWebApi_get("Wan_PVC","BIDIRECTIONALAUTHENTICATION","s")%>" >
	<INPUT TYPE="HIDDEN" NAME="IPv6PrivacyAddrsSupportedFlag" value="<%tcWebApi_get("WebCustom_Entry","isIPv6PrivacyAddrsSupported","s")%>" >
	<tr>
		<td height="5" class="light-orange" colspan="5">&nbsp;</td>
	</tr>
<%if tcWebApi_get("WebCustom_Entry","noWanModeDefined","h") <> "Yes" then%>
	<tr>
		<td width="150" height="30" class="title-main"><font color="#FFFFFF">WAN Transfer Mode</font></td>
		<td width="10" class="black">&nbsp;</td>
		<td width="150"></td>
		<td width="10" ></td>
		<td width="440"></td>
	</tr>
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata"><div align=right><font color="#000000">Transfer Modes</font></div></td>
		<td class="tabdata"><div align=center>:</div></td>
		<td class="tabdata">
		<SELECT NAME="wan_TransMode" SIZE="1" onChange="doTransChange()">

	<%if tcWebApi_get("WebCustom_Entry","haveAtm","h") = "Yes" then%>
			<option <% if tcWebApi_get("Wan_Common","TransMode","h") = "ATM" then asp_Write("selected") end if %>>ATM
	<%end if%>
	<%if tcWebApi_get("WebCustom_Entry","havePtm","h") = "Yes" then%>
			<option <% if tcWebApi_get("Wan_Common","TransMode","h") = "PTM" then asp_Write("selected") end if %>>PTM
	<%end if%>
	<%if tcWebApi_get("WebCustom_Entry","haveWan0","h") = "Yes" then%>
			<option <% if tcWebApi_get("Wan_Common","TransMode","h") = "Ethernet" then asp_Write("selected") end if %>>Ethernet
	<%end if%>
	<%if tcWebApi_get("WebCustom_Entry","haveXPON","h") = "Yes" then%>
			<option <% if tcWebApi_get("Wan_Common","TransMode","h") = "PON" then asp_Write("selected") end if %>>Fiber
	<%end if%>
		</SELECT></td>
	</tr>

<%if tcWebApi_get("WebCustom_Entry", "haveXPON", "h") <> "Yes" then%>
<%if tcWebApi_get("Wan_Common","TransMode","h") <> "ATM" then%>

<%if tcWebApi_get("Wan_Common","TransMode","h") = "PTM" then%>
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata"><div align=right><font color="#000000">Barrier</font></div></td>
		<td class="tabdata"><div align=center>:</div></td>
		<td class="tabdata"><div align=left>0</div></td>
	</tr>
<%end if%>

	<%if tcWebApi_get("WebCustom_Entry","isMultiSerSupported","h") = "Yes" then%>
		<tr>
			<td width="150" height="30" class="title-main"><font color="#FFFFFF">MULTI SERVICE</font></td>
			<td width="10" class="black">&nbsp;</td>
			<td width="150"><hr noshade></td>
			<td width="10" ><hr noshade></td>
			<td width="440"><hr noshade></td>
		</tr>
		<tr>
			<td class="light-orange">&nbsp;</td>
			<td class="light-orange"></td>
			<td class="tabdata"><div align=right><font color="#000000">Service Num</font></div></td>
			<td class="tabdata"><div align=center>:</div></td>
			<td class="tabdata">
			<SELECT NAME="service_num" SIZE="1" onChange="doServiceChange()">
				<option <% if tcWebApi_get("WebCurSet_Entry","wan_pvc_ext","h") = "0" then asp_Write("selected") end if %>>0
				<option <% if tcWebApi_get("WebCurSet_Entry","wan_pvc_ext","h") = "1" then asp_Write("selected") end if %>>1
				<option <% if tcWebApi_get("WebCurSet_Entry","wan_pvc_ext","h") = "2" then asp_Write("selected") end if %>>2
				<option <% if tcWebApi_get("WebCurSet_Entry","wan_pvc_ext","h") = "3" then asp_Write("selected") end if %>>3
				<option <% if tcWebApi_get("WebCurSet_Entry","wan_pvc_ext","h") = "4" then asp_Write("selected") end if %>>4
				<option <% if tcWebApi_get("WebCurSet_Entry","wan_pvc_ext","h") = "5" then asp_Write("selected") end if %>>5
				<option <% if tcWebApi_get("WebCurSet_Entry","wan_pvc_ext","h") = "6" then asp_Write("selected") end if %>>6
				<option <% if tcWebApi_get("WebCurSet_Entry","wan_pvc_ext","h") = "7" then asp_Write("selected") end if %>>7
			</SELECT>&nbsp;
			<INPUT TYPE="BUTTON" NAME="Serv_Summary" VALUE="Services Summary" onClick="onClickServiceSummary();">
			<td/>
		</tr>
	<%end if%>
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata"><div align=right><font color="#000000">Status</font></div></td>
		<td class="tabdata"><div align=center>:</div></td>
		<td class="tabdata">
			<INPUT TYPE="RADIO" NAME="wan_TransStatus" VALUE="Yes" <% if tcWebApi_get("Wan_PVC","Active","h") = "Yes" then asp_Write("checked") end if %> ><font color="#000000">Activated</font>
			<INPUT TYPE="RADIO" NAME="wan_TransStatus" VALUE="No" <% if tcWebApi_get("Wan_PVC","Active","h") = "No" then asp_Write("checked") end if if tcWebApi_get("Wan_PVC","Active","h") = "N/A" then asp_Write("checked") end if %> ><font color="#000000">Deactivated</font></td>
	</tr>
<%end if%>
<%End If%><!--end  tcWebApi_get(...haveXPON...) <> "yes" -->
<%end if%> <!--end  tcWebApi_get(...noWanModeDefined...) <> "yes" -->

<%if tcWebApi_get("Wan_Common","TransMode","h") = "PON" then%>
	<tr>
		<td width="150" height="30" class="title-main"><font color="#FFFFFF">xPON</font></td>
		<td width="10" class="black">&nbsp;</td>
	<%if tcWebApi_get("WebCustom_Entry","noWanModeDefined","h") <> "Yes" then%>
		<td width="150"><hr noshade></td>
		<td width="10" ><hr noshade></td>
		<td width="440"><hr noshade></td>
	<%else%>
		<td width="150"></td>
		<td width="10" ></td>
		<td width="440"></td>
	<%end if%>
	</tr>
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange">&nbsp;</td>
		<td class="tabdata"><div align=right><font color="#000000">WAN</font></div></td>
		<td class="tabdata"><div align=center>:</div></td>
		<td class="tabdata">
			<SELECT NAME="wan_VC" SIZE="1" onChange="doVCChange()" style="width: 50px;">
				<OPTION <% if tcWebApi_get("WebCurSet_Entry","wan_pvc","h") = "0" then asp_Write("selected")
					elseif tcWebApi_get("WebCurSet_Entry","wan_pvc","h") = "8" then asp_Write("selected")
					elseif tcWebApi_get("WebCurSet_Entry","wan_pvc","h") = "9" then asp_Write("selected")
					elseif tcWebApi_get("WebCurSet_Entry","wan_pvc","h") = "10" then asp_Write("selected")
					end if %>>0
				<OPTION <% if tcWebApi_get("WebCurSet_Entry","wan_pvc","h") = "1" then asp_Write("selected") end if %>>1
				<OPTION <% if tcWebApi_get("WebCurSet_Entry","wan_pvc","h") = "2" then asp_Write("selected") end if %>>2
				<OPTION <% if tcWebApi_get("WebCurSet_Entry","wan_pvc","h") = "3" then asp_Write("selected") end if %>>3
				<OPTION <% if tcWebApi_get("WebCurSet_Entry","wan_pvc","h") = "4" then asp_Write("selected") end if %>>4
				<OPTION <% if tcWebApi_get("WebCurSet_Entry","wan_pvc","h") = "5" then asp_Write("selected") end if %>>5
				<OPTION <% if tcWebApi_get("WebCurSet_Entry","wan_pvc","h") = "6" then asp_Write("selected") end if %>>6
				<OPTION <% if tcWebApi_get("WebCurSet_Entry","wan_pvc","h") = "7" then asp_Write("selected") end if %>>7
			</SELECT>&nbsp;
			<INPUT TYPE="BUTTON" NAME="PVC_Summary" VALUE="WANs Summary" onClick="onClickPVCSummary();">
	</tr>
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata"><div align=right><font color="#000000">Status</font></div></td>
		<td class="tabdata"><div align=center>:</div></td>
		<td class="tabdata">
			<INPUT TYPE="RADIO" NAME="wan_VCStatus" VALUE="Yes" <% if tcWebApi_get("Wan_PVC","Active","h") = "Yes" then asp_Write("checked") end if %> ><font color="#000000">Activated</font>
			<INPUT TYPE="RADIO" NAME="wan_VCStatus" VALUE="No" <% if tcWebApi_get("Wan_PVC","Active","h") = "No" then asp_Write("checked") end if if tcWebApi_get("Wan_PVC","Active","h") = "N/A" then asp_Write("checked") end if %> ><font color="#000000">Deactivated</font></td>
	</tr>
<%else%><!-- TransMode=="PON" end -->
<%if tcWebApi_get("Wan_Common","TransMode","h") = "ATM" then%>
	<tr>
		<td width="150" height="30" class="title-main"><font color="#FFFFFF">ATM VC</font></td>
		<td width="10" class="black">&nbsp;</td>
	<%if tcWebApi_get("WebCustom_Entry","noWanModeDefined","h") <> "Yes" then%>
		<td width="150"><hr noshade></td>
		<td width="10" ><hr noshade></td>
		<td width="440"><hr noshade></td>
	<%else%>
		<td width="150"></td>
		<td width="10" ></td>
		<td width="440"></td>
	<%end if%>
	</tr>
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange">&nbsp;</td>
		<td class="tabdata"><div align=right><font color="#000000">Virtual Circuit</font></div></td>
		<td class="tabdata"><div align=center>:</div></td>
		<td class="tabdata"><font color="#000000">PVC</font>
			<SELECT NAME="wan_VC" SIZE="1" onChange="doVCChange()">
				<OPTION <% if tcWebApi_get("WebCurSet_Entry","wan_pvc","h") = "0" then asp_Write("selected")
					elseif tcWebApi_get("WebCurSet_Entry","wan_pvc","h") = "8" then asp_Write("selected")
					elseif tcWebApi_get("WebCurSet_Entry","wan_pvc","h") = "9" then asp_Write("selected")
					elseif tcWebApi_get("WebCurSet_Entry","wan_pvc","h") = "10" then asp_Write("selected")
					end if %>>0
				<OPTION <% if tcWebApi_get("WebCurSet_Entry","wan_pvc","h") = "1" then asp_Write("selected") end if %>>1
				<OPTION <% if tcWebApi_get("WebCurSet_Entry","wan_pvc","h") = "2" then asp_Write("selected") end if %>>2
				<OPTION <% if tcWebApi_get("WebCurSet_Entry","wan_pvc","h") = "3" then asp_Write("selected") end if %>>3
				<OPTION <% if tcWebApi_get("WebCurSet_Entry","wan_pvc","h") = "4" then asp_Write("selected") end if %>>4
				<OPTION <% if tcWebApi_get("WebCurSet_Entry","wan_pvc","h") = "5" then asp_Write("selected") end if %>>5
				<OPTION <% if tcWebApi_get("WebCurSet_Entry","wan_pvc","h") = "6" then asp_Write("selected") end if %>>6
				<OPTION <% if tcWebApi_get("WebCurSet_Entry","wan_pvc","h") = "7" then asp_Write("selected") end if %>>7
			</SELECT>&nbsp;
			<INPUT TYPE="BUTTON" NAME="PVC_Summary" VALUE="PVCs Summary" onClick="onClickPVCSummary();">
	</tr>
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata"><div align=right><font color="#000000">Status</font></div></td>
		<td class="tabdata"><div align=center>:</div></td>
		<td class="tabdata">
			<INPUT TYPE="RADIO" NAME="wan_VCStatus" VALUE="Yes" <% if tcWebApi_get("Wan_PVC","Active","h") = "Yes" then asp_Write("checked") end if %> ><font color="#000000">Activated</font>
			<INPUT TYPE="RADIO" NAME="wan_VCStatus" VALUE="No" <% if tcWebApi_get("Wan_PVC","Active","h") = "No" then asp_Write("checked") end if if tcWebApi_get("Wan_PVC","Active","h") = "N/A" then asp_Write("checked") end if %> ><font color="#000000">Deactivated</font></td>
	</tr>
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange">&nbsp;</td>
		<td class="tabdata"><div align=right><font color="#000000">VPI</font></div></td>
		<td class="tabdata"><div align=center>:</div></td>
		<td class="tabdata">
			<INPUT TYPE="TEXT" NAME="Alwan_VPI" SIZE="5" MAXLENGTH="3" VALUE="<%if tcWebApi_get("Wan_PVC","VPI","h") <> "N/A" then tcWebApi_get("Wan_PVC","VPI","s") else asp_Write("0") end if%>" ><font color="#000000">(range: 0~255)</font></td>
	</tr>
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange">&nbsp;</td>
		<td class="tabdata"><div align=right><font color="#000000">VCI</font></div></td>
		<td class="tabdata"><div align=center>:</div></td>
		<td class="tabdata">
			<INPUT TYPE="TEXT" NAME="Alwan_VCI" SIZE="5" MAXLENGTH="5" VALUE="<%if tcWebApi_get("Wan_PVC","VCI","h") <> "N/A" then tcWebApi_get("Wan_PVC","VCI","s") else asp_Write("0") end if%>" >
        <font color="#000000">(range: 32~65535)</font></td>
	</tr>
	<tr>
		<td class="title-sub"><font color="#000000">QoS</font></td>
		<td class="light-orange">&nbsp;</td>
		<td><hr noshade class="light-orange-line"></td>
		<td><hr noshade class="light-orange-line"></td>
		<td><hr noshade class="light-orange-line"></td>
	</tr>
	<tr>
		<td  class="light-orange">&nbsp;</td>
		<td class="light-orange">&nbsp;</td>
		<td class="tabdata"><div align=right><font color="#000000">ATM QoS</font></div></td>
		<td class="tabdata"><div align=center>:</div></td>
		<td class="tabdata">
			<SELECT NAME="Alwan_QoS" SIZE="1" onChange="QosCheck()">
				<option <% if tcWebApi_get("Wan_PVC","QOS","h") = "ubr" then asp_Write("selected") end if %>>ubr
				<option <% if tcWebApi_get("Wan_PVC","QOS","h") = "cbr" then asp_Write("selected") end if %>>cbr
				<option <% if tcWebApi_get("Wan_PVC","QOS","h") = "rt-vbr" then asp_Write("selected") end if %>>rt-vbr
				<option <% if tcWebApi_get("Wan_PVC","QOS","h") = "nrt-vbr" then asp_Write("selected") end if %>>nrt-vbr
			</SELECT></td>
	</tr>
	<tr>
		<td  class="light-orange">&nbsp;</td>
		<td class="light-orange">&nbsp;</td>
		<td class="tabdata"><div align=right><font color="#000000">PCR</font></div></td>
		<td class="tabdata"><div align=center>:</div></td>
		<td class="tabdata">
			<INPUT TYPE="TEXT" NAME="wan_PCR" SIZE="5" MAXLENGTH="7" VALUE="<%if tcWebApi_get("Wan_PVC","PCR","h") <> "N/A" then tcWebApi_get("Wan_PVC","PCR","s") else asp_Write("0") end if%>" ><font color="#000000">cells/second</font></td>
	</tr>
	<tr>
		<td  class="light-orange">&nbsp;</td>
		<td class="light-orange">&nbsp;</td>
		<td class="tabdata"><div align=right><font color="#000000">SCR</font></div></td>
		<td class="tabdata"><div align=center>:</div></td>
		<td class="tabdata">
			<INPUT TYPE="TEXT" NAME="wan_SCR" SIZE="5" MAXLENGTH="7" VALUE="<%if tcWebApi_get("Wan_PVC","SCR","h") <> "N/A" then tcWebApi_get("Wan_PVC","SCR","s") else asp_Write("0") end if%>" ><font color="#000000">cells/second</font></td>
	</tr>
	<tr>
		<td  class="light-orange">&nbsp;</td>
		<td class="light-orange">&nbsp;</td>
		<td class="tabdata"><div align=right><font color="#000000">MBS</font></div></td>
		<td class="tabdata"><div align=center>:</div></td>
		<td class="tabdata">
			<INPUT TYPE="TEXT" NAME="wan_MBS" SIZE="5" MAXLENGTH="7" VALUE="<%if tcWebApi_get("Wan_PVC","MBS","h") <> "N/A" then tcWebApi_get("Wan_PVC","MBS","s") else asp_Write("0") end if%>" ><font color="#000000">cells</font></td>
	</tr>

<% if tcWebApi_get("Wan_Common","CDVT_Enable","h") = "1" then%>
	<tr>
		<td  class="light-orange">&nbsp;</td>
		<td class="light-orange">&nbsp;</td>
		<td class="tabdata"><div align=right><font color="#000000">CDVT</font></div></td>
		<td class="tabdata"><div align=center>:</div></td>
		<td class="tabdata">
			<INPUT TYPE="TEXT" NAME="wan_CDVT" SIZE="5" MAXLENGTH="7" VALUE="<%if tcWebApi_get("Wan_PVC","CDVT","h") <> "N/A" then tcWebApi_get("Wan_PVC","CDVT","s") else asp_Write("0") end if%>" ><font color="#000000">cells</font></td>
	</tr>	
<%end if%>
<%end if%> <!-- TransMode=="ATM" end -->
<%End If%> <!-- TransMode<>"PON" end -->
</table>
<!-- jrchen add -->
<div id="div_isipv6sup">
<table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">	
	<tr>
		<td width="150" height="30" class="title-main"><font color="#FFFFFF">IPv4/IPv6</font></td>
		<td width="10" class="black">&nbsp;</td>
		<td width="150"><hr noshade></td>
		<td width= "10"><hr noshade></td>
		<td width="440"><hr noshade></td>
	</tr>
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata"><div align=right><font color="#000000">IP Version</font></div></td>
		<td class="tabdata"><div align=center>:</div></td>
		<td class="tabdata">
			<INPUT TYPE="RADIO" NAME="ipVerRadio" VALUE="IPv4" <% if tcWebApi_get("Wan_PVC","IPVERSION","h") = "IPv4" then asp_Write("checked") elseif tcWebApi_get("Wan_PVC","IPVERSION","h") = "N/A" then asp_Write("checked") end if %> onClick="doIPVersionChangeIPv4()"><font color="#000000">IPv4</font>
			<INPUT TYPE="RADIO" NAME="ipVerRadio" VALUE="IPv4/IPv6" <% if tcWebApi_get("Wan_PVC","IPVERSION","h") = "IPv4/IPv6" then asp_Write("checked") end if %> onClick="doIPVersionChangeIPv4()"><font color="#000000">IPv4/IPv6</font>
			<input type="RADIO" name="ipVerRadio" value="IPv6" <% if tcWebApi_get("Wan_PVC","IPVERSION","h") = "IPv6" then asp_Write("checked") end if %> onClick="doIPVersionChangeIPv4()"><font color="#000000">IPv6</font></td>
	</tr>
</table></div>
<!-- jrchen add end -->

<div align=center>
<table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
	<tr>
		<td width="150" height="30" class="title-main"><font color="#FFFFFF">Encapsulation</font></td>
		<td width="10" class="black">&nbsp;</td>
		<td width="150"><hr noshade></td>
		<td width= "10"><hr noshade></td>
		<td width="440"><hr noshade></td>
	</tr>
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata"><div align=right><font color="#000000">ISP</font></div></td>
		<td class="tabdata"><div align=center>:</div></td>
		<td class="tabdata">
			<INPUT TYPE="RADIO" NAME="wanTypeRadio" VALUE="0" <% if tcWebApi_get("Wan_PVC","ISP","h") = "0" then asp_Write("checked") end if %> onClick="doConTypeChange(this)"><font color="#000000">Dynamic IP Address</font></td>
	</tr>
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata">&nbsp;</td>
		<td>&nbsp;</td>
		<td class="tabdata">
			<INPUT TYPE="RADIO" NAME="wanTypeRadio" VALUE="1" <% if tcWebApi_get("Wan_PVC","ISP","h") = "1" then asp_Write("checked") end if %> onClick="doConTypeChange(this)"><font color="#000000">Static IP Address</font></td>
	</tr>
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata">&nbsp;</td>
		<td>&nbsp;</td>
		<td class="tabdata">
			<INPUT TYPE="RADIO" NAME="wanTypeRadio" VALUE="2" <% if tcWebApi_get("Wan_PVC","ISP","h") = "2" then asp_Write("checked") end if %> onClick="doConTypeChange(this)"><font color="#000000"><%if tcWebApi_get("Wan_Common","TransMode","h") = "ATM" then%><%if tcWebApi_get("WebCustom_Entry","sharepvc","h") <> "Yes" then%>PPPoA/<%end if%><%End if%>PPPoE</font></td>
	</tr>
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata">&nbsp;</td>
		<td>&nbsp;</td>
		<td class="tabdata">
			<INPUT TYPE="RADIO" NAME="wanTypeRadio" VALUE="3" <% if tcWebApi_get("Wan_PVC","ISP","h") = "3" then asp_Write("checked") end if 
    			if tcWebApi_get("Wan_PVC","ISP","h") = "N/A" then asp_Write("checked") end if%> onClick="doConTypeChange(this)"><font color="#000000">Bridge Mode</font></td>
	</tr>
</table>
<div id="div_8021q">
<table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
	<tr>
		<td width="150" height="30" class="title-main"><div align=right><font color="#FFFFFF">8021q Options</font></div></td>
		<td width="10" class="black">&nbsp;</td>
		<td width="150"><hr noshade></td>
		<td width="10"><hr noshade></td>
		<td width="440"><hr noshade></td>
	</tr>
	<TR>
    	<TD class=light-orange>&nbsp;</TD>
    	<TD class=light-orange></TD>
    	<TD class=tabdata><DIV align=right><FONT color=#000000>802.1q</FONT></DIV></TD>
    	<TD class=tabdata><DIV align=center>:</DIV></TD>
    	<TD class=tabdata>
			<INPUT value="Yes" type=radio name=wan_dot1q onclick="wanVidOper(1)" <% if tcWebApi_get("Wan_PVC","dot1q","h") = "Yes" then asp_Write("checked")end if %> ><FONT color=#000000>Activated</FONT>
			<INPUT value="No" type=radio name=wan_dot1q onclick="wanVidOper(0)" <% if tcWebApi_get("Wan_PVC","dot1q","h") = "No" then asp_Write("checked")end if if tcWebApi_get("Wan_PVC","dot1q","h") = "N/A" then asp_Write("checked") end if  %> ><FONT color=#000000>Deactivated</FONT></TD>
	</TR>
<% if tcWebApi_get("WebCustom_Entry", "isTPIDSupported", "h") = "Yes" then %>
	 <TR>
    	<TD class=light-orange>&nbsp;</TD>
    	<TD class=light-orange>&nbsp;</TD>
    	<TD class=tabdata><DIV align=right><FONT color=#000000>TPID</FONT></DIV></TD>
    	<TD class=tabdata><DIV align=center>:</DIV></TD>
    	<TD class=tabdata>
			<INPUT maxLength=2 size=5 name="wan_tpid" VALUE="<%if tcWebApi_get("Wan_PVC","TPID","h") <> "N/A" then tcWebApi_get("Wan_PVC","TPID","s") else asp_Write("0") end if%>" ><FONT color=#000000></FONT></TD>
	</TR>
<%end if%>	
    <TR>
    	<TD class=light-orange>&nbsp;</TD>
    	<TD class=light-orange>&nbsp;</TD>
    	<TD class=tabdata><DIV align=right><FONT color=#000000>VLAN ID</FONT></DIV></TD>
    	<TD class=tabdata><DIV align=center>:</DIV></TD>
    	<TD class=tabdata>
			<INPUT maxLength=5 size=5 name="wan_vid" VALUE="<%if tcWebApi_get("Wan_PVC","VLANID","h") <> "N/A" then tcWebApi_get("Wan_PVC","VLANID","s") else asp_Write("0") end if%>" ><FONT color=#000000>(range: 0~4095)</FONT></TD>
	</TR>
<% if tcWebApi_get("WebCustom_Entry", "isdot1pSupport", "h") = "Yes" then %>
	 <TR>
    	<TD class=light-orange>&nbsp;</TD>
    	<TD class=light-orange>&nbsp;</TD>
    	<TD class=tabdata><DIV align=right><FONT color=#000000>802.1p</FONT></DIV></TD>
    	<TD class=tabdata><DIV align=center>:</DIV></TD>
    	<TD class=tabdata>
			<INPUT maxLength=2 size=2 name="wan_dot1p" VALUE="<%if tcWebApi_get("Wan_PVC","DOT1P","h") <> "N/A" then tcWebApi_get("Wan_PVC","DOT1P","s") else asp_Write("0") end if%>" ><FONT color=#000000>(range: 0~7)</FONT></TD>
	</TR>
<%end if%>
<% if tcWebApi_get("WebCustom_Entry", "isWanTagChk", "h") = "Yes" then %>
	<tr>
		<td  class="light-orange">&nbsp;</td>
		<td class="light-orange">&nbsp;</td>
		<td class="tabdata"><div align=right><font color="#000000">VLAN TAG</font></div></td>
		<td class="tabdata"><div align=center>:</div></td>
		
      <td class="tabdata"> <select name="TAGSEL" size="1" >
	  	  <option <% if tcWebApi_get("Wan_PVC","TAG","h") = "untaged" then asp_Write("selected") elseif tcWebApi_get("Wan_PVC","TAG","h") = "N/A" then asp_Write("selected") end if %> value="untaged">untaged
		  <option <% if tcWebApi_get("Wan_PVC","TAG","h") = "taged" then asp_Write("selected") end if %> value="taged">taged
        </select></td>
	</tr>
<%end if%>
</table>
</div>

<% if tcWebApi_get("WebCustom_Entry", "isMultiVLanSupported", "h") = "Yes" then %>
<div id="div_mvlan">
<table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
	<tr>
		<td width="150" height="30" class="title-main"><div align=right><font color="#FFFFFF">MVLan Options</font></div></td>
		<td width="10" class="black">&nbsp;</td>
		<td width="150"><hr noshade></td>
		<td width="10"><hr noshade></td>
		<td width="440"><hr noshade></td>
	</tr>
  <TR>
    	<TD class=light-orange>&nbsp;</TD>
    	<TD class=light-orange>&nbsp;</TD>
    	<TD class=tabdata><DIV align=right><FONT color=#000000>Multi VLan</FONT></DIV></TD>
    	<TD class=tabdata><DIV align=center>:</DIV></TD>
    	<TD class=tabdata><INPUT maxLength=5 size=5 name="wan_mvlan" VALUE="<%if tcWebApi_get("Wan_PVC","MVLAN","h") <> "N/A" then tcWebApi_get("Wan_PVC","MVLAN","s") else asp_Write("-1") end if%>" ><FONT color=#000000>(range: -1~4095, -1 means no multi vlan)</FONT></TD>
  </TR>
</table>
</div>
<%end if%>

<div id="div_802_1x">
<table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
	<tr>
		<td width="150" height="30" class="title-main"><div align=right><font color="#FFFFFF">802.1X</font></div></td>
		<td width="10" class="black">&nbsp;</td>
		<td width="150"><hr noshade></td>
		<td width= "10"><hr noshade></td>
		<td width="440"><hr noshade></td>
	</tr>

 <tr>
		<td class="title-sub"><div align=right><font color="#000000">802.1X Authentication</font></div></td>
		<td class="light-orange">&nbsp;</td>
		<td><hr noshade class="light-orange-line"></td>
		<td><hr noshade class="light-orange-line"></td>
		<td><hr noshade class="light-orange-line"></td>
	</tr>
	
   <tr>
    <td class="light-orange">&nbsp;</td>
    <td class="light-orange">&nbsp;</td>
    <td class="tabdata"><div align=right><font color="#000000">802.1X</font></div></td>
    <td width="10" class="tabdata"><div align=center>:</div></td>
    <td class="tabdata">
        <SELECT NAME="wan_status" SIZE="1" onchange="doStatusChange()">
		<option <% if tcWebApi_get("Wan_PVC","IPOE_DOT1X_STATUS","h") = "Disabled" then asp_Write("selected") end if %>>Disabled
		<option <% if tcWebApi_get("Wan_PVC","IPOE_DOT1X_STATUS","h") = "Enable" then asp_Write("selected") end if %>>Enable
		</SELECT>
		</td>
		</tr>
		<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange">&nbsp;</td>
		<td class="tabdata"><div align=right><font color="#000000">EAP Identity</font>    </div></td>
		<td class="tabdata"><div align=center>:</div></td>
		<td class="tabdata">
        <INPUT TYPE="TEXT" NAME="wan_eapIdentity" SIZE="20" MAXLENGTH="45" VALUE="<% if tcWebApi_get("Wan_PVC","EAPIDENTITY","h") <> "N/A" then tcWebApi_get("Wan_PVC","EAPIDENTITY","s")  end if%>" >	        
     </td></tr>
     
		<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange">&nbsp;</td>
		<td class="tabdata"><div align=right><font color="#000000">EAP Method</font>  </div></td>
    <td class="tabdata"><div align=center>:</div></td>
    <td class="tabdata">EAP-TLS </td>
    </tr>
    
     <tr>
     <td class="light-orange">&nbsp;</td>
     <td class="light-orange">&nbsp;</td>
     <td class="tabdata"><div align=right> <INPUT type="checkbox" NAME="wan_authentication" onclick="doauthenticationChange()" <% if tcWebApi_get("Wan_PVC","BIDIRECTIONALAUTHENTICATION","h") = "Yes" then asp_Write("checked") end if 
				 if tcWebApi_get("Wan_PVC","BIDIRECTIONALAUTHENTICATION","h") = "N/A" then asp_write("checked") end if %>> </div></td>
	<td class="tabdata"></td>
	<td class="tabdata">Enable Bidirectional Authentication</td>
	</tr>

     <tr>
    <td class="light-orange">&nbsp;</td>
    <td class="light-orange">&nbsp;</td>
    <td class="tabdata"><div align=right><font color="#000000">Certificate</font></div></td>
    <td class="tabdata"><div align=center>:</div></td>
    <td class="tabdata">
    <iframe src="/cgi-bin/getCertNames.cgi" name="Iframe2" frameBorder="0" height="22" scrolling="no" marginheight="0" align="top"></iframe>
		</td>
		</tr>
		
		 <tr>
    <td class="light-orange">&nbsp;</td>
    <td class="light-orange">&nbsp;</td>
    <td class="tabdata"><div align=right><font color="#000000">Trusted CA</font></div></td>
    <td class="tabdata"><div align=center>:</div></td>
    <td class="tabdata">      
    <iframe src="/cgi-bin/getCANames.cgi" name="Iframe3" frameBorder="0" height="22" scrolling="no" marginheight="0" align="top"></iframe>
       
		</td>
		</tr>
   </table>
   </div>
 
<div id="div_isp0">
<table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
	<tr>
		<td width="150" height="30" class="title-main"><div align=right><font color="#FFFFFF">Dynamic IP</font></div></td>
		<td width="10" class="black">&nbsp;</td>
		<td width="150"><hr noshade></td>
		<td width="10"><hr noshade></td>
		<td width="440"><hr noshade></td>
	</tr>

	<tr>
		<td class="title-sub"><font color="#000000">IP Common Options</font></td>
		<td class="light-orange">&nbsp;</td>
		<td><hr noshade class="light-orange-line"></td>
		<td><hr noshade class="light-orange-line"></td>
		<td><hr noshade class="light-orange-line"></td>
	</tr>

<%if tcWebApi_get("Wan_Common","TransMode","h") = "ATM" then%>
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange">&nbsp;</td>
		<td class="tabdata"><div align=right><font color="#000000">Encapsulation</font></div></td>
		<td class="tabdata"><div align=center>:</div></td>
		<td class="tabdata">
			<SELECT NAME="wan_Encap0" SIZE="1" onChange="doEncapChange()">
				<option <% if tcWebApi_get("Wan_PVC","ENCAP","h") = "1483 Bridged IP LLC" then asp_Write("selected") end if %>>1483 Bridged IP LLC
				<option <% if tcWebApi_get("Wan_PVC","ENCAP","h") = "1483 Bridged IP VC-Mux" then asp_Write("selected") end if %>>1483 Bridged IP VC-Mux
				<option <% if tcWebApi_get("Wan_PVC","ENCAP","h") = "1483 Routed IP LLC(IPoA)" then asp_Write("selected") end if %>>1483 Routed IP LLC(IPoA)
				<option <% if tcWebApi_get("Wan_PVC","ENCAP","h") = "1483 Routed IP VC-Mux" then asp_Write("selected") end if %>>1483 Routed IP VC-Mux
			</SELECT></td>
	</tr>
<%end if%>
	<tr id="div_isp0encap">
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata"><div align=right><font color="#000000">Bridge Interface for PPPoE</font></div></td>
		<td class="tabdata"><div align=center>:</div></td>
		<td class="tabdata">
			<INPUT TYPE="RADIO" NAME="wan_BridgeInterface0" VALUE="Yes" <% if tcWebApi_get("Wan_PVC","BridgeInterface","h") = "Yes" then asp_Write("checked")end if %> ><font color="#000000">Activated</font>        
			<INPUT TYPE="RADIO" NAME="wan_BridgeInterface0" VALUE="No"  <% if tcWebApi_get("Wan_PVC","BridgeInterface","h") = "No" then asp_Write("checked") elseif tcWebApi_get("Wan_PVC","BridgeInterface","h") = "N/A" then asp_Write("checked") end if %> ><font color="#000000">Deactivated</font></td>
	</tr>

	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata"><div align=right><font color="#000000">Default Route</font></div></td>
		<td class="tabdata"><div align=center>:</div></td>
		<td class="tabdata">
			<INPUT TYPE="RADIO" NAME="WAN_DefaultRoute0" VALUE="Yes" onClick="doDefaultRouteYes()" <% if tcWebApi_get("Wan_PVC","DEFAULTROUTE","h") = "Yes" then asp_Write("checked")end if %> ><font color="#000000">Yes</font>
			<input type="RADIO" name="WAN_DefaultRoute0" value="No"  onClick="doDefaultRouteNo()" <% if tcWebApi_get("Wan_PVC","DEFAULTROUTE","h") = "No" then asp_Write("checked") elseif tcWebApi_get("Wan_PVC","DEFAULTROUTE","h") = "N/A" then asp_Write("checked") end if %> ><font color="#000000">No</font></td>
	</tr>
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata"><div align=right><font color="#000000">TCP MTU Option</font></div></td>
		<td class="tabdata"><div align=center>:</div></td>
		<td class="tabdata"><font color="#000000">TCP MTU(0:default)</font>
			<INPUT TYPE="TEXT" NAME="wan_TCPMTU0" SIZE="5" MAXLENGTH="4" VALUE=<% if tcWebApi_get("Wan_PVC","MTU","h") = "N/A" then asp_write("0") else tcWebApi_get("Wan_PVC","MTU","s")  end if%> ><font color="#000000">bytes</font></td>
	</tr>
  	

<!--ipv4/ipv6 and ipv4 dgk wait modify -->	
	<tr id="div_ipv4nat_0">
		<td class="title-sub"><font color="#000000">IPv4 Options</font></td>
		<td class="light-orange">&nbsp;</td>
		<td><hr noshade class="light-orange-line"></td>
		<td><hr noshade class="light-orange-line"></td>
		<td><hr noshade class="light-orange-line"></td>
	</tr>
	<tr id="div_ipv4nat_1">
		<td width="150" class="light-orange">&nbsp;</td>
		<td width="10" class="light-orange"></td>
		<td width="150" class="tabdata"><div align=right><font color="#000000">NAT</font></div></td>
		<td width="10" class="tabdata"><div align=center>:</div></td>
		<td width="440" class="tabdata">
			<SELECT NAME="wan_NAT0" SIZE="1">
				<option <% if tcWebApi_get("Wan_PVC","NATENABLE","h") = "Enable" then asp_Write("selected") end if %>>Enable
				<option <% if tcWebApi_get("Wan_PVC","NATENABLE","h") = "Disabled" then asp_Write("selected") elseif tcWebApi_get("Wan_PVC","NATENABLE","h") = "N/A" then asp_Write("selected") end if %>>Disabled
			</SELECT></td>
	</tr>	
	
	
</table>

<!--ipv4/ipv6 and ipv4 dgk wait modify -->
<div id="div_ipv4IP">
<table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
	<tr>
		<td width="150" class="light-orange">&nbsp;</td>
		<td width="10" class="light-orange"></td>
		<td width="150" class="tabdata"><div align=right><font color="#000000">Dynamic Route</font></div></td>
		<td width="10" class="tabdata"><div align=center>:</div></td>
		<td width="440" class="tabdata">
			<SELECT NAME="wan_RIP0" SIZE="1" >
				<option <% if tcWebApi_get("Wan_PVC","RIPVERSION","h") = "RIP1" then asp_Write("selected") end if %>>RIP1
				<option <% if tcWebApi_get("Wan_PVC","RIPVERSION","h") = "RIP2" then asp_Write("selected") end if %>>RIP2
			</SELECT>
			<font color="#000000">Direction</font>
			<SELECT NAME="wan_RIP_Dir0" SIZE="1" >
				<option <% if tcWebApi_get("Wan_PVC","DIRECTION","h") = "None" then asp_Write("selected") end if %>>None
				<option <% if tcWebApi_get("Wan_PVC","DIRECTION","h") = "Both" then asp_Write("selected") end if %>>Both
				<option <% if tcWebApi_get("Wan_PVC","DIRECTION","h") = "IN Only" then asp_Write("selected") end if %>>IN Only
				<option <% if tcWebApi_get("Wan_PVC","DIRECTION","h") = "OUT Only" then asp_Write("selected") end if %>> OUT Only
			</SELECT></td>
	</tr>
<%if tcWebApi_get("WebCurSet_Entry","UserMode","h")<>"1" then%>
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata"><div align=right><font color="#000000">IGMP Proxy</font></div></td>
		<td class="tabdata"><div align=center>:</div></td>
		<td class="tabdata">
			<INPUT TYPE="RADIO" NAME="wan_IGMP0" VALUE="Yes" <% if tcWebApi_get("Wan_PVC","IGMPproxy","h") = "Yes" then asp_Write("checked")end if %> > <font color="#000000">Enable</font>
			<INPUT TYPE="RADIO" NAME="wan_IGMP0" VALUE="No"  <% if tcWebApi_get("Wan_PVC","IGMPproxy","h") = "No" then asp_Write("checked") elseif tcWebApi_get("Wan_PVC","IGMPproxy","h") = "N/A" then asp_Write("checked") end if %> > <font color="#000000">Disable</font></td>
	</tr>
<%end if%>
</table>
</div> <!--end div id="div_isp0" -->

<!-- jrchen add -->	
<!-- ipv4/ipv6 deleted-->
<!-- jrchen add end -->

<!--ipv4/ipv6 dgk wait modify -->
<div align=center id="div_ipv6dhcp">
<table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
	<tr>
		<td width="150" height="30" class="title-main"><font color="#FFFFFF">IPv6 Address</font></td>
		<td width="10" class="black">&nbsp;</td>
		<td width="150"><hr noshade></td>
		<td width="10"><hr noshade></td>
		<td width="440"><hr noshade></td>
	</tr>
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata"><div align=right><font color="#000000">IPv6 Message Fetch Type</font></div></td>
		<td class="tabdata"><div align=center>:</div></td>
		<td class="tabdata"><font color="#000000">Dynamic Mode</font> </td>
	</tr>
	 
	 <tr>
		<td height="27" class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata"><div align=right><font color="#000000">DHCP IPv6 Enable</font></div></td>
		<td class="tabdata"><div align=center>:</div></td>
		<td class="tabdata">
			<%if tcWebApi_get("WebCustom_Entry","isIPv6PrivacyAddrsSupported","h") = "Yes" then%>
			<INPUT TYPE="RADIO" NAME="DynIPv6EnableRadio" VALUE="1"  onClick="doPrivacyaddrsShow0();" <% if tcWebApi_get("Wan_PVC","EnableDynIPv6","h") = "1" then asp_Write("checked") elseif tcWebApi_get("Wan_PVC","EnableDynIPv6","h") = "N/A" then asp_Write("checked") end if %> > <font color="#000000">DHCP</font>
			<INPUT TYPE="RADIO" NAME="DynIPv6EnableRadio" VALUE="0"  onClick="doPrivacyaddrsShow0();"<% if tcWebApi_get("Wan_PVC","EnableDynIPv6","h") = "0" then asp_Write("checked") end if %> > <font color="#000000">SLAAC</font>        
			<%else %>
			<INPUT TYPE="RADIO" NAME="DynIPv6EnableRadio" VALUE="1" <% if tcWebApi_get("Wan_PVC","EnableDynIPv6","h") = "1" then asp_Write("checked") elseif tcWebApi_get("Wan_PVC","EnableDynIPv6","h") = "N/A" then asp_Write("checked") end if %> > <font color="#000000">DHCP</font>
			<INPUT TYPE="RADIO" NAME="DynIPv6EnableRadio" VALUE="0" <% if tcWebApi_get("Wan_PVC","EnableDynIPv6","h") = "0" then asp_Write("checked") end if %> > <font color="#000000">SLAAC</font>        
			<%end if%>
		</td>
	</tr>
	<%if tcWebApi_get("WebCustom_Entry","isIPv6PrivacyAddrsSupported","h") = "Yes" then%>
	<tr id="div_privacyaddrs0">
		<td height="27" class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata"><div align=right><font color="#000000">Privacy Address</font></div></td>
		<td class="tabdata"><div align=center>:</div></td>
		<td class="tabdata">
			<INPUT TYPE="RADIO" NAME="privacyaddrsradio0" VALUE="2" <% if tcWebApi_get("Wan_PVC","IPv6Extension","h") = "2" then asp_Write("checked") end if %> > <font color="#000000">Enable</font>
			<INPUT TYPE="RADIO" NAME="privacyaddrsradio0" VALUE="0"  <% if tcWebApi_get("Wan_PVC","IPv6Extension","h") <> "2" then asp_Write("checked") end if %> > <font color="#000000">Disable</font></td>
			<script language="JavaScript" type="text/JavaScript">
				doPrivacyaddrsShow0();
			</script>	
	</tr>
	<%end if%>
	<tr>
		<td height="27" class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata"><div align=right><font color="#000000">DHCP PD Enable</font></div></td>
		<td class="tabdata"><div align=center>:</div></td>
		<td class="tabdata">			     
			<INPUT TYPE="RADIO" NAME="PPPIPv6PDRadio0" VALUE="Yes" onClick="doPPPv6PDEnable()" <% if tcWebApi_get("Wan_PVC","PPPv6PD","h") = "Yes" then asp_Write("checked")  end if %> > <font color="#000000">Enable</font>
			<INPUT TYPE="RADIO" NAME="PPPIPv6PDRadio0" VALUE="No" onClick="doPPPv6PDDisable()" <% if tcWebApi_get("Wan_PVC","PPPv6PD","h") = "No" then asp_Write("checked") elseif tcWebApi_get("Wan_PVC","PPPv6PD","h") = "N/A" then asp_Write("checked") end if %> > <font color="#000000">Disable</font>   
		</td>
	</tr>
<%if tcWebApi_get("WebCurSet_Entry","UserMode","h")<>"1" then%>
	 	<tr>
		<td height="24" class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata"><div align=right><font color="#000000">MLD Proxy</font></div></td>
		<td class="tabdata"><div align=center>:</div></td>
		<td class="tabdata">
			<INPUT TYPE="RADIO" NAME="wan_MLD0" VALUE="Yes" <% if tcWebApi_get("Wan_PVC","MLDproxy","h") = "Yes" then asp_Write("checked")end if %> > <font color="#000000">Enable</font>
			<INPUT TYPE="RADIO" NAME="wan_MLD0" VALUE="No"  <% if tcWebApi_get("Wan_PVC","MLDproxy","h") = "No" then asp_Write("checked") elseif tcWebApi_get("Wan_PVC","MLDproxy","h") = "N/A" then asp_Write("checked") end if %> > <font color="#000000">Disable</font></td>
	</tr>
<%end if%>
<% If tcWebApi_get("WebCustom_Entry","isRipngSupported","h") = "Yes" Then %>
	 <tr>
		<td height="23" class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata"><div align=right><font color="#000000">Ripng Dync</font></div></td>
		<td class="tabdata"><div align=center>:</div></td>
		<td class="tabdata">
 				<INPUT TYPE="RADIO" NAME="ripngEnableRadio0" VALUE="1"  onClick="ripngEnableChanged0()" <%If tcWebApi_get("Wan_PVC","RIPNGENABLE","h") = "1" then asp_Write("checked") end if%> > 
                <font color="#000000">Enable</font>
				<INPUT TYPE="RADIO" NAME="ripngEnableRadio0" VALUE="0"  onClick="ripngEnableChanged0()" <%If tcWebApi_get("Wan_PVC","RIPNGENABLE","h") <> "1" then asp_Write("checked") end if%> > 
                <font color="#000000">Disable</font>  </td>
	</tr>
	 <tr id="div_ripng_direction0">
		<td height="23" class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata"><div align=right><font color="#000000">Ripng Direction</font></div></td>
		<td class="tabdata"><div align=center>:</div></td>
		<td class="tabdata">
			<SELECT NAME="wan_Ripng_Dir0" SIZE="1">
			<OPTION <% if tcWebApi_get("Wan_PVC","RIPNGDIRECTION","h") = "None" then asp_Write("selected") end if %>>None
			<OPTION <% if tcWebApi_get("Wan_PVC","RIPNGDIRECTION","h") = "Both" then asp_Write("selected") end if %>>Both
			<OPTION <% if tcWebApi_get("Wan_PVC","RIPNGDIRECTION","h") = "IN Only" then asp_Write("selected") end if %>>IN Only
			<OPTION <% if tcWebApi_get("Wan_PVC","RIPNGDIRECTION","h") = "OUT Only" then asp_Write("selected") end if %>>OUT Only
			</SELECT>
	</tr>
	<script language="JavaScript" type="text/JavaScript">
			ripngEnableChanged0();
	</script>
<%end if%>
	</table></div>
	<div align=center id="div_isp0dsl">
	<table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
	<tr>
		<td width="150" class="title-sub"><font color="#000000">Dual Stack Lite</font></td>
		<td width="10" class="light-orange">&nbsp;</td>
		<td width="150"><hr noshade class="light-orange-line"></td>
		<td width="10"><hr noshade class="light-orange-line"></td>
		<td width="440"><hr noshade class="light-orange-line"></td>
	</tr>
	<tr>
		<td height="27" class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata"><div align=right><font color="#000000">Enable</font></div></td>
		<td class="tabdata"><div align=center>:</div></td>
		<td class="tabdata"> <input type="RADIO" name="DSLITEEnableRadio0" value="Yes" onClick="doDSLiteEnable(0)" <% if tcWebApi_get("Wan_PVC","DSLITEEnable","h") = "Yes" then asp_Write("checked")  end if %> >
          <font color="#000000">Enable</font> 
          <INPUT TYPE="RADIO" NAME="DSLITEEnableRadio0" VALUE="No" onClick="doDSLiteDisable(0)" <% if tcWebApi_get("Wan_PVC","DSLITEEnable","h") = "No" then asp_Write("checked")  elseif tcWebApi_get("Wan_PVC","DSLITEEnable","h") = "N/A" then asp_Write("checked") end if %> >
          <font color="#000000">Disable</font> </td>
	</tr>
	<tr id="div_dslite0_0">
		<td height="27" class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata"><div align=right><font color="#000000">Mode</font></div></td>
		<td class="tabdata"><div align=center>:</div></td>
		<td class="tabdata">
			<INPUT TYPE="RADIO" NAME="DSLITEModeRadio0" VALUE="0"  onClick="doDSLiteModeAuto()" <% if tcWebApi_get("Wan_PVC","DSLITEMode","h") = "0" then asp_Write("checked") elseif  tcWebApi_get("Wan_PVC","DSLITEMode","h") = "N/A" then asp_Write("checked") end if %> >
          <font color="#000000">Auto</font> <input type="RADIO" name="DSLITEModeRadio0" value="1"   onClick="doDSLiteModeManual()" <% if tcWebApi_get("Wan_PVC","DSLITEMode","h") = "1" then asp_Write("checked") end if %> >
          <font color="#000000">Manual</font></td>
	</tr>
	<tr id="div_dslite0_1">
		<td height="29" class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata"><div align=right><font color="#000000">Remote IPv6 Address</font></div></td>
		<td class="tabdata"><div align=center>:</div></td>
		<td class="tabdata"><INPUT TYPE="TEXT" NAME="DSLITEAddr0" SIZE="39" MAXLENGTH="39" VALUE= <% if tcWebApi_get("Wan_PVC","DSLITEMode","h") = "1" then tcWebApi_get("Wan_PVC","DSLITEAddr","s") else asp_Write("N/A") end if%> >
		<script language="JavaScript" type="text/JavaScript">
		if (document.Alpha_WAN.isDSLITESupported.value == "1"){
			if(document.Alpha_WAN.DSLITEEnableRadio0[0].checked)
				doDSLiteEnable(0);
			else
				doDSLiteDisable(0);
		}
		</script></td>
	</tr>
	</table></div>
</div>

<div id="div_isp1">
<table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
	<tr>
		<td width="150" height="30" class="title-main"><font color="#FFFFFF">Static IP</font></td>
		<td width="10" class="black">&nbsp;</td>
		<td width="150"><hr noshade></td>
		<td width="10"><hr noshade></td>
		<td width="440"><hr noshade></td>
	</tr>
	
	
	
	<tr>
		<td class="title-sub"><font color="#000000">IP Common Options</font></td>
		<td class="light-orange">&nbsp;</td>
		<td><hr noshade class="light-orange-line"></td>
		<td><hr noshade class="light-orange-line"></td>
		<td><hr noshade class="light-orange-line"></td>
	</tr>
	
	
<%if tcWebApi_get("Wan_Common","TransMode","h") = "ATM" then%>	
	
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata"><div align=right><font color="#000000">Encapsulation</font></div></td>
		<td class="tabdata"><div align=center>:</div></td>
		<td class="tabdata">
			<SELECT NAME="wan_Encap1" SIZE="1"onChange="doEncapChange()">
				<option <% if tcWebApi_get("Wan_PVC","ENCAP","h") = "1483 Bridged IP LLC" then asp_Write("selected") end if %>>1483 Bridged IP LLC
				<option <% if tcWebApi_get("Wan_PVC","ENCAP","h") = "1483 Bridged IP VC-Mux" then asp_Write("selected") end if %>>1483 Bridged IP VC-Mux
				<option <% if tcWebApi_get("Wan_PVC","ENCAP","h") = "1483 Routed IP LLC(IPoA)" then asp_Write("selected") end if %>>1483 Routed IP LLC(IPoA)
				<option <% if tcWebApi_get("Wan_PVC","ENCAP","h") = "1483 Routed IP VC-Mux" then asp_Write("selected") end if %>>1483 Routed IP VC-Mux
			</SELECT></td>
	</tr>
<%end if%>	
	
	<tr id="div_isp1encap">
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata"><div align=right><font color="#000000">Bridge Interface for PPPoE</font></div></td>
		<td class="tabdata"><div align=center>:</div></td>
		<td class="tabdata">
			<INPUT TYPE="RADIO" NAME="wan_BridgeInterface1" VALUE="Yes" <% if tcWebApi_get("Wan_PVC","BridgeInterface","h") = "Yes" then asp_Write("checked")end if %> ><font color="#000000">Activated</font>        
			<INPUT TYPE="RADIO" NAME="wan_BridgeInterface1" VALUE="No"  <% if tcWebApi_get("Wan_PVC","BridgeInterface","h") = "No" then asp_Write("checked") elseif tcWebApi_get("Wan_PVC","BridgeInterface","h") = "N/A" then asp_Write("checked") end if %> ><font color="#000000">Deactivated</font></td>
	</tr>
	
	
	
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata"><div align=right><font color="#000000">Default Route</font></div></td>
		<td class="tabdata"><div align=center>:</div></td>
		<td class="tabdata">
			<INPUT TYPE="RADIO" NAME="WAN_DefaultRoute1" VALUE="Yes"  onClick="doDefaultRouteYes()" <% if tcWebApi_get("Wan_PVC","DEFAULTROUTE","h") = "Yes" then asp_Write("checked")end if %> ><font color="#000000">Yes</font>
			<INPUT TYPE="RADIO" NAME="WAN_DefaultRoute1" VALUE="No"   onClick="doDefaultRouteNo()" <% if tcWebApi_get("Wan_PVC","DEFAULTROUTE","h") = "No" then asp_Write("checked") elseif tcWebApi_get("Wan_PVC","DEFAULTROUTE","h") = "N/A" then asp_Write("checked") end if %> ><font color="#000000">No</font></td>
	</tr>
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata"><div align=right><font color="#000000">TCP MTU Option</font></div></td>
		<td class="tabdata"><div align=center>:</div></td>
		<td class="tabdata"><font color="#000000">TCP MTU(0:default)</font>
			<INPUT TYPE="TEXT" NAME="wan_TCPMTU1" SIZE="5" MAXLENGTH="4" VALUE=<% if tcWebApi_get("Wan_PVC","MTU","h") = "N/A" then asp_write("0") else tcWebApi_get("Wan_PVC","MTU","s")  end if%> ><font color="#000000">bytes</font></td>
	</tr>
</table>	
	
	
	
	
	

<div id="div_ipv4static">
<table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
	<tr>
		<td class="title-sub"><font color="#000000">IPv4 Options</font></td>
		<td class="light-orange">&nbsp;</td>
		<td><hr noshade class="light-orange-line"></td>
		<td><hr noshade class="light-orange-line"></td>
		<td><hr noshade class="light-orange-line"></td>
	</tr>
	
	
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata"><div align=right><font color="#000000">Static IP Address</font></div></td>
		<td class="tabdata"><div align=center>:</div></td>
		<td class="tabdata">
			<INPUT TYPE="TEXT" NAME="wan_StaticIPaddr1" SIZE="16" MAXLENGTH="15"		
			<%if tcwebApi_get("WebCustom_Entry", "isCZGeneralSupported", "h")="Yes" Then%>
			 	VALUE= "<% if tcWebApi_get("Wan_PVC","IPADDR","h") <> "N/A" then tcWebApi_get("Wan_PVC","IPADDR","s") else asp_Write("0.0.0.0") end if%>" >
			<%else%>
				VALUE= "<% if tcWebApi_get("Wan_PVC","IPADDR","h") <> "N/A" then tcWebApi_get("Wan_PVC","IPADDR","s") end if%>" >		
			<%end if%>
		</td>
	</tr>
	
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata"><div align=right><font color="#000000">IP Subnet Mask</font></div></td>
		<td class="tabdata"><div align=center>:</div></td>
		<td class="tabdata">
			<INPUT TYPE="TEXT" NAME="wan_StaticIPSubMask1" SIZE="16" MAXLENGTH="15"		
			<%if tcwebApi_get("WebCustom_Entry", "isCZGeneralSupported", "h")="Yes" Then%>
			 	VALUE= "<% if tcWebApi_get("Wan_PVC","NETMASK","h") <> "N/A" then tcWebApi_get("Wan_PVC","NETMASK","s") else asp_Write("0.0.0.0") end if%>" >
			<%else%>
				VALUE= "<% if tcWebApi_get("Wan_PVC","NETMASK","h") <> "N/A" then tcWebApi_get("Wan_PVC","NETMASK","s") end if%>" >		
			<%end if%>
			</td>
	</tr>
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata"><div align=right><font color="#000000">Gateway</font></div></td>
		<td class="tabdata"><div align=center>:</div></td>
		<td class="tabdata">
			<INPUT TYPE="TEXT" NAME="wan_StaticIpGateway1" SIZE="16" MAXLENGTH="15"		
			<%if tcwebApi_get("WebCustom_Entry", "isCZGeneralSupported", "h")="Yes" Then%>
			 	VALUE= "<% if tcWebApi_get("Wan_PVC","GATEWAY","h") <> "N/A" then tcWebApi_get("Wan_PVC","GATEWAY","s") else asp_Write("0.0.0.0") end if%>" >
			<%else%>
				VALUE= "<% if tcWebApi_get("Wan_PVC","GATEWAY","h") <> "N/A" then tcWebApi_get("Wan_PVC","GATEWAY","s") end if%>" >		
			<%end if%>
			</td>
	</tr>
	<%if tcwebApi_get("WebCustom_Entry","isWanDNSEachPVC","h") = "Yes" then%>
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata"><div align=right><font color="#000000">PrimaryDNS</font></div></td>
		<td class="tabdata"><div align=center>:</div></td>
		<td class="tabdata">
			<INPUT TYPE="TEXT" NAME="PrimaryDNS1" SIZE="16" MAXLENGTH="15" VALUE= <% if tcWebApi_get("Wan_PVC","PRIMARYDNS","h") <> "N/A" then tcWebApi_get("Wan_PVC","PRIMARYDNS","s") end if%>></td>
	</tr>
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata"><div align=right><font color="#000000">SecondaryDNS</font></div></td>
		<td class="tabdata"><div align=center>:</div></td>
		<td class="tabdata">
			<INPUT TYPE="TEXT" NAME="SecondaryDNS1" SIZE="16" MAXLENGTH="15" VALUE= <% if tcWebApi_get("Wan_PVC","SECONDARYDNS","h") <> "N/A" then tcWebApi_get("Wan_PVC","SECONDARYDNS","s") end if%>></td>
	</tr>
	<%end if%>
	<tr>
		<td width="150" class="light-orange">&nbsp;</td>
		<td width="10" class="light-orange"></td>
		<td width="150" class="tabdata"><div align=right><font color="#000000">NAT</font></div></td>
		<td width="10" class="tabdata"><div align=center>:</div></td>
		
      <td width="440" class="tabdata"> <select name="wan_NAT1" size="1">
          <option <% if tcWebApi_get("Wan_PVC","NATENABLE","h") = "Enable" then asp_Write("selected") end if %>>Enable 
          <option <% if tcWebApi_get("Wan_PVC","NATENABLE","h") = "Disabled" then asp_Write("selected") elseif tcWebApi_get("Wan_PVC","NATENABLE","h") = "N/A" then asp_Write("selected") end if %>>Disabled 
        </select></td>
	</tr>
	
	
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata"><div align=right><font color="#000000">Dynamic Route</font></div></td>
		<td width="10" class="tabdata"><div align=center>:</div></td>
		<td class="tabdata">
			<SELECT NAME="wan_RIP1" SIZE="1" >
				<option <% if tcWebApi_get("Wan_PVC","RIPVERSION","h") = "RIP1" then asp_Write("selected") end if %>>RIP1
				<option <% if tcWebApi_get("Wan_PVC","RIPVERSION","h") = "RIP2" then asp_Write("selected") end if %>>RIP2
			</SELECT>
			<font color="#000000">Direction</font>
			<SELECT NAME="wan_RIP_Dir1" SIZE="1" >
				<option <% if tcWebApi_get("Wan_PVC","DIRECTION","h") = "None" then asp_Write("selected") end if %>>None
				<option <% if tcWebApi_get("Wan_PVC","DIRECTION","h") = "Both" then asp_Write("selected") end if %>>Both
				<option <% if tcWebApi_get("Wan_PVC","DIRECTION","h") = "IN Only" then asp_Write("selected") end if %>>IN Only
				<option <% if tcWebApi_get("Wan_PVC","DIRECTION","h") = "OUT Only" then asp_Write("selected") end if %>> OUT Only
			</SELECT></td>
	</tr>	
</table></div>
<%if tcWebApi_get("WebCurSet_Entry","UserMode","h")<>"1" then%>
<table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">			
	<tr id="div_ipv4igp">
		<td width="150" height="25" class="light-orange">&nbsp;</td>
		<td width="10" class="light-orange"></td>
		<td width="150" class="tabdata"><div align=right><font color="#000000">IGMP Proxy</font></div></td>
		<td width="10" class="tabdata"><div align=center>:</div></td>
		<td width="440" class="tabdata">
			<INPUT TYPE="RADIO" NAME="wan_IGMP1" VALUE="Yes" <% if tcWebApi_get("Wan_PVC","IGMPproxy","h") = "Yes" then asp_Write("checked")end if %> > <font color="#000000">Enable</font>
			<INPUT TYPE="RADIO" NAME="wan_IGMP1" VALUE="No"  <% if tcWebApi_get("Wan_PVC","IGMPproxy","h") = "No" then asp_Write("checked") elseif tcWebApi_get("Wan_PVC","IGMPproxy","h") = "N/A" then asp_Write("checked") end if %> > <font color="#000000">Disable</font></td>
	</tr>
</table>
<%end if%>
<!-- jrchen add -->	

<div id="div_ipv6_staticip">
<table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
	<tr>
		<td width="150" class="title-sub"><font color="#000000">IPv6 Options</font></td>
		<td width="10" class="light-orange">&nbsp;</td>
		<td width="150"><hr noshade class="light-orange-line"></td>
		<td width="10"><hr noshade class="light-orange-line"></td>
		<td width="440"><hr noshade class="light-orange-line"></td>
	</tr>
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata"><div align=right><font color="#000000">IPv6 Message Fetch Type</font></div></td>
		<td class="tabdata"><div align=center>:</div></td>
		<td class="tabdata"><font color="#000000">Static Mode</font> </td>
	</tr>
	<tr>
    <td class="light-orange">&nbsp;</td><td class="light-orange">&nbsp;</td><td class="tabdata"><div align=right>
    <font color="#000000">IPv6 Address</font>    </div></td><td class="tabdata"><div align=center>:</div></td><td class="tabdata">
		<INPUT TYPE="TEXT" NAME="wan_IPv6Addr" SIZE="39" MAXLENGTH="39"		
		<%if tcwebApi_get("WebCustom_Entry", "isCZGeneralSupported", "h")="Yes" Then%>
		 	VALUE= "<% if tcWebApi_get("Wan_PVC","IPADDR6","h") <> "N/A" then tcWebApi_get("Wan_PVC","IPADDR6","s") else asp_Write("::") end if%>" >
		<%else%>
			VALUE= "<% if tcWebApi_get("Wan_PVC","IPADDR6","h") <> "N/A" then tcWebApi_get("Wan_PVC","IPADDR6","s") end if%>" >	
		<%end if%>
		
 		<font size=+1>&nbsp;/&nbsp;</font><INPUT TYPE="TEXT" NAME="wan_IPv6Prefix" SIZE="3" MAXLENGTH="3" 
		<%if tcwebApi_get("WebCustom_Entry", "isCZGeneralSupported", "h")="Yes" Then%>
		 	VALUE= "<% if tcWebApi_get("Wan_PVC","PREFIX6","h") <> "N/A" then tcWebApi_get("Wan_PVC","PREFIX6","s") else asp_Write("0") end if%>" >
		<%else%>
			VALUE= "<% if tcWebApi_get("Wan_PVC","PREFIX6","h") <> "N/A" then tcWebApi_get("Wan_PVC","PREFIX6","s") end if%>" >	
		<%end if%>
        </td>
     </tr>
	 <tr>
    <td class="light-orange">&nbsp;</td><td class="light-orange">&nbsp;</td><td class="tabdata"><div align=right>
    <font color="#000000">IPv6 Default Gateway</font>    </div></td><td class="tabdata"><div align=center>:</div></td><td class="tabdata">
 		<INPUT TYPE="TEXT" NAME="wan_IPv6DefGw" SIZE="39" MAXLENGTH="39"		
		<%if tcwebApi_get("WebCustom_Entry", "isCZGeneralSupported", "h")="Yes" Then%>
		 	VALUE= "<% if tcWebApi_get("Wan_PVC","DEFGATEWAY6","h") <> "N/A" then tcWebApi_get("Wan_PVC","DEFGATEWAY6","s") else asp_Write("::") end if%>" >
		<%else%>
			VALUE= "<% if tcWebApi_get("Wan_PVC","DEFGATEWAY6","h") <> "N/A" then tcWebApi_get("Wan_PVC","DEFGATEWAY6","s") end if%>" >	
		<%end if%>
       </td>
     </tr>
	 
	 <tr>
    <td class="light-orange">&nbsp;</td><td class="light-orange">&nbsp;</td><td class="tabdata"><div align=right>
    <font color="#000000">IPv6 DNS Server1</font>    </div></td><td class="tabdata"><div align=center>:</div></td><td class="tabdata">
  		<INPUT TYPE="TEXT" NAME="wan_IPv6DNS1" SIZE="39" MAXLENGTH="39"		
		<%if tcwebApi_get("WebCustom_Entry", "isCZGeneralSupported", "h")="Yes" Then%>
		 	VALUE= "<% if tcWebApi_get("Wan_PVC","DNSIPv61st","h") <> "N/A" then tcWebApi_get("Wan_PVC","DNSIPv61st","s") else asp_Write("::") end if%>" >
		<%else%>
			VALUE= "<% if tcWebApi_get("Wan_PVC","DNSIPv61st","h") <> "N/A" then tcWebApi_get("Wan_PVC","DNSIPv61st","s") end if%>" >	
		<%end if%>
       </td>
     </tr>
	 
	 <tr>
    <td class="light-orange">&nbsp;</td><td class="light-orange">&nbsp;</td><td class="tabdata"><div align=right>
    <font color="#000000">IPv6 DNS Server2</font>    </div></td><td class="tabdata"><div align=center>:</div></td><td class="tabdata">
   		<INPUT TYPE="TEXT" NAME="wan_IPv6DNS2" SIZE="39" MAXLENGTH="39"		
		<%if tcwebApi_get("WebCustom_Entry", "isCZGeneralSupported", "h")="Yes" Then%>
		 	VALUE= "<% if tcWebApi_get("Wan_PVC","DNSIPv62nd","h") <> "N/A" then tcWebApi_get("Wan_PVC","DNSIPv62nd","s") else asp_Write("::") end if%>" >
		<%else%>
			VALUE= "<% if tcWebApi_get("Wan_PVC","DNSIPv62nd","h") <> "N/A" then tcWebApi_get("Wan_PVC","DNSIPv62nd","s") end if%>" >	
		<%end if%>
       </td>
     </tr>
<%if tcWebApi_get("WebCurSet_Entry","UserMode","h")<>"1" then%> 
	 <tr>
		<td height="23" class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata"><div align=right><font color="#000000">MLD Proxy</font></div></td>
		<td class="tabdata"><div align=center>:</div></td>
		<td class="tabdata">
			<INPUT TYPE="RADIO" NAME="wan_MLD1" VALUE="Yes" <% if tcWebApi_get("Wan_PVC","MLDproxy","h") = "Yes" then asp_Write("checked")end if %> > <font color="#000000">Enable</font>
			<INPUT TYPE="RADIO" NAME="wan_MLD1" VALUE="No"  <% if tcWebApi_get("Wan_PVC","MLDproxy","h") = "No" then asp_Write("checked") elseif tcWebApi_get("Wan_PVC","MLDproxy","h") = "N/A" then asp_Write("checked") end if %> > <font color="#000000">Disable</font></td>
	</tr>
<%end if%>
<% If tcWebApi_get("WebCustom_Entry","isRipngSupported","h") = "Yes" Then %>
	 <tr>
		<td height="23" class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata"><div align=right><font color="#000000">Ripng Static</font></div></td>
		<td class="tabdata"><div align=center>:</div></td>
		<td class="tabdata">
 				<INPUT TYPE="RADIO" NAME="ripngEnableRadio1" VALUE="1"  onClick="ripngEnableChanged1()" <%If tcWebApi_get("Wan_PVC","RIPNGENABLE","h") = "1" then asp_Write("checked") end if%> > 
                <font color="#000000">Enable</font>
				<INPUT TYPE="RADIO" NAME="ripngEnableRadio1" VALUE="0"  onClick="ripngEnableChanged1()" <%If tcWebApi_get("Wan_PVC","RIPNGENABLE","h") <> "1" then asp_Write("checked") end if%> > 
                <font color="#000000">Disable</font>  </td>
	</tr>
	 <tr id="div_ripng_direction1">
		<td height="23" class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata"><div align=right><font color="#000000">Ripng Direction</font></div></td>
		<td class="tabdata"><div align=center>:</div></td>
		<td class="tabdata">
			<SELECT NAME="wan_Ripng_Dir1" SIZE="1">
			<OPTION <% if tcWebApi_get("Wan_PVC","RIPNGDIRECTION","h") = "None" then asp_Write("selected") end if %>>None
			<OPTION <% if tcWebApi_get("Wan_PVC","RIPNGDIRECTION","h") = "Both" then asp_Write("selected") end if %>>Both
			<OPTION <% if tcWebApi_get("Wan_PVC","RIPNGDIRECTION","h") = "IN Only" then asp_Write("selected") end if %>>IN Only
			<OPTION <% if tcWebApi_get("Wan_PVC","RIPNGDIRECTION","h") = "OUT Only" then asp_Write("selected") end if %>>OUT Only
			</SELECT>
	</tr>
	<script language="JavaScript" type="text/JavaScript">
			ripngEnableChanged1();
	</script>
<%end if%>
	</table></div>
	<div align=center id="div_isp1dsl">
	<table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
	<tr>
		<td width="150" class="title-sub"><font color="#000000">Dual Stack Lite</font></td>
		<td width="10" class="light-orange">&nbsp;</td>
		<td width="150"><hr noshade class="light-orange-line"></td>
		<td width="10"><hr noshade class="light-orange-line"></td>
		<td width="440"><hr noshade class="light-orange-line"></td>
	</tr>

	<tr>
		<td height="27" class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata"><div align=right><font color="#000000">Enable</font></div></td>
		<td class="tabdata"><div align=center>:</div></td>
		<td class="tabdata">
			<INPUT TYPE="RADIO" NAME="DSLITEEnableRadio1" VALUE="Yes" onClick="doDSLiteEnable(1)" <% if tcWebApi_get("Wan_PVC","DSLITEEnable","h") = "Yes" then asp_Write("checked")  end if %> >
          <font color="#000000">Enable</font> 
          <INPUT TYPE="RADIO" NAME="DSLITEEnableRadio1" VALUE="No" onClick="doDSLiteDisable(1)" <% if tcWebApi_get("Wan_PVC","DSLITEEnable","h") = "No" then asp_Write("checked")  elseif tcWebApi_get("Wan_PVC","DSLITEEnable","h") = "N/A" then asp_Write("checked") end if %> >
          <font color="#000000">Disable</font> </td>
	</tr>
	<tr  id="div_dslite1_0">
		<td height="27" class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata"><div align=right><font color="#000000">Mode</font></div></td>
		<td class="tabdata"><div align=center>:</div></td>
		<td class="tabdata"> 
          <font color="#000000">Manual</font></td>
	</tr>

	<tr id="div_dslite1_1">
		<td height="29" class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata"><div align=right><font color="#000000">Remote Address</font></div></td>
		<td class="tabdata"><div align=center>:</div></td>
		<td class="tabdata"><INPUT TYPE="TEXT" NAME="DSLITEAddr1" SIZE="39" MAXLENGTH="39" VALUE= <% if tcWebApi_get("Wan_PVC","DSLITEAddr","h") <> "N/A" then tcWebApi_get("Wan_PVC","DSLITEAddr","s") end if%> >
		<script language="JavaScript" type="text/JavaScript">
		if (document.Alpha_WAN.isDSLITESupported.value == "1"){
			if(document.Alpha_WAN.DSLITEEnableRadio1[0].checked)
				doDSLiteEnable(1);
			else
				doDSLiteDisable(1);
		}
		</script></td>
	</tr>
	</table></div>
</div>
<!-- jrchen add end -->
 	


<div id="div_isp2">        
<table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
	<tr>
		<td width="150" height="30" class="title-main"><font color="#FFFFFF">PPPoE<%if tcWebApi_get("Wan_Common","TransMode","h") = "ATM" then%>/PPPoA<%end if%></font></td>
		<td width="10" class="black">&nbsp;</td>
		<td width="150"><hr noshade></td>
		<td width="10"><hr noshade></td>
		<td width="440"><hr noshade></td>
	</tr>
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata"><div align=right><font color="#000000">Username</font></div></td>
		<td class="tabdata"><div align=center>:</div></td>
		<td class="tabdata">
		<TABLE cellSpacing=0 cellPadding=0 align=left border=0>
        	<TBODY>
		<TR>
		  <% if tcWebApi_get("Info_Wan","TTNETGuiSupported","h") <> "Yes" then %>
          		<INPUT TYPE="TEXT" NAME="wan_PPPUsername" SIZE="32" MAXLENGTH="64" VALUE="<%if tcWebApi_get("Wan_PVC","USERNAME","h") <> "N/A" then tcWebApi_get("Wan_PVC","USERNAME","s") end if%>" >
		  <% elseif tcWebApi_get("Info_Wan","TTNETGuiSupported","h") = "Yes" then %>
		  	<TD style="BORDER-RIGHT-STYLE: none">
          			<INPUT TYPE="TEXT" NAME="wan_PPPUsername" SIZE="32" MAXLENGTH="64" VALUE="" >
		  	</td>
		  	<TD style="BORDER-LEFT-STYLE: none">
          	<DIV style="POSITION: relative"> 
                      <SELECT name="wan_PPPDomain" style="WIDTH: 70px; HEIGHT: 22px" size="1" onchange="doDomainChange(this.selectedIndex)" >
                        <OPTION>@ttnet 
                        <OPTION>Other 
                      </SELECT>
                      <DIV id="IFrame1div" style="LEFT: 0px; POSITION: absolute; TOP: 0px" name="IFrame1div"> 
                        <IFRAME id="IFrame1" style="DISPLAY: none; WIDTH: 100px; HEIGHT: 20px" name="IFrame1" src="about:blank" frameBorder="0" scrolling="no"></IFRAME>
                      </DIV>
                      <DIV id="suffixText" style="LEFT: 0px; POSITION: absolute; TOP: 0px" name="suffixText"> 
                        <INPUT class="tableinputvalue" name="wan_CompanyName" style="DISPLAY: none; LEFT: 0px; WIDTH: 100px; POSITION: absolute; HEIGHT: 20px" onfocus="setTtnetCompanyDefalutText(this)" maxLength="24" value="">
                      </DIV>
                    </DIV>
          		<input type="HIDDEN" name="username1" value=<% if tcWebApi_get("Wan_PVC","USERNAME","h") <> "N/A" then tcWebApi_get("Wan_PVC","USERNAME","s") end if %>>
		  <% end if %>
		  <input type="HIDDEN" name="TTNETGuiSupport" value=<% if tcWebApi_get("Info_Wan","TTNETGuiSupported","h") = "Yes" then asp_Write("1") elseif tcWebApi_get("Info_Wan","TTNETGuiSupported","h") <> "Yes" then asp_Write("0") end if %>>
		  </TD></TR></TBODY></TABLE></td>
      </tr>
	<tr id=PPPUsername_ErrorMsg_TR style="DISPLAY: none">
    		<td class="light-orange">&nbsp;</td>
    		<td class="light-orange">&nbsp;</td>
    		<td class="tabdata">&nbsp;</td>
    		<td class="tabdata">&nbsp;</td>
    		<td class="tabdata"><FONT color=#ff0000>The domain name is invalid. Please check!</FONT> </td>
	</tr>
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata"><div align=right><font color="#000000">Password</font></div></td>
		<td class="tabdata"><div align=center>:</div></td>
		<td class="tabdata">
			<INPUT TYPE="PASSWORD" NAME="wan_PPPPassword" SIZE="32" MAXLENGTH="64" VALUE="" ></td>
	</tr>
<%if tcWebApi_get("Wan_Common","TransMode","h") = "ATM" then%>
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata"><div align=right><font color="#000000">Encapsulation</font></div></td>
		<td class="tabdata"><div align=center>:</div></td>
		<td class="tabdata">
			<SELECT NAME="wan_Encap2" SIZE="1" onChange="doEncapChange()">
				<option <% if tcWebApi_get("Wan_PVC","ENCAP","h") = "PPPoE LLC" then asp_Write("selected") end if %>>PPPoE LLC
				<option <% if tcWebApi_get("Wan_PVC","ENCAP","h") = "PPPoE VC-Mux" then asp_Write("selected") end if %>>PPPoE VC-Mux
			<%if tcWebApi_get("WebCustom_Entry","sharepvc","h") <> "Yes" then%>
				<option <% if tcWebApi_get("Wan_PVC","ENCAP","h") = "PPPoA LLC" then asp_Write("selected") end if %>>PPPoA LLC
				<option <% if tcWebApi_get("Wan_PVC","ENCAP","h") = "PPPoA VC-Mux" then asp_Write("selected") end if %>>PPPoA VC-Mux
			<% end if%>
			</SELECT></td>
	</tr>
	<% if tcWebApi_get("WebCustom_Entry","isPPPAuthen","h") = "Yes" then %>
	<tr>
		<td  class="light-orange">&nbsp;</td>
		<td class="light-orange">&nbsp;</td>
		<td class="tabdata"><div align=right><font color="#000000">PPP Authentication</font></div></td>
		<td class="tabdata"><div align=center>:</div></td>
		
      <td class="tabdata"> <select name="PPPAuthen" size="1" >
	  	  <option <% if tcWebApi_get("Wan_PVC","AUTHEN","h") = "AUTO" then asp_Write("selected") elseif tcWebApi_get("Wan_PVC","AUTHEN","h") = "N/A" then asp_Write("selected") end if %> value="AUTO">AUTO
		  <option <% if tcWebApi_get("Wan_PVC","AUTHEN","h") = "PAP" then asp_Write("selected") end if %> value="PAP">PAP
          <option <% if tcWebApi_get("Wan_PVC","AUTHEN","h") = "CHAP" then asp_Write("selected") end if %> value="CHAP">CHAP 
        </select></td>
	</tr>
	<%end if%>
<%end if%>
	<tr id="div_isp2encap">
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata"><div align=right><font color="#000000">Bridge Interface for PPPoE</font></div></td>
		<td class="tabdata"><div align=center>:</div></td>
		<td class="tabdata">
			<input type="RADIO" name="wan_BridgeInterface2" value="Yes" <% if tcWebApi_get("Wan_PVC","BridgeInterface","h") = "Yes" then asp_Write("checked")end if %> ><font color="#000000">Activated</font> 
          	<INPUT TYPE="RADIO" NAME="wan_BridgeInterface2" VALUE="No"  <% if tcWebApi_get("Wan_PVC","BridgeInterface","h") = "No" then asp_Write("checked") elseif tcWebApi_get("Wan_PVC","BridgeInterface","h") = "N/A" then asp_Write("checked") end if %> ><font color="#000000">Deactivated</font></td>
	</tr>
	<tr>
		<td class="title-sub"><font color="#000000">Connection Setting</font></td>
		<td class="light-orange">&nbsp;</td>
		<td><hr noshade class="light-orange-line"></td>
		<td><hr noshade class="light-orange-line"></td>
		<td><hr noshade class="light-orange-line"></td>
	</tr>
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata"><div align=right><font color="#000000">Connection </font></div></td>
		<td class="tabdata"><div align=center>:</div></td>
		<td class="tabdata">
			<INPUT TYPE="RADIO" NAME="wan_ConnectSelect" VALUE="Connect_Keep_Alive"  <%if tcWebApi_get("Wan_PVC","CONNECTION","h") = "Connect_Keep_Alive" then asp_Write("checked") elseif tcWebApi_get("Wan_PVC","CONNECTION","h") = "N/A" then asp_Write("checked") end if%> onClick="WANChkIdleTimeT();"><font color="#000000">Always On (Recommended)</font></td>
	</tr>
	<tr id="connondemand_info">
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata">&nbsp;</td>
		<td>&nbsp;</td>
		<td class="tabdata">
			<INPUT TYPE="RADIO" NAME="wan_ConnectSelect" VALUE="Connect_on_Demand"   <%if tcWebApi_get("Wan_PVC","CONNECTION","h") = "Connect_on_Demand" then asp_Write("checked") end if%> onClick="WANChkIdleTimeT();"><font color="#000000">Connect On-Demand (Close if idle for</font>        
			<INPUT TYPE="TEXT" NAME="wan_IdleTimeT" SIZE="5" MAXLENGTH="3" VALUE=<% if tcWebApi_get("Wan_PVC","CLOSEIFIDLE","h") = "N/A" then asp_Write("0") else tcWebApi_get("Wan_PVC","CLOSEIFIDLE","s") end if%> ><font color="#000000">minutes)</font></td>
	</tr>
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata">&nbsp;</td>
		<td>&nbsp;</td>
		<td class="tabdata">
			<INPUT TYPE="RADIO" NAME="wan_ConnectSelect" VALUE="Connect_Manually"  <%if tcWebApi_get("Wan_PVC","CONNECTION","h") = "Connect_Manually" then asp_Write("checked")  end if%> onClick="WANChkIdleTimeT();"><font color="#000000">Connect Manually</font></td>
	</tr>	
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata"><div align=right><font color="#000000">TCP MSS Option</font></div></td>
		<td class="tabdata"><div align=center>:</div></td>
		<td class="tabdata"><font color="#000000">TCP MSS(0 means use default)</font>
			<INPUT TYPE="TEXT" NAME="wan_TCPMSS" SIZE="5" MAXLENGTH="4" VALUE=<% if tcWebApi_get("Wan_PVC","MSS","h") = "N/A" then asp_Write("0") else tcWebApi_get("Wan_PVC","MSS","s") end if%> ><font color="#000000">bytes</font></td>
	</tr>

	<tr>
		<td width="150" height="30" class="title-main"><font color="#FFFFFF">IP Options</font></td>
		<td width="10" class="black">&nbsp;</td>
		<td width="150"><hr noshade></td>
		<td width="10"><hr noshade></td>
		<td width="440"><hr noshade></td>
	</tr>
	
	<tr>
		<td class="title-sub"><font color="#000000">IP Common Options</font></td>
		<td class="light-orange">&nbsp;</td>
		<td><hr noshade class="light-orange-line"></td>
		<td><hr noshade class="light-orange-line"></td>
		<td><hr noshade class="light-orange-line"></td>
	</tr>

	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata"><div align=right><font color="#000000">Default Route</font></div></td>
		<td class="tabdata"><div align=center>:</div></td>
		<td class="tabdata">
			<INPUT TYPE="RADIO" NAME="WAN_DefaultRoute2" VALUE="Yes"  onClick="doDefaultRouteYes()" <% if tcWebApi_get("Wan_PVC","DEFAULTROUTE","h") = "Yes" then asp_Write("checked")end if %> ><font color="#000000">Yes</font>        
			<INPUT TYPE="RADIO" NAME="WAN_DefaultRoute2" VALUE="No"   onClick="doDefaultRouteNo()" <% if tcWebApi_get("Wan_PVC","DEFAULTROUTE","h") = "No" then asp_Write("checked") elseif tcWebApi_get("Wan_PVC","DEFAULTROUTE","h") = "N/A" then asp_Write("checked") end if %> ><font color="#000000">No</font></td>
	</tr>
</table>

<!--ipv4 and ipv4/v6 dgk wait modify -->
<div id="div_ipv4getip">
<table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">	
	<tr>
		<td class="title-sub"><font color="#000000">IPv4 Options</font></td>
		<td class="light-orange">&nbsp;</td>
		<td><hr noshade class="light-orange-line"></td>
		<td><hr noshade class="light-orange-line"></td>
		<td><hr noshade class="light-orange-line"></td>
	</tr>
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata"><div align=right><font color="#000000">Get IP Address</font></div></td>
		<td class="tabdata"><div align=center>:</div></td>
		<td class="tabdata">
			<INPUT TYPE="RADIO" NAME="wan_PPPGetIP" VALUE="Static" 	<%if tcWebApi_get("Wan_PVC","PPPGETIP","h") = "Static" then asp_Write("checked") elseif tcWebApi_get("Wan_PVC","PPPGETIP","h") = "N/A" then asp_Write("checked") end if%> onClick="pppStaticCheck()"><font color="#000000">Static</font>
			<INPUT TYPE="RADIO" NAME="wan_PPPGetIP" VALUE="Dynamic" <%if tcWebApi_get("Wan_PVC","PPPGETIP","h") = "Dynamic" then asp_Write("checked") end if%> onClick="pppStaticCheck()"><font color="#000000">Dynamic</font></td>
	</tr>
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata"><div align=right><font color="#000000">Static IP Address</font></div></td>
		<td class="tabdata"><div align=center>:</div></td>
		<td class="tabdata">
			<INPUT TYPE="TEXT" NAME="wan_StaticIPaddr2" SIZE="16" MAXLENGTH="15" VALUE= <% if tcWebApi_get("Wan_PVC","IPADDR","h") <> "N/A" then tcWebApi_get("Wan_PVC","IPADDR","s") end if%> ></td>
	</tr>
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata"><div align=right><font color="#000000">IP Subnet Mask</font></div></td>
		<td class="tabdata"><div align=center>:</div></td>
		<td class="tabdata">
			<INPUT TYPE="TEXT" NAME="wan_StaticIPSubMask2" SIZE="16" MAXLENGTH="15" VALUE=<% if tcWebApi_get("Wan_PVC","NETMASK","h") <> "N/A" then tcWebApi_get("Wan_PVC","NETMASK","s") end if%> ></td>
	</tr>
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata"><div align=right><font color="#000000">Gateway</font></div></td>
		<td class="tabdata"><div align=center>:</div></td>
		<td class="tabdata">
			<INPUT TYPE="TEXT" NAME="wan_StaticIpGateway2" SIZE="16" MAXLENGTH="15" VALUE=<% if tcWebApi_get("Wan_PVC","GATEWAY","h") <> "N/A" then tcWebApi_get("Wan_PVC","GATEWAY","s") end if%> ></td>
	</tr>
	
	<tr>
		<td width="150" class="light-orange">&nbsp;</td>
		<td width="10" class="light-orange"></td>
		<td width="150" class="tabdata"><div align=right><font color="#000000">NAT</font></div></td>
		<td width="10" class="tabdata"><div align=center>:</div></td>
		<td width="440" class="tabdata">
			<SELECT NAME="wan_NAT2" SIZE="1">
				<option <% if tcWebApi_get("Wan_PVC","NATENABLE","h") = "Enable" then asp_Write("selected") end if %>>Enable
				<option <% if tcWebApi_get("Wan_PVC","NATENABLE","h") = "Disabled" then asp_Write("selected") elseif tcWebApi_get("Wan_PVC","NATENABLE","h") = "N/A" then asp_Write("selected") end if %>>Disabled
			</SELECT></td>
	</tr>
	
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata"><div align=right><font color="#000000">Dynamic Route</font></div></td>
		<td width="10" class="tabdata"><div align=center>:</div></td>
		<td class="tabdata">
			<SELECT NAME="wan_RIP2" SIZE="1" >
				<option <% if tcWebApi_get("Wan_PVC","RIPVERSION","h") = "RIP1" then asp_Write("selected") end if %>>RIP1
				<option <% if tcWebApi_get("Wan_PVC","RIPVERSION","h") = "RIP2" then asp_Write("selected") end if %>>RIP2
			</SELECT>
			<font color="#000000">Direction</font>
			<SELECT NAME="wan_RIP_Dir2" SIZE="1" >
				<option <% if tcWebApi_get("Wan_PVC","DIRECTION","h") = "None" then asp_Write("selected") end if %>>None
				<option <% if tcWebApi_get("Wan_PVC","DIRECTION","h") = "Both" then asp_Write("selected") end if %>>Both
				<option <% if tcWebApi_get("Wan_PVC","DIRECTION","h") = "IN Only" then asp_Write("selected") end if %>>IN Only
				<option <% if tcWebApi_get("Wan_PVC","DIRECTION","h") = "OUT Only" then asp_Write("selected") end if %>> OUT Only
			</SELECT></td>
	</tr>
	
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata"><div align=right><font color="#000000">TCP MTU Option</font></div></td>
		<td class="tabdata"><div align=center>:</div></td>
		<td class="tabdata"><font color="#000000">TCP MTU(0 means use default:1492)</font>
		<INPUT TYPE="TEXT" NAME="wan_TCPMTU2" SIZE="5" MAXLENGTH="4" VALUE=<% if tcWebApi_get("Wan_PVC","MTU","h") = "N/A" then asp_Write("0") else tcWebApi_get("Wan_PVC","MTU","s") end if%> ><font color="#000000">bytes</font></td>
	</tr>
<%if tcWebApi_get("WebCurSet_Entry","UserMode","h")<>"1" then%>
	<tr>
		<td width="150" height="27" class="light-orange">&nbsp;</td>
		<td width="10" class="light-orange"></td>
		<td width="150" class="tabdata"><div align=right><font color="#000000">IGMP Proxy</font></div></td>
		<td width="10" class="tabdata"><div align=center>:</div></td>
		<td width="440" class="tabdata">
			<INPUT TYPE="RADIO" NAME="wan_IGMP2" VALUE="Yes" <% if tcWebApi_get("Wan_PVC","IGMPproxy","h") = "Yes" then asp_Write("checked")end if %> > <font color="#000000">Enable</font>        
			<INPUT TYPE="RADIO" NAME="wan_IGMP2" VALUE="No"  <% if tcWebApi_get("Wan_PVC","IGMPproxy","h") = "No" then asp_Write("checked") elseif tcWebApi_get("Wan_PVC","IGMPproxy","h") = "N/A" then asp_Write("checked") end if %> > <font color="#000000">Disable</font></td>
	</tr>
<%end if%>
	</table></div>
<!-- jrchen add -->	

<!-- ipv4 and ipv6 dgk wait modify-->
<div id="div_ipv6pdm">
<table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">	
	
	<tr>
		<td width="150" class="title-sub"><font color="#000000">IPv6 Options</font></td>
		<td width="10" class="light-orange">&nbsp;</td>
		<td width="150"><hr noshade class="light-orange-line"></td>
		<td width="10"><hr noshade class="light-orange-line"></td>
		<td width="440"><hr noshade class="light-orange-line"></td>
	</tr>
		

			
	
	
	<tr>
		<td height="27" class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata"><div align=right><font color="#000000">DHCP IPv6 Mode</font></div></td>
		<td class="tabdata"><div align=center>:</div></td>
		<td class="tabdata">
			<%if tcWebApi_get("WebCustom_Entry","isIPv6PrivacyAddrsSupported","h") = "Yes" then%>
			<INPUT TYPE="RADIO" NAME="PPPIPv6ModeRadio" VALUE="1" onClick="doPPPv6ModeDHCP();doPrivacyaddrsShow2();" <% if tcWebApi_get("Wan_PVC","PPPv6Mode","h") = "1" then asp_Write("checked")  end if %> > <font color="#000000">DHCP</font>
			<INPUT TYPE="RADIO" NAME="PPPIPv6ModeRadio" VALUE="0" onClick="doPPPv6ModeSLAAC();doPrivacyaddrsShow2();" <% if tcWebApi_get("Wan_PVC","PPPv6Mode","h") = "0" then asp_Write("checked") elseif tcWebApi_get("Wan_PVC","PPPv6Mode","h") = "N/A" then asp_Write("checked") end if %> > <font color="#000000">SLAAC</font>        
			<%else %>
			<INPUT TYPE="RADIO" NAME="PPPIPv6ModeRadio" VALUE="1" onClick="doPPPv6ModeDHCP()" <% if tcWebApi_get("Wan_PVC","PPPv6Mode","h") = "1" then asp_Write("checked")  end if %> > <font color="#000000">DHCP</font>
			<INPUT TYPE="RADIO" NAME="PPPIPv6ModeRadio" VALUE="0" onClick="doPPPv6ModeSLAAC()" <% if tcWebApi_get("Wan_PVC","PPPv6Mode","h") = "0" then asp_Write("checked") elseif tcWebApi_get("Wan_PVC","PPPv6Mode","h") = "N/A" then asp_Write("checked") end if %> > <font color="#000000">SLAAC</font>        
			<%end if%>
					
		</td>
	</tr>
	<%if tcWebApi_get("WebCustom_Entry","isIPv6PrivacyAddrsSupported","h") = "Yes" then%>
	<tr id="div_privacyaddrs2">
		<td height="27" class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata"><div align=right><font color="#000000">Privacy Address</font></div></td>
		<td class="tabdata"><div align=center>:</div></td>
		<td class="tabdata">
			<INPUT TYPE="RADIO" NAME="privacyaddrsradio2" VALUE="2" <% if tcWebApi_get("Wan_PVC","IPv6Extension","h") = "2" then asp_Write("checked") end if %> > <font color="#000000">Enable</font>
			<INPUT TYPE="RADIO" NAME="privacyaddrsradio2" VALUE="0"  <% if tcWebApi_get("Wan_PVC","IPv6Extension","h") <> "2" then asp_Write("checked") end if %> > <font color="#000000">Disable</font></td>
			<script language="JavaScript" type="text/JavaScript">
				doPrivacyaddrsShow2();
			</script>	
	</tr>
	<%end if%>
	<tr>
		<td height="27" class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata"><div align=right><font color="#000000">DHCP PD Enable</font></div></td>
		<td class="tabdata"><div align=center>:</div></td>
		<td class="tabdata">
			<INPUT TYPE="RADIO" NAME="PPPIPv6PDRadio2" VALUE="Yes" <% if tcWebApi_get("Wan_PVC","PPPv6PD","h") = "Yes" then asp_Write("checked") end if %> > <font color="#000000">Enable</font>
			<INPUT TYPE="RADIO" NAME="PPPIPv6PDRadio2" VALUE="No"  <% if tcWebApi_get("Wan_PVC","PPPv6PD","h") = "No" then asp_Write("checked") elseif  tcWebApi_get("Wan_PVC","PPPv6PD","h") = "N/A" then asp_Write("checked") end if %> > <font color="#000000">Disable</font></td>
	</tr>
<%if tcWebApi_get("WebCurSet_Entry","UserMode","h")<>"1" then%>
	 	<tr>
		<td height="29" class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata"><div align=right><font color="#000000">MLD Proxy</font></div></td>
		<td class="tabdata"><div align=center>:</div></td>
		<td class="tabdata">
			<INPUT TYPE="RADIO" NAME="wan_MLD2" VALUE="Yes" <% if tcWebApi_get("Wan_PVC","MLDproxy","h") = "Yes" then asp_Write("checked")end if %> > <font color="#000000">Enable</font>
			<INPUT TYPE="RADIO" NAME="wan_MLD2" VALUE="No"  <% if tcWebApi_get("Wan_PVC","MLDproxy","h") = "No" then asp_Write("checked") elseif tcWebApi_get("Wan_PVC","MLDproxy","h") = "N/A" then asp_Write("checked") end if %> > <font color="#000000">Disable</font></td>
	</tr>
<%end if%>
<% If tcWebApi_get("WebCustom_Entry","isRipngSupported","h") = "Yes" Then %>
	 <tr>
		<td height="23" class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata"><div align=right><font color="#000000">Ripng PPPoE</font></div></td>
		<td class="tabdata"><div align=center>:</div></td>
		<td class="tabdata">
 				<INPUT TYPE="RADIO" NAME="ripngEnableRadio2" VALUE="1"  onClick="ripngEnableChanged2()" <%If tcWebApi_get("Wan_PVC","RIPNGENABLE","h") = "1" then asp_Write("checked") end if%> > 
                <font color="#000000">Enable</font>
				<INPUT TYPE="RADIO" NAME="ripngEnableRadio2" VALUE="0"  onClick="ripngEnableChanged2()" <%If tcWebApi_get("Wan_PVC","RIPNGENABLE","h") <> "1" then asp_Write("checked") end if%> > 
                <font color="#000000">Disable</font>  </td>
	</tr>
	 <tr id="div_ripng_direction2">
		<td height="23" class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata"><div align=right><font color="#000000">Ripng Direction</font></div></td>
		<td class="tabdata"><div align=center>:</div></td>
		<td class="tabdata">
			<SELECT NAME="wan_Ripng_Dir2" SIZE="1">
			<OPTION <% if tcWebApi_get("Wan_PVC","RIPNGDIRECTION","h") = "None" then asp_Write("selected") end if %>>None
			<OPTION <% if tcWebApi_get("Wan_PVC","RIPNGDIRECTION","h") = "Both" then asp_Write("selected") end if %>>Both
			<OPTION <% if tcWebApi_get("Wan_PVC","RIPNGDIRECTION","h") = "IN Only" then asp_Write("selected") end if %>>IN Only
			<OPTION <% if tcWebApi_get("Wan_PVC","RIPNGDIRECTION","h") = "OUT Only" then asp_Write("selected") end if %>>OUT Only
			</SELECT>
	</tr>
	<script language="JavaScript" type="text/JavaScript">
			ripngEnableChanged2();
	</script>
<%end if%>
	</table></div>
	<div align=center id="div_isp2dsl">
	<table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
	<tr>
		<td width="150" class="title-sub"><font color="#000000">Dual Stack Lite</font></td>
		<td width="10" class="light-orange">&nbsp;</td>
		<td width="150"><hr noshade class="light-orange-line"></td>
		<td width="10"><hr noshade class="light-orange-line"></td>
		<td width="440"><hr noshade class="light-orange-line"></td>
	</tr>

	<tr>
		<td height="27" class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata"><div align=right><font color="#000000">Enable</font></div></td>
		<td class="tabdata"><div align=center>:</div></td>
		<td class="tabdata">
			<INPUT TYPE="RADIO" NAME="DSLITEEnableRadio2" VALUE="Yes" onClick="doDSLiteEnable(2)" <% if tcWebApi_get("Wan_PVC","DSLITEEnable","h") = "Yes" then asp_Write("checked")  end if %> >
          <font color="#000000">Enable</font> 
          <INPUT TYPE="RADIO" NAME="DSLITEEnableRadio2" VALUE="No" onClick="doDSLiteDisable(2)" <% if tcWebApi_get("Wan_PVC","DSLITEEnable","h") = "No" then asp_Write("checked")  elseif tcWebApi_get("Wan_PVC","DSLITEEnable","h") = "N/A" then asp_Write("checked") end if %> >
          <font color="#000000">Disable</font> </td>
	</tr>
	<tr  id="div_dslite2_0">
		<td height="27" class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata"><div align=right><font color="#000000">Mode</font></div></td>
		<td class="tabdata"><div align=center>:</div></td>
		<td class="tabdata">
			<INPUT TYPE="RADIO" NAME="DSLITEModeRadio2" VALUE="0"  onClick="doDSLiteModeAuto()" <% if tcWebApi_get("Wan_PVC","DSLITEMode","h") = "0" then asp_Write("checked") elseif  tcWebApi_get("Wan_PVC","DSLITEMode","h") = "N/A" then asp_Write("checked") end if %> >
          <font color="#000000">Auto</font> 
          <INPUT TYPE="RADIO" NAME="DSLITEModeRadio2" VALUE="1"   onClick="doDSLiteModeManual()" <% if tcWebApi_get("Wan_PVC","DSLITEMode","h") = "1" then asp_Write("checked") end if %> >
          <font color="#000000">Manual</font></td>
	</tr>
	<tr id="div_dslite2_1">
		<td height="29" class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata"><div align=right><font color="#000000">Remote Address</font></div></td>
		<td class="tabdata"><div align=center>:</div></td>
		<td class="tabdata"><INPUT TYPE="TEXT" NAME="DSLITEAddr2" SIZE="39" MAXLENGTH="39" VALUE= <% if tcWebApi_get("Wan_PVC","DSLITEAddr","h") <> "N/A" then tcWebApi_get("Wan_PVC","DSLITEAddr","s") end if%> >
		<script language="JavaScript" type="text/JavaScript">
		if (document.Alpha_WAN.isDSLITESupported.value == "1"){
			if(document.Alpha_WAN.DSLITEEnableRadio2[0].checked)
				doDSLiteEnable(2);
			else
				doDSLiteDisable(2);
		}
		</script></td>
	</tr>
	</table></div>

<!-- jrchen add end -->	
</div>

<div id="div_isp3">
<%if tcWebApi_get("Wan_Common","TransMode","h") = "ATM" then%>
<table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
	<tr>
		<td width="150" height="30" class="title-main"><font color="#FFFFFF">Bridge Mode</font></td>
		<td width="10" class="black">&nbsp;</td>
		<td width="150"><hr noshade></td>
		<td width="10"><hr noshade></td>
		<td width="440"><hr noshade></td>
	</tr>
	<tr>
		<td class="light-orange">&nbsp;</td>
		<td class="light-orange"></td>
		<td class="tabdata"><div align=right><font color="#000000">Encapsulation</font></div></td>
		<td class="tabdata"><div align=center>:</div></td>
		<td class="tabdata">
			<SELECT NAME="wan_Encap3" SIZE="1">
				<option <% if tcWebApi_get("Wan_PVC","ENCAP","h") = "1483 Bridged Only LLC" then asp_Write("selected") end if %>>1483 Bridged Only LLC
				<option <% if tcWebApi_get("Wan_PVC","ENCAP","h") = "1483 Bridged Only VC-Mux" then asp_Write("selected") end if %>>1483 Bridged Only VC-Mux
			</SELECT></td>
	</tr>
</table>
<%end if%>

</div>
<table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
	<tr>
		<td width="150" class="light-orange">&nbsp;</td>
		<td width="10"  class="light-orange"></td>
		<td width="150" class="tabdata">&nbsp;</td>
		<td width="10">&nbsp;</td>
		<td width="440" class="tabdata">&nbsp;</td>
	</tr>
</table>
<table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
	<tr>
		<INPUT TYPE="HIDDEN" NAME="isPPPAuthen" VALUE="<%tcWebApi_get("WebCustom_Entry","isPPPAuthen","s")%>">
		<INPUT TYPE="HIDDEN" NAME="isWanTagChk" VALUE="<%tcWebApi_get("WebCustom_Entry","isWanTagChk","s")%>">
		<INPUT TYPE="HIDDEN" NAME="isdot1pSupport" VALUE="<%tcWebApi_get("WebCustom_Entry","isdot1pSupport","s")%>">
		<INPUT TYPE="HIDDEN" NAME="isTPIDSupported" VALUE="<%tcWebApi_get("WebCustom_Entry","isTPIDSupported","s")%>">	
		<td width="160" height="42" class="black">&nbsp;</td>
		<td width="160" class="orange">
		<script language="JavaScript" type="text/JavaScript">
		with (document.Alpha_WAN){
			/*8021q check*/
			setDisplay('div_8021q', 0);
			if((wan_8021q.value == 1) && (disp_wan_8021q.value==1)){
				setDisplay('div_8021q', 1);
			}
			if (is8021xsupport.value != "1") {
			    setDisplay('div_802_1x', 0);
			}
			if(isIPv6Supported.value != "1"){
				ipVerRadio[0].checked = true;
				doIPVersionChangeIPv4();
				setDisplay('div_isipv6sup', 0);
			}
			else{
				doIPVersionChangeIPv4();
			}
			//vlan id check
			wan8021QCheck();
			WANChkIdleTimeT();
		}
		</script></td>
		<td width="440" class="orange">
			<INPUT TYPE="button" NAME="SaveBtn" VALUE="SAVE" onClick="uiSave()" class="sbutton"><%if tcWebApi_get("Wan_PVC","ISP","h") <> "3" then %><%if tcWebApi_get("Wan_PVC","DEFAULTROUTE","h") = "No" then %><INPUT TYPE="BUTTON" NAME="DeleteBtn" VALUE="DELETE" onClick="doDelete()"><% End If %><% End If %><%if tcWebApi_get("Wan_PVC","ISP","h") = "3" then %><%if tcWebApi_get("Wan_PVC","Active","h") = "Yes" then %><INPUT TYPE="BUTTON" NAME="DeleteBtn" VALUE="DELETE" onClick="doDelete()"><% End If %><% End If %>
	<INPUT TYPE="HIDDEN" NAME="DefaultDmz_Active" VALUE="No" >
	<INPUT TYPE="HIDDEN" NAME="DefaultDmz_HostIP" VALUE="0.0.0.0" ></td>
	</tr>      
</table></div></form></body></html>
