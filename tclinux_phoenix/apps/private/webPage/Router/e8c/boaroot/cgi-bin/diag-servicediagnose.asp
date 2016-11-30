<%
If Request_Form("linetest_commitflag") = "1" then
    TCWebApi_set("VoIPTest_Entry0","TestState","TestStateVal") 
    TCWebApi_set("VoIPTest_Entry0","TestSelector","TestSelectorVal") 
	TCWebApi_commit("VoIPTest")
elseif Request_Form("simulatetest_commitflag") = "1" then
    TCWebApi_set("VoIPTest_Entry0","TestState","TestStateSimVal") 
    TCWebApi_set("VoIPTest_Entry0","TestSelector","TestSelectorSimVal") 
    TCWebApi_set("VoIPSimulateTest_Entry0","CalledNumber","callednumber")
    TCWebApi_set("VoIPSimulateTest_Entry0","TestType","TestTypeSimVal")
	TCWebApi_commit("VoIPTest")
else
	TCWebApi_unset("VoIPTest")
End If

%>

<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0 Transitional//EN">
<HTML><HEAD><TITLE>业务诊断</TITLE>
<META http-equiv=Content-Language content=zh-cn>
<META http-equiv=Content-Type content="text/html; charset=gb2312"><LINK 
href="/JS/stylemain.css" type=text/css rel=stylesheet>

<SCRIPT language=javascript src="/JS/menu.js"></SCRIPT>

<SCRIPT language=javascript src="/JS/util.js"></SCRIPT>

<SCRIPT type="text/javascript" src="/JS/jquery.js"></SCRIPT>

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
        <TR>
          <TD class=LocationDisplay id=LocationDisplay align=middle width=163 
          bgColor=#ef8218 rowSpan=3></TD>
          <TD width=434 bgColor=#427594 height=33>
            <P align=right><FONT face=黑体 color=#ffffff><B><FONT face=黑体 
            color=#ffffff size=6><INPUT id=Selected_Menu type=hidden 
            value="诊断->业务诊断" name=Selected_Menu> </FONT></B><SPAN 
            class=GatewayName>网关名称:
            <SCRIPT language=javascript>
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

			function limitNum(o)
			{
				if (o.value != "")
				{
					if (!isInteger(o.value))
					{
						alert('"' + o.value + '"' + '只能输入数字.');	
						window.setTimeout( function(){ o.focus(); }, 0);
					}
				}
			}					

			function startLineTest_vDcDiff()
			{
			    document.LineTestForm.TestSelectorVal.value = "LineVoltageTest";
			    startLineTest();
				return true;
			}

			function startLineTest_rtr()
			{
				document.LineTestForm.TestSelectorVal.value = "LineResistiveTest";
			    startLineTest();
				return true;
			}

			function startLineTest()
			{
				setDisplay('LineTestResult', 0);
				document.LineTestForm.linetest_commitflag.value = "1";
				document.LineTestForm.TestStateVal.value = "Requested";
				document.getElementById("WaitMsgLineTest").className = " ";				
				document.LineTestForm.submit();
			}
			
			function startSimulateTest()
			{
				setDisplay('SimulateTestResults', 0);
				document.SimulateTestForm.simulatetest_commitflag.value = "1";
				document.SimulateTestForm.TestStateSimVal.value = "Requested";
				document.SimulateTestForm.TestSelectorSimVal.value = "X_CT-COM_SimulateTest";
				document.SimulateTestForm.TestTypeSimVal.value = "Caller";
				document.getElementById("WaitMsgSimulateTest").className = " ";
				document.SimulateTestForm.submit();
			}

		    function simGetData()
	        {
			   var simStatus =window.frames['simrefresh'].document.getElementById("simStatus").value;
			   var lineStatus =window.frames['simrefresh'].document.getElementById("lineStatus").value;
			   var simulateResult =window.frames['simrefresh'].document.getElementById("simulateResult").value;
			   var callerFaiReason =window.frames['simrefresh'].document.getElementById("callerFaiReason").value;
			   var failedResponseCode =window.frames['simrefresh'].document.getElementById("failedResponseCode").value;

			   $("#lineStatus").val(lineStatus);
			   $("#simulateResult").val(simulateResult);
			   $("#callerFaiReason").val(callerFaiReason);
			   $("#failedResponseCode").val(failedResponseCode);

				if ( lineStatus == 'Idle' )
		          	lineStatus = '空闲';
	          	else if ( lineStatus == 'Ringing' )
		          	lineStatus = '振铃';
	          	else if ( lineStatus == 'Dialing' )
		          	lineStatus = '拨号中';
	          	else if ( lineStatus == 'Ringback' )
		          	lineStatus = '听回铃音';
	          	else if ( lineStatus == 'Connect' )
		          	lineStatus = '已连接';
	          	else if ( lineStatus == 'Disconnect' )
		          	lineStatus = '断开连接';	
	          	else
		          	lineStatus = '--';
				
		       $("#SIM_LINESTATUS").html(lineStatus);			
		       $("#SIM_RESULT").html(simulateResult);
			   $("#SIM_FAILREASON").html(callerFaiReason);
		       $("#SIM_FAILCODE").html(failedResponseCode);

			 if (document.SimulateTestForm.simulatetest_TestType.value == "X_CT-COM_SimulateTest") {
			 	if(simStatus != "Testend") {
		          setTimeout(simFresh,1000);
				  document.getElementById("WaitMsgSimulateTest").className = " ";
				}
				else
				  document.getElementById("WaitMsgSimulateTest").className = "hidden";
				//  setDisplay('SimulateTestResults', 0);
			 }
		   }	

		   function simFresh()
		   {  
	          window.frames['simrefresh'].location.reload();
		   }
				
		</SCRIPT>

      <TABLE height="100%" cellSpacing=0 cellPadding=0 border=0>
        <TBODY>
        <TR>
          <TD width=157 bgColor=#ef8218 height=30>
            <P class=Item_L1>语音诊断</P></TD>
          <TD width=10 bgColor=#ef8218>　</TD>
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
                  href="/cgi-bin/help_content.asp#业务诊断" 
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
                  <FORM id="LineTestForm" name="LineTestForm" action="/cgi-bin/diag-servicediagnose.asp" method="post">
                  	<input type="hidden" name="linetest_commitflag" value="0">
                  	<input type="hidden" name="linetest_TestType" value="<%TCWebApi_get("VoIPTest_Entry0","TestSelector","s")%>">
                  	<input type="hidden" name="TestStateVal" value="0">
                  	<input type="hidden" name="TestSelectorVal" value="0">
					
                  <P><STRONG>线路测试</STRONG></P>


                  <TABLE border=1 cellSpacing=0 cellPadding=4 width="80%">
                    <TBODY>
                    <TR>
			        <TD width="25%" align="center" >外线电压： </TD>
			        <TD width="45%" align=right ><%If tcWebApi_get("VoIPTest_Entry0","LineTestData_vDcDiff","h") <> "N/A" Then tcWebApi_get("VoIPTest_Entry0","LineTestData_vDcDiff","s") end if%>
                        mV
                        &nbsp;</TD>
	                <TD align="center">
                       <INPUT onclick="startLineTest_vDcDiff()" type='button' value=' 应用 ' name="btnLineTest"> 
                    </TD> 
	                   
		            </TR>
                    </TBODY></TABLE>

                  <TABLE border=1 cellSpacing=0 cellPadding=4 width="80%">
                    <TBODY>
                    <TR>
			        <TD width="25%" align="center" >振铃阻抗： </TD>
			        <TD width="45%" align=right ><%If tcWebApi_get("VoIPTest_Entry0","LineTestData_rtr","h") <> "N/A" Then tcWebApi_get("VoIPTest_Entry0","LineTestData_rtr","s") end if%>
                        Kohm                      
                        &nbsp; </TD>
	                <TD align="center">
                       <INPUT onclick="startLineTest_rtr()" type='button' value=' 应用 ' name="btnLineTest"> 
                    </TD> 
	                   
		            </TR>
                    </TBODY></TABLE><BR>
                  
                  <DIV id=LineTestResult font-size:9px;>
				   <SCRIPT language=javascript>
					if (document.LineTestForm.linetest_TestType.value == "LineVoltageTest")
					{
							var result = "<%TCWebApi_get("VoIPTest_Entry0","TestState","s")%>" ;
							if(result != "Complete")
							{
								document.writeln("<B align='center'><font color='#FF0000' size='-1'>线路测试失败！</font></B>");
							}
					}

				  </SCRIPT></DIV>
				  <DIV id=WaitMsgLineTest class="hidden">
				  <B><font color="#FF0000" size="-1">正在进行线路测试,请稍等...</font></B>
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
          <TD vAlign=top width=157 bgColor=#e7e7e7 height=10></TD>
          <TD width=7 background=/img/panel3.gif>　</TD>
          <TD>
            <TABLE cellSpacing=0 cellPadding=0 width="100%" border=0>
              <TBODY>
              <TR>
                <TD width=10>&nbsp;</TD>
                <TD>
                  <FORM id="SimulateTestForm" name="SimulateTestForm" action="/cgi-bin/diag-servicediagnose.asp" method="post">
                    <input type="hidden" name="simulatetest_TestType" value="<%TCWebApi_get("VoIPTest_Entry0","TestSelector","s")%>">
                    <input type="hidden" name="simulatetest_commitflag" value="0">
                    <input type="hidden" name="TestStateSimVal" value="0">
                  	<input type="hidden" name="TestSelectorSimVal" value="0">    
                  	<input type="hidden" name="TestTypeSimVal" value="0">
                  	<input type="hidden" id="lineStatus" name="lineStatus" value="<% tcWebApi_get("VoIPSysParam_Entry0","SC_LINE_INFO_STATUS","s") %>"/>
                  	<input type="hidden" id="simulateResult" name="simulateResult" value="<% tcWebApi_get("VoIPSimulateTest_Entry0","Conclusion","s") %>"/>
                  	<input type="hidden" id="callerFaiReason" name="callerFaiReason" value="<% tcWebApi_get("VoIPSimulateTest_Entry0","CallerFailReason","s") %>"/>
                  	<input type="hidden" id="failedResponseCode" name="failedResponseCode" value="<% tcWebApi_get("VoIPSimulateTest_Entry0","FailedResponseCode","s") %>"/>

                  <P><STRONG>模拟主叫测试</STRONG></P>

                  <TABLE border=1 cellSpacing=0 cellPadding=4 width="80%">
                    <TBODY>
                    <TR>
			        <TD width="25%" align="center" >呼叫号码：</TD>
			        <TD width="40%" align="center" >
			          <LABEL> 
                      <input id="callednumber" name="callednumber" onblur="limitNum(this)" value=
					  "<% If TCWebApi_get("VoIPSimulateTest_Entry0","CalledNumber","h") <> "N/A" then
					  TCWebApi_get("VoIPSimulateTest_Entry0","CalledNumber","s")
					  end if%>" >
                      </LABEL> 
                    </TD>
	                <TD align="center">
                       <INPUT onclick="startSimulateTest()" type='button' value=' 测试 ' name="btnSimulateTest"> 
                    </TD> 
	                   
		            </TR>
                    </TBODY></TABLE>

                    <TABLE border=1 cellSpacing=0 cellPadding=4 width="80%">
                    <TBODY>
                    <TR>
			        <TD width="20%" align="center" >线路状态：</TD>
			        <TD width="60%" align=right ><span id="SIM_LINESTATUS" class="font_chara font_bold">--</span></TD>  	                   
		            </TR>
                    </TBODY></TABLE>

                    <TABLE border=1 cellSpacing=0 cellPadding=4 width="80%">
                    <TBODY>
                    <TR>
			        <TD width="20%" align="center" >仿真结果： </TD>
			        <TD width="60%" align=right ><span id="SIM_RESULT" class="font_chara font_bold">--</span></TD>                
		            </TR>
                    </TBODY></TABLE>

                    <TABLE border=1 cellSpacing=0 cellPadding=4 width="80%">
                    <TBODY>
                    <TR>
			        <TD width="20%" align="center" >失败原因： </TD>
			        <TD width="60%" align=right ><span id="SIM_FAILREASON" class="font_chara font_bold">--</span></TD>                 
		            </TR>
                    </TBODY></TABLE>

                    <TABLE border=1 cellSpacing=0 cellPadding=4 width="80%">
                    <TBODY>
                    <TR>
			        <TD width="20%" align="center" >错误代码： </TD>
			        <TD width="60%" align=right ><span id="SIM_FAILCODE" class="font_chara font_bold">--</span></TD>                
		            </TR>
                    </TBODY></TABLE>
                    
                  <DIV id="SimulateTestResults" font-size:9px;>
  				   <SCRIPT language=javascript>
					if (document.SimulateTestForm.simulatetest_TestType.value != "X_CT-COM_SimulateTest" )
					{
						document.SimulateTestForm.callednumber.value = "";		
					}

				  </SCRIPT></DIV>
				  <DIV id=WaitMsgSimulateTest class="hidden">
				  <B><font color="#FF0000" size="-1">正在进行模拟主叫测试,请稍等...</font></B>
			  	  </DIV>
			  	  <DIV class="clear" style='display:none'>
                  <iframe frameborder="0" id='simrefresh' name='simrefresh' onload='simGetData()' src='/cgi-bin/sysgwdata.asp'  >
                  </iframe>	
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
          <TD width=162 bgColor=#ef8218>　</TD>
          <TD width=278 bgColor=#427594>　</TD>
          <TD width=196 bgColor=#427594>
            <P align=center></P></TD>
          <TD width=170 
bgColor=#313031>　</TD></TR></TBODY></TABLE></TD></TR></TBODY></TABLE></BODY></HTML>
