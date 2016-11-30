<%
if Request_Form("pushviewcommit")="1" then 
	tcWebApi_set("OSGIUpgrade_Entry", "pushselected", "pushviewsel")
	if Request_Form("pushviewsel")="later" then
		tcWebApi_constSet("OSGIUpgrade_Entry", "pushviewstart", "")
	end if
	tcWebApi_commit("OSGIUpgrade")
	tcWebApi_save()
end if
%>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3c.org/TR/1999/REC-html401-19991224/loose.dtd">
<HTML xmlns="http://www.w3.org/1999/xhtml">
<TITLE>网关系统更新</TITLE>
<META http-equiv=Content-Type content="text/html; charset=gb2312">
<style type="text/css">
.b1,.b2,.b3,.b4,.b1b,.b2b,.b3b,.b4b,.b{display:block;overflow:hidden;}
.b1,.b2,.b3,.b1b,.b2b,.b3b{height:1px;}
.b2,.b3,.b4,.b2b,.b3b,.b4b,.b{border-left:1px solid #999;border-right:1px solid #999;}
.b1,.b1b{margin:0 5px;background:#999;}
.b2,.b2b{margin:0 3px;border-width:2px;}
.b3,.b3b{margin:0 2px;}
.b4,.b4b{height:2px;margin:0 1px;}
.d1{background:#E4E4E4;}
.k {height:30px;}
</style>

<body>
	<table width='100%' bgcolor='#427594'>
		<tr height='20'>
			<td>&nbsp;</td>
		</tr>
	</table>
	
	<br><br>
	
	<table width='96%' bgcolor='#DEF1FF' cellspacing='1' align='center'>
		<tr height='1'>
			<td>&nbsp;</td>
		</tr>
		<tr>
			<td bgcolor='#FFFFFF'>
				<br><br>
				<table width='80%' cellspacing='0' align='center'>
				<tr>
					<td>
						<div>
							<strong class="b1"></strong>
							<strong class="b2 d1"></strong>
							<strong class="b3 d1"></strong>
							<strong class="b4 d1"></strong>
							<div class="b d1 k" style='padding-top:8px;' ><font style="font-size: 20px; margin: 0px 30px;">
										网关<% if tcWebApi_get("OSGIUpgrade_Entry", "mode", "h") = "0" then %>固件<% elseif tcWebApi_get("OSGIUpgrade_Entry", "mode", "h") = "1" then %>OSGI framework<% elseif tcWebApi_get("OSGIUpgrade_Entry", "mode", "h") = "2" then %>JAVA machine<% elseif tcWebApi_get("OSGIUpgrade_Entry", "mode", "h") = "5" then %>Mobile manager<% elseif tcWebApi_get("OSGIUpgrade_Entry", "mode", "h") = "6" then %>Interface modules<% end if %>升级
    	<% if tcWebApi_get("OSGIUpgrade_Entry", "pushselected", "h") = "ignore" then %>
    								已取消
    	<% elseif tcWebApi_get("OSGIUpgrade_Entry", "pushselected", "h") = "later" then %>
    								将在7天之后提醒
    	<% end if %>	
								</font>
								</div>
							<strong class="b4b d1"></strong>
							<strong class="b3b d1"></strong>
							<strong class="b2b d1"></strong>
							<strong class="b1b"></strong>
						</div>
					</td>
				</tr>
				</table>
				<br>
				<table width='80%' cellspacing='0' align='center'>
				<tr>
					<td>
						<font style="font-size: 14px;">
						<% if tcWebApi_get("OSGIUpgrade_Entry", "pushselected", "h") = "upgrade" then %>
						■ &nbsp;网关正在进行升级操作，操作完成后会重启。
						<% end if %>
						</font>
					</td>
				</tr>
				<tr>
					<td>
						<font style="font-size: 14px;">
						<% if tcWebApi_get("OSGIUpgrade_Entry", "pushselected", "h") = "upgrade" then %>
						■ &nbsp;关闭网关配置窗口，并在重新打开Web浏览器之前等待5分钟。如果需要，重新配置您PC机的IP地址来匹配新的配置。
						<% end if %>
						</font>
					</td>
				</tr>
				</table>
				<br><br>
				<br><br>
			</td>
		</tr>
		<tr height='1'>
			<td>&nbsp;</td>
		</tr>
	</table>


<p>&nbsp;</p>

</body>
</html>