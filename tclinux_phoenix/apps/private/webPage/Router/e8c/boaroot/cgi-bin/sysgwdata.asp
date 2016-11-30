<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0 Transitional//EN">
<head>
<META http-equiv=Content-Language content=zh-cn>
<META http-equiv=Content-Type content="text/html; charset=gb2312">
	<link rel="stylesheet" href="/JS/stylemain.css"/>



</head>
<body style="display:block">
<input type="hidden" id="simStatus" name="simStatus" value="<% tcWebApi_get("VoIPSimulateTest_Entry0","Status","s") %>"/>
<input type="hidden" id="lineStatus" name="lineStatus" value="<% tcWebApi_get("VoIPSysParam_Entry0","SC_LINE_INFO_STATUS","s") %>"/>
<input type="hidden" id="simulateResult" name="simulateResult" value="<% tcWebApi_get("VoIPSimulateTest_Entry0","Conclusion","s") %>"/>
<input type="hidden" id="callerFaiReason" name="callerFaiReason" value="<% tcWebApi_get("VoIPSimulateTest_Entry0","CallerFailReason","s") %>"/>
<input type="hidden" id="failedResponseCode" name="failedResponseCode" value="<% tcWebApi_get("VoIPSimulateTest_Entry0","FailedResponseCode","s") %>"/>


</body>

</html>