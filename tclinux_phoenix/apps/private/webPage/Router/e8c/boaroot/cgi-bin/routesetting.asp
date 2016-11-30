<%
If Request_Form("ChangeRoute") = "Bridge" then 
	TCWebApi_set("WanInfo_ModeSwitch","wanmode","ChangeRoute")
	TCWebApi_commit("WanInfo_ModeSwitch")
	TCWebApi_save()
ElseIf Request_Form("ChangeRoute") = "Route" then 
	TCWebApi_set("WanInfo_ModeSwitch","wanmode","ChangeRoute")
	TCWebApi_set("WanInfo_ModeSwitch","username","username")
	TCWebApi_set("WanInfo_ModeSwitch","password","pwd")
	TCWebApi_commit("WanInfo_ModeSwitch")
	TCWebApi_save()
End If
%>

<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0 Transitional//EN">
<HTML><HEAD><TITLE>·������</TITLE>
<META http-equiv=Content-Language content=zh-cn>
<META http-equiv=Content-Type content="text/html; charset=gb2312">
<LINK href="/JS/stylemain.css" type=text/css rel=stylesheet>
<SCRIPT language=javascript src="/JS/util.js"></SCRIPT>
<script language="JavaScript" type="text/JavaScript">
	var	isIPv4 = "<% tcWebApi_get("WebCurSet_Entry","IsIPv4","s") %>";
	var	lanip;
	var lanip_v4 = "<% tcWebApi_get("Lan_Entry0","IP","s") %>";
	var lanip_v6 = "[" + "<% tcWebApi_get("Lan_Entry0","IP6","s") %>" + "]";
	if ( 'N/A' == lanip_v4 )
		lanip_v4 = '192.168.1.1';
	if ( 'N/A' == lanip_v6 )
		lanip_v6 = '[fe80::1]';
	if ( '0' == isIPv4 && top.window.location.href.indexOf(lanip_v6) < 0 )
		top.window.location.href = 'http://' + lanip_v6 + '/cgi-bin/routesetting.asp';
	else if ( '1' == isIPv4 && top.window.location.href.indexOf(lanip_v4) < 0 )
		top.window.location.href = 'http://' + lanip_v4 + '/cgi-bin/routesetting.asp';
	
	function writeReturnhref()
	{
		var httpip;
		if("0" == isIPv4){
			lanip = "[" + "<% tcWebApi_get("Lan_Entry0","IP6","s") %>" + "]";
			if(lanip == "N/A")
				httpip = "http://[fe80::1]";
			else
				httpip = "http://" + lanip;
		}
		else{
			lanip = "<% tcWebApi_get("Lan_Entry0","IP","s") %>";
			if(lanip == "N/A")	
				httpip = "http://192.168.1.1";
			else
				httpip = "http://" + lanip;
		}
		document.write('<A href="' + httpip + '" onclick="resetCookie();">���ص�¼ҳ��</A>');
	}
	
	function resetCookie() {
		document.cookie = "RegPageStatus=0";	
		return true;
	}
	
</script>

<META content="MSHTML 6.00.2900.3698" name=GENERATOR></HEAD>
<BODY style="TEXT-ALIGN: center; valign: middle" vLink="#000000" aLink="#000000" 
	link="#000000" leftMargin="0" topMargin="0" onload="LoadFrame()" marginheight="0" 
	marginwidth="0">
	<SCRIPT language=JavaScript type=text/javascript>
		function LoadFrame()
		{
  		with (getElById('ConfigForm'))
			{
				var rs=getPageStatus();
				if((rs == "2") || (rs == "1"))
				{
					setDisplay('mainScreen', 0);
				}
				else
				{
					setDisplay('mainScreen', 1);
				}
			}
		}
		
		function getPageStatus()
		{
			if(document.cookie.length > 0)
			{
				tstart = document.cookie.indexOf("RegPageStatus=");
				if( tstart != -1)
				{
					tstart += 14;
					tend = document.cookie.indexOf(";",tstart);
					if(tend == -1) tend = document.cookie.length;
					return document.cookie.substring(tstart,tend);
				}
				else return "N/A";
			}
			else return "N/A";
		}
		
		function getCookie(Name)
		{
			var searched = Name + "=";
			if(document.cookie.length > 0) 
			{
				offset = document.cookie.indexOf(searched);
				if(offset != -1) 
				{
					offset += searched.length;
					end = document.cookie.indexOf(";", offset);
					if(end == -1) end = document.cookie.length;
					return parseInt(document.cookie.substring(offset, end));
				}
				else 
					return 0;
			}
			else return 0;
		}
		
		function CheckForm()
		{
			var usrname = getValue("username");
			var passwd = getValue("pwd");
			if (usrname == "")
			{
				alert("�����ʺŲ���Ϊ��!");
				return false;
			}
			if (passwd == "")
			{
				alert("����������!");
				return false;
			}
			if (usrname.length > 128)
			{
				alert("�����ʺų��Ȳ��ܴ���128λ!");
				return false;
			}
			if (passwd.length > 128)
			{
				alert("���볤�Ȳ��ܴ���128λ!");
				return false;
			}
			return true;
		}
		
		function onBtnBridge()
		{

			document.ConfigForm.ChangeRoute.value = "Bridge";
			document.ConfigForm.submit();
		}
		
		function onBtnRoute()
		{
			if(!CheckForm())
				return false;
			document.ConfigForm.ChangeRoute.value = "Route";
			document.ConfigForm.submit();
		}
		function pageHeight()
		{
			return window.innerHeight != null? window.innerHeight : document.documentElement && document.documentElement.clientHeight ? document.documentElement.clientHeight : document.body != null? document.body.clientHeight : null;
		}
		
		function RefreshSelf()
		{
			top.window.location.href="/cgi-bin/routesetting.asp";	
		}
		
		function ShowResult()
		{
			var	ret = "<% tcWebApi_get("WanInfo_ModeSwitch","Result","s") %>";
			var	mode = "<% tcWebApi_get("WanInfo_ModeSwitch","wanmode","s") %>";
			<% TCWebApi_set("WanInfo_ModeSwitch","Result","DefaultVal") %>
			document.ConfigForm.ChangeRoute.value = "None";
			if(mode == "Route")
			{
				if(ret == "Succ")
				{
					alert("�����·�ɷ�ʽ���ã���֧��4���ն�ͬʱ���������������������ʵ�������ʺš��͡��������롱�Ƿ���ȷ¼�룬\n�����ٴ�¼��������ת��·�ɡ�������ָ�������ӷ�ʽ�����ȵ�����ŽӸ�ԭ��������лл��");
				}
				else if(ret == "Fail")
				{
					alert("����ʧ�ܡ�������MODEM�����ԣ��򲦴�10000�ͻ�����������ѯ��лл��");
					RefreshSelf();
				}
			}
			else if(mode == "Bridge")
			{
				if(ret == "Succ")
				{
					alert("����ɿ�����ӷ�ʽ���ã��뿪�����������ϵġ�������ӡ��������������лл��");
				}
				else if(ret == "Fail")
				{
					alert("����ʧ�ܡ�������MODEM�����ԣ��򲦴�10000�ͻ�����������ѯ��лл��");
					RefreshSelf();
				}
			}
		}
	</SCRIPT>

	<FORM id="ConfigForm" name="ConfigForm" method="post">
		<INPUT TYPE="HIDDEN" NAME="DefaultVal" VALUE="default">
	  <INPUT TYPE="HIDDEN" NAME="ChangeRoute" VALUE="-1">
	  <DIV id=mainScreen style="DISPLAY: none">
			<SCRIPT language=javascript type=text/javascript>
				var percent = 0;
				var winHeight = pageHeight();
				if (winHeight > 540)
				{
					winHeight = (winHeight - 540)/2;
				}
				else
				{
					winHeight = 80;
				}
				document.write('<table width="808" height="540" align="center" background="../img/bgroutesetting.jpg" style="margin-top:'+winHeight+'px">');
			</SCRIPT>
			<TR valign="top" height="40%">
				<TD>
					<TABLE style="MARGIN-TOP: 150px" align=right>
					  <TBODY>
						  <TR>
						    <TD style="MARGIN-TOP: 100px; FONT-SIZE: 13px">
									<script language="JavaScript" type="text/JavaScript">
										writeReturnhref();
									</script>
								</TD>
						  	<TD width="50%">&nbsp;</TD>
						  </TR>
					  </TBODY>
					 </TABLE>
				</TD>
			</TR>
			<TR valign="top">
				<TD colspan="3">
					<TABLE align="center">
			  		<TBODY>
							<TR>
			    			<TD width="8%">&nbsp;</TD>
			    			<TD style="FONT-SIZE: 14px" colSpan=2 nowrap>�������������ʺź��������롣</TD>
			    		</TR>
			  			<TR>
			  					<TD>&nbsp;</TD>
			  			</TR>
			  			<TR id="useraccount">
						    <TD width="8%">&nbsp;<P></TD>
								<TD width="38%" align="right" style="FONT-SIZE: 13px">�����ʺ�:</TD>
						    <TD width="60%"><INPUT id="username" style="WIDTH: 120px; FONT-FAMILY: '����'" name="username" height="20" value=""></TD>
			  			</TR>
			  			<TR id="userpassword">
			    			<TD width="8%">&nbsp;</TD>
								<TD style="FONT-SIZE: 13px" align="right">��������:</TD>
			    			<TD>
			    				<INPUT id="pwd" style="WIDTH: 120px; FONT-FAMILY: '����'" type="password" name="pwd" height="20" value="">
			    			</TD>
			  			</TR>
			  			<TR>
			    			<TD width="8%">&nbsp;</TD>
			  			</TR>
			  			<TR id="msgbutton">
			    			<TD width="8%">&nbsp;</TD>
			    			<TD colSpan=2>
			      			<TABLE height="15%" width="100%" border=0>
			        			<TBODY>
			        				<TR>
			        					<TR>
			          					<TD width="45%" align=right ><INPUT id="btnRoute" style="WIDTH: 60px;HEIGHT: 20px" onclick="onBtnRoute()" type=button value="ת��·��" <%if tcWebApi_get("WanInfo_ModeSwitch","CurMode","h") = "Route" then asp_Write("disabled") end if %>></TD>
			          					<TD width="10%">&nbsp;</TD>
			          					<TD width="45%" align=left ><INPUT id="btnBridge" style="WIDTH: 60px;HEIGHT: 20px" onclick="onBtnBridge()" type="button" value="�ŽӸ�ԭ" <%if tcWebApi_get("WanInfo_ModeSwitch","CurMode","h") = "Bridge" then asp_Write("disabled") end if %>></TD>
			          				</TR>
			          			</TR>
			          		</TBODY>
			          	</TABLE>
			          </TD>
			          <script language="JavaScript" type="text/JavaScript">
										ShowResult();
								</script>
			        </TR>
			      </TBODY>
			    </TABLE>
			  </TD>
			</TR>
		</DIV>
	</FORM>
</BODY>
</HTML>
