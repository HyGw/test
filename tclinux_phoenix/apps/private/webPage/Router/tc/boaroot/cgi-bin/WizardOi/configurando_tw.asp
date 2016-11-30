<%
if request_form("encryption") <> "" Then
	tcWebApi_constSet("WebCurSet_Entry","wlan_id","0")
	tcWebApi_set("WLan_Entry","SSID","network")
	tcWebApi_set("WLan_Entry","AuthMode","encryption")
	if request_form("encryption") = "WPAPSK" Then
		tcWebApi_set("WLan_Entry","WPAPSK","password")
		tcWebApi_set("WLan_Entry","EncrypType","TKIPAES")
	end if
	if request_form("encryption") = "WPA2PSK" Then
		tcWebApi_set("WLan_Entry","WPAPSK","password")
		tcWebApi_set("WLan_Entry","EncrypType","TKIPAES")
	end if				
	if request_form("encryption") = "WPAPSKWPA2PSK" Then
		tcWebApi_set("WLan_Entry","WPAPSK","password")
		tcWebApi_set("WLan_Entry","EncrypType","TKIPAES")
	end if				
	if request_form("encryption") = "WPA" Then
		tcWebApi_set("WLan_Entry","RADIUS_Key","password")
	end if
	if request_form("encryption") = "WPA2" Then
		tcWebApi_set("WLan_Entry","RADIUS_Key","password")
	end if
	if request_form("encryption") = "WPAPSKWPA2PSK" Then
		tcWebApi_set("WLan_Entry","RADIUS_Key","password")
	end if	
	if request_form("encryption") = "WEP-64Bits" Then
		tcWebApi_set("WLan_Entry","Key1Str","password")
		tcWebApi_constSet("WLan_Entry","WPSConfMode","0")
	end if
		if request_form("encryption") = "WEP-128Bits" Then
		tcWebApi_set("WLan_Entry","Key1Str","password")
		tcWebApi_constSet("WLan_Entry","WPSConfMode","0")
	end if		
		
	tcWebApi_commit("WLan_Entry")
end if
tcWebApi_constSet("WebCurSet_Entry","wan_pvc","0")
if request_form("WAN_NoWanIpFlag") = "1" Then
	tcWebApi_set("Wan_PVC","USERNAME","wizardOIAuth_PPPUserName")
	tcWebApi_set("Wan_PVC","PASSWORD","wizardOIAuth_PPPUserPassword")
end if
tcWebApi_commit("Wan_PVC")
%>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01//EN" "http://www.w3.org/TR/html4/strict.dtd">
<html lang="pt-br">
    <head>	
		<meta http-equiv=Content-Type content="text/html; charset=UTF-8">
		<meta http-equiv="Pragma" content="no-cache">
		<meta http-equiv="Expires" content="-1">
		
        <title>
		OI VELOX 
		</title>

        <link rel="shortcut icon" href="favicon.ico" >

        <link href="../../include/css/style.css" rel="stylesheet" type="text/css" />
        <link href="../../include/css/layout.css" rel="stylesheet" type="text/css" />
        <link href="../../include/css/home.css" rel="stylesheet" type="text/css" />
        <link href="../../include/css/pages.css" rel="stylesheet" type="text/css" />

        <script src="../../include/scripts/main.js" type="text/javascript"></script>
		
		<script type="text/javascript" language="JavaScript">
		<%if request_form("WizardOI_States") = "1"  Then %>
			<%if tcWebApi_get("Info_Adsl","lineState","h") <> "up" then%>
			navPage( 'authentication_failure_velox' );
			<% else %>
			navPage( 'authentication_failure' );
			<% end if %>
		<% end if %>
		function redirectToErrorPage()
		{
			document.configure_modem_detect.submit();
		}
		var refreshLimit = 40; //2 miniute
		var wanip;
		var intervalFlag;
		function refreshWizardPage()
		{
			window.frames["wanUpStatus"].location.href="wanUpStatus.asp";
			refreshLimit--;
			if((refreshLimit == 0) && (null != intervalFlag))
				clearInterval(intervalFlag);			
			checkConnect();
		}
		function checkConnect()
		{
			wanip = window.frames["wanUpStatus"].wanip;
			if( (wanip != "N/A") && (typeof(wanip) != "undefined"))
			{
				if(null != intervalFlag)
					clearInterval(intervalFlag);
				<%if tcWebApi_get("GUI_Entry0","wzOISetted","h") <> "1" then %>
				navPage( 'rede_fon' );
				<%else%>
				<%if tcWebApi_get("WebCustom_Entry","oiWzPage","h") = "5" then%>
				navPage( 'rede_fon' );
				<%else%>
				<%if tcWebApi_get("GUI_Entry0", "OI_USER_ENTERPRISE", "h") <> "1" then %>
				navPage( 'browsingu' );
				<%else%>
				navPage( 'browsingem' );
				<%end if%>	
				<%end if%>
				<%end if%>
			}
			else
			{
				if(refreshLimit == 0)
				{
					redirectToErrorPage();
				}
			}
		}	
		function doTest()
		{
			<%if request_form("WizardOI_States") <> "1"  Then %>
			intervalFlag = setInterval("refreshWizardPage();", 3*1000);	
			<%end if%>	
		}	
		</script>
    </head>

    <body onLoad="doTest()">
	<form name="configure_modem_detect" method="post" action="/cgi-bin/WizardOi/configurando.asp">
        <div id="layout_bg_left" class="layout">
            <div id="layout_bg_right" class="layout">
                &nbsp;
            </div>
        </div>
         <table border="0" cellpadding="0" cellspacing="0" width="100%" height="100%" class="layout_pages">
             <tr>
                <td height="94" valign="top" align="left">
                <div id="layout_top">
                    <div id="layout_top_content">
                        <img src="../../include/images/layout_top.png" width="181" height="60" title="OI VELOX" alt="OI VELOX">
                    </div>
                </div>
            	</td>
            </tr>
            <tr>
            	<td valign="middle">
                <div id="layout_content">
                <div class="page_content">
                    <div id="page_setting_content"> 
                            <div class="page_txt_load">
							<%if tcWebApi_get("GUI_Entry0","wzOISetted","h") <> "1" then %>
												<%if tcWebApi_get("WebCustom_Entry", "isCZH108LSupported", "h") = "Yes" then %>
															 Aguarde enquanto seu modem MTK &eacute; configurado	
												<%else%>
                               Aguarde enquanto seu modem MTK &eacute; configurado
                        <%end if%>       
							 <%else%>
                               Aguarde enquanto sua conex&atilde;o &eacute; verificada
							 <%end if%>
                            </div>
                            <div class="page_load">
                                <img src="../../include/images/page_load.gif" width="140" height="18" alt="Load" />
                            </div>
                    </div>
                </div>
                </div>
				<DIV style="DISPLAY: none"><iframe id="wanUpStatus" name="wanUpStatus" frameborder="0" width="0" height="0"></iframe></DIV>
                </td>
            </tr>
        </table>
		<input type="hidden" name="WizardOI_States" value="1"> 
		</form>
    </body>
</html>
