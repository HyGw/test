<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0 Transitional//EN">
<HTML>
    
    <HEAD>
        <TITLE>
            宽带语音信息
        </TITLE>
        <META http-equiv=Content-Language content=zh-cn>
        <META content="MSHTML 6.00.6000.16809" name=GENERATOR>
        <META http-equiv=Content-Type content="text/html; charset=gb2312">
        <LINK href="/JS/stylemain.css" type=text/css rel=stylesheet>
        <SCRIPT language=javascript src="/JS/menu.js"></SCRIPT>
        <SCRIPT language=javascript src="/JS/util.js"></SCRIPT>
        <SCRIPT language=JavaScript src="/JS/printtable.js"></SCRIPT>
        <SCRIPT language=JavaScript type=text/javascript>
<% if tcWebApi_get("WebCustom_Entry", "isCTCUCSIPH248Supported", "h") = "Yes"  then %>
	<% if tcWebApi_get("VoIPBasic_Common", "SIPProtocol", "h") = "H.248"  then %>
		location.replace("/cgi-bin/sta-VoIP248.asp");
	<% end if %>
<% end if %>
          function VoipInfoClass()
          {
              this.Reg1Status = '<%if tcWebAPI_get("InfoVoIP_Entry0", "Status", "h") <> "N/A" then tcWebAPI_get("InfoVoIP_Entry0", "Status", "s") else asp_Write("") end if %>';
              this.Line1Status = '<%if tcWebApi_get("VoIPSysParam_Entry0", "SC_LINE_INFO_STATUS", "h") <> "N/A" then tcWebAPI_get("VoIPSysParam_Entry0", "SC_LINE_INFO_STATUS", "s") else asp_Write("") end if %>';
              <% if tcwebApi_get("VoIPBasic_Common","VoIPLine2Enable","h") = "Yes" then %>
              this.Reg2Status = '<%if tcWebAPI_get("InfoVoIP_Entry1", "Status", "h") <> "N/A" then tcWebAPI_get("InfoVoIP_Entry1", "Status", "s") else asp_Write("") end if %>';
              this.Line2Status = '<%if tcWebApi_get("VoIPSysParam_Entry1", "SC_LINE_INFO_STATUS", "h") <> "N/A" then tcWebAPI_get("VoIPSysParam_Entry1", "SC_LINE_INFO_STATUS", "s") else asp_Write("") end if %>';
              <% end if %>
              var line1number='<%if tcWebApi_get("VoIPBasic_Common","SIPProtocol", "h") = "SIP" then asp_Write("") elseif tcWebAPI_get("VoIPBasic_Entry0", "SIPDisplayName", "h") <> "N/A" then tcWebAPI_get("VoIPBasic_Entry0", "SIPDisplayName", "s") else asp_Write("") end if %>';
              if(line1number == ''){
              	var tmp='<%if tcWebAPI_get("VoIPBasic_Entry0", "SIPAuthenticationName", "h") <> "N/A" then tcWebAPI_get("VoIPBasic_Entry0", "SIPAuthenticationName", "s") else asp_Write("") end if %>';
               	line1number=tmp.substr(0,tmp.indexOf('@'));
              }
              this.Line1Number = line1number;
              <% if tcwebApi_get("VoIPBasic_Common","VoIPLine2Enable","h") = "Yes" then %>
              var line2number = '<%if tcWebApi_get("VoIPBasic_Common","SIPProtocol", "h") = "SIP" then asp_Write("") elseif tcWebAPI_get("VoIPBasic_Entry1", "SIPDisplayName", "h") <> "N/A" then tcWebAPI_get("VoIPBasic_Entry1", "SIPDisplayName", "s") else asp_Write("") end if %>';
              if(line2number == ''){
              	var tmp2='<%if tcWebAPI_get("VoIPBasic_Entry1", "SIPAuthenticationName", "h") <> "N/A" then tcWebAPI_get("VoIPBasic_Entry1", "SIPAuthenticationName", "s") else asp_Write("") end if %>';
              	line2number=tmp2.substr(0,tmp2.indexOf('@'));
              }
              this.Line2Number = line2number;
              <% end if %> 
              this.showRegisterState = showRegisterState;
              this.showVoipNumber = showVoipNumber;
              this.showLineState = showLineState;
          }

          function showRegisterState(port)
          {
          	var Status = '';
          	var LineEnable='';

          	if ( 1 == port ){
          		Status = this.Reg1Status;
          		LineEnable = '<%if tcWebAPI_get("VoIPBasic_Entry0", "Enable", "h") <> "N/A" then tcWebAPI_get("VoIPBasic_Entry0", "Enable", "s") else asp_Write("") end if %>';

          	}
          	<% if tcwebApi_get("VoIPBasic_Common","VoIPLine2Enable","h") = "Yes" then %>
          	else if ( 2 == port ){
          		Status = this.Reg2Status;
          		LineEnable = '<%if tcWebAPI_get("VoIPBasic_Entry1", "Enable", "h") <> "N/A" then tcWebAPI_get("VoIPBasic_Entry1", "Enable", "s") else asp_Write("") end if %>';
          	}
          	<% end if %>
          	else
          		return;
						if(LineEnable == 'No'){
							document.write('去使能');
							return;
						}
						if ( Status == 'Up' )
							document.write('已注册');
						else if ( Status == 'Initializing' )
							document.write('初始化中');
						else if ( Status == 'Registering' )
							document.write('注册中');
						else if ( Status == 'Unregistering' )
							document.write('未注册');
						else if ( Status == 'Quiescent' )
							document.write('停止的');
						else if ( Status == 'Disabled' )
							document.write('去使能');
						else if ( Status == 'Error' )
							document.write('故障');
						else if ( Status == 'Testing' )
							document.write('测试中');
						else
							document.write('--');
          }
          
          function showVoipNumber(port)
          {
          	var Number = '';
          	var DigNum = '';
          	var k;

          	if ( 1 == port )
          		Number = this.Line1Number
          	<% if tcwebApi_get("VoIPBasic_Common","VoIPLine2Enable","h") = "Yes" then %>
          	else if ( 2 == port )
          		Number = this.Line2Number
          	<% end if %>
          	else
          		return;

          	k = Number.split('@');
          	DigNum = k[0];

          	document.write(DigNum);
          }
          
          function showLineState(port)
          {
          	var lineStatus = '';

          	if ( 1 == port ){
          		lineStatus = this.Line1Status;
          	}
          	<% if tcwebApi_get("VoIPBasic_Common","VoIPLine2Enable","h") = "Yes" then %>
          	else if ( 2 == port ){
          		lineStatus = this.Line2Status;	
          	}
          	<% end if %>
          	else
          		return;
          	if ( lineStatus == 'Idle' )
	          	document.write('空闲');
          	else if ( lineStatus == 'Ringing' )
	          	document.write('振铃');
          	else if ( lineStatus == 'Dialing' )
	          	document.write('拨号中');
          	else if ( lineStatus == 'Ringback' )
	          	document.write('听回铃音');
          	else if ( lineStatus == 'Connect' )
	          	document.write('已连接');
          	else if ( lineStatus == 'Disconnect' )
	          	document.write('断开连接');	
          	else
	          	document.write('--');
          }
          
          var VoipInfo = new VoipInfoClass();
        </SCRIPT>
    </HEAD>
    
    <BODY style="TEXT-ALIGN: center" vLink=#000000 aLink=#000000 link=#000000
    leftMargin=0 topMargin=0 onload="DisplayLocation(getElement('Selected_Menu').value);FinishLoad();if(getElById('ConfigForm') != null)LoadFrame()"
    onunload=DoUnload() marginheight="0" marginwidth="0">
        <TABLE height="100%" cellSpacing=0 cellPadding=0 width=808 align=center
        border=0>
            <TBODY>
                <TR>
                    <TD height=1>
<% if tcwebApi_get("WebCustom_Entry","isCYE8SFUSupported","h") = "Yes" then %>
                        <TABLE height=80 cellSpacing=0 cellPadding=0 width=808 background=/img/framelogo.jpg
                        border=0>
<% else %>
                        <TABLE height=117 cellSpacing=0 cellPadding=0 width=808 background=/img/framelogo.jpg
                        border=0>
<% end if %> 
                            <TBODY>
                                <TR>
                                    <TD>
                                        &nbsp;
                                    </TD>
                                    <TD vAlign=bottom align=right width=358>
                                        <TABLE id=table8 cellSpacing=0 cellPadding=0 border=0>
                                            <TBODY>
                                                <TR>
                                                    <TD vAlign=bottom align=right>
                                                        <SPAN class=curUserName>
                                                            &nbsp;
                                                        </SPAN>
                                                    </TD>
                                                    <TD class=welcom vAlign=bottom align=middle width=120>
                                                        欢迎您！
                                                    </TD>
                                                    <TD vAlign=bottom width=50>
                                                        <A onclick=DoLogout() href="/cgi-bin/logout.cgi" target=_top>
                                                            <SPAN class=logout>
                                                                退出
                                                            </SPAN>
                                                        </A>
                                                    </TD>
                                                </TR>
                                            </TBODY>
                                        </TABLE>
                                    </TD>
                                </TR>
                            </TBODY>
                        </TABLE>
                        <TABLE id=table2 height=100 cellSpacing=0 cellPadding=0 width=808 border=0>
                            <TBODY>
                                <TR>
                                    <TD class=LocationDisplay id=LocationDisplay align=middle width=163 bgColor=#ef8218
                                    rowSpan=3>
                                    </TD>
                                    <TD width=434 bgColor=#427594 height=33 class=CucBgColorTD_1>
                                        <P align=right>
                                            <FONT face=黑体 color=#ffffff>
                                                <B>
                                                    <FONT face=黑体 color=#ffffff size=6>
                                                        <INPUT id=Selected_Menu type=hidden value="状态->宽带语音信息" name=Selected_Menu>
                                                    </FONT>
                                                </B>
                                                <SPAN class=GatewayName>
                                                    网关名称:
                                                    <SCRIPT language=javascript>
                                                        document.write(top.gateWayName);
                                                    </SCRIPT>
                                                </SPAN>
                                            </FONT>
                                        </P>
                                    </TD>
                                    <TD width=211 bgColor=#ef8218 height=33 class=CucBgColorTD_1>
                                        <P class=GatewayType align=center>
                                            型号:
                                            <SCRIPT language=javascript>
                                                document.write(top.ModelName);
                                            </SCRIPT>
                                        </P>
                                    </TD>
                                </TR>
                                <TR>
                                    <TD id=MenuArea_L1 vAlign=bottom bgColor=#ef8218 colSpan=2 height=43 class=CucBgColorTD_1>
                                        &nbsp;
                                    </TD>
                                </TR>
                                <TR>
                                    <TD id=MenuArea_L2 bgColor=#427594 colSpan=2 height=24 class=CucBgColorTD_3>
                                    </TD>
                                </TR>
                            </TBODY>
                        </TABLE>
                        <SCRIPT language=javascript>
                            MakeMenu(getElById('Selected_Menu').value);
                        </SCRIPT>
                        <TABLE id=table3 height=15 cellSpacing=0 cellPadding=0 width=808 border=0>
                            <TBODY>
                                <TR>
                                    <TD height=15>
                                        <IMG height=15 src="/img/panel1.gif" width=164 border=0>
                                    </TD>
                                    <TD>
                                        <IMG height=15 src="/img/panel2.gif" width=645 border=0>
                                    </TD>
                                </TR>
                            </TBODY>
                        </TABLE>
                    </TD>
                </TR>
                <TR>
                    <TD vAlign=top>
                        <TABLE height="100%" cellSpacing=0 cellPadding=0 border=0>
                            <TBODY>
                                <TR>
                                    <TD class=Item_L1 width=157 bgColor=#ef8218 height=30>
                                        <P>
                                            宽带语音信息
                                        </P>
                                    </TD>
                                    <TD width=7 bgColor=#ef8218 class=CucBgColorTD_2>
                                        　
                                    </TD>
                                    <TD width=474>
                                        　
                                    </TD>
                                    <TD vAlign=top width=170 background=/img/panel4.gif rowSpan=4>
                                        <TABLE cellSpacing=0 cellPadding=20 width="100%" border=0>
                                            <TBODY>
                                                <TR>
                                                    <TD>
                                                        <A href="/cgi-bin/help_content.asp#宽带语音信息" target=_blank>
                                                            <IMG height=34 src="/img/help.gif" width=40 border=0>
                                                        </A>
                                                    </TD>
                                                </TR>
<% if tcwebApi_get("WebCustom_Entry","isCYE8SFUSupported","h") = "Yes" then %>
                                                <TR>
                                                    <TD valign='bottom'>
                                                        <IMG src='/img/tybottom.jpg' height=76 width=112>
                                                    </TD>
                                                </TR>
<% end if %>
                                            </TBODY>
                                        </TABLE>
                                    </TD>
                                </TR>
                                <TR>
                                    <TD vAlign=top width=157 bgColor=#e7e7e7 height=10>
                                    </TD>
                                    <TD width=7 background=/img/panel3.gif>
                                        　
                                    </TD>
                                    <TD>
                                    </TD>
                                </TR>
                                <TR>
                                    <TD class=Item_L2 vAlign=top width=157 bgColor=#e7e7e7 height=30>
                                        <P>
                                        </P>
                                    </TD>
                                    <TD width=7 background=/img/panel3.gif>
                                        　
                                    </TD>
                                    <TD>
                                        <TABLE cellSpacing=0 cellPadding=0 width="100%" border=0>
                                            <TBODY>
                                                <TR>
                                                    <TD width=10>
                                                        &nbsp;
                                                    </TD>
                                                    <TD>
                                                        <TABLE cellSpacing=0 cellPadding=3 width="100%" border=1>
                                                              <TBODY>
                                                                  <TR>
                                                                      <TD class=table_title width="30%">
                                                                          名称
                                                                      </TD>
                                                                      <TD class=table_title width="35%">
                                                                          线路1
                                                                      </TD>
                                                                      <% if tcwebApi_get("VoIPBasic_Common","VoIPLine2Enable","h") = "Yes" then %>
                                                                      <TD class=table_title width="35%">
                                                                          线路2
                                                                      </TD>
                                                                      <% end if %>
                                                                  </TR>
                                                                  <TR>
                                                                      <TD class=table_title>
                                                                          注册服务器
                                                                      </TD>
                                                                      <TD>
                                                                          <%if tcWebApi_get("VoIPBasic_Common", "RegistrarServer", "h") <> "N/A" then tcWebAPI_get("VoIPBasic_Common", "RegistrarServer", "s") else asp_Write("--") end if %>
                                                                          &nbsp;
                                                                      </TD>
                                                                      <% if tcwebApi_get("VoIPBasic_Common","VoIPLine2Enable","h") = "Yes" then %>
                                                                      <TD>
                                                                          <%if tcWebApi_get("VoIPBasic_Common", "RegistrarServer", "h") <> "N/A" then tcWebAPI_get("VoIPBasic_Common", "RegistrarServer", "s") else asp_Write("--") end if %>
                                                                          &nbsp;
                                                                      </TD>
                                                                      <% end if %>
                                                                  </TR>
                                                                  <TR>
                                                                      <TD class=table_title>
                                                                          注册服务器端口
                                                                      </TD>
                                                                      <TD>
                                                                          <%if tcWebApi_get("VoIPBasic_Common", "RegistrarServerPort", "h") <> "N/A" then tcWebAPI_get("VoIPBasic_Common", "RegistrarServerPort", "s") else asp_Write("--") end if %>
                                                                          &nbsp;
                                                                      </TD>
                                                                      <% if tcwebApi_get("VoIPBasic_Common","VoIPLine2Enable","h") = "Yes" then %>
                                                                      <TD>
                                                                          <%if tcWebApi_get("VoIPBasic_Common", "RegistrarServerPort", "h") <> "N/A" then tcWebAPI_get("VoIPBasic_Common", "RegistrarServerPort", "s") else asp_Write("--") end if %>
                                                                          &nbsp;
                                                                      </TD>
                                                                      <% end if %>
                                                                  </TR>          
                                                                  <TR>
                                                                      <TD class=table_title>
                                                                          备用注册服务器
                                                                      </TD>
                                                                      <TD>
                                                                          <%if tcWebApi_get("VoIPBasic_Common", "SBRegistrarServer", "h") <> "N/A" then tcWebAPI_get("VoIPBasic_Common", "SBRegistrarServer", "s") else asp_Write("--") end if %>
                                                                          &nbsp;
                                                                      </TD>
                                                                      <% if tcwebApi_get("VoIPBasic_Common","VoIPLine2Enable","h") = "Yes" then %>
                                                                      <TD>
                                                                          <%if tcWebApi_get("VoIPBasic_Common", "SBRegistrarServer", "h") <> "N/A" then tcWebAPI_get("VoIPBasic_Common", "SBRegistrarServer", "s") else asp_Write("--") end if %>
                                                                          &nbsp;
                                                                      </TD>
                                                                      <% end if %>
                                                                  </TR>
                                                                  <TR>
                                                                      <TD class=table_title>
                                                                          备用注册服务器端口
                                                                      </TD>
                                                                      <TD>
                                                                          <%if tcWebApi_get("VoIPBasic_Common", "SBRegistrarServerPort", "h") <> "N/A" then tcWebAPI_get("VoIPBasic_Common", "SBRegistrarServerPort", "s") else asp_Write("--") end if %>
                                                                          &nbsp;
                                                                      </TD>
                                                                      <% if tcwebApi_get("VoIPBasic_Common","VoIPLine2Enable","h") = "Yes" then %>
                                                                      <TD>
                                                                          <%if tcWebApi_get("VoIPBasic_Common", "SBRegistrarServerPort", "h") <> "N/A" then tcWebAPI_get("VoIPBasic_Common", "SBRegistrarServerPort", "s") else asp_Write("--") end if %>
                                                                          &nbsp;
                                                                      </TD>
                                                                      <% end if %>
                                                                  </TR>       
                                                                  <TR>
                                                                      <TD class=table_title>
                                                                          电话号码
                                                                      </TD>
                                                                      <TD>
                                                                          <script language=JavaScript type=text/javascript>
                                                                          VoipInfo.showVoipNumber(1);
                                                                          </script>
                                                                          &nbsp;
                                                                      </TD>
                                                                      <% if tcwebApi_get("VoIPBasic_Common","VoIPLine2Enable","h") = "Yes" then %>
                                                                      <TD>
                                                                          <script language=JavaScript type=text/javascript>
                                                                          VoipInfo.showVoipNumber(2);
                                                                          </script>
                                                                          &nbsp;
                                                                     	</TD>
                                                                     	<% end if %>
                                                                  </TR>    
                                                                  <TR>
                                                                      <TD class=table_title>
                                                                          注册状态
                                                                      </TD>
                                                                      <TD>
                                                                          <script language=JavaScript type=text/javascript>
                                                                          VoipInfo.showRegisterState(1);
                                                                          </script>
                                                                          &nbsp;
                                                                      </TD>
                                                                      <% if tcwebApi_get("VoIPBasic_Common","VoIPLine2Enable","h") = "Yes" then %>
                                                                      <TD>
                                                                          <script language=JavaScript type=text/javascript>
                                                                          VoipInfo.showRegisterState(2);
                                                                          </script>
                                                                          &nbsp;
                                                                     	</TD>
                                                                     	<% end if %>
                                                                  </TR>
                                                                  <TR>
                                                                      <TD class=table_title>
                                                                          用户线路状态
                                                                      </TD>
                                                                      <TD>
                                                                          <script language=JavaScript type=text/javascript>
                                                                          VoipInfo.showLineState(1);
                                                                          </script>
                                                                          &nbsp;
                                                                      </TD>
                                                                      <% if tcwebApi_get("VoIPBasic_Common","VoIPLine2Enable","h") = "Yes" then %>
                                                                      <TD>
                                                                          <script language=JavaScript type=text/javascript>
                                                                          VoipInfo.showLineState(2);
                                                                          </script>
                                                                          &nbsp;
                                                                     	</TD>
                                                                      <% end if %>
                                                                  </TR> 
                                                              </TBODY>
                                                          </TABLE>
                                                    </TD>
                                                    <TD width=10>
                                                        &nbsp;
                                                    </TD>
                                                </TR>
                                                <TR>
                                                    <TD width=10>
                                                        &nbsp;
                                                    </TD>
                                                    <TD>
                                                    </TD>
                                                    <TD width=10>
                                                        &nbsp;
                                                    </TD>
                                                </TR>
                                            </TBODY>
                                        </TABLE>
                                    </TD>
                                </TR>
                                <TR>
                                    <TD vAlign=top width=157 bgColor=#e7e7e7>
                                    </TD>
                                    <TD width=7 background=/img/panel3.gif>
                                    </TD>
                                    <TD>
                                    </TD>
                                </TR>                                
                            </TBODY>
                        </TABLE>
                    </TD>
                </TR>
                <TR>
                    <TD height=1>
                        <TABLE id=table7 height=35 cellSpacing=0 cellPadding=0 width=808 border=0>
                            <TBODY>
                                <TR>
                                    <TD width=162 bgColor=#ef8218 class=CucBgColorTD_2>
                                        　
                                    </TD>
                                    <TD width=278 bgColor=#427594 class=CucBgColorTD_2>
                                        　
                                    </TD>
                                    <TD width=196 bgColor=#427594 class=CucBgColorTD_2>
                                        <P align=center>
                                        </P>
                                    </TD>
                                    <TD width=170 bgColor=#313031 class=CucBgColorTD_2>
                                        　
                                    </TD>
                                </TR>
                            </TBODY>
                        </TABLE>
                    </TD>
                </TR>
            </TBODY>
        </TABLE>
    </BODY>

</HTML>