<%
If Request_Form("saveFlag")="1" Then

	If Request_Form("isAutoDMZSupported")="0" Then
		If Request_Form("dmzFlag")= "1" Then
			tcWebApi_set("Dmz_PVC","Active","dmz_active")
			tcWebApi_set("Dmz_PVC","DMZ_IP","dmzHostIP")
			If Request_Form("isPrdDMZSupported") = "1" Then
				tcWebApi_set("Dmz_PVC","dmzPrdActive","dmzPrd_Active")
				tcWebApi_set("Dmz_PVC","FromMTH","From_MTH")
				tcWebApi_set("Dmz_PVC","FromD","From_D")
				tcWebApi_set("Dmz_PVC","FromY","From_Y")
				tcWebApi_set("Dmz_PVC","FromH","From_H")
				tcWebApi_set("Dmz_PVC","FromMIN","From_MIN")
				tcWebApi_set("Dmz_PVC","ToMTH","To_MTH")
				tcWebApi_set("Dmz_PVC","ToD","To_D")
				tcWebApi_set("Dmz_PVC","ToY","To_Y")
				tcWebApi_set("Dmz_PVC","ToH","To_H")
				tcWebApi_set("Dmz_PVC","ToMIN","To_MIN")
			end if
		End If
	elseif Request_Form("isAutoDMZSupported")="1" Then
		tcWebApi_set("Dmz_PVC","autoActive","dmz_active1")
		tcWebApi_set("Dmz_PVC","DefaultServerEnable","DefaultSrvActive_H")
		If Request_Form("dmzEnabled")="1" Then
			tcWebApi_set("Dmz_PVC","ChangeToServer","dmzHostIP1")
		End If
	End If
	tcWebApi_commit("Dmz_PVC")
End If
%>

<html><head>
<meta http-equiv=Content-Script-Type content=text/javascript>
<meta http-equiv=Content-Style-Type content=text/css>
<meta http-equiv=Content-Type content="text/html; charset=iso-8859-1">
<link rel="stylesheet" type="text/css" href="/style.css">
<script language="JavaScript" type='text/javascript' src="/ip_new.js"></script>
<script language="JavaScript" src="/general.js"></script>
<script language='javascript'>
function formatStr(object)
{
	var zeroStr = ["", "0", "00", "000"];
	object.value = zeroStr[parseInt(object.getAttribute("maxlength")) - object.value.length] + object.value;
}
function doInactive() {
	// Check if Disabled is selected
	if(document.NAT_DMZ_form.isAutoDMZSupported.value == 0)
	{
		var value =document.NAT_DMZ_form.dmz_active[1].checked;
		if (value) {
			document.NAT_DMZ_form.dmzHostIP.disabled = true;
			document.NAT_DMZ_form.dmzHostIP.value = "0.0.0.0";
			
			/* hide the dmz period setting page */
			if (document.NAT_DMZ_form.isPrdDMZSupported.value == 1) {
				var i = 0;
				while (document.getElementById("DMZ_PERIOD_TR_" + i) != null) {
					document.getElementById("DMZ_PERIOD_TR_" + i).style.display = "none";
					i = i + 1;
				}
			}	
		} else {
			document.NAT_DMZ_form.dmzHostIP.disabled = false;
			
			/* display the dmz period setting page */
			if (document.NAT_DMZ_form.isPrdDMZSupported.value == 1) {
				var i = 0;
				while (document.getElementById("DMZ_PERIOD_TR_" + i) != null) {
					document.getElementById("DMZ_PERIOD_TR_" + i).style.display = "";
					i = i + 1;
				}
			}
		}
	}
	else
	{
		var value0 =document.NAT_DMZ_form.dmz_active1[0].checked;
		var value1 =document.NAT_DMZ_form.dmz_active1[1].checked;
		var value2 =document.NAT_DMZ_form.dmz_active1[2].checked;
		if (value0) {
			document.NAT_DMZ_form.dmzHostIP1.disabled = false;
			document.NAT_DMZ_form.DefaultSrvActive_H.value = 0;
			document.NAT_DMZ_form.dmzEnabled.value = 1;
		} else if (value1) {
			document.NAT_DMZ_form.dmzHostIP1.disabled = true;
			document.NAT_DMZ_form.dmzHostIP1.value = "0.0.0.0";
			document.NAT_DMZ_form.DefaultSrvActive_H.value = 0;
			document.NAT_DMZ_form.dmzEnabled.value = 0;
		} else if (value2){
			document.NAT_DMZ_form.dmzHostIP1.disabled = true;
			document.NAT_DMZ_form.DefaultSrvActive_H.value = 1;
			document.NAT_DMZ_form.dmzEnabled.value = 0;
		}
	}
}

function doInactive2()
{
	if (document.NAT_DMZ_form.isPrdDMZSupported.value == 1) {
		var value = document.NAT_DMZ_form.dmzPrd_Active[1].checked;
		if (value) {
			document.NAT_DMZ_form.From_MTH.disabled = true;
			document.NAT_DMZ_form.From_D.disabled = true;
			document.NAT_DMZ_form.From_Y.disabled = true;
			document.NAT_DMZ_form.From_H.disabled = true;
			document.NAT_DMZ_form.From_MIN.disabled = true;
			
			document.NAT_DMZ_form.To_MTH.disabled = true;
			document.NAT_DMZ_form.To_D.disabled = true;
			document.NAT_DMZ_form.To_Y.disabled = true;
			document.NAT_DMZ_form.To_H.disabled = true;
			document.NAT_DMZ_form.To_MIN.disabled = true;	
		} else {
			document.NAT_DMZ_form.From_MTH.disabled = false;
			document.NAT_DMZ_form.From_D.disabled = false;
			document.NAT_DMZ_form.From_Y.disabled = false;
			document.NAT_DMZ_form.From_H.disabled = false;
			document.NAT_DMZ_form.From_MIN.disabled = false;
			
			document.NAT_DMZ_form.To_MTH.disabled = false;
			document.NAT_DMZ_form.To_D.disabled = false;
			document.NAT_DMZ_form.To_Y.disabled = false;
			document.NAT_DMZ_form.To_H.disabled = false;
			document.NAT_DMZ_form.To_MIN.disabled = false;
		}
	}
}
function doSave() {
	
  	<%if tcwebApi_get("WebCustom_Entry", "isCZGeneralSupported", "h")="Yes" Then%>
		if(!confirm("Are you sure you want to save these configurations?"))
			return;
	<%end if%>	

	if(document.NAT_DMZ_form.isAutoDMZSupported.value == 0){
			if (document.NAT_DMZ_form.dmz_active[0].checked && 
				inValidIPAddr(document.NAT_DMZ_form.dmzHostIP.value))
				return;
			document.forms[0].dmzFlag.value = 1;
			
			/* check if dmz period function is enable or not */
		if (document.NAT_DMZ_form.isPrdDMZSupported.value == 1) {
			if (document.NAT_DMZ_form.dmz_active[0].checked && document.NAT_DMZ_form.dmzPrd_Active[0].checked) {
				/* check month */
				var val1 = document.NAT_DMZ_form.From_MTH.value;
				var val2 = document.NAT_DMZ_form.To_MTH.value;
				if (!isNumeric(val1) || !isNumeric(val2)) {
					alert("Invalid month");
					return;
				}
				val1 = parseInt(val1);
				val2 = parseInt(val2);
				if (val1 < 1 || val1 > 12 || val2 < 1 || val2 > 12) {
					alert("Invalid month, it must be 1~12");
					return;
				}
				
				/* check day */
				val1 = document.NAT_DMZ_form.From_D.value;
				val2 = document.NAT_DMZ_form.To_D.value;
				if (!isNumeric(val1) || !isNumeric(val2)) {
					alert("Invalid day");
					return;
				}
				val1 = parseInt(val1);
				val2 = parseInt(val2);
				if (val1 < 1 || val1 > 31 || val2 < 1 || val2 > 31) {
					alert("Invalid day, it must be 1~31");
					return;
				}
				
				/* check year */
				val1 = document.NAT_DMZ_form.From_Y.value;
				val2 = document.NAT_DMZ_form.To_Y.value;
				if (!isNumeric(val1) || !isNumeric(val2)) {
					alert("Invalid year");
					return;
				}
				val1 = parseInt(val1);
				val2 = parseInt(val2);
				if (val1 < 1000 || val1 > 2100 || val2 < 1000 || val2 > 2100) {
					alert("Invalid year!");
					return;
				}
				
				/* check hour */
				val1 = document.NAT_DMZ_form.From_H.value;
				val2 = document.NAT_DMZ_form.To_H.value;
				if (!isNumeric(val1) || !isNumeric(val2)) {
					alert("Invalid hour");
					return;
				}
				val1 = parseInt(val1);
				val2 = parseInt(val2);
				if (val1 < 0 || val1 > 23 || val2 < 0 || val2 > 23) {
					alert("Invalid hour, it must be 0~23.");
					return;
				}
				
				/* check minute */
				val1 = document.NAT_DMZ_form.From_MIN.value;
				val2 = document.NAT_DMZ_form.To_MIN.value;
				if (!isNumeric(val1) || !isNumeric(val2)) {
					alert("Invalid miniute");
					return;
				}
				val1 = parseInt(val1);
				val2 = parseInt(val2);
				if (val1 < 0 || val1 > 59 || val2 < 0 || val2 > 59) {
					alert("Invalid miniute, it must be 0~59.");
					return;
				}
				
				formatStr(document.NAT_DMZ_form.From_MTH);
				formatStr(document.NAT_DMZ_form.From_D);
				formatStr(document.NAT_DMZ_form.From_Y);
				formatStr(document.NAT_DMZ_form.From_H);
				formatStr(document.NAT_DMZ_form.From_MIN);
				formatStr(document.NAT_DMZ_form.To_MTH);
				formatStr(document.NAT_DMZ_form.To_D);
				formatStr(document.NAT_DMZ_form.To_Y);
				formatStr(document.NAT_DMZ_form.To_H);
				formatStr(document.NAT_DMZ_form.To_MIN);
				
				var value_date_time_from = document.NAT_DMZ_form.From_MTH.value + document.NAT_DMZ_form.From_D.value + document.NAT_DMZ_form.From_Y.value + document.NAT_DMZ_form.From_H.value + document.NAT_DMZ_form.From_MIN.value;
				var value_date_time_to = document.NAT_DMZ_form.To_MTH.value + document.NAT_DMZ_form.To_D.value + document.NAT_DMZ_form.To_Y.value + document.NAT_DMZ_form.To_H.value + document.NAT_DMZ_form.To_MIN.value;
				if (value_date_time_from >= value_date_time_to) {
					alert("From Date/Time should be smaller than To Date/Time!");
					return;
				}
			}
		}
	
		}
	else{
			if(document.NAT_DMZ_form.dmzEnabled.value == 1){
			if (document.NAT_DMZ_form.dmz_active1[0].checked)
				IPSplit=document.NAT_DMZ_form.dmzHostIP1.value.split(".");
				if(IPSplit.length != 4)
				{
					alert("IP format error!");
					document.NAT_DMZ_form.dmzHostIP1.value="0.0.0.0";
					return;
				}
				if(document.NAT_DMZ_form.dmzHostIP1.value!="0.0.0.0" && inValidIPAddr(document.NAT_DMZ_form.dmzHostIP1.value))
				
					return;
			}
			document.forms[0].dmzFlag.value = 0;
		}
	document.forms[0].saveFlag.value = 1;
	document.forms[0].submit();
}
</script>
</head>

<body onLoad="doInactive();doInactive2()">

<form name="NAT_DMZ_form" method="post" ACTION="/cgi-bin/adv_nat_dmz.asp">
<INPUT TYPE="HIDDEN" NAME="saveFlag" VALUE="0">
<INPUT TYPE="HIDDEN" NAME="dmzFlag" VALUE="0">
<INPUT TYPE="HIDDEN" NAME="dmzEnabled" VALUE="0"> 
<input type="hidden" name="isAutoDMZSupported" value="<%if tcWebApi_get("Dmz_PVC","isAutoDMZSupported","h") = "Yes" then asp_write("1") else asp_write("0") end if%>">
<input type="hidden" name="isPrdDMZSupported" value="<%if tcWebApi_get("WebCustom_Entry","isPrdDMZSupported","h") = "Yes" then asp_write("1") else asp_write("0") end if%>">
<input type="hidden" name="DefaultSrvActive_H" value="<%if tcWebApi_get("Dmz_PVC","DefaultServerEnable","h") = "1" then tcWebApi_get("Dmz_PVC","DefaultServerEnable","s") else asp_Write("0") end if%>">  
<table width="760" border="0" align="center" cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
  <tr>
      <td height="5" class="light-orange" colspan="5">&nbsp;</td>
  </tr>
  <tr>
    <td width="150" height="30" class="title-main">
    
    DMZ
    
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
    
    DMZ setting for
    
    </div></td>
    <td class="tabdata"><div align="center">:</div></td>
    <td class="tabdata">
<%if tcWebApi_get("WebCustom_Entry","noWanModeDefined","h") = "Yes" then%>
	<%If tcWebApi_get("Nat_PVC","IPMODE","h") = "Single" Then%>
    Single IPs Account/	<%if tcWebApi_get("WebCustom_Entry","haveXPON","h") = "Yes" then%>WAN<%else%>PVC<%End if%><%tcWebApi_get("WebCurSet_Entry","nat_pvc","s")%>
	<%Else%>
    Multiple IPs Account/ PVC<%tcWebApi_get("WebCurSet_Entry","nat_pvc","s")%>
	<%End If%>

<%else%>
	<%If tcWebApi_get("Nat_PVC","IPMODE","h") = "Multiple" Then%>
		<%if tcWebApi_get("WebCurSet_Entry","nat_pvc","h") = "0" then%>
			 Multiple IPs Account/ PVC0
		<%elseif tcWebApi_get("WebCurSet_Entry","nat_pvc","h") = "1" then%>
			 Multiple IPs Account/ PVC1
		<%elseif tcWebApi_get("WebCurSet_Entry","nat_pvc","h") = "2" then%>
			 Multiple IPs Account/ PVC2
		<%elseif tcWebApi_get("WebCurSet_Entry","nat_pvc","h") = "3" then%>
			 Multiple IPs Account/ PVC3
		<%elseif tcWebApi_get("WebCurSet_Entry","nat_pvc","h") = "4" then%>
			 Multiple IPs Account/ PVC4
		<%elseif tcWebApi_get("WebCurSet_Entry","nat_pvc","h") = "5" then%>
			 Multiple IPs Account/ PVC5
		<%elseif tcWebApi_get("WebCurSet_Entry","nat_pvc","h") = "6" then%>
			 Multiple IPs Account/ PVC6
		<%elseif tcWebApi_get("WebCurSet_Entry","nat_pvc","h") = "7" then%>
			 Multiple IPs Account/ PVC7
		<%elseif tcWebApi_get("WebCurSet_Entry","nat_pvc","h") = "8" then%>
			 Multiple IPs Account/ PTM0
		<%elseif tcWebApi_get("WebCurSet_Entry","nat_pvc","h") = "9" then%>
			 Multiple IPs Account/ PTM1
		<%elseif tcWebApi_get("WebCurSet_Entry","nat_pvc","h") = "10" then%>
			 Multiple IPs Account/ WAN0
		<%end if%>
		<%else%>
		<%if tcWebApi_get("WebCustom_Entry","haveXPON","h") = "Yes" then%>
			Single IPs Account/ WAN<%tcWebApi_get("WebCurSet_Entry","nat_pvc","s")%>
		<%else%>
		<%if tcWebApi_get("WebCurSet_Entry","nat_pvc","h") = "0" then%>
			 Single IPs Account/ PVC0
		<%elseif tcWebApi_get("WebCurSet_Entry","nat_pvc","h") = "1" then%>
			 Single IPs Account/ PVC1
		<%elseif tcWebApi_get("WebCurSet_Entry","nat_pvc","h") = "2" then%>
			 Single IPs Account/ PVC2
		<%elseif tcWebApi_get("WebCurSet_Entry","nat_pvc","h") = "3" then%>
			 Single IPs Account/ PVC3
		<%elseif tcWebApi_get("WebCurSet_Entry","nat_pvc","h") = "4" then%>
			 Single IPs Account/ PVC4
		<%elseif tcWebApi_get("WebCurSet_Entry","nat_pvc","h") = "5" then%>
			 Single IPs Account/ PVC5
		<%elseif tcWebApi_get("WebCurSet_Entry","nat_pvc","h") = "6" then%>
			 Single IPs Account/ PVC6
		<%elseif tcWebApi_get("WebCurSet_Entry","nat_pvc","h") = "7" then%>
			 Single IPs Account/ PVC7
		<%elseif tcWebApi_get("WebCurSet_Entry","nat_pvc","h") = "8" then%>			
			<%if tcWebApi_get("WebCustom_Entry","isMultiSerSupported","h") = "Yes" then%>
				<%if tcWebApi_get("WebCurSet_Entry","nat_pvc_ext","h") = "0" then%>
		    		Single IPs Account/ PTM0 Service ID 0
				<%elseif tcWebApi_get("WebCurSet_Entry","nat_pvc_ext","h") = "1" then%>
		    		Single IPs Account/ PTM0 Service ID 1
				<%elseif tcWebApi_get("WebCurSet_Entry","nat_pvc_ext","h") = "2" then%>
		    		Single IPs Account/ PTM0 Service ID 2
				<%elseif tcWebApi_get("WebCurSet_Entry","nat_pvc_ext","h") = "3" then%>
		    		Single IPs Account/ PTM0 Service ID 3
				<%elseif tcWebApi_get("WebCurSet_Entry","nat_pvc_ext","h") = "4" then%>
		    		Single IPs Account/ PTM0 Service ID 4
				<%elseif tcWebApi_get("WebCurSet_Entry","nat_pvc_ext","h") = "5" then%>
		    		Single IPs Account/ PTM0 Service ID 5
				<%elseif tcWebApi_get("WebCurSet_Entry","nat_pvc_ext","h") = "6" then%>
		    		Single IPs Account/ PTM0 Service ID 6
				<%elseif tcWebApi_get("WebCurSet_Entry","nat_pvc_ext","h") = "7" then%>
		    		Single IPs Account/ PTM0 Service ID 7
				<%End if%>	
			<%else%>
		    	Single IPs Account/ PTM0
			<%End if%>	
		<%elseif tcWebApi_get("WebCurSet_Entry","nat_pvc","h") = "9" then%>
			<%if tcWebApi_get("WebCustom_Entry","isMultiSerSupported","h") = "Yes" then%>
				<%if tcWebApi_get("WebCurSet_Entry","nat_pvc_ext","h") = "0" then%>
		    		Single IPs Account/ PTM1 Service ID 0
				<%elseif tcWebApi_get("WebCurSet_Entry","nat_pvc_ext","h") = "1" then%>
		    		Single IPs Account/ PTM1 Service ID 1
				<%elseif tcWebApi_get("WebCurSet_Entry","nat_pvc_ext","h") = "2" then%>
		    		Single IPs Account/ PTM1 Service ID 2
				<%elseif tcWebApi_get("WebCurSet_Entry","nat_pvc_ext","h") = "3" then%>
		    		Single IPs Account/ PTM1 Service ID 3
				<%elseif tcWebApi_get("WebCurSet_Entry","nat_pvc_ext","h") = "4" then%>
		    		Single IPs Account/ PTM1 Service ID 4
				<%elseif tcWebApi_get("WebCurSet_Entry","nat_pvc_ext","h") = "5" then%>
		    		Single IPs Account/ PTM1 Service ID 5
				<%elseif tcWebApi_get("WebCurSet_Entry","nat_pvc_ext","h") = "6" then%>
		    		Single IPs Account/ PTM1 Service ID 6
				<%elseif tcWebApi_get("WebCurSet_Entry","nat_pvc_ext","h") = "7" then%>
		    		Single IPs Account/ PTM1 Service ID 7
				<%End if%>
			<%else%>
		    	Single IPs Account/ PTM1
			<%End if%>	
		<%elseif tcWebApi_get("WebCurSet_Entry","nat_pvc","h") = "10" then%>
			<%if tcWebApi_get("WebCustom_Entry","isMultiSerSupported","h") = "Yes" then%>
				<%if tcWebApi_get("WebCurSet_Entry","nat_pvc_ext","h") = "0" then%>
		    		Single IPs Account/ WAN0 Service ID 0
				<%elseif tcWebApi_get("WebCurSet_Entry","nat_pvc_ext","h") = "1" then%>
		    		Single IPs Account/ WAN0 Service ID 1
				<%elseif tcWebApi_get("WebCurSet_Entry","nat_pvc_ext","h") = "2" then%>
		    		Single IPs Account/ WAN0 Service ID 2
				<%elseif tcWebApi_get("WebCurSet_Entry","nat_pvc_ext","h") = "3" then%>
		    		Single IPs Account/ WAN0 Service ID 3
				<%elseif tcWebApi_get("WebCurSet_Entry","nat_pvc_ext","h") = "4" then%>
		    		Single IPs Account/ WAN0 Service ID 4
				<%elseif tcWebApi_get("WebCurSet_Entry","nat_pvc_ext","h") = "5" then%>
		    		Single IPs Account/ WAN0 Service ID 5
				<%elseif tcWebApi_get("WebCurSet_Entry","nat_pvc_ext","h") = "6" then%>
		    		Single IPs Account/ WAN0 Service ID 6
				<%elseif tcWebApi_get("WebCurSet_Entry","nat_pvc_ext","h") = "7" then%>
		    		Single IPs Account/ WAN0 Service ID 7
				<%End if%>	
			<%else%>
		    	Single IPs Account/ WAN0
			<%End if%>	
		<%end if%>
		<%End If%>
	<%end if%>

<%end if%>

	  </td>
  </tr>
  <%If tcWebApi_get("Dmz_PVC0","isAutoDMZSupported","h") = "Yes" Then%>
 <tr>
    <td class="light-orange">&nbsp;</td>
    <td class="light-orange"></td>
	
    <td class="tabdata"><div align="right">
    
    DMZ
    
    </div></td>
    <td class="tabdata"><div align="center">:</div></td>
    <td class="tabdata">
        
	    <input name="dmz_active1" type="radio" value="1" <% if tcWebApi_get("Dmz_PVC","autoActive","h") = "1" then asp_Write("checked") end if %> onClick="doInactive()">
	    
	    Enabled
	    
      
      <input name="dmz_active1" type="radio" value="0"  <% if tcWebApi_get("Dmz_PVC","autoActive","h") = "0" then asp_Write("checked") end if %> onClick="doInactive()">
        
        
        Disabled
		
	  <input name="dmz_active1" type="radio" value="2"  <% if tcWebApi_get("Dmz_PVC","autoActive","h") = "2" then asp_Write("checked") end if %> onClick="doInactive()">
      
      
        Auto
        
        </td>
  </tr>
  <tr>
    <td class="light-orange">&nbsp;</td>
    <td class="light-orange"></td>
    <td class="tabdata"><div align="right">
    
    DMZ Host IP Address 
    
    </div></td>
    <td class="tabdata"><div align="center">:</div></td>
    <td class="tabdata">

		 <input type="text" class="uiTextInput"  size="15" maxlength="15" name="dmzHostIP1" id="uiViewdmzHostIP1" VALUE="<% If tcWebApi_get("Dmz_PVC","DefaultServerEnable","h") = "0" then tcWebApi_get("Dmz_PVC" , "ChangeToServer" , "s") else tcWebApi_get("Dmz_PVC","DefaultServer","s") end if %>">

    </td>
  </tr>
  <%else%>
   <tr>
    <td class="light-orange">&nbsp;</td>
    <td class="light-orange"></td>
	
    <td class="tabdata"><div align="right">
    
    DMZ
    
    </div></td>
    <td class="tabdata"><div align="center">:</div></td>
    <td class="tabdata">
        
	    <input name="dmz_active" type="radio" value="Yes" <% if tcWebApi_get("Dmz_PVC","Active","h") = "Yes" then asp_Write("checked") end if %> onClick="doInactive()">
	    
	    Enabled
	    
      
      <input name="dmz_active" type="radio" value="No"  <% if tcWebApi_get("Dmz_PVC","Active","h") = "No" then asp_Write("checked") end if %> onClick="doInactive()">
        
        
        Disabled
        
        </td>
  </tr>
  <tr>
    <td class="light-orange">&nbsp;</td>
    <td class="light-orange"></td>
    <td class="tabdata"><div align="right">
    
    DMZ Host IP Address 
    
    </div></td>
    <td class="tabdata"><div align="center">:</div></td>
    <td class="tabdata">
        
        <input type="text" class="uiTextInput"  size="15" maxlength="15" name="dmzHostIP" id="uiViewdmzHostIP" VALUE="<% If tcWebApi_get("Dmz_PVC","DMZ_IP","h") <> "N/A" then  tcWebApi_get("Dmz_PVC","DMZ_IP","s") end if %>">
        
    </td>
  </tr>
  <%If tcWebApi_get("WebCustom_Entry","isPrdDMZSupported","h") = "Yes" Then%>
  <tr id="DMZ_PERIOD_TR_0">
	<td class="title-sub"><font color="#000000">DMZ Period</font></td>
	<td class="light-orange">&nbsp;</td><td class="tabdata"><hr noshade class="light-orange-line"></td>
	<td class="tabdata"><hr noshade class="light-orange-line"></td>
	<td class="tabdata"><hr noshade class="light-orange-line"></td>
  </tr>
  <tr id="DMZ_PERIOD_TR_1">
      <td class="light-orange">&nbsp;</td>
    <td class="light-orange"></td>
	
    <td class="tabdata"><div align="right">
    
    DMZ Period
    
    </div></td>
    <td class="tabdata"><div align="center">:</div></td>
    <td class="tabdata">
        
	    <input name="dmzPrd_Active" type="radio" value="Yes" <% if tcWebApi_get("Dmz_PVC","dmzPrdActive","h") = "Yes" then asp_Write("checked") end if %> onClick="doInactive2()">
        Enabled 
        <input name="dmzPrd_Active" type="radio" value="No"  <% if tcWebApi_get("Dmz_PVC","dmzPrdActive","h") = "No" then asp_Write("checked") end if %> onClick="doInactive2()">
        Disabled </td>
  </tr>
  <tr id="DMZ_PERIOD_TR_2">
    <td class="light-orange">&nbsp;</td>
    <td class="light-orange"></td>
    <td class="tabdata"><div align="right">
    Current Date/Time
    </div></td>
    <td class="tabdata"><div align="center">:</div></td>
    <td class="tabdata">
        <% TCWebApi_get("DeviceInfo","cur_time2","s") %>
    </td>
  </tr>
  <tr id="DMZ_PERIOD_TR_3">
    <td class="light-orange">&nbsp;</td>
    <td class="light-orange"></td>
    <td class="tabdata"><div align="right">
    From
    </div></td>
    <td class="tabdata"><div align="center">:</div></td>
    <td class="tabdata">
        
        <input type="text" class="uiTextInput"  size="2" maxlength="2" name="From_MTH" VALUE="<% If tcWebApi_get("Dmz_PVC","FromMTH","h") <> "N/A" then  tcWebApi_get("Dmz_PVC","FromMTH","s") end if %>">
		/
		<input type="text" class="uiTextInput"  size="2" maxlength="2" name="From_D" VALUE="<% If tcWebApi_get("Dmz_PVC","FromD","h") <> "N/A" then  tcWebApi_get("Dmz_PVC","FromD","s") end if %>">
		/
		<input type="text" class="uiTextInput"  size="4" maxlength="4" name="From_Y" VALUE="<% If tcWebApi_get("Dmz_PVC","FromY","h") <> "N/A" then  tcWebApi_get("Dmz_PVC","FromY","s") end if %>">
		(Month/Day/Year)
		<input type="text" class="uiTextInput"  size="2" maxlength="2" name="From_H" VALUE="<% If tcWebApi_get("Dmz_PVC","FromH","h") <> "N/A" then  tcWebApi_get("Dmz_PVC","FromH","s") end if %>">
		/
		<input type="text" class="uiTextInput"  size="2" maxlength="2" name="From_MIN" VALUE="<% If tcWebApi_get("Dmz_PVC","FromMIN","h") <> "N/A" then  tcWebApi_get("Dmz_PVC","FromMIN","s") end if %>">
		(Hour/Minute)
    </td>
  </tr>
  <tr id="DMZ_PERIOD_TR_4">
    <td class="light-orange">&nbsp;</td>
    <td class="light-orange"></td>
    <td class="tabdata"><div align="right">
    To
    </div></td>
    <td class="tabdata"><div align="center">:</div></td>
    <td class="tabdata">
        
        <input type="text" class="uiTextInput"  size="2" maxlength="2" name="To_MTH" VALUE="<% If tcWebApi_get("Dmz_PVC","ToMTH","h") <> "N/A" then  tcWebApi_get("Dmz_PVC","ToMTH","s") end if %>">
		/
		<input type="text" class="uiTextInput"  size="2" maxlength="2" name="To_D" VALUE="<% If tcWebApi_get("Dmz_PVC","ToD","h") <> "N/A" then  tcWebApi_get("Dmz_PVC","ToD","s") end if %>">
		/
		<input type="text" class="uiTextInput"  size="4" maxlength="4" name="To_Y" VALUE="<% If tcWebApi_get("Dmz_PVC","ToY","h") <> "N/A" then  tcWebApi_get("Dmz_PVC","ToY","s") end if %>">
		(Month/Day/Year)
		<input type="text" class="uiTextInput"  size="2" maxlength="2" name="To_H" VALUE="<% If tcWebApi_get("Dmz_PVC","ToH","h") <> "N/A" then  tcWebApi_get("Dmz_PVC","ToH","s") end if %>">
		/
		<input type="text" class="uiTextInput"  size="2" maxlength="2" name="To_MIN" VALUE="<% If tcWebApi_get("Dmz_PVC","ToMIN","h") <> "N/A" then  tcWebApi_get("Dmz_PVC","ToMIN","s") end if %>">
		(Hour/Minute)
    </td>
  </tr>
  <%end if%>
  <%End If%>
  <tr>
    <td class="light-orange">&nbsp;</td>
    <td class="light-orange"></td>
    <td>&nbsp;</td>
    <td>&nbsp;</td>
    <td>&nbsp;</td>
  </tr>
</table>

<table width="760" border="0" align="center" cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
  <tr>
    <td width="160" height="42" class="black">&nbsp;</td>
    <td width="160" class="orange">&nbsp;</td>
    <td width="440" class="orange">
		<input type="button" name="SaveBtn" value="APPLY" onClick="doSave();">
		<input type="button" name="BackBtn" value="CANCEL" onClick="javascript:window.location='/cgi-bin/adv_nat_top.asp'">
	</td>
  </tr>
</table>
</div>
</form>
</body>
</html>        
