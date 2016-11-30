<% tcWebApi_constSet("WebCurSet_Entry", "isLockState", "0") %>
<html>
<head>
<META http-equiv="Content-Type" content="text/html; charset=gb2312">
<title>content</title>
<script language="JavaScript" type="text/javascript">
var ssidIdx = 0;
var lanDevIndex = 1;
var QoSCurInterface = '';
var DDNSProvider = '';
var curUser = "<%tcWebApi_get("WebCurSet_Entry","CurrentAccess","s")%>";
var ripIndex = "";
var previousPage = "";
var gateWayName = "<%tcWebApi_get("DeviceInfo","GateWay","s")%>";
var ModelName = "<%tcWebApi_get("DeviceInfo_devParaStatic","ModelName","s")%>";
var pageMap =  new Array();//pageMap[8][9]
var voipType = "<%tcWebApi_get("VoIPBasic_Common", "SIPProtocol","s")%>";
for (i = 0; i < 8; i++)
{
	pageMap[i] = new Array();
}
<% tcWebApi_set("dynDisp_Entry", "CurPage", "0")%>
pageMap[0][0] = "<% tcWebApi_get("dynDisp_Entry","MainMaskBit0","s") %>";
pageMap[1][0] = "<% tcWebApi_get("dynDisp_Entry","MainMaskBit1","s") %>";
pageMap[2][0] = "<% tcWebApi_get("dynDisp_Entry","MainMaskBit2","s") %>";
pageMap[3][0] = "<% tcWebApi_get("dynDisp_Entry","MainMaskBit3","s") %>";
<% if tcWebApi_get("WebCustom_Entry", "isCUCPONSFUSupported","h") <> "Yes" then%>
pageMap[4][0] = "<% tcWebApi_get("dynDisp_Entry","MainMaskBit4","s") %>";
<% else %>
pageMap[4][0] = "0";
<% end if %>
pageMap[5][0] = "<% tcWebApi_get("dynDisp_Entry","MainMaskBit5","s") %>";
pageMap[6][0] = "<% tcWebApi_get("dynDisp_Entry","MainMaskBit6","s") %>";
pageMap[7][0] = "<% tcWebApi_get("dynDisp_Entry","MainMaskBit7","s") %>";
pageMap[0][1] = "<% tcWebApi_get("dynDisp_Entry","CurMaskBit0","s") %>";
pageMap[0][2] = "<% tcWebApi_get("dynDisp_Entry","CurMaskBit1","s") %>";
pageMap[0][3] = "<% tcWebApi_get("dynDisp_Entry","CurMaskBit2","s") %>";
pageMap[0][4] = "<% tcWebApi_get("dynDisp_Entry","CurMaskBit3","s") %>";
pageMap[0][5] = "<% tcWebApi_get("dynDisp_Entry","CurMaskBit4","s") %>";
pageMap[0][6] = "<% tcWebApi_get("dynDisp_Entry","CurMaskBit5","s") %>";
pageMap[0][7] = "<% tcWebApi_get("dynDisp_Entry","CurMaskBit6","s") %>";
pageMap[0][8] = "<% tcWebApi_get("dynDisp_Entry","CurMaskBit7","s") %>";
<% tcWebApi_set("dynDisp_Entry", "CurPage", "1")%>
pageMap[1][1] = "<% tcWebApi_get("dynDisp_Entry","CurMaskBit0","s") %>";
pageMap[1][2] = "<% tcWebApi_get("dynDisp_Entry","CurMaskBit1","s") %>";
pageMap[1][3] = "<% tcWebApi_get("dynDisp_Entry","CurMaskBit2","s") %>";
<% if tcWebApi_get("WebCustom_Entry", "isCTPONNOVOIP","h") <> "Yes" then%>
pageMap[1][4] = "<% tcWebApi_get("dynDisp_Entry","CurMaskBit3","s") %>";
<% else %>
pageMap[1][4] = "0";
<% end if %>
pageMap[1][5] = "<% tcWebApi_get("dynDisp_Entry","CurMaskBit4","s") %>";
pageMap[1][6] = "<% tcWebApi_get("dynDisp_Entry","CurMaskBit5","s") %>";
pageMap[1][7] = "<% tcWebApi_get("dynDisp_Entry","CurMaskBit6","s") %>";
pageMap[1][8] = "<% tcWebApi_get("dynDisp_Entry","CurMaskBit7","s") %>";
<% tcWebApi_set("dynDisp_Entry", "CurPage", "2")%>
pageMap[2][1] = "<% tcWebApi_get("dynDisp_Entry","CurMaskBit0","s") %>";
<% if tcWebApi_get("WebCustom_Entry", "isCUCPONSFUSupported","h") <> "Yes" then%>
pageMap[2][2] = "<% tcWebApi_get("dynDisp_Entry","CurMaskBit1","s") %>";
<% else %>
pageMap[2][2] = "0";
<% end if %>	
pageMap[2][3] = "<% tcWebApi_get("dynDisp_Entry","CurMaskBit2","s") %>";
<% if tcWebApi_get("WebCustom_Entry", "isWLanSupported","h") = "Yes" then%>
pageMap[2][4] = "<% tcWebApi_get("dynDisp_Entry","CurMaskBit3","s") %>";
<% else %>
pageMap[2][4] = 0;
<% end if %>
pageMap[2][5] = "<% tcWebApi_get("dynDisp_Entry","CurMaskBit4","s") %>";
<% if tcWebApi_get("WebCustom_Entry", "isCUCPONSFUSupported","h") <> "Yes" then%>
pageMap[2][6] = "<% tcWebApi_get("dynDisp_Entry","CurMaskBit5","s") %>";
<% else %>
pageMap[2][6] = "0";
<% end if %>
pageMap[2][7] = "<% tcWebApi_get("dynDisp_Entry","CurMaskBit6","s") %>";
<% if tcWebApi_get("WebCustom_Entry", "isCUCPONSFUSupported","h") <> "Yes" then%>
pageMap[2][8] = "<% tcWebApi_get("dynDisp_Entry","CurMaskBit7","s") %>";
<% else %>
pageMap[2][8] = "0";	
<% end if %>
<% tcWebApi_set("dynDisp_Entry", "CurPage", "3")%>
pageMap[3][1] = "<% tcWebApi_get("dynDisp_Entry","CurMaskBit0","s") %>";
pageMap[3][2] = "<% tcWebApi_get("dynDisp_Entry","CurMaskBit1","s") %>";
pageMap[3][3] = "<% tcWebApi_get("dynDisp_Entry","CurMaskBit2","s") %>";
pageMap[3][4] = "<% tcWebApi_get("dynDisp_Entry","CurMaskBit3","s") %>";
pageMap[3][5] = "<% tcWebApi_get("dynDisp_Entry","CurMaskBit4","s") %>";
pageMap[3][6] = "<% tcWebApi_get("dynDisp_Entry","CurMaskBit5","s") %>";
pageMap[3][7] = "<% tcWebApi_get("dynDisp_Entry","CurMaskBit6","s") %>";
pageMap[3][8] = "<% tcWebApi_get("dynDisp_Entry","CurMaskBit7","s") %>";
<% if tcWebApi_get("WebCustom_Entry", "isCUCPONSFUSupported","h") <> "Yes" then%>
<% tcWebApi_set("dynDisp_Entry", "CurPage", "4")%>
pageMap[4][1] = "<% tcWebApi_get("dynDisp_Entry","CurMaskBit0","s") %>";
pageMap[4][2] = "<% tcWebApi_get("dynDisp_Entry","CurMaskBit1","s") %>";
pageMap[4][3] = "<% tcWebApi_get("dynDisp_Entry","CurMaskBit2","s") %>";
pageMap[4][4] = "<% tcWebApi_get("dynDisp_Entry","CurMaskBit3","s") %>";
pageMap[4][5] = "<% tcWebApi_get("dynDisp_Entry","CurMaskBit4","s") %>";
pageMap[4][6] = "<% tcWebApi_get("dynDisp_Entry","CurMaskBit5","s") %>";
pageMap[4][7] = "<% tcWebApi_get("dynDisp_Entry","CurMaskBit6","s") %>";
pageMap[4][8] = "<% tcWebApi_get("dynDisp_Entry","CurMaskBit7","s") %>";
<% end if %>
<% if tcWebApi_get("WebCustom_Entry", "isCUCPONSFUSupported","h") <> "Yes" then%>
<% tcWebApi_set("dynDisp_Entry", "CurPage", "5")%>
<% else %>
<% tcWebApi_set("dynDisp_Entry", "CurPage", "4")%>
<% end if %>	
pageMap[5][1] = "<% tcWebApi_get("dynDisp_Entry","CurMaskBit0","s") %>";
pageMap[5][2] = "<% tcWebApi_get("dynDisp_Entry","CurMaskBit1","s") %>";
pageMap[5][3] = "<% tcWebApi_get("dynDisp_Entry","CurMaskBit2","s") %>";
pageMap[5][4] = "<% tcWebApi_get("dynDisp_Entry","CurMaskBit3","s") %>";
pageMap[5][5] = "<% tcWebApi_get("dynDisp_Entry","CurMaskBit4","s") %>";
pageMap[5][6] = "<% tcWebApi_get("dynDisp_Entry","CurMaskBit5","s") %>";
pageMap[5][7] = "<% tcWebApi_get("dynDisp_Entry","CurMaskBit6","s") %>";
pageMap[5][8] = "<% tcWebApi_get("dynDisp_Entry","CurMaskBit7","s") %>";
<% if tcWebApi_get("WebCustom_Entry", "isCUCPONSFUSupported","h") <> "Yes" then%>
<% tcWebApi_set("dynDisp_Entry", "CurPage", "6")%>
<% else %>
<% tcWebApi_set("dynDisp_Entry", "CurPage", "5")%>
<% end if %>	
pageMap[6][1] = "<% tcWebApi_get("dynDisp_Entry","CurMaskBit0","s") %>";
pageMap[6][2] = "<% tcWebApi_get("dynDisp_Entry","CurMaskBit1","s") %>";
pageMap[6][3] = "<% tcWebApi_get("dynDisp_Entry","CurMaskBit2","s") %>";
pageMap[6][4] = "<% tcWebApi_get("dynDisp_Entry","CurMaskBit3","s") %>";
pageMap[6][5] = "<% tcWebApi_get("dynDisp_Entry","CurMaskBit4","s") %>";
pageMap[6][6] = "<% tcWebApi_get("dynDisp_Entry","CurMaskBit5","s") %>";
pageMap[6][7] = "<% tcWebApi_get("dynDisp_Entry","CurMaskBit6","s") %>";
pageMap[6][8] = "<% tcWebApi_get("dynDisp_Entry","CurMaskBit7","s") %>";
<% if tcWebApi_get("WebCustom_Entry", "isCUCPONSFUSupported","h") <> "Yes" then%>
<% tcWebApi_set("dynDisp_Entry", "CurPage", "7")%>
<% else %>
<% tcWebApi_set("dynDisp_Entry", "CurPage", "6")%>
<% end if %>	
pageMap[7][1] = "<% tcWebApi_get("dynDisp_Entry","CurMaskBit0","s") %>";
pageMap[7][2] = "<% tcWebApi_get("dynDisp_Entry","CurMaskBit1","s") %>";
pageMap[7][3] = "<% tcWebApi_get("dynDisp_Entry","CurMaskBit2","s") %>";
pageMap[7][4] = "<% tcWebApi_get("dynDisp_Entry","CurMaskBit3","s") %>";
pageMap[7][5] = "<% tcWebApi_get("dynDisp_Entry","CurMaskBit4","s") %>";
pageMap[7][6] = "<% tcWebApi_get("dynDisp_Entry","CurMaskBit5","s") %>";
pageMap[7][7] = "<% tcWebApi_get("dynDisp_Entry","CurMaskBit6","s") %>";
pageMap[7][8] = "<% tcWebApi_get("dynDisp_Entry","CurMaskBit7","s") %>";
</script>
<frameset rows='0,*' frameborder='0' border='0'>
<frame src='/cgi-bin/refresh.asp' name='refreshfrm' frameborder='no' border='0' scrolling='no' target='_self' marginwidth='0' marginheight='0' noresize>
<frame src='/cgi-bin/sta-device.asp' name='contentfrm' frameborder='no' border='0' target='_self' marginwidth='0' marginheight='0' >
</frameset>
<noframes>The information on the page makes use of frame technology. For best results, use the new version of Browser.</noframes>
</head>
</body>
</html>
