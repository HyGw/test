<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0 Transitional//EN">
<HTML>
    
    <HEAD>
        <TITLE>
            ���������Ϣ
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
              this.Reg1Status = '<%if tcWebAPI_get("VoIPH248_Entry0", "UserServiceState", "h") <> "N/A" then tcWebAPI_get("VoIPH248_Entry0", "UserServiceState", "s") else asp_Write("") end if %>';
              this.Reg2Status = '<%if tcWebAPI_get("VoIPH248_Entry1", "UserServiceState", "h") <> "N/A" then tcWebAPI_get("VoIPH248_Entry1", "UserServiceState", "s") else asp_Write("") end if %>';
              this.UserState1 = '<%if tcWebAPI_get("VoIPH248_Entry0", "UserState", "h") <> "N/A" then tcWebAPI_get("VoIPH248_Entry0", "UserState", "s") else asp_Write("") end if %>';
              this.UserState2 = '<%if tcWebAPI_get("VoIPH248_Entry1", "UserState", "h") <> "N/A" then tcWebAPI_get("VoIPH248_Entry1", "UserState", "s") else asp_Write("") end if %>';
              this.showRegisterState = showRegisterState;
              this.showUserState = showUserState;
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

						if ( Status == '1' )
							document.write('��ע��');
						else if ( Status == '3' )
							document.write('ע����');
						else if ( Status == '5' )
							document.write('ע��ʧ��');
						else if ( Status == '8' )
							document.write('����');
						else
							document.write('--');
          }
          
          function showUserState(port)
          {
          	var Status = '';

          	if ( 1 == port )
          		Status = this.UserState1;
          	else if ( 2 == port )
          		Status = this.UserState2;
          	else
          		return;

						if ( Status == '1' )
							document.write('ע����');
						else if ( Status == '2' )
							document.write('����');
						else if ( Status == '3' )
							document.write('ժ��');
						else if ( Status == '4' )
							document.write('������');
						else if ( Status == '5' )
							document.write('����');
						else if ( Status == '6' )
							document.write('��������');
						else if ( Status == '7' )
							document.write('������');
						else if ( Status == '8' )
							document.write('���ڶϿ�����');
						else if ( Status == '9' )
							document.write('��æ��');
						else if ( Status == '10' )
							document.write('ע��ʧ��');
						else if ( Status == '11' )
							document.write('��·����');
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
                        <TABLE height=117 cellSpacing=0 cellPadding=0 width=808 background=/img/framelogo.jpg
                        border=0>
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
                                                        ��ӭ����
                                                    </TD>
                                                    <TD vAlign=bottom width=50>
                                                        <A onclick=DoLogout() href="/cgi-bin/logout.cgi" target=_top>
                                                            <SPAN class=logout>
                                                                �˳�
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
                                            <FONT face=���� color=#ffffff>
                                                <B>
                                                    <FONT face=���� color=#ffffff size=6>
                                                        <INPUT id=Selected_Menu type=hidden value="״̬->���������Ϣ" name=Selected_Menu>
                                                    </FONT>
                                                </B>
                                                <SPAN class=GatewayName>
                                                    ��������:
                                                    <SCRIPT language=javascript>
                                                        document.write(top.gateWayName);
                                                    </SCRIPT>
                                                </SPAN>
                                            </FONT>
                                        </P>
                                    </TD>
                                    <TD width=211 bgColor=#ef8218 height=33 class=CucBgColorTD_1>
                                        <P class=GatewayType align=center>
                                            �ͺ�:
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
                                            ���������Ϣ
                                        </P>
                                    </TD>
                                    <TD width=7 bgColor=#ef8218 class=CucBgColorTD_2>
                                        ��
                                    </TD>
                                    <TD width=474>
                                        ��
                                    </TD>
                                    <TD vAlign=top width=170 background=/img/panel4.gif rowSpan=4>
                                        <TABLE cellSpacing=0 cellPadding=20 width="100%" border=0>
                                            <TBODY>
                                                <TR>
                                                    <TD>
                                                        <A href="/cgi-bin/help_content.asp#���������Ϣ" target=_blank>
                                                            <IMG height=34 src="/img/help.gif" width=40 border=0>
                                                        </A>
                                                    </TD>
                                                </TR>
                                            </TBODY>
                                        </TABLE>
                                    </TD>
                                </TR>
                                <TR>
                                    <TD vAlign=top width=157 bgColor=#e7e7e7 height=10>
                                    </TD>
                                    <TD width=7 background=/img/panel3.gif>
                                        ��
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
                                        ��
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
                                                                          ����
                                                                      </TD>
                                                                      <TD class=table_title width="35%">
                                                                          ��·1
                                                                      </TD>
                                                                      <TD class=table_title width="35%">
                                                                          ��·2
                                                                      </TD>
                                                                  </TR>
                                                                  <TR>
                                                                      <TD class=table_title>
                                                                          ��������ַ
                                                                      </TD>
                                                                      <TD>
                                                                          <%if tcWebApi_get("VoIPH248_Common", "MediaGatewayControler", "h") <> "N/A" then tcWebAPI_get("VoIPH248_Common", "MediaGatewayControler", "s") else asp_Write("--") end if %>
                                                                          &nbsp;
                                                                      </TD>
                                                                      <TD>
                                                                          <%if tcWebApi_get("VoIPH248_Common", "MediaGatewayControler", "h") <> "N/A" then tcWebAPI_get("VoIPH248_Common", "MediaGatewayControler", "s") else asp_Write("--") end if %>
                                                                          &nbsp;
                                                                      </TD>
                                                                  </TR>
                                                                  <TR>
                                                                      <TD class=table_title>
                                                                          �������˿ں�
                                                                      </TD>
                                                                      <TD>
                                                                          <%if tcWebApi_get("VoIPH248_Common", "MediaGatewayControlerPort", "h") <> "N/A" then tcWebAPI_get("VoIPH248_Common", "MediaGatewayControlerPort", "s") else asp_Write("--") end if %>
                                                                          &nbsp;
                                                                      </TD>
                                                                      <TD>
                                                                          <%if tcWebApi_get("VoIPH248_Common", "MediaGatewayControlerPort", "h") <> "N/A" then tcWebAPI_get("VoIPH248_Common", "MediaGatewayControlerPort", "s") else asp_Write("--") end if %>
                                                                          &nbsp;
                                                                      </TD>
                                                                  </TR>          
                                                                  <TR>
                                                                      <TD class=table_title>
                                                                          ���÷�������ַ
                                                                      </TD>
                                                                      <TD>
                                                                          <%if tcWebApi_get("VoIPH248_Common", "SBMediaGatewayControler", "h") <> "N/A" then tcWebAPI_get("VoIPH248_Common", "SBMediaGatewayControler", "s") else asp_Write("--") end if %>
                                                                          &nbsp;
                                                                      </TD>
                                                                      <TD>
                                                                          <%if tcWebApi_get("VoIPH248_Common", "SBMediaGatewayControler", "h") <> "N/A" then tcWebAPI_get("VoIPH248_Common", "SBMediaGatewayControler", "s") else asp_Write("--") end if %>
                                                                          &nbsp;
                                                                      </TD>
                                                                  </TR>
                                                                  <TR>
                                                                      <TD class=table_title>
                                                                          ���÷������˿ں�
                                                                      </TD>
                                                                      <TD>
                                                                          <%if tcWebApi_get("VoIPH248_Common", "SBMediaGatewayControlerPort", "h") <> "N/A" then tcWebAPI_get("VoIPH248_Common", "SBMediaGatewayControlerPort", "s") else asp_Write("--") end if %>
                                                                          &nbsp;
                                                                      </TD>
                                                                      <TD>
                                                                          <%if tcWebApi_get("VoIPH248_Common", "SBMediaGatewayControlerPort", "h") <> "N/A" then tcWebAPI_get("VoIPH248_Common", "SBMediaGatewayControlerPort", "s") else asp_Write("--") end if %>
                                                                          &nbsp;
                                                                      </TD>
                                                                  </TR>       
                                                                  <TR>
                                                                      <TD class=table_title>
                                                                          ע��˿ں�
                                                                      </TD>
                                                                      <TD>
                                                                          <%if tcWebApi_get("VoIPH248_Common", "MediaGatewayPort", "h") <> "N/A" then tcWebAPI_get("VoIPH248_Common", "MediaGatewayPort", "s") else asp_Write("--") end if %>
                                                                          &nbsp;
                                                                      </TD>
                                                                      <TD>
                                                                          <%if tcWebApi_get("VoIPH248_Common", "MediaGatewayPort", "h") <> "N/A" then tcWebAPI_get("VoIPH248_Common", "MediaGatewayPort", "s") else asp_Write("--") end if %>
                                                                          &nbsp;
                                                                      </TD>
                                                                  </TR>    
                                                                  <TR>
                                                                      <TD class=table_title>
                                                                          ע��״̬
                                                                      </TD>
                                                                      <TD>
                                                                          <script language=JavaScript type=text/javascript>
                                                                          VoipInfo.showRegisterState(1);
                                                                          </script>
                                                                          &nbsp;
                                                                      </TD>
                                                                      <TD>
                                                                          <script language=JavaScript type=text/javascript>
                                                                          VoipInfo.showRegisterState(2);
                                                                          </script>
                                                                          &nbsp;
                                                                     	</TD>
                                                                  </TR>
                                                                  <TR>
                                                                      <TD class=table_title>
                                                                          �û��˿�״̬
                                                                      </TD>
                                                                      <TD>
                                                                          <script language=JavaScript type=text/javascript>
                                                                          VoipInfo.showUserState(1);
                                                                          </script>
                                                                          &nbsp;
                                                                      </TD>
                                                                      <TD>
                                                                          <script language=JavaScript type=text/javascript>
                                                                          VoipInfo.showUserState(2);
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
                                        ��
                                    </TD>
                                    <TD width=278 bgColor=#427594 class=CucBgColorTD_2>
                                        ��
                                    </TD>
                                    <TD width=196 bgColor=#427594 class=CucBgColorTD_2>
                                        <P align=center>
                                        </P>
                                    </TD>
                                    <TD width=170 bgColor=#313031 class=CucBgColorTD_2>
                                        ��
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