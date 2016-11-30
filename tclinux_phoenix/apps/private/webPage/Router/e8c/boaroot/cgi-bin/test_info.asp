<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0 Transitional//EN">
<HTML><HEAD><TITLE>test_info</TITLE>
<META http-equiv=Content-Language content=zh-cn>
<META http-equiv=Content-Type content="text/html; charset=gb2312"><LINK 
href="/JS/stylemain.css" type=text/css rel=stylesheet>
<SCRIPT language=javascript src="/JS/util.js"></SCRIPT>

<META content="MSHTML 6.00.6000.16809" name=GENERATOR></HEAD>
<BODY style="TEXT-ALIGN: center" vLink=#000000 aLink=#000000 link=#000000 
leftMargin=0 topMargin=0 
onload="if(getElById('ConfigForm') != null)LoadFrame()" 
onunload=DoUnload() marginheight="0" marginwidth="0">
<TABLE height="100%" cellSpacing=0 cellPadding=0 width=808 align=center border=0>
  <TBODY>
  <TR>
    <TD height=1>
      <TABLE height=117 cellSpacing=0 cellPadding=0 width=808 background=/img/framelogo.jpg border=0>
        <TBODY>
        <TR>
          <TD>&nbsp;</TD>
          <TD vAlign=bottom align=right width=358>
            <TABLE id=table8 cellSpacing=0 cellPadding=0 border=0>
              <TBODY>
              <TR>
                <TD vAlign=bottom align=right><SPAN class=curUserName>&nbsp;</SPAN></TD>
                <TD vAlign=bottom align=middle width=100>欢迎您！</TD>
                <TD vAlign=bottom width=70><A href="/cgi-bin/index.asp" target=_top>返回主页</A>
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
          <TD class=LocationDisplay id=LocationDisplay align=middle width=163 
          bgColor=#ef8218 rowSpan=3></TD>
          <TD width=434 bgColor=#427594 height=33>
            <P align=right><FONT face=黑体 color=#ffffff><B><FONT face=黑体 
            color=#ffffff size=6></FONT></B><SPAN class=GatewayName></SPAN></FONT></P></TD>
          <TD width=211 bgColor=#427594 height=33>
            </TD></TR>
        <TR>
          <TD id=MenuArea_L1 vAlign=bottom bgColor=#ef8218 colSpan=2 
          height=43>&nbsp;</TD></TR>
        <TR>
          <TD id=MenuArea_L2 bgColor=#427594 colSpan=2 height=24></TD>
        </TR>
        </TBODY>
      </TABLE>
      <TABLE id=table3 height=15 cellSpacing=0 cellPadding=0 width=808 border=0>
        <TBODY>
        <TR>
          <TD height=15><IMG height=15 src="/img/panel1.gif" width=164 border=0> </TD>
          <TD><IMG height=15 src="/img/panel2.gif" width=645 border=0> </TD>
        </TR>
        </TBODY>
      </TABLE>
    </TD>
  </TR>
  <TR>
    <TD vAlign=top>
      <SCRIPT language=JavaScript type=text/javascript>

    var nWanNum = "<% tcWebApi_get("WanInfo_Common","CycleNum","s") %>";

      function LoadFrame()
      {  
      }
      
      </SCRIPT>

     <TABLE height="100%" cellSpacing=0 cellPadding=0 border=0>
        <TBODY>
        <TR>
          <TD width=157 bgColor=#ef8218 height=30>
            <P class=Item_L1>工厂出厂信息</P></TD>
          <TD width=7 bgColor=#ef8218>　</TD>
          <TD width=474></TD>
          <TD vAlign=top width=170 background=/img/panel4.gif rowSpan=5>
            &nbsp;
          </TD>
        </TR>
        <TR>
          <TD vAlign=top width=157 bgColor=#e7e7e7 height=10></TD>
          <TD width=7 background=/img/panel3.gif>　</TD>
          <TD></TD>
        </TR>
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
                  <TABLE cellSpacing=0 cellPadding=3 width=400 border=1>
                    <TBODY>
                    <TR>
                      <TD class=table_title>内部版本</TD>
                      <TD width=300>
                        <%tcWebApi_get("DeviceInfo","FwVer","s")%> (svn:<%tcWebApi_get("DeviceInfo_devParaStatic","SvnRev","s")%>)
                        &nbsp;
                      </TD>
                    </TR>

                    <TR>
                      <TD class=table_title>编译时间</TD>
                      <TD width=300>
                        <%tcWebApi_get("DeviceInfo_devParaStatic","CompileTime","s")%>
                        &nbsp;
                      </TD>
                    </TR>

                    <TR>
                      <TD class=table_title>设备型号</TD>
                      <TD width=300>
                        <%tcWebApi_get("DeviceInfo_devParaStatic","ModelName","s")%>
                        &nbsp;
                      </TD>
                    </TR>

                    <TR>
                      <TD class=table_title>硬件版本</TD>
                      <TD width=300>
                        <%tcWebApi_get("DeviceInfo_devParaStatic","CustomerHWVersion","s")%>
                        &nbsp;
                      </TD>
                    </TR>

                    <TR>
                      <TD class=table_title>软件版本</TD>
                      <TD width=300>
                        <%tcWebApi_get("DeviceInfo_devParaStatic","CustomerSWVersion","s")%>
                        &nbsp;
                      </TD>
                    </TR>

                    <TR>
                      <TD class=table_title>工厂生产标识</TD>
                      <TD width=300>
                        <%tcWebApi_get("SysInfo_Entry","ResetFlag","s")%> 
                        &nbsp;
                      </TD>
                    </TR>

                    <TR>
                      <TD class=table_title>WAN连接</TD>
                      <TD width=250 style="word-wrap:break-word;">
                        <% tcWebApi_get("WanInfo_Common","CycleValue","s") %>
                        &nbsp;
                      </TD>
                    </TR>

                    </TBODY>
                  </TABLE>
                </TD>
                <TD width=10>&nbsp;</TD>
              </TR>
              </TBODY>
            </TABLE>
          </TD>
        </TR>
        <TR>
          <TD vAlign=top width=157 bgColor=#e7e7e7 height=10></TD>
          <TD width=7 background=/img/panel3.gif>　</TD>
          <TD></TD></TR>
        <TR>
          <TD vAlign=top width=157 bgColor=#e7e7e7></TD>
          <TD width=7 background=/img/panel3.gif>　</TD>
          <TD></TD>
        </TR>
        </TBODY>

        <TBODY>
        <TR>
          <TD width=157 bgColor=#ef8218 height=30>
            <P class=Item_L1>条码信息</P></TD>
          <TD width=7 bgColor=#ef8218>　</TD>
          <TD width=474></TD>
          <TD vAlign=top width=170 background=/img/panel4.gif rowSpan=5>
            &nbsp;
          </TD>
        </TR>
        <TR>
          <TD vAlign=top width=157 bgColor=#e7e7e7 height=10></TD>
          <TD width=7 background=/img/panel3.gif>　</TD>
          <TD></TD>
        </TR>
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
                  <TABLE cellSpacing=0 cellPadding=3 width=400 border=1>
                    <TBODY>
                    <TR>
                      <TD class=table_title>DB条码</TD>
                      <TD width=300>
                        <%tcWebApi_get("DeviceInfo_devParaDynamic","BarCode","s")%>
                        &nbsp;
                      </TD>
                    </TR>

                    <TR>
                      <TD class=table_title>用户密码</TD>
                      <TD width=300>
                        <%tcWebApi_get("Account_Entry1","web_passwd","s")%>
                        &nbsp;
                      </TD>
                    </TR>

                    <TR>
                      <TD class=table_title>设备标识号</TD>
                      <TD width=300>
                        <%tcWebApi_get("DeviceInfo_devParaDynamic","ManufacturerOUI","s")%>-<%tcWebApi_get("DeviceInfo_devParaDynamic","SerialNum","s")%>
                        &nbsp;
                      </TD>
                    </TR>

                    <TR>
                      <TD class=table_title>MAC</TD>
                      <TD width=300>
                        <%tcWebApi_get("Info_Ether","mac","s")%> 
                        &nbsp;
                      </TD>
                    </TR>

                    <TR>
                      <TD class=table_title>装箱SN</TD>
                      <TD width=300>
                        &nbsp;
                      </TD>
                    </TR>
                    </TBODY>
                  </TABLE>
                </TD>
                <TD width=10>&nbsp;</TD>
              </TR>
              </TBODY>
            </TABLE>
          </TD>
        </TR>
        <TR>
          <TD vAlign=top width=157 bgColor=#e7e7e7 height=10></TD>
          <TD width=7 background=/img/panel3.gif>　</TD>
          <TD></TD></TR>
        <TR>
          <TD vAlign=top width=157 bgColor=#e7e7e7></TD>
          <TD width=7 background=/img/panel3.gif>　</TD>
          <TD></TD>
        </TR>
        </TBODY>

        <TBODY>
        <TR>
          <TD width=157 bgColor=#ef8218 height=30>
            <P class=Item_L1>LOID</P></TD>
          <TD width=7 bgColor=#ef8218>　</TD>
          <TD width=474></TD>
          <TD vAlign=top width=170 background=/img/panel4.gif rowSpan=5>
            &nbsp;
          </TD>
        </TR>
        <TR>
          <TD vAlign=top width=157 bgColor=#e7e7e7 height=10></TD>
          <TD width=7 background=/img/panel3.gif>　</TD>
          <TD></TD>
        </TR>
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
                  <TABLE cellSpacing=0 cellPadding=3 width=400 border=1>
                    <TBODY>
                    <TR>
                      <TD class=table_title>LOID</TD>
                      <TD width=300>
                        <%If tcWebApi_get("EPON_LOIDAuth","LOID0","h") <> "N/A" Then tcWebApi_get("EPON_LOIDAuth","LOID0","s") end if%>
                        &nbsp;
                      </TD>
                    </TR>

                    <TR>
                      <TD class=table_title>Password</TD>
                      <TD width=300>
                        <%If tcWebApi_get("EPON_LOIDAuth","Password0","h") <> "N/A" Then tcWebApi_get("EPON_LOIDAuth","Password0","s") end if%>
                        &nbsp;
                      </TD>
                    </TR>
                    </TBODY>
                  </TABLE>
                </TD>
                <TD width=10>&nbsp;</TD>
              </TR>
              <TR>
                <TD width=10>&nbsp;</TD>
                <TD></TD>
                <TD width=10>&nbsp;</TD>
              </TR>
              </TBODY>
            </TABLE>
          </TD>
        </TR>
        <TR>
          <TD vAlign=top width=157 bgColor=#e7e7e7 height=10></TD>
          <TD width=7 background=/img/panel3.gif>　</TD>
          <TD></TD></TR>
        <TR>
          <TD vAlign=top width=157 bgColor=#e7e7e7></TD>
          <TD width=7 background=/img/panel3.gif>　</TD>
          <TD></TD>
        </TR>
        </TBODY>
      </TABLE>


    </TD>
  </TR>
  <TR>
    <TD height=1>
      <TABLE id=table7 height=35 cellSpacing=0 cellPadding=0 width=808 
        border=0><TBODY>
        <TR>
          <TD width=162 bgColor=#ef8218>&nbsp;</TD>
          <TD width=278 bgColor=#427594>&nbsp;</TD>
          <TD width=196 bgColor=#427594>
            <P align=center></P>
          </TD>
          <TD width=170 bgColor=#313031>&nbsp;</TD>
        </TR>
      </TABLE>
    </TD>
  </TR>
</TBODY>
</TABLE>
</BODY>
</HTML>