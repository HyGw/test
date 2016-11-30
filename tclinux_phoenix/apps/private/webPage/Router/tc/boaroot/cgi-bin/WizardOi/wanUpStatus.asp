<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<meta http-equiv="Pragma" content="no-cache">
<meta http-equiv="Expires" content="-1">
</head>
<body style="background-color:transparent">
<script language="JavaScript" type="text/JavaScript">
var wanip = "<% tcWebApi_Get("DeviceInfo_PVC0","WanIP","s") %>";
<%if request_form("jump") = "true" Then%>
	if(wanip!="N/A")
		setTimeout('window.top.open( "http://primeiroacesso.oi.com.br", "_self");',1000);
<%end if%>
</script>
</body>
</html>
