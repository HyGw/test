<%
if Request_Form("delFlag") = "1" then

	if Request_Form("hSrouteFlag0") <> "17" then 
		TCWebApi_set("WebCurSet_Entry","route_id","hSrouteFlag0")
		TCWebApi_unset("Route_Entry")
	end if
	if Request_Form("hSrouteFlag1") <> "17" then 
		TCWebApi_set("WebCurSet_Entry","route_id","hSrouteFlag1")
		TCWebApi_unset("Route_Entry")
	end if
	if Request_Form("hSrouteFlag2") <> "17" then 
		TCWebApi_set("WebCurSet_Entry","route_id","hSrouteFlag2")
		TCWebApi_unset("Route_Entry")
	end if
	if Request_Form("hSrouteFlag3") <> "17" then 
		TCWebApi_set("WebCurSet_Entry","route_id","hSrouteFlag3")
		TCWebApi_unset("Route_Entry")
	end if
	if Request_Form("hSrouteFlag4") <> "17" then 
		TCWebApi_set("WebCurSet_Entry","route_id","hSrouteFlag4")
		TCWebApi_unset("Route_Entry")
	end if
	if Request_Form("hSrouteFlag5") <> "17" then 
		TCWebApi_set("WebCurSet_Entry","route_id","hSrouteFlag5")
		TCWebApi_unset("Route_Entry")
	end if
	if Request_Form("hSrouteFlag6") <> "17" then 
		TCWebApi_set("WebCurSet_Entry","route_id","hSrouteFlag6")
		TCWebApi_unset("Route_Entry")
	end if
	if Request_Form("hSrouteFlag7") <> "17" then 
		TCWebApi_set("WebCurSet_Entry","route_id","hSrouteFlag7")
		TCWebApi_unset("Route_Entry")
	end if
	if Request_Form("hSrouteFlag8") <> "17" then 
		TCWebApi_set("WebCurSet_Entry","route_id","hSrouteFlag8")
		TCWebApi_unset("Route_Entry")
	end if
	if Request_Form("hSrouteFlag9") <> "17" then 
		TCWebApi_set("WebCurSet_Entry","route_id","hSrouteFlag9")
		TCWebApi_unset("Route_Entry")
	end if
	if Request_Form("hSrouteFlag10") <> "17" then 
		TCWebApi_set("WebCurSet_Entry","route_id","hSrouteFlag10")
		TCWebApi_unset("Route_Entry")
	end if
	if Request_Form("hSrouteFlag11") <> "17" then 
		TCWebApi_set("WebCurSet_Entry","route_id","hSrouteFlag11")
		TCWebApi_unset("Route_Entry")
	end if
	if Request_Form("hSrouteFlag12") <> "17" then 
		TCWebApi_set("WebCurSet_Entry","route_id","hSrouteFlag12")
		TCWebApi_unset("Route_Entry")
	end if
	if Request_Form("hSrouteFlag13") <> "17" then 
		TCWebApi_set("WebCurSet_Entry","route_id","hSrouteFlag13")
		TCWebApi_unset("Route_Entry")
	end if
	if Request_Form("hSrouteFlag14") <> "17" then 
		TCWebApi_set("WebCurSet_Entry","route_id","hSrouteFlag14")
		TCWebApi_unset("Route_Entry")
	end if
	if Request_Form("hSrouteFlag15") <> "17" then 
		TCWebApi_set("WebCurSet_Entry","route_id","hSrouteFlag15")
		TCWebApi_unset("Route_Entry")
	end if
	TCWebApi_commit("WebCurSet_Entry")
	TCWebApi_commit("Route_Entry")
	TCWebApi_save("Route_Entry")
end if
If Request_Form("Rip_Flag") = "1" then
	TCWebApi_set("WanInfo_Common","CurIFIndex","sel_Interface")
	TCWebApi_commit("WanInfo_Common")
elseif Request_Form("Rip_Flag") = "2" then 
	TCWebApi_set("WanInfo_Common","CurIFIndex","curSet_Flag")
	TCWebApi_set("WanInfo_WanPVC","Action","wanif_action")
	TCWebApi_set("WanInfo_WanIF","RIPVERSION","X_ATP_RIPOperation")
	TCWebApi_set("WanInfo_WanIF","DIRECTION","RouteProtocolRx")
	
	TCWebApi_commit("WanInfo_WanPVC")
	TCWebApi_save()
End if

if Request_Form("route6flag") = "2" then
	if Request_Form("route6delflag0") <> "17" then 
		TCWebApi_set("WebCurSet_Entry","route6_id","route6delflag0")
		TCWebApi_unset("Route6_Entry")
	end if
	if Request_Form("route6delflag1") <> "17" then 
		TCWebApi_set("WebCurSet_Entry","route6_id","route6delflag1")
		TCWebApi_unset("Route6_Entry")
	end if
	if Request_Form("route6delflag2") <> "17" then 
		TCWebApi_set("WebCurSet_Entry","route6_id","route6delflag2")
		TCWebApi_unset("Route6_Entry")
	end if
	if Request_Form("route6delflag3") <> "17" then 
		TCWebApi_set("WebCurSet_Entry","route6_id","route6delflag3")
		TCWebApi_unset("Route6_Entry")
	end if
	if Request_Form("route6delflag4") <> "17" then 
		TCWebApi_set("WebCurSet_Entry","route6_id","route6delflag4")
		TCWebApi_unset("Route6_Entry")
	end if
	if Request_Form("route6delflag5") <> "17" then 
		TCWebApi_set("WebCurSet_Entry","route6_id","route6delflag5")
		TCWebApi_unset("Route6_Entry")
	end if
	if Request_Form("route6delflag6") <> "17" then 
		TCWebApi_set("WebCurSet_Entry","route6_id","route6delflag6")
		TCWebApi_unset("Route6_Entry")
	end if
	if Request_Form("route6delflag7") <> "17" then 
		TCWebApi_set("WebCurSet_Entry","route6_id","route6delflag7")
		TCWebApi_unset("Route6_Entry")
	end if
	if Request_Form("route6delflag8") <> "17" then 
		TCWebApi_set("WebCurSet_Entry","route6_id","route6delflag8")
		TCWebApi_unset("Route6_Entry")
	end if
	if Request_Form("route6delflag9") <> "17" then 
		TCWebApi_set("WebCurSet_Entry","route6_id","route6delflag9")
		TCWebApi_unset("Route6_Entry")
	end if
	if Request_Form("route6delflag10") <> "17" then 
		TCWebApi_set("WebCurSet_Entry","route6_id","route6delflag10")
		TCWebApi_unset("Route6_Entry")
	end if
	if Request_Form("route6delflag11") <> "17" then 
		TCWebApi_set("WebCurSet_Entry","route6_id","route6delflag11")
		TCWebApi_unset("Route6_Entry")
	end if
	if Request_Form("route6delflag12") <> "17" then 
		TCWebApi_set("WebCurSet_Entry","route6_id","route6delflag12")
		TCWebApi_unset("Route6_Entry")
	end if
	if Request_Form("route6delflag13") <> "17" then 
		TCWebApi_set("WebCurSet_Entry","route6_id","route6delflag13")
		TCWebApi_unset("Route6_Entry")
	end if
	if Request_Form("route6delflag14") <> "17" then 
		TCWebApi_set("WebCurSet_Entry","route6_id","route6delflag14")
		TCWebApi_unset("Route6_Entry")
	end if
	if Request_Form("route6delflag15") <> "17" then 
		TCWebApi_set("WebCurSet_Entry","route6_id","route6delflag15")
		TCWebApi_unset("Route6_Entry")
	end if
	TCWebApi_commit("Route6_Entry")
	TCWebApi_save()
end if
%>

<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0 Transitional//EN">
<HTML><HEAD><TITLE>路由配置</TITLE>
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
            value="网络->路由配置" name=Selected_Menu> </FONT></B><SPAN 
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
          <TD height=15><IMG height=15 src="/img/panel1.gif" width=164 
            border=0> </TD>
          <TD><IMG height=15 src="/img/panel2.gif" width=645 border=0> 
          </TD></TR></TBODY></TABLE></TD></TR>
  <TR>
    <TD vAlign=top>
      <SCRIPT language=JavaScript src="/JS/printbox.js"></SCRIPT>

      <SCRIPT language=JavaScript type=text/javascript>
	var nEntryNum = <% tcWebApi_get("WanInfo_Common","CycleNum","s") %>;
	var vArrayStr = "<% tcWebApi_get("WanInfo_Common","CycleValue","s") %>";
	var vEntryName = vArrayStr.split(',');
	vArrayStr = "<% tcWebApi_get("WanInfo_Common","CycleValue","s") %>";
	var vEntryValue = vArrayStr.split(',');
	vArrayStr = "<% tcWebApi_get("WanInfo_Common","ValidIFIndex","s") %>";
	var vEntryIndex = vArrayStr.split(',');
	
function stRip(domain,Name,Operation,Protocol)
{
this.domain = domain;
this.key = domain.substr(domain.indexOf('WANDevice'));
this.Name = Name;
this.Operation = Operation;
this.Protocol = Protocol;
}
var WanIPRip = new Array(null);
var WanPPPRip = new Array(new stRip("InternetGatewayDevice.WANDevice.1.WANConnectionDevice.4.WANPPPConnection.2","1_TR069_INTERNET_R_0_35","Active","Off"),null);
var Rip = new Array();
for (i = 0; i < WanIPRip.length-1; i++)
{
Rip[i] = WanIPRip[i];
}
for (j = 0; j < WanPPPRip.length-1; i++,j++)
{
Rip[i] =  WanPPPRip[j];
}
function ShowIfName(val,arrMsgInfo)
{
		for (i = 0; i < Rip.length; i++)
		{
			if (Rip[i].domain == val)
			{
				return Rip[i].Name;
			}
			else if ('br0' == val)
			{
				return 'br0';
			}
		}
		return '&nbsp;';
}
//....................dsynic static  route  wait added
var nCount = 16;
function CheckRoutesActives()
{
	var	nCurTemp = 0;
	var	vDestIPAddress = new Array(nCount);
	var	vSub_mask = new Array(nCount);
	var	vGatewayIPAddress = new Array(nCount);
	var	vInterface = new Array(nCount);
	
	var	vcurLinks = new Array(nCount+3);
	
	var	vTemp;
	var	RouteNameTemp;

	vDestIPAddress[0] = "<% tcWebApi_get("Route_Entry0","DST_IP","s") %>";
	vDestIPAddress[1] = "<% tcWebApi_get("Route_Entry1","DST_IP","s") %>";
	vDestIPAddress[2] = "<% tcWebApi_get("Route_Entry2","DST_IP","s") %>";
	vDestIPAddress[3] = "<% tcWebApi_get("Route_Entry3","DST_IP","s") %>";
	vDestIPAddress[4] = "<% tcWebApi_get("Route_Entry4","DST_IP","s") %>";
	vDestIPAddress[5] = "<% tcWebApi_get("Route_Entry5","DST_IP","s") %>";
	vDestIPAddress[6] = "<% tcWebApi_get("Route_Entry6","DST_IP","s") %>";
	vDestIPAddress[7] = "<% tcWebApi_get("Route_Entry7","DST_IP","s") %>";
	vDestIPAddress[8] = "<% tcWebApi_get("Route_Entry8","DST_IP","s") %>";
	vDestIPAddress[9] = "<% tcWebApi_get("Route_Entry9","DST_IP","s") %>";
	vDestIPAddress[10] = "<% tcWebApi_get("Route_Entry10","DST_IP","s") %>";
	vDestIPAddress[11] = "<% tcWebApi_get("Route_Entry11","DST_IP","s") %>";
	vDestIPAddress[12] = "<% tcWebApi_get("Route_Entry12","DST_IP","s") %>";
	vDestIPAddress[13] = "<% tcWebApi_get("Route_Entry13","DST_IP","s") %>";
	vDestIPAddress[14] = "<% tcWebApi_get("Route_Entry14","DST_IP","s") %>";
	vDestIPAddress[15] = "<% tcWebApi_get("Route_Entry15","DST_IP","s") %>";
	
	vSub_mask[0] = "<% tcWebApi_get("Route_Entry0","Sub_mask","s") %>";
	vSub_mask[1] = "<% tcWebApi_get("Route_Entry1","Sub_mask","s") %>";
	vSub_mask[2] = "<% tcWebApi_get("Route_Entry2","Sub_mask","s") %>";
	vSub_mask[3] = "<% tcWebApi_get("Route_Entry3","Sub_mask","s") %>";
	vSub_mask[4] = "<% tcWebApi_get("Route_Entry4","Sub_mask","s") %>";
	vSub_mask[5] = "<% tcWebApi_get("Route_Entry5","Sub_mask","s") %>";
	vSub_mask[6] = "<% tcWebApi_get("Route_Entry6","Sub_mask","s") %>";
	vSub_mask[7] = "<% tcWebApi_get("Route_Entry7","Sub_mask","s") %>";
	vSub_mask[8] = "<% tcWebApi_get("Route_Entry8","Sub_mask","s") %>";
	vSub_mask[9] = "<% tcWebApi_get("Route_Entry9","Sub_mask","s") %>";
	vSub_mask[10] = "<% tcWebApi_get("Route_Entry10","Sub_mask","s") %>";
	vSub_mask[11] = "<% tcWebApi_get("Route_Entry11","Sub_mask","s") %>";
	vSub_mask[12] = "<% tcWebApi_get("Route_Entry12","Sub_mask","s") %>";
	vSub_mask[13] = "<% tcWebApi_get("Route_Entry13","Sub_mask","s") %>";
	vSub_mask[14] = "<% tcWebApi_get("Route_Entry14","Sub_mask","s") %>";
	vSub_mask[15] = "<% tcWebApi_get("Route_Entry15","Sub_mask","s") %>";
	
	vGatewayIPAddress[0] = "<% tcWebApi_get("Route_Entry0","Gateway","s") %>";
	vGatewayIPAddress[1] = "<% tcWebApi_get("Route_Entry1","Gateway","s") %>";
	vGatewayIPAddress[2] = "<% tcWebApi_get("Route_Entry2","Gateway","s") %>";
	vGatewayIPAddress[3] = "<% tcWebApi_get("Route_Entry3","Gateway","s") %>";
	vGatewayIPAddress[4] = "<% tcWebApi_get("Route_Entry4","Gateway","s") %>";
	vGatewayIPAddress[5] = "<% tcWebApi_get("Route_Entry5","Gateway","s") %>";
	vGatewayIPAddress[6] = "<% tcWebApi_get("Route_Entry6","Gateway","s") %>";
	vGatewayIPAddress[7] = "<% tcWebApi_get("Route_Entry7","Gateway","s") %>";
	vGatewayIPAddress[8] = "<% tcWebApi_get("Route_Entry8","Gateway","s") %>";
	vGatewayIPAddress[9] = "<% tcWebApi_get("Route_Entry9","Gateway","s") %>";
	vGatewayIPAddress[10] = "<% tcWebApi_get("Route_Entry10","Gateway","s") %>";
	vGatewayIPAddress[11] = "<% tcWebApi_get("Route_Entry11","Gateway","s") %>";
	vGatewayIPAddress[12] = "<% tcWebApi_get("Route_Entry12","Gateway","s") %>";
	vGatewayIPAddress[13] = "<% tcWebApi_get("Route_Entry13","Gateway","s") %>";
	vGatewayIPAddress[14] = "<% tcWebApi_get("Route_Entry14","Gateway","s") %>";
	vGatewayIPAddress[15] = "<% tcWebApi_get("Route_Entry15","Gateway","s") %>";
	
	vInterface[0] = "<% tcWebApi_get("Route_Entry0","Device","s") %>";
	vInterface[1] = "<% tcWebApi_get("Route_Entry1","Device","s") %>";
	vInterface[2] = "<% tcWebApi_get("Route_Entry2","Device","s") %>";
	vInterface[3] = "<% tcWebApi_get("Route_Entry3","Device","s") %>";
	vInterface[4] = "<% tcWebApi_get("Route_Entry4","Device","s") %>";
	vInterface[5] = "<% tcWebApi_get("Route_Entry5","Device","s") %>";
	vInterface[6] = "<% tcWebApi_get("Route_Entry6","Device","s") %>";
	vInterface[7] = "<% tcWebApi_get("Route_Entry7","Device","s") %>";
	vInterface[8] = "<% tcWebApi_get("Route_Entry8","Device","s") %>";
	vInterface[9] = "<% tcWebApi_get("Route_Entry9","Device","s") %>";
	vInterface[10] = "<% tcWebApi_get("Route_Entry10","Device","s") %>";
	vInterface[11] = "<% tcWebApi_get("Route_Entry11","Device","s") %>";
	vInterface[12] = "<% tcWebApi_get("Route_Entry12","Device","s") %>";
	vInterface[13] = "<% tcWebApi_get("Route_Entry13","Device","s") %>";
	vInterface[14] = "<% tcWebApi_get("Route_Entry14","Device","s") %>";
	vInterface[15] = "<% tcWebApi_get("Route_Entry15","Device","s") %>";
	
	vcurLinks[nCurTemp++] = new Array("net-routeset.asp", "StaticRoute", "", "20%", "20%", "20%", "20%", "10%", "10%");
	vcurLinks[nCurTemp++] = new Array("cgi-bin/net-routeadd.asp","", "InternetGatewayDevice.Layer3Forwarding.Forwarding", "目的地址", "网关", "子网掩码", "接口");
	vcurLinks[nCurTemp++] = new Array("", "", "", "", "", "", "","");
	//......
	for(var i=0; i<nCount; i++)
	{
		if(vDestIPAddress[i] != "N/A")
			vcurLinks[nCurTemp++] = new Array("","", i, vDestIPAddress[i], vGatewayIPAddress[i], vSub_mask[i], vInterface[i],"","");
	}
	var	vObjRet = new Array(nCurTemp+1);
	for(var m=0; m<nCurTemp; m++)
	{
		vObjRet[m] = vcurLinks[m];
	}
	vObjRet[nCurTemp] = null;
	return vObjRet;
}
var StaticRoute = CheckRoutesActives();

/*var StaticRoute = new Array(
new Array("net-routeset.asp", "StaticRoute", "", "20%", "20%", "20%", "20%", "10%", "10%"),
new Array("cgi-bin/net-routeadd.asp","", "InternetGatewayDevice.Layer3Forwarding.Forwarding", "目的地址", "网关", "子网掩码", "接口"),
new Array("", "", "", "", "", "", "ShowIfName",""),
new Array("0", "21", "22", "232", "24", "25", "ShowIfName",""),
new Array()
);*/
function stStaticRoute(Domain, Enable, DestIPAddress, PrefixLength, GatewayIPAddress, Interface, Status)
{
this.Domain = Domain;
this.enable = Enable;
this.DestIPAddress = DestIPAddress;
this.PrefixLength = PrefixLength;
this.GatewayIPAddress = GatewayIPAddress;
this.Interface = Interface;
this.Status = Status;

}
function GetRoute6Info()
{
	var ncount6 = 16;
	var	nCurTemp = 0;
	var	vRoute6Actives = new Array(ncount6);
	var	vDestIPAddress = new Array(ncount6);
	var	vPrefixLength = new Array(ncount6);
	var	vGatewayIPAddress = new Array(ncount6);
	var	vInterface = new Array(ncount6);
	
	var	vcurLinks = new Array(ncount6);
	var	vTemp;
	var	Route6NameTemp;
	vRoute6Actives[0] = "<% tcWebApi_get("Route6_Entry0","Active","s") %>";
	vRoute6Actives[1] = "<% tcWebApi_get("Route6_Entry1","Active","s") %>";
	vRoute6Actives[2] = "<% tcWebApi_get("Route6_Entry2","Active","s") %>";
	vRoute6Actives[3] = "<% tcWebApi_get("Route6_Entry3","Active","s") %>";
	vRoute6Actives[4] = "<% tcWebApi_get("Route6_Entry4","Active","s") %>";
	vRoute6Actives[5] = "<% tcWebApi_get("Route6_Entry5","Active","s") %>";
	vRoute6Actives[6] = "<% tcWebApi_get("Route6_Entry6","Active","s") %>";
	vRoute6Actives[7] = "<% tcWebApi_get("Route6_Entry7","Active","s") %>";
	vRoute6Actives[8] = "<% tcWebApi_get("Route6_Entry8","Active","s") %>";
	vRoute6Actives[9] = "<% tcWebApi_get("Route6_Entry9","Active","s") %>";
	vRoute6Actives[10] = "<% tcWebApi_get("Route6_Entry10","Active","s") %>";
	vRoute6Actives[11] = "<% tcWebApi_get("Route6_Entry11","Active","s") %>";
	vRoute6Actives[12] = "<% tcWebApi_get("Route6_Entry12","Active","s") %>";
	vRoute6Actives[13] = "<% tcWebApi_get("Route6_Entry13","Active","s") %>";
	vRoute6Actives[14] = "<% tcWebApi_get("Route6_Entry14","Active","s") %>";
	vRoute6Actives[15] = "<% tcWebApi_get("Route6_Entry15","Active","s") %>";
	
	vDestIPAddress[0] = "<% tcWebApi_get("Route6_Entry0","DST_IP","s") %>";
	vDestIPAddress[1] = "<% tcWebApi_get("Route6_Entry1","DST_IP","s") %>";
	vDestIPAddress[2] = "<% tcWebApi_get("Route6_Entry2","DST_IP","s") %>";
	vDestIPAddress[3] = "<% tcWebApi_get("Route6_Entry3","DST_IP","s") %>";
	vDestIPAddress[4] = "<% tcWebApi_get("Route6_Entry4","DST_IP","s") %>";
	vDestIPAddress[5] = "<% tcWebApi_get("Route6_Entry5","DST_IP","s") %>";
	vDestIPAddress[6] = "<% tcWebApi_get("Route6_Entry6","DST_IP","s") %>";
	vDestIPAddress[7] = "<% tcWebApi_get("Route6_Entry7","DST_IP","s") %>";
	vDestIPAddress[8] = "<% tcWebApi_get("Route6_Entry8","DST_IP","s") %>";
	vDestIPAddress[9] = "<% tcWebApi_get("Route6_Entry9","DST_IP","s") %>";
	vDestIPAddress[10] = "<% tcWebApi_get("Route6_Entry10","DST_IP","s") %>";
	vDestIPAddress[11] = "<% tcWebApi_get("Route6_Entry11","DST_IP","s") %>";
	vDestIPAddress[12] = "<% tcWebApi_get("Route6_Entry12","DST_IP","s") %>";
	vDestIPAddress[13] = "<% tcWebApi_get("Route6_Entry13","DST_IP","s") %>";
	vDestIPAddress[14] = "<% tcWebApi_get("Route6_Entry14","DST_IP","s") %>";
	vDestIPAddress[15] = "<% tcWebApi_get("Route6_Entry15","DST_IP","s") %>";
	
	vPrefixLength[0] = "<% tcWebApi_get("Route6_Entry0","Prefix_len","s") %>";
	vPrefixLength[1] = "<% tcWebApi_get("Route6_Entry1","Prefix_len","s") %>";
	vPrefixLength[2] = "<% tcWebApi_get("Route6_Entry2","Prefix_len","s") %>";
	vPrefixLength[3] = "<% tcWebApi_get("Route6_Entry3","Prefix_len","s") %>";
	vPrefixLength[4] = "<% tcWebApi_get("Route6_Entry4","Prefix_len","s") %>";
	vPrefixLength[5] = "<% tcWebApi_get("Route6_Entry5","Prefix_len","s") %>";
	vPrefixLength[6] = "<% tcWebApi_get("Route6_Entry6","Prefix_len","s") %>";
	vPrefixLength[7] = "<% tcWebApi_get("Route6_Entry7","Prefix_len","s") %>";
	vPrefixLength[8] = "<% tcWebApi_get("Route6_Entry8","Prefix_len","s") %>";
	vPrefixLength[9] = "<% tcWebApi_get("Route6_Entry9","Prefix_len","s") %>";
	vPrefixLength[10] = "<% tcWebApi_get("Route6_Entry10","Prefix_len","s") %>";
	vPrefixLength[11] = "<% tcWebApi_get("Route6_Entry11","Prefix_len","s") %>";
	vPrefixLength[12] = "<% tcWebApi_get("Route6_Entry12","Prefix_len","s") %>";
	vPrefixLength[13] = "<% tcWebApi_get("Route6_Entry13","Prefix_len","s") %>";
	vPrefixLength[14] = "<% tcWebApi_get("Route6_Entry14","Prefix_len","s") %>";
	vPrefixLength[15] = "<% tcWebApi_get("Route6_Entry15","Prefix_len","s") %>";
	
	vGatewayIPAddress[0] = "<% tcWebApi_get("Route6_Entry0","Gateway","s") %>";
	vGatewayIPAddress[1] = "<% tcWebApi_get("Route6_Entry1","Gateway","s") %>";
	vGatewayIPAddress[2] = "<% tcWebApi_get("Route6_Entry2","Gateway","s") %>";
	vGatewayIPAddress[3] = "<% tcWebApi_get("Route6_Entry3","Gateway","s") %>";
	vGatewayIPAddress[4] = "<% tcWebApi_get("Route6_Entry4","Gateway","s") %>";
	vGatewayIPAddress[5] = "<% tcWebApi_get("Route6_Entry5","Gateway","s") %>";
	vGatewayIPAddress[6] = "<% tcWebApi_get("Route6_Entry6","Gateway","s") %>";
	vGatewayIPAddress[7] = "<% tcWebApi_get("Route6_Entry7","Gateway","s") %>";
	vGatewayIPAddress[8] = "<% tcWebApi_get("Route6_Entry8","Gateway","s") %>";
	vGatewayIPAddress[9] = "<% tcWebApi_get("Route6_Entry9","Gateway","s") %>";
	vGatewayIPAddress[10] = "<% tcWebApi_get("Route6_Entry10","Gateway","s") %>";
	vGatewayIPAddress[11] = "<% tcWebApi_get("Route6_Entry11","Gateway","s") %>";
	vGatewayIPAddress[12] = "<% tcWebApi_get("Route6_Entry12","Gateway","s") %>";
	vGatewayIPAddress[13] = "<% tcWebApi_get("Route6_Entry13","Gateway","s") %>";
	vGatewayIPAddress[14] = "<% tcWebApi_get("Route6_Entry14","Gateway","s") %>";
	vGatewayIPAddress[15] = "<% tcWebApi_get("Route6_Entry15","Gateway","s") %>";
	
	vInterface[0] = "<% tcWebApi_get("Route6_Entry0","Device","s") %>";
	vInterface[1] = "<% tcWebApi_get("Route6_Entry1","Device","s") %>";
	vInterface[2] = "<% tcWebApi_get("Route6_Entry2","Device","s") %>";
	vInterface[3] = "<% tcWebApi_get("Route6_Entry3","Device","s") %>";
	vInterface[4] = "<% tcWebApi_get("Route6_Entry4","Device","s") %>";
	vInterface[5] = "<% tcWebApi_get("Route6_Entry5","Device","s") %>";
	vInterface[6] = "<% tcWebApi_get("Route6_Entry6","Device","s") %>";
	vInterface[7] = "<% tcWebApi_get("Route6_Entry7","Device","s") %>";
	vInterface[8] = "<% tcWebApi_get("Route6_Entry8","Device","s") %>";
	vInterface[9] = "<% tcWebApi_get("Route6_Entry9","Device","s") %>";
	vInterface[10] = "<% tcWebApi_get("Route6_Entry10","Device","s") %>";
	vInterface[11] = "<% tcWebApi_get("Route6_Entry11","Device","s") %>";
	vInterface[12] = "<% tcWebApi_get("Route6_Entry12","Device","s") %>";
	vInterface[13] = "<% tcWebApi_get("Route6_Entry13","Device","s") %>";
	vInterface[14] = "<% tcWebApi_get("Route6_Entry14","Device","s") %>";
	vInterface[15] = "<% tcWebApi_get("Route6_Entry15","Device","s") %>";
	
	//......
	for(var i=0; i<ncount6; i++)
	{
		if(vRoute6Actives[i] != "N/A")
		{
			vcurLinks[nCurTemp++] = new stStaticRoute(i, vRoute6Actives[i], vDestIPAddress[i], vPrefixLength[i], vGatewayIPAddress[i], vInterface[i], 0);
		}
	}
	var	vObjRet = new Array(nCurTemp+1);
	for(var m=0; m<nCurTemp; m++)
	{
		vObjRet[m] = vcurLinks[m];
	}
	vObjRet[nCurTemp] = null;
	return vObjRet;
}
var StaticRoute6 = GetRoute6Info();
function LoadFrame()
{
	document.StaticRouteForm.delFlag.value = "0";
	document.RipCfg.Rip_Flag.value = "0";
	document.Ipv6route.route6flag.value = "0";
}
function AddStaticRt()
{
location.replace('net-route6add.asp');
}
function removeClick(rml)
{
	with(getElById('Ipv6route'))
	{
		if (rml == null)
		{
			return;
		}
//var Form = new webSubmitForm();
		var Form = document.Ipv6route;
		var k;
		if (rml.length > 0)
		{
			for (k = 0; k < rml.length; k++)
			{
				var strindex = "route6delflag" + rml[k].value;
				if ( rml[k].checked == true )
				{
					//Form.addParameter(rml[k].value,'');
					setText(strindex, rml[k].value);
				}
				else
					setText(strindex, "17");
				
			}
		}
		else if ( rml.checked == true )
		{
			//Form.addParameter(rml.value,'');
			var strindex = "route6delflag" + rml.value;
			setText(strindex, rml.value);
			
		}
//Form.setAction('del.cgi?RequestFile=html/network/route.asp');
		Form.route6flag.value = "2";
		Form.submit();
	}
}
function CheckForm()
{
if (getElById('sel_Interface').length == 0)
{
alert('当前没有WAN连接，请新建新的WAN连接后再进行RIP配置！');
return false;
}
return true;
}
function AddSubmitParam(SubmitForm,type)
{
SubmitForm.addParameter('x.X_ATP_RIPOperation',getSelectVal('X_ATP_RIPOperation')) ;
SubmitForm.addParameter('x.RouteProtocolRx',getSelectVal('RouteProtocolRx')) ;
SubmitForm.setAction('setcfg.cgi?x=' + getSelectVal('sel_Interface') + '&RequestFile=html/network/route.asp');
}
function ChangeInterface(Interface)
{
	//var ripInfo = Rip[Interface.selectedIndex];
//setSelect('X_ATP_RIPOperation',ripInfo.Operation);
//setSelect('RouteProtocolRx',ripInfo.Protocol);
//top.ripIndex = Interface.selectedIndex;
	document.RipCfg.Rip_Flag.value = "1";
	document.RipCfg.submit();
	
}
</SCRIPT>

      <TABLE height="100%" cellSpacing=0 cellPadding=0 border=0>
        <TBODY>
        <TR>
          <TD width=157 bgColor=#ef8218 height=30>
            <P class=Item_L1>静态路由</P></TD>
          <TD width=7 bgColor=#ef8218>　</TD>
          <TD width=474>　</TD>
          <TD vAlign=top width=170 background=/img/panel4.gif 
            rowSpan=13><TABLE cellSpacing=0 cellPadding=20 width="100%" 
border=0>
              <TBODY>
              <TR>
                <TD><A 
                  href="/cgi-bin/help_content.asp#路由配置" 
                  target=_blank><IMG height=34 src="/img/help.gif" 
                  width=40 border=0></A></TD></TR></TBODY></TABLE>　 　　　 　　　</TD></TR>
        <TR>
          <TD vAlign=top width=157 bgColor=#e7e7e7 height=10></TD>
          <TD width=7 background=/img/panel3.gif>　</TD>
          <TD></TD></TR>
        <TR>
          <TD vAlign=top width=157 bgColor=#e7e7e7 height=30>
            <P class=Item_L2></P></TD>
          <TD width=7 background=/img/panel3.gif>　</TD>
          <TD>
            <DIV action="" name="ConfigForm">
			<FORM id="StaticRouteForm" name="StaticRouteForm" action="/cgi-bin/net-routeset.asp" method="post">
            <TABLE cellSpacing=0 cellPadding=0 width="100%" border=0>
              <TBODY>
              <TR>
                <TD width=10>&nbsp;</TD>
                <TD>
                  <P>
				  
                  <TABLE cellSpacing=0 cellPadding=0 width="100%" border=0>
                    <TBODY>
                    <TR>
                      <TD>
                        <SCRIPT language=JavaScript type=text/javascript>
printInstNoEditBox('StaticRoute',400);
</SCRIPT>
                        <input type="hidden" name="del_num">
						<input type="hidden" name="hSrouteFlag0" value="17">
                        <input type="hidden" name="hSrouteFlag1" value="17">
                        <input type="hidden" name="hSrouteFlag2" value="17">
                        <input type="hidden" name="hSrouteFlag3" value="17">
                        <input type="hidden" name="hSrouteFlag4" value="17">
                        <input type="hidden" name="hSrouteFlag5" value="17">
                        <input type="hidden" name="hSrouteFlag6" value="17">
                        <input type="hidden" name="hSrouteFlag7" value="17">
						<input type="hidden" name="hSrouteFlag8" value="17">
						<input type="hidden" name="hSrouteFlag9" value="17">
						<input type="hidden" name="hSrouteFlag10" value="17">
						<input type="hidden" name="hSrouteFlag11" value="17">
						<input type="hidden" name="hSrouteFlag12" value="17">
						<input type="hidden" name="hSrouteFlag13" value="17">
						<input type="hidden" name="hSrouteFlag14" value="17">
						<input type="hidden" name="hSrouteFlag15" value="17">
                        <input type="hidden" name="delFlag" value="0">
                         <BR><BR></TD>
                      <TD width=10>&nbsp;</TD></TR></TBODY></TABLE></P></TD>
                <TD width=10>&nbsp;</TD></TR>
              <TR>
                <TD width=10>&nbsp;</TD>
                <TD>
                  <HR>
                </TD>
                <TD width=10>&nbsp;</TD></TR></TBODY></TABLE></FORM></DIV></TD></TR>
        <TR>
          <TD vAlign=top width=157 bgColor=#e7e7e7 height=10></TD>
          <TD width=7 background=/img/panel3.gif>　</TD>
          <TD></TD></TR>
        <TR id="DynaRouteInfo1">
          <TD width=157 bgColor=#ef8218 height=30>
            <P class=Item_L1>动态路由</P></TD>
          <TD width=7 bgColor=#ef8218>　</TD>
          <TD width=474>　</TD></TR>
        <TR>
          <TD vAlign=top width=157 bgColor=#e7e7e7 height=10></TD>
          <TD width=7 background=/img/panel3.gif>　</TD>
          <TD></TD></TR>
        <TR id="DynaRouteInfo2">
          <TD vAlign=top width=157 bgColor=#e7e7e7 height=30>
            <P class=Item_L2></P></TD>
          <TD width=7 background=/img/panel3.gif>　</TD>
          <TD>
            <TABLE cellSpacing=0 cellPadding=0 width="100%" border=0>
              <TBODY>
              <TR>
                <TD width=10>&nbsp;</TD>
                    <TD> 
                      <FORM id=RipCfg name="RipCfg" action="/cgi-bin/net-routeset.asp" method="post">
                  <TABLE cellSpacing=0 cellPadding=3 width=400 border=1>
                    <TBODY>
                    <TR align=middle>
                              <TD class=table_title>接口
                                <input type="hidden" name="curSet_Flag" value="<% tcWebApi_get("WanInfo_Common","CurIFIndex","s") %>">
                                <input type="hidden" name="Rip_Flag" value="0">
                                <input type="hidden" name="wanif_action" value="Modify"> 
                                <script language="JavaScript" type="text/JavaScript">
								function btnRipSave()
								{
									document.RipCfg.Rip_Flag.value = "2";
									document.RipCfg.submit();
									
								}
var		nEntryCount = 8;
function WanIndexConstruction(domain,WanName)
{
	this.domain = domain;
	this.WanName = WanName;
}
	
function WriteWanNameSelected()
{
	/*var WanIDNums = CurWan;
	var nlength = WanIDNums.length-1;
	for(var i=0; i< nlength; i++)
	{
		document.writeln("<option value=" + WanIDNums[i].domain + ">" + WanIDNums[i].WanName + "</option>");
		if(document.RipCfg.curSet_Flag.value == WanIDNums[i].domain)
				document.RipCfg.sel_Interface.selectedIndex= i;
	}*/
	
	for (var i = 0; i < nEntryNum; i++)
	{
			document.write('<option value=' + vEntryIndex[i] + '>' + vEntryName[i] + '</option>');
			if(document.RipCfg.curSet_Flag.value == vEntryIndex[i])
				document.RipCfg.sel_Interface.selectedIndex= i;
	}
	
}
								</script> </TD>
                      <TD class=table_title>操作</TD>
                      <TD class=table_title>版本</TD></TR>
                    <TR align=middle>
                              <TD><select id=select onChange=ChangeInterface(this) name=sel_Interface>
                                  <script language=JavaScript type=text/javascript>
						  WriteWanNameSelected();
//for (var i = 0; i < Rip.length; i++)
//{
//document.write('<option value=' + Rip[i].domain + '>'
//+ Rip[i].Name + '</option>');
//}
</script>
                                </select> </TD>
                              <TD><select id=X_ATP_RIPOperation name=X_ATP_RIPOperation>
                                  <option value="RIP1" <%if tcWebApi_get("WanInfo_WanIF","RIPVERSION","h") = "RIP1" then asp_Write("selected") end if%>>active</option>
                                  <option value="RIP2" <%if tcWebApi_get("WanInfo_WanIF","RIPVERSION","h") = "RIP2" then asp_Write("selected") end if%>>passive</option>
                                </select> </TD>
                      <TD><SELECT id=RouteProtocolRx name=RouteProtocolRx> 
                          <OPTION value="IN Only" <%if tcWebApi_get("WanInfo_WanIF","DIRECTION","h") = "IN Only" then asp_Write("selected") end if %>>1</OPTION> 
						  <OPTION value="OUT Only" <%if tcWebApi_get("WanInfo_WanIF","DIRECTION","h") = "OUT Only" then asp_Write("selected") end if %>>2</OPTION> 
						  <OPTION value="Both" <%if tcWebApi_get("WanInfo_WanIF","DIRECTION","h") = "Both" then asp_Write("selected") end if %>>Both</OPTION> 
						  <OPTION value="None" <%if tcWebApi_get("WanInfo_WanIF","DIRECTION","h") = "None" then asp_Write("selected") elseif tcWebApi_get("WanInfo_WanIF","DIRECTION","h") = "N/A" then asp_Write("selected") end if %>>Off</OPTION>
						 </SELECT>
						  </TD></TR>
                    <SCRIPT language=JavaScript type=text/javascript>
if (getElById('sel_Interface').length != 0)
{
var obj = getElById('sel_Interface');
if (top.ripIndex == '')
{
//top.ripIndex = 0;
}
else
{
//obj.selectedIndex = top.ripIndex;
}
//ChangeInterface(obj);
}
</SCRIPT>
                    </TBODY></TABLE></FORM><BR>
					<INPUT onclick="btnRipSave()" type=button value=保存/应用> 
                  </P></TD>
                <TD width=10>&nbsp;</TD></TR>
              <TR>
                <TD width=10>&nbsp;</TD>
                <TD></TD>
                <TD width=10>&nbsp;</TD></TR></TBODY></TABLE></TD></TR>
        <TR>
          <TD width=157 bgColor=#ef8218 height=30>
            <P class=Item_L1>IPv6 静态路由</P></TD>
          <TD width=7 bgColor=#ef8218>　</TD>
            <TD width=474>　
              <script language="JavaScript" type="text/JavaScript">
			  	getElById("DynaRouteInfo1").style.display = "none";
				getElById("DynaRouteInfo2").style.display = "none";
			  </script></TD></TR>
        <TR>
          <TD vAlign=top width=157 bgColor=#e7e7e7 height=10></TD>
          <TD width=7 background=/img/panel3.gif>　</TD>
          <TD></TD></TR>
        <TR>
          <TD vAlign=top width=157 bgColor=#e7e7e7 height=30>
            <P class=Item_L2></P></TD>
          <TD width=7 background=/img/panel3.gif>　</TD>
          <TD>
            <FORM id="Ipv6route" name="Ipv6route" action="/cgi-bin/net-routeset.asp" method="post">
            <TABLE cellSpacing=0 cellPadding=0 width="100%" border=0>
              <TBODY>
              <TR>
                <TD width=10>&nbsp;</TD>
                <TD>
                  <P>
                  <TABLE cellSpacing=0 cellPadding=0 width="100%" border=0>
                    <TBODY>
                    <TR>
                      <TD>
                        <TABLE cellSpacing=1 cellPadding=3 width=400 border=1>
                          <TBODY>
                          <TR align=middle>
                            <TD class=table_title align=middle>目的地址</TD>
                            <TD class=table_title align=middle>网关</TD>
                            <TD class=table_title align=middle>接口</TD>
                            <TD class=table_title align=middle>使能</TD>
                            <TD class=table_title align=middle>删除</TD></TR>
                          <SCRIPT language=JavaScript type=text/javascript>
for (i = 0; i < StaticRoute6.length-1; i++)
{
document.write('<TR align="middle">');
document.write('<TD align="center">' + StaticRoute6[i].DestIPAddress + '</TD>');
if (StaticRoute6[i].GatewayIPAddress != "N/A")
{
document.write('<TD align="center">' + StaticRoute6[i].GatewayIPAddress + '</TD>');
}
else
{
document.write('<TD align="center">' + '&nbsp' + '</TD>');
}
var wanname = "";
/*if (StaticRoute6[i].Interface != "br0")
{
for (j = 0; j < Rip.length; j++)
{
if (Rip[j].domain == StaticRoute6[i].Interface)
{
wanname = Rip[j].Name;
break;
}
}
}
else*/
{
wanname = StaticRoute6[i].Interface;
}
document.write('<TD align="center">' + wanname + '</TD>');
var str;
if ("Yes" == StaticRoute6[i].enable)
{
str = "使能";
}
else
{
str = "禁用";
}
document.write('<TD align="center">' + str + '</TD>');
document.write('<TD align="center">' + '<input type="checkbox" name="rml"'  + ' value="'+ StaticRoute6[i].Domain + '">' + '</TD>');
document.write('</TR>');
}
</SCRIPT>
                          </TBODY></TABLE>
                                <input type="hidden" name="route6flag" value="0">
                                <input type="hidden" name="route6delflag0" value="17">
								<input type="hidden" name="route6delflag1" value="17">
                                <input type="hidden" name="route6delflag2" value="17">
                                <input type="hidden" name="route6delflag3" value="17">
                                <input type="hidden" name="route6delflag4" value="17">
                                <input type="hidden" name="route6delflag5" value="17">
                                <input type="hidden" name="route6delflag6" value="17">
                                <input type="hidden" name="route6delflag7" value="17">
                                <input type="hidden" name="route6delflag8" value="17">
                                <input type="hidden" name="route6delflag9" value="17">
                                <input type="hidden" name="route6delflag10" value="17">
                                <input type="hidden" name="route6delflag11" value="17">
                                <input type="hidden" name="route6delflag12" value="17">
                                <input type="hidden" name="route6delflag13" value="17">
                                <input type="hidden" name="route6delflag14" value="17">
                                <input type="hidden" name="route6delflag15" value="17">
                                <BR>
                                <LEFT><INPUT id=btnAdd onclick="AddStaticRt()" type=button value=添加> 
                        &nbsp; <INPUT id="btnRemove" onclick="removeClick(this.form.rml)" type=button value=删除> 
                        </LEFT></TD></TR></TBODY></TABLE></P></TD>
                <TD width=10>&nbsp;</TD></TR>
              <TR>
                <TD width=10>&nbsp;</TD>
                <TD>
                  <HR>
                </TD>
                <TD width=10>&nbsp;</TD></TR></TBODY></TABLE></FORM></TD></TR>
        <DIV></DIV>
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
            <P align=center><IMG id=btnOK onclick="" height=23  src="/img/ok.gif" width=80 border=0>&nbsp;&nbsp;<IMG 
            id=btnCancel onclick="RefreshPage()" height=23 src="/img/cancel.gif" 
            width=80 border=0> </P></TD>
          <TD width=170 
bgColor=#313031>　</TD></TR></TBODY></TABLE></TD></TR></TBODY></TABLE></BODY></HTML>
