<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0 Transitional//EN">
<HTML><HEAD><TITLE>网络侧信息</TITLE>
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
            value="状态->网络侧信息" name=Selected_Menu> </FONT></B><SPAN 
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
      <SCRIPT language=JavaScript src="/img/printtable.js"></SCRIPT>

      <SCRIPT language=JavaScript type=text/javascript>
	  
	 	var para = 0;
		var pvc_counts=<% tcWebApi_get("WanInfo_Common","CycleNum","s") %>;
		//get all value
		// num 0
		var vArrayStr = "<% tcWebApi_get("WanInfo_Common","CycleValue","s") %>";
		var Wan_WanName = vArrayStr.split(',');
		// num 1
		vArrayStr = "<% tcWebApi_get("WanInfo_Common","CycleValue","s") %>";
		var Wan_WanValue = vArrayStr.split(',');
		// num 2
		vArrayStr = "<% tcWebApi_get("WanInfo_Common","CycleValue","s") %>";
		// num 3
		vArrayStr = "<% tcWebApi_get("WanInfo_Common","CycleValue","s") %>";
		Wan_Actives = vArrayStr.split(',');
		// num 4		
		vArrayStr = "<% tcWebApi_get("WanInfo_Common","CycleValue","s") %>";
		var WANEnNAT = vArrayStr.split(',');
		// num 5		
		vArrayStr = "<% tcWebApi_get("WanInfo_Common","CycleValue","s") %>";
		// num 6		
		vArrayStr = "<% tcWebApi_get("WanInfo_Common","CycleValue","s") %>";
		var Wan_IPVERSION = vArrayStr.split(',');
		// num 7		
		vArrayStr = "<% tcWebApi_get("WanInfo_Common","CycleValue","s") %>";
		var Wan_Status4 = vArrayStr.split(',');
		// num 8		
		vArrayStr = "<% tcWebApi_get("WanInfo_Common","CycleValue","s") %>";
		var Wan_IP4 = vArrayStr.split(',');
		// num 9		
		vArrayStr = "<% tcWebApi_get("WanInfo_Common","CycleValue","s") %>";
		var Wan_ENCAP = vArrayStr.split(',');
		// num 10		
		vArrayStr = "<% tcWebApi_get("WanInfo_Common","CycleValue","s") %>";
		var Wan_GateWay4 = vArrayStr.split(',');
		// num 11		
		vArrayStr = "<% tcWebApi_get("WanInfo_Common","CycleValue","s") %>";
		var Wan_DNS4 = vArrayStr.split(',');
		// num 12		
		vArrayStr = "<% tcWebApi_get("WanInfo_Common","CycleValue","s") %>";
		var Wan_SecDNS4 = vArrayStr.split(',');
		// num 13		
		vArrayStr = "<% tcWebApi_get("WanInfo_Common","CycleValue","s") %>";
		var Wan_Status6 = vArrayStr.split(',');
		// num 14		
		vArrayStr = "<% tcWebApi_get("WanInfo_Common","CycleValue","s") %>";
		var Wan_IP6 = vArrayStr.split(',');
		// num 15		
		vArrayStr = "<% tcWebApi_get("WanInfo_Common","CycleValue","s") %>";
		var Wan_GateWay6 = vArrayStr.split(',');
		// num 16		
		vArrayStr = "<% tcWebApi_get("WanInfo_Common","CycleValue","s") %>";
		var Wan_DNS6 = vArrayStr.split(',');
		// num 17		
		vArrayStr = "<% tcWebApi_get("WanInfo_Common","CycleValue","s") %>";
		var Wan_SecDNS6 = vArrayStr.split(',');
		// num 18		
		vArrayStr = "<% tcWebApi_get("WanInfo_Common","CycleValue","s") %>";
		var Wan_PD6 = vArrayStr.split(',');
		// num 19		
		vArrayStr = "<% tcWebApi_get("WanInfo_Common","CycleValue","s") %>";
		var Wan_NetMask4 = vArrayStr.split(',');
		// num 20		
		vArrayStr = "<% tcWebApi_get("WanInfo_Common","CycleValue","s") %>";
		var Wan_VPI = vArrayStr.split(',');
		// num 21		
		vArrayStr = "<% tcWebApi_get("WanInfo_Common","CycleValue","s") %>";
		var Wan_VCI = vArrayStr.split(',');

		function ListCreate_info1(para)
		{
				var index = 1;
				for(var i=0;i<pvc_counts;i++)
				{
					if(Wan_Actives[i] != "N/A")
					{
						if(para == 6)
						{

							if(Wan_IPVERSION[i] == "IPv6" || Wan_IPVERSION[i] == "IPv4/IPv6")
							{

								document.write('<tr align="middle">');
								document.write('<td>' + Wan_WanName[i] + '</td>');
								if(DslInfo.status == "up"){
									document.write('<td>' + Wan_Status6[i] + '</td>');
									document.write('<td>' + Wan_IP6[i] + '&nbsp;</td>');
									document.write('<td>' + Wan_GateWay6[i] + '&nbsp;</td>');
								}
								else{
									document.write('<td>' + DslInfo.status + '&nbsp;</td>');
									document.write('<td>&nbsp;</td>');
									document.write('<td>&nbsp;</td>');
								}
								document.write('</tr>');
								index = index + 1;
							}
							else 
							{
								if(Wan_IPVERSION[i] == "IPv4")
								{
									index = index + 1;
								}
							}
						}
						else 
						{
							if(para == 4)
							{
								if(Wan_IPVERSION[i] == "IPv4" || Wan_IPVERSION[i] == "IPv4/IPv6")
								{
									document.write('<tr align="middle">');
									document.write('<td>' + Wan_WanName[i] + '</td>');
									if(DslInfo.status == "up"){
										document.write('<td>' + Wan_Status4[i] + '&nbsp;</td>');
										document.write('<td>' + Wan_IP4[i] + '&nbsp;</td>');
										document.write('<td>' + Wan_NetMask4[i] + '&nbsp;</td>');
									}
									else{
										document.write('<td>' + DslInfo.status + '&nbsp;</td>');
										document.write('<td>&nbsp;</td>');
										document.write('<td>&nbsp;</td>');
									}
									document.write('</tr>');
									index = index + 1;
								}
								else 
								{
									if(Wan_IPVERSION[i] == "IPv6")
									{
										index = index + 1;
									}
								}
							}
						}
					}
				}
			}
			
		function ListCreate_info2(para)
		{
				var index = 1;
				for(var i=0;i<pvc_counts;i++)
				{
					if(Wan_Actives[i] != "N/A")
					{
						if(para == 6)
						{
							if(Wan_IPVERSION[i] == "IPv6" || Wan_IPVERSION[i] == "IPv4/IPv6")
							{
								document.write('<tr align="middle">');
								document.write('<td>' + Wan_WanName[i] + '</td>');
								document.write('<td>' + Wan_VPI[i] + '/' + Wan_VCI[i] + '</td>');
								document.write('<td>' + Wan_ENCAP[i] + '&nbsp;</td>');
								document.write('</tr>');
								index = index + 1;
							}
							else 
							{
								if(Wan_IPVERSION[i] == "IPv4")
								{
									index = index + 1;
								}
							}
						}
						else 
						{
							if(para == 4)
							{
								if(Wan_IPVERSION[i] == "IPv4" || Wan_IPVERSION[i] == "IPv4/IPv6")
								{
									document.write('<tr align="middle">');
									document.write('<td>' + Wan_WanName[i] + '</td>');
									document.write('<td>' + Wan_VPI[i] + '/' + Wan_VCI[i] + '</td>');
									document.write('<td>' + Wan_ENCAP[i] + '&nbsp;</td>');
									document.write('</tr>');
									index = index + 1;
								}
								else 
								{
									if(Wan_IPVERSION[i] == "IPv6")
									{
										index = index + 1;
									}
								}
							}
						}
					}
				}
			}
						
		function ListCreate_info3(para)
		{
				var index = 1;
				for(var i=0;i<pvc_counts;i++)
				{
					if(Wan_Actives[i] != "N/A")
					{
						if(para == 6)
						{
							if(Wan_IPVERSION[i] == "IPv6" || Wan_IPVERSION[i] == "IPv4/IPv6")
							{
								document.write('<tr align="middle">');
								document.write('<td>' + Wan_WanName[i] + '</td>');
								if(DslInfo.status == "up"){
									document.write('<td>' + Wan_DNS6[i] + '&nbsp;</td>');
									document.write('<td>' + Wan_SecDNS6[i] + '&nbsp;</td>');
									document.write('<td>' + Wan_PD6[i] + '&nbsp;</td>');
								}
								else{
									document.write('<td>&nbsp;</td>');
									document.write('<td>&nbsp;</td>');
									document.write('<td>&nbsp;</td>');
								}
								document.write('</tr>');
								index = index + 1;
							}
							else 
							{
								if(Wan_IPVERSION[i] == "IPv4")
								{
									index = index + 1;
								}
							}
						}
						else 
						{
							if(para == 4)
							{
								if(Wan_IPVERSION[i] == "IPv4" || Wan_IPVERSION[i] == "IPv4/IPv6")
								{
									document.write('<tr align="middle">');
									document.write('<td>' + Wan_WanName[i] + '</td>');
									if(DslInfo.status == "up"){
										document.write('<td>' + Wan_GateWay4[i] + '&nbsp;</td>');
										document.write('<td>' + Wan_DNS4[i] + '&nbsp;</td>');
										document.write('<td>' + Wan_SecDNS4[i] + '&nbsp;</td>');
									}
									else{
										document.write('<td>&nbsp;</td>');
										document.write('<td>&nbsp;</td>');
										document.write('<td>&nbsp;</td>');
									}
									document.write('</tr>');
									index = index + 1;
								}
								else 
								{
									if(Wan_IPVERSION[i] == "IPv6")
									{
										index = index + 1;
									}
								}
							}
						}
					}
				}
			}

function stDsl(domain,Modulation,status,UpCurrRate,DownCurrRate,UpMargin,DownMargin,UpDepth,DownDepth,Encoding,UpAttenuation,DownAttenuation,UpPower,DownPower,DataPath)
{
	this.domain = domain;
	this.Modulation = Modulation; 
	this.status = status; 
	this.UpCurrRate = UpCurrRate; 
	this.DownCurrRate = DownCurrRate; 
	this.UpMargin = UpMargin; 
	this.DownMargin = DownMargin;
 	this.UpDepth = UpDepth; 
	this.DownDepth = DownDepth;  
	this.Encoding = Encoding;
	this.UpAttenuation = UpAttenuation;
	this.DownAttenuation = DownAttenuation;
	this.UpPower = UpPower;
	this.DownPower = DownPower;
	this.DataPath = DataPath;
}
var DSLCfg = new Array(new stDsl("0","<% tcWebApi_get("Info_Adsl","Opmode","s") %>","<% tcWebApi_get("Info_Adsl","lineState","s") %>","<% tcWebApi_get("Info_Adsl","DataRateUp","s") %>","<% tcWebApi_get("Info_Adsl","DataRateDown","s") %>","<% tcWebApi_get("Info_Adsl","SNRMarginUp","s") %>","<% tcWebApi_get("Info_Adsl","SNRMarginDown","s") %>","<% tcWebApi_get("Info_Adsl","InterleaveDepth","s") %>","<% tcWebApi_get("Info_Adsl","InterleaveDepthDown","s") %>","<% tcWebApi_get("Adsl_Entry","MODULATIONTYPE","s") %>","<% tcWebApi_get("Info_Adsl","AttenUp","s") %>","<% tcWebApi_get("Info_Adsl","AttenDown","s") %>","<% tcWebApi_get("Info_Adsl","PowerUp","s") %>","<% tcWebApi_get("Info_Adsl","PowerDown","s") %>","<% tcWebApi_get("Info_Adsl","mtenStandard","s") %>"));
var DslInfo = DSLCfg[0];
</SCRIPT>

      <TABLE height="100%" cellSpacing=0 cellPadding=0 border=0>
        <TBODY>
        <TR>
          <TD width=157 bgColor=#ef8218 height=30>
            <P class=Item_L1>IPv4连接信息</P></TD>
          <TD width=7 bgColor=#ef8218>　</TD>
          <TD width=474>　</TD>
          <TD vAlign=top width=170 background=/img/panel4.gif 
          rowSpan=16>
            <TABLE cellSpacing=0 cellPadding=20 width="100%" border=0>
              <TBODY>
              <TR>
                <TD><A 
                  href="/cgi-bin/help_content.asp#网络侧信息" 
                  target=_blank><IMG height=34 src="/img/help.gif" 
                  width=40 border=0></A></TD></TR></TBODY></TABLE></TD></TR>
        <TR>
          <TD vAlign=top width=157 bgColor=#e7e7e7 height=10></TD>
          <TD width=7 background=/img/panel3.gif>　</TD>
          <TD></TD></TR>
        <TR>
          <TD vAlign=top width=157 bgColor=#e7e7e7 height=30>
            <P class=Item_L2>&nbsp;</P></TD>
          <TD width=7 background=/img/panel3.gif>　</TD>
          <TD>
            <TABLE cellSpacing=0 cellPadding=0 width="100%" border=0>
              <TBODY>
              <TR>
                <TD width=10>&nbsp;</TD>
                <TD>
                  <TABLE cellSpacing=0 cellPadding=3 width=406 border=1>
                    <TBODY>
                    <TR align=middle>
                      <TD class=table_title align=middle>连接名称</TD>
                      <TD class=table_title align=middle>状态</TD>
                      <TD class=table_title align=middle>IP地址</TD>
                      <TD class=table_title align=middle>子网掩码</TD></TR>
                    <SCRIPT language=JavaScript type=text/javascript>
						ListCreate_info1(4);
					</SCRIPT>
                    </TBODY></TABLE><BR>
                  <TABLE id=tabOptionv4 cellSpacing=0 cellPadding=3 width=406 
                  border=1 name="tabOptionv4">
                    <TBODY>
                    <TR align=middle>
                      <TD class=table_title align=middle>连接名称</TD>
                      <TD class=table_title id=secPVC align=middle>VPI/VCI</TD>
                      <TD class=table_title id=secEncap 
align=middle>封装信息</TD></TR>
                    <SCRIPT language=JavaScript type=text/javascript>
						ListCreate_info2(4);
					</SCRIPT>
                    </TBODY></TABLE><BR>
                  <TABLE cellSpacing=0 cellPadding=3 border=1>
                    <TBODY>
                    <TR>
                      <TD class=table_title align=middle width=180>连接名称</TD>
                      <TD class=table_title align=middle width=208>默认网关</TD>
                      <TD class=table_title align=middle width=208>首选DNS</TD>
                      <TD class=table_title align=middle width=208>备用DNS</TD></TR>
                    <SCRIPT language=javascript>
						ListCreate_info3(4);
					</SCRIPT>
                    </TBODY></TABLE></TD>
                <TD width=10>&nbsp;</TD></TR>
              <TR>
                <TD width=10>&nbsp;</TD>
                <TD>
                  <HR>
                </TD>
                <TD width=10>&nbsp;</TD></TR></TBODY></TABLE></TD></TR>
        <TR>
          <TD vAlign=top width=157 bgColor=#e7e7e7 height=10></TD>
          <TD width=7 background=/img/panel3.gif>　</TD>
          <TD></TD></TR>
        <TR>
          <TD width=157 bgColor=#ef8218 height=30>
            <P class=Item_L1>IPv6连接信息</P></TD>
          <TD width=7 bgColor=#ef8218>　</TD>
          <TD width=474>　</TD></TR>
        <TR>
          <TD vAlign=top width=157 bgColor=#e7e7e7 height=10></TD>
          <TD width=7 background=/img/panel3.gif>　</TD>
          <TD></TD></TR>
        <TR>
          <TD vAlign=top width=157 bgColor=#e7e7e7 height=30>
            <P class=Item_L2>&nbsp;</P></TD>
          <TD width=7 background=/img/panel3.gif>　</TD>
          <TD>
            <TABLE cellSpacing=0 cellPadding=0 width="100%" border=0>
              <TBODY>
              <TR>
                <TD width=10>&nbsp;</TD>
                <TD>
                  <TABLE cellSpacing=0 cellPadding=3 width=406 border=1>
                    <TBODY>
                    <TR align=middle>
                      <TD class=table_title align=middle>连接名称</TD>
                      <TD class=table_title align=middle>IPv6状态</TD>
                      <TD class=table_title align=middle>IPv6地址</TD>
                      <TD class=table_title align=middle>IPv6默认网关</TD></TR>
                    <SCRIPT language=JavaScript type=text/javascript>
						ListCreate_info1(6);
					</SCRIPT>
                    </TBODY></TABLE><BR>
                  <TABLE id=tabOptionv6 cellSpacing=0 cellPadding=3 width=406 
                  border=1 name="tabOptionv6">
                    <TBODY>
                    <TR align=middle>
                      <TD class=table_title align=middle>连接名称</TD>
                      <TD class=table_title id=secPVC align=middle>VPI/VCI</TD>
                      <TD class=table_title id=secEncap 
align=middle>封装信息</TD></TR>
                    <SCRIPT language=JavaScript type=text/javascript>
						ListCreate_info2(6);
					</SCRIPT>
                    </TBODY></TABLE><BR>
                  <TABLE cellSpacing=0 cellPadding=3 border=1>
                    <TBODY>
                    <TR>
                      <TD class=table_title align=middle width=180>连接名称</TD>
                      <TD class=table_title align=middle 
width=208>IPv6首选DNS</TD>
                      <TD class=table_title align=middle 
width=208>IPv6备用DNS</TD>
                      <TD class=table_title align=middle width=208>获取前缀</TD></TR>
                    <SCRIPT language=javascript>
						ListCreate_info3(6);
					</SCRIPT>
                    </TBODY></TABLE></TD>
                <TD width=10>&nbsp;</TD></TR>
              <TR>
                <TD width=10>&nbsp;</TD>
                <TD>
                  <HR>
                </TD>
                <TD width=10>&nbsp;</TD></TR></TBODY></TABLE></TD></TR>
        <TR>
          <TD vAlign=top width=157 bgColor=#e7e7e7 height=10></TD>
          <TD width=7 background=/img/panel3.gif>　</TD>
          <TD></TD></TR>
        <TR>
          <TD width=157 bgColor=#ef8218 height=30>
            <P class=Item_L1>DSL信息</P></TD>
          <TD width=7 bgColor=#ef8218>　</TD>
          <TD width=474>　</TD></TR>
        <TR>
          <TD vAlign=top width=157 bgColor=#e7e7e7 height=10></TD>
          <TD width=7 background=/img/panel3.gif>　</TD>
          <TD></TD></TR>
        <TR>
          <TD vAlign=top width=157 bgColor=#e7e7e7 height=30>
            <P class=Item_L2>&nbsp;</P></TD>
          <TD width=7 background=/img/panel3.gif>　</TD>
          <TD>
            <TABLE cellSpacing=0 cellPadding=0 width="100%" border=0>
              <TBODY>
              <TR>
                <TD width=10>&nbsp;</TD>
                <TD>
                  <DIV id=secDslInfo>
                  <TABLE cellSpacing=0 cellPadding=3 border=1>
                    <TBODY>
                    <TR>
                      <TD class=table_title width=208>DSL的当前同步状态:</TD>
                                <TD width=180> <script language=JavaScript type=text/javascript>
						if ( DslInfo.status == "NoSignal")
						{
						document.write("空闲");
						}
						else if( DslInfo.status == "Initializing")
						{
						document.write("握手");
						}
						else if( DslInfo.status == "EstablishingLink")
						{
						document.write("训练");
						}
						else if( DslInfo.status == "up")
						{
						document.write("激活");
						}
						else
						{
						document.write("未知");
						}
						</script>
                                  &nbsp;</TD>
                              </TR>
                    <TR>
                      <TD class=table_title>连接状态:</TD>
                      <TD>
                        <SCRIPT language=JavaScript type=text/javascript>
						if(DslInfo.status == "up")
						{
						document.write("激活");
						}
						else
						{
						document.write("未激活");
						}
						</SCRIPT>
                        &nbsp;</TD></TR></TBODY></TABLE></DIV>
                  <DIV id=secDslStat><BR>
                  <TABLE cellSpacing=0 cellPadding=3 border=1>
                    <SCRIPT language=javascript>
					document.write("<tr>");
					document.write("<td class='table_title' width='200'>线路速率 - 上行 (Kbps):</td>");
					document.write("<td width='180'>" + DslInfo.UpCurrRate + "</td>");
					document.write("</tr>" );
					document.write("<tr>");
					document.write("<td class='table_title'>线路速率 - 下行 (Kbps):</td>");
					document.write("<td>" + DslInfo.DownCurrRate + "</td>");
					document.write("</tr>");
					</SCRIPT>

                    <TBODY>
                    <TR>
                      <TD class=table_title width=208>噪声裕量上行 (dB):&nbsp;</TD>
                      <TD width=180>
                        <SCRIPT language=JavaScript type=text/javascript>
						document.write(DslInfo.UpMargin);
						</SCRIPT>
                        &nbsp;</TD></TR>
                    <TR>
                      <TD class=table_title>噪声裕量下行 (dB):&nbsp;</TD>
                      <TD>
                        <SCRIPT language=JavaScript type=text/javascript>
						document.write(DslInfo.DownMargin);
						</SCRIPT>
					</TD></TR>
						<SCRIPT language=javascript>
						var mode = DslInfo.Modulation;
						if (mode != "")
						{
						document.writeln("<tr><td class='table_title'><nobreak>交织深度上行:</nobreak></td>");
						document.writeln("<td>" + DslInfo.UpDepth + "&nbsp;</td></tr>");
						document.writeln("<tr><td class='table_title'><nobreak>交织深度下行:</nobreak></td>");
						document.writeln("<td>" + DslInfo.DownDepth + "&nbsp;</td></tr>");
						}
						</SCRIPT>

                    <TR>
                      <TD class=table_title>线路协议:</TD>
                      <TD>
                        <SCRIPT language=JavaScript type=text/javascript>
						if ( DslInfo.Encoding == "Auto Sync-Up")
						{
							document.write("Auto Sync-Up");
						}
						else if( DslInfo.Encoding == "ADSL2+")
						{
							document.write("ADSL2+");
						}
						else if( DslInfo.Encoding == "ADSL2")
						{
							document.write("ADSL2");
						}
						else if( DslInfo.Encoding == "G.DMT")
						{
							document.write("G.DMT");
						}
						else if( DslInfo.Encoding == "T1.413")
						{
							document.write("T1.413");
						}
						else if( DslInfo.Encoding == "G.lite")
						{
							document.write("G.lite");
						}
						else
						{
							document.write("未知");
						}
						</SCRIPT>
                        &nbsp;</TD></TR>
                    <TR>
                      <TD class=table_title>线路衰减上行:&nbsp;</TD>
                      <TD>
                        <SCRIPT language=JavaScript type=text/javascript>
						document.write(DslInfo.UpAttenuation);
						</SCRIPT>
                        &nbsp;</TD></TR>
                    <TR>
                      <TD class=table_title>线路衰减下行:&nbsp;</TD>
                      <TD>
                        <SCRIPT language=JavaScript type=text/javascript>
						document.write(DslInfo.DownAttenuation);
						</SCRIPT>
                        &nbsp;</TD></TR>
                    <TR>
                      <TD class=table_title>输出功率上行:&nbsp;</TD>
                                <TD> <script language=JavaScript type=text/javascript>
						document.write(DslInfo.UpPower);
						</script>
                                  &nbsp;</TD>
                              </TR>
                    <TR>
                      <TD class=table_title>输出功率下行:&nbsp;</TD>
                      <TD>
                        <SCRIPT language=JavaScript type=text/javascript>
						document.write(DslInfo.DownPower);
						</SCRIPT>
                        &nbsp;</TD></TR>
                    <TR>
                      <TD class=table_title>信道类型:&nbsp;</TD>
                      <TD>
                        <SCRIPT language=JavaScript type=text/javascript>
						document.write(DslInfo.DataPath);
						</SCRIPT>
                        &nbsp;</TD></TR></TBODY></TABLE></DIV></TD>
                <TD width=10>&nbsp;</TD></TR>
              <TR>
                <TD width=10>&nbsp;</TD>
                <TD>&nbsp;</TD>
                <TD width=10>&nbsp;</TD></TR></TBODY></TABLE></TD></TR>
        <TR>
          <TD vAlign=top width=157 bgColor=#e7e7e7 height=10></TD>
          <TD width=7 background=/img/panel3.gif>　</TD>
          <TD></TD></TR></TBODY></TABLE>
      <SCRIPT language=JavaScript type=text/javascript>
		if (curUserName != sptUserName)
		{
		tabOptionv4.style.display = "none";
		tabOptionv6.style.display = "none";
		secDslStat.style.display = "none";
		}
	  </SCRIPT>
    </TD></TR>
  <TR>
    <TD height=1>
      <TABLE id=table7 height=35 cellSpacing=0 cellPadding=0 width=808 
        border=0><TBODY>
        <TR>
          <TD width=162 bgColor=#ef8218>　</TD>
          <TD width=278 bgColor=#427594>　</TD>
          <TD width=196 bgColor=#427594>
            <P align=center></P></TD>
          <TD width=170 
bgColor=#313031>　</TD></TR></TBODY></TABLE></TD></TR></TBODY></TABLE></BODY></HTML>
