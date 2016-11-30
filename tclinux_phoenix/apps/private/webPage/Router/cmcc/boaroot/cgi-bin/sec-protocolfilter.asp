<%
if Request_Form("formcommit") = "1" then
	if Request_Form("entryidx") = "0" then
		TCWebApi_set("IpMacFilter_EtherTypeFilter","P0","filterProtocolValue")
	elseif Request_Form("entryidx") = "1" then
		TCWebApi_set("IpMacFilter_EtherTypeFilter","P1","filterProtocolValue")
	elseif Request_Form("entryidx") = "2" then
		TCWebApi_set("IpMacFilter_EtherTypeFilter","P2","filterProtocolValue")
	elseif Request_Form("entryidx") = "3" then
		TCWebApi_set("IpMacFilter_EtherTypeFilter","P3","filterProtocolValue")
	elseif Request_Form("entryidx") = "4" then
		TCWebApi_set("IpMacFilter_EtherTypeFilter","P4","filterProtocolValue")	
	elseif Request_Form("entryidx") = "5" then
		TCWebApi_set("IpMacFilter_EtherTypeFilter","P5","filterProtocolValue")		
	elseif Request_Form("entryidx") = "6" then
		TCWebApi_set("IpMacFilter_EtherTypeFilter","P6","filterProtocolValue")		
	elseif Request_Form("entryidx") = "7" then
		TCWebApi_set("IpMacFilter_EtherTypeFilter","P7","filterProtocolValue")													
	end if
	TCWebApi_commit("IpMacFilter_EtherTypeFilter")
	tcWebApi_save()
end if
%>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0 Transitional//EN">
<HTML><HEAD><TITLE>协议过滤</TITLE>
<META http-equiv=Content-Language content=zh-cn>
<META http-equiv=Content-Type content="text/html; charset=gb2312"><LINK 
href="/JS/stylemain.css" type=text/css rel=stylesheet>
<SCRIPT language=javascript src="/JS/menu.js"></SCRIPT>
<SCRIPT language=javascript src="/JS/util.js"></SCRIPT>
<SCRIPT language=JavaScript src="/JS/printtable.js"></SCRIPT>
<SCRIPT language=JavaScript type=text/javascript>
function LoadFrame()
{
}

function ModifyInstance(index)
{
  var lanmode = LanArray[index -1].Mode;
  var proV = LanArray[index -1].FilterP;

  document.getElementById("TableUrlInfo").style.display = "block";
    
  if(proV.indexOf('IPv4oE')>=0)
  	setCheck("cbIPv4oE" ,1);
  else
  	setCheck("cbIPv4oE" ,0);
  if(proV.indexOf('PPPoE')>=0)
  	setCheck("cbPPPoE" ,1);
  else
  	setCheck("cbPPPoE" ,0);  	
  if(proV.indexOf('ARP')>=0)
  	setCheck("cbARP" ,1);
  else
  	setCheck("cbARP" ,0);
  if(proV.indexOf('IPv6oE')>=0)
  	setCheck("cbIPv6oE" ,1);
  else
  	setCheck("cbIPv6oE" ,0);
}
 
function setControl(index) 
{ 
	var identi = "";

	selectIndex = index;
	if (index < -1)
		return;

	identi = index > 4 ? "SSID"+(index-4) : "LAN"+index;
	document.getElementById("PortId").innerHTML = identi;

	return ModifyInstance(index);
}

function FilterPInfoClass(vP)
{
    this.FilterP = ( 'N/A' == vP ) ? '' : vP;
}

var LanArray = new Array(
new FilterPInfoClass('<% tcWebApi_get("IpMacFilter_EtherTypeFilter","P0","s") %>')
, new FilterPInfoClass('<% tcWebApi_get("IpMacFilter_EtherTypeFilter","P1","s") %>')
, new FilterPInfoClass('<% tcWebApi_get("IpMacFilter_EtherTypeFilter","P2","s") %>')
, new FilterPInfoClass('<% tcWebApi_get("IpMacFilter_EtherTypeFilter","P3","s") %>')
, new FilterPInfoClass('<% tcWebApi_get("IpMacFilter_EtherTypeFilter","P4","s") %>')
, new FilterPInfoClass('<% tcWebApi_get("IpMacFilter_EtherTypeFilter","P5","s") %>')
, new FilterPInfoClass('<% tcWebApi_get("IpMacFilter_EtherTypeFilter","P6","s") %>')
, new FilterPInfoClass('<% tcWebApi_get("IpMacFilter_EtherTypeFilter","P7","s") %>')
,null);

function CreateList()
{     
  var HtmlCode = "";
  var Lan_Indenti1 = "LAN";
  var Lan_Indenti2 = "SSID";
  var Lan = "";
  var LanCnt = 4;
  var LanWire = 4;

	ssidNum = '<% tcWebApi_get("WLan_Common","BssidNum","s") %>';
	if ( isPlusInteger(ssidNum) )
		LanCnt += parseInt(ssidNum);
  for (var i = 1; i <= LanCnt; i++)
  {  
      Lan = i > LanWire ? (Lan_Indenti2+(i-4)) : (Lan_Indenti1+i);

      HtmlCode += '<tr id= "record_' + i +'" align = "center" class="table1_01" onclick="selectLine(this.id); " >';
      HtmlCode += '<td >' + Lan + '</td>';      
      if(LanArray[i-1].FilterP == "")
      {
          HtmlCode += '<td></td>';
      }
      else
      {
          HtmlCode += '<td style="word-break:break-all;word-wrap:break-word;">' + LanArray[i-1].FilterP + '</td>'; 
      }
      
      HtmlCode += '</tr>'            
  }  
  
  return HtmlCode;    
}

//用户点击了取消按钮后的代码逻辑
function OnCancelButtonClick()
{
    document.getElementById("TableUrlInfo").style.display = "none";
    return false;
}

function getEntryIndex(portid)
{
	switch ( portid )
	{
		case 'LAN1':
		case 'LAN2':
		case 'LAN3':
		case 'LAN4':
			return parseInt(portid.substring(3, 4)) - 1;
		case 'SSID1':
		case 'SSID2':
		case 'SSID3':
		case 'SSID4':
			return parseInt(portid.substring(4, 5)) + 3;
		default:
			return -1;
	}
}

function OnApplyButtonClick()
{
		var entryidx = 0;
		var PortId = getElementById("PortId").innerHTML;
		var proV = '';

		entryidx = getEntryIndex(PortId);
		if ( entryidx < 0 )	{
			alert('Invalid entry!')
			return false;	
		}
		document.forms[0].entryidx.value = entryidx;
		document.forms[0].formcommit.value = 1;
		
		if (getCheckVal('cbIPv4oE') == 1)
			proV = 'IPv4oE';
		if (getCheckVal('cbPPPoE') == 1){
			if(proV == '')
				proV = 'PPPoE';
			else
				proV = proV + ',PPPoE';
		}
		if (getCheckVal('cbARP') == 1){
			if(proV == '')
				proV = 'ARP';
			else
				proV = proV + ',ARP';
		}
		if (getCheckVal('cbIPv6oE') == 1){
			if(proV == '')
				proV = 'IPv6oE';
			else
				proV = proV + ',IPv6oE';
		}
		document.forms[0].filterProtocolValue.value = proV;
		document.forms[0].submit();
		return true;
}
</SCRIPT>      
 
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
<% if tcwebApi_get("WebCustom_Entry","isCYE8SFUSupported","h") = "Yes" then %>
      <TABLE height=80 cellSpacing=0 cellPadding=0 width=808 
      background=/img/framelogo.jpg border=0>
<% else %>
      <TABLE height=117 cellSpacing=0 cellPadding=0 width=808 
      background=/img/framelogo.jpg border=0>
<% end if %> 
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
          bgColor=#d8d8d8 rowSpan=3></TD>
          <TD width=434 bgColor=#0059A0 height=33>
            <P align=right><FONT face=黑体 color=#ffffff><B><FONT face=黑体 
            color=#ffffff size=6><INPUT id=Selected_Menu type=hidden 
            value="安全->协议过滤" name=Selected_Menu> </FONT></B><SPAN 
            class=GatewayName>网关名称:
            <SCRIPT 
            language=javascript>
document.write(top.gateWayName);
</SCRIPT>
             </SPAN></FONT></P></TD>
          <TD width=211 bgColor=#d8d8d8 height=33>
            <P class=GatewayType align=center>型号:
            <SCRIPT language=javascript>
document.write(top.ModelName);
</SCRIPT>
             </P></TD></TR>
        <TR>
          <TD id=MenuArea_L1 vAlign=bottom bgColor=#d8d8d8 colSpan=2 
          height=43>&nbsp;</TD></TR>
        <TR>
          <TD id=MenuArea_L2 bgColor=#0059A0 colSpan=2 
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
      <TABLE height="100%" cellSpacing=0 cellPadding=0 border=0>
        <TBODY>
        <TR>
          <TD class=Item_L1 width=157 bgColor=#e7e7e7 height=30>
            <P>协议过滤</P></TD>
          <TD width=7 bgColor=#e7e7e7>　</TD>
          <TD width=474>　</TD>
          <TD vAlign=top width=170 background=/img/panel4.gif 
          rowSpan=4>
            <TABLE cellSpacing=0 cellPadding=20 width="100%" border=0 height='100%'>
              <TBODY>
              <TR>
                <TD valign='top'><A href="/cgi-bin/help_content.asp#协议过滤" 
                  target=_blank><IMG height=34 src="/img/help.gif" 
                  width=40 border=0></A></TD></TR>
<% if tcwebApi_get("WebCustom_Entry","isCYE8SFUSupported","h") = "Yes" then %>
              <TR>
                <TD valign='bottom'><IMG src='/img/tybottom.jpg' height=76 width=112></TD></TR>
<% end if %>
              </TBODY></TABLE></TD></TR>
        <TR>
          <TD vAlign=top width=157 bgColor=#e7e7e7 height=10></TD>
          <TD width=7 background=/img/panel3.gif>　</TD>
          <TD></TD></TR>
        <TR>
          <TD class=Item_L2 vAlign=top width=157 bgColor=#e7e7e7 height=30>
            <P></P></TD>
          <TD width=7 background=/img/panel3.gif>　</TD>
          <TD>
          	<FORM name="ConfigForm" action="/cgi-bin/sec-protocolfilter.asp" method="post">
						<!--解释头-->
						<div id="PromptPanel">					  
						</div>

					<!--TODO:在这里加入你的具体页面展现-->
					  <table class="table1_bg" border="0" cellspacing="1" style="margin-left: 5px;" width="98%">
					    <tr class="table1_title">
						       <td width="10%">端口</td>
						       <td width="75%">过滤的协议</td>
						  </tr>    
						    <script>
						   document.write(CreateList()); 
<% if tcwebApi_get("WebCustom_Entry","isCT1PORTSupported","h") = "Yes" then %>
						   setDisplay('record_2', 0);
<% end if %>
<% if tcwebApi_get("WebCustom_Entry","isCT2PORTSSupported","h") = "Yes" then %>
						   setDisplay('record_3', 0);
						   setDisplay('record_4', 0);
<% end if %>
<% if TCWebApi_get("WebCustom_Entry","isWLanSupported","h" ) <> "Yes" then %>
								var ssidNum = parseInt('<% tcWebApi_get("WLan_Common","BssidNum","s") %>');
								for (var i = 5; i <= 4+ssidNum; i++){
									var ctlID = 'record_'+i;
									setDisplay(ctlID, 0);
								}
<% end if %>
						   </script>
					 </table>

					 <!--Apply和Cancel按钮-->
					  <div id="TableUrlInfo" style="display:none">					
					  <table cellspacing="1" cellpadding="0" class="table1_bg" style="margin-left: 5px;" width="98%">					  
					    <tr class="trTabConfigure">  
					    <td align= left width=25% class="table1_left">端口: <td  class="table1_right"> <div id="PortId"></div></td> </td>
					    </tr>
					</table>

					  <div id="BindVlanRow">
					  <table cellspacing="1" cellpadding="0" class="table1_bg" style="margin-left: 5px;" width="98%">
					    <tr class="trTabConfigure"><td width="25%"  align=left class="table1_left">协议:</td><td class="table1_right">
					    	<INPUT id=cbIPv4oE type=checkbox name="cbIPv4oE">IPv4oE
					    	<INPUT id=cbPPPoE type=checkbox name="cbPPPoE">PPPoE
					    	<INPUT id=cbARP type=checkbox name="cbARP">ARP
					    	<INPUT id=cbIPv6oE type=checkbox name="cbIPv6oE">IPv6oE</td></tr>
					  </table>
					  </div>
					  <table id="ConfigPanelButtons" width="100%" cellspacing="1" class="table1_button">
					    <tr>
					        <td width="25%">
					        </td>
					        <td class="table1_submit" style="padding-left: 5px">
					        		<input type='hidden' value='0' name='entryidx'>
					        		<input type='hidden' value='0' name='formcommit'>
					        		<input type='hidden' value='' name='filterProtocolValue'>
					            <input id="ButtonApply"  type="button" value="应用" onclick="javascript:return OnApplyButtonClick();" class="submit" />
					            <input id="ButtonCancel" type="button" value="取消" onclick="javascript:OnCancelButtonClick();" class="submit" />
					        </td>
					    </tr>
					  </table>
					</div>
            </FORM></TD></TR>
        <TR>
          <TD vAlign=top width=157 bgColor=#e7e7e7></TD>
          <TD width=7 background=/img/panel3.gif>　</TD>
          <TD></TD></TR></TBODY></TABLE></TD></TR>
  <TR>
    <TD height=1>
      <TABLE id=table7 height=35 cellSpacing=0 cellPadding=0 width=808 
        border=0><TBODY>
        <TR>
          <TD width=162 bgColor=#d8d8d8>　</TD>
          <TD width=278 bgColor=#0059A0>　</TD>
          <TD width=196 bgColor=#0059A0>
            <P align=center></P></TD>
          <TD width=170 
bgColor=#313031>　</TD></TR></TBODY></TABLE></TD></TR></TBODY></TABLE></BODY></HTML>
