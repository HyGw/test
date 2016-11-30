<%
if Request_Form("QoS_Flag") = "1" then 
	TCWebApi_set("WebCurSet_Entry","qos_id","Mode")
	TCWebApi_set("QoS_Common","Mode","TempleteString")
elseif Request_Form("QoS_Flag") = "2" then
	TCWebApi_set("QoS_Common","Discipline","DiscplineFlag")
	TCWebApi_commit("QoS_Common")
	TCWebApi_save()	
end if


if Request_Form("Speedlimit_flag") = "1" then	
	if Request_Form("Speedlimitup_flag") = "1" then 
		TCWebApi_set("DataSpeedLimit_Entry","InterfaceLimitUP","vInterfacelimitUP") 
	elseif Request_Form("Speedlimitup_flag") = "2" then
		TCWebApi_set("DataSpeedLimit_Entry","VlanTagLimitUP","vVlanTaglimitUP")
	elseif Request_Form("Speedlimitup_flag") = "3" then
		TCWebApi_set("DataSpeedLimit_Entry","IPLimitUP","vIplimitUP")
	end if
	
	if Request_Form("Speedlimitdown_flag") = "1" then 
		TCWebApi_set("DataSpeedLimit_Entry","InterfaceLimitDOWN","vInterfacelimitDOWN") 
	elseif Request_Form("Speedlimitdown_flag") = "2" then
		TCWebApi_set("DataSpeedLimit_Entry","VlanTagLimitDOWN","vVlanTaglimitDOWN")
	elseif Request_Form("Speedlimitdown_flag") = "3" then
		TCWebApi_set("DataSpeedLimit_Entry","IPLimitDOWN","vIplimitDOWN")
	end if

	if Request_Form("Speedlimitup_flag") <> "0" then 
		TCWebApi_set("DataSpeedLimit_Entry","SpeedLimitModeUP","ModeswitchUP")
	end if
	if Request_Form("Speedlimitdown_flag") <> "0" then 
		TCWebApi_set("DataSpeedLimit_Entry","SpeedLimitModeDOWN","ModeswitchDOWN")
	end if
	TCWebApi_commit("DataSpeedLimit_Entry")
	TCWebApi_save()
end if	

If Request_Form("SaveFlag") = "1" then	
	TCWebApi_set("WebCurSet_Entry","qos_id","Mode")
	TCWebApi_unset("QoS_Common")
	TCWebApi_set("QoS_Common","QoSOptType","QoSRuleFlag")
	TCWebApi_set("QoS_Common","Mode","TempleteString")
	TCWebApi_set("QoS_Common","Active","QosFlag")
	TCWebApi_set("QoS_Common","UplinkBandwidth","Bandwidth")
	TCWebApi_set("QoS_Common","Discipline","DiscplineFlag")
	TCWebApi_set("QoS_Common","EnableDSCPMark","EnDscpFlag")
	TCWebApi_set("QoS_Common","En8021PRemark","Enable8021P")
	
	if Request_Form("DiscplineFlag") = "CAR" then 
		TCWebApi_set("QoS_Common","QueueBW1","Q1Car")
		TCWebApi_set("QoS_Common","QueueBW2","Q2Car")
		TCWebApi_set("QoS_Common","QueueBW3","Q3Car")
		TCWebApi_set("QoS_Common","QueueBW4","Q4Car")
		TCWebApi_set("QoS_Common","QueueBW5","Q5Car")
		TCWebApi_set("QoS_Common","QueueBW6","Q6Car")
		TCWebApi_set("QoS_Common","QueueSW1","Q1EnableFlag")
		TCWebApi_set("QoS_Common","QueueSW2","Q2EnableFlag")
		TCWebApi_set("QoS_Common","QueueSW3","Q3EnableFlag")
		TCWebApi_set("QoS_Common","QueueSW4","Q4EnableFlag")
		TCWebApi_set("QoS_Common","QueueSW5","Q5EnableFlag")
		TCWebApi_set("QoS_Common","QueueSW6","Q6EnableFlag")
		TCWebApi_set("QoS_Common","QueueP1","Q1PValue")
		TCWebApi_set("QoS_Common","QueueP2","Q2PValue")
		TCWebApi_set("QoS_Common","QueueP3","Q3PValue")
		TCWebApi_set("QoS_Common","QueueP4","Q4PValue")
		TCWebApi_set("QoS_Common","QueueP5","Q5PValue")
		TCWebApi_set("QoS_Common","QueueP6","Q6PValue")
		
	elseif Request_Form("DiscplineFlag") = "WRR" then
		TCWebApi_set("QoS_Common","EnableForceWeight","EnableForceFlag")
		TCWebApi_set("QoS_Common","QueueBW1","Q1Weight")
		TCWebApi_set("QoS_Common","QueueBW2","Q2Weight")
		TCWebApi_set("QoS_Common","QueueBW3","Q3Weight")
		TCWebApi_set("QoS_Common","QueueBW4","Q4Weight")
		TCWebApi_set("QoS_Common","QueueSW1","Q1EnableFlag")
		TCWebApi_set("QoS_Common","QueueSW2","Q2EnableFlag")
		TCWebApi_set("QoS_Common","QueueSW3","Q3EnableFlag")
		TCWebApi_set("QoS_Common","QueueSW4","Q4EnableFlag")
		TCWebApi_set("QoS_Common","QueueP1","Q1PValue")
		TCWebApi_set("QoS_Common","QueueP2","Q2PValue")
		TCWebApi_set("QoS_Common","QueueP3","Q3PValue")
		TCWebApi_set("QoS_Common","QueueP4","Q4PValue")		
	elseif Request_Form("DiscplineFlag") = "PQ" then 
		TCWebApi_set("QoS_Common","QueueSW1","Q1EnableFlag")
		TCWebApi_set("QoS_Common","QueueSW2","Q2EnableFlag")
		TCWebApi_set("QoS_Common","QueueSW3","Q3EnableFlag")
		TCWebApi_set("QoS_Common","QueueSW4","Q4EnableFlag")
		TCWebApi_set("QoS_Common","QueueP1","Q1PValue")
		TCWebApi_set("QoS_Common","QueueP2","Q2PValue")
		TCWebApi_set("QoS_Common","QueueP3","Q3PValue")
		TCWebApi_set("QoS_Common","QueueP4","Q4PValue")		
	end if
	
	TCWebApi_commit("QoS_Common")
	
	if Request_Form("ReCommitFlg") = "1" then 
		TCWebApi_set("QoS_Common","QoSOptType","TypeRuleFlag")
		TCWebApi_set("WebCurSet_Entry","qos_id","Entry_Const_0")
		TCWebApi_commit("QoS_Entry")
		TCWebApi_set("WebCurSet_Entry","qos_id","Entry_Const_1")
		TCWebApi_commit("QoS_Entry")
		TCWebApi_set("WebCurSet_Entry","qos_id","Entry_Const_2")
		TCWebApi_commit("QoS_Entry")
		TCWebApi_set("WebCurSet_Entry","qos_id","Entry_Const_3")
		TCWebApi_commit("QoS_Entry")
		TCWebApi_set("WebCurSet_Entry","qos_id","Entry_Const_4")
		TCWebApi_commit("QoS_Entry")
		TCWebApi_set("WebCurSet_Entry","qos_id","Entry_Const_5")
		TCWebApi_commit("QoS_Entry")
		TCWebApi_set("WebCurSet_Entry","qos_id","Entry_Const_6")
		TCWebApi_commit("QoS_Entry")
		TCWebApi_set("WebCurSet_Entry","qos_id","Entry_Const_7")
		TCWebApi_commit("QoS_Entry")
		TCWebApi_set("WebCurSet_Entry","qos_id","Entry_Const_8")
		TCWebApi_commit("QoS_Entry")
		TCWebApi_set("WebCurSet_Entry","qos_id","Entry_Const_9")
		TCWebApi_commit("QoS_Entry")
		
		TCWebApi_set("QoS_Common", "QoSOptType","AppRuleFlag")	
		TCWebApi_set("WebCurSet_Entry","qos_id","Entry_Const_10")
		TCWebApi_commit("QoS_Entry")
		TCWebApi_set("WebCurSet_Entry","qos_id","Entry_Const_11")
		TCWebApi_commit("QoS_Entry")
		TCWebApi_set("WebCurSet_Entry","qos_id","Entry_Const_12")
		TCWebApi_commit("QoS_Entry")
		TCWebApi_set("WebCurSet_Entry","qos_id","Entry_Const_13")
		TCWebApi_commit("QoS_Entry")
	end if
	
	if Request_Form("ReCommitFlg") = "2" then 
		TCWebApi_set("QoS_Common","QoSOptType","TypeRuleFlag")
		TCWebApi_set("WebCurSet_Entry","qos_id","Entry_Const_0")
		TCWebApi_commit("QoS_Entry")
		TCWebApi_set("WebCurSet_Entry","qos_id","Entry_Const_1")
		TCWebApi_commit("QoS_Entry")
		TCWebApi_set("WebCurSet_Entry","qos_id","Entry_Const_2")
		TCWebApi_commit("QoS_Entry")
		TCWebApi_set("WebCurSet_Entry","qos_id","Entry_Const_3")
		TCWebApi_commit("QoS_Entry")
		TCWebApi_set("WebCurSet_Entry","qos_id","Entry_Const_4")
		TCWebApi_commit("QoS_Entry")
		TCWebApi_set("WebCurSet_Entry","qos_id","Entry_Const_5")
		TCWebApi_commit("QoS_Entry")
		TCWebApi_set("WebCurSet_Entry","qos_id","Entry_Const_6")
		TCWebApi_commit("QoS_Entry")
		TCWebApi_set("WebCurSet_Entry","qos_id","Entry_Const_7")
		TCWebApi_commit("QoS_Entry")
		TCWebApi_set("WebCurSet_Entry","qos_id","Entry_Const_8")
		TCWebApi_commit("QoS_Entry")
		TCWebApi_set("WebCurSet_Entry","qos_id","Entry_Const_9")
		TCWebApi_commit("QoS_Entry")
	end if

	TCWebApi_set("DataSpeedLimit_Entry","SpeedLimitModeUP","ModeswitchUP")
	TCWebApi_set("DataSpeedLimit_Entry","SpeedLimitModeDOWN","ModeswitchDOWN")
	TCWebApi_commit("DataSpeedLimit_Entry")
		
	TCWebApi_set("QoS_Common","QoSOptType","QoSRuleFlag")
	TCWebApi_commit("QoS_Common")
	TCWebApi_save()	
End if

%>

<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0 Transitional//EN">
<HTML><HEAD><TITLE>Qos</TITLE>
<META http-equiv=Content-Language content=zh-cn>
<META http-equiv=Content-Type content="text/html; charset=gb2312"><LINK 
href="/JS/stylemain.css" type=text/css rel=stylesheet>
<STYLE type=text/css>
	STYLE2 {FONT-SIZE: 12px}
</STYLE>
<SCRIPT language=javascript src="/JS/menu.js"></SCRIPT>
<SCRIPT language=javascript src="/JS/util.js"></SCRIPT>

<SCRIPT language=JavaScript type=text/javascript>
function btnSaveCheck()
{
	var  vForm = document.ConfigForm;
	GetTempleteString();
	checkboxstatus();
	convertNullValue();
	vForm.SaveFlag.value = "1";
	vForm.submit();
}
function btnSave()
{
	var  vForm = document.ConfigForm;
	GetTempleteString();
	checkboxstatus();
	convertNullValue();
<%if tcwebApi_get("Info_WLan", "isCUCSupport","h")="Yes" then%>
	if(vForm.DiscplineFlag.value == "WRR")
		if(checkIfInt(vForm.Q1Weight.value)==false  || checkIfInt(vForm.Q2Weight.value)==false  ||checkIfInt(vForm.Q3Weight.value)==false  ||checkIfInt(vForm.Q4Weight.value)==false  )
			return false;
<%end if%>
	if(vForm.DiscplineFlag.value == "WRR")
	{
		var weightArr = new Array(vForm.Q1Weight.value, vForm.Q2Weight.value, vForm.Q3Weight.value, vForm.Q4Weight.value);
		var weightArrST = new Array(vForm.Q5Enable.checked, vForm.Q6Enable.checked, vForm.Q7Enable.checked, vForm.Q8Enable.checked);
		var idx = 0;
		var weightTotal = 0;
		
		for ( idx = 0; idx < weightArr.length; idx ++ )
		{
			if ( false == isPlusInteger(weightArr[idx]))
			{
				alert('权重值非法');
				return false;
			}
			
			//if ( weightArrST[idx] )
				weightTotal += parseInt(weightArr[idx]);
		}
		
		if ( weightTotal > 100 )
		{
				alert('WRR权重总和不能大于100');
				return false;
		}
	}
	var bdw = vForm.Bandwidth.value;
	if (bdw!="0"){
	  if (parseInt(bdw) < 8192
	      || parseInt(bdw) > 1000000000
	      || !isInteger(bdw))
	  {
	      alert('上行带宽范围从8192到1000000000,或者输入0（不限速）。');
	      return false;
	  }
	}
	else
	{
		bdw = 1000000000;
	}
	
	if(vForm.DiscplineFlag.value == "CAR")
	{
		var bandArr = new Array(vForm.Q1Car.value, vForm.Q2Car.value, vForm.Q3Car.value, vForm.Q4Car.value,vForm.Q5Car.value,vForm.Q6Car.value);
		var bandArrST = new Array(vForm.Q9Enable.checked, vForm.Q10Enable.checked, vForm.Q11Enable.checked, vForm.Q12Enable.checked,vForm.Q13Enable.checked,vForm.Q14Enable.checked);
		var idx = 0;
		var bandTotal = 0;
		
		for ( idx = 0; idx < bandArr.length; idx ++ )
		{
			if ( false == isPlusInteger(bandArr[idx]))
			{
				alert('带宽值非法');
				return false;
			}
			
			if ( bandArrST[idx] )
				bandTotal += parseInt(bandArr[idx]);
		}
		
		if ( bandTotal*1000 > bdw )
		{
				alert('带宽总和不能大于上行带宽');
				return false;
		}
	}
	vForm.SaveFlag.value = "1";
	vForm.submit();
}
function checkIfInt(weight)
{
	var len ;
	if(isNaN(weight) ){
		alert("please input an interger for weight!");
		return false;
	}
	len = weight.length;
	var ch;
	for(i=0;i<len;i++)
	{
		ch = weight.charAt(i);
		if(ch >'9' || ch<'0')
		{
			alert("please input interger for weight!");
			return false;
		}	
	}
	if(weight>15 || weight<1)
	{
		alert("weight must be 1-15!");
		return false;
	}
	return true;
	
}
function convertNullValue()
{
	var  vForm = document.ConfigForm;
	if(vForm.Q1Weight.value == "") vForm.Q1Weight.value = "0";
	if(vForm.Q2Weight.value == "") vForm.Q2Weight.value = "0";
	if(vForm.Q3Weight.value == "") vForm.Q3Weight.value = "0";
	if(vForm.Q4Weight.value == "") vForm.Q4Weight.value = "0";
<%if tcwebApi_get("Info_WLan", "isCUCSupport","h")="Yes" then%>
	if(vForm.DiscplineFlag.value == "WRR")
		if(checkIfInt(vForm.Q1Weight.value)==false  || checkIfInt(vForm.Q2Weight.value)==false  ||checkIfInt(vForm.Q3Weight.value)==false  ||checkIfInt(vForm.Q4Weight.value)==false  )
			return false;
<%end if%>
	if(vForm.Q1Car.value == "") vForm.Q1Car.value = "0";
	if(vForm.Q2Car.value == "") vForm.Q2Car.value = "0";
	if(vForm.Q3Car.value == "") vForm.Q3Car.value = "0";
	if(vForm.Q4Car.value == "") vForm.Q4Car.value = "0";
	if(vForm.Q5Car.value == "") vForm.Q5Car.value = "0";
	if(vForm.Q6Car.value == "") vForm.Q6Car.value = "0";	
	return true;
}
function checkboxstatus()
{
	var  vForm = document.ConfigForm;
	if(vForm.Enable.checked)
		vForm.QosFlag.value = "Yes";
	else vForm.QosFlag.value = "No";
	
	if(vForm.EnableForceWeight.checked)
		vForm.EnableForceFlag.value = "Yes";
	else  vForm.EnableForceFlag.value = "No";
	if(vForm.EnableDSCPMark.checked)
		vForm.EnDscpFlag.value = "Yes";
	else vForm.EnDscpFlag.value = "No";
	
	if("priority" == getRadioVal("Plan"))
	{
		vForm.DiscplineFlag.value = "PQ";
		if(vForm.Q1Enable.checked) vForm.Q1EnableFlag.value = "Yes";
		else vForm.Q1EnableFlag.value = "No";
		
		if(vForm.Q2Enable.checked) vForm.Q2EnableFlag.value = "Yes";
		else vForm.Q2EnableFlag.value = "No";
		
		if(vForm.Q3Enable.checked) vForm.Q3EnableFlag.value = "Yes";
		else vForm.Q3EnableFlag.value = "No";
		
		if(vForm.Q4Enable.checked) vForm.Q4EnableFlag.value = "Yes";
		else vForm.Q4EnableFlag.value = "No";
	}
	else if("weight" == getRadioVal("Plan"))
	{
		vForm.DiscplineFlag.value = "WRR";
		if(vForm.Q5Enable.checked) vForm.Q1EnableFlag.value = "Yes";
		else vForm.Q1EnableFlag.value = "No";
		
		if(vForm.Q6Enable.checked) vForm.Q2EnableFlag.value = "Yes";
		else vForm.Q2EnableFlag.value = "No";
		
		if(vForm.Q7Enable.checked) vForm.Q3EnableFlag.value = "Yes";
		else vForm.Q3EnableFlag.value = "No";
		
		if(vForm.Q8Enable.checked) vForm.Q4EnableFlag.value = "Yes";
		else vForm.Q4EnableFlag.value = "No";
	}
	/*else if("car" == getRadioVal("Plan"))
	{
		vForm.DiscplineFlag.value = "CAR";
		if(vForm.Q9Enable.checked) vForm.Q1EnableFlag.value = "Yes";
		else vForm.Q1EnableFlag.value = "No";
		
		if(vForm.Q10Enable.checked) vForm.Q2EnableFlag.value = "Yes";
		else vForm.Q2EnableFlag.value = "No";
		
		if(vForm.Q11Enable.checked) vForm.Q3EnableFlag.value = "Yes";
		else vForm.Q3EnableFlag.value = "No";
		
		if(vForm.Q12Enable.checked) vForm.Q4EnableFlag.value = "Yes";
		else vForm.Q4EnableFlag.value = "No";
		
		if(vForm.Q13Enable.checked) vForm.Q5EnableFlag.value = "Yes";
		else vForm.Q5EnableFlag.value = "No";
		
		if(vForm.Q14Enable.checked) vForm.Q6EnableFlag.value = "Yes";
		else vForm.Q6EnableFlag.value = "No";
	}*/
	if(vForm.oldDSCP.value != vForm.EnDscpFlag.value || vForm.old8021P.value != vForm.Enable8021P.value)
	{
		vForm.ReCommitFlg.value = "2";
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
                <TD class=welcom vAlign=bottom align=middle width=120>欢迎您！ </TD>
                <TD vAlign=bottom width=50><A onclick=DoLogout() 
                  href="/cgi-bin/logout.cgi" target=_top><SPAN 
                  class=logout>退出 
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
            <P align=right><FONT face=黑体 color=#ffffff><B><FONT face=黑体 
            color=#ffffff size=6><INPUT id=Selected_Menu type=hidden 
            value="网络->QoS" name=Selected_Menu> </FONT></B><SPAN 
            class=GatewayName>网关名称:
            <SCRIPT language=javascript>
				document.write(top.gateWayName);
			</SCRIPT>
             </SPAN></FONT></P></TD>
          <TD width=211 bgColor=#d8d8d8 height=33>
            <P class=GatewayType align=center>型号:
            <SCRIPT language=javascript>
				document.write(top.ModelName);
			</SCRIPT>
             </P></TD></TR>
        <TR>
          <TD id=MenuArea_L1 vAlign=bottom bgColor=#d8d8d8 colSpan=2 
          height=43>&nbsp;</TD></TR>
        <TR>
          <TD id=MenuArea_L2 bgColor=#0059A0 colSpan=2 
      height=24></TD>
</TR></TBODY></TABLE>
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
		function QosModeConstruction(domain,Mode,Enable,Bandwidth,Plan,EnableForceWeight,EnableDSCPMark,Enable8021P)
		{
			this.domain = domain;
			this.Mode = Mode;
			this.Enable = Enable;
			this.Bandwidth = Bandwidth;
			this.Plan = Plan;
			this.EnableForceWeight = EnableForceWeight;
			this.EnableDSCPMark = EnableDSCPMark;
			this.Enable8021P = Enable8021P;
		}
		var CurMode = new Array(new QosModeConstruction("InternetGatewayDevice.X_CT-COM_UplinkQoS","OTHER","0","0","priority","0","0","0"),null);
		
		function QosAppConstruction(domain,AppName,ClassQueue)
		{
			this.domain = domain;
			this.AppName = AppName;
			this.ClassQueue = ClassQueue;
		}
		var CurApp = new Array(new QosAppConstruction("InternetGatewayDevice.X_CT-COM_UplinkQoS.App.1","","4"),new QosAppConstruction("InternetGatewayDevice.X_CT-COM_UplinkQoS.App.2","","4"),null);
		
		function QosClassConstruction(domain,ClassQueue,Type,Max,Min,ProtocolList,DSCPMarkValue,Value8021P)
		{
			this.domain = domain;
			this.ClassQueue = ClassQueue;
			this.Type = Type;
			this.Max  = Max;
			this.Min  = Min;
			this.ProtocolList  = ProtocolList;
			this.DSCPMarkValue = DSCPMarkValue;
			this.Value8021P    = Value8021P;
		}
		var CurClassArray = new Array(null);
		
		function QosQueueConstruction(domain,Enable,Priority,Weight)
		{
			this.domain = domain;
			this.Enable = Enable;
			this.Priority = Priority;
			this.Weight = Weight;
		}
		var CurQueue = new Array(new QosQueueConstruction("InternetGatewayDevice.X_CT-COM_UplinkQoS.PriorityQueue.1","0","1","0"),new QosQueueConstruction("InternetGatewayDevice.X_CT-COM_UplinkQoS.PriorityQueue.2","0","2","0"),new QosQueueConstruction("InternetGatewayDevice.X_CT-COM_UplinkQoS.PriorityQueue.3","0","3","0"),new QosQueueConstruction("InternetGatewayDevice.X_CT-COM_UplinkQoS.PriorityQueue.4","0","4","0"),new QosQueueConstruction("InternetGatewayDevice.X_CT-COM_UplinkQoS.PriorityQueue.5","0","5","0"),new QosQueueConstruction("InternetGatewayDevice.X_CT-COM_UplinkQoS.PriorityQueue.6","0","6","0"),null);
		
		function Qos(Mode,App,Class,Queue)
		{
		this.Mode  = Mode;
		this.App   = App;
		this.Class = Class;
		this.Queue = Queue;
		}
		var CurQoS = new Qos(CurMode,CurApp,CurClassArray,CurQueue);
		var AppCnt = 0;
		var ClsCnt = 0;
		var QueueCnt= 0;
		var ClsAddFlag  = false;
		
		function CurQoSShow()
		{
			with (getElById('ConfigForm'))
			{
				var	vForm = document.ConfigForm;
				if (!vForm.Enable.checked)
				{
					setDisplay('QOSGlobe', 0);
					setDisplay('COMVlanBtn', 0);
					setDisplay('QueueEdit',0);
					setDisplay('AddBtn',0);
					return;
				}
				else
				{
					setDisplay('QOSGlobe', 1);
					setDisplay('COMVlanBtn', 1);
					setDisplay('QueueEdit',1);
					setDisplay('AddBtn',1);
				}
				if(vForm.DiscplineFlag.value == "PQ")
				{
					setDisplay('PQEdit',1);
					setDisplay('WRREdit',0);
					setDisplay('CAREdit',0);
				}
				else if (vForm.DiscplineFlag.value == "WRR")
				{
					setDisplay('PQEdit',0);
					setDisplay('WRREdit',1);
					setDisplay('CAREdit',0);
				}
				AppCnt = CurQoS.App.length - 1;
				ClsCnt = CurQoS.Class.length - 1;
				QueueCnt = CurQoS.Queue.length - 1;
			}
		}
		
		function QoSEnableChange()
		{
			with (getElById('ConfigForm'))
			{
				var vForm = document.ConfigForm;
				var url;
				var SubmitForm = new webSubmitForm();
			
				if(Enable.checked == false)
				{
					if(!confirm("是否禁用该Qos模板"))
					{
					Enable.checked = true;
					vForm.SaveFlag.value = "0";
					return;
					}
					setDisplay('QueueEdit',0);
					setDisplay('AddBtn',0);
					setDisplay('QOSGlobe',0);
					setDisplay('COMVlanBtn',0);					
				}
				else
				{
					if(!confirm("是否启用该Qos模板"))
					{
						Enable.checked = false;
						vForm.SaveFlag.value = "0";
						return;
					}
					vForm.ReCommitFlg.value = "1";
				}
				btnSaveCheck();
			}
		}
		
		function GetTempleteString()
		{
			var Form = document.ConfigForm;
			var selectValue = getSelectVal('Mode');
			switch (selectValue)
			{
				case "20":
					Form.TempleteString.value = "TR069,INTERNET";
				break;
				case "21":
					Form.TempleteString.value = "TR069,VOIP,INTERNET";
				break;
				case "22":
					Form.TempleteString.value = "TR069,IPTV,INTERNET";
				break;
				case "23":
					Form.TempleteString.value = "TR069,VOIP,IPTV,INTERNET";
				break;
				case "24":
					Form.TempleteString.value = "OTHER";
				break;
				default:
					Form.TempleteString.value = selectValue;
				break;
			}
		}
		
		function QoSChangeMode()
		{
			document.ConfigForm.QoS_Flag.value = "1";
			GetTempleteString();
			document.ConfigForm.submit();
		}
		
		function PlanChange()
		{
			var Form = new webSubmitForm();
			var url ;
			with (getElById('ConfigForm'))
			{
				if (false == confirm("是否更换调度策略?"))
				{
					if(DiscplineFlag.value == "PQ")
						Plan[0].checked = true;
					else if((DiscplineFlag.value == "WRR"))
						Plan[1].checked = true;
					return;
				}
				checkboxstatus();
				document.ConfigForm.QoS_Flag.value = "2";
				document.ConfigForm.submit();
			}
		}
		
		function LoadFrame()
		{
			if ((document.ConfigForm.DiscplineFlag.value == "PQ") || (document.ConfigForm.DiscplineFlag.value == "CAR"))//0  or 2
			{
				setDisplay('EnableForce', 0);
			}
			document.ConfigForm.QoS_Flag.value = "0";
			document.ConfigForm.SaveFlag.value = "0";
			document.ConfigForm.ReCommitFlg.value = "-1";
			ModeChange();
			writeLaninfoTable();
			writevlantagTable();
			writeipTable();
			ModeChange_down	();
			writeLaninfodownTable();
			writevlantagdownTable();
			writeipdownTable();
		}
		
		function VlanSubmit(filename)
		{
			location.replace(filename);
		}
		
		function initQoSTemplate()
	  {
	  	var templateName = '<% tcWebApi_get("QoS_Common","Mode","s") %>';
	  	var templateArray = new Array('TR069,INTERNET'
	  													<% if tcwebApi_get("WebCustom_Entry","isCTPONNOVOIP","h") <> "Yes" then %>
	  													<% if tcwebApi_get("WebCustom_Entry","isCTPONCNJSSupported","h") = "Yes" then %>
	  													, 'VOIP,TR069,INTERNET'
	  													<% else %>
	  													, 'TR069,VOIP,INTERNET'
	  													<% end if %>
	  													<% end if %>
	  													<% if tcwebApi_get("WebCustom_Entry","isCTPONJSSupported","h") = "Yes" then %>
	  													, 'IPTV,TR069,INTERNET'
	  													<% else %>
	  													, 'TR069,IPTV,INTERNET'
	  													<% end if %>
	  													<% if tcwebApi_get("WebCustom_Entry","isCTPONNOVOIP","h") <> "Yes" then %>
	  													<% if tcwebApi_get("WebCustom_Entry","isCTPONCNJSSupported","h") = "Yes" then %>
	  													, 'VOIP,IPTV,TR069,INTERNET'
	  													<% else %>
	  													, 'TR069,VOIP,IPTV,INTERNET'
	  													<% end if %>
	  													<% end if %>
	  													, 'OTHER');
			var i = 0;
	  	var cusname = templateName;
	  	var hascusopt = true;
	  	var isSel = 0;

  		with (getElById('Mode'))
			{
				for( i=0; i< templateArray.length; i++)
				{
					var opt = new Option(templateArray[i], templateArray[i]);
					if ( templateName == templateArray[i] )
					{
						opt.selected = true
						isSel = i;
						hascusopt = false;
					}
					options.add ( opt );
					options[isSel].setAttribute('selected', 'true');
				}

				if ( hascusopt && 0 != templateName.length )
				{
					var optcus = new Option(cusname, cusname);
					optcus.selected = true
					options.add ( optcus );
					isSel = i;
					options[isSel].setAttribute('selected', 'true');
				}
			}
	  }
	  
function different(str)
{
	if(str == '')
	{
		return '';
	}
	var tempArray = str.split(',');
	var tempbuf;
	var index = 0;
	var resultArray = new Array();
	for(i = 0; i < tempArray.length-1; i++)
	{
		for(j =0;j < tempArray.length-i-1;j++)
		{
				if(tempArray[j] > tempArray[parseInt(j)+1 ])
				{
					tempbuf = tempArray[j];
					tempArray[j] = tempArray[parseInt(j)+1];
					tempArray[parseInt(j)+1] = tempbuf;
				}
		}
	}
	resultArray[0] = tempArray[0];
	for(i = 1; i< tempArray.length; i++)
	{
		if(tempArray[i] != tempArray[i - 1])
		{
				index ++;
				resultArray[index] =  tempArray[i];
		}	
	}
	return resultArray;
}

function issame(str1,tempArray)
{
	for(i = 0 ; i<tempArray.length; i ++)
	{
		 if(parseInt(str1) == parseInt(tempArray[i]))
		 {
		 		return true;
		 }
	}	
	return false;
	
}

function isNumber(c)
{
	if(c == '0' || c == '')
		return false;
	for(var i=0; i < c.length;i ++ )
	{
		if (c.charAt(i) < "0" || c.charAt(i) > "9")
		{
			return false;
		}	
	}
	return true;
}

var i=0;
var lanx = new Array();
var lanxa = new Array();

var vlanx = new Array();
var vlanxa = new Array();

var ipx = new Array();
var ipxa = new Array();


var interfacelimitup = '<%tcWebApi_get("DataSpeedLimit_Entry","InterfaceLimitUP","s")%>';
var vlantaglimitup = '<%tcWebApi_get("DataSpeedLimit_Entry","VlanTagLimitUP","s")%>';
var iplimitup = '<%tcWebApi_get("DataSpeedLimit_Entry","IPLimitUP","s")%>';
var firstindex = '<%tcWebApi_get("DataSpeedLimit_Entry","SpeedLimitModeUP","s")%>';
var policy = '<%tcWebApi_get("QoS_Common","Discipline","s") %>';

ipx = iplimitup.split(',');
for(i = 0 ; i < ipx.length ; i ++)
{
	ipxa[i] = ipx[i].split('/');	
}

vlanx = vlantaglimitup.split(',');
for(i = 0 ; i < vlanx.length ; i ++)
{
	vlanxa[i] = vlanx[i].split('/');	
}

lanx = interfacelimitup.split(',');
var CurIndex = 0;

for(i = 0 ; i < lanx.length ; i ++)
{
	lanxa[i] = lanx[i].split('/');	
}
function btnEditApp(index)
{
	var CurEditAppIndex = index.substr(index.indexOf('_') + 1);
	CurIndex = CurEditAppIndex;
	
	with(getElById('ConfigForm'))
	{
		setSelect('AppName',lanxa[CurEditAppIndex][0]);
		vCuridxValue.value = lanxa[CurEditAppIndex][1];
		setDisplay('AppEdit',1);
		setDisplay('AppAdd',0);
	}
}

function btnEditVlan(index)
{
	var CurEditAppIndex = index.substr(index.indexOf('_') + 1);
	CurIndex = CurEditAppIndex;
	
	with(getElById('ConfigForm'))
	{
		VlanName.value = vlanxa[CurEditAppIndex][0];
		Vlan_CuridxValue.value = vlanxa[CurEditAppIndex][1];
		setDisplay('VlanEdit',1);
		setDisplay('VlanAdd',0);
	}
	
}

function btnEditIp(index)
{
	var CurEditAppIndex = parseInt(index.substr(index.indexOf('_') + 1));
	CurIndex = CurEditAppIndex;
	var ip = ipxa[CurEditAppIndex][0].split('-');
	var ip_start = ip[0];
	var ip_end = ip[1];
	
	with(getElById('ConfigForm'))
	{
		Ip_Start.value = ip_start;
		Ip_End.value = ip_end;
		Ip_CuridxValue.value = ipxa[CurEditAppIndex][1];
		setDisplay('IpEdit',1);
		setDisplay('IpAdd',0);
	}
	
}

function btnAddApp()
{
		setDisplay('AppAdd',1);
		setDisplay('AppEdit',0);
		setSelect('AppName1',"LAN1");
}
function LanAddSubmit()
{
	var str = '';
	var tempbuf='';
	var dif_elements = new Array();
	var bandwidth = 0;
	var total_bandwidth = '<% tcWebApi_get("QoS_Common","UplinkBandwidth","s") %>';
	var Queue;
	
	with(getElById('ConfigForm'))
	{	
		for( i = 0 ; i< lanxa.length -1 ;  i ++)
		{
			tempbuf += lanxa[i][1] + ',';
		}
		if(interfacelimitup != '')
			tempbuf += lanxa[lanxa.length -1][1] ;
		else
			tempbuf += '' ;
		
		dif_elements = different(tempbuf);
		if((issame(vCuridxValue1.value , dif_elements) == false && dif_elements.length >= 6) || dif_elements.length > 6)
		{
			alert("最多有６个队列，请调整");
			return;	
		}
		if(interfacelimitup.indexOf(getSelectVal('AppName1')) >= 0)
		{
			var info =  getSelectVal('AppName1') + '已经存在,不可重复添加!';
			alert(info);
			return ;
		}
		
		if(isNumber(vCuridxValue1.value) == false)
		{
			alert("请填入大于0的数");
			return;
		}
		if(interfacelimitup == '')
		{
			bandwidth = parseInt(vCuridxValue1.value)+ parseInt(bandwidth);
		}
		else if(lanx.length == 1)
		{
			if(parseInt(vCuridxValue1.value) != lanxa[0][1])
				bandwidth = parseInt(vCuridxValue1.value)+ parseInt(lanxa[0][1]);
			else
				bandwidth = parseInt(lanxa[0][1]);
		}
		else
		{
			if(issame(vCuridxValue1.value , dif_elements) == false )
			{
					for(i = 0; i< dif_elements.length;i++)
					{
							bandwidth = parseInt(dif_elements[i] )+ parseInt(bandwidth);
					}
					bandwidth = parseInt(vCuridxValue1.value)+ parseInt(bandwidth);
			}
			else
			{
					for(i = 0; i< dif_elements.length;i++)
					{
							bandwidth = parseInt(dif_elements[i] )+ parseInt(bandwidth);
					}
			}
		}
		if(parseInt(total_bandwidth) != 0)
		{
			if(parseInt(bandwidth*512*1000) > parseInt(total_bandwidth))
			{
					alert("带宽之和不能大于上行总带宽，请调整");	
					return;
			}
		}
		else
		{
			if(parseInt(bandwidth*512*1000) > 1000000000)
			{
					alert("带宽之和不能大于上行总带宽，请调整");	
					return;
			}
		}
		
		str = getSelectVal('AppName1') + '/' + vCuridxValue1.value;	
		
		if(interfacelimitup == '')
			interfacelimitup += str;
		else
			interfacelimitup += ',' + str;
		
		Queue = interfacelimitup.split(',');
		if(Queue.length > 6)
		{
			alert("最多有６个队列，请调整");
			interfacelimitup = vInterfacelimitUP.value;
			return;
		}
		vInterfacelimitUP.value = interfacelimitup;
	}
	document.ConfigForm.Speedlimit_flag.value = 1;
	document.ConfigForm.Speedlimitup_flag.value = 1;
	document.ConfigForm.submit();
} 

function btnAddVlan()
{
		setDisplay('VlanAdd',1);
		setDisplay('VlanEdit',0);
}
function VlanAddSubmit()
{
	var str = '';
	var tempbuf='';
	var dif_elements = new Array();
	var bandwidth = 0;
	var total_bandwidth = '<% tcWebApi_get("QoS_Common","UplinkBandwidth","s") %>';
	var Queue;
	
	with(getElById('ConfigForm'))
	{
		for( i = 0 ; i< vlanxa.length ;  i ++)
		{
			if(vlanxa[i][0] == VlanName1.value)
			{
				var info = 'VLAN ' + VlanName1.value + '已经存在,不可重复添加!';
				alert(info);
				return ;
			}
		}
		for( i = 0 ; i< vlanxa.length -1 ;  i ++)
		{
			tempbuf += vlanxa[i][1] + ',';
		}
		if(vlantaglimitup != '')
			tempbuf += vlanxa[vlanxa.length -1][1] ;
		else
			tempbuf += '';
		
		dif_elements = different(tempbuf);
		if((issame(Vlan_CuridxValue1.value , dif_elements) == false && dif_elements.length >= 6) || dif_elements.length > 6)
		{
			alert("最多有６个队列，请调整");
			return;	
		}
		if(isNumber(VlanName1.value) == false && VlanName1.value != 'untagged')
		{
				alert("请填入正确的VLAN或者填入untagged");	
				return;
		}
		if(Number(VlanName1.value) > 4096 || Number(VlanName1.value) < 1)
		{
				alert("VLAN范围是1-4096,请填入正确的VLAN");	
				return;
		}
		if(isNumber(Vlan_CuridxValue1.value) == false)
		{
			alert("请填入大于0的数");
			return;
		}
		if(vlantaglimitup == '')
		{
			bandwidth = parseInt(Vlan_CuridxValue1.value)+ parseInt(bandwidth);
		}
		else if(vlanx.length == 1)
		{
			if(parseInt(Vlan_CuridxValue1.value) != vlanxa[0][1])
				bandwidth = parseInt(Vlan_CuridxValue1.value)+ parseInt(vlanxa[0][1]);
			else
				bandwidth = parseInt(vlanxa[0][1]);
		}
		else{
			if(issame(Vlan_CuridxValue1.value , dif_elements) == false )
			{
					for(i = 0; i< dif_elements.length;i++)
					{
							bandwidth= parseInt(dif_elements[i] ) + parseInt(bandwidth);
					}
					bandwidth = parseInt(Vlan_CuridxValue1.value)+ parseInt(bandwidth);
			}
			else
			{
					for(i = 0; i< dif_elements.length;i++)
					{
							bandwidth = parseInt(dif_elements[i] )+ parseInt(bandwidth);
					}
			}
		}
		if(parseInt(total_bandwidth) != 0)
		{
			if(parseInt(bandwidth*512*1000) > parseInt(total_bandwidth))
			{
					alert("带宽之和不能大于上行总带宽，请调整");	
					return;
			}
		}
		else
		{
			if(parseInt(bandwidth*512*1000) > 1000000000)
			{
					alert("带宽之和不能大于上行总带宽，请调整");	
					return;
			}
		}
		
		str = VlanName1.value + '/' + Vlan_CuridxValue1.value;	
		if(vlantaglimitup == '')
			vlantaglimitup += str;
		else
			vlantaglimitup += ',' + str;
			
		Queue = vlantaglimitup.split(',');
		if(Queue.length > 6)
		{
			alert("最多有６个队列，请调整");
			vlantaglimitup = vVlanTaglimitUP.value;
			return;
		}
		vVlanTaglimitUP.value = vlantaglimitup;
	}
	document.ConfigForm.Speedlimit_flag.value = 1;
	document.ConfigForm.Speedlimitup_flag.value = 2;
	document.ConfigForm.submit();
} 


function btnAddIP()
{
		setDisplay('IpAdd',1);
		setDisplay('IpEdit',0);
}
function IpAddSubmit()
{
	var str = '';
	var dif_elements = new Array();
	var tempbuf = '';
	var bandwidth = 0;
	var total_bandwidth = '<% tcWebApi_get("QoS_Common","UplinkBandwidth","s") %>';
	var Queue;
	
	with(getElById('ConfigForm'))
	{
		if(isIpv6Address(Ip_Start1.value) == true  || isIpv6Address(Ip_End1.value) == true)
		{
				if(Ip_Start1.value != Ip_End1.value)
				{
					alert("IPv6地址最小值与最大值必须相同");
					return false;
				}
		}
		else{
				
				if(!isValidIpAddress(Ip_Start1.value) || !isValidIpAddress(Ip_End1.value)) 
				{
					alert("IP地址格式不正确，请填写正确的IP地址");	
					return false;
				}
				
				if(cmpIpAddress(Ip_End1.value,Ip_Start1.value) != true)
						return false;
		}
		for( i = 0 ; i< ipxa.length -1 ;  i ++)
		{
			tempbuf += ipxa[i][1] + ',';
		}
		if(iplimitup != '')
			tempbuf += ipxa[ipxa.length -1][1] ;
		else
			tempbuf += '';
		
		dif_elements = different(tempbuf);
		if((issame(Ip_CuridxValue1.value , dif_elements) == false && dif_elements.length >= 6) || dif_elements.length > 6)
		{
			alert("最多有６个队列，请调整");
			return;	
		}
		if(isNumber(Ip_CuridxValue1.value) == false)
		{
			alert("请填入大于0的数");
			return;
		}
		if(iplimitup == '')
		{
			bandwidth = parseInt(Ip_CuridxValue1.value)+ parseInt(bandwidth);
		}
		else if(ipx.length == 1)
		{
			if(parseInt(Ip_CuridxValue1.value) != ipxa[0][1])
				bandwidth = parseInt(Ip_CuridxValue1.value)+ parseInt(ipxa[0][1]);
			else
				bandwidth = parseInt(ipxa[0][1]);
		}
		else{
			if(issame(Ip_CuridxValue1.value , dif_elements) == false )
			{
					for(i = 0; i< dif_elements.length;i++)
					{
							bandwidth = parseInt(dif_elements[i] )+ parseInt(bandwidth);
					}
					bandwidth = parseInt(Ip_CuridxValue1.value)+ parseInt(bandwidth);
			}
			else
			{
					for(i = 0; i< dif_elements.length;i++)
					{
							bandwidth = parseInt(dif_elements[i] )+ parseInt(bandwidth);
					}
			}
		}
		if(parseInt(total_bandwidth) != 0)
		{
			if(parseInt(bandwidth*512*1000) > parseInt(total_bandwidth))
			{
					alert("带宽之和不能大于上行总带宽，请调整");	
					return;
			}
		}
		else
		{
			if(parseInt(bandwidth*512*1000) > 1000000000)
			{
					alert("带宽之和不能大于上行总带宽，请调整");	
					return;
			}
		}
		
		str = Ip_Start1.value + '-' + Ip_End1.value + '/' + Ip_CuridxValue1.value;	
		if(iplimitup == '')
			iplimitup += str;
		else
			iplimitup += ',' + str;
		Queue = iplimitup.split(',');
		if(Queue.length > 6)
		{
			alert("最多有６个队列，请调整");
			iplimitup = vIplimitUP.value;
			return;
		}
		vIplimitUP.value = iplimitup;
		
		
	}
	document.ConfigForm.Speedlimit_flag.value = 1;
	document.ConfigForm.Speedlimitup_flag.value = 3;
	document.ConfigForm.submit();
} 

function writeLaninfoTable()
{
		var k,loc;
		
		loc = '<TABLE cellSpacing=0 cellPadding=3 width=465 border=1>';
		loc += '<TR align="middle">';
		loc += '<TD align="center">LAN/SSID</TD>';
		loc += '<TD align="center">限速(单位：512Kbps)</TD>';
		loc+= '<TD align="center">修改</TD>';
		loc += '<TD align="center">删除</TD>';
		loc+= '</TR>';
		if(interfacelimitup != '')
		{
				for(k=0; k < lanx.length; k++)
				{
					loc += '<TR align="middle">';
					loc += '<TD align="center">'+	lanxa[k][0] +'</TD>';
					loc += '<TD align="center">'+ lanxa[k][1] +'</TD>';
					loc += '<TD align="center"><input name="EditLAN" type="button" id="LAN_'+ k +'" onClick="btnEditApp(this.id)" value="Edit"/></td>\n';
					loc+= '<TD width=92><INPUT id="Del_'+ k +'" onclick="btnDelApp(this.id)" type=button value=删除 name=DelApp></TD>'
					loc += '</TR>';
				}
		}
		loc += '</TABLE >';
		loc += '<TABLE  width=100% border=0>';
		loc += '<TR>';
		loc += '<TD width="92"><INPUT id="AddApp" onclick="btnAddApp()" type=button value=添加 name=AddApp></TD>';
		loc+= '</TR>';
		loc += '</TABLE >';
		
		getElById('interfacelimit_up').innerHTML = loc;
}

function writevlantagTable()
{
		var k,loc;
		
		loc = '<TABLE cellSpacing=0 cellPadding=3 width=465 border=1>';
		loc += '<TR align="middle">';
		loc += '<TD align="center">VLAN</TD>';
		loc += '<TD align="center">限速(单位：512Kbps)</TD>';
		loc+= '<TD align="center">修改</TD>';
		loc += '<TD align="center">删除</TD>';
		loc+= '</TR>';
		if(vlantaglimitup != '')
		{
				for(k=0; k < vlanx.length; k++)
				{
					loc += '<TR align="middle">';
					loc += '<TD align="center">'+	vlanxa[k][0] +'</TD>';
					loc += '<TD align="center">'+ vlanxa[k][1] +'</TD>';
					loc += '<TD align="center"><input name="EditVlan" type="button" id="VLAN_'+ k +'" onClick="btnEditVlan(this.id)" value="Edit"/></td>\n';
					loc+= '<TD width=92><INPUT id="Del_'+ k +'" onclick="btnDelVlan(this.id)" type=button value=删除 name=DelVlan></TD>'
					loc += '</TR>';
				}
			
		}
		loc += '</TABLE >';
		loc += '<TABLE  width=100% border=0>';
		loc += '<TR>';
		loc += '<TD width="92"><INPUT id="AddVlan" onclick="btnAddVlan()" type=button value=添加 name=AddVlan></TD>';
		loc+= '</TR>';
		loc += '</TABLE >';
		
		getElById('vlantaglimit_up').innerHTML = loc;
}

function writeipTable()
{
		var k,loc;
		
		loc = '<TABLE cellSpacing=0 cellPadding=3 width=465 border=1>';
		loc += '<TR align="middle">';
		loc += '<TD align="center">IP范围</TD>';
		loc += '<TD align="center">限速(单位：512Kbps)</TD>';
		loc+= '<TD align="center">修改</TD>';
		loc += '<TD align="center">删除</TD>';
		loc+= '</TR>';
		if(iplimitup != '')
		{
				for(k=0; k < ipx.length; k++)
				{
					loc += '<TR align="middle">';
					loc += '<TD align="center">'+	ipxa[k][0] +'</TD>';
					loc += '<TD align="center">'+ ipxa[k][1] +'</TD>';
					loc += '<TD align="center"><input name="EditIP" type="button" id="IP_'+ k +'" onClick="btnEditIp(this.id)" value="Edit"/></td>\n';
					loc+= '<TD width=92><INPUT id="Del_'+ k +'" onclick="btnDelIp(this.id)" type=button value=删除 name=DelIP></TD>'
					loc += '</TR>';
				}
		}
		loc += '</TABLE >';
		loc += '<TABLE  width=100% border=0>';
		loc += '<TR>';
		loc += '<TD width="92"><INPUT id="AddIp" onclick="btnAddIP()" type=button value=添加 name=AddIP></TD>';
		loc+= '</TR>';
		loc += '</TABLE >';
		
		getElById('iplimit_up').innerHTML = loc;
}

function ModeChange()
{
	    var ModeIndex = getElById('ModeswitchUP').selectedIndex;
	    if(ModeIndex == 0)
	    {
	    	setDisplay('vlantaglimit_up',0);
	      setDisplay('AppEdit',0);
	      setDisplay('AppAdd',0);
		    setDisplay('interfacelimit_up',0);
		    setDisplay('VlanEdit',0);
		    setDisplay('VlanAdd',0);
		    setDisplay('iplimit_up',0);
		    setDisplay('IpEdit',0);
		    setDisplay('IpAdd',0);
		    setDisplay('policy',1);
		    if(policy == 'CAR')
		    {
		    	setRadio('Plan','priority');
		    }
		    CurQoSShow();
	    }
	    else if(ModeIndex == 1)
	    {
	    	setDisplay('interfacelimit_up',1);
	      setDisplay('AppEdit',0);
	      setDisplay('AppAdd',0);
	      setDisplay('vlantaglimit_up',0);
	      setDisplay('VlanEdit',0);
		    setDisplay('VlanAdd',0);
	      setDisplay('iplimit_up',0);
	      setDisplay('IpEdit',0);
		    setDisplay('IpAdd',0);
		    if(firstindex == 0)
		    {
		    	alert('将自动关闭＂SP/WRR＂调度策略');
		    }
		    setDisplay('policy',0);
		    setDisplay('PQEdit',0);
		    setDisplay('CAREdit',0);
		    setDisplay('WRREdit',0);
		    firstindex = 1; 
	    		
	    }
	    else if(ModeIndex == 2)
	    {
	    	setDisplay('vlantaglimit_up',1);
	      setDisplay('AppEdit',0);
	      setDisplay('AppAdd',0);
	      setDisplay('interfacelimit_up',0);
	      setDisplay('VlanEdit',0);
		    setDisplay('VlanAdd',0);
	      setDisplay('iplimit_up',0);
	      setDisplay('IpEdit',0);
		    setDisplay('IpAdd',0);
		    if(firstindex == 0)
		    {
		    	alert('将自动关闭＂SP/WRR＂调度策略');	
		    
		    }
		    	setDisplay('policy',0);
		    	setDisplay('PQEdit',0);
		    	setDisplay('CAREdit',0);
		    	setDisplay('WRREdit',0);
		    	firstindex = 2;
	    }
	    else if(ModeIndex == 3)
	    {
	    	setDisplay('interfacelimit_up',0);
	      setDisplay('AppEdit',0);
	      setDisplay('AppAdd',0);
	      setDisplay('vlantaglimit_up',0);
	      setDisplay('VlanEdit',0);
		    setDisplay('VlanAdd',0);
	      setDisplay('iplimit_up',1);
	      setDisplay('IpEdit',0);
		    setDisplay('IpAdd',0);
		    if(firstindex == 0)
		    {
		    	alert('将自动关闭＂SP/WRR＂调度策略');	
		  
		    }
		      setDisplay('policy',0);
		    	setDisplay('PQEdit',0);
		    	setDisplay('CAREdit',0);
		    	setDisplay('WRREdit',0);
		    	firstindex = 3; 
	    }
}
function LanEditSubmit()
{
		var str = '';
		var conflit = '';
		var buf = new Array();
		var tempbuf = '';
		var dif_elements = new Array();
		var bandwidth = 0;
		var total_bandwidth = '<% tcWebApi_get("QoS_Common","UplinkBandwidth","s") %>';

		with(getElById('ConfigForm'))
		{
			if(CurIndex == lanxa.length -1 )
			{
				for(i = 0; i< lanxa.length -2 ; i++)	
				{
						conflit += lanxa[i][0] + ',';
						tempbuf += lanxa[i][1] + ',';
				}
				if(lanxa.length != 1)
				{
					conflit += lanxa[lanxa.length -2][0];
					tempbuf += lanxa[lanxa.length -2][1];
				}
				else
				{
					conflit += '';
					tempbuf += '';
				}
			}
			else
			{
				for(i = 0; i< CurIndex ; i++)
				{
					conflit += lanxa[i][0] + ',';
					tempbuf += lanxa[i][1] + ',';
				}	
				for(i = parseInt(CurIndex )+1 ; i < lanxa.length - 1 ; i++)
				{
					conflit += lanxa[i][0] + ',';
					tempbuf += lanxa[i][1] + ',';
				}	
				conflit += lanxa[lanxa.length -1][0];
				tempbuf += lanxa[lanxa.length -1][1];
			}
			buf = conflit.split(',');
			for( i = 0 ; i<buf.length ; i ++)
			{
				if(buf[i] == getSelectVal('AppName'))	
				{
					alert("修改后的LAN/SSID与已经存在的冲突，请调整。");	
					return;	
				}
			}
			if(isNumber(vCuridxValue.value) == false)
			{
				alert("请填入大于0的数");
				return;
			}
			dif_elements = different(tempbuf);
			if((issame(vCuridxValue.value , dif_elements) == false && dif_elements.length >= 6) || dif_elements.length > 6)
			{
				alert("最多有６个队列，请调整");
				return;	
			}
			if(lanx.length == 1)
			{
				bandwidth = parseInt(vCuridxValue.value)+ parseInt(bandwidth);
			}
			else
			{
				if(issame(vCuridxValue.value , dif_elements) == false )
				{
					for(i = 0; i< dif_elements.length;i++)
					{
							bandwidth = parseInt(dif_elements[i] ) + parseInt(bandwidth);
					}
					bandwidth = parseInt(vCuridxValue.value)+ parseInt(bandwidth);
				}
				else
				{
					for(i = 0; i< dif_elements.length;i++)
					{
							bandwidth = parseInt(dif_elements[i] )+ parseInt(bandwidth);
					}
				}
			}
			
			
			if(parseInt(total_bandwidth) != 0)
			{
				if(parseInt(bandwidth*512*1000) > parseInt(total_bandwidth))
				{
						alert("带宽之和不能大于上行总带宽，请调整");	
						return;
				}
			}
			else
			{
				if(parseInt(bandwidth*512*1000) > 1000000000)
				{
						alert("带宽之和不能大于上行总带宽，请调整");	
						return;
				}
			}
			lanx[CurIndex] = getSelectVal('AppName') + '/' + vCuridxValue.value;
			
			for(i = 0; i< lanx.length -1 ; i++)
			{
						str += lanx[i] + ',';
			}
				str += lanx[lanx.length -1];
			interfacelimitup = str;
			vInterfacelimitUP.value = interfacelimitup;
		}
		document.ConfigForm.Speedlimit_flag.value = 1;
		document.ConfigForm.Speedlimitup_flag.value = 1;
		document.ConfigForm.submit();
}

function VlanEditSubmit()
{
		var str = '';
		var conflit = '';
		var buf = new Array();
		var tempbuf = '';
		var dif_elements = new Array();;
		var bandwidth = 0;
		var total_bandwidth = '<% tcWebApi_get("QoS_Common","UplinkBandwidth","s") %>';
		with(getElById('ConfigForm'))
		{
			if(CurIndex == vlanxa.length -1 )
			{
				for(i = 0; i< vlanxa.length -2 ; i++)	
				{
						conflit += vlanxa[i][0] + ',';
						tempbuf += vlanxa[i][1] + ',';
				}
				if(vlanxa.length != 1)
				{
					conflit += vlanxa[vlanxa.length -2][0];
					tempbuf += vlanxa[vlanxa.length -2][1];
			}
			else
			{
					conflit += '';
					tempbuf += '';
				}
			}
			else
			{
				for( i = 0; i< CurIndex ; i++)
				{
					conflit += vlanxa[i][0] + ',';
					tempbuf += vlanxa[i][1] + ',';
				}	
				for(i = parseInt(CurIndex) + 1; i < vlanxa.length -1; i++)
				{
					conflit += vlanxa[i][0] + ',';
					tempbuf += vlanxa[i][1] + ',';
				}	
				conflit += vlanxa[vlanxa.length -1][0];
				tempbuf += vlanxa[vlanxa.length -1][1];
			}
			
			buf = conflit.split(',');
			for( i = 0 ; i<buf.length ; i ++)
			{
				if(buf[i] == VlanName.value)	
				{
					alert("修改后的VLAN与已经存在的冲突，请调整。");	
					return;	
				}
			}
			if(isNumber(VlanName.value) == false && VlanName.value != 'untagged')
			{
					alert("请填入正确的VLAN或者填入untagged");	
					return;
			}
			if(Number(VlanName.value) > 4096 || Number(VlanName.value) < 1)
			{
				alert("VLAN范围是1-4096,请填入正确的VLAN");	
				return;
			}
			if(isNumber(Vlan_CuridxValue.value) == false)
			{
				alert("请填入大于0的数");
				return;
			}
			dif_elements = different(tempbuf);
			if((issame(Vlan_CuridxValue.value , dif_elements) == false && dif_elements.length >= 6) || dif_elements.length > 6)
			{
				alert("最多有６个队列，请重新调整");
				return;	
			}
			if(vlanx.length == 1)
			{
				bandwidth = parseInt(Vlan_CuridxValue.value) + parseInt(bandwidth);
			}
			else{
				if(issame(Vlan_CuridxValue.value , dif_elements) == false )
				{
					for(i = 0; i< dif_elements.length;i++)
					{
							bandwidth = parseInt(dif_elements[i] ) + parseInt(bandwidth);
					}
					bandwidth = parseInt(Vlan_CuridxValue.value) + parseInt(bandwidth);
				}
				else
				{
					for(i = 0; i< dif_elements.length;i++)
					{
							bandwidth = parseInt(dif_elements[i])+ parseInt(bandwidth);
					}
				}
			}
			if(parseInt(total_bandwidth) != 0)
			{
				if(parseInt(bandwidth*512*1000) > parseInt(total_bandwidth))
				{
						alert("带宽之和不能大于上行总带宽，请调整");	
						return;
				}
			}
			else
			{
				if(parseInt(bandwidth*512*1000) > 1000000000)
				{
						alert("带宽之和不能大于上行总带宽，请调整");	
						return;
				}
			}
			
			vlanx[CurIndex] = VlanName.value + '/' + Vlan_CuridxValue.value;
			for(i = 0; i< vlanx.length -1 ; i++)
			{
						str += vlanx[i] + ',';
			}
			str += vlanx[vlanx.length -1];
			vlantaglimitup = str;
			vVlanTaglimitUP.value = vlantaglimitup;
		}
		
		document.ConfigForm.Speedlimit_flag.value = 1;
		document.ConfigForm.Speedlimitup_flag.value = 2;
		document.ConfigForm.submit();
} 

function cmpIpAddress(address1,address2)
{
		var Lnum = 0;
		var Snum = 0;
		var addrParts1 = address1.split('.');
		var addrParts2 = address2.split('.');
		for (i = 0; i <= 3; i++)
		{
			Lnum = parseInt(addrParts1[i]);
			Snum = parseInt(addrParts2[i]);
			if (Lnum < Snum)
			{
					return false;
			}
		}
		return true;
}

function IpEditSubmit()
{
		var str = '';
		var ipaddr;
		var ip_start;
		var ip_end;
		var tempbuf = '';
		var dif_elements = new Array();;
		var bandwidth = 0;
		var total_bandwidth = '<% tcWebApi_get("QoS_Common","UplinkBandwidth","s") %>';
		
		with(getElById('ConfigForm'))
		{
			if(CurIndex == ipxa.length -1 )
			{
				for(i = 0; i< ipxa.length -2 ; i++)	
				{
						tempbuf += ipxa[i][1] + ',';
				}
				if(ipxa.length != 1)
				{
					tempbuf += ipxa[ipxa.length -2][1];
			}
			else
			{
					tempbuf += '';
				}
			}
			else
			{
				for(i = 0; i< CurIndex ; i++)
				{
					tempbuf += ipxa[i][1] + ',';
				}	
				for( i= parseInt(CurIndex) +1; i<ipxa.length-1 ;i++)
				{
					tempbuf += ipxa[i][1] + ',';
				}	
				
				tempbuf += ipxa[ipxa.length -1][1];
			}
			dif_elements = different(tempbuf);
			if(issame(Ip_CuridxValue.value , dif_elements) == false && dif_elements.length >= 6)
			{
				alert("最多有６个队列，请调整");
				return;	
			}
			if(isNumber(Ip_CuridxValue.value) == false)
			{
				alert("请填入大于0的数");
				return;
			}
			if(ipx.length == 1)
			{
				bandwidth = parseInt(Ip_CuridxValue.value) + parseInt(bandwidth);
			}
			else{
				if(issame(Ip_CuridxValue.value , dif_elements) == false )
				{
					for(i = 0; i< dif_elements.length;i++)
					{
							bandwidth = parseInt(dif_elements[i] ) + parseInt(bandwidth);
					}
						bandwidth = parseInt(Ip_CuridxValue.value) + parseInt(bandwidth);
				}
				else
				{
					for(i = 0; i< dif_elements.length;i++)
					{
							bandwidth = parseInt(dif_elements[i]) + parseInt(bandwidth);
					}
				}
			}
			if(parseInt(total_bandwidth) != 0)
			{
				if(parseInt(bandwidth*512*1000) > parseInt(total_bandwidth))
				{
						alert("带宽之和不能大于上行总带宽，请调整");	
						return;
				}
			}
			else
			{
				if(parseInt(bandwidth*512*1000) > 1000000000)
				{
						alert("带宽之和不能大于上行总带宽，请调整");	
						return;
				}
			}
		
			if(isIpv6Address(Ip_Start.value) == true  || isIpv6Address(Ip_End.value) == true)
			{
				if(Ip_Start.value != Ip_End.value)
				{
					alert("IPv6地址最小值与最大值必须相同");
					return;
				}
			}
			else{
				
				if(!isValidIpAddress(Ip_Start.value)) return false;
				if(!isValidIpAddress(Ip_End.value)) return false;
				
				if(cmpIpAddress(Ip_End.value,Ip_Start.value) != true)
						return ;
			}
			
			ipx[CurIndex] = Ip_Start.value + '-' + Ip_End.value + '/' + Ip_CuridxValue.value;
			
			for(i = 0; i< ipx.length -1 ; i++)
			{
						str += ipx[i] + ',';
			}
				str += ipx[ipx.length -1];
			iplimitup = str;
			vIplimitUP.value = iplimitup;
		}
		document.ConfigForm.Speedlimit_flag.value = 1;
		document.ConfigForm.Speedlimitup_flag.value = 3;
		document.ConfigForm.submit();
}


function btnDelApp(index)
{
		var str = '';
		var CurEditAppIndex = parseInt(index.substr(index.indexOf('_') + 1));

		if(!confirm("您确认要删除吗?"))
		{
			return;
		}
		
		if(CurEditAppIndex == lanx.length -1)
		{
			if(lanx.length >1)
			{
					for(i = 0; i< lanx.length -2 ; i++)
					{
								str += lanx[i] + ',';
					}
					str += lanx[lanx.length -2];
					interfacelimitup = str;
			}
			else
			{
				interfacelimitup = '';
			}
		}
		else 
		{
			if(lanx.length > 1)
			{
				for(i = 0; i< CurEditAppIndex ; i++)
				{
							str += lanx[i] + ',';
				}	
				for(i = parseInt(CurEditAppIndex) +1 ; i < lanx.length - 1 ; i++)
				{
							str += lanx[i] + ',';
				}	
				str += lanx[lanx.length -1];
				interfacelimitup = str;
			}
			else
			{
				interfacelimitup = '';
			}
		}
		document.ConfigForm.vInterfacelimitUP.value = interfacelimitup;
		
		document.ConfigForm.Speedlimit_flag.value = 1;
		document.ConfigForm.Speedlimitup_flag.value = 1;
		document.ConfigForm.submit();
}

function btnDelVlan(index)
{
		var str = '';
		var CurEditAppIndex = parseInt(index.substr(index.indexOf('_') + 1));
		
		if(!confirm("您确认要删除吗?"))
		{
			return;
		}
		if(CurEditAppIndex == vlanx.length -1)
		{
			if(vlanx.length > 1)
			{
				for(i = 0; i< vlanx.length -2 ; i++)
				{
							str += vlanx[i] + ',';
				}
				str += vlanx[vlanx.length -2];
				vlantaglimitup = str;
			}
			else
			{
				vlantaglimitup = '';
			}
		}
		else 
		{
			if(vlanx.length > 1)
			{
				for(i = 0; i< CurEditAppIndex ; i++)
				{
							str += vlanx[i] + ',';
				}	
				for(i = parseInt(CurEditAppIndex) +1 ; i < vlanx.length - 1 ; i++)
				{
							str += vlanx[i] + ',';
				}	
				str += vlanx[vlanx.length -1];
				vlantaglimitup = str;
			}
			else
			{
				vlantaglimitup = '';
			}
		}
		
		document.ConfigForm.vVlanTaglimitUP.value = vlantaglimitup;
		
		document.ConfigForm.Speedlimit_flag.value = 1;
		document.ConfigForm.Speedlimitup_flag.value = 2;
		document.ConfigForm.submit();
}

function btnDelIp(index)
{
		var str = '';
		var CurEditAppIndex = parseInt(index.substr(index.indexOf('_') + 1));

		if(!confirm("您确认要删除吗?"))
		{
			return;
		}
		
		if(CurEditAppIndex == ipx.length -1)
		{
			if(ipx.length > 1)
			{
				for(i = 0; i< ipx.length -2 ; i++)
				{
							str += ipx[i] + ',';
				}
				str += ipx[ipx.length -2];
				iplimitup = str;
			}
			else
			{
				iplimitup = '';
			}
		}
		else 
		{
			if(ipx.length > 1)
			{
				for(i = 0; i< CurEditAppIndex ; i++)
				{
							str += ipx[i] + ',';
				}	
				for(i = parseInt(CurEditAppIndex) +1 ; i < ipx.length - 1 ; i++)
				{
							str += ipx[i] + ',';
				}	
				str += ipx[ipx.length -1];
				iplimitup = str;
			}
			else
			{
				iplimitup = '';
			}
		}
		document.ConfigForm.vIplimitUP.value = iplimitup;
		
		document.ConfigForm.Speedlimit_flag.value = 1;
		document.ConfigForm.Speedlimitup_flag.value = 3;
		document.ConfigForm.submit();
}


var lanx_down = new Array();
var lanxa_down = new Array();

var vlanx_down = new Array();
var vlanxa_down = new Array();

var ipx_down = new Array();
var ipxa_down = new Array();

var interfacelimitdown = '<%tcWebApi_get("DataSpeedLimit_Entry","InterfaceLimitDOWN","s")%>';
var vlantaglimitdown = '<%tcWebApi_get("DataSpeedLimit_Entry","VlanTagLimitDOWN","s")%>';
var iplimitdown = '<%tcWebApi_get("DataSpeedLimit_Entry","IPLimitDOWN","s")%>';



ipx_down = iplimitdown.split(',');
for(i = 0 ; i < ipx_down.length ; i ++)
{
	ipxa_down[i] = ipx_down[i].split('/');	
}

vlanx_down = vlantaglimitdown.split(',');
for(i = 0 ; i < vlanx_down.length ; i ++)
{
	vlanxa_down[i] = vlanx_down[i].split('/');	
}

lanx_down = interfacelimitdown.split(',');
var CurIndex_down = 0;
	
for(i = 0 ; i < lanx_down.length ; i ++)
{
	lanxa_down[i] = lanx_down[i].split('/');	
}
function btnEditApp_down(index)
{
	var CurEditAppIndex = parseInt(index.substr(index.indexOf('_') + 1));
	CurIndex_down = CurEditAppIndex;
	
	with(getElById('ConfigForm'))
	{
		setSelect('AppName_down',lanxa_down[CurEditAppIndex][0]);
		vCuridxValue_down.value = lanxa_down[CurEditAppIndex][1];
		setDisplay('AppEdit_down',1);
		setDisplay('AppAdd_down',0);
	}
	
}

function btnEditVlan_down(index)
{
	var CurEditAppIndex = parseInt(index.substr(index.indexOf('_') + 1));
	CurIndex_down = CurEditAppIndex;
	
	with(getElById('ConfigForm'))
	{
		VlanName_down.value = vlanxa_down[CurEditAppIndex][0];
		Vlan_CuridxValue_down.value = vlanxa_down[CurEditAppIndex][1];
		setDisplay('VlanEdit_down',1);
		setDisplay('VlanAdd_down',0);
	}
	
}

function btnEditIp_down(index)
{
	var CurEditAppIndex = parseInt(index.substr(index.indexOf('_') + 1));
	CurIndex_down = CurEditAppIndex;
	var ip = ipxa_down[CurEditAppIndex][0].split('-');
	var ip_start = ip[0];
	var ip_end = ip[1];
	
	with(getElById('ConfigForm'))
	{
		Ip_Start_down.value = ip_start;
		Ip_End_down.value = ip_end;
		Ip_CuridxValue_down.value = ipxa_down[CurEditAppIndex][1];
		setDisplay('IpEdit_down',1);
		setDisplay('IpAdd_down',0);
	}
	
}

function LanEdit_downSubmit()
{
		var str = '';
		var conflit = '';
		var tempbuf= '';
		var dif_elements;
		var buf;
		with(getElById('ConfigForm'))
		{
			if(CurIndex_down == lanxa_down.length -1 )
			{
				for(i = 0; i< lanxa_down.length -2 ; i++)	
				{
						conflit += lanxa_down[i][0] + ',';
						tempbuf += lanxa_down[i][1] + ',';
				}
				if(lanxa_down.length != 1)
				{
					conflit += lanxa_down[lanxa_down.length -2][0];
					tempbuf += lanxa_down[lanxa_down.length -2][1];
				}
				else
				{
					conflit += '';
					tempbuf += '';
				}
			}
			else
			{
				for(i = 0; i< CurIndex_down ; i++)
				{
					conflit += lanxa_down[i][0] + ',';
					tempbuf += lanxa_down[i][1] + ',';
				}	
				for(i = parseInt(CurIndex_down) +1 ; i < lanxa_down.length - 1 ; i++)
				{
					conflit += lanxa_down[i][0] + ',';
					tempbuf += lanxa_down[i][1] + ',';
				}	
				conflit += lanxa_down[lanxa_down.length -1][0];
				tempbuf += lanxa_down[lanxa_down.length -1][1];
				
			}
			dif_elements = different(tempbuf);
			if((issame(vCuridxValue.value , dif_elements) == false && dif_elements.length >= 6) || dif_elements.length > 6)
			{
				alert("最多有６个队列，请调整");
				return;	
			}
			buf = conflit.split(',');
			
			for( i = 0 ; i<buf.length ; i ++)
			{
				if(buf[i] == getSelectVal('AppName_down'))	
				{
					alert("修改后的LAN/SSID与已经存在的冲突，请调整。");	
					return;	
				}
			}
			if(isNumber(vCuridxValue_down.value) == false)
			{
				alert("请填入大于0的数");
				return;
			}
			lanx_down[CurIndex_down] = getSelectVal('AppName_down') + '/' + vCuridxValue_down.value;
			
			for(i = 0; i< lanx_down.length -1 ; i++)
			{
						str += lanx_down[i] + ',';
			}
				str += lanx_down[lanx_down.length -1];
				
			interfacelimitdown = str;
			vInterfacelimitDOWN.value = interfacelimitdown;
		}
			document.ConfigForm.Speedlimit_flag.value = 1;
			document.ConfigForm.Speedlimitdown_flag.value = "1";
			document.ConfigForm.submit();
}

function VlanEdit_downSubmit()
{
		var str = '';
		var conflit = '';
		var buf = new Array();
		var tempbuf = '';
		var dif_elements;
		with(getElById('ConfigForm'))
		{
			if(CurIndex_down == vlanxa_down.length -1 )
			{
				for(i = 0; i< vlanxa_down.length -2 ; i++)	
				{
						conflit += vlanxa_down[i][0] + ',';
						tempbuf += vlanxa_down[i][1] + ',';
				}
				if(vlanxa_down.length != 1)
				{
					conflit += vlanxa_down[vlanxa_down.length -2][0];
					tempbuf += vlanxa_down[vlanxa_down.length -2][1];
				}
				else
				{
					conflit += '';
					tempbuf += '';
				}
			}
			else
			{
				for(i = 0; i< parseInt(CurIndex_down) ; i++)
				{
					conflit += vlanxa_down[i][0] + ',';
					tempbuf += vlanxa_down[i][1] + ',';
				}	
				for(i = parseInt(CurIndex_down) +1 ; i < vlanxa_down.length - 1 ; i++)
				{
					conflit += vlanxa_down[i][0] + ',';
					tempbuf += vlanxa_down[i][1] + ',';
				}	
				conflit += vlanxa_down[vlanxa_down.length -1][0];
	
				tempbuf += vlanxa_down[vlanxa_down.length -1][1];
			}
			buf = conflit.split(',');
			
			for( i = 0 ; i<buf.length ; i ++)
			{
				if(buf[i] == VlanName_down.value)	
				{
					alert("修改后的VLAN与已经存在的冲突，请调整。");	
					return;	
				}
			}
			if(isNumber(VlanName_down.value) == false && VlanName_down.value != 'untagged')
			{
					alert("请填入正确的VLAN或者填入untagged");	
					return;
			}
			if(Number(VlanName_down.value) > 4096 || Number(VlanName_down.value) < 1)
			{
				alert("VLAN范围是1-4096,请填入正确的VLAN");		
				return;
			}
			if(isNumber(Vlan_CuridxValue_down.value) == false)
			{
				alert("请填入大于0的数");
				return;
			}
			
			dif_elements = different(tempbuf);
			if(issame(Vlan_CuridxValue_down.value , dif_elements) == false && dif_elements.length >= 6)
			{
				alert("最多有６个队列，请调整");
				return;	
			}
			
			vlanx_down[CurIndex_down] = VlanName_down.value + '/' + Vlan_CuridxValue_down.value;
			for(i = 0; i< vlanx_down.length -1 ; i++)
			{
						str += vlanx_down[i] + ',';
			}
			str += vlanx_down[vlanx_down.length -1];
			vlantaglimitdown = str;
			vVlanTaglimitDOWN.value = vlantaglimitdown;
		}
		
		document.ConfigForm.Speedlimit_flag.value = 1;
		document.ConfigForm.Speedlimitdown_flag.value = "2";
		document.ConfigForm.submit();
} 

function IpEdit_downSubmit()
{
		var str = '';
		var ipaddr;
		var ip_start;
		var ip_end;
		var tempbuf = '';
		var dif_elements;
		
		with(getElById('ConfigForm'))
		{
			if(CurIndex_down == ipxa_down.length -1 )
			{
				for(i = 0; i< ipxa_down.length -2 ; i++)	
				{
						tempbuf += ipxa_down[i][1] + ',';
				}
				if(ipxa_down.length != 1)
				{
					tempbuf += ipxa_down[ipxa_down.length -2][1];
				}
				else
				{
					tempbuf += '';
				}
			}
			else
			{
				for(i = 0; i< CurIndex_down ; i++)
				{
					tempbuf += ipxa_down[i][1] + ',';
				}	
				for( i= parseInt(CurIndex_down) +1; i<ipxa_down.length-1 ;i++)
				{
					tempbuf += ipxa_down[i][1] + ',';
				}	
				
				tempbuf += ipxa_down[ipxa_down.length -1][1];
			}
			dif_elements = different(tempbuf);
			if(issame(Ip_CuridxValue_down.value , dif_elements) == false && dif_elements.length >= 6)
			{
				alert("最多有６个队列，请调整");
				return;	
			}
			if(isNumber(Ip_CuridxValue_down.value) == false)
			{
				alert("请填入大于0的数");
				return;
			}
			if(isIpv6Address(Ip_Start_down.value) == true  || isIpv6Address(Ip_End_down.value) == true)
			{
				if(Ip_Start_down.value != Ip_End_down.value)
				{
					alert("IPv6地址最小值与最大值必须相同");
					return;
				}
			}
			else{
				
				if(!isValidIpAddress(Ip_Start_down.value)) return false;
				if(!isValidIpAddress(Ip_End_down.value)) return false;
				
				if(cmpIpAddress(Ip_End_down.value,Ip_Start_down.value) != true)
						return;
			}
			
			ipx_down[CurIndex_down] = Ip_Start_down.value + '-' + Ip_End_down.value + '/' + Ip_CuridxValue_down.value;
			
			for(i = 0; i< ipx_down.length -1 ; i++)
			{
						str += ipx_down[i] + ',';
			}
				str += ipx_down[ipx_down.length -1];
			
			iplimitdown = str;
			vIplimitDOWN.value = iplimitdown;
			
		}
		document.ConfigForm.Speedlimit_flag.value = 1;
		document.ConfigForm.Speedlimitdown_flag.value = "3";
		document.ConfigForm.submit();
}

function btnDelApp_down(index)
{
		var str = '';
		var CurEditAppIndex = parseInt(index.substr(index.indexOf('_') + 1));
		if(!confirm("您确认要删除吗?"))
		{
			return;
		}
		
		if(CurEditAppIndex == lanx_down.length -1)
		{
			if(lanx_down.length > 1)
			{
				for(i = 0; i< lanx_down.length -2 ; i++)
				{
							str += lanx_down[i] + ',';
				}
				str += lanx_down[lanx_down.length -2];
				interfacelimitdown = str;
			}
			else
			{
				interfacelimitdown = '';
			}
		}
		else
		{
			if(lanx_down.length > 1)
			{
				for(i = 0; i< CurEditAppIndex ; i++)
				{
							str += lanx_down[i] + ',';
				}	
				for(i = CurEditAppIndex + 1; i< lanx_down.length - 1  ; i++)
				{
							str += lanx_down[i] + ',';	
				}
				str += lanx_down[lanx_down.length -1];
				interfacelimitdown = str;
			}
			else
			{
				interfacelimitdown = '';
			}
		}
		document.ConfigForm.vInterfacelimitDOWN.value = interfacelimitdown;
		document.ConfigForm.Speedlimit_flag.value = 1;
		document.ConfigForm.Speedlimitdown_flag.value = "1";
		document.ConfigForm.submit();
}

function btnDelVlan_down(index)
{	
		var str = '';
		var CurEditAppIndex = parseInt(index.substr(index.indexOf('_') + 1));
		if(!confirm("您确认要删除吗?"))
		{
			return;
		}
		
		if(CurEditAppIndex == vlanx_down.length -1)
		{
			if(vlanx_down.length > 1)
			{
				for(i = 0; i< vlanx_down.length -2 ; i++)
				{
							str += vlanx_down[i] + ',';
				}
				str += vlanx_down[vlanx_down.length -2];
				vlantaglimitdown = str;
			}
			else
			{
				vlantaglimitdown = '';
			}
		}
		else
		{
			if(vlanx_down.length > 1)
			{
				for(i = 0; i< CurEditAppIndex ; i++)
				{
							str += vlanx_down[i] + ',';
				}	
				for(i = CurEditAppIndex + 1; i< vlanx_down.length-1 ; i++)
				{
							str += vlanx_down[i] + ',';	
				}
				str += vlanx_down[vlanx_down.length -1];
				vlantaglimitdown = str;
			}
			else
			{
				vlantaglimitdown = '';
			}
		}
		document.ConfigForm.vVlanTaglimitDOWN.value = vlantaglimitdown;
		document.ConfigForm.Speedlimit_flag.value = 1;
		document.ConfigForm.Speedlimitdown_flag.value = "2";
		document.ConfigForm.submit();
}

function btnDelIp_down(index)
{	
		var str = '';
		var CurEditAppIndex = parseInt(index.substr(index.indexOf('_') + 1));
		if(!confirm("您确认要删除吗?"))
		{
			return;
		}
		
		if(CurEditAppIndex == ipx_down.length -1)
		{
			if(ipx_down.length > 1)
			{
				for(i = 0; i< ipx_down.length -2 ; i++)
				{
							str += ipx_down[i] + ',';
				}
				str += ipx_down[ipx_down.length -2];
				iplimitdown = str;
			}
			else
			{
				iplimitdown = '';
			}
		}
		else
		{
			if(ipx_down.length > 1)
			{
				for(i = 0; i< CurEditAppIndex ; i++)
				{
							str += ipx_down[i] + ',';
				}	
				for(i = CurEditAppIndex + 1; i< ipx_down.length-1 ; i++)
				{
							str += ipx_down[i] + ',';	
				}
				str += ipx_down[ipx_down.length -1];
				iplimitdown = str;
			}
			else
			{
				iplimitdown = '';
			}
		}
		document.ConfigForm.vIplimitDOWN.value = iplimitdown;
		document.ConfigForm.Speedlimit_flag.value = 1;
		document.ConfigForm.Speedlimitdown_flag.value = "3";
		document.ConfigForm.submit();
}

function btnAddApp_down()
{
		setDisplay('AppAdd_down',1);
		setDisplay('AppEdit_down',0);
		setSelect('AppName1_down',"LAN1");
}
function LanAdd_downSubmit()
{
	var str = '';
	var dif_elements;
	var tempbuf='';
	var Queue;
	
	with(getElById('ConfigForm'))
	{
		for( i = 0 ; i< lanxa_down.length -1 ;  i ++)
		{
				tempbuf += lanxa_down[i][1] + ',';
		}
		if(interfacelimitdown != '')
				tempbuf += lanxa_down[lanxa_down.length -1][1] ;
		else
				tempbuf += '' ;
		dif_elements = different(tempbuf);
		if((issame(vCuridxValue1_down.value , dif_elements) == false && dif_elements.length >= 6) || dif_elements.length > 6)
		{
				alert("最多有６个队列，请调整");
				return;	
		}
		if(interfacelimitdown.indexOf(getSelectVal('AppName1_down')) >= 0)
		{
			var info =  getSelectVal('AppName1_down') + '已经存在,不可重复添加!';
			alert(info);
			return ;
		}
		if(isNumber(vCuridxValue1_down.value) == false)
		{
			alert("请填入大于0的数");
			return;
		}
		
		str = getSelectVal('AppName1_down') + '/' + vCuridxValue1_down.value;	
		if(interfacelimitdown == '')
			interfacelimitdown += str;
		else
			interfacelimitdown += ',' + str;
		Queue = interfacelimitdown.split(',');
		if(Queue.length > 6)
		{
			alert("最多有６个队列，请调整");
			interfacelimitdown = vInterfacelimitDOWN.value;
			return;
		}
		vInterfacelimitDOWN.value = interfacelimitdown;
	}
	document.ConfigForm.Speedlimit_flag.value = 1;
	document.ConfigForm.Speedlimitdown_flag.value = "1";
	document.ConfigForm.submit();
} 

function btnAddVlan_down()
{
		setDisplay('VlanAdd_down',1);
		setDisplay('VlanEdit_down',0);
}
function VlanAdd_downSubmit()
{
	var str = '';
	var tempbuf='';
	var dif_elements;
	var Queue;
	
	with(getElById('ConfigForm'))
	{
		str = VlanName1_down.value + '/' + Vlan_CuridxValue1_down.value;	
		
		for( i = 0 ; i< vlanxa_down.length ;  i ++)
		{
			if(vlanxa_down[i][0] == VlanName1_down.value)
			{
				var info = 'VLAN' + VlanName1_down.value + '已经存在,不可重复添加!';
				alert(info);
				return ;
			}
		}
		
		for( i = 0 ; i< vlanxa_down.length -1 ;  i ++)
		{
			tempbuf += vlanxa_down[i][1] + ',';
		}
		if(vlantaglimitdown != '')
			tempbuf += vlanxa_down[vlanxa_down.length -1][1] ;
		else
			tempbuf += '';
			
		dif_elements = different(tempbuf);
		if((issame(Vlan_CuridxValue1_down.value , dif_elements) == false && dif_elements.length >= 6) || dif_elements.length > 6)
		{
			alert("最多有６个队列，请调整");
			return;	
		}
		if(isNumber(VlanName1_down.value) == false && VlanName1_down.value != 'untagged')
		{
				alert("请填入正确的VLAN或者填入untagged");	
				return;
		}
		if(Number(VlanName1_down.value) > 4096 || Number(VlanName1_down.value) < 1)
		{
				alert("VLAN范围是1-4096,请填入正确的VLAN");	
				return;
		}
		if(isNumber(Vlan_CuridxValue1_down.value) == false)
		{
			alert("请填入大于0的数");
			return;
		}
		if(vlantaglimitdown == '')
			vlantaglimitdown += str;
		else
			vlantaglimitdown += ',' + str;
		Queue = vlantaglimitdown.split(',');
		if(Queue.length > 6)
		{
			alert("最多有６个队列，请调整");
			vlantaglimitdown = vVlanTaglimitDOWN.value;
			return;
		}
		vVlanTaglimitDOWN.value = vlantaglimitdown;
	}
	document.ConfigForm.Speedlimit_flag.value = 1;
	document.ConfigForm.Speedlimitdown_flag.value = 2;
	document.ConfigForm.submit();
} 

function btnAddIP_down()
{
		setDisplay('IpAdd_down',1);
		setDisplay('IpEdit_down',0);
}
function IpAdd_downSubmit()
{
	var str = '';
	var tempbuf = '';
	var dif_elements;
	var Queue;
	
	with(getElById('ConfigForm'))
	{
		str = Ip_Start1_down.value + '-' + Ip_End1_down.value + '/' + Ip_CuridxValue1_down.value;	
		
		
		if(isIpv6Address(Ip_Start1_down.value) == true  || isIpv6Address(Ip_End1_down.value) == true)
			{
				if(Ip_Start1_down.value != Ip_End1_down.value)
				{
					alert("IPv6地址最小值与最大值必须相同");
					return;
				}
			}
			else{
				if(!isValidIpAddress(Ip_Start1_down.value) || !isValidIpAddress(Ip_End1_down.value)) 
				{
					alert("IP地址格式不正确，请填写正确的IP地址");	
					return;
				}
				if(cmpIpAddress(Ip_End1_down.value,Ip_Start1_down.value) != true)
						return ;
			}
		for( i = 0 ; i< ipxa_down.length -1 ;  i ++)
		{
			tempbuf += ipxa_down[i][1] + ',';
		}
		if(iplimitdown != '')
			tempbuf += ipxa_down[ipxa_down.length -1][1];
		else
			tempbuf += '';
			
		dif_elements = different(tempbuf);
		if(issame(Ip_CuridxValue1_down.value , dif_elements) == false && dif_elements.length >= 6 || dif_elements.length > 6)
		{
			alert("最多有６个队列，请调整");
			return;	
		}
		if(isNumber(Ip_CuridxValue1_down.value) == false)
		{
			alert("请填入大于0的数");
			return;
		}
		
		if(iplimitdown == '')
			iplimitdown += str;
		else
			iplimitdown += ',' + str;
		Queue = iplimitdown.split(',');
		if(Queue.length > 6)
		{
			alert("最多有６个队列，请调整");
			iplimitdown = vIplimitDOWN.value;
			return;
		}
		vIplimitDOWN.value = iplimitdown;
		
	}
	document.ConfigForm.Speedlimit_flag.value = 1;
	document.ConfigForm.Speedlimitdown_flag.value = "3";
	document.ConfigForm.submit();
} 
     

function ModeChange_down()
{
	  var ModeIndex = getElById('ModeswitchDOWN').selectedIndex;
	    if(ModeIndex == 0)
	    {
	    	setDisplay('vlantaglimit_down',0);
	      setDisplay('AppEdit_down',0);
	      setDisplay('AppAdd_down',0);
		    setDisplay('interfacelimit_down',0);
		    setDisplay('VlanEdit_down',0);
		    setDisplay('VlanAdd_down',0);
		    setDisplay('iplimit_down',0);
		    setDisplay('IpEdit_down',0);
		    setDisplay('IpAdd_down',0);
	    }
	    else if(ModeIndex == 1)
	    {
	    	setDisplay('interfacelimit_down',1);
	      setDisplay('AppEdit_down',0);
	      setDisplay('AppAdd_down',0);
	      setDisplay('vlantaglimit_down',0);
	      setDisplay('VlanEdit_down',0);
		    setDisplay('VlanAdd_down',0);
	      setDisplay('iplimit_down',0);
	      setDisplay('IpEdit_down',0);
		    setDisplay('IpAdd_down',0);
	    }
	    else if(ModeIndex == 2)
	    {
	    	setDisplay('vlantaglimit_down',1);
	      setDisplay('AppEdit_down',0);
	      setDisplay('AppAdd_down',0);
	      setDisplay('interfacelimit_down',0);
	      setDisplay('VlanEdit_down',0);
		    setDisplay('VlanAdd_down',0);
	      setDisplay('iplimit_down',0);
	      setDisplay('IpEdit_down',0);
		    setDisplay('IpAdd_down',0);
	    }
	    else if(ModeIndex == 3)
	    {
	    	setDisplay('interfacelimit_down',0);
	      setDisplay('AppEdit_down',0);
	      setDisplay('AppAdd_down',0);
	      setDisplay('vlantaglimit_down',0);
	      setDisplay('VlanEdit_down',0);
		    setDisplay('VlanAdd_down',0);
	      setDisplay('iplimit_down',1);
	      setDisplay('IpEdit_down',0);
		    setDisplay('IpAdd_down',0);
	    }
	 
}
function writeLaninfodownTable()
{
		var k,loc;
		
		loc = '<TABLE cellSpacing=0 cellPadding=3 width=465 border=1>';
		loc += '<TR align="middle">';
		loc += '<TD align="center">LAN/SSID</TD>';
		loc += '<TD align="center">限速(单位：512Kbps)</TD>';
		loc+= '<TD align="center">修改</TD>';
		loc += '<TD align="center">删除</TD>';
		loc+= '</TR>';
		if(interfacelimitdown != '')
		{
				for(k=0; k < lanx_down.length; k++)
				{
					loc += '<TR align="middle">';
					loc += '<TD align="center">'+	lanxa_down[k][0] +'</TD>';
					loc += '<TD align="center">'+ lanxa_down[k][1] +'</TD>';
					loc += '<TD align="center"><input name="EditLAN" type="button" id="LANdown_'+ k +'" onClick="btnEditApp_down(this.id)" value="Edit"/></td>\n';
					loc+= '<TD width=92><INPUT id="Deldown_'+ k +'" onclick="btnDelApp_down(this.id)" type=button value=删除 name=DelApp></TD>'
					loc += '</TR>';
				}
		}
		loc += '</TABLE >';
		loc += '<TABLE  width=100% border=0>';
		loc += '<TR>';
		loc += '<TD width="92"><INPUT id="AddApp_down" onclick="btnAddApp_down()" type=button value=添加 name=AddApp_down></TD>';
		loc+= '</TR>';
		loc += '</TABLE >';
		
		getElById('interfacelimit_down').innerHTML = loc;
}

function writevlantagdownTable()
{
		var k,loc;
		
		loc = '<TABLE cellSpacing=0 cellPadding=3 width=465 border=1>';
		loc += '<TR align="middle">';
		loc += '<TD align="center">VLAN</TD>';
		loc += '<TD align="center">限速(单位：512Kbps)</TD>';
		loc+= '<TD align="center">修改</TD>';
		loc += '<TD align="center">删除</TD>';
		loc+= '</TR>';
		if(vlantaglimitdown != '')
		{
				for(k=0; k < vlanx_down.length; k++)
				{
					loc += '<TR align="middle">';
					loc += '<TD align="center">'+	vlanxa_down[k][0] +'</TD>';
					loc += '<TD align="center">'+ vlanxa_down[k][1] +'</TD>';
					loc += '<TD align="center"><input name="EditVlan_down" type="button" id="VLANdown_'+ k +'" onClick="btnEditVlan_down(this.id)" value="Edit"/></td>\n';
					loc+= '<TD width=92><INPUT id="Deldown_'+ k +'" onclick="btnDelVlan_down(this.id)" type=button value=删除 name=DelVlan_down></TD>'
					loc += '</TR>';
				}
		}
		loc += '</TABLE >';
		loc += '<TABLE  width=100% border=0>';
		loc += '<TR>';
		loc += '<TD width="92"><INPUT id="AddVlan_down" onclick="btnAddVlan_down()" type=button value=添加 name=AddVlan_down></TD>';
		loc+= '</TR>';
		loc += '</TABLE >';
		
		getElById('vlantaglimit_down').innerHTML = loc;
}
function writeipdownTable()
{
		var k,loc;
		
		loc = '<TABLE cellSpacing=0 cellPadding=3 width=465 border=1>';
		loc += '<TR align="middle">';
		loc += '<TD align="center">IP范围</TD>';
		loc += '<TD align="center">限速(单位：512Kbps)</TD>';
		loc+= '<TD align="center">修改</TD>';
		loc += '<TD align="center">删除</TD>';
		loc+= '</TR>';
		if(iplimitdown != '')
		{
				for(k=0; k < ipx_down.length; k++)
				{
					loc += '<TR align="middle">';
					loc += '<TD align="center">'+	ipxa_down[k][0] +'</TD>';
					loc += '<TD align="center">'+ ipxa_down[k][1] +'</TD>';
					loc += '<TD align="center"><input name="EditIP" type="button" id="IPdown_'+ k +'" onClick="btnEditIp_down(this.id)" value="Edit"/></td>\n';
					loc+= '<TD width=92><INPUT id="Deldown_'+ k +'" onclick="btnDelIp_down(this.id)" type=button value=删除 name=DelIP_down></TD>'
					loc += '</TR>';
				}
		}
		loc += '</TABLE >';
		loc += '<TABLE  width=100% border=0>';
		loc += '<TR>';
		loc += '<TD width="92"><INPUT id="AddIp_down" onclick="btnAddIP_down()" type=button value=添加 name=AddIP_down></TD>';
		loc+= '</TR>';
		loc += '</TABLE >';
		
		getElById('iplimit_down').innerHTML = loc;
}
	</SCRIPT>

      <TABLE height="100%" cellSpacing=0 cellPadding=0 border=0>
        <TBODY>
        <TR>
          <TD width=157 bgColor=#e7e7e7 height=30>
            <P class=Item_L1>分类标识及策略调度</P></TD>      
          <TD width=7 bgColor=#e7e7e7>　</TD>
          <TD width=474>　</TD>
          <TD vAlign=top width=170 background=/img/panel4.gif rowSpan=11>
            <TABLE cellSpacing=0 cellPadding=20 width="100%" border=0 height='100%'>
              <TBODY>
              <TR>
                <TD valign='top'><A 
                  href="/cgi-bin/help_content.asp#QoS" 
                  target=_blank><IMG height=34 src="/img/help.gif" width=40 
                  border=0></A></TD></TR>
<% if tcwebApi_get("WebCustom_Entry","isCYE8SFUSupported","h") = "Yes" then %>
              <TR>
                <TD valign='bottom'><IMG src='/img/tybottom.jpg' height=76 width=112></TD></TR>
<% end if %>
              </TBODY></TABLE>　 </TD></TR>
        <TR>
          <TD vAlign=top width=157 bgColor=#e7e7e7 height=10></TD>
          <TD width=7 background=/img/panel3.gif>　</TD>
          <TD></TD></TR>
        <TR>
          <TD vAlign=top width=157 bgColor=#e7e7e7 height=30>
            <P class=Item_L2></P></TD>
          <TD width=7 background=/img/panel3.gif>　</TD>
          <TD>
            <FORM name="ConfigForm" action="/cgi-bin/net-qos.asp" method="post">
            <TABLE cellSpacing=0 cellPadding=0 width="100%" border=0>
              <TBODY>
              <TR>
                <TD width=10>&nbsp;</TD>
                <TD>
                  <TABLE width="100%" border=0>
                    <TBODY>
                    <TR>
                      <TD width=108><B>规则模板：</B></TD>
                                <TD colSpan=6>
																<select id=select onChange="QoSChangeMode()" size=1 name="Mode" id='Mode'>
                                </select> 
			                        <SCRIPT language=JavaScript type=text/javascript>
															initQoSTemplate();
															</SCRIPT>
                                  <input type="hidden" name="QoS_Flag" value="0"> </TD>
                              </TR>
                    <TR>
                      <TD height=30><B>启用QoS：</B></TD>
                      <TD width=392><INPUT id="Enable" onclick="QoSEnableChange()" type=checkbox value=0 name="Enable" <%if TCWebApi_get("QoS_Common", "Active","h") = "Yes" then asp_Write("checked") end if%>>
                                  <input type="hidden" name="QosFlag" value="Yes">
                                  <input type="hidden" name="UpBandValue" value="<% tcWebApi_get("QoS_Common","UplinkBandwidth","s") %>">
                                  <input type="hidden" name="DiscplineFlag" value="<% tcWebApi_get("QoS_Common","Discipline","s") %>">
                                  <input type="hidden" name="QoSRuleFlag" value="discRule">
                                  <input type="hidden" name="TempleteString" value="TR069,INTERNET"></TD>
                              </TR></TBODY></TABLE>
                  <DIV id=QOSGlobe>
                  <TABLE width="100%" border=0>
                    <TBODY>
                    <TR>
                      <TD width=108><B>上行带宽：</B></TD>
                                  <TD width=122><input id=Bandwidth maxlength=10 size=16 value="<%if tcWebApi_get("QoS_Common","UplinkBandwidth","h") <> "N/A" then tcWebApi_get("QoS_Common","UplinkBandwidth","s") else asp_Write("0") end if%>" name="Bandwidth">
                                    <script language="JavaScript" type="text/JavaScript">
									</script> 
                                  </TD>
                      <TD width=280>(0,8192-1000000000)bps</TD></TR></TBODY></TABLE>
                  <TABLE width="100%" border=0 id="policy" name="policy">
                    <TBODY>
                    <TR>
                      <TD width=310 height=30><B>调度策略：</B></TD>
                      <TD width=30><INPUT id="Plan" onclick="PlanChange()" type=radio value="priority" name="Plan" <%if tcWebApi_get("QoS_Common","Discipline","h") = "PQ" then asp_Write("checked") end if%>></TD>
                      <TD width=180>PQ</TD>
                      <TD width=30><INPUT id="Plan" onclick="PlanChange()" type=radio value="weight" name="Plan" <%if tcWebApi_get("QoS_Common","Discipline","h") = "WRR" then asp_Write("checked") end if%>></TD>
                      <TD width=180>WRR</TD>
                      <TD width=30></TD>
                      <TD width=180></TD>
                                </TR></TBODY></TABLE>
                  <TABLE width="100%" border=0>
                    <TBODY>
                    <TR id=EnableForce>
                      <TD width=108 height=30><B>使能强制带宽：</B></TD>
                      <TD colSpan=3><INPUT id=EnableForceWeight type=checkbox value=0 name="EnableForceWeight" <%if tcWebApi_get("QoS_Common","EnableForceWeight","h") = "Yes" then asp_Write("checked") end if%>>
                                    <input type="hidden" name="EnableForceFlag" value="No"></TD></TR>
                    <TR>
                      <TD width=130 height=30><B>启用DSCP/TC标志：</B></TD>
                      <TD width=90><INPUT id=EnableDSCPMark type=checkbox 
                         value=0  name="EnableDSCPMark" <%if tcWebApi_get("QoS_Common","EnableDSCPMark","h") = "Yes" then asp_Write("checked") end if%>>
                                    <input type="hidden" name="EnDscpFlag" value="No"></TD>
                                  <TD width=132><B>启用802.1P标志：</B></TD>
                      <TD width=130><SELECT id=Enable8021P size=1 name="Enable8021P"> 
                          <OPTION value="0" <%if tcWebApi_get("QoS_Common","En8021PRemark","h") = "0" then asp_Write("selected") end if%>>0标记</OPTION> 
						  <OPTION value="1" <%if tcWebApi_get("QoS_Common","En8021PRemark","h") = "1" then asp_Write("selected") end if%>>透传</OPTION> 
						  <OPTION value="2" <%if tcWebApi_get("QoS_Common","En8021PRemark","h") = "2" then asp_Write("selected") end if%>>重标记</OPTION>
						  </SELECT></TD></TR></TBODY></TABLE></DIV>
                  <DIV id=COMVlanBtn>
                  <TABLE width="100%" border=0>
                    <TBODY>
                    <TR>
                      <TD width=92><INPUT id=COMVlanCls onclick="VlanSubmit('/cgi-bin/qos-comvlan.asp')" type=button value=设置VLAN值 name=COMVlanCls></TD></TR></TBODY></TABLE></DIV>
                  <DIV id=QueueEdit>
                  <DIV id=PQEdit>
                  <TABLE width=360 border=1>
                    <TBODY>
                    <TR>
                      <TD class=table_title align=middle width=100>队列</TD>
                      <TD class=table_title align=middle>优先级</TD>
                      <TD class=table_title align=middle>使能</TD></TR>
                    <TR>
                      <TD align=middle>Q1</TD>
                      <TD align=middle>最高</TD>
                      <TD align=middle><INPUT id=Q1Enable type=checkbox value="0" <%if tcWebApi_get("QoS_Common","QueueSW1","h") = "Yes" then asp_Write("checked") end if%>></TD></TR>
                    <TR>
                      <TD align=middle>Q2</TD>
                      <TD align=middle>高</TD>
                      <TD align=middle><INPUT id=Q2Enable type=checkbox value=0 <%if tcWebApi_get("QoS_Common","QueueSW2","h") = "Yes" then asp_Write("checked") end if%>></TD></TR>
                    <TR>
                      <TD align=middle>Q3</TD>
                      <TD align=middle>中</TD>
                      <TD align=middle><INPUT id=Q3Enable type=checkbox value=0 <%if tcWebApi_get("QoS_Common","QueueSW3","h") = "Yes" then asp_Write("checked") end if%>></TD></TR>
                    <TR>
                      <TD align=middle>Q4</TD>
                      <TD align=middle>低</TD>
                      <TD align=middle><INPUT id=Q4Enable type=checkbox value=0 <%if tcWebApi_get("QoS_Common","QueueSW4","h") = "Yes" then asp_Write("checked") end if%>></TD></TR>
					  </TBODY></TABLE></DIV>
                  <DIV id=WRREdit>
                  <TABLE width=420 border=1>
                    <TBODY>
                    <TR>
                      <TD class=table_title align=middle width=100>队列</TD>
                      <TD class=table_title align=middle>权重</TD>
                      <TD class=table_title align=middle>使能</TD></TR>
                    <TR>
                      <TD align=middle>Q1</TD>
                      <TD align=middle><INPUT id=Q1Weight name="Q1Weight" size=3 value="<%if tcWebApi_get("QoS_Common","QueueBW1","h") <> "N/A" then tcWebApi_get("QoS_Common","QueueBW1","s") end if%>"></TD>
                      <TD align=middle><INPUT id=Q5Enable type=checkbox value=0 <%if tcWebApi_get("QoS_Common","QueueSW1","h") = "Yes" then asp_Write("checked") end if%>></TD></TR>
                    <TR>
                      <TD align=middle>Q2</TD>
                      <TD align=middle><INPUT id=Q2Weight name="Q2Weight" size=3 value="<%if tcWebApi_get("QoS_Common","QueueBW2","h") <> "N/A" then tcWebApi_get("QoS_Common","QueueBW2","s") end if%>"> 
                      <TD align=middle><INPUT id=Q6Enable type=checkbox value=0 <%if tcWebApi_get("QoS_Common","QueueSW2","h") = "Yes" then asp_Write("checked") end if %>></TD></TR>
                    <TR>
                      <TD align=middle>Q3</TD>
                      <TD align=middle><INPUT id=Q3Weight  name="Q3Weight" size=3 value="<%if tcWebApi_get("QoS_Common","QueueBW3","h") <> "N/A" then tcWebApi_get("QoS_Common","QueueBW3","s") end if%>"> 
                      <TD align=middle><INPUT id=Q7Enable type=checkbox value=0  <%if tcWebApi_get("QoS_Common","QueueSW3","h") = "Yes" then asp_Write("checked") end if %>></TD></TR>
                    <TR>
                      <TD align=middle>Q4</TD>
                      <TD align=middle><INPUT id=Q4Weight name="Q4Weight" size=3 value="<%if tcWebApi_get("QoS_Common","QueueBW4","h") <> "N/A" then tcWebApi_get("QoS_Common","QueueBW4","s") end if%>"> 
                      <TD align=middle><INPUT id=Q8Enable type=checkbox value=0 <%if tcWebApi_get("QoS_Common","QueueSW4","h") = "Yes" then asp_Write("checked") end if%>></TD></TR></TBODY></TABLE></DIV>
                  <DIV id=CAREdit>
                  <TABLE width=420 border=1>
                    <TBODY>
                    <TR>
                      <TD class=table_title align=middle width=100>队列</TD>
                      <TD class=table_title align=middle>带宽(kbps)</TD>
                      <TD class=table_title align=middle>使能</TD></TR>
                    <TR>
                      <TD align=middle>Q1</TD>
                      <TD align=middle><INPUT id=Q1Car size=3 name="Q1Car" value="<%if tcWebApi_get("QoS_Common","QueueBW1","h") <> "N/A" then tcWebApi_get("QoS_Common","QueueBW1","s") end if%>"></TD>
                      <TD align=middle><INPUT id=Q9Enable type=checkbox name="Q9Enable" <%if tcWebApi_get("QoS_Common","QueueSW1","h") = "Yes" then asp_Write("checked") end if%>></TD></TR>
                    <TR>
                      <TD align=middle>Q2</TD>
                      <TD align=middle><INPUT id=Q2Car size=3 name="Q2Car" value="<%if tcWebApi_get("QoS_Common","QueueBW2","h") <> "N/A" then tcWebApi_get("QoS_Common","QueueBW2","s") end if%>"> 
                      <TD align=middle><INPUT id=Q10Enable type=checkbox name="Q10Enable" <%if tcWebApi_get("QoS_Common","QueueSW2","h") = "Yes" then asp_Write("checked") end if%>></TD></TR>
                    <TR>
                      <TD align=middle>Q3</TD>
                      <TD align=middle><INPUT id=Q3Car size=3 name="Q3Car" value="<%if tcWebApi_get("QoS_Common","QueueBW3","h") <> "N/A" then tcWebApi_get("QoS_Common","QueueBW3","s") end if%>"> 
                      <TD align=middle><INPUT id=Q11Enable type=checkbox name="Q11Enable" <%if tcWebApi_get("QoS_Common","QueueSW3","h") = "Yes" then asp_Write("checked") end if%>></TD></TR>
                    <TR>
                      <TD align=middle>Q4</TD>
                      <TD align=middle><INPUT id=Q4Car size=3 name="Q4Car" value="<%if tcWebApi_get("QoS_Common","QueueBW4","h") <> "N/A" then tcWebApi_get("QoS_Common","QueueBW4","s") end if%>"> 
                      <TD align=middle><INPUT id=Q12Enable type=checkbox name="Q12Enable" <%if tcWebApi_get("QoS_Common","QueueSW4","h") = "Yes" then asp_Write("checked") end if%>></TD></TR>
                    <TR>
                      <TD align=middle>Q5</TD>
                      <TD align=middle><INPUT id=Q5Car size=3 name="Q5Car" value="<%if tcWebApi_get("QoS_Common","QueueBW5","h") <> "N/A" then tcWebApi_get("QoS_Common","QueueBW5","s") end if%>"> 
                      <TD align=middle><INPUT id=Q13Enable type=checkbox name="Q13Enable" <%if tcWebApi_get("QoS_Common","QueueSW5","h") = "Yes" then asp_Write("checked") end if%>></TD></TR>
                    <TR>
                      <TD align=middle>Q6</TD>
                      <TD align=middle><INPUT id=Q6Car size=3 name="Q6Car" value="<%if tcWebApi_get("QoS_Common","QueueBW6","h") <> "N/A" then tcWebApi_get("QoS_Common","QueueBW6","s") end if%>"> 
                      <TD align=middle><INPUT id=Q14Enable type=checkbox name="Q14Enable" <%if tcWebApi_get("QoS_Common","QueueSW6","h") = "Yes" then asp_Write("checked") end if%>></TD></TR></TBODY></TABLE></DIV></DIV>
                          <P> 
                            <input type="hidden" name="Q1EnableFlag" value="<% tcWebApi_get("QoS_Common","QueueSW1","s") %>">
                            <input type="hidden" name="Q2EnableFlag" value="<% tcWebApi_get("QoS_Common","QueueSW2","s") %>">
                            <input type="hidden" name="Q3EnableFlag" value="<% tcWebApi_get("QoS_Common","QueueSW3","s") %>">
                            <input type="hidden" name="Q4EnableFlag" value="<% tcWebApi_get("QoS_Common","QueueSW4","s") %>">
                            <input type="hidden" name="Q5EnableFlag" value="<% tcWebApi_get("QoS_Common","QueueSW5","s") %>">
                            <input type="hidden" name="Q6EnableFlag" value="<% tcWebApi_get("QoS_Common","QueueSW6","s") %>">
							
                            <input type="hidden" name="Q1PValue" value="1">
                            <input type="hidden" name="Q2PValue" value="2">
                            <input type="hidden" name="Q3PValue" value="3">
                            <input type="hidden" name="Q4PValue" value="4">
                            <input type="hidden" name="Q5PValue" value="5">
                            <input type="hidden" name="Q6PValue" value="6">
							
                            <input type="hidden" name="Entry_Const_0" value="0">
                            <input type="hidden" name="Entry_Const_1" value="1">
                            <input type="hidden" name="Entry_Const_2" value="2">
                            <input type="hidden" name="Entry_Const_3" value="3">
                            <input type="hidden" name="Entry_Const_4" value="4">
                            <input type="hidden" name="Entry_Const_5" value="5">
                            <input type="hidden" name="Entry_Const_6" value="6">
                            <input type="hidden" name="Entry_Const_7" value="7">
                            <input type="hidden" name="Entry_Const_8" value="8">
                            <input type="hidden" name="Entry_Const_9" value="9">

                            <input type="hidden" name="Entry_Const_10" value="10">
                            <input type="hidden" name="Entry_Const_11" value="11">
                            <input type="hidden" name="Entry_Const_12" value="12">
                            <input type="hidden" name="Entry_Const_13" value="13">
                            <input type="hidden" name="ReCommitFlg" value="-1">
						 	<input type="hidden" name="TypeRuleFlag" value="typeRule">
	  						<input type="hidden" name="AppRuleFlag" value="appRule">
	  						<input type="hidden" name="oldDSCP" value="<% tcWebApi_get("QoS_Common","EnableDSCPMark","s") %>">
	  						<input type="hidden" name="old8021P" value="<% tcWebApi_get("QoS_Common","En8021PRemark","s") %>">
	  						<input type="hidden" name="oldActive" value="<% tcWebApi_get("QoS_Common","Active","s") %>">
	  						<input type="hidden" name="NAValue" value="N/A">
							
                          </P>
                  <DIV id=AddBtn>
                  <TABLE width="100%" border=0>
                    <TBODY>
                    <TR>
                      <TD width=92><INPUT id=AddCls onclick="VlanSubmit('/cgi-bin/qos-clsedit.asp')" type=button value=进入分类编辑页面 name=AddCls></TD></TR></TBODY></TABLE></DIV>
                <TD width=10>&nbsp;</TD></TR>
              <TR>
                <TD width=10>&nbsp;</TD>
                <TD>
                  <TABLE cellSpacing=0 cellPadding=3 width=462 border=1>
                    <TBODY></TBODY></TABLE>
                  <SCRIPT language=JavaScript type=text/javascript>
CurQoSShow();
</SCRIPT>
                          <input type="hidden" name="SaveFlag" value="0"> 
                        <TD width=10>&nbsp;</TD></TR></TBODY></TABLE></TD></TR>
                        	
         <TR>               	
         <TD width=157 bgColor=#e7e7e7 height=30>
            <P class=Item_L1>上行业务限速</P></TD>        
          <TD width=7 bgColor=#e7e7e7>　</TD>
        <TD width=474>　</TD></TR>              	
         <TR>
          <TD vAlign=top width=157 bgColor=#e7e7e7 height=10></TD>
          <TD width=7 background=/img/panel3.gif>　</TD>
          <TD></TD></TR>	
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
              			<TABLE width="100%" border=0>
              				<TBODY>
              					<TR>
              						<TD>上行数据限速模式:</TD>
				              		<TD>
				              			<SELECT id=ModeswitchUP size=1 name="ModeswitchUP" onChange="ModeChange()"> 
				                          <OPTION value="0" <%if tcWebApi_get("DataSpeedLimit_Entry","SpeedLimitModeUP","h") = "0" then asp_Write("selected") end if%>>关闭</OPTION> 
										  						<OPTION value="1" <%if tcWebApi_get("DataSpeedLimit_Entry","SpeedLimitModeUP","h") = "1" then asp_Write("selected") end if%>>基于用户接口限速</OPTION> 
										  						<OPTION value="2" <%if tcWebApi_get("DataSpeedLimit_Entry","SpeedLimitModeUP","h") = "2" then asp_Write("selected") end if%>>基于用户侧VLAN限速</OPTION>
										  						<OPTION value="3" <%if tcWebApi_get("DataSpeedLimit_Entry","SpeedLimitModeUP","h") = "3" then asp_Write("selected") end if%>>基于用户侧设备IP地址段限速</OPTION>
										  			</SELECT>
				              		</TD>
              					</TR>
              				</TBODY>
              			</TABLE>
              			<DIV id=vlantaglimit_up style="display:none"></DIV>
              			<DIV id=interfacelimit_up style="display:none"></DIV>
              			<DIV id=iplimit_up style="display:none"></DIV>
              			<div id="AppEdit" style="display:none">
											<table width="100%" border="0">  <tr>
											<td width="92" height="30">LAN/SSID：</td>
											<td width="381" colspan="6">
											<select size="1" id="AppName" name="AppName" >
												<option value="LAN1" >LAN1</option>
												<% if tcwebApi_get("WebCustom_Entry","isCT1PORTSupported","h") <> "Yes" then %>
												<option value="LAN2" >LAN2</option>
												<% end if %>
												<% if tcwebApi_get("WebCustom_Entry","isCT2PORTSSupported","h") <> "Yes" then %>
												<option value="LAN3" >LAN3</option>
												<option value="LAN4" >LAN4</option>
												<% end if %>
												<% if tcwebApi_get("WebCustom_Entry","isWLanSupported","h") = "Yes" then %>
												<option value="SSID1" >SSID1</option>
												<option value="SSID2" >SSID2</option>
												<option value="SSID3" >SSID3</option>
												<option value="SSID4" >SSID4</option>
												<%end if%>
											</select>
											</td>
											</tr>
											</table>
											<table id = 'CarQueue' width="100%" border="0">  <tr>
											<td width="92">限速：</td>
											<td width="405"><input name="vCuridxValue" type="text" id="vCuridxValue" value="0" size="10" maxlength="10"> </td>
											</tr>
											</table>
											<table width="100%" border="0">
											<tr>
											<td width="92">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
											<input name="AddConfirm" type="button" id="AddConfirm" onClick="LanEditSubmit()" value="提交"/>      </td>
											<input type="hidden" name="vInterfacelimitUP" value="">
											<input type="hidden" name="vVlanTaglimitUP" value="">
											<input type="hidden" name="vIplimitUP" value="">
											<input type="hidden" name="Speedlimitup_flag" value="0">
											<input type="hidden" name="Speedlimit_flag" value="0">
											</tr>
											</table>
										</div>
										<div id="AppAdd" style="display:none">
											<table width="100%" border="0">  <tr>
											<td width="92" height="30">LAN/SSID：</td>
											<td width="381" colspan="6">
											<select size="1" id="AppName1" name="AppName1" >
												<option value="LAN1" >LAN1</option>
												<% if tcwebApi_get("WebCustom_Entry","isCT1PORTSupported","h") <> "Yes" then %>
												<option value="LAN2" >LAN2</option>
												<% end if %>
												<% if tcwebApi_get("WebCustom_Entry","isCT2PORTSSupported","h") <> "Yes" then %>
												<option value="LAN3" >LAN3</option>
												<option value="LAN4" >LAN4</option>
												<% end if %>
												<% if tcwebApi_get("WebCustom_Entry","isWLanSupported","h") = "Yes" then %>
												<option value="SSID1" >SSID1</option>
												<option value="SSID2" >SSID2</option>
												<option value="SSID3" >SSID3</option>
												<option value="SSID4" >SSID4</option>
												<%end if%>
											</select>
											</td>
											</tr>
											</table>
											<table id = 'CarQueue' width="100%" border="0">  <tr>
											<td width="92">限速：</td>
											<td width="405"><input name="vCuridxValue1" type="text" id="vCuridxValue1" value="0" size="10" maxlength="10"> </td>
											</tr>
											</table>
											<table width="100%" border="0">
											<tr>
											<td width="92">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
											<input name="AddConfirm" type="button" id="AddConfirm" onClick="LanAddSubmit()" value="提交"/>      </td>
											
											</tr>
											</table>
										</div>
										
										
										<div id="VlanEdit" style="display:none">
											<table width="100%" border="0">  <tr>
											<td width="92" height="30">VLAN：</td>
											<td width="381" colspan="6">
											<input name="VlanName" type="text" id="VlanName" value="0" size="15" maxlength="15">
											</td>
											</tr>
											</table>
											<table id = 'CarQueue' width="100%" border="0">  <tr>
											<td width="92">限速：</td>
											<td width="405"><input name="Vlan_CuridxValue" type="text" id="Vlan_CuridxValue" value="0" size="10" maxlength="10"> </td>
											</tr>
											</table>
											<table width="100%" border="0">
											<tr>
											<td width="92">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
											<input name="AddConfirm" type="button" id="AddConfirm" onClick="VlanEditSubmit()" value="提交"/>      </td>
											</tr>
											</table>
										</div>
										<div id="VlanAdd" style="display:none">
											<table width="100%" border="0">  <tr>
											<td width="92" height="30">VLAN：</td>
											<td width="381" colspan="6">
											<input name="VlanName1" type="text" id="VlanName1" value="0" size="15" maxlength="15">
											</td>
											</tr>
											</table>
											<table id = 'CarQueue' width="100%" border="0">  <tr>
											<td width="92">限速：</td>
											<td width="405"><input name="Vlan_CuridxValue1" type="text" id="Vlan_CuridxValue1" value="0" size="10" maxlength="10"> </td>
											</tr>
											</table>
											<table width="100%" border="0">
											<tr>
											<td width="92">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
											<input name="AddConfirm" type="button" id="AddConfirm" onClick="VlanAddSubmit()" value="提交"/>      </td>
											</tr>
											</table>
										</div>
											<div id="IpEdit" style="display:none">
											<table width="100%" border="0">  <tr>
											<td width="92" height="30">IP范围：</td>
											<td width="95"><input name="Ip_Start" type="text" id="Ip_Start" value="0" size="15" maxlength="15">
											</td>
											<td width="15" style="text-align:center">-
											</td>
											<td width="295"><input name="Ip_End" type="text" id="Ip_End" value="0" size="15" maxlength="15">
											</td>
											</tr>
											</table>
											<table id = 'CarQueue' width="100%" border="0">  <tr>
											<td width="92">限速：</td>
											<td width="405"><input name="Ip_CuridxValue" type="text" id="Ip_CuridxValue" value="0" size="10" maxlength="10"> </td>
											</tr>
											</table>
											<table width="100%" border="0">
											<tr>
											<td width="92">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
											<input name="AddConfirm" type="button" id="AddConfirm" onClick="IpEditSubmit()" value="提交"/>      </td>
											</tr>
											</table>
										</div>
										<div id="IpAdd" style="display:none">
											<table width="100%" border="0">  <tr>
											<td width="92" height="30">IP范围：</td>
											<td width="95"><input name="Ip_Start1" type="text" id="Ip_Start1" value="0" size="15" maxlength="15">
											</td>
											<td width="15" style="text-align:center">-</td>
											<td width="295"><input name="Ip_End1" type="text" id="Ip_End1" value="0" size="15" maxlength="15">
											</td>
											</td>
											</tr>
											</table>
											<table id = 'CarQueue' width="100%" border="0">  <tr>
											<td width="92">限速：</td>
											<td width="405"><input name="Ip_CuridxValue1" type="text" id="Ip_CuridxValue1" value="0" size="10" maxlength="10"> </td>
											</tr>
											</table>
											<table width="100%" border="0">
											<tr>
											<td width="92">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
											<input name="AddConfirm" type="button" id="AddConfirm" onClick="IpAddSubmit()" value="提交"/>      </td>
											
											</tr>
											</table>
										</div>
              		</TD>
              		<TD width=10>&nbsp;</TD>
              		
                </TR>
              </TBODY>
          </TABLE></TD></TR> 
          <TR>               	
         <TD width=157 bgColor=#e7e7e7 height=30>
            <P class=Item_L1>下行业务限速</P></TD>        
          <TD width=7 bgColor=#e7e7e7>　</TD>
        <TD width=474>　</TD></TR>              	
         <TR>
          <TD vAlign=top width=157 bgColor=#e7e7e7 height=10></TD>
          <TD width=7 background=/img/panel3.gif>　</TD>
          <TD></TD></TR>	
          	
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
              			<TABLE width="100%" border=0>
              				<TBODY>
              					<TR>
              						<TD>下行数据限速模式:</TD>
				              		<TD>
				              			<SELECT id=ModeswitchDOWN size=1 name="ModeswitchDOWN" onChange="ModeChange_down()"> 
				                          <OPTION value="0" <%if tcWebApi_get("DataSpeedLimit_Entry","SpeedLimitModeDOWN","h") = "0" then asp_Write("selected") end if%>>关闭</OPTION> 
										  						<OPTION value="1" <%if tcWebApi_get("DataSpeedLimit_Entry","SpeedLimitModeDOWN","h") = "1" then asp_Write("selected") end if%>>基于用户接口限速</OPTION> 
										  						<OPTION value="2" <%if tcWebApi_get("DataSpeedLimit_Entry","SpeedLimitModeDOWN","h") = "2" then asp_Write("selected") end if%>>基于用户侧VLAN限速</OPTION>
										  						<OPTION value="3" <%if tcWebApi_get("DataSpeedLimit_Entry","SpeedLimitModeDOWN","h") = "3" then asp_Write("selected") end if%>>基于用户侧设备IP地址段限速</OPTION>
										  			</SELECT>
				              		</TD>
              					</TR>
              				</TBODY>
              			</TABLE>
              			<DIV id=vlantaglimit_down style="display:none"></DIV>
              			<DIV id=interfacelimit_down style="display:none"></DIV>
              			<DIV id=iplimit_down style="display:none"></DIV>
              			<div id="AppEdit_down" style="display:none">
											<table width="100%" border="0">  <tr>
											<td width="92" height="30">LAN/SSID：</td>
											<td width="381" colspan="6">
											<select size="1" id="AppName_down" name="AppName_down" >
												<option value="LAN1" >LAN1</option>
												<% if tcwebApi_get("WebCustom_Entry","isCT1PORTSupported","h") <> "Yes" then %>
												<option value="LAN2" >LAN2</option>
												<% end if %>
												<% if tcwebApi_get("WebCustom_Entry","isCT2PORTSSupported","h") <> "Yes" then %>
												<option value="LAN3" >LAN3</option>
												<option value="LAN4" >LAN4</option>
												<% end if %>
												<% if tcwebApi_get("WebCustom_Entry","isWLanSupported","h") = "Yes" then %>
												<option value="SSID1" >SSID1</option>
												<option value="SSID2" >SSID2</option>
												<option value="SSID3" >SSID3</option>
												<option value="SSID4" >SSID4</option>
												<%end if%>
											</select>
											</td>
											</tr>
											</table>
											<table id = 'CarQueue' width="100%" border="0">  <tr>
											<td width="92">限速：</td>
											<td width="405"><input name="vCuridxValue_down" type="text" id="vCuridxValue_down" value="0" size="10" maxlength="10"> </td>
											</tr>
											</table>
											<table width="100%" border="0">
											<tr>
											<td width="92">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
											<input name="AddConfirm" type="button" id="AddConfirm" onClick="LanEdit_downSubmit()" value="提交"/>      </td>
											<input type="hidden" name="vInterfacelimitDOWN" value="">
											<input type="hidden" name="vVlanTaglimitDOWN" value="">
											<input type="hidden" name="vIplimitDOWN" value="">
											<input type="hidden" name="Speedlimitdown_flag" value="0">
											</tr>
											</table>
										</div>
										<div id="AppAdd_down" style="display:none">
											<table width="100%" border="0">  <tr>
											<td width="92" height="30">LAN/SSID：</td>
											<td width="381" colspan="6">
											<select size="1" id="AppName1_down" name="AppName1_down" >
												<option value="LAN1" >LAN1</option>
												<% if tcwebApi_get("WebCustom_Entry","isCT1PORTSupported","h") <> "Yes" then %>
												<option value="LAN2" >LAN2</option>
												<% end if %>
												<% if tcwebApi_get("WebCustom_Entry","isCT2PORTSSupported","h") <> "Yes" then %>
												<option value="LAN3" >LAN3</option>
												<option value="LAN4" >LAN4</option>
												<% end if %>
												<% if tcwebApi_get("WebCustom_Entry","isWLanSupported","h") = "Yes" then %>
												<option value="SSID1" >SSID1</option>
												<option value="SSID2" >SSID2</option>
												<option value="SSID3" >SSID3</option>
												<option value="SSID4" >SSID4</option>
												<%end if%>
											</select>
											</td>
											</tr>
											</table>
											<table id = 'CarQueue' width="100%" border="0">  <tr>
											<td width="92">限速：</td>
											<td width="405"><input name="vCuridxValue1_down" type="text" id="vCuridxValue1_down" value="0" size="10" maxlength="10"> </td>
											</tr>
											</table>
											<table width="100%" border="0">
											<tr>
											<td width="92">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
											<input name="AddConfirm" type="button" id="AddConfirm" onClick="LanAdd_downSubmit()" value="提交"/>      </td>
											
											</tr>
											</table>
										</div>
										
										
										<div id="VlanEdit_down" style="display:none">
											<table width="100%" border="0">  <tr>
											<td width="92" height="30">VLAN：</td>
											<td width="381" colspan="6">
											<input name="VlanName_down" type="text" id="VlanName_down" value="0" size="15" maxlength="15">
											</td>
											
											</tr>
											</table>
											<table id = 'CarQueue' width="100%" border="0">  <tr>
											<td width="92">限速：</td>
											<td width="405"><input name="Vlan_CuridxValue_down" type="text" id="Vlan_CuridxValue_down" value="0" size="10" maxlength="10"> </td>
											</tr>
											</table>
											<table width="100%" border="0">
											<tr>
											<td width="92">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
											<input name="AddConfirm" type="button" id="AddConfirm" onClick="VlanEdit_downSubmit()" value="提交"/>      </td>
											</tr>
											</table>
										</div>
										<div id="VlanAdd_down" style="display:none">
											<table width="100%" border="0">  <tr>
											<td width="92" height="30">VLAN：</td>
											<td width="381" colspan="6">
											<input name="VlanName1_down" type="text" id="VlanName1_down" value="0" size="15" maxlength="15">	
											</td>
											</tr>
											</table>
											<table id = 'CarQueue' width="100%" border="0">  <tr>
											<td width="92">限速：</td>
											<td width="405"><input name="Vlan_CuridxValue1_down" type="text" id="Vlan_CuridxValue1_down" value="0" size="10" maxlength="10"> </td>
											</tr>
											</table>
											<table width="100%" border="0">
											<tr>
											<td width="92">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
											<input name="AddConfirm" type="button" id="AddConfirm" onClick="VlanAdd_downSubmit()" value="提交"/>      </td>
											</tr>
											</table>
										</div>
											<div id="IpEdit_down" style="display:none">
											<table width="100%" border="0">  <tr>
											<td width="92" height="30">IP范围：</td>
											<td width="95"><input name="Ip_Start_down" type="text" id="Ip_Start_down" value="0" size="15" maxlength="15">
											</td>
											<td width="15" style="text-align:center">-
											</td>
											<td width="295"><input name="Ip_End_down" type="text" id="Ip_End_down" value="0" size="15" maxlength="15">
											</td>
											</tr>
											</table>
											<table id = 'CarQueue' width="100%" border="0">  <tr>
											<td width="92">限速：</td>
											<td width="405"><input name="Ip_CuridxValue_down" type="text" id="Ip_CuridxValue_down" value="0" size="10" maxlength="10"> </td>
											</tr>
											</table>
											<table width="100%" border="0">
											<tr>
											<td width="92">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
											<input name="AddConfirm" type="button" id="AddConfirm" onClick="IpEdit_downSubmit()" value="提交"/>      </td>
											</tr>
											</table>
										</div>
										<div id="IpAdd_down" style="display:none">
											<table width="100%" border="0">  <tr>
											<td width="92" height="30">IP范围：</td>
											<td width="95"><input name="Ip_Start1_down" type="text" id="Ip_Start1_down" value="0" size="10" maxlength="15">
											</td>
											<td width="15" style="text-align:center">-
											</td>
											<td width="295"><input name="Ip_End1_down" type="text" id="Ip_End1_down" value="0" size="15" maxlength="15">
											</td>
											</td>
											</tr>
											</table>
											<table id = 'CarQueue' width="100%" border="0">  <tr>
											<td width="92">限速：</td>
											<td width="405"><input name="Ip_CuridxValue1_down" type="text" id="Ip_CuridxValue1_down" value="0" size="10" maxlength="10"> </td>
											</tr>
											</table>
											<table width="100%" border="0">
											<tr>
											<td width="92">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
											<input name="AddConfirm" type="button" id="AddConfirm" onClick="IpAdd_downSubmit()" value="提交"/>      </td>
											
											</tr>
											</table>
										</div>
              		</TD>
              		<TD width=10>&nbsp;</TD>
              		
                </TR>
              </TBODY>
          </TABLE></TD></TR> </FORM>	

</SCRIPT>                        	
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
          <TD width=162 bgColor=#d8d8d8>　</TD>
          <TD width=278 bgColor=#0059A0>　</TD>
          <TD width=196 bgColor=#0059A0>

            <P align=center><IMG id=btnOK 

            onclick="btnSave()" height=23 

            src="/img/ok.gif" width=80 border=0>&nbsp;&nbsp;<IMG id=btnCancel 

            height=23 src="/img/cancel.gif" onclick="RefreshPage()" width=80 border=0></P></TD>

          <TD width=170 bgColor=#313031></TD></TR></TBODY></TABLE></TD></TR></TBODY></TABLE>

</BODY></HTML>
