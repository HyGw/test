<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=gb2312">
</head>
<body style="background-color:transparent">
<script language="JavaScript" type="text/JavaScript">
<% if tcWebApi_get("WebCustom_Entry", "isCTGDSupported", "h") = "Yes" then%>
var rPhyStatus = "<%tcWebApi_get("XPON_Common","losStatus","s") %>";
<%else%>
var rPhyStatus = "<%tcWebApi_get("XPON_Common","losStatus","s") %>";
<%end if%>
<% if tcWebApi_get("WebCustom_Entry", "isCTGDSupported", "h") = "Yes" then%>
var rLinkState = "<%tcWebApi_get("XPON_Common","webtrafficStatus","s") %>";
<%else%>
var rLinkState = "<%tcWebApi_get("XPON_Common","trafficStatus","s") %>";
<%end if%>

var rstatus = "<%tcWebApi_get("deviceAccount_Entry","registerStatus","s") %>";
var rresult = "<%tcWebApi_get("deviceAccount_Entry","registerResult","s") %>";	
var rnewStatus = "<%tcWebApi_get("deviceAccount_Entry","newStatus","s") %>";
var rnewResult = "<%tcWebApi_get("deviceAccount_Entry","newResult","s") %>";

var acsresult = "<%TCWebApi_get("Cwmp_Entry","Inform_Status","s")%>";

<% if tcWebApi_get("WebCustom_Entry", "isCTFJSupported", "h") = "Yes"  then %>
var serviceName = "<%tcWebApi_get("deviceAccount_Entry","ProvisioningCode","s") %>";
<%else%>
var serviceName = "<%tcWebApi_get("deviceAccount_Entry","serviceName","s") %>";
<% end if%>
var serviceNum = parseInt("<%tcWebApi_get("deviceAccount_Entry","serviceNum","s") %>");	

var pppup = "<% tcWebApi_get("CwmpRoute_Entry","ifName0","s") %>"; 

<% if tcWebApi_get("WebCustom_Entry", "isCTGDSupported", "h") = "Yes" then%>
var provisioningCode = "<%tcWebApi_get("deviceAccount_Entry","ProvisioningCode","s") %>";
var pingResult = "<%TCWebApi_get("Diagnostic_PVC","PingOther","s")%>";

var linkMode = "<%TCWebApi_get("XPON_LinkCfg","LinkSta","s")%>";

var authStatus = "";

if(linkMode == "2")         // epon
{
    authStatus = "<%TCWebApi_get("EPON_LOIDAuth","AuthStatus","s")%>";
}
else                       // gpon
{
    authStatus = "<%TCWebApi_get("GPON_LOIDAuth","AuthStatus","s")%>";
}
<% end if%>
<% if tcWebApi_get("WebCustom_Entry", "isCTPONCAUSupported", "h") = "Yes" then%>
var authStatus = "";

var linkMode = "<%TCWebApi_get("XPON_LinkCfg","LinkSta","s")%>";

if(linkMode == "2")         // epon
{
    authStatus = "<%TCWebApi_get("EPON_LOIDAuth","AuthStatus","s")%>";
}
else                       // gpon
{
    authStatus = "<%TCWebApi_get("GPON_LOIDAuth","AuthStatus","s")%>";
}
<% end if%>
<% if tcWebApi_get("WebCustom_Entry", "isCTPONBIND2WebSupported", "h") = "Yes"  then %>
var isVoipSet = "<%tcWebApi_get("WebCurSet_Entry", "isVoipSet","s") %>";
<% end if%>
</script>
</body>
</html>
