<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01//EN" "http://www.w3.org/TR/html4/strict.dtd">
<html lang="pt-br">
<head>	
<meta http-equiv=Content-Type content="text/html; charset=iso-8859-1">
<meta http-equiv="Pragma" content="no-cache">
<meta http-equiv="Expires" content="-1">     
	<title>
	OI VELOX 
	</title>

	<link rel="shortcut icon" href="../../include/images/favicon.ico" >

	<link href="../../include/css/style.css" rel="stylesheet" type="text/css" />
	<link href="../../include/css/layout.css" rel="stylesheet" type="text/css" />
	<link href="../../include/css/home.css" rel="stylesheet" type="text/css" />
	<link href="../../include/css/pages.css" rel="stylesheet" type="text/css" />
</head>
<!--
oiWzPage:the flag which page load
	0: index.asp, wizard1, default case
	1: ppzmobicare.asp, open new window, use mac to dial
	2: rede_fon.asp, if conditions satisfy,show fon webpage directly else set flag to 5
	5: after detect wan status, whether go to fon webpage
	3: falha_conexao_velox_video.asp, adsl down
	4: falha_autenticacao.asp, adsl up, no wan ip
-->
<frameset framespacing="0" frameborder="No",border="0" noresize>
	<%if tcWebApi_get("WebCustom_Entry","oiWzPage","h") = "1" then%>
	<frame src="/cgi-bin/WizardOi/ppzmobicare.asp" name="blank" frameborder="no" noresize marginwidth="0" marginheight="0">
	<%tcWebApi_constSet("WebCustom_Entry","oiWzPage","0")%>	
	<%else%>	
	<%if tcWebApi_get("GUI_Entry0","wzOISetted","h") <> "1" then %>
	<frame src="/cgi-bin/WizardOi/index.asp" name="blank" frameborder="no" noresize marginwidth="0" marginheight="0">
	<%tcWebApi_constSet("WebCustom_Entry","oiWzPage","0")%>
	<%else%>
	<%if tcWebApi_get("WebCustom_Entry","oiWzPage","h") = "2" then%>
		<%if tcWebApi_get("Info_Adsl","lineState","h") <> "up" then%>
		<frame src="/cgi-bin/WizardOi/falha_conexao_velox_video.asp" name="blank" frameborder="no" noresize marginwidth="0" marginheight="0">
		<%tcWebApi_constSet("WebCustom_Entry","oiWzPage","5")%>
		<%elseif tcWebApi_Get("DeviceInfo_PVC0","WanIP","h") = "N/A" then%>
		<frame src="/cgi-bin/WizardOi/falha_autenticacao.asp" name="blank" frameborder="no" noresize marginwidth="0" marginheight="0">	
		<%tcWebApi_constSet("WebCustom_Entry","oiWzPage","5")%>
		<%else%>
		<frame src="/cgi-bin/WizardOi/rede_fon.asp" name="blank" frameborder="no" noresize marginwidth="0" marginheight="0">
		<%tcWebApi_constSet("WebCustom_Entry","oiWzPage","0")%>
		<%end if%>		
	<%elseif tcWebApi_get("WebCustom_Entry","oiWzPage","h") = "3" then%>
	<frame src="/cgi-bin/WizardOi/falha_conexao_velox_video.asp" name="blank" frameborder="no" noresize marginwidth="0" marginheight="0">
	<%tcWebApi_constSet("WebCustom_Entry","oiWzPage","0")%>
	<%elseif tcWebApi_get("WebCustom_Entry","oiWzPage","h") = "4" then%>	
	<frame src="/cgi-bin/WizardOi/falha_autenticacao.asp" name="blank" frameborder="no" noresize marginwidth="0" marginheight="0">	
	<%tcWebApi_constSet("WebCustom_Entry","oiWzPage","0")%>
	<%else%>
	<frame src="/cgi-bin/WizardOi/index.asp" name="blank" frameborder="no" noresize marginwidth="0" marginheight="0">
	<%tcWebApi_constSet("WebCustom_Entry","oiWzPage","0")%>
	<%end if%>
	<%end if%>
	<%end if%>
</frameset>
</html>
