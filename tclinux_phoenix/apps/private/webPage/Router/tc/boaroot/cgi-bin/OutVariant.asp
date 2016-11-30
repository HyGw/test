<% If tcWebApi_get("WebCustom_Entry","isCZOTESupported","h") <> "Yes" Then%>
var pppPwd="";
pppPwd="<%if tcWebApi_get("Wan_PVC","PASSWORD","h" ) <> "N/A" then tcWebApi_get("Wan_PVC","PASSWORD","s") end if%>";
var ddnsPwd="";
ddnsPwd="<% If TCWebApi_get("Ddns_Entry","PASSWORD","h") <> "N/A" then TCWebApi_get("Ddns_Entry","PASSWORD","s") end if%>";
var sambaPwd="";
sambaPwd="<% If TCWebApi_get("Samba_Entry","Passwd","h") <> "N/A" then TCWebApi_get("Samba_Entry","Passwd","s") end if%>";
<%if tcwebApi_get("WebCustom_Entry","isThreeLevelChangePassWord","h") = "Yes" then%>
<%if TCWebAPI_get("WebCurSet_Entry", "CurrentAccess", "h") = "0" then %>
var adminPwd="";
adminPwd="<% tcWebApi_get("Account_Entry0","web_passwd","s") %>";
<%end if%>
<%if TCWebAPI_get("WebCurSet_Entry", "CurrentAccess", "h") <> "2" then%>
var userPwd="";
userPwd="<% tcWebApi_get("Account_Entry1","web_passwd","s") %>";
<%end if%>
var user2Pwd="";
user2Pwd="<% tcWebApi_get("Account_Entry2","web_passwd","s") %>";
<%end if%>
<% If tcWebApi_get("WebCustom_Entry","isCZGeneralSupported","h") = "Yes" Then%>
var acsPwd="";
acsPwd="<%if tcWebApi_get("Cwmp_Entry", "acsPassword", "h") <> "N/A" then tcWebApi_get("Cwmp_Entry", "acsPassword", "s") end if%>";
var conReqPwd="";
conReqPwd="<%if tcWebApi_get("Cwmp_Entry", "conReqPassword", "h") <> "N/A" then tcWebApi_get("Cwmp_Entry", "conReqPassword" ,"s") end if%>";
<%end if%>
<%else%>
var pppPwd="******";
var ddnsPwd="******";
//var adminPwd="******";
//var userPwd="******";
//var user2Pwd="******";
var sambaPwd="******";
var acsPwd="******";
var conReqPwd="******";
<%end if%>