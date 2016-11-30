<%
IF Request_Form("algFlag")="1" THEN
		tcWebApi_set("ALGSwitch_Entry","FTPSW","ftp_active")
		tcWebApi_set("ALGSwitch_Entry","SIPSW","sip_active")
		tcWebApi_set("ALGSwitch_Entry","H323SW","h323_active")
		tcWebApi_set("ALGSwitch_Entry","RTSPSW","rtsp_active")
		tcWebApi_set("ALGSwitch_Entry","L2TPSW","l2tp_active")		
		tcWebApi_set("ALGSwitch_Entry","IPSECSW","ipsec_active")
		tcWebApi_commit("ALGSwitch_Entry")
END IF
%>

<html><head>
<meta http-equiv=Content-Script-Type content=text/javascript>
<meta http-equiv=Content-Style-Type content=text/css>
<meta http-equiv=Content-Type content="text/html; charset=iso-8859-1">
<link rel="stylesheet" type="text/css" href="/style.css">

<script language='javascript'>
function doSave()
{
	document.NAT_ALG_form.algFlag.value = 1;
	document.NAT_ALG_form.submit();
	return;
}			
</script>
</head>

<body>
<form name="NAT_ALG_form" method="post" ACTION="/cgi-bin/adv_nat_alg_switch.asp">
<INPUT TYPE="HIDDEN" NAME="algFlag" VALUE="0">
<table width="760" border="0" align="center" cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
  <tr>
      <td height="5" class="light-orange" colspan="5">&nbsp;</td>
  </tr>
  <tr>
      <td width="150" height="30" class="title-main"><div align="right">
	  ALG Switch
	  </div>
	  </td>
    <td width="10" class="black">&nbsp;</td>
    <td width="150"></td>
    <td width="10" ></td>
    <td width="440"></td>
  </tr>
  <tr>
    <td class="light-orange">&nbsp;</td>
    <td class="light-orange"></td>
    <td class="tabdata"><div align="right">
    FTP ALG
	</div></td>
   <td class="tabdata"><div align="center">:</div></td>
    <td class="tabdata">
       <input name="ftp_active" type="radio" value="on" <% if tcWebApi_get("ALGSwitch_Entry","FTPSW","h") = "on" then asp_Write("checked") end if %>>
	     ON&nbsp;&nbsp;&nbsp;	        
      <input name="ftp_active" type="radio" value="off"  <% if tcWebApi_get("ALGSwitch_Entry","FTPSW","h") = "off" then asp_Write("checked") elseif tcWebApi_get("ALGSwitch_Entry","FTPSW","h") = "N/A" then asp_Write("checked") end if %>>
       	 OFF
    </td>
 </tr>
 
   <tr>
    <td class="light-orange">&nbsp;</td>
    <td class="light-orange"></td>
    <td class="tabdata"><div align="right">
    SIP ALG
	</div></td>
   <td class="tabdata"><div align="center">:</div></td>
      <td class="tabdata"> <input name="sip_active" type="radio" value="on" <% if tcWebApi_get("ALGSwitch_Entry","SIPSW","h") = "on" then asp_Write("checked") end if %>>
        ON&nbsp;&nbsp;&nbsp; 
        <input name="sip_active" type="radio" value="off"  <% if tcWebApi_get("ALGSwitch_Entry","SIPSW","h") = "off" then asp_Write("checked") elseif tcWebApi_get("ALGSwitch_Entry","SIPSW","h") = "N/A" then asp_Write("checked") end if %>>
       	 OFF
    </td>
 </tr>
 
 
    <tr>
    <td class="light-orange">&nbsp;</td>
    <td class="light-orange"></td>
    <td class="tabdata"><div align="right">
    H323 ALG
	</div></td>
   <td class="tabdata"><div align="center">:</div></td>
    <td class="tabdata">
       <input name="h323_active" type="radio" value="on" <% if tcWebApi_get("ALGSwitch_Entry","H323SW","h") = "on" then asp_Write("checked") end if %>>
	     ON&nbsp;&nbsp;&nbsp;	        
      <input name="h323_active" type="radio" value="off"  <% if tcWebApi_get("ALGSwitch_Entry","H323SW","h") = "off" then asp_Write("checked") elseif tcWebApi_get("ALGSwitch_Entry","H323SW","h") = "N/A" then asp_Write("checked") end if %>>
       	 OFF
    </td>
 </tr>
 
    <tr>
    <td class="light-orange">&nbsp;</td>
    <td class="light-orange"></td>
    <td class="tabdata"><div align="right">
    RTSP ALG
	</div></td>
   <td class="tabdata"><div align="center">:</div></td>
    <td class="tabdata">
       <input name="rtsp_active" type="radio" value="on" <% if tcWebApi_get("ALGSwitch_Entry","RTSPSW","h") = "on" then asp_Write("checked") end if %>>
	     ON&nbsp;&nbsp;&nbsp;	        
      <input name="rtsp_active" type="radio" value="off" <% if tcWebApi_get("ALGSwitch_Entry","RTSPSW","h") = "off" then asp_Write("checked") elseif tcWebApi_get("ALGSwitch_Entry","RTSPSW","h") = "N/A" then asp_Write("checked") end if %>>
       	 OFF
    </td>
 </tr>
 
    <tr>
    <td class="light-orange">&nbsp;</td>
    <td class="light-orange"></td>
    <td class="tabdata"><div align="right">
    L2TP ALG
	</div></td>
   <td class="tabdata"><div align="center">:</div></td>
    <td class="tabdata">
       <input name="l2tp_active" type="radio" value="on" <% if tcWebApi_get("ALGSwitch_Entry","L2TPSW","h") = "on" then asp_Write("checked") end if %>>
	     ON&nbsp;&nbsp;&nbsp;	        
      <input name="l2tp_active" type="radio" value="off"  <% if tcWebApi_get("ALGSwitch_Entry","L2TPSW","h") = "off" then asp_Write("checked") elseif tcWebApi_get("ALGSwitch_Entry","L2TPSW","h") = "N/A" then asp_Write("checked") end if %>>
       	 OFF
    </td>
 </tr>
 

    <tr>
    <td class="light-orange">&nbsp;</td>
    <td class="light-orange"></td>
    <td class="tabdata"><div align="right">
    IPSEC ALG
	</div></td>
   <td class="tabdata"><div align="center">:</div></td>
    <td class="tabdata">
       <input name="ipsec_active" type="radio" value="on" <% if tcWebApi_get("ALGSwitch_Entry","IPSECSW","h") = "on" then asp_Write("checked") end if %>>
	     ON&nbsp;&nbsp;&nbsp;	        
      <input name="ipsec_active" type="radio" value="off"  <% if tcWebApi_get("ALGSwitch_Entry","IPSECSW","h") = "off" then asp_Write("checked") elseif tcWebApi_get("ALGSwitch_Entry","IPSECSW","h") = "N/A" then asp_Write("checked") end if %>>
       	 OFF
    </td>
 </tr>
 <tr>
    <td class="light-orange"></td>
    <td class="light-orange">&nbsp;</td>
    <td></td>
	<td></td>
	<td></td>   
 </tr>  
</table>

<table width="760" border="0" align="center" cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
  <tr>
    <td width="160" height="42" class="black">&nbsp;</td>
    <td width="160" class="orange">&nbsp;</td>
    <td width="440" class="orange">
		<input type="button" name="SaveBtn" value="APPLY" onClick="doSave();">&nbsp;&nbsp;&nbsp;&nbsp;
		<input type="button" name="BackBtn" value="CANCELL" onClick="javascript:window.location='/cgi-bin/adv_nat_alg_switch.asp'">
	</td>
  </tr>
</table>
</form>
</body>
</html>  
