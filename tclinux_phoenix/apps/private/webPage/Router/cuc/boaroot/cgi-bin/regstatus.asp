<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=gb2312">
</head>
<body style="background-color:transparent">
<script language="JavaScript" type="text/JavaScript">
var rPhyStatus = "<%tcWebApi_get("XPON_Common","phyStatus","s") %>";
var rLinkState = "<%tcWebApi_get("XPON_Common","trafficStatus","s") %>";

var rstatus = "<%tcWebApi_get("deviceAccount_Entry","registerStatus","s") %>";
var rresult = "<%tcWebApi_get("deviceAccount_Entry","registerResult","s") %>";	
var rnewStatus = "<%tcWebApi_get("deviceAccount_Entry","newStatus","s") %>";
var rnewResult = "<%tcWebApi_get("deviceAccount_Entry","newResult","s") %>";

var acsresult = "<%TCWebApi_get("Cwmp_Entry","Inform_Status","s")%>";

var serviceName = "<%tcWebApi_get("deviceAccount_Entry","serviceName","s") %>";
var serviceNum = parseInt("<%tcWebApi_get("deviceAccount_Entry","serviceNum","s") %>");	

var pppup = "<% tcWebApi_get("CwmpRoute_Entry","ifName0","s") %>"; 
</script>
</body>
</html>
