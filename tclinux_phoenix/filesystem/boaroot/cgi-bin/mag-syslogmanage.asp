<%
If Request_Form("loglevelchange")="1" then 
	TCWebApi_set("SysLog_Entry","WriteLevel","loglevel")
	TCWebApi_set("SysLog_Entry","LogEnable","Enable")
	TCWebApi_commit("SysLog_Entry")
	TCWebApi_save()
End If
%>

<%
if Request_form("frameloglevel") <> "" then
		TCWebApi_set("SysLog_Entry","DisplayLevel","frameloglevel")
end if
%>


<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0 Transitional//EN">
<HTML><HEAD><TITLE>��־�ļ�����</TITLE>
<META http-equiv=Content-Language content=zh-cn>
<META http-equiv=Content-Type content="text/html; charset=gb2312"><LINK 
href="/JS/stylemain.css" type=text/css rel=stylesheet>

<SCRIPT language=javascript src="/JS/menu.js"></SCRIPT>

<SCRIPT language=javascript src="/JS/util.js"></SCRIPT>
<SCRIPT language=javascript>

function SubmitChange(){
	document.ConfigForm.loglevelchange.value = "1"; 
	document.ConfigForm.loglevel.value = document.ConfigForm.Level.value;
	document.ConfigForm.submit();
}

function Back_Syslog()
{
   var cfg = '/syslog.txt';
	 var code ;
	 if (document.ConfigForm.logenable.value == "Yes")
	 {
	     code = 'location.assign("'+cfg+'")';
	     eval(code);
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
                <TD class=welcom vAlign=bottom align=middle width=120>��ӭ���� </TD>
                <TD vAlign=bottom width=50><A onclick=DoLogout() 
                  href="/cgi-bin/logout.cgi" target=_top><SPAN 
                  class=logout>�˳� 
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
            <P align=right><FONT face=���� color=#ffffff><B><FONT face=���� 
            color=#ffffff size=6><INPUT id=Selected_Menu type=hidden 
            value="����->��־�ļ�����" name=Selected_Menu> </FONT></B><SPAN 
            class=GatewayName>��������:
            <SCRIPT language=javascript>
			document.write(top.gateWayName);
			</SCRIPT>
             </SPAN></FONT></P></TD>
          <TD width=211 bgColor=#d8d8d8 height=33>
            <P class=GatewayType align=center>�ͺ�:
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
          <TD height=15><IMG height=15 src="/img/panel1.gif" width=164 
            border=0> </TD>
          <TD><IMG height=15 src="/img/panel2.gif" width=645 border=0> 
          </TD></TR></TBODY></TABLE></TD></TR>
  <TR>
    <TD vAlign=top>
    <SCRIPT language=JavaScript type=text/javascript>
		function LoadFrame()
		{
			return;
		}
		function SetLogEnbl()
		{
			alert("ע��:���ڿ�����־��������������!");
		}
	</SCRIPT>
      <TABLE height="100%" cellSpacing=0 cellPadding=0 border=0>
	    <FORM name="ConfigForm" action="/cgi-bin/mag-syslogmanage.asp" method="post" >
		<INPUT TYPE="HIDDEN" NAME="loglevel" VALUE="<%TCWebApi_get("SysLog_Entry","WriteLevel","s")%>">
		<INPUT TYPE="HIDDEN" NAME="loglevelchange" VALUE="0">
		<INPUT TYPE="HIDDEN" NAME="logenablechange" VALUE="0">
		<INPUT TYPE="HIDDEN" NAME="logenable" VALUE="<% TCWebApi_get("SysLog_Entry","LogEnable","s") %>">
		<INPUT TYPE="HIDDEN" NAME="frameloglevel">
        <TBODY>
        <TR>
          <TD width=157 bgColor=#e7e7e7 height=30>
            <P class=Item_L1>д��ȼ�����</P></TD>
          <TD width=7 bgColor=#e7e7e7>��</TD>
          <TD width=474>��</TD>
          <TD vAlign=top width=170 background=/img/panel4.gif 
            rowSpan=11><TABLE cellSpacing=0 cellPadding=20 width="100%" 
              border=0 height='100%'><TBODY>
              <TR>
                <TD valign='top'><A href="/cgi-bin/help_content.asp#��־�ļ�����" 
                  target=_blank><IMG height=34 src="/img/help.gif" 
                  width=40 border=0></A></TD></TR>
<% if tcwebApi_get("WebCustom_Entry","isCYE8SFUSupported","h") = "Yes" then %>
              <TR>
                <TD valign='bottom'><IMG src='/img/tybottom.jpg' height=76 width=112></TD></TR>
<% end if %>
              </TBODY></TABLE></TD></TR>
        <TR>
          <TD vAlign=top width=157 bgColor=#e7e7e7 height=10></TD>
          <TD width=7 background=/img/panel3.gif>��</TD>
          <TD></TD></TR>
        <TR>
          <TD vAlign=top width=157 bgColor=#e7e7e7 height=30>
            <P class=Item_L2></P></TD>
          <TD width=7 background=/img/panel3.gif>��</TD>
          <TD>
            <TABLE cellSpacing=0 cellPadding=0 width="100%" border=0>
              <TBODY>
              <TR>
                <TD width=10>&nbsp;</TD>
                <TD>
                  <TABLE cellSpacing=0 cellPadding=0 border=0>
                    <TBODY>
                    <TR>
                      <TD width=80>��־:</TD>
                      <TD><INPUT type="radio"  name="Enable" VALUE="No" <% If TCWebApi_get("SysLog_Entry","LogEnable","h") = "No" then asp_Write("checked") end if %> >����</TD>
                      <TD><INPUT onclick="SetLogEnbl()" value="Yes" type="radio" name="Enable" <% If TCWebApi_get("SysLog_Entry","LogEnable","h") = "Yes" then asp_Write("checked") end if %>  > ����</TD></TR></TBODY></TABLE><BR>
                  <TABLE cellSpacing=0 cellPadding=0 border=0>
                    <TBODY>
                    <TR>
                      <TD>��־�ȼ�:</TD>
                      <TD colSpan=2>
					  <SELECT id="Level" size="1" name="Level" > 
					  
<script language="JavaScript" type='text/javascript'>
	var agearr = new Array(8);
	agearr[0]="����";
	agearr[1]="����";
	agearr[2]="��Ҫ";
	agearr[3]="����";
	agearr[4]="����";
	agearr[5]="ע��";
	agearr[6]="֪ͨ";
	agearr[7]="����";
	
	for(i=0;i<8;i++)
	{
		if(document.ConfigForm.loglevel.value == i)
		{
			document.ConfigForm.Level[i]=new Option(agearr[i],i,false,false);
			document.ConfigForm.Level[i].selected=true;
		}
		else
		{
		 	if(agearr[i] == "����")
		 	{
				document.ConfigForm.Level[i]=new Option(agearr[i],i,false,false);
				document.ConfigForm.Level[i].selected=true;
			}
			else		
			{
				document.ConfigForm.Level[i]=new Option(agearr[i],i,false,false);
				document.ConfigForm.Level[i].selected=false;
			}
		}
	}
</SCRIPT>					  
				  </SELECT></TD></TR></TBODY></TABLE>
                  <DIV id=srvInfo style="DISPLAY: none">
                  <TABLE cellSpacing=0 cellPadding=0 border=0>
                    <TBODY>
                    <TR>
                      <TD width=120>������IP��ַ:</TD>
                      <TD><INPUT id=MainServer maxLength=15 size=15 
                        name=MainServer></TD></TR>
                    <TR>
                      <TD>������UDP�˿�:</TD>
                      <TD><INPUT maxLength=5 size=5 
                    name=MainServerPort></TD></TR></TBODY></TABLE></DIV>
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
          <TD width=7 background=/img/panel3.gif>��</TD>
          <TD></TD></TR>
        <TR>
          <TD width=157 bgColor=#e7e7e7 height=30>
            <P class=Item_L1>��־</P></TD>
          <TD width=7 bgColor=#e7e7e7>��</TD>
          <TD width=474>��</TD></TR>
        <TR>
          <TD vAlign=top width=157 bgColor=#e7e7e7 height=10></TD>
          <TD width=7 background=/img/panel3.gif>��</TD>
          <TD></TD></TR>
        <TR>
          <TD vAlign=top width=157 bgColor=#e7e7e7 height=30>
            <P class=Item_L2></P></TD>
          <TD width=7 background=/img/panel3.gif>��</TD>
          <TD>
            <TABLE cellSpacing=0 cellPadding=0 width="100%" border=0>
              <TBODY>
              <TR>
							<iframe src="/cgi-bin/syslog.cgi" frameborder="0" scrolling="no" width="474" height="580" align=left></iframe></TR>
							<TR><TD>
							<P align=center>
          		<input type="button" value="������־"  name="SysLogSave"  onClick="Back_Syslog()" >
          <BR></P></TD></TR></TBODY></TABLE></TD></TR>
        <TR>
          <TD vAlign=top width=157 bgColor=#e7e7e7 height=10></TD>
          <TD width=7 background=/img/panel3.gif> ��</TD>
          <TD></TD></TR>
				<TR>
          <TD width=157 bgColor=#e7e7e7 height=30>
            <P class=Item_L1>ά��</P></TD>
          <TD bgColor=#e7e7e7>��</TD>
          <TD></TD></TR>          	
        <TR>
          <TD vAlign=top width=157 bgColor=#e7e7e7 height=10></TD>
          <TD width=7 background=/img/panel3.gif> ��</TD>
          <TD><P align=center>
          <input type="button" value="ά�������ϱ�"  name="buttonmaintend"  onClick="window.location.href='/cgi-bin/maintainreport.cgi';" >
          <BR></P></TD></TR>
        <TR>
          <TD vAlign=top width=157 bgColor=#e7e7e7></TD>
          <TD width=7 background=/img/panel3.gif>��</TD>
          <TD></TD></TR></TBODY></FORM></TABLE></TD></TR>
  <TR>
    <TD height=1>
      <TABLE id=table7 height=35 cellSpacing=0 cellPadding=0 width=808 
        border=0><TBODY>
        <TR>
          <TD width=162 bgColor=#d8d8d8>��</TD>
          <TD width=278 bgColor=#0059A0>��</TD>
          <TD width=196 bgColor=#0059A0>
            <P align=center><IMG id=btnOK onclick=SubmitChange() height=23 
            src="/img/ok.gif" width=80 border=0>&nbsp;&nbsp;<IMG 
            id=btnCancel onclick="RefreshPage()" height=23 src="/img/cancel.gif" 
            width=80 border=0> </P>
			</TD>
          <TD width=170  bgColor=#313031>��</TD></TR></TBODY></TABLE></TD></TR></TBODY></TABLE></BODY></HTML>
