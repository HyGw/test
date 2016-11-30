<%
if Request_Form("Save_Flag") = "1" then
	TCWebApi_set("UrlFilter_Common","Action","Actionflag")
	TCWebApi_set("WebCurSet_Entry","url_filter_id","add_num")
	TCWebApi_set("UrlFilter_Entry","Activate","EnableUrlFilterFlag")
	TCWebApi_set("UrlFilter_Entry","URL","urlitem")
	
	TCWebApi_commit("UrlFilter")
	TCWebApi_save()
elseif Request_Form("Save_Flag") ="2" then
	TCWebApi_set("UrlFilter_Common","Action","Actionflag")
	TCWebApi_set("UrlFilter_Common","Activate","EnableUrlFilterFlag")
	TCWebApi_commit("UrlFilter")
	TCWebApi_save()
elseif Request_Form("Save_Flag") = "3" then
	TCWebApi_set("UrlFilter_Common","Action","Actionflag")
	TCWebApi_set("UrlFilter_Common","DeleteIndex","delnum")
	TCWebApi_commit("UrlFilter")
	TCWebApi_save()
elseif Request_Form("Save_Flag") = "4" then
	TCWebApi_set("UrlFilter_Common","Action","Actionflag")
	TCWebApi_set("UrlFilter_Common","Filter_Policy","FilterPolicy")
	TCWebApi_commit("UrlFilter")
	TCWebApi_save()
end if
%>

<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0 Transitional//EN">
<HTML><HEAD><TITLE>广域网访问设置</TITLE>
<META http-equiv=Content-Language content=zh-cn>
<META http-equiv=Content-Type content="text/html; charset=gb2312"><LINK 
href="/JS/stylemain.css" type=text/css rel=stylesheet>
<SCRIPT language=javascript src="/JS/menu.js"></SCRIPT>

<SCRIPT language=javascript src="/JS/util.js"></SCRIPT>

<META content="MSHTML 6.00.6000.16809" name=GENERATOR></HEAD>
<BODY style="TEXT-ALIGN: center" vLink=#000000 aLink=#000000 link=#000000 
leftMargin=0 topMargin=0 
onload="DisplayLocation(getElement('Selected_Menu').value);FinishLoad();if(getElById('UrlFilterForm') != null)LoadFrame()" 
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
          <TD width=434 bgColor=#427594 height=33>
            <P align=right><FONT face=黑体 color=#ffffff><B><FONT face=黑体 
            color=#ffffff size=6><INPUT id=Selected_Menu type=hidden 
            value="安全->广域网访问设置" name=Selected_Menu> </FONT></B><SPAN 
            class=GatewayName>网关名称:
            <SCRIPT 
            language=javascript>
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
      <SCRIPT 
      language=javascript>
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
      <TABLE height="100%" cellSpacing=0 cellPadding=0 border=0>
        <TBODY>
        <TR>
          <TD width=157 bgColor=#ef8218 height=30>
            <P class=Item_L1>广域网访问设置</P></TD>
          <TD width=7 bgColor=#ef8218>　</TD>
          <TD width=474>　</TD>
          <TD vAlign=top width=170 background=/img/panel4.gif 
          rowSpan=5>
            <TABLE cellSpacing=0 cellPadding=20 width="100%" border=0>
              <TBODY>
              <TR>
                <TD><A 
                  href="/cgi-bin/help_content.asp#广域网访问设置" 
                  target=_blank><IMG height=34 src="/img/help.gif" 
                  width=40 border=0></A></TD></TR></TBODY></TABLE></TD></TR>
        <TR>
          <TD vAlign=top width=157 bgColor=#e7e7e7 height=10></TD>
          <TD width=7 background=/img/panel3.gif>　</TD>
          <TD></TD></TR>
        <TR>
          <TD vAlign=top width=157 bgColor=#e7e7e7 height=30>
            <P class=Item_L2></P></TD>

          <TD width=7 background=/img/panel3.gif>　</TD>
          <TD>
            <FORM name="UrlFilterForm" action="/cgi-bin/sec-urlfilter.asp" method="post">
            <TABLE cellSpacing=0 cellPadding=0 width="100%" border=0>
              <TBODY>
              <TR>
                <TD width=10>&nbsp;</TD>
                        
                        <TD> <P> 
                            <input type="hidden" name="Save_Flag" value="0">
                            <input type="hidden" name="Actionflag" value="Del">
                            <input type="hidden" name="EnableUrlFilterFlag" value="<% tcWebApi_get("UrlFilter_Common","Activate","s") %>">
                            <input type="hidden" name="delnum" value="">
                            <input type="hidden" name="add_num" value="<% tcWebApi_get("UrlFilter","add_num","s") %>">
                            <input type="hidden" name="Url_num" value="<% tcWebApi_get("UrlFilter","Url_num","s") %>">
                            <script language="JavaScript" type="text/JavaScript">
var urlfltpolicy = "<% tcWebApi_get("UrlFilter_Common","Filter_Policy","s") %>";
var enbl = "<% tcWebApi_get("UrlFilter_Common","Activate","s") %>";
var urllist = [null]
var state = enbl;
function isValidUrlName(url)
{
var i=0;
var invalidArray = new Array();
invalidArray[i++] = "www";
invalidArray[i++] = "com";
invalidArray[i++] = "org";
invalidArray[i++] = "net";
invalidArray[i++] = "edu";
invalidArray[i++] = "www.";
invalidArray[i++] = ".com";
invalidArray[i++] = ".org";
invalidArray[i++] = ".net";
invalidArray[i++] = ".edu";
if (isValidAscii(url) != '')
{
return false;
}
for (i = 0; i < url.length; i++)
{
if (url.charAt(i) == '\\')
{
return false;
}
}
if (url == "")
{
return false;
}
if (url.length < 3)
{
return false;
}
for(j=0; j< invalidArray.length; j++)
{
if (url == invalidArray[j])
{
return false;
}
}
return true;
}
function btnAdd()
{
//var SubmitForm = new webSubmitForm();
	with ( document.forms[0] )
	{
		if(Url_num >= 100)
		{
			alert("最多能设置100条记录!")
			return;
		}
		if (urlitem.length > 31)
		{
			alert("URL长度不能超过31个字符!")
			return;
		}
		if(isValidUrlName(urlitem.value) == false)
		{
			alert("非法的URL格式，请重新输入.")
			return;
		}
		var str = urlitem.value;
		var i;
		if (-1 != (i = str.indexOf("http")))
		{
			if(-1 != str.indexOf("https"))
				str = str.substring(i+8);
			else str = str.substring(i+7);
		}
		urlitem.value = str;
		for(k = 0; k < urllist.length - 1; k++)
		{
			/*if(urllist[k][1].toLowerCase() == str.toLowerCase())
			{
				alert("重复的名字，请重新输入.")
				return;
			}*/
		}
//SubmitForm.addParameter('x.Url',str);
		
	}
//SubmitForm.setAction('addcfg.cgi?x=InternetGatewayDevice.X_ATP_Security.UrlFilter&'
//+ 'RequestFile=html/security/urlfilter.asp');
//SubmitForm.submit();
	var	vForm = document.forms[0];
	vForm.Actionflag.value = "Add";
	vForm.Save_Flag.value = "1";
	vForm.submit();

}
function btnRemove(rml)
{
	/*if (rml == null)
	{
		alert("rml is null")
		return;
	}
	//var SubmitForm = new webSubmitForm();
	with (document.forms[0])
	{
		if (rml.length > 0)
		{
			var i;
			for (i = 0; i < rml.length; i++)
			{
				if (rml[i].checked == true)
				{
					//SubmitForm.addParameter(rml[i].value,'');
					curUrlfilter.value = rml[i].value - 1;
				}
			}
		}
		else if (rml.checked == true)
		{
			//SubmitForm.addParameter(rml.value,'');
			curUrlfilter.value = rml[i].value - 1;
		}
	}*/
	//SubmitForm.setAction('del.cgi?RequestFile=html/security/urlfilter.asp');
	//SubmitForm.submit();
	
	document.UrlFilterForm.Actionflag.value = "Del";
	document.UrlFilterForm.Save_Flag.value = "3";
	document.UrlFilterForm.submit();
}
function LoadFrame()
{
	var vForm = document.UrlFilterForm;
	vForm.Save_Flag.value = "0";
	vForm.delnum.value = "";
	setCheck('enableFilter',enbl);
//setRadio('FilterPolicy',urlfltpolicy);
}
function SubmitForm()
{
/*var SubmitForm = new webSubmitForm();
if (state == "0" && getCheckVal("enableFilter") == 1)
{
SubmitForm.addParameter('InternetGatewayDevice.X_ATP_Security.X_ATP_UrlFilterEnabled',1);
}
if (state == "1" && getCheckVal("enableFilter") == 0)
{
SubmitForm.addParameter('InternetGatewayDevice.X_ATP_Security.X_ATP_UrlFilterEnabled',0);
}
SubmitForm.setAction('set.cgi?RequestFile=html/security/urlfilter.asp');
SubmitForm.submit();*/
	var vForm = document.UrlFilterForm;
	vForm.Save_Flag.value = "2";
	document.UrlFilterForm.Actionflag.value = "Status";
	if(vForm.enableFilter.checked)
		vForm.EnableUrlFilterFlag.value = "1";
	else vForm.EnableUrlFilterFlag.value = "0";
	vForm.submit();
	
}
function cliEnableBox(checkBox)
{
	if (checkBox.checked == true)
	{
		if (document.UrlFilterForm.EnableUrlFilterFlag.value == "1")
			setDisplay("Filter",1);
	}
	else
	{
		setDisplay("Filter",0);
	}
}
function ChangePolicy()
{
	if (urlfltpolicy == getRadioVal('FilterPolicy'))
		return;
	//var Form = new webSubmitForm();
	var FilterMode = getElById("FilterPolicy");
	if (FilterMode[0].checked == true)
	{
		if (confirm("改变过滤模式将切换所有过滤规则，您是否真的要改变过滤模式为黑名单？"))
		{
			//state = false;
			//Form.addParameter('InternetGatewayDevice.X_ATP_Security.X_ATP_UrlFilterPolicy',0);
				document.UrlFilterForm.Actionflag.value = "Filter";
				document.UrlFilterForm.Save_Flag.value = "4";
				document.UrlFilterForm.submit();
		}
		else
		{
			FilterMode[0].checked = false;
			FilterMode[1].checked = true;
			return;
		}
	}
	else if (FilterMode[1].checked == true )
	{
		if (confirm("改变过滤模式将切换所有过滤规则，您是否真的要改变过滤模式为白名单？"))
		{
			//state = true;
			//Form.addParameter('InternetGatewayDevice.X_ATP_Security.X_ATP_UrlFilterPolicy',1);
				document.UrlFilterForm.Actionflag.value = "Filter";
				document.UrlFilterForm.Save_Flag.value = "4";
				document.UrlFilterForm.submit();
		}
		else
		{
			FilterMode[0].checked = true;
			FilterMode[1].checked = false;
			return;
		}
	}
	//Form.setAction('set.cgi?RequestFile=html/security/urlfilter.asp');
	//Form.submit();
}

							</script>
                            <BR>
                          <TABLE>
                    <TBODY>
                    <TR>
                      <TD>使能: </TD>
                      <TD>&nbsp;&nbsp; <INPUT id=enableFilter onclick="cliEnableBox(this)" type=checkbox name="enableFilter" <%if tcWebApi_get("UrlFilter_Common","Activate","h") = "1" then asp_Write("checked") end if%>> </TD></TR></TBODY></TABLE>
                  <DIV id=Filter>设置URL过滤模式以及过滤名单。（可设置100条）<BR>单击“更改模式”按钮改变URL过滤模式；<BR>单击“添加”按钮增加一条URL到对应的URL列表；单击“删除”按钮删除一条URL，<BR>
                            URL不要带http://或https://前缀，URL长度不能超过31个字符。 <BR>
                  <SCRIPT language=JavaScript type=text/javascript>
				  setDisplay('Filter',enbl);
if(urlfltpolicy == "0")
{
document.write('当前的过滤列表为：<B>黑名单</B>\n');
}
else
{
document.write('当前的过滤列表为：<B>白名单</B><BR>\n');
document.write('<font color="#FF0000">提示：填入的URL将与您访问的URL做完全匹配</font>');
}
</SCRIPT>
                  <BR><BR>过滤模式： <INPUT id=FilterPolicy onclick="ChangePolicy()" type=radio value="0" name="FilterPolicy" <%if tcWebApi_get("UrlFilter_Common","Filter_Policy","h") = "0" then asp_Write("checked")  end if%>>黑名单 
				  <INPUT id=FilterPolicy onclick="ChangePolicy()" type=radio value="1" name="FilterPolicy" <%if tcWebApi_get("UrlFilter_Common","Filter_Policy","h") = "1" then asp_Write("checked") end if%>>白名单 
				  <BR><BR><BR>
                  <DIV id=dnsdomain>
                  <TABLE cellSpacing=0 cellPadding=0 border=0>
                    <TBODY>
                    <TR>
                      <TD width=60>URL:</TD>
                      <TD width=100>
					  <INPUT onkeydown="if(event.keyCode==13)return false" maxLength=31 size=30 name="urlitem">
					  </TD>
                      <TD align=middle width=70><INPUT onclick="btnAdd()" type=button value=" 添 加 "> 
                      </TD></TR></TBODY></TABLE></DIV><BR><BR>
					<% if tcWebApi_get("UrlFilter_Common","Activate","h") = "1" then %>
                  <DIV id=ddnsInfo>
				  <iframe src="/cgi-bin/sec_urlfilterlist.cgi" frameborder="0" width="350"></iframe>
                  </DIV>
					<% end if %>
					<BR><INPUT onclick='btnRemove(this.form.rml)' type=button value=" 删 除 "> 
                  </DIV>
                  </TD>
                <TD width=10>&nbsp;</TD></TR>
              <TR>
                <TD width=10>&nbsp;</TD>
                <TD></TD>
                <TD width=10>&nbsp;</TD></TR></TBODY></TABLE></FORM></TD></TR>
        <TR>
          <TD vAlign=top width=157 bgColor=#e7e7e7 height=10></TD>
          <TD width=7 background=/img/panel3.gif>　</TD>
          <TD></TD></TR>
        <TR>
          <TD vAlign=top width=157 bgColor=#e7e7e7></TD>
          <TD width=7 background="/img/panel3.gif">　</TD>
        <TD></TD></TR></TBODY></TABLE></TD></TR>
  <TR>
    <TD height=1>
      <TABLE id=table7 height=35 cellSpacing=0 cellPadding=0 width=808 
        border=0><TBODY>
        <TR>
          <TD width=162 bgColor=#ef8218>　</TD>
          <TD width=278 bgColor=#427594>　</TD>
          <TD width=196 bgColor=#427594>
            <P align=center><IMG id=btnOK onclick="SubmitForm()" height=23 
            src="/img/ok.gif" width=80 border=0>&nbsp;&nbsp;<IMG 
            id=btnCancel onclick="RefreshPage()" height=23 src="/img/cancel.gif" 
            width=80 border=0> </P></TD>
          <TD width=170 
bgColor=#313031>　</TD></TR></TBODY></TABLE></TD></TR></TBODY></TABLE></BODY></HTML>
