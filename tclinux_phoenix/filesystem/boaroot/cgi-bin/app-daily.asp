<%
if TCWebApi_get("WebCustom_Entry","isCTUsbSupported","h" ) = "Yes" then
if Request_Form("Down_Flag") = "1" then
	TCWebApi_set("appFTP_Downloading","username","getUserName") 
	TCWebApi_set("appFTP_Downloading","password","getPassWord") 
	TCWebApi_set("appFTP_Downloading","url","DownLoadUrl") 
	TCWebApi_set("appFTP_Downloading","port","Port") 
	TCWebApi_set("appFTP_Downloading","deviceid","getClDevType") 
	TCWebApi_set("appFTP_Downloading","savepath","getLocalPath") 
	TCWebApi_commit("appFTP_Downloading")
end if
end if
if Request_Form("ApplyFlag") = "1" then

	TCWebApi_set("WanInfo_Common","CurIFIndex","curSetIndex")
	TCWebApi_commit("WanInfo_Common")		

	tcWebApi_get("WanInfo_WanIF","MulticastVID","h")

	TCWebApi_set("WanInfo_WanPVC","Action","OperatorStyle")
	
	TCWebApi_set("WanInfo_WanIF","MulticastVID","McVlanID")
	TCWebApi_commit("WanInfo_WanPVC")
	TCWebApi_save()
end if
%>

<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0 Transitional//EN">
<HTML><HEAD><TITLE>�ճ�Ӧ��</TITLE>
<META http-equiv=Content-Language content=zh-cn>
<META http-equiv=Content-Type content="text/html; charset=gb2312"><LINK 
href="/JS/stylemain.css" type=text/css rel=stylesheet>

<SCRIPT language=javascript src="/JS/menu.js"></SCRIPT>
<% if TCWebApi_get("WebCustom_Entry","isCTUsbSupported","h" ) = "Yes" then %>
<SCRIPT language=javascript type=text/javascript src="/cgi-bin/showhis.cgi"></SCRIPT>
<SCRIPT language=JavaScript type=text/javascript src="/cgi-bin/showusb.cgi"></SCRIPT>
<% end if %>
<SCRIPT language=javascript src="/JS/util.js"></SCRIPT>
<SCRIPT language=JavaScript src="/JS/printtable.js"></SCRIPT>
<SCRIPT language=javascript>
function LoadFrame()
{
//getusb();
	with(document.forms[0])
	{
//		if (ClDevType.value == "N/A")
//			btnDown.disabled = true;
		var maxjobvalue = "<%TCWebApi_get("appFTP_Downloading","maxjob","s")%>";
		if(DownloadingCount>=parseInt(maxjobvalue) || ClDevType.value=="N/A")
			btnDown.disabled=true;
	}
	
	document.IPTVForm.McVlanID.value = "";
	
	if (curUserName != sptUserName)
	{
		document.getElementById("iptvTR1").style.display = "none";
		document.getElementById("iptvTR2").style.display = "none";
		document.getElementById("iptvTR3").style.display = "none";
	}

	autoRefreshMainTable("mainTableSet");
}
//////////////////
 function SetCwinHeight() {   

   var iframeid = document.getElementById("showhis"); //iframe id   
   iframeid.height = "0px";//�ȸ�һ����С�ĳ�ֵ,Ȼ���ٳ���.   
  if (document.getElementById) {   
  if (iframeid && !window.opera) {   
        if (iframeid.contentDocument && iframeid.contentDocument.body.offsetHeight) {   
          iframeid.height = iframeid.contentDocument.body.offsetHeight;   
          } else if (iframeid.Document && iframeid.Document.body.scrollHeight) {   
          iframeid.height = iframeid.Document.body.scrollHeight;   
       }   
 }   
 }   
 }
 //////////////////////////////////////////
function SubmitFresh()
{
location.replace("/cgi-bin/app-daily.asp");
//center.window.location.reload();
}
///////////////////////////////
function	btnSubmit()
{
    if (CheckForm() == true)
	{
	   	var Form = document.ConfigForm;
//		getusb();
		Form.Down_Flag.value = "1";
		Form.LocalPath.value = Form.LocalPath.value.replace(/\\/g, '/'); 
		Form.LocalPath.value = Form.LocalPath.value.replace(/(^\s*)|(\s*$)/g, "");
//		Form.LocalPath.value = Form.LocalPath.value.replace(/'/g, "'\\''");
/*		var subdoc;		
        if (document.all){//IE
                subdoc = document.frames["usbdev"].document;
        }else{//Firefox    
                subdoc = document.getElementById("usbdev").contentDocument;
        }
		document.forms[0].ClDevType.value=subdoc.getElementById("ClDevType").value;*/
		if (Form.UserName.value == '') 
		{
			Form.getUserName.value="anonymous";
		}
		else
		{
			Form.getUserName.value=Form.UserName.value;
		}
		if (Form.PassWord.value == '') 
		{
			Form.getPassWord.value="anonymous";
		}
		else
		{
			Form.getPassWord.value=Form.PassWord.value;
		}
//		Form.getUserName.value = Form.getUserName.value.replace(/'/g, "'\\''");
//		Form.getPassWord.value = Form.getPassWord.value.replace(/'/g, "'\\''");
		Form.getClDevType.value=Form.ClDevType.value;
		Form.getLocalPath.value=Form.LocalPath.value;
		Form.ClDevType.disabled=true;
		Form.LocalPath.disabled=true;
		
		Form.submit();
	}

}
///////////////////////////////////
/*function getusb(){
        var doc;		
        if (document.all){//IE
                doc = document.frames["usbdev"].document;
        }else{//Firefox    
                doc = document.getElementById("usbdev").contentDocument;
        }
        document.forms[0].ClDevType.value=doc.getElementById("ClDevType").value;
		if(document.forms[0].ClDevType.value=="" || document.forms[0].ClDevType.value=="N/A")
		{
		document.forms[0].btnDown.disabled=true;
		doc.getElementById("ClDevType").disabled=true;
		}
//		alert(document.forms[0].ClDevType.value);
}*/
/////////////////////////////////////
function checkFtpClient() {
with( document.forms[0] )
{
if ( (Port.value !='') &&(isNaN(parseInt(Port.value )) == true))
{
alert('��Ч�˿�');
return false;
}
var info = parseFloat(Port.value );
if (info < 1 || info > 65535){
alert('��Ч�˿�,Ӧ��Ϊ<1-65535>');
return false;
}

//can not input the mark:"'"
var testQuote=new RegExp("[']");
if (UserName.value.length > 20) {
alert('�û���̫����ӦС��20�ַ�');
return false;
}
if (testQuote.test(UserName.value)==true) {
alert('�û���������Ч�ַ�');
return false;
}
/*if (UserName.value == '') {
UserName.value="anonymous";
return false;
}*/
if (isValidString(UserName.value) == false ) {
msg = '�û�����������Чֵ:���������ַ�';
alert(msg);
return false;
}
if (PassWord.value.length > 20) {
alert('����̫����ӦС��20�ַ�');
return false;
}
if (testQuote.test(PassWord.value)==true) {
alert('���������Ч�ַ�');
return false;
}

if ( isValidString(PassWord.value) == false ) {
msg = '���������Ч�ַ�';
alert(msg);
return false;
}
var tmp = LocalPath.value;
var testpath=new RegExp("[:*?\"<>\|']");
if (tmp.length > 40)
{
alert("�����ļ�·��̫��");
return false;
}
if (tmp.indexOf("..") >= 0 || tmp == '\\' || tmp.charAt(0) == '/'
|| tmp.charAt(tmp.length-1) == '/' || testpath.test(tmp)==true)
{
msg = '�ǺϷ��ļ�·��';
alert(msg);
return false;
}
if (DownLoadUrl.value.length > 128) {
alert('����url̫����ӦС��128�ַ�');
return false;
}
if (testQuote.test(DownLoadUrl.value)==true) {
alert('�ǺϷ�ftp��URL');
return false;
}
if (DownLoadUrl.value == '' || DownLoadUrl.value.substr(0,6) != "ftp://" || DownLoadUrl.value.length <= 6 ||DownLoadUrl.value.length > 512)
{
msg = '�ǺϷ�ftp��URL';
alert(msg);
return false;
}
var patt1=new RegExp("^ftp://[^@ ]+/.*[^/]$");
if(patt1.test(DownLoadUrl.value)==false)
{
msg = '�ǺϷ�ftp��URL';
alert(msg);
return false;
}
}
//var Form = document.ConfigForm;
//Form.Down_Flag.value = "1";
return true;
}
/////////////////////////////////////
function CheckForm()
{
return checkFtpClient();
}
///////////////////////////////////
function showPathText(val)
{
var lineLength = 8;
var content = '';
var index = 0;
var len = val.length;
while (len > lineLength)
{
content += val.substr(index,lineLength) + '<br>';
len -= lineLength;
index += lineLength;
}
content += val.substr(index);
return content;
}
////////////////////////////////////////////
//function AddSubmitParam(Form,type)
//{
//setDisable('ClDevType',1);
//setDisable('LocalPath',1);
//Form.addDiv('ftpClient','x');
//}
		var pvc_counts=<% tcWebApi_get("WanInfo_Common","CycleNum","s") %>;
	
		//get all value
		// num 0
		var vArrayStr = "<% tcWebApi_get("WanInfo_Common","CycleValue","s") %>";
		var Wan_WanName = vArrayStr.split(',');
		// num 1
		vArrayStr = "<% tcWebApi_get("WanInfo_Common","CycleValue","s") %>";
		// num 2
		vArrayStr = "<% tcWebApi_get("WanInfo_Common","CycleValue","s") %>";
		var Wan_IFIndex = vArrayStr.split(',');
		// num 3
		vArrayStr = "<% tcWebApi_get("WanInfo_Common","CycleValue","s") %>";
		Wan_Actives = vArrayStr.split(',');
		// num 4		
		vArrayStr = "<% tcWebApi_get("WanInfo_Common","CycleValue","s") %>";
		// num 5		
		vArrayStr = "<% tcWebApi_get("WanInfo_Common","CycleValue","s") %>";
		// num 6		
		vArrayStr = "<% tcWebApi_get("WanInfo_Common","CycleValue","s") %>";
		// num 7		
		vArrayStr = "<% tcWebApi_get("WanInfo_Common","CycleValue","s") %>";
		// num 8		
		vArrayStr = "<% tcWebApi_get("WanInfo_Common","CycleValue","s") %>";
		// num 9		
		vArrayStr = "<% tcWebApi_get("WanInfo_Common","CycleValue","s") %>";
		var Wan_ENCAP = vArrayStr.split(',');
		// num 10
		vArrayStr = "<% tcWebApi_get("WanInfo_Common","CycleValue","s") %>";
		// num 11		
		vArrayStr = "<% tcWebApi_get("WanInfo_Common","CycleValue","s") %>";
		// num 12		
		vArrayStr = "<% tcWebApi_get("WanInfo_Common","CycleValue","s") %>";
		// num 13		
		vArrayStr = "<% tcWebApi_get("WanInfo_Common","CycleValue","s") %>";
		// num 14		
		vArrayStr = "<% tcWebApi_get("WanInfo_Common","CycleValue","s") %>";
		// num 15		
		vArrayStr = "<% tcWebApi_get("WanInfo_Common","CycleValue","s") %>";
		// num 16		
		vArrayStr = "<% tcWebApi_get("WanInfo_Common","CycleValue","s") %>";
		// num 17		
		vArrayStr = "<% tcWebApi_get("WanInfo_Common","CycleValue","s") %>";
		// num 18		
		vArrayStr = "<% tcWebApi_get("WanInfo_Common","CycleValue","s") %>";
		// num 19		
		vArrayStr = "<% tcWebApi_get("WanInfo_Common","CycleValue","s") %>";
		// num 20		
		vArrayStr = "<% tcWebApi_get("WanInfo_Common","CycleValue","s") %>";
		// num 21		
		vArrayStr = "<% tcWebApi_get("WanInfo_Common","CycleValue","s") %>";
		// num 22		
		vArrayStr = "<% tcWebApi_get("WanInfo_Common","CycleValue","s") %>";
		// num 23		
		vArrayStr = "<% tcWebApi_get("WanInfo_Common","CycleValue","s") %>";
		var Wan_VidPRI = vArrayStr.split(',');
		// num 24		
		vArrayStr = "<% tcWebApi_get("WanInfo_Common","CycleValue","s") %>";
		// num 25	
		vArrayStr = "<% tcWebApi_get("WanInfo_Common","CycleValue","s") %>";
		var Wan_McVid = vArrayStr.split(',');

		Wan_WanName = SortUtil.UpdateWanIFName( Wan_WanName, pvc_counts );

		Wan_Actives = SortUtil.SortMyArray( Wan_Actives );
		Wan_IFIndex = SortUtil.SortMyArray( Wan_IFIndex );
		Wan_VidPRI = SortUtil.SortMyArray( Wan_VidPRI );
		Wan_ENCAP = SortUtil.SortMyArray( Wan_ENCAP );
		Wan_McVid = SortUtil.SortMyArray( Wan_McVid );
				
function ListCreate_iptv()
{
	for( var i=0; i<pvc_counts; i++ )
			{
				if( Wan_Actives[i] != 'N/A' )
				{
					if(Wan_WanName[i].indexOf('_INTERNET_') > -1 || Wan_WanName[i].indexOf('_OTHER_') > -1)
					{
						document.write('<tr>');
						document.write("<td><div align=center><input TYPE='checkbox' NAME='chk_" + i + "' ID='chk_" + i + "_" + Wan_IFIndex[i] + "' onclick='selectChkBox(this.id)'></div></td>");
						document.write('<td align=middle colspan=2>' + Wan_WanName[i] + '</td>');
						document.write('<td align=middle>' + Wan_VidPRI[i] + '</td>');
						if(Wan_WanName[i].indexOf('_R_') > -1)
							document.write('<td align=middle>' + Wan_ENCAP[i] + '&nbsp;</td>');
						else
							document.write('<td align=middle>-&nbsp;</td>');
						document.write('</tr>');
					}
				}
			}
}

function selectChkBox(chkId)
{
	var allElements = document.getElementsByTagName("input");
	document.getElementById(chkId).checked = true;

	for(var i = 0;i < allElements.length; i++)
	{
		if(allElements[i].type == "checkbox" && allElements[i].id != chkId)
			allElements[i].checked = false;
	}
	
	var WanIndex = chkId.split('_');
	document.IPTVForm.curSetIndex.value = WanIndex[2];

	if(Wan_McVid[WanIndex[1]] != "-")
		document.IPTVForm.McVlanID.value = Wan_McVid[WanIndex[1]];
	else
		document.IPTVForm.McVlanID.value = "";
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
            value="Ӧ��->�ճ�Ӧ��" name=Selected_Menu> </FONT></B><SPAN 
            class=GatewayName>��������:
            <SCRIPT 
            language=javascript>
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
      

      <TABLE height="100%" cellSpacing=0 cellPadding=0 border=0 id="mainTableSet">
        <TBODY>
<% if TCWebApi_get("WebCustom_Entry","isCTUsbSupported","h" ) = "Yes" then %>
        <TR>
          <TD width=157 bgColor=#e7e7e7 height=30>
            <P class=Item_L1>��ͥ�洢</P></TD>
          <TD width=7 bgColor=#e7e7e7>��</TD>
          <TD width=474>��</TD>
          <TD vAlign=top width=170 background=/img/panel4.gif rowSpan=7 id='TRSpan'>
            <TABLE cellSpacing=0 cellPadding=20 width="100%" border=0 height='100%'>
              <TBODY>
              <TR>
                <TD valign='top'><A 
                  href="/cgi-bin/help_content.asp#�ճ�Ӧ��" 
                  target=_blank><IMG height=34 src="/img/help.gif" 
                  width=40 border=0></A></TD></TR>
<% if tcwebApi_get("WebCustom_Entry","isCYE8SFUSupported","h") = "Yes" then %>
              <TR>
                <TD valign='bottom'><IMG src='/img/tybottom.jpg' height=76 width=112></TD></TR>
<% end if %>
              </TBODY></TABLE>�� �� ��</TD></TR>
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
                      <TD> <FORM name=ConfigForm action="/cgi-bin/app-daily.asp" method="post">
            <TABLE cellSpacing=0 cellPadding=0 width="100%" border=0>
              <TBODY>
              <TR>
                <TD width=10>&nbsp;</TD>
                <TD><SPAN class=Item_L2>ftp client����</SPAN><BR><BR>
                  <DIV id=ftpClient>
                  <TABLE cellSpacing=0 cellPadding=0 border=0>
                    <TBODY>
                    <TR>
                      <TD width=150>�û���</TD>
                      <TD><INPUT name=UserName style="WIDTH: 150px"></TD></TR>
                    <TR>
                      <TD width=150>����</TD>
                      <TD><INPUT style="WIDTH: 150px" type=password 
                        name=PassWord></TD></TR>
                    <TR>
                      <TD width=150>����url</TD>
                      <TD><INPUT style="WIDTH: 150px" value=ftp:// 
                        name=DownLoadUrl></TD></TR>
                    <TR>
                      <TD width=150>�˿�</TD>
                      <TD><INPUT style="WIDTH: 150px" value=21 
name=Port></TD></TR>
                    <TR>
                      <TD>�豸</TD>
                      <TD>
  <SELECT id=ClDevType style="WIDTH: 150px" name=ClDevType>
   <SCRIPT language=JavaScript type=text/javascript>
var i;
for(i=0;i<usbvalue.length;i++)
{
var t1 =usbvalue[i].split("usb");  
var t2 =t1[1].split("_");
document.write("<option value=\""+usbvalue[i]+"\">USB"+t2[0]+"-"+t2[1]+"&nbsp;����</option>");
}
if(usbvalue.length==0)
	document.write("<option value=\"N/A\">��USB�豸</option>");

</SCRIPT>  
</SELECT> </TD></TR>
                    <TR>
                      <TD width=150>����·��</TD>
                      <TD><INPUT style="WIDTH: 150px" name=LocalPath id=LocalPath>
					  </TD></TR></TBODY></TABLE></DIV>
					<DIV id=ftpnotice><BR>
                  <TABLE cellSpacing=0 cellPadding=0 border=0>
                    <TBODY>
                    <TR>
                      <TD>ע�⣺������ɺ���ȴ��嵽ʮ���ٰβ�USB�豸</TD></TR></TBODY></TABLE></DIV><BR><BR><B>���ʮ�����ؼ�¼</B> 
<INPUT id=btnDown2 onclick=SubmitFresh() type=button value=״̬ˢ�� name=btnDown2> 
                  <input type="hidden" name="getUserName" value="0">
				  <input type="hidden" name="getPassWord" value="0">
				  <input type="hidden" name="getClDevType" value="0">
				  <input type="hidden" name="getLocalPath" value="0">
				  <input type="hidden" name="Down_Flag" value="0"><BR><BR>
<SCRIPT language=JavaScript type=text/javascript>
		var DownloadInfo = new Array();
		DownloadInfo[0]=new Array("", "", "", "20%", "15%", "15%", "20%", "20%", "10%","","");
		DownloadInfo[1]=new Array("","", "", "�û���", "����", "�˿�", "����URL","����·��","״̬");
		DownloadInfo[2]=new Array("", "", "", "", "", "", "","","","");
		var i=0;
		var DownloadingCount=0;
		for(;i<DownEntry.length;i++)
		{
			DownloadInfo[i+3]=DownEntry[i];
			if(DownEntry[i][8]=="Downloading")
				DownloadingCount++;
		}
		DownloadInfo[i+3]=new Array();
		printVTable('DownloadInfo',400);

</SCRIPT>                 
<BR>
<BR>
				   
                  <CENTER><INPUT id=btnDown onclick="btnSubmit()" type=button value=���� name=btnDown> 
                  </CENTER>
                  </TD>
                <TD width=10>&nbsp;</TD></TR>
              </TBODY></TABLE>
				</FORM>
</TD>
                <TD width=10>&nbsp;</TD></TR></TBODY></TABLE></TD></TR>
<% end if %>
<TR id="iptvTR1">
          <TD width=157 bgColor=#e7e7e7 height=30>
            <P class=Item_L1>IPTV</P></TD>
          <TD width=8 bgColor=#e7e7e7>��</TD>
          <TD width=474>��</TD>
<% if TCWebApi_get("WebCustom_Entry","isCTUsbSupported","h" ) <> "Yes" then %>
          <TD vAlign=top width=170 background=/img/panel4.gif rowSpan=4 id='TRSpan'>
            <TABLE cellSpacing=0 cellPadding=20 width="100%" border=0>
              <TBODY>
              <TR>
                <TD><A 
                  href="/cgi-bin/help_content.asp#�ճ�Ӧ��" 
                  target=_blank><IMG height=34 src="/img/help.gif" 
                  width=40 border=0></A></TD></TR></TBODY></TABLE></TD>
<% end if %></TR>
        <TR id="iptvTR2" vAlign=top>
          <TD vAlign=top width=157 bgColor=#e7e7e7></TD>
          <TD width=8 background=/img/panel3.gif>��</TD>
          <TD>
              <FORM name="IPTVForm" action="/cgi-bin/app-daily.asp" method="post">
        <TABLE cellSpacing=0 cellPadding=0 width="100%" border=0>
              <TBODY>
              <TR>
                <TD width=20>&nbsp;</TD>
                <TD>
                  <P>����������WAN�� �鲥VLAN ID������ѡ����ӦWAN�ڽ����޸ġ�</P>
                      </TD>
              </TR>
              <TR>
              	<TD>&nbsp;</TD>
              	<TD>
              		<TABLE cellSpacing=0 cellPadding=0 width="98%" border=1>
              		<TBODY>
              		<TR>
                      <TD class=table_title align=middle width=10%>&nbsp;</TD>
                      <TD class=table_title align=middle width=40% colspan=2>��������</TD>
                      <TD class=table_title align=middle width=25%>VLAN/���ȼ�</TD>
                      <TD class=table_title align=middle width=25%>��ȡIP��ʽ</TD>
                  </TR>
                      <SCRIPT language=JavaScript type=text/javascript>
											ListCreate_iptv();
											</SCRIPT>
									<TR>
											<TD class=table_title colspan=2>�鲥VLAN ID:</TD>
											<TD colspan=3><INPUT id=McVlanID name=McVlanID style="WIDTH: 150px" value="<%if tcWebApi_get("WanInfo_WanIF","MulticastVID","h") <> "N/A" then tcWebApi_get("WanInfo_WanIF","MulticastVID","s") end if%>">(1-4094)</TD>
									</TR>
                	</TBODY>
                	</TABLE>
                </TD>
              </TR>
							<TR>
                <TD>&nbsp;</TD>
                <TD>&nbsp;</TD></TR>
							<TR>
                <TD align=center colspan=2>
									<BUTTON onclick="doApply()" type="button" name=btnApply value="Apply">Ӧ��</BUTTON>&nbsp;&nbsp;
									<BUTTON onclick="RefreshPage()" type="button" name=btnCancel value="Cancel">ȡ��</BUTTON>
									<input type="hidden" name="ApplyFlag">
									<input type="hidden" name="curSetIndex" value="<% tcWebApi_get("WanInfo_Common","CurIFIndex","s") %>">
									<input type="hidden" name="OperatorStyle" value="Modify">
								</TD></TR>
								<SCRIPT language=JavaScript type=text/javascript>
									function checkVlanID()
									{
										
										VlanID = document.IPTVForm.McVlanID.value;
										if((VlanID != "") && (isNaN(VlanID) || parseInt(VlanID) > 4094 || parseInt(VlanID) < 1))
										{
											alert("�鲥VLAN ID�Ƿ�.");
											return false;
										}
										
										var allElements = document.getElementsByTagName("input");
									
										for(var i = 0;i < allElements.length; i++)
										{
											if(allElements[i].type == "checkbox" && allElements[i].checked == true)
												return true;
										}
										
										return false;
									}
									function doApply()
									{
										if(checkVlanID() == false)
											return;
											
										document.IPTVForm.ApplyFlag.value = "1";
										document.IPTVForm.submit();
									}
									
								</SCRIPT>
              </TBODY>
        </TABLE>
        		</FORM>
					</TD></TR>
        <TR id="iptvTR3">
          <TD vAlign=top width=157 bgColor=#e7e7e7 height=10></TD>
          <TD width=7 background=/img/panel3.gif>��</TD>
          <TD></TD></TR>
        <TR>
          <TD vAlign=top width=157 bgColor=#e7e7e7></TD>
          <TD width=7 background=/img/panel3.gif>��</TD>
          <TD></TD></TR></TBODY></TABLE></TD></TR>
  <TR>
    <TD height=1>
      <TABLE id=table7 height=35 cellSpacing=0 cellPadding=0 width=808 
        border=0><TBODY>
        <TR>
          <TD width=162 bgColor=#d8d8d8>��</TD>
          <TD width=278 bgColor=#0059A0>��</TD>
          <TD width=196 bgColor=#0059A0>
		  <!-- InstanceEndEditable --></TD>
          <TD width=170 
bgColor=#313031>��</TD></TR></TBODY></TABLE></TD></TR></TBODY></TABLE></BODY></HTML>
