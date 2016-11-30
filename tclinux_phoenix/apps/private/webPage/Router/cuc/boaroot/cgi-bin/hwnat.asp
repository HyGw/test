<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0 Transitional//EN">
<HTML><HEAD><TITLE>ParaSet</TITLE>
<META http-equiv=Content-Language content=zh-cn>
<META http-equiv=Content-Type content="text/html; charset=gb2312"><LINK 
href="/JS/stylemain.css" type=text/css rel=stylesheet>
<SCRIPT language=javascript src="/JS/util.js"></SCRIPT>

<META content="MSHTML 6.00.6000.16809" name=GENERATOR></HEAD>
<BODY style="TEXT-ALIGN: center" vLink=#000000 aLink=#000000 link=#000000 
leftMargin=0 topMargin=0 
onload="if(getElById('ConfigForm') != null)LoadFrame()" 
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
                <TD vAlign=bottom align=middle width=100>欢迎您！</TD>
                <TD vAlign=bottom width=70><A href="/cgi-bin/index.asp" target=_top>返回主页
				</A></TD></TR></TBODY></TABLE></TD></TR></TBODY></TABLE>
      <TABLE id=table2 height=100 cellSpacing=0 cellPadding=0 width=808 
border=0>
        <TBODY>
        <TR>
          <TD class=LocationDisplay id=LocationDisplay align=middle width=163 
          bgColor=#ef8218 rowSpan=3></TD>
          <TD width=434 bgColor=#427594 height=33 class=CucBgColorTD_1>
            <P align=right><FONT face=黑体 color=#ffffff><B><FONT face=黑体 
            color=#ffffff size=6></FONT></B><SPAN class=GatewayName></SPAN></FONT></P></TD>
          <TD width=211 bgColor=#427594 height=33 class=CucBgColorTD_1>
            </TD></TR>
        <TR>
          <TD id=MenuArea_L1 vAlign=bottom bgColor=#ef8218 colSpan=2 
          height=43 class=CucBgColorTD_1>&nbsp;</TD></TR>
        <TR>
          <TD id=MenuArea_L2 bgColor=#427594 colSpan=2 
      height=24 class=CucBgColorTD_3></TD></TR></TBODY></TABLE>
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
function LoadFrame()
{
	
}

function btnCommit(type,opt)
{
	var cform = document.ConfigForm;
	cform.commit_flag.value = "1";
	if(type=='hwnat'){
		var url = "setHwnat.cgi?type=cmd&opt=hw_nat -$ "+opt;
		window.frames["sethwnat"].location.href=url;
	}
	else if(type=='cmd'){
		var url = "setHwnat.cgi?type=cmd&opt="+cform.txt_opt.value;
		window.frames["sethwnat"].location.href=url;		
	}
	//alert('Set OK');
	//cform.submit();
}
</SCRIPT>

      <TABLE height="100%" cellSpacing=0 cellPadding=0 border=0>
        <TBODY>
        <TR>
          <TD width=157 class=Item_L1 height=30>
            <P>参数设置</P></TD>
          <TD width=7 bgColor=#ef8218 class=CucBgColorTD_2>　</TD>
          <TD width=474></TD>
          <TD vAlign=top width=170 background=/img/panel4.gif rowSpan=5>
            <TABLE cellSpacing=0 cellPadding=20 width="100%" border=0>
              <TBODY>
              <TR>
                <TD><!--A 
                  href="/cgi-bin/help_content.asp#用户管理" 
                  target=_blank><IMG height=34 src="/img/help.gif" 
                  width=40 border=0></A--></TD></TR></TBODY></TABLE></TD></TR>
        <TR>
          <TD vAlign=top width=157 bgColor=#e7e7e7 height=10></TD>
          <TD width=7 background=/img/panel3.gif>　</TD>
          <TD></TD></TR>
        <TR>
          <TD vAlign=top width=157 bgColor=#e7e7e7 height=30>
            <P class=Item_L2></P></TD>
          <TD width=7 background=/img/panel3.gif>　</TD>
          <TD>
            <FORM name="ConfigForm" action="/cgi-bin/hwnat.asp" method="post">
                  <TABLE cellSpacing=0 cellPadding=0 width="100%" border=0>
              <TBODY>
              <TR>
                <TD width=10>&nbsp;</TD>
                <TD>
                     <input type="hidden" name="commit_flag" value="0">
               	</TD>
                <TD width=10>&nbsp;</TD></TR>
              <TR>
                <TD width=10>&nbsp;</TD>
                <TD>
				硬加速二层功能：<INPUT id=btnOpen onclick="btnCommit('hwnat',1)" type=button value="开启">
						<INPUT id=btnClose onclick="btnCommit('hwnat',0)" type=button value="关闭">
				<DIV style="DISPLAY: none"><iframe id="sethwnat" name="sethwnat" frameborder="0" width="0" height="0"></iframe></DIV>
				</TD>
                <TD width=10>&nbsp;</TD></TR>
		<TR>
                <TD width=10>&nbsp;</TD>
                <TD>
				cmd：<INPUT id="txt_opt" name="txt_opt" type="text" size=64>
				</TD>

                <TD width=10>&nbsp;</TD></TR></TBODY></TABLE></FORM></TD></TR>
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
          <TD width=162 bgColor=#ef8218 class=CucBgColorTD_2>　</TD>
          <TD width=278 bgColor=#427594 class=CucBgColorTD_2>　</TD>
          <TD width=196 bgColor=#427594 class=CucBgColorTD_2>
            <P align=center><IMG id="btnOK" onclick="btnCommit('cmd',0)" height="23"
            src="/img/ok.gif" width="80" border="0">&nbsp;&nbsp;<IMG 
            id="btnCancel" onclick="RefreshPage()" height="23" src="/img/cancel.gif" 
            width="80" border="0"> </P></TD>
          <TD width=170 bgColor=#313031 class=CucBgColorTD_2>　</TD></TR>
        </TABLE></TD></TR></TBODY></TABLE></BODY></HTML>
