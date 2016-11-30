<%
if Request_Form("Port_Flag") = "5" then
	TCWebApi_set("IpMacFilter_Common","IpMacType","IpMacType_Flag")
	TCWebApi_set("IpMacFilter_Common","Action","Actionflag")
	TCWebApi_set("IpMacFilter_Common","ActivePortOut","EnPortFilterOut_Flag")
	TCWebApi_set("IpMacFilter_Common","ListTypeIpUp","PortListType_Up_Flag")
	TCWebApi_commit("IpMacFilter_Entry")
	TCWebApi_save()
elseif Request_Form("Port_Flag") = "6" then
	TCWebApi_set("IpMacFilter_Common","IpMacType","IpMacType_Flag")
	TCWebApi_set("IpMacFilter_Common","Action","Actionflag")
	TCWebApi_set("IpMacFilter_Common","ActivePortIn","EnPortFilterIn_Flag")
	TCWebApi_set("IpMacFilter_Common","ListTypeIpDown","PortListType_Down_Flag")
	TCWebApi_commit("IpMacFilter_Entry")
	TCWebApi_save()
	
elseif Request_Form("Port_Flag") = "2" then
	if Request_Form("delnum") <> "" then
		TCWebApi_set("IpMacFilter_Common","Action","Actionflag")
		TCWebApi_set("IpMacFilter_Common","IpMacType","IpMacType_Flag")
		TCWebApi_set("IpMacFilter_Common","DeleteIndex","delnum")
	end if
	TCWebApi_commit("IpMacFilter_Entry")
	TCWebApi_save()
elseif Request_Form("Port_Flag") = "3" then
	TCWebApi_set("IpMacFilter_Common","Action","Actionflag")
	TCWebApi_set("IpMacFilter_Common","IpMacType","IpMacType_Flag")
	TCWebApi_set("IpMacFilter_Common","ListTypeIpUp","PortListType_Up_Flag")
	TCWebApi_commit("IpMacFilter_Entry")
	TCWebApi_save()
elseif Request_Form("Port_Flag") = "4" then
	TCWebApi_set("IpMacFilter_Common","Action","Actionflag")
	TCWebApi_set("IpMacFilter_Common","IpMacType","IpMacType_Flag")
	TCWebApi_set("IpMacFilter_Common","ListTypeIpDown","PortListType_Down_Flag")
	TCWebApi_commit("IpMacFilter_Entry")
	TCWebApi_save()		
end if
%>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0 Transitional//EN">
<HTML><HEAD><TITLE>端口过滤</TITLE>
<META http-equiv=Content-Language content=zh-cn>
<META http-equiv=Content-Type content="text/html; charset=gb2312"><LINK 
href="/JS/stylemain.css" type=text/css rel=stylesheet>
<SCRIPT language=javascript src="/JS/menu.js"></SCRIPT>

<SCRIPT language=javascript src="/JS/util.js"></SCRIPT>

<META content="MSHTML 6.00.6000.16809" name=GENERATOR></HEAD>
<BODY style="TEXT-ALIGN: center" vLink=#000000 aLink=#000000 link=#000000 
leftMargin=0 topMargin=0 
onload="DisplayLocation(getElement('Selected_Menu').value);FinishLoad();if(getElById('ConfigForm') != null)LoadFrame()" 
onunload=DoUnload() marginheight="0" marginwidth="0">
<TABLE height="100%" cellSpacing=0 cellPadding=0 width=808 align=center 
border=0>
  <TBODY>
  <TR>
    <TD height=1>
      <TABLE height=117 cellSpacing=0 cellPadding=0 width=808 
      background=/img/framelogo.jpg border=0>
        <TBODY>
        <TR>
          <TD>&nbsp;</TD>
          <TD vAlign=bottom align=right width=358>
            <TABLE id=table8 cellSpacing=0 cellPadding=0 border=0>
              <TBODY>
              <TR>
                <TD vAlign=bottom align=right><SPAN class=curUserName>&nbsp; 
                  </SPAN></TD>
                <TD class=welcom vAlign=bottom align=middle width=120>欢迎您！ </TD>
                <TD vAlign=bottom width=50><A onclick=DoLogout() 
                  href="/cgi-bin/logout.cgi" target=_top><SPAN 
                  class=logout>退出 
      </SPAN></A></TD></TR></TBODY></TABLE></TD></TR></TBODY></TABLE>
      <TABLE id=table2 height=100 cellSpacing=0 cellPadding=0 width=808 
border=0>
        <TBODY>
        <TR>
          <TD class=LocationDisplay id=LocationDisplay align=middle width=163 
          bgColor=#ef8218 rowSpan=3></TD>
          <TD width=434 bgColor=#427594 height=33>
            <P align=right><FONT face=黑体 color=#ffffff><B><FONT face=黑体 
            color=#ffffff size=6><INPUT id=Selected_Menu type=hidden 
            value="安全->端口过滤" name=Selected_Menu> </FONT></B><SPAN 
            class=GatewayName>网关名称:
            <SCRIPT 
            language=javascript>
document.write(top.gateWayName);
</SCRIPT>
             </SPAN></FONT></P></TD>
          <TD width=211 bgColor=#ef8218 height=33>
            <P class=GatewayType align=center>型号:
            <SCRIPT language=javascript>
document.write(top.ModelName);
</SCRIPT>
             </P></TD></TR>
        <TR>
          <TD id=MenuArea_L1 vAlign=bottom bgColor=#ef8218 colSpan=2 
          height=43>&nbsp;</TD></TR>
        <TR>
          <TD id=MenuArea_L2 bgColor=#427594 colSpan=2 
      height=24></TD></TR></TBODY></TABLE>
      <SCRIPT 
      language=javascript>
MakeMenu(getElById ('Selected_Menu').value);
</SCRIPT>

      <TABLE id=table3 height=15 cellSpacing=0 cellPadding=0 width=808 
        border=0><TBODY>
        <TR>
          <TD height=15><IMG height=15 src="/img/panel1.gif" 
            width=164 border=0> </TD>
          <TD><IMG height=15 src="/img/panel2.gif" width=645 
            border=0> </TD></TR></TBODY></TABLE></TD></TR>
  <TR>
    <TD vAlign=top>
      <SCRIPT language=JavaScript type=text/javascript>
var portFilter = [
["InternetGatewayDevice.X_ATP_Security","<% tcWebApi_get("IpMacFilter_Common","ActivePortOut","s")%>","<% tcWebApi_get("IpMacFilter_Common","ListTypeIpUp","s")%>","<% tcWebApi_get("IpMacFilter_Common","ListTypeIpDown","s")%>","<% tcWebApi_get("IpMacFilter_Common","ActivePortIn","s")%>"],
null
]
var enblFilterOut = portFilter[0][1];
var ModeUp = portFilter[0][2];
var ModeDown = portFilter[0][3];
var enblFilterIn = portFilter[0][4];

function stWanInfo(domain,Name)
{
this.domain = domain;
this.Name = Name;
}
var WanIP  = new Array(null);
var WanPPP = new Array(new stWanInfo("InternetGatewayDevice.WANDevice.1.WANConnectionDevice.4.WANPPPConnection.2","1_TR069_INTERNET_R_0_35"),null);
var WanInfo = new Array();
var i;
for (i = 0; i < WanIP.length-1; i++)
{
WanInfo[i] = WanIP[i];
}
for (j = 0; j < WanPPP.length-1; i++,j++)
{
WanInfo[i] =  WanPPP[j];
}
function ShowSrcAddr(val,arrMsgInfo)
{
return val + '-' + arrMsgInfo[i][10];
}
function ShowSrcPort(val,arrMsgInfo)
{
return val + '-' + arrMsgInfo[i][11];
}
function ShowDestAddr(val,arrMsgInfo)
{
return val + '-' + arrMsgInfo[i][12];
}
function ShowDestPort(val,arrMsgInfo)
{
return val + '-' + arrMsgInfo[i++][13];
}
function ShowIface(val,arrMsgInfo)

{
for (k = 0; k < WanInfo.length; k++)
{
if (WanInfo[k].domain == arrMsgInfo[i-1][14])
{
return WanInfo[k].Name;
}
}
}
function ShowEnable(val,arrMsgInfo)
{
if (val == 1)
{
return '启用';
}
else
{
return '禁用';
}
}
var FilterOut = new Array(
new Array("html/security/portfilter.asp", "FilterOut", "", "18%", "10%", "14%", "12%", "12%", "12%", "12%","10%",""),
new Array("html/security/addportfilter.asp","", "InternetGatewayDevice.X_ATP_Security.IpFilterOut", "过滤名称", "协议", "源地址", "源端口", "目的地址", "目的端口","使能"),
new Array("", "", "", "", "", "ShowSrcAddr", "ShowSrcPort", "ShowDestAddr", "ShowDestPort","ShowEnable"),

new Array()
);
var FilterIn = new Array(
new Array("html/security/portfilter.asp", "FilterIn", "", "10%", "10%", "14%", "12%", "12%", "12%", "10%", "8%","12%",""),
new Array("html/security/addportfilter.asp","", "InternetGatewayDevice.X_ATP_Security.IpFilterIn", "名称", "协议", "源地址", "源端口", "目的地址", "目的端口","使能","接口"),
new Array("", "", "", "", "", "ShowSrcAddr", "ShowSrcPort", "ShowDestAddr", "ShowDestPort","ShowEnable","ShowIface"),

new Array()
);
var stateUp,stateDown;
function LoadFrame()
{
	document.ConfigForm.delnum.value = "";
	document.ConfigForm.Port_Flag.value = "0";
	var FilterMode = getElById("FilterMode");
	var FilterModeDown = getElById("FilterModeDown");
	var FilterInfo = getElById("FilterOutInfo");
	if (ModeUp == "black")
	{
		FilterMode[0].checked = true;
		stateUp = false;
	}
	else
	{
		FilterMode[1].checked = true;
		stateUp = true;
	}
	
	if (ModeDown == "black")
	{
		FilterModeDown[0].checked = true;
		stateDown = false;
	}
	else
	{
		FilterModeDown[1].checked = true;
		stateDown = true;
	}
	
	setCheck('enblFilterOut',enblFilterOut);
	setCheck('enblFilterIn',enblFilterIn);
}

function selFilter(filter,enable,divinfo)
{
	var Form = document.ConfigForm;
	var enableFilter = filter.checked;
	if(0 == enable){
		if (enableFilter == false)
		{
			if(!confirm("是否禁用上行端口过滤？"))
			{
				filter.checked = true;
				Form.Port_Flag.value = "0";
				return;
			}
			Form.Port_Flag.value = "5";
			Form.IpMacType_Flag.value = "IpUp";
			setDisplay(divinfo,enableFilter);
		}
		else
		{
			if(!confirm("是否启用上行端口过滤？"))
			{
				filter.checked = false;
				Form.Port_Flag.value = "0";
				return;
			}
			Form.Port_Flag.value = "5";
			Form.IpMacType_Flag.value = "IpUp";
			setDisplay(divinfo,enableFilter);
		}
	}
	else if(1 == enable)
	{
		if (enableFilter == false)
		{
			if(!confirm("是否禁用下行端口过滤？"))
			{
				filter.checked = true;
				Form.Port_Flag.value = "0";
				return;
			}
			Form.Port_Flag.value = "6";
			Form.IpMacType_Flag.value = "IpDown";
			setDisplay(divinfo,enableFilter);
		}
		else
		{
			if(!confirm("是否启用下行端口过滤？"))
			{
				filter.checked = false;
				Form.Port_Flag.value = "0";
				return;
			}
			Form.Port_Flag.value = "6";
			Form.IpMacType_Flag.value = "IpDown";
			setDisplay(divinfo,enableFilter);
		}
	}
	Form.Actionflag.value = "Mod";
	btnSubmit();
	
}
function ChangeMode()
{
	var FilterMode = getElById("FilterMode");
	var isFilter = getElById("enblFilterOut");
	if (FilterMode[0].checked == true && stateUp)
	{
		if (confirm("改变过滤模式将切换所有过滤规则，您是否真的要改变过滤模式为黑名单？"))
		{
			stateUp = false;
			document.ConfigForm.PortListType_Up_Flag.value="black";
			document.ConfigForm.IpMacType_Flag.value = "IpUp";
			document.ConfigForm.Actionflag.value = "Mod";
			document.ConfigForm.Port_Flag.value = "3";
			document.ConfigForm.submit();
		}
		else
		{
			FilterMode[0].checked = false;
			FilterMode[1].checked = true;
			return;
		}
	}
	else if (FilterMode[1].checked == true && !stateUp)
	{
		if (confirm("改变过滤模式将切换所有过滤规则，您是否真的要改变过滤模式为白名单？"))
		{
			stateUp = true;
			document.ConfigForm.PortListType_Up_Flag.value="white";
			document.ConfigForm.IpMacType_Flag.value = "IpUp";
			document.ConfigForm.Actionflag.value = "Mod";
			document.ConfigForm.Port_Flag.value = "3";
			document.ConfigForm.submit();
		}
		else
		{
			FilterMode[0].checked = true;
			FilterMode[1].checked = false;
			return;
		}
	}
}

function ChangeModeDown()
{
	var FilterMode = getElById("FilterModeDown");
	var isFilter = getElById("enblFilterIn");
	if (FilterMode[0].checked == true && stateDown)
	{
		if (confirm("改变过滤模式将切换所有过滤规则，您是否真的要改变过滤模式为黑名单？"))
		{
			stateDown = false;
			document.ConfigForm.PortListType_Down_Flag.value="black";
			document.ConfigForm.IpMacType_Flag.value = "IpDown";
			document.ConfigForm.Actionflag.value = "Mod";
			document.ConfigForm.Port_Flag.value = "4";
			document.ConfigForm.submit();
		}
		else
		{
			FilterMode[0].checked = false;
			FilterMode[1].checked = true;
			return;
		}
	}
	else if (FilterMode[1].checked == true && !stateDown)
	{
		if (confirm("改变过滤模式将切换所有过滤规则，您是否真的要改变过滤模式为白名单？"))
		{
			stateDown = true;
			document.ConfigForm.PortListType_Down_Flag.value="white";
			document.ConfigForm.IpMacType_Flag.value = "IpDown";
			document.ConfigForm.Actionflag.value = "Mod";
			document.ConfigForm.Port_Flag.value = "4";
			document.ConfigForm.submit();
		}
		else
		{
			FilterMode[0].checked = true;
			FilterMode[1].checked = false;
			return;
		}
	}
}
</SCRIPT>

      <TABLE height="100%" cellSpacing=0 cellPadding=0 border=0>
        <TBODY>
        <TR>
          <TD width=157 bgColor=#ef8218 height=30>
            <P class=Item_L1>端口过滤</P></TD>
          <TD width=7 bgColor=#ef8218>　</TD>
          <TD width=474>　</TD>
          <TD vAlign=top width=170 background=/img/panel4.gif 
          rowSpan=5>
            <TABLE cellSpacing=0 cellPadding=20 width="100%" border=0>
              <TBODY>
              <TR>
                <TD><A 
                  href="/cgi-bin/help_content.asp#端口过滤" 
                  target=_blank><IMG height=34 src="/img/help.gif" 
                  width=40 border=0></A></TD></TR></TBODY></TABLE>　 </TD></TR>
        <TR>
          <TD vAlign=top width=157 bgColor=#e7e7e7 height=10></TD>
          <TD width=7 background=/img/panel3.gif>　</TD>
          <TD></TD></TR>
        <TR>
          <TD vAlign=top width=157 bgColor=#e7e7e7 height=30>
            <P class=Item_L2></P></TD>
          <TD width=7 background=/img/panel3.gif>　</TD>
          <TD>
            <FORM name="ConfigForm" action="/cgi-bin/sec-portfilter.asp" method="post">
            <TABLE cellSpacing=0 cellPadding=0 width="100%" border=0>
              <TBODY>
              <TR>
                <TD width=10>&nbsp;</TD>
                <TD>
                  <DIV>配置上行的端口过滤规则</DIV>
                          <input type="hidden" name="Port_Flag" value="0">
						  <input type="hidden" name="PortListType_Up_Flag" value="<% tcWebApi_get("IpMacFilter_Common","ListTypeIpUp","s") %>">
						  <input type="hidden" name="PortListType_Down_Flag" value="<% tcWebApi_get("IpMacFilter_Common","ListTypeIpDown","s") %>">
						  <input type="hidden" name="EnPortFilterOut_Flag" value="<% tcWebApi_get("IpMacFilter_Common","ActivePortOut","s") %>">
						  <input type="hidden" name="EnPortFilterIn_Flag" value="<% tcWebApi_get("IpMacFilter_Common","ActivePortIn","s") %>">
                          <input type="hidden" name="delnum" value="">
                          <input type="hidden" name="Actionflag" value="Mod">
						  <input type="hidden" name="IpMacType_Flag" value="IpUp">
                          <input type="hidden" name="ipup_num" value="<% tcWebApi_get("IpMacFilter","ipup_num","s") %>">
                          <input type="hidden" name="ipdown_num" value="<% tcWebApi_get("IpMacFilter","ipdown_num","s") %>"> 
                          <SCRIPT language=JavaScript src="/JS/printbox.js"></SCRIPT>
						  <script language="JavaScript" type="text/JavaScript">
function addClick(iType)
{
	var Form = document.ConfigForm;
	var FilterType;
	if(iType == 0)
	{
		FilterType = "40";
		if(parseInt(Form.ipup_num.value) >= 40)
		{
			alert("上行端口过滤规则数达到上限！");
			return false;
		}
		
	}
	else
	{
		FilterType = "80";
		if(parseInt(Form.ipdown_num.value) >= 40)
		{
			alert("下行端口过滤规则数达到上限！");
			return false;
		}
	}
	window.location = '/cgi-bin/sec-addportfilter.asp?PortFilterType='+FilterType;
}
function removeClick(iType)
{
	document.ConfigForm.Port_Flag.value = "2";
	if(0 == iType)
	{
		document.ConfigForm.IpMacType_Flag.value = "IpUp";
	}
	else
	{
		document.ConfigForm.IpMacType_Flag.value = "IpDown";
	}
	document.ConfigForm.Actionflag.value = "Del";
	document.ConfigForm.submit();
}
function	btnSubmit()
{
	var Form = document.ConfigForm;
	if(Form.enblFilterOut.checked)
	{
		Form.EnPortFilterOut_Flag.value = "1";
		if (Form.FilterMode[0].checked)
			Form.PortListType_Up_Flag.value = "black";
		else if (Form.FilterMode[1].checked)
			Form.PortListType_Up_Flag.value = "white";
	}
	else
	{
		Form.EnPortFilterOut_Flag.value = "0";
	}
	if(Form.enblFilterIn.checked)
	{
		Form.EnPortFilterIn_Flag.value = "1";
		if (Form.FilterModeDown[0].checked)
			Form.PortListType_Down_Flag.value = "black";
		else if (Form.FilterModeDown[1].checked)
			Form.PortListType_Down_Flag.value = "white";		
	}
	else
	{
		Form.EnPortFilterIn_Flag.value = "0";		
	}
	Form.submit();
}
</script>
                          <DIV>使能: <INPUT id=enblFilterOut onclick="selFilter(this, 0,'FilterOutInfo')" type=checkbox name="enblFilterOut" <%if tcWebApi_get("IpMacFilter_Common","ActivePortOut","h") = "1" then asp_Write("checked") end if%>></DIV><BR>
                  <DIV id=FilterOutInfo>过滤模式： 
				  <INPUT id=FilterMode onclick="ChangeMode()" type=radio name="FilterMode" <%if tcWebApi_get("IpMacFilter_Common","ListTypeIpUp","h") = "black" then asp_Write("checked") end if%>>黑名单 
				  <INPUT id=FilterMode onclick="ChangeMode()" type=radio name="FilterMode" <%if tcWebApi_get("IpMacFilter_Common","ListTypeIpUp","h") = "white" then asp_Write("checked") end if%>>白名单 <BR><BR>
                  <SCRIPT language=JavaScript type=text/javascript>
				  	setDisplay("FilterOutInfo",enblFilterOut);
				  </SCRIPT>
                            <% if tcWebApi_get("IpMacFilter_Common","ActivePortOut","h") = "1" then %>
                            <iframe src="/cgi-bin/sec_portfilteroutlist.cgi" frameborder="0" width="450" height="220" ></iframe>
                            <% end if %>
                            <BR>
                            <BR>
				  <INPUT id=btnAdd onclick="addClick(0)" type=button value=添加> 
				  <INPUT id=btnRemove onclick="removeClick(0)" type=button value=删除>
                          </DIV>                            
						  <HR>

                  <DIV>配置下行的端口过滤规则</DIV><BR>
                  <DIV>使能: <INPUT id=enblFilterIn  onclick="selFilter(this, 1,'FilterInInfo')" type=checkbox name="enblFilterIn" <%if tcWebApi_get("IpMacFilter_Common","ActivePortIn","h") = "1" then asp_Write("checked") end if%>> </DIV><BR>
                  <DIV id=FilterInInfo>过滤模式： 
				  <INPUT id=FilterMode onclick="ChangeModeDown()" type=radio name="FilterModeDown" <%if tcWebApi_get("IpMacFilter_Common","ListTypeIpDown","h") = "black" then asp_Write("checked") end if%>>黑名单 
				  <INPUT id=FilterMode onclick="ChangeModeDown()" type=radio name="FilterModeDown" <%if tcWebApi_get("IpMacFilter_Common","ListTypeIpDown","h") = "white" then asp_Write("checked") end if%>>白名单 <BR><BR>
                  <SCRIPT language=JavaScript type=text/javascript>
					  setDisplay("FilterInInfo",enblFilterIn);
				  </SCRIPT>
                            <% if tcWebApi_get("IpMacFilter_Common","ActivePortIn","h") = "1" then %>
                            <iframe src="/cgi-bin/sec_portfilterinlist.cgi" frameborder="0" width="450" height="220" ></iframe>
					<% end if %>
                            <p><BR>
                              <BR>
                              <INPUT id=btnAdd onclick="addClick(1)" type=button value=添加>
                              <INPUT id=btnRemove onclick="removeClick(1)" type=button value=删除>
                            </p>
                          </DIV>							
						  
                        </TD>
                <TD width=10>&nbsp;</TD></TR>
              <TR>
                <TD width=10>&nbsp;</TD>
                <TD> </TD>
                <TD width=10>&nbsp;</TD></TR></TBODY></TABLE></FORM></TD></TR>
        <TR>
          <TD vAlign=top width=157 bgColor=#e7e7e7 height=10></TD>
          <TD width=7 background=/img/panel3.gif>　</TD>
          <TD></TD></TR>
        <TR>
          <TD vAlign=top width=157 bgColor=#e7e7e7></TD>
          <TD width=7 background=/img/panel3.gif>　</TD>
          <TD></TD></TR></TBODY></TABLE></TD></TR>
  <TR>
    <TD height=1>
      <TABLE id=table7 height=35 cellSpacing=0 cellPadding=0 width=808 
        border=0><TBODY>
        <TR>
          <TD width=162 bgColor=#ef8218>　</TD>
          <TD width=278 bgColor=#427594>　</TD>
          <TD width=196 bgColor=#427594>
            <P align=center><IMG id=btnOK onclick="btnSubmit()" height=23 
            src="/img/ok.gif" width=80 border=0>&nbsp;&nbsp;<IMG 
            id=btnCancel onclick="RefreshPage()" height=23 src="/img/cancel.gif" 
            width=80 border=0> </P></TD>
          <TD width=170 
bgColor=#313031>　</TD></TR></TBODY></TABLE></TD></TR></TBODY></TABLE></BODY></HTML>
