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
          function VoipInfoClass()
          {
              this.Reg1Status = 'disabled';
              this.Reg2Status = 'disabled';
              this.Line1Number = '101@10.10.0.2';
              this.Line2Number = '102@10.10.0.6';
              this.showRegisterState = showRegisterState;
              this.showVoipNumber = showVoipNumber;
          }

          function showRegisterState(port)
          {
          	var Status = '';

          	if ( 1 == port )
          		Status = this.Reg1Status;
          	else if ( 2 == port )
          		Status = this.Reg2Status;
          	else
          		return;

						if ( Status == 'up' )
							document.write('注册成功');
						else if ( Status == 'initializing' )
							document.write('初始化中');
						else if ( Status == 'registering' )
							document.write('注册中');
						else if ( Status == 'unregistering' )
							document.write('未注册');
						else if ( Status == 'quiescent' )
							document.write('停止的');
						else if ( Status == 'disabled' )
							document.write('去使能');
						else if ( Status == 'error' )
							document.write('故障');
						else if ( Status == 'testing' )
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
          	else if ( 2 == port )
          		Number = this.Line2Number
          	else
          		return;

          	k = Number.split('@');
          	DigNum = k[0];

          	document.write(DigNum);
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
                            		<tr>
												        	<TD width=808 bgColor=#d8d8d8 height=2 colSpan=3></td>
												        	</tr>	
                                <TR>
                                    <TD class=LocationDisplay id=LocationDisplay align=middle width=163 bgColor=#d8d8d8
                                    rowSpan=3>
                                    </TD>
                                    <TD width=434 bgColor=#0059A0 height=33>
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
                                    <TD width=211 bgColor=#d8d8d8 height=33>
                                        <P class=GatewayType align=center>
                                            型号:
                                            <SCRIPT language=javascript>
                                                document.write(top.ModelName);
                                            </SCRIPT>
                                        </P>
                                    </TD>
                                </TR>
                                <TR>
                                    <TD id=MenuArea_L1 vAlign=bottom bgColor=#d8d8d8 colSpan=2 height=43>
                                        &nbsp;
                                    </TD>
                                </TR>
                                <TR>
                                    <TD id=MenuArea_L2 bgColor=#0059A0 colSpan=2 height=24>
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
                                    <TD class=Item_L1 width=157 bgColor=#e7e7e7 height=30>
                                        <P>
                                            业务注册状态
                                        </P>
                                    </TD>
                                    <TD width=7 bgColor=#e7e7e7>
                                        　
                                    </TD>
                                    <TD width=474>
                                        　
                                    </TD>
                                    <TD vAlign=top width=170 background=/img/panel4.gif rowSpan=8>
                                        <TABLE cellSpacing=0 cellPadding=20 width="100%" border=0  height='100%'>
                                            <TBODY>
                                                <TR>
                                                    <TD valign='top'>
                                                        <A href="/cgi-bin/help_content.asp#状态" target=_blank>
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
                                                                      <TD class=table_title width="20%">
                                                                          用户编号:
                                                                      </TD>
                                                                      <TD class=table_title width="60%">
                                                                          用户注册状态:
                                                                      </TD>
                                                                  </TR>
                                                                  <TR>
                                                                      <TD>
                                                                          1
                                                                      </TD>
                                                                      <TD>
                                                                      		<script language=JavaScript type=text/javascript>
                                                                          VoipInfo.showRegisterState(1);
                                                                          </script>
                                                                          &nbsp;
                                                                      </TD>
                                                                  </TR>
                                                                  <TR>
                                                                      <TD>
                                                                          2
                                                                      </TD>
                                                                      <TD>
                                                                      		<script language=JavaScript type=text/javascript>
                                                                          VoipInfo.showRegisterState(2);
                                                                          </script>
                                                                          &nbsp;
                                                                      </TD>
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
                                    <TD vAlign=top width=157 bgColor=#e7e7e7 height="11px">
                                    </TD>
                                    <TD width=7 background=/img/panel3.gif>
                                    </TD>
                                    <TD>
                                    </TD>
                                </TR>
                                <TR>
                                    <TD width=157 bgColor=#e7e7e7 height=30>
                                        <P class=Item_L1>
                                            电话号码
                                        </P>
                                    </TD>
                                    <TD width=7 bgColor=#e7e7e7>
                                    </TD>
                                    <TD width=474>　
                                    </TD>
                                </TR>
                                <TR>
                                    <TD vAlign=top width=157 bgColor=#e7e7e7 height="11px">
                                    </TD>
                                    <TD width=7 background=/img/panel3.gif></TD>
                                    <TD></TD>
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
                                                                      <TD class=table_title width="20%">
                                                                          编号:
                                                                      </TD>
                                                                      <TD class=table_title width="60%">
                                                                          用户名(电话号码):
                                                                      </TD>
                                                                  </TR>
                                                                  <TR>
                                                                      <TD>
                                                                          1
                                                                      </TD>
                                                                      <TD>
                                                                      		<script language=JavaScript type=text/javascript>
                                                                          VoipInfo.showVoipNumber(1);
                                                                          </script>
                                                                          &nbsp;
                                                                      </TD>
                                                                  </TR>
                                                                  <TR>
                                                                      <TD>
                                                                          2
                                                                      </TD>
                                                                      <TD>
                                                                      		<script language=JavaScript type=text/javascript>
                                                                          VoipInfo.showVoipNumber(2);
                                                                          </script>
                                                                          &nbsp;
                                                                      </TD>
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
                                    <TD width=7 background=/img/panel3.gif></TD>
                                    <TD></TD>
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
                                    <TD width=162 bgColor=#d8d8d8>
                                        　
                                    </TD>
                                    <TD width=278 bgColor=#0059A0>
                                        　
                                    </TD>
                                    <TD width=196 bgColor=#0059A0>
                                        <P align=center>
                                        </P>
                                    </TD>
                                    <TD width=170 bgColor=#313031>
                                        　
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