<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0 Transitional//EN">
<HTML><HEAD><TITLE>test_factory</TITLE>
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

    function RestorePureFactory()
    {
        if(confirm("您确认要恢复干净的出厂状态？"))
        {
            document.DefaultForm.defaultflag.value = "1";
            document.DefaultForm.restoreflag2.value = "6";
            document.DefaultForm.submit();
            getElById('RestorePureFactory').disabled = true;
            setTimeout("doRefresh()",10*1000);
        }
    }

      function LoadFrame()
      {  
      }
      
      </SCRIPT>

     <TABLE height="100%" cellSpacing=0 cellPadding=0 border=0>
        <TBODY>
        <TR>
          <TD width=157 bgColor=#ef8218 height=30>
            <P class=Item_L1>恢复预配置</P></TD>
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
                        <P align=center>
          		        <!--<input type="button" value="恢复纯净出厂"  name="RestorePureFactory"  onClick="RestorePureFactory();">-->
          		        <input type="button" value="恢复纯净出厂"  name="RestorePureFactory"  onClick="window.location.href='/cgi-bin/restorepurefactory.cgi';">
          		        <!--'{location.replace("restoreprecfg.cgi");}' >-->
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
        </TBODY>

        <TBODY>
        <TR>
          <TD width=157 bgColor=#ef8218 height=30>
            <P class=Item_L1>长发光开关</P></TD>
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
                    <!-- <TR>
                      <TD class=table_title>长发光状态</TD>
                      <TD>
                        <iframe src="/cgi-bin/laserstatus.cgi" frameborder="0" scrolling="no" width="474" height="580" align=left></iframe>
					  </TD>
                    </TR>
                    <br> -->
                    <TR>
                        <TD align=middle>
                        <!-- <A href="/cgi-bin/laserforce.cgi"> -->
          		        <input type="button" value="开启长发光"  name="LaserForce"  onClick="window.location.href='/cgi-bin/laserforce.cgi';">
          		        <!--'{location.replace("laserforce.cgi");}' >-->
                        </TD>
                        <TD align=middle>
                        <!-- <A href="/cgi-bin/lasernormal.cgi"> -->
                        <P align=center>
          		        <input type="button" value="关闭长发光"  name="LaserNormal"  onClick="window.location.href='/cgi-bin/lasernormal.cgi';">
          		        <!--'{location.replace("lasernormal.cgi");}' >-->
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
