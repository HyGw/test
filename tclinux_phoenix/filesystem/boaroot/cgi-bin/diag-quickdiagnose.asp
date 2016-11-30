<script language=JavaScript type=text/javascript>
var doManaulRep = 0;
</script>
<%
If Request_Form("ping_commitflag") = "1" then
	TCWebApi_set("WebCurSet_Entry","diag_pvc","ping_curSetIndex") 
	TCWebApi_set("Diagnostic_PVC","PingOtherIPaddr","ipaddress")
	TCWebApi_set("Diagnostic_PVC","PingOtherType","ping_PingOtherType") 
	TCWebApi_set("Diagnostic_PVC","PingOtherTimes","Repetitions") 
	TCWebApi_set("Diagnostic_PVC","TestType","ping_TestType")
	TCWebApi_set("Diagnostic_PVC","AddRoute","AddRoute_Ping")	
	TCWebApi_commit("Diagnostic_PVC")
	TCWebApi_commit("WebCurSet_Entry")
	
elseif Request_Form("Tracert_commitflag") = "1" then
	TCWebApi_set("WebCurSet_Entry","diag_pvc","Tracert_curSetIndex") 
	TCWebApi_set("Diagnostic_PVC","TraceIPaddr","ipaddress1")
	TCWebApi_set("Diagnostic_PVC","TestType","Tracert_TestType")
	TCWebApi_set("Diagnostic_PVC","AddRoute","AddRoute_Tracert")
	TCWebApi_commit("Diagnostic_PVC")
	TCWebApi_commit("WebCurSet_Entry")
	
elseif Request_Form("manual_commitflag") = "1" then
	TCWebApi_set("Cwmp_Entry","Manual_Inform","ManualInform")
	TCWebApi_commit("Cwmp_Entry")
	asp_Write("<script language=JavaScript type=text/javascript>doManaulRep=1;</script>")
	TCWebApi_unset("Diagnostic_PVC")

elseif Request_Form("report_commitflag") = "1" then
	TCWebApi_set("MonitorCollector_Common","Enable","reportvalue") 
	TCWebApi_commit("MonitorCollector")
	TCWebApi_set("PppoeEmulatorBridge_Entry","Enable","reportvalue") 
	TCWebApi_commit("PppoeEmulatorBridge")
else
	TCWebApi_unset("Diagnostic_PVC")
End If

%>

<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0 Transitional//EN">
<HTML><HEAD><TITLE>网络诊断</TITLE>
<META http-equiv=Content-Language content=zh-cn>
<META http-equiv=Content-Type content="text/html; charset=gb2312"><LINK 
href="/JS/stylemain.css" type=text/css rel=stylesheet>

<SCRIPT language=javascript src="/JS/menu.js"></SCRIPT>

<SCRIPT language=javascript src="/JS/util.js"></SCRIPT>
<style type="text/css">
.hidden
{
	display:none;
}
</style>
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
        <tr>
        	<TD width=808 bgColor=#d8d8d8 height=2 colSpan=3></td>
        	</tr>	
        <TR>
          <TD class=LocationDisplay id=LocationDisplay align=middle width=163 
          bgColor=#d8d8d8 rowSpan=3></TD>
          <TD width=434 bgColor=#0059A0 height=33>
            <P align=right><FONT face=黑体 color=#ffffff><B><FONT face=黑体 
            color=#ffffff size=6><INPUT id=Selected_Menu type=hidden 
            value="诊断->网络诊断" name=Selected_Menu> </FONT></B><SPAN 
            class=GatewayName>网关名称:
            <SCRIPT language=javascript>
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
		<SCRIPT language=javascript>
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
			var nEntryNum = <% tcWebApi_get("WanInfo_Common","CycleNum","s") %>;
			var vArrayStr = "<% tcWebApi_get("WanInfo_Common","CycleValue","s") %>";
			var vEntryName = vArrayStr.split(',');
			vArrayStr = "<% tcWebApi_get("WanInfo_Common","CycleValue","s") %>";
			var vEntryValue = vArrayStr.split(',');
			vArrayStr = "<% tcWebApi_get("WanInfo_Common","CycleValue","s") %>";
			var vEntryIndex = vArrayStr.split(',');
			vArrayStr = "<% tcWebApi_get("WanInfo_Common","CycleValue","s") %>";
			vArrayStr = "<% tcWebApi_get("WanInfo_Common","CycleValue","s") %>";
			vArrayStr = "<% tcWebApi_get("WanInfo_Common","CycleValue","s") %>";
			var vNasName = vArrayStr.split(',');
			
			//sort
			vEntryName = SortUtil.UpdateWanIFName( vEntryName, nEntryNum );
			vEntryValue = SortUtil.SortMyArray( vEntryValue );		
			vEntryIndex = SortUtil.SortMyArray( vEntryIndex );	
			vNasName = SortUtil.SortMyArray( vNasName );			
			
			var slIFNnme = "<% tcWebApi_get("Diagnostic_PVC","AddRoute","s") %>";			
			function listCreate(l_flag)
			{
					for (i = 0; i < nEntryNum; i++)
					{
						if(vEntryName[i].indexOf('_R_') > 0){
						if(slIFNnme == vEntryValue[i])
						{
							document.write('<option value=' + vEntryValue[i] + ' selected>'	+ vEntryName[i] + '</option>');
						}
						else
						{
							document.write('<option value=' + vEntryValue[i] + '>'	+ vEntryName[i] + '</option>');
						}
						}
					}
			}	 
			
			function inValidIPAddr(Address)
			{
				var address = Address.match("^[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}$");
				var digits;
				var i;
				
				if(address == null) { 
					return false;
				}
				
				digits = address[0].split(".");
				for(i=0; i < 4; i++)
				{
					if((Number(digits[i]) > 255 ) || (Number(digits[i]) < 0 ) || (Number(digits[0]) > 223) || (digits[i] == null))
					{ 
						return false;
					}
				}
				return true;
			}
			
			function isValidUrlName(url)
			{
				var i=0;
				var invalidArray = new Array();
				invalidArray[i++] = "www";
				invalidArray[i++] = "com";
				invalidArray[i++] = "org";
				invalidArray[i++] = "net";
				invalidArray[i++] = "edu";
				invalidArray[i++] = "www.";
				invalidArray[i++] = ".com";
				invalidArray[i++] = ".org";
				invalidArray[i++] = ".net";
				invalidArray[i++] = ".edu";
				if (isValidAscii(url) != '')
				{
					return false;
				}
				for (i = 0; i < url.length; i++)
				{
					if (url.charAt(i) == '\\')
					{
						return false;
					}
				}
				if (url == "")
				{
					return false;
				}
				if (url.length < 3)
				{
					return false;
				}
				for(j=0; j< invalidArray.length; j++)
				{
					if (url == invalidArray[j])
					{
						return false;
					}
				}
				return true;
			} 

			function LoadFrame()
			{
			}
			
			function startPing()
			{
				var ip_text = document.PingForm.ipaddress.value;
				var ip_calltimes = document.PingForm.Repetitions.value;
				if ((ip_calltimes > 10) || (ip_calltimes < 1) || (!isInteger(ip_calltimes)))
				{
					alert('重复次数非法! 请重新输入!');
					return false;
				}
			   if(inValidIPAddr(ip_text) )
				{
					setDisplay('PingResult', 0);
					document.PingForm.ping_PingOtherType.value = "Yes";
					document.PingForm.ping_commitflag.value = "1";
					document.PingForm.ping_TestType.value = "2";
					document.PingForm.AddRoute_Ping.value=getElById('pingCurrInterface').value;		
					document.getElementById("WaitMsgPing").className = " ";
					document.PingForm.submit();
				}
				else if(isValidUrlName(ip_text))
				{
					setDisplay('PingResult', 0);
					document.PingForm.ping_PingOtherType.value = "Yes";
					document.PingForm.ping_commitflag.value = "1";
					document.PingForm.ping_TestType.value = "2";
					document.PingForm.AddRoute_Ping.value=getElById('pingCurrInterface').value;		
					document.getElementById("WaitMsgPing").className = " ";
					document.PingForm.submit();
				}
				else
				{
					alert("目的地址非法! 请重新输入!");
					return false;
				}
			}
			
			function startTracert()
			{
				var tracert_text = document.TracertForm.ipaddress1.value;
			
				if(inValidIPAddr(tracert_text))
				{
					setDisplay('TracertResults', 0);
					document.TracertForm.Tracert_commitflag.value = "1";
					document.TracertForm.Tracert_TestType.value = "3";
					document.TracertForm.AddRoute_Tracert.value=getElById('tracertCurrInterface').value;
					document.getElementById("WaitMsgTracert").className = " ";
					document.TracertForm.submit();
				}
				else if(isValidUrlName(tracert_text))
				{
					setDisplay('TracertResults', 0);
					document.TracertForm.Tracert_commitflag.value = "1";
					document.TracertForm.Tracert_TestType.value = "3";
					document.TracertForm.AddRoute_Tracert.value=getElById('tracertCurrInterface').value;
					document.getElementById("WaitMsgTracert").className = " ";
					document.TracertForm.submit();
				
				}
				else
				{
					alert("目的地址非法! 请重新输入!");
					return false;
				}
			}

			function startInform()
			{
				document.ReportForm.manual_commitflag.value = "1";
				document.cookie = "DiagnosticPageStatus=1";
				document.ReportForm.submit();
			}
<% if tcWebApi_get("WebCustom_Entry","isCTMONITORCOLLECTORSupported","h") = "Yes" then %>			
			function report()
			{
				document.closeReportForm.report_commitflag.value = "1";
				document.closeReportForm.submit();
			}
<% end if %>
		</SCRIPT>

      <TABLE height="100%" cellSpacing=0 cellPadding=0 border=0>
        <TBODY>
        <TR>
          <TD width=157 bgColor=#e7e7e7 height=30>
            <P class=Item_L1>PING测试</P></TD>
          <TD width=10 bgColor=#e7e7e7>　</TD>
          <TD width=474>　</TD>
<% if tcWebApi_get("WebCustom_Entry","isCTMONITORCOLLECTORSupported","h") = "Yes" then %>	
          <TD vAlign=top width=170 background=/img/panel4.gif 
          rowSpan=9>
<% else %>
          <TD vAlign=top width=170 background=/img/panel4.gif 
          rowSpan=8>
<% end if %>
            <TABLE cellSpacing=0 cellPadding=20 width="100%" border=0 height='100%'>
              <TBODY>
              <TR>
                <TD height=74 valign='top'><A 
                  href="/cgi-bin/help_content.asp#网络诊断" 
                  target=_blank><IMG height=34 
                  src="/img/help.gif" width=40 border=0></A> 
              </TD></TR>
<% if tcwebApi_get("WebCustom_Entry","isCYE8SFUSupported","h") = "Yes" then %>
              <TR>
                <TD valign='bottom'><IMG src='/img/tybottom.jpg' height=76 width=112></TD></TR>
<% end if %>
              </TBODY></TABLE></TD></TR>
        <TR>
          <TD vAlign=top width=157 bgColor=#e7e7e7 height=30>
            <P class=Item_L2></P></TD>
          <TD width=7 background=/img/panel3.gif>　</TD>
          <TD>
            <TABLE cellSpacing=0 cellPadding=0 width="100%" border=0>
              <TBODY>
              <TR>
                <TD width=12>&nbsp;</TD>
                <TD width=480>
                  <FORM id="PingForm" name="PingForm" action="/cgi-bin/diag-quickdiagnose.asp" method="post">
           			<input type="hidden" name="ping_TestType" value="<%TCWebApi_get("Diagnostic_PVC","TestType","s")%>">
					<input type="hidden" name="ping_curSetIndex" value="0">
					<input type="hidden" name="ping_commitflag" value="0">
					<input type="hidden" name="ping_PingOtherType" value="<%TCWebApi_get("Diagnostic_PVC","PingOtherType","s")%>">
					<input type="hidden" name="AddRoute_Ping" value="">
					
                  <P><STRONG>PING测试</STRONG></P>
                  <TABLE cellSpacing=0 cellPadding=0 width="100%" border=0>
                    <TBODY>
                    <TR>
                        <TD>重复次数：&nbsp;&nbsp; <LABEL>
                                  <input id="Repetitions" name="Repetitions" value=
								  "<% If TCWebApi_get("Diagnostic_PVC","PingOtherTimes","h") <> "N/A"  then
									TCWebApi_get("Diagnostic_PVC","PingOtherTimes","s")
									end if%>" >
                                  </LABEL> (1-10次)</TD>
                              </TR>
                    <TR>
                      <TD>接口选择：&nbsp;&nbsp; <SELECT id="pingCurrInterface" 
                        size="1" name="pingCurrInterface" > 
                          <SCRIPT language=javascript>
						  listCreate(1);
						  </SCRIPT>
                        </SELECT> </TD></TR>
                    <TR>
                      <TD>目的地址：&nbsp;&nbsp; <LABEL><INPUT id="ipaddress" name="ipaddress" value=
					  "<% If TCWebApi_get("Diagnostic_PVC","PingOtherIPaddr","h") <> "N/A"  then
						TCWebApi_get("Diagnostic_PVC","PingOtherIPaddr","s")
						end if%>" ></LABEL><LABEL><INPUT onclick="startPing()" type="button" value="开始" name="btnPing"> 
                        </LABEL></TD></TR></TBODY></TABLE>
                  <DIV id=PingResult font-size:9px;>
				   <SCRIPT language=javascript>
					if (document.PingForm.ping_TestType.value == "2")
					{
							var result = "<%TCWebApi_get("Diagnostic_PVC","PingOther","s")%>";
							if(result == "PASS")
							{								
								document.writeln("<iframe src='/cgi-bin/getPingResult.cgi' frameborder='0' width='400' ></iframe>" );
							}
							else
							{
								document.writeln("<B align='center'><font color='#FF0000' size='-1'>Ping测试失败！</font></B>");
							}
					}
					else
					{
						document.PingForm.ipaddress.value = "";
						document.PingForm.Repetitions.value = "";
					}
				  </SCRIPT></DIV>
				  <DIV id=WaitMsgPing class="hidden">
				  <B><font color="#FF0000" size="-1">正在进行Ping测试,请稍等...</font></B>
			  	</DIV>
				  
                  <P><LABEL></LABEL></P></FORM></TD>
                <TD width=10>&nbsp;</TD></TR>
              <TR>
                <TD width=12>&nbsp;</TD>
                <TD>
                  <HR>
                </TD>
                <TD width=10>&nbsp;</TD></TR></TBODY></TABLE></TD></TR>
        <TR>
          <TD width=157 bgColor=#e7e7e7 height=30>
            <P class=Item_L1>Tracert测试</P></TD>
          <TD width=7 bgColor=#e7e7e7>　</TD>
          <TD width=474>　</TD></TR>
        <TR>
          <TD vAlign=top width=157 bgColor=#e7e7e7 height=10></TD>
          <TD width=7 background=/img/panel3.gif>　</TD>
          <TD>
            <TABLE cellSpacing=0 cellPadding=0 width="100%" border=0>
              <TBODY>
              <TR>
                <TD width=10>&nbsp;</TD>
                <TD>
                  <FORM id="TracertForm" name="TracertForm" action="/cgi-bin/diag-quickdiagnose.asp" method="post">
                    <input type="hidden" name="Tracert_TestType" value="<%TCWebApi_get("Diagnostic_PVC","TestType","s")%>">
  					<input type="hidden" name="Tracert_curSetIndex" value="0">
					<input type="hidden" name="Tracert_commitflag" value="0">
					<input type="hidden" name="Tracert_Ressult" value="<%TCWebApi_get("Diagnostic_PVC","TraceResult","s")%>">
					<input type="hidden" name="Tracert_PingOtherType" value="<%TCWebApi_get("Diagnostic_PVC","PingOtherType","s")%>">
					<input type="hidden" name="AddRoute_Tracert" value="">
                  <P><STRONG>Tracert测试</STRONG></P>
                  <TABLE cellSpacing=0 cellPadding=0 width="100%" border=0>
                    <TBODY>
                    <TR>
                      <TD>接口选择：&nbsp;&nbsp; <SELECT id="tracertCurrInterface" 
                        size="1" name="tracertCurrInterface"> 
                          <SCRIPT language=javascript>
						  listCreate(2);
						  </SCRIPT>
                        </SELECT> </TD></TR>
                    <TR>
                                <TD>目的地址：&nbsp;&nbsp; <LABEL> 
                                  <input id="ipaddress1" name="ipaddress1" value=
						"<% If TCWebApi_get("Diagnostic_PVC","TraceIPaddr","h") <> "N/A" then
						TCWebApi_get("Diagnostic_PVC","TraceIPaddr","s")
						end if%>" >
                                  </LABEL> 
                                  <LABEL><INPUT onclick="startTracert()" type="button" value="开始" name="btnTracert"> 
                        </LABEL></TD></TR></TBODY></TABLE>
                  <DIV id="TracertResults" name="" font-size:9px;>
  				   <SCRIPT language=javascript>
					if (document.TracertForm.Tracert_TestType.value == "3")
					{
							var result = "<%TCWebApi_get("Diagnostic_PVC","TraceResult","s")%>" ;
							if (result == "PASS")
							{	
								document.writeln("<iframe src='/cgi-bin/getTracentResult.cgi' frameborder='0' width='400' ></iframe>" );
							}
							else
							{
								document.writeln("<B><font color='#FF0000' size='-1'>Tracert测试失败！</font></B>");
							}

					}
					else
					{
						document.TracertForm.ipaddress1.value = "";
					}
					
				  </SCRIPT></DIV>
				            <DIV id=WaitMsgTracert class="hidden"> <B><font color="#FF0000" size="-1">正在进行Tracert测试,请稍等...</font> </B>
                            </DIV>
				  <BR></FORM>
                  <P></P></TD>
                <TD width=10>&nbsp;</TD></TR>
              <TR>
                <TD width=10>&nbsp;</TD>
                <TD>
                  <HR>
                </TD>
                <TD width=10>&nbsp;</TD></TR></TBODY></TABLE></TD></TR>
        <TR>
          <TD width=157 bgColor=#e7e7e7 height=30>
            <P class=Item_L1>Inform手动上报</P></TD>
          <TD width=7 bgColor=#e7e7e7>　</TD>
          <TD width=474>　</TD></TR>
        <TR>
          <TD vAlign=top width=157 bgColor=#e7e7e7 height=94></TD>
          <TD width=7 background=/img/panel3.gif>　</TD>
          <DIV id=InForm name="InForm"></DIV>
          <TD>
<% if tcWebApi_get("WebCustom_Entry", "isCTMONITORCOLLECTORSupported", "h") = "Yes"  then %>
          <TABLE cellSpacing=0 cellPadding=0 width="100%" height="100%" border=0>
              <TBODY>
              <TR>
                <TD width=10>&nbsp;</TD>
                <TD>
<% end if %>
        <FORM id="ReportForm" name="ReportForm" action="/cgi-bin/diag-quickdiagnose.asp" method="post">
		<input type="hidden" name="ManualInform" value="1">
		<input type="hidden" name="manual_commitflag" value="0">

		  <STRONG>Inform手动上报</STRONG>&nbsp;<INPUT onclick="startInform()" type="button" value="手动上报" name="btninform">
            <P>
            <P>
            <P>
            <DIV id=InformResults name="InformResults">
  				   <SCRIPT language=javascript>
				   function printResult()
				   {
				  		var result = "<%TCWebApi_get("Cwmp_Entry","Inform_Status","s")%>";
						switch(result)
						{
							case "0":
								document.writeln("Inform手动上报测试结果:未上报（e家终端正在启动）");
								break;
							case "1":
								document.writeln("Inform手动上报测试结果:未上报（无远程管理WAN连接）");
								break;
							case "2":
								document.writeln("Inform手动上报测试结果:未上报（远程管理WAN连接未生效）");
								break;
							case "3":
								document.writeln("Inform手动上报测试结果:未上报（无管理通道DNS信息）");
								break;
							case "4":
								document.writeln("Inform手动上报测试结果:未上报（无ACS配置参数）");
								break;
							case "5":
								document.writeln("Inform手动上报测试结果:未上报（ACS域名解析失败）");
								break;
							case "6":
								document.writeln("Inform手动上报测试结果:上报无回应");
								break;
							case "7":
								document.writeln("Inform手动上报测试结果:上报过程中断");
								break;
							case "8":
								document.writeln("Inform手动上报测试结果:上报成功");
								break;
							 default:
								document.writeln("Inform手动上报测试结果:无结果");
								break;			
						}	
				   }

				   function printWaitMsg()
				   {
					  document.writeln("<B><font color='#FF0000' size='-1'>正在手动上报,请稍等...</font></B>");
				   }
				   
				   function refreshPage()
				   {
						document.cookie = "DiagnosticPageStatus=2";
						document.ReportForm.submit();
				   }
				   
				   function disableCtl()
				   {		   
				   		setDisable("Repetitions",1);
				   		setDisable("pingCurrInterface",1);
				   		setDisable("ipaddress",1);
				   		setDisable("btnPing",1);
				   		setDisable("tracertCurrInterface",1);
				   		setDisable("ipaddress1",1);
				   		setDisable("btnTracert",1);
				   		setDisable("btninform",1);
				   }
				   
					function getPageStatus()
					{
						if(document.cookie.length > 0)
						{
							tstart = document.cookie.indexOf("DiagnosticPageStatus=");
							if( tstart != -1)
							{
								tstart += 21;
								tend = document.cookie.indexOf(";",tstart);
								if(tend == -1) tend = document.cookie.length;
								return document.cookie.substring(tstart,tend);
							}
						}
					}
					
					var pst = getPageStatus();
					if (pst == "1" && doManaulRep)
					{
						printWaitMsg();
						disableCtl();
						setTimeout(refreshPage,15000);						
					}
					else if(pst =="2")
					{
						printResult();
						document.cookie = "DiagnosticPageStatus=3";
					}
				  </SCRIPT>
			
			</DIV></FORM>
			</TD>
<% if tcWebApi_get("WebCustom_Entry", "isCTMONITORCOLLECTORSupported", "h") = "Yes"  then %>
			<TD width=10>&nbsp;</TD></TR>
			 <TR>
         		  <TD width=10>&nbsp;</TD>
         		 <TD>
            		     <HR>
          		 </TD>
       		  	 <TD width=10>&nbsp;</TD></TR></TBODY></TABLE></TD>
<% end if %>
          <DIV></DIV></TR>
        <TR>
<% if tcWebApi_get("WebCustom_Entry", "isCTMONITORCOLLECTORSupported", "h") = "Yes"  then %>
          <TD width=157 bgColor=#e7e7e7 height=30>
            <P class=Item_L1>智能诊断系统</P></TD>
          <TD width=7 bgColor=#e7e7e7>　</TD>
          <TD width=474>　</TD></TR>
        <TR>
          <TD vAlign=top width=157 bgColor=#e7e7e7 height=10></TD>
          <TD width=7 background=/img/panel3.gif>　</TD>
          <TD>
            <TABLE cellSpacing=0 cellPadding=0 width="100%" border=0>
              <TBODY>
              <TR>
                <TD width=10>&nbsp;</TD>
                <TD>
                  <FORM id="closeReportForm" name="closeReportForm" action="/cgi-bin/diag-quickdiagnose.asp" method="post"> 
                  <input type="hidden" name="report_commitflag" value="0">
                  <input type="hidden" name="reportvalue" value="0">
                  <TABLE cellSpacing=0 cellPadding=0 width="100%" border=0>
                    <TBODY>
                    <TR>
                      <TD>
                      	<P><STRONG>智能诊断系统URL</STRONG></P>
                </TD>
                      <TD><input type="text" disabled=true" name="url" id="url" value="<%if tcWebApi_get("MonitorCollector_Common","reportUrl","h") <> "N/A" then tcWebApi_get("MonitorCollector_Common","reportUrl","s") end if%>"></TD>
                      </TR>
                      </TBODY></TABLE>
                    <BR>

		   <LABEL>点击关闭上报按钮,e家终端将关闭向智能诊断系统上传状态信息 </LABEL>
                   <TABLE cellSpacing=0 cellPadding=0 width="100%" border=0>
                      <TR>
                    	 	<TD>当前状态：<% If TCWebApi_get("MonitorCollector_Common","Enable","h") = "1" then asp_Write("打开") else asp_Write("关闭") end if %></TD>
                      	<TD width=10>&nbsp;</TD>
                      </TR>
                      <TR>
                      	<TD colspan="2" align="center">
                      		<input type="button" name="closereport" id="closereport" value="关闭上报" onclick="report()">
                      	</TD>
                    	</TR>
                   </TBODY></TABLE>   
				  <BR></FORM>
                </TD>
                <TD width=10>&nbsp;</TD></TR></TBODY></TABLE></TD></TR>
        <TR>
<% end if %>
          <TD vAlign=top width=157 bgColor=#e7e7e7 height=10></TD>
          <TD width=7 background=/img/panel3.gif>　</TD>
          <TD>
            <TABLE cellSpacing=0 cellPadding=0 width="100%" border=0>
              <TBODY>
              <TR>
                <TD width=10>&nbsp;</TD>
<% if tcWebApi_get("WebCustom_Entry","isCTMONITORCOLLECTORSupported","h") = "Yes" then %>	
                <TD></TD>
<% else %> 
                <TD><HR></TD>
<% end if %>
                <TD 
  width=10>&nbsp;</TD></TR></TBODY></TABLE></TD></TR></TBODY></TABLE></TD></TR>
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
