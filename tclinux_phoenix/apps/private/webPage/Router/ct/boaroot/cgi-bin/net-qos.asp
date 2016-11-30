<%
if Request_Form("QoS_Flag") = "1" then 
	TCWebApi_set("WebCurSet_Entry","qos_id","Mode")
	TCWebApi_set("QoS_Common","Mode","TempleteString")
elseif Request_Form("QoS_Flag") = "2" then
	TCWebApi_set("QoS_Common","Discipline","DiscplineFlag")
	TCWebApi_commit("QoS_Common")
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
	else if("car" == getRadioVal("Plan"))
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
	}
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
            value="网络->QoS" name=Selected_Menu> </FONT></B><SPAN 
            class=GatewayName>网关名称:
            <SCRIPT language=javascript>
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
				else
				{
					setDisplay('PQEdit',0);
					setDisplay('WRREdit',0);
					setDisplay('CAREdit',1);
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
					Form.TempleteString.value = "INTERNET,TR069";
				break;
				case "21":
					Form.TempleteString.value = "INTERNET,TR069,VOIP";
				break;
				case "22":
					Form.TempleteString.value = "INTERNET,TR069,IPTV";
				break;
				case "23":
					Form.TempleteString.value = "INTERNET,TR069,VOIP,IPTV";
				break;
				case "24":
					Form.TempleteString.value = "OTHER";
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
					else
						Plan[2].checked = true;
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
		}
		
		function VlanSubmit(filename)
		{
			location.replace(filename);
		}
	</SCRIPT>

      <TABLE height="100%" cellSpacing=0 cellPadding=0 border=0>
        <TBODY>
        <TR>
          <TD width=157 bgColor=#ef8218 height=30>
            <P class=Item_L1>QoS</P></TD>
          <TD width=7 bgColor=#ef8218>　</TD>
          <TD width=474>　</TD>
          <TD vAlign=top width=170 background=/img/panel4.gif rowSpan=5>
            <TABLE cellSpacing=0 cellPadding=20 width="100%" border=0>
              <TBODY>
              <TR>
                <TD><A 
                  href="/cgi-bin/help_content.asp#QoS" 
                  target=_blank><IMG height=34 src="/img/help.gif" width=40 
                  border=0></A></TD></TR></TBODY></TABLE>　 </TD></TR>
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
								<select id=select onChange="QoSChangeMode()" size=1 name="Mode">
                                    <option value="20" <%if tcWebApi_get("QoS_Common","Mode","h") = "INTERNET,TR069" then asp_Write("selected") end if %>>INTERNET,TR069</option>
                                    <option value="21" <%if tcWebApi_get("QoS_Common","Mode","h") = "INTERNET,TR069,VOIP" then asp_Write("selected") end if %>>INTERNET,TR069,VOIP</option>
                                    <option value="22" <%if tcWebApi_get("QoS_Common","Mode","h") = "INTERNET,TR069,IPTV" then asp_Write("selected") end if %>>INTERNET,TR069,IPTV</option>
                                    <option value="23" <%if tcWebApi_get("QoS_Common","Mode","h") = "INTERNET,TR069,VOIP,IPTV" then asp_Write("selected") end if %>>INTERNET,TR069,VOIP,IPTV</option>
                                    <option value="24" <%if tcWebApi_get("QoS_Common","Mode","h") = "OTHER" then asp_Write("selected") end if %>>OTHER</option>
                                </select> 
                                  <input type="hidden" name="QoS_Flag" value="0"> </TD>
                              </TR>
                    <TR>
                      <TD height=30><B>启用QoS：</B></TD>
                      <TD width=392><INPUT id="Enable" onclick="QoSEnableChange()" type=checkbox value=0 name="Enable" <%if TCWebApi_get("QoS_Common", "Active","h") = "Yes" then asp_Write("checked") end if%>>
                                  <input type="hidden" name="QosFlag" value="Yes">
                                  <input type="hidden" name="UpBandValue" value="<% tcWebApi_get("QoS_Common","UplinkBandwidth","s") %>">
                                  <input type="hidden" name="DiscplineFlag" value="<% tcWebApi_get("QoS_Common","Discipline","s") %>">
                                  <input type="hidden" name="QoSRuleFlag" value="discRule">
                                  <input type="hidden" name="TempleteString" value="INTERNET,TR069"></TD>
                              </TR></TBODY></TABLE>
                  <DIV id=QOSGlobe>
                  <TABLE width="100%" border=0>
                    <TBODY>
                    <TR>
                      <TD width=108><B>上行带宽：</B></TD>
                                  <TD width=122><input id=Bandwidth maxlength=8 size=16 value="<%if tcWebApi_get("QoS_Common","UplinkBandwidth","h") <> "N/A" then tcWebApi_get("QoS_Common","UplinkBandwidth","s") else asp_Write("0") end if%>" name="Bandwidth">
                                    <script language="JavaScript" type="text/JavaScript">
									</script> 
                                  </TD>
                      <TD width=280>(0-1024000)bps</TD></TR></TBODY></TABLE>
                  <TABLE width="100%" border=0>
                    <TBODY>
                    <TR>
                      <TD width=310 height=30><B>调度策略：</B></TD>
                      <TD width=30><INPUT id="Plan" onclick="PlanChange()" type=radio value="priority" name="Plan" <%if tcWebApi_get("QoS_Common","Discipline","h") = "PQ" then asp_Write("checked") end if%>></TD>
                      <TD width=180>PQ</TD>
                      <TD width=30><INPUT id="Plan" onclick="PlanChange()" type=radio value="weight" name="Plan" <%if tcWebApi_get("QoS_Common","Discipline","h") = "WRR" then asp_Write("checked") end if%>></TD>
                      <TD width=180>WRR</TD>
                      <TD width=30><INPUT id="Plan" onclick="PlanChange()"  type=radio value="car" name="Plan" <%if tcWebApi_get("QoS_Common","Discipline","h") = "CAR" then asp_Write("checked") elseif tcWebApi_get("QoS_Common","Discipline","h") = "N/A" then asp_Write("checked") end if%>></TD>
                                  <TD width=553>CAR </TD>
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
          <TD width=162 bgColor=#ef8218>　</TD>
          <TD width=278 bgColor=#427594>　</TD>

          <TD width=196 bgColor=#427594>

            <P align=center><IMG id=btnOK 

            onclick="btnSave()" height=23 

            src="/img/ok.gif" width=80 border=0>&nbsp;&nbsp;<IMG id=btnCancel 

            height=23 src="/img/cancel.gif" onclick="RefreshPage()" width=80 border=0></P></TD>

          <TD width=170 bgColor=#313031></TD></TR></TBODY></TABLE></TD></TR></TBODY></TABLE>

</BODY></HTML>
