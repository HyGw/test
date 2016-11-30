<%
If request_Form("VoIP_h248_flag")="1" then
	tcWebApi_set("VoIPH248_Common", "DeviceIDType", "mgregselect")	
	tcWebApi_set("VoIPH248_Common", "DeviceID", "mymgname")	
	tcWebApi_set("VoIPH248_Common", "MediaGatewayPort", "mymgport")
	tcWebApi_set("VoIPH248_Common", "MediaGatewayControler", "mymgcip")
	tcWebApi_set("VoIPH248_Common", "MediaGatewayControlerPort", "mymgcport")
	tcWebApi_set("VoIPH248_Common", "SBMediaGatewayControler", "mybkmgcip")	
	tcWebApi_set("VoIPH248_Common", "SBMediaGatewayControlerPort", "mybkmgcport")	
	tcWebApi_set("VoIPH248_Common", "MediaGatewayDNS", "myNewworkDNS")				
	tcWebApi_set("VoIPH248_Common", "RTPPrefix", "mgRTPPrefix")	
	tcWebApi_set("VoIPH248_Common", "EphemeralTermIDAddLen", "mgEpTermIDLen")	
	tcWebApi_set("VoIPH248_Common", "EphemeralTermIDUniform", "mgEpTermIDUniform")	
	tcWebApi_set("VoIPH248_Common", "EphemeralTermIDStart", "mgEpTermIDStart")	
	tcWebApi_set("VoIPH248_Common", "MegacoSignalDSCP", "mymgtos")
	tcWebApi_set("VoIPH248_Common", "MegacoRTPDSCP", "mymgtos2")	
	tcWebApi_set("VoIPH248_Entry0", "PhysicalTermID", "mymgterm0")	
	tcWebApi_set("VoIPH248_Entry1", "PhysicalTermID", "mymgterm1")			
	tcWebApi_commit("VoIPH248_Common")
	tcWebApi_commit("VoIPH248")
	tcWebApi_save()
end if
%>
<HTML><HEAD><TITLE>VOIP设置</TITLE>
<META http-equiv=Content-Language content=zh-cn>
<META http-equiv=Content-Type content="text/html; charset=gb2312"><LINK 
href="/JS/stylemain.css" type=text/css rel=stylesheet>
<SCRIPT language=javascript src="/JS/menu.js"></SCRIPT>
<SCRIPT language=javascript src="/JS/util.js"></SCRIPT>
<SCRIPT language="javascript" type = "text/javascript">
function LoadFrame() 
{
	with (document.ConfigForm)
	{
		if(mgRTPPrefix.value != "")
			mgrtpstartid.value = mgRTPPrefix.value + mgEpTermIDStart.value;
			
		generatePerLineTable1();
	}
}
		

function generatePerLineTable1()
{
   if (document.body.innerHTML) 
   {
      var tableHtmlOutput = "<tr>";
	  tableHtmlOutput += "<td>";
	  tableHtmlOutput += "<table width='391' border='1' cellpadding='0' cellspacing='0' bordercolor='#C0C0C0' style='border-collapse: collapse'>";
      tableHtmlOutput += "<tr align='left'>";
      tableHtmlOutput += "<td height='25' colspan='3' ><strong id='in_h_resource1'>资源</strong></td>";
	  tableHtmlOutput += "</tr>";
	  
	  tableHtmlOutput += "<tr align='center'>";
	  tableHtmlOutput += "<td width='162' height='25' bgcolor='eeeeee'>&nbsp;</td>"
	  	
	  for (var mgline = 1; mgline <= 2; mgline++) 
      {
         var data = "<td width='162' height='25' bgcolor='#EEEEEE' id=\'in_h_line" + mgline+ "\'>线路"+mgline+"</td>";
         tableHtmlOutput += data;
      }
	  
	  tableHtmlOutput += "</tr>";
	  tableHtmlOutput += "<tr align='center'>";
	  tableHtmlOutput += "<td height='25' bgcolor='eeeeee' id='in_h_term1'>终端</td>";
	  
  	var data = "<input name=\'mymgterm0\' type='text' id=\'in_h_term1_val0\' value=\'<%if tcWebApi_get("VoIPH248_Entry0", "PhysicalTermID", "h") <> "N/A" then tcWebAPI_get("VoIPH248_Entry0", "PhysicalTermID", "s") else asp_Write("") end if %>\'>";
  	tableHtmlOutput += "<td height='25'>" + data + "</td>";
  	var data = "<input name=\'mymgterm1\' type='text' id=\'in_h_term1_val1\' value=\'<%if tcWebApi_get("VoIPH248_Entry1", "PhysicalTermID", "h") <> "N/A" then tcWebAPI_get("VoIPH248_Entry1", "PhysicalTermID", "s") else asp_Write("") end if %>\'>";
  	tableHtmlOutput += "<td height='25'>" + data + "</td>";
	  
	  tableHtmlOutput += "</tr>";
	  tableHtmlOutput += "</table>";
	  tableHtmlOutput += "</td>";
	  tableHtmlOutput += "</tr>";  
      
      document.getElementById("perLineTablePlaceholder2").innerHTML = tableHtmlOutput;
   }
   else 
   {
      alert("Cannot create per line table in this browser.");
   }
}		
		
function applyClickAdvanced()
{
   var loc = "app-VoIP_Adv.asp";
   var code = "location='" + loc + " '";
   eval(code);
}

function VoiceApplyBasic()
{
	with (document.ConfigForm)
	{
   	if(mymgcip.value != "")
   	{
			if (isValidIpAddress(mymgcip.value) == false)
			{
				alert('"服务器地址' + mymgcip.value + '"是无效IP地址.');	
				return;
			}
		}

		if (mybkmgcip.value != "")
		{	
			if (isValidIpAddress(mybkmgcip.value) == false)
			{
				alert('"备用服务器地址' + mybkmgcip.value + '"是无效IP地址.');	
				return;
			}			
		}	
	
		if (myNewworkDNS.value != "")
		{	
			if (isValidIpAddress(myNewworkDNS.value) == false)
			{
				alert('"DNS' + myNewworkDNS.value + '"是无效IP地址.');	
				return;
			}			
		}		
		
		if (mymgname.value != "")
		{
			if (mgregselect.selectedIndex == "0")
			{
				if (isValidIpAddress(mymgname.value) == false)
				{
					alert('"' + mymgname.value + '"是无效IP地址.');	
					return;
				}		
			}
					
			if (mgregselect.selectedIndex == "2")
			{
				if (isValidMacAddress(mymgname.value) == false)
				{
					alert('"' + mymgname.value + '"是无效MAC地址.');	
					return;
				}		
			}
			
			if(mgrtpstartid.value == "")
			{
				mgRTPPrefix.value = "";
				mgEpTermIDLen.value = "0";
				mgEpTermIDUniform.value = "0";
				mgEpTermIDStart.value = "0";
			}
			else
			{
				var tempVal = mgrtpstartid.value.split("/");
				if(tempVal.length != 2)
				{
					alert('起始临时端点格式错误.');
					return;				
				}
				
				if (!isInteger(tempVal[1]))
				{
					alert('起始临时端点内容错误.');
					return;					
				}
				
				mgRTPPrefix.value = tempVal[0] + "/";
				mgEpTermIDLen.value = tempVal[1].length;
				mgEpTermIDUniform.value = "0";
				mgEpTermIDStart.value = tempVal[1];
			}
		}
		
		var temp = parseInt(mymgtos.value);	
		if (temp < 0 || temp > 63)
		{
			alert('TOS/DSCP 信令优先级的范围是0~63.');
			return;
		}
		
		var temp = parseInt(mymgtos2.value);	
		if (temp < 0 || temp > 63)
		{
			alert('TOS/DSCP RTP优先级的范围是0~63.');
			return;
		}		
		
		if (mymgcip.value != "")
		{
			if (mymgcport.value == "")
			{
				alert('服务器端口号不能为空.');
				return;
			}
		}
		else
		{
			if (mymgcport.value != "")
			{
				alert('服务器地址不能为空.');
				return;
			}
		}
		
		if (mybkmgcip.value != "")
		{
			if (mybkmgcport.value == "")
			{
				alert('备用服务器端口号不能为空.');
				return;
			}
		}
		else
		{
			if (mybkmgcport.value != "")
			{
				alert('备用服务器地址不能为空.');
				return;
			}
		}		
				
		VoIP_h248_flag.value = 1;
		submit();
	}
}

function checkPort(o)
{
	if (o.value != "")
	{
		if (!isInteger(o.value) || !isValidPort(o.value))
		{
			alert('"' + o.value + '"' + '是无效端口.');	
			window.setTimeout( function(){ o.focus(); }, 0);
		}
	}
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
          <TD width=434 bgColor=#427594 height=33 class=CucBgColorTD_1>
            <P align=right><FONT face=黑体 color=#ffffff><B><FONT face=黑体 
            color=#ffffff size=6><INPUT id=Selected_Menu type=hidden 
            value="应用->VOIP设置" name=Selected_Menu> </FONT></B><SPAN 
            class=GatewayName>网关名称:
            <SCRIPT 
            language=javascript>
document.write(top.gateWayName);
</SCRIPT>
             </SPAN></FONT></P></TD>
          <TD width=211 bgColor=#ef8218 height=33 class=CucBgColorTD_1>
            <P class=GatewayType align=center>型号:
            <SCRIPT language=javascript>
document.write(top.ModelName);
</SCRIPT>
             </P></TD></TR>
        <TR>
          <TD id=MenuArea_L1 vAlign=bottom bgColor=#ef8218 colSpan=2 
          height=43 class=CucBgColorTD_1>&nbsp;</TD></TR>
        <TR>
          <TD id=MenuArea_L2 bgColor=#427594 colSpan=2 
      height=24 class=CucBgColorTD_3></TD></TR></TBODY></TABLE>
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
      <TABLE height="100%" cellSpacing=0 cellPadding=0 border=0>
        <TBODY>
        <TR>
          <TD class=Item_L1 width=157 bgColor=#ef8218 height=30>
            <P>VOIP设置</P></TD>
          <TD width=7 bgColor=#ef8218 class=CucBgColorTD_2>　</TD>
          <TD width=474>　</TD>
          <TD vAlign=top width=170 background=/img/panel4.gif 
            rowSpan=13><TABLE cellSpacing=0 cellPadding=20 width="100%" 
border=0>
              <TBODY>
              <TR>
                <TD><A 
                  href="/cgi-bin/help_content.asp#宽带电话设置" 
                  target=_blank><IMG height=34 src="/img/help.gif" 
                  width=40 border=0> </A></TD></TR></TBODY></TABLE></TD></TR>
        <TR>
          <TD vAlign=top width=157 bgColor=#e7e7e7 height=10></TD>
          <TD width=7 background=/img/panel3.gif>　</TD>
          <TD></TD></TR>
        <TR>
          <TD vAlign=top width=157 bgColor=#e7e7e7 height=30>
            <P class=Item_L2></P></TD>
          <TD width=7 background=/img/panel3.gif>　</TD>
          <TD>
            <TABLE cellSpacing=0 cellPadding=0 width="100%" border=0>
              <TBODY>
              <TR>
                <TD width=10>&nbsp;</TD>
                <TD>
                  <FORM id=ConfigForm name="ConfigForm" action="/cgi-bin/app-VoIP.asp" method="post">
                 		<DIV id=pptp>
                  		<TABLE border=0 cellSpacing=0 cellPadding=0 width="100%">
										     <TBODY>
							                <TR>
							                    <TD>
							                        <input type='button' style="font-size:100%;width:170;height:30" onClick="applyClickAdvanced()" value='宽带电话高级设置'>
							                    </TD>
							                </TR>
							                <TR>
							                    <TD height="10">
							                        
							                    </TD>
							                </TR>
							                <TR>
							                    <TD class=table1_head>
							                    	<B>
							                        宽带电话语音基本设置
							                      </B>
							                    </TD>
							                </TR>
							                <TR>
							                    <TD height="10">
							                        
							                    </TD>
							                </TR>
							            </TBODY>
								        </TABLE>
								        <TABLE border=0 cellSpacing=0 cellPadding=0 width="100%">
								            <TBODY>
								                <TR>
								                    <TD class=prompt>
								                        <TABLE border=0 cellSpacing=0 cellPadding=0 width="100%">
								                            <TBODY>
								                                <TR>
								                                    <TD style="PADDING-LEFT: 10px" width="100%"></TD>
								                                </TR>
								                            </TBODY>
								                        </TABLE>
								                    </TD>
								                </TR>
								            </TBODY>
								        </TABLE>
								        <TABLE border=0 cellSpacing=0 cellPadding=0 width="100%">
								            <TBODY>
								                <TR>
								                    <TD height=5></TD>
								                </TR>
								            </TBODY>
							       		</TABLE>
								        <TABLE class=table1_bg border=0 cellSpacing=1 cellPadding=0 width="98%">
								            <TBODY>
								                <TR>
								                    <TD class=table1_left width="35%" align=left>
								                        MG注册方式
								                    </TD>
								                    <TD class=table1_right width="65%" colSpan=2 align=left>
								                  
										                        <select name="mgregselect" id="mg_regist">
						                                    <option value="0" <%if tcWebApi_get("VoIPH248_Common", "DeviceIDType", "h") = "0" then asp_Write("selected") end if %>>IP</option>
						                                    <option value="1" <%if tcWebApi_get("VoIPH248_Common", "DeviceIDType", "h") = "1" then asp_Write("selected") end if %>>Domain name</option>             
						                                    <option value="2" <%if tcWebApi_get("VoIPH248_Common", "DeviceIDType", "h") = "2" then asp_Write("selected") end if %>>MAC</option>
						                                </select>
						                            
						                              	<input name="mymgname" type="text" id="in_h_domain1_val" value="<%if tcWebApi_get("VoIPH248_Common", "DeviceID", "h") <> "N/A" then tcWebAPI_get("VoIPH248_Common", "DeviceID", "s") else asp_Write("") end if %>">
						                       
								                    </TD>
								                </TR>
								                <TR>
								                    <TD class=table1_left align=left>
								                        MG端口号
								                    </TD>
								                    <TD class=table1_right colSpan=2 align=left>
								                        <input name="mymgport" type="text" id="in_h_mg1_val" onblur="checkPort(this)" value="<%if tcWebApi_get("VoIPH248_Common", "MediaGatewayPort", "h") <> "N/A" then tcWebAPI_get("VoIPH248_Common", "MediaGatewayPort", "s") else asp_Write("") end if %>">
								                    </TD>
								                </TR>
								                <TR>
								                    <TD class=table1_left align=left>
								                        服务器地址
								                    </TD>
								                    <TD class=table1_right colSpan=2 align=left>
								                        <input name="mymgcip" type="text" id="in_h_servaddr1_val" value="<%if tcWebApi_get("VoIPH248_Common", "MediaGatewayControler", "h") <> "N/A" then tcWebAPI_get("VoIPH248_Common", "MediaGatewayControler", "s") else asp_Write("") end if %>">
								                    </TD>
								                </TR>
								                <TR>
								                    <TD class=table1_left align=left>
								                        服务器端口号
								                    </TD>
								                    <TD class=table1_right colSpan=2 align=left>
								                        <input name="mymgcport" type="text" id="in_h_servnum1_val" onblur="checkPort(this)" title=an_tos2 value="<%if tcWebApi_get("VoIPH248_Common", "MediaGatewayControlerPort", "h") <> "N/A" then tcWebAPI_get("VoIPH248_Common", "MediaGatewayControlerPort", "s") else asp_Write("") end if %>">
										                </TD>
										            </TR>
								                <TR>
								                    <TD class=table1_left align=left>
								                        备用服务器地址
								                    </TD>
								                    <TD class=table1_right colSpan=2 align=left>
								                        <input name="mybkmgcip" type="text" id="in_h_standbyaddr1_val" value="<%if tcWebApi_get("VoIPH248_Common", "SBMediaGatewayControler", "h") <> "N/A" then tcWebAPI_get("VoIPH248_Common", "SBMediaGatewayControler", "s") else asp_Write("") end if %>">
								                    </TD>
								                </TR>
								                <TR>
								                    <TD class=table1_left align=left>
								                        备用服务器端口号
								                    </TD>
								                    <TD class=table1_right colSpan=2 align=left>
								                        <input name="mybkmgcport" type="text" id="in_h_standbynum1_val" onblur="checkPort(this)" value="<%if tcWebApi_get("VoIPH248_Common", "SBMediaGatewayControlerPort", "h") <> "N/A" then tcWebAPI_get("VoIPH248_Common", "SBMediaGatewayControlerPort", "s") else asp_Write("") end if %>">
								                    </TD>
								                </TR>
								                <TR>
								                    <TD class=table1_left align=left>
								                        DNS
								                    </TD>
								                    <TD class=table1_right colSpan=2 align=left>
								                        <input name="myNewworkDNS" type="text" id="in_h_dns1_val" value="<%if tcWebApi_get("VoIPH248_Common", "MediaGatewayDNS", "h") <> "N/A" then tcWebAPI_get("VoIPH248_Common", "MediaGatewayDNS", "s") else asp_Write("") end if %>">
								                    </TD>
								                </TR>
								                <TR>
								                    <TD class=table1_left align=left>
								                        起始临时端点
								                    </TD>
								                    <TD class=table1_right colSpan=2 align=left>
								                        <input name="mgrtpstartid" type="text" id="in_h_in_tem_ep1_val" value="">
								                        <input type="hidden" name="mgRTPPrefix" value="<%if tcWebApi_get("VoIPH248_Common", "RTPPrefix", "h") <> "N/A" then tcWebAPI_get("VoIPH248_Common", "RTPPrefix", "s") else asp_Write("") end if %>">
								                        <input type="hidden" name="mgEpTermIDLen" value="<%if tcWebApi_get("VoIPH248_Common", "EphemeralTermIDAddLen", "h") <> "N/A" then tcWebAPI_get("VoIPH248_Common", "EphemeralTermIDAddLen", "s") else asp_Write("") end if %>">
								                        <input type="hidden" name="mgEpTermIDUniform" value="<%if tcWebApi_get("VoIPH248_Common", "EphemeralTermIDUniform", "h") <> "N/A" then tcWebAPI_get("VoIPH248_Common", "EphemeralTermIDUniform", "s") else asp_Write("") end if %>">
								                        <input type="hidden" name="mgEpTermIDStart" value="<%if tcWebApi_get("VoIPH248_Common", "EphemeralTermIDStart", "h") <> "N/A" then tcWebAPI_get("VoIPH248_Common", "EphemeralTermIDStart", "s") else asp_Write("") end if %>">
								                    </TD>
								                </TR>
								                <TR>
								                    <TD class=table1_left align=left>
								                        TOS/DSCP优先级(信令)
								                    </TD>
								                    <TD class=table1_right colSpan=2 align=left>
								                        <input name="mymgtos" type="text" id="in_h_tos1_val" value="<%if tcWebApi_get("VoIPH248_Common", "MegacoSignalDSCP", "h") <> "N/A" then tcWebAPI_get("VoIPH248_Common", "MegacoSignalDSCP", "s") else asp_Write("") end if %>">
								                    </TD>
								                </TR>
								                <TR>
								                    <TD class=table1_left align=left>
								                        TOS/DSCP优先级(RTP)
								                    </TD>
								                    <TD class=table1_right colSpan=2 align=left>
								                        <input name="mymgtos2" type="text" id="in_h_tos2_val" value="<%if tcWebApi_get("VoIPH248_Common", "MegacoRTPDSCP", "h") <> "N/A" then tcWebAPI_get("VoIPH248_Common", "MegacoRTPDSCP", "s") else asp_Write("") end if %>">
								                    </TD>
								                </TR>
										          </TBODY>
										        </TABLE>
										        <TABLE border=0 cellSpacing=1 cellPadding=0 width="98%">
										        	<tr>
										        		<td height="5">
										        		</td>
										        	</tr>
										        	<tr>
							                    <td><div class="tableWrapper" id="perLineTablePlaceholder2" ></div></td>
							                </tr> 
							                <tr>
										        		<td height="5">
										        		</td>
										        	</tr>
										        	<tr>
					                        <td align="center">
					                          <input name="VoIP_h248_flag" type="hidden" value="0">
					                          <input name="bt_apply" type="button"  id="VoiceApplyButton"  value="应用" onClick="VoiceApplyBasic()">
					                        </td>
					                    </tr>
										        </TABLE>
										      </DIV>
                    </FORM>
                  </TD>
                </TR>
              </TBODY>
            </TABLE>
          </TD>
        </TR>      
        <TR>
          <TD vAlign=top width=157 bgColor=#e7e7e7 height=30>
            <P class=Item_L2></P></TD>
          <TD width=7 background=/img/panel3.gif>　</TD>
          <TD>&nbsp;</TD></TR>
        <TR>
          <TD vAlign=top width=157 bgColor=#e7e7e7 height=10></TD>
          <TD width=7 background=/img/panel3.gif>　</TD>
          <TD></TD></TR>
        <TR>
          <TD vAlign=top width=157 bgColor=#e7e7e7></TD>
          <TD width=7 background=/img/panel3.gif>　</TD>
          <TD></TD></TR></TBODY></TABLE>
      </TD></TR>
  <TR>
    <TD height=1>
      <TABLE id=table7 height=35 cellSpacing=0 cellPadding=0 width=808 
        border=0><TBODY>
        <TR>
          <TD width=162 bgColor=#ef8218 class=CucBgColorTD_2>　</TD>
          <TD width=278 bgColor=#427594 class=CucBgColorTD_2>　</TD>
          <TD width=196 bgColor=#427594 class=CucBgColorTD_2><!-- InstanceEndEditable --></TD>
          <TD width=170 
bgColor=#313031  class=CucBgColorTD_2>　</TD></TR></TBODY></TABLE></TD></TR></TBODY></TABLE></BODY></HTML>
