<%
If Request_Form("SaveBtn") = "SAVE" Then 
	TCWebApi_set("Ddns_Entry","Active","Enable_DyDNS")
	TCWebApi_set("Ddns_Entry","SERVERNAME","ddns_ServerName")
	TCWebApi_set("Ddns_Entry","MYHOST","sysDNSHost")
	TCWebApi_set("Ddns_Entry","USERNAME","sysDNSUser")
	If tcWebApi_get("WebCustom_Entry","isCZOTESupported","h") <> "Yes" Then	
	TCWebApi_set("Ddns_Entry","PASSWORD","sysDNSPassword")
	else
		if Request_Form("sysDNSPassword") <> "******" Then
	TCWebApi_set("Ddns_Entry","PASSWORD","sysDNSPassword")
		end if
	end if
	TCWebApi_set("Ddns_Entry","WILDCARD","Enable_Wildcard")
  	tcWebApi_commit("Ddns_Entry")

End If

%>

<html><head>
<meta http-equiv=Content-Script-Type content=text/javascript>
<meta http-equiv=Content-Style-Type content=text/css>
<meta http-equiv=Content-Type content="text/html; charset=iso-8859-1">
<link rel="stylesheet" type="text/css" href="/style.css">
<script language="JavaScript" src="OutVariant.asp"></script>
</head><script language="JavaScript">
function ddnsSwitch(on_off)
{
	if(on_off == 0)
		for(i = 2; i < 8; i++)
			document.DDNS_form.elements[i].disabled = true;
	else
		for(i = 2; i < 9; i++)
			document.DDNS_form.elements[i].disabled = false;
}

function checkSubmit()
{
	
  	<%if tcwebApi_get("WebCustom_Entry", "isCZGeneralSupported", "h")="Yes" Then%>
		if(!confirm("Are you sure you want to save these configurations?"))
			return false;
	<%end if%>	

	if(document.DDNS_form.elements[0].checked)
	{
		for(i = 3; i < 6; i++)
		{
			if(document.DDNS_form.elements[i].value.length == 0)
			{
				alert('Please fill out all fields before the submission');
				document.DDNS_form.elements[i].focus();
				return false;
			}
		}
		if(invalidCharCheck(document.DDNS_form.sysDNSHost) ) 
			return false;
		if(invalidCharCheck(document.DDNS_form.sysDNSUser) ) 
			return false;
		if(invalidCharCheck(document.DDNS_form.sysDNSPassword) ) 
			return false;
	}
}
function invalidCharCheck(object)
{
	var len = object.value.length;
	var c;
	var i;
    for (i = 0; i < len; i++)
    {
	 	 var c = object.value.charAt(i);
      
	 	 if (c == '"' || c == ':' || c == '&' || c == '\'' || c == '(' || c== ')' || c==';' || c=='`' || c =='|' || c=='\\')
		 {
				alert('Invaild characters.( & \' \( \) " : ; ` | \\ )');      	  								    	    	   		
		 		return true;
		 }
    }
    
	return false;
}
function doLoad()
{
	if(!document.DDNS_form.elements[0].checked)
		ddnsSwitch(0);
	
	if(document.DDNS_form.sysDNSPassword != null)
		document.DDNS_form.sysDNSPassword.value = ddnsPwd;
}
</script>

<body onLoad="doLoad()">

<FORM METHOD="POST" ACTION="/cgi-bin/access_ddns.asp" name="DDNS_form"><div align=center>
<table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
  <tr>
      <td height="5" class="light-orange" colspan="5">&nbsp;</td></tr><tr>
    <td width="150" height="30" class="title-main">
    <font color="#FFFFFF">Dynamic DNS</font>    </td><td width="10" class="black">&nbsp;</td><td width="150"></td><td width="10" ></td><td width="440"></td></tr><tr>
    <td class="light-orange">&nbsp;</td><td class="light-orange"></td><td class="tabdata"><div align=right>
    <font color="#000000">Dynamic DNS</font>    </div></td><td class="tabdata"><div align=center>:</div></td><td class="tabdata">
		<INPUT NAME="Enable_DyDNS" TYPE="RADIO" onClick="ddnsSwitch(1)" VALUE="Yes" <% If TCWebApi_get("Ddns_Entry","Active","h") = "Yes" then asp_Write("checked") end if%> >		<font color="#000000">Activated</font>		
		<INPUT TYPE="RADIO" NAME="Enable_DyDNS" VALUE="No" <% If TCWebApi_get("Ddns_Entry","Active","h") = "No" then asp_Write("checked") end if%> onClick="ddnsSwitch(0)">		<font color="#000000">Deactivated</font>	</td></tr><tr>
    <td class="light-orange">&nbsp;</td><td class="light-orange"></td><td class="tabdata"><div align=right>
    <font color="#000000">Service Provider</font>    </div></td><td class="tabdata"><div align=center>:</div></td><td class="tabdata">
<SELECT NAME="ddns_ServerName" SIZE="1">
 <option <% if tcWebApi_get("Ddns_Entry","SERVERNAME","h") = "www.dyndns.org" then asp_Write("selected") end if %>>www.dyndns.org
 <option <% if tcWebApi_get("Ddns_Entry","SERVERNAME","h") = "www.tzo.net" then asp_Write("selected") end if %>>www.tzo.net
 <option <% if tcWebApi_get("Ddns_Entry","SERVERNAME","h") = "www.zoneedit.com" then asp_Write("selected") end if %>>www.zoneedit.com
 <option <% if tcWebApi_get("Ddns_Entry","SERVERNAME","h") = "www.dhs.org" then asp_Write("selected") end if %>>www.dhs.org
 	<%if tcWebApi_get("WebCustom_Entry","isC2TrueSupported","h") <> "Yes" then%>
 <option <% if tcWebApi_get("Ddns_Entry","SERVERNAME","h") = "www.hn.org" then asp_Write("selected") end if %>>www.hn.org
 <option <% if tcWebApi_get("Ddns_Entry","SERVERNAME","h") = "www.ez-ip.net" then asp_Write("selected") end if %>>www.ez-ip.net
 	<% end if %>
 <option <% if tcWebApi_get("Ddns_Entry","SERVERNAME","h") = "www.easydns.com" then asp_Write("selected") end if %>>www.easydns.com
 	<%if tcWebApi_get("WebCustom_Entry","isNOIPSupported","h") = "Yes" then%>
 <option <% if tcWebApi_get("Ddns_Entry","SERVERNAME","h") = "www.no-ip.com" then asp_Write("selected") end if %>>www.no-ip.com
 	<% end if %>
 
</SELECT>
</td></tr><tr>
    <td class="light-orange">&nbsp;</td><td class="light-orange"></td><td class="tabdata"><div align=right>
    <font color="#000000">My Host Name</font>    </div></td><td class="tabdata"><div align=center>:</div></td><td class="tabdata">
       <INPUT TYPE="TEXT" NAME="sysDNSHost" SIZE="32" MAXLENGTH="63" VALUE="<% If TCWebApi_get("Ddns_Entry","MYHOST","h") <> "N/A" then TCWebApi_get("Ddns_Entry","MYHOST","s") end if%>" >    </td></tr><tr>
    <td class="light-orange">&nbsp;</td><td class="light-orange"></td><td class="tabdata"><div align=right>
    <font color="#000000">Username</font>    </div></td><td class="tabdata"><div align=center>:</div></td><td class="tabdata">
		<INPUT TYPE="TEXT" NAME="sysDNSUser" SIZE="32" MAXLENGTH="31" VALUE="<% If TCWebApi_get("Ddns_Entry","USERNAME","h") <> "N/A" then TCWebApi_get("Ddns_Entry","USERNAME","s") end if%>" >	</td></tr><tr>
    <td class="light-orange">&nbsp;</td><td class="light-orange"></td><td class="tabdata"><div align=right>
    <font color="#000000">Password</font>    </div></td><td class="tabdata"><div align=center>:</div></td><td class="tabdata">
            <%if tcwebApi_get("WebCustom_Entry","isCZOTESupported","h")="Yes" then%>
        <INPUT TYPE="PASSWORD" NAME="sysDNSPassword" SIZE="32" MAXLENGTH="40" VALUE="" >
            <%else%>
        <INPUT TYPE="PASSWORD" NAME="sysDNSPassword" SIZE="32" MAXLENGTH="31" VALUE="" >
            <% end if %>
    </td></tr><tr>
    <td class="light-orange">&nbsp;</td><td class="light-orange"></td><td class="tabdata"><div align=right>
    <font color="#000000">Wildcard support</font>    </div></td><td class="tabdata"><div align=center>:</div></td><td class="tabdata"><input type="RADIO" name="Enable_Wildcard" value="Yes" <% If TCWebApi_get("Ddns_Entry","WILDCARD","h") = "Yes" then asp_Write("checked") end if%> >
      <font color="#000000">Yes</font>
        <INPUT TYPE="RADIO" NAME="Enable_Wildcard" VALUE="No" <% If TCWebApi_get("Ddns_Entry","WILDCARD","h") = "No" then asp_Write("checked") end if%> > <font color="#000000">No</font>	</td></tr><tr>
    <td class="light-orange">&nbsp;</td><td class="light-orange"></td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td></tr></table>
<table width="760" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
  <tr>
    <td width="160" class="black" height="42" >&nbsp;</td><td width="160" class="orange">&nbsp;</td><td width="440" class="orange">
		<INPUT TYPE="SUBMIT" NAME="SaveBtn" VALUE="SAVE" onClick=" return checkSubmit()">		</td></tr></table></div></form>
</body></html>
