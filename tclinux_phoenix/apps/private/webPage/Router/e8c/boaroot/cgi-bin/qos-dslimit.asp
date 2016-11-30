<%
if Request_Form("speedmodechg") = "1" then
	tcWebApi_set("DataSpeedLimit_Entry","SpeedLimitModeUP","dslimitModeUP")
	tcWebApi_commit("DataSpeedLimit_Entry")
	if Request_Form("dslimitModeUP") = "0" then
		tcWebApi_constSet("QoS_Common","Discipline","PQ")
		tcWebApi_constSet("QoS_Common","QoSOptType","discRule")
		tcWebApi_commit("QoS_Common")
	end if
	tcWebApi_save()
elseif Request_Form("speedmodechg") = "2" then
	tcWebApi_set("DataSpeedLimit_Entry","SpeedLimitModeDOWN","dslimitModeDOWN")
	tcWebApi_commit("DataSpeedLimit_Entry")	
	tcWebApi_save()
elseif Request_Form("speedmodechg") = "3" then
	tcWebApi_set("DataSpeedLimit_Entry","InterfaceLimitUP","dslimitpairupvalues")
	tcWebApi_set("DataSpeedLimit_Entry","InterfaceLimitTypeUP","dslimitpairuptypes")
	tcWebApi_commit("DataSpeedLimit_Entry")	
	tcWebApi_save()
elseif Request_Form("speedmodechg") = "4" then
	tcWebApi_set("DataSpeedLimit_Entry","IPLimitUP","dslimitpairupvalues")
	tcWebApi_set("DataSpeedLimit_Entry","IPLimitTypeUP","dslimitpairuptypes")
	tcWebApi_commit("DataSpeedLimit_Entry")	
	tcWebApi_save()
elseif Request_Form("speedmodechg") = "5" then
	tcWebApi_set("DataSpeedLimit_Entry","InterfaceLimitDOWN","dslimitpairdownvalues")
	tcWebApi_set("DataSpeedLimit_Entry","InterfaceLimitTypeDOWN","dslimitpairdowntypes")
	tcWebApi_commit("DataSpeedLimit_Entry")	
	tcWebApi_save()
elseif Request_Form("speedmodechg") = "6" then
	tcWebApi_set("DataSpeedLimit_Entry","IPLimitDOWN","dslimitpairdownvalues")
	tcWebApi_set("DataSpeedLimit_Entry","IPLimitTypeDOWN","dslimitpairdowntypes")
	tcWebApi_commit("DataSpeedLimit_Entry")	
	tcWebApi_save()
end if
%>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0 Transitional//EN">
<HTML><HEAD><TITLE>QoS带宽设置</TITLE>
<META http-equiv=Content-Language content=zh-cn>
<META http-equiv=Content-Type content="text/html; charset=gb2312"><LINK 
href="/JS/stylemain.css" type=text/css rel=stylesheet>
<style type="text/css">
.title_01 {
	COLOR: #5c5d55
}
.prompt {
	PADDING-BOTTOM: 5px; PADDING-LEFT: 0px; PADDING-RIGHT: 0px; FONT-FAMILY: "Tohama", "Arial", "????"; FONT-SIZE: 12px; PADDING-TOP: 5px
}
.table_title_up {
	BORDER-BOTTOM: #c4d7e5; TEXT-ALIGN: center; BORDER-LEFT: #c4d7e5; LINE-HEIGHT: 20px; BACKGROUND-COLOR: #ffffff; HEIGHT: 20px; FONT-SIZE: 12px; BORDER-TOP: #c4d7e5; FONT-WEIGHT: bold; BORDER-RIGHT: #c4d7e5
}
</style>
<SCRIPT language=javascript src="/JS/menu.js"></SCRIPT>
<SCRIPT language=javascript src="/JS/util.js"></SCRIPT>
<SCRIPT language=JavaScript src="/JS/printtable.js"></SCRIPT>
<script language='JavaScript' type='text/javascript'>
function LoadFrame()
{
}

function cmpIpAddress(address1,address2)
{
		var Lnum = 0;
		var Snum = 0;
		var addrParts1 = address1.split('.');
		var addrParts2 = address2.split('.');
		for (i = 0; i <= 3; i++)
		{
			Lnum |= (parseInt(addrParts1[i]) << (8*(3-i)) );
			Snum |= (parseInt(addrParts2[i]) << (8*(3-i)) );

		}
		Lnum = Lnum >>> 0;
		Snum = Snum >>> 0;
		if (Lnum < Snum)
		{
			return false;
		}

		return true;
}

var dsLimitObjsUP = new Array();
var dsLimitObjsDOWN = new Array();

function dslimitObj(objId, base, speed, objType)
{
	this.objId = objId;
	this.base = base;
	this.speed = speed;
	this.objType = isNaN(objType) ? 0 : objType;
}

function showDSLimitTable(dsmode, objTypes, dsdata, dsLimitObjs, up_down)
{
	var htmlCode = '';
	var i = 1;
	var dataLines = '', objTypeLines = '';
	var idx = 0;
	var HtmlCode = '';

	if ( 1 != dsmode && 3 != dsmode )
		return HtmlCode;
		
	if ( 'N/A' == dsdata )
		dsdata = '';

	HtmlCode += '<tr class="table1_title">';
	if ( 1 == dsmode )
	{
		HtmlCode += '<td width="25%">用户端口</td>';
	}
	else if ( 3 == dsmode )
	{
		HtmlCode += '<td width="25%">用户IP地址</td>';
	}
	HtmlCode += '<td width="30%">限速(单位：512Kbps)</td>';
	HtmlCode += '<td width="15%">类型</td>';
	HtmlCode += '<td width="30%">操作</td>';
	HtmlCode += '</tr>';

	if ( '' == dsdata )
		dataLines = '';
	else
		dataLines = dsdata.split(',');
		
	if ( 'N/A' == objTypes || '' == objTypes )
		objTypeLines = '';
	else
		objTypeLines = objTypes.split(',');
	
	if ( objTypeLines.length != dataLines.length )
	{
		objTypeLines = new Array();
		objTypeLines.push(0);
	}

	for ( idx = 0; idx < dataLines.length; idx++ )
	{
		var pairs = dataLines[idx].split('/');
		if ( 2 != pairs.length || isNaN(parseInt(pairs[1])) )
			continue;
		var out_idx = idx +1;

		var newdslimitObj = new dslimitObj(out_idx, pairs[0], pairs[1], parseInt(objTypeLines[idx]));
		dsLimitObjs.push(newdslimitObj);
		HtmlCode += '<tr class="table1_01" id= "record_' + out_idx +'" align = "center" class="table1_01">';
	  HtmlCode += '<td style="word-break:break-all;word-wrap:break-word;">' + pairs[0] + '</td>';
	  HtmlCode += '<td>' + pairs[1] + '</td>';
	  if ( 0 == newdslimitObj.objType )
	  	HtmlCode += '<td>独占</td>';
	  else
	  	HtmlCode += '<td>共享</td>';
	  HtmlCode += '<td>';

	  HtmlCode += '<input name="editpair_'+ out_idx +'" type="button" onClick="btnEditPair('+out_idx+', '+up_down+')" value="编辑"/>';
	  HtmlCode += '&nbsp;&nbsp;<input name="delpair_'+ out_idx +'" type="button" onClick="btnDelPair('+out_idx+', '+up_down+', this)" value="删除"/>';
	  HtmlCode += '</td>';
	  HtmlCode += '</tr>';
	}
	
	if ( dataLines.length >= 6 )
	{
		if ( 1 == up_down )
			getElById('newpairup').style.display = 'none';
		else if ( 2 == up_down )
			getElById('newpairdown').style.display = 'none';
	}

	return HtmlCode;
}

function dslimitModeChange(up_down)
{
	with ( getElById('ConfigForm') )
	{
		speedmodechg.value = up_down;
		submit();
	}
}

function showItemsUP(idx)
{
	var idx_x = 0;

	with ( getElById('ConfigForm') )
	{
		if ( -1 == idx )
		{
		<% if tcWebApi_get("DataSpeedLimit_Entry","SpeedLimitModeUP","h") = "1" then %>
			SpeedLimitInterfaceUP.value = '';
			setRadio('SpeedLimitTypeUP', '1');
			SpeedLimitUPValue.value = '';
		<% elseif tcWebApi_get("DataSpeedLimit_Entry","SpeedLimitModeUP","h") = "3" then %>
			SpeedLimitIPUPStart.value = '';
			SpeedLimitIPUPEnd.value = '';
			setRadio('SpeedLimitTypeUP', '1');
			SpeedLimitUPValue.value = '';
		<% end if %>
			return true;	
		}

		for ( idx_x = 0; idx_x < dsLimitObjsUP.length; idx_x ++ )
		{
			if ( dsLimitObjsUP[idx_x].objId == idx )
				break;
		}
		if ( idx_x >= dsLimitObjsUP.length )
			return false;

		<% if tcWebApi_get("DataSpeedLimit_Entry","SpeedLimitModeUP","h") = "1" then %>
			SpeedLimitInterfaceUP.value = dsLimitObjsUP[idx_x].base;
			setRadio('SpeedLimitTypeUP', dsLimitObjsUP[idx_x].objType);
			SpeedLimitUPValue.value = dsLimitObjsUP[idx_x].speed;
		<% elseif tcWebApi_get("DataSpeedLimit_Entry","SpeedLimitModeUP","h") = "3" then %>
			var ip_pair = dsLimitObjsUP[idx_x].base.split('-');
			SpeedLimitIPUPStart.value = ip_pair[0];
			SpeedLimitIPUPEnd.value = ip_pair[1];
			setRadio('SpeedLimitTypeUP', dsLimitObjsUP[idx_x].objType);
			SpeedLimitUPValue.value = dsLimitObjsUP[idx_x].speed;
		<% end if %>
	}
	return true;
}

function showItemsDOWN(idx)
{
	var idx_x = 0;

	with ( getElById('ConfigForm') )
	{
		if ( -1 == idx )
		{
		<% if tcWebApi_get("DataSpeedLimit_Entry","SpeedLimitModeDOWN","h") = "1" then %>
			SpeedLimitInterfaceDOWN.value = '';
			setRadio('SpeedLimitTypeDOWN', '1');
			SpeedLimitDOWNValue.value = '';
		<% elseif tcWebApi_get("DataSpeedLimit_Entry","SpeedLimitModeDOWN","h") = "3" then %>
			SpeedLimitIPDOWNStart.value = '';
			SpeedLimitIPDOWNEnd.value = '';
			setRadio('SpeedLimitTypeDOWN', '1');
			SpeedLimitDOWNValue.value = '';
		<% end if %>
			return true;
		}

		for ( idx_x = 0; idx_x < dsLimitObjsDOWN.length; idx_x ++ )
		{
			if ( dsLimitObjsDOWN[idx_x].objId == idx )
				break;
		}
		if ( idx_x >= dsLimitObjsDOWN.length )
			return false;

		<% if tcWebApi_get("DataSpeedLimit_Entry","SpeedLimitModeDOWN","h") = "1" then %>
			SpeedLimitInterfaceDOWN.value = dsLimitObjsDOWN[idx_x].base;
			setRadio('SpeedLimitTypeDOWN', dsLimitObjsDOWN[idx_x].objType);
			SpeedLimitDOWNValue.value = dsLimitObjsDOWN[idx_x].speed;
		<% elseif tcWebApi_get("DataSpeedLimit_Entry","SpeedLimitModeDOWN","h") = "3" then %>
			var ip_pair = dsLimitObjsDOWN[idx_x].base.split('-');
			SpeedLimitIPDOWNStart.value = ip_pair[0];
			SpeedLimitIPDOWNEnd.value = ip_pair[1];
			setRadio('SpeedLimitTypeDOWN', dsLimitObjsDOWN[idx_x].objType);
			SpeedLimitDOWNValue.value = dsLimitObjsDOWN[idx_x].speed;
		<% end if %>
	}
	return true;
}

function btnNewPair(up_down)
{
	with ( getElById('ConfigForm') )
	{
		if ( 1 == up_down )
		{
			getElById('TableUPInfoTitle').innerHTML = '新建用户带宽限速';
			getElById('TableUPInfo').style.display = '';
			dsmodeupop.value = 'new';
			showItemsUP(-1);
		}
		else if ( 2 == up_down )
		{
			getElById('TableDOWNInfoTitle').innerHTML = '新建用户带宽限速';
	  	getElById("TableDOWNInfo").style.display = '';
	  	dsmodedownop.value = 'new';
	  	showItemsDOWN(-1);
	  }
	}
}

function btnEditPair(idx, up_down)
{
	with ( getElById('ConfigForm') )
	{
		if ( 1 == up_down )
		{
			getElById('TableUPInfoTitle').innerHTML = '编辑用户带宽限速';
			getElById('TableUPInfo').style.display = '';
			dsmodeupop.value = 'edit';
			dsmodeupopidx.value = idx;
			showItemsUP(idx);
		}
		else if ( 2 == up_down ) 
		{
			getElById('TableDOWNInfoTitle').innerHTML = '编辑用户带宽限速';
	  	getElById("TableDOWNInfo").style.display = '';
			dsmodedownop.value = 'edit';
			dsmodedownopidx.value = idx;
			showItemsDOWN(idx);
	  }
	}
}

function delPairUP(idx, btnobj)
{
	var idx_x = 0;

	with ( getElById('ConfigForm') )
	{
		var pairs = new Array();
		var pairs_types = new Array();
		for ( idx_x = 0; idx_x < dsLimitObjsUP.length; idx_x ++ )
		{
			if ( dsLimitObjsUP[idx_x].objId == idx )
				continue;
			pairs.push(dsLimitObjsUP[idx_x].base+'/'+dsLimitObjsUP[idx_x].speed);
			pairs_types.push(dsLimitObjsUP[idx_x].objType);
		}

		dslimitpairupvalues.value = pairs.join(',');
		dslimitpairuptypes.value = pairs_types.join(',');	
	<% if tcWebApi_get("DataSpeedLimit_Entry","SpeedLimitModeUP","h") = "1" then %>
		speedmodechg.value = '3';
	<% elseif tcWebApi_get("DataSpeedLimit_Entry","SpeedLimitModeUP","h") = "3" then %>
		speedmodechg.value = '4';
	<% else %>
		alert('非法提交参数');
		return false;
	<% end if %>
	
		btnobj.disabled = true;
		submit();
	}

	return true;
}

function delPairDOWN(idx, btnobj)
{
	var idx_x = 0;

	with ( getElById('ConfigForm') )
	{
		var pairs = new Array();
		var pairs_types = new Array();
		for ( idx_x = 0; idx_x < dsLimitObjsDOWN.length; idx_x ++ )
		{
			if ( dsLimitObjsDOWN[idx_x].objId == idx )
				continue;
			pairs.push(dsLimitObjsDOWN[idx_x].base+'/'+dsLimitObjsDOWN[idx_x].speed);
			pairs_types.push(dsLimitObjsDOWN[idx_x].objType);
		}

		dslimitpairdownvalues.value = pairs.join(',');
		dslimitpairdowntypes.value = pairs_types.join(',');	
	<% if tcWebApi_get("DataSpeedLimit_Entry","SpeedLimitModeDOWN","h") = "1" then %>
		speedmodechg.value = '5';
	<% elseif tcWebApi_get("DataSpeedLimit_Entry","SpeedLimitModeDOWN","h") = "3" then %>
		speedmodechg.value = '6';
	<% else %>
		alert('非法提交参数');
		return false;
	<% end if %>
	
		btnobj.disabled = true;
		submit();
	}

	return true;
}

function btnDelPair(idx, up_down, btnobj)
{
	if( !confirm("您确认要删除吗?") )
	{
		return false;
	}
	
	if ( 1 == up_down )
		return delPairUP(idx, btnobj);
	else if ( 2 == up_down ) 
		return delPairDOWN(idx, btnobj);
}

//用户点击了取消按钮后的代码逻辑
function OnCancelButtonClick(up_down)
{
		if ( 1 == up_down )
			getElById('TableUPInfo').style.display = 'none';
		else if ( 2 == up_down ) 
    	getElById('TableDOWNInfo').style.display = 'none';
    return false;
}

function checkPort( in_port )
{
	var totalPorts = new Array();
	var idx = 0;
	
	totalPorts.push('LAN1');
<% if tcwebApi_get("WebCustom_Entry","isCT1PORTSupported","h") <> "Yes" then %>
	totalPorts.push('LAN2');
<% end if %>
<% if tcwebApi_get("WebCustom_Entry","isCT2PORTSSupported","h") <> "Yes" then %>
	totalPorts.push('LAN3');
	totalPorts.push('LAN4');
<% end if %>
<% if TCWebApi_get("WebCustom_Entry","isWLanSupported","h" ) = "Yes" then %>
	var ssidNum = parseInt('<% tcWebApi_get("WLan_Common", "BssidNum", "s") %>');
	if (isNaN(ssidNum))
		ssidNum = 0;
		
	for ( idx = 1; idx <= ssidNum; idx ++ )
		totalPorts.push('SSID'+idx);
<% end if %>

	for ( idx = 0; idx < totalPorts.length; idx ++ )
	{
		if ( in_port == totalPorts[idx] )
			return true;
	}

	return false;
}

function checkLANIP( start_addr, end_addr )
{
	if ( '' == start_addr
	  	|| (isAbcIpAddress(start_addr) == false) && (isIpv6Address(start_addr) == false) )
	{
		var TipStr = '起始地址[' + start_addr + ']是无效IP地址';
		alert(TipStr);
		return false;
	}
	else if ( '' == end_addr
					|| (isAbcIpAddress(end_addr) == false) && (isIpv6Address(end_addr) == false) )
	{
		var TipStr = '结束地址[' + end_addr + ']是无效IP地址';
		alert(TipStr);
		return false;
	}
	else if ((isAbcIpAddress(start_addr) == true) && (isAbcIpAddress(end_addr) == true)	&& (cmpIpAddress(end_addr, start_addr) == false))
	{
		var TipStr = '起始地址[' + start_addr + ']大于结束地址['+end_addr+']';
		alert(TipStr);
		return false;
	}
	else if ((isIpv6Address(start_addr) == true)	 || (isIpv6Address(end_addr) == true) )
	{
		if (start_addr != end_addr)
		{
			alert("IPv6地址起始地址与结束地址必须相同");
			return false;
		}
	}

	return true;
}

function OnApplyButtonClickUP(btnobj)
{
	var idx = 0;
	var total_bandwidth = '<% tcWebApi_get("QoS_Common","UplinkBandwidth","s") %>';
	var dstotal_bw = 0;
	var dsedit_idx = 0;
	
	if( 0 == parseInt(total_bandwidth)
		|| isNaN(parseInt(total_bandwidth)) )
		total_bandwidth = 1000000000;

	with ( getElById('ConfigForm') )
	{
	<% if tcWebApi_get("DataSpeedLimit_Entry","SpeedLimitModeUP","h") = "1" then %>
		switch( dsmodeupop.value )
		{
			case 'new':
			case 'edit':
				if ( !checkPort(SpeedLimitInterfaceUP.value) )
				{
					alert('LAN端口不存在');
					return false;
				}
				
				for ( idx = 0; idx < dsLimitObjsUP.length; idx ++ )
				{
					if ( 'edit' == dsmodeupop.value
							&& dsmodeupopidx.value == dsLimitObjsUP[idx].objId )
					{
							dsedit_idx = idx;
							continue;
					}

					dstotal_bw += (dsLimitObjsUP[idx].speed * 512 * 1024);
					if ( SpeedLimitInterfaceUP.value
							== dsLimitObjsUP[idx].base )
						{
							alert('LAN端口已经存在');
							return false;
						}
				}
				
				if ( isNaN(parseInt(SpeedLimitUPValue.value))
					|| 0 == parseInt(SpeedLimitUPValue.value) )
				{
					alert('限速速率不合法');
					return false;
				}
				dstotal_bw += (parseInt(SpeedLimitUPValue.value) * 512 * 1024);
				if ( dstotal_bw > 1000000000 )
				{
					alert('带宽之和不能大于上行总带宽');
					return false;
				}
				break;
			default:
				alert('非法提交参数');
				return false;
		}
		
		switch( dsmodeupop.value )
		{
			case 'new':
				var newdslimitObj = new dslimitObj(99, SpeedLimitInterfaceUP.value, SpeedLimitUPValue.value, parseInt(getRadioVal('SpeedLimitTypeUP')));
				dsLimitObjsUP.push(newdslimitObj);
				break;
			case 'edit':
				dsLimitObjsUP[dsedit_idx].base = SpeedLimitInterfaceUP.value;
				dsLimitObjsUP[dsedit_idx].speed = parseInt(SpeedLimitUPValue.value);
				dsLimitObjsUP[dsedit_idx].objType = parseInt(getRadioVal('SpeedLimitTypeUP'));
				break;
			default:
				break;
		}
		
		var pairs = new Array();
		var pairs_types = new Array();
		for ( idx = 0; idx < dsLimitObjsUP.length; idx ++ )
		{
			pairs.push(dsLimitObjsUP[idx].base+'/'+dsLimitObjsUP[idx].speed);
			pairs_types.push(dsLimitObjsUP[idx].objType);
		}

		dslimitpairupvalues.value = pairs.join(',');
		dslimitpairuptypes.value = pairs_types.join(',');
		
		speedmodechg.value = '3';
	<% elseif tcWebApi_get("DataSpeedLimit_Entry","SpeedLimitModeUP","h") = "3" then %>
		switch( dsmodeupop.value )
		{
			case 'new':
			case 'edit':
				if ( !checkLANIP(SpeedLimitIPUPStart.value, SpeedLimitIPUPEnd.value) )
				{
					return false;
				}
				
				for ( idx = 0; idx < dsLimitObjsUP.length; idx ++ )
				{
					if ( 'edit' == dsmodeupop.value
							&& dsmodeupopidx.value == dsLimitObjsUP[idx].objId )
					{
							dsedit_idx = idx;
							continue;
					}

					dstotal_bw += (dsLimitObjsUP[idx].speed * 512 * 1024);
				}
				
				if ( isNaN(parseInt(SpeedLimitUPValue.value))
					|| 0 == parseInt(SpeedLimitUPValue.value) )
				{
					alert('限速速率不合法');
					return false;
				}
				dstotal_bw += (parseInt(SpeedLimitUPValue.value) * 512 * 1024);
				if ( dstotal_bw > 1000000000 )
				{
					alert('带宽之和不能大于上行总带宽');
					return false;
				}
				break;
			default:
				alert('非法提交参数');
				return false;
		}
		
		switch( dsmodeupop.value )
		{
			case 'new':
				var newdslimitObj = new dslimitObj(99, 
										SpeedLimitIPUPStart.value+'-'+SpeedLimitIPUPEnd.value
										, SpeedLimitUPValue.value, parseInt(getRadioVal('SpeedLimitTypeUP')));
				dsLimitObjsUP.push(newdslimitObj);
				break;
			case 'edit':
				dsLimitObjsUP[dsedit_idx].base = 	SpeedLimitIPUPStart.value+'-'+SpeedLimitIPUPEnd.value
				dsLimitObjsUP[dsedit_idx].speed = parseInt(SpeedLimitUPValue.value);
				dsLimitObjsUP[dsedit_idx].objType = parseInt(getRadioVal('SpeedLimitTypeUP'));
				break;
			default:
				break;
		}

		var pairs = new Array();
		var pairs_types = new Array();
		for ( idx = 0; idx < dsLimitObjsUP.length; idx ++ )
		{
			pairs.push(dsLimitObjsUP[idx].base+'/'+dsLimitObjsUP[idx].speed);
			pairs_types.push(dsLimitObjsUP[idx].objType);
		}

		dslimitpairupvalues.value = pairs.join(',');
		dslimitpairuptypes.value = pairs_types.join(',');

		speedmodechg.value = '4';
	<% else %>
		alert('非法提交参数');
		return false;
	<% end if %>

		btnobj.disabled = true;
		submit();
	}

	return true;
}

function OnApplyButtonClickDOWN(btnobj)
{
	var idx = 0;
	var dstotal_bw = 0;
	var dsedit_idx = 0;

	with ( getElById('ConfigForm') )
	{
	<% if tcWebApi_get("DataSpeedLimit_Entry","SpeedLimitModeDOWN","h") = "1" then %>
		switch( dsmodedownop.value )
		{
			case 'new':
			case 'edit':
				if ( !checkPort(SpeedLimitInterfaceDOWN.value) )
				{
					alert('LAN端口不存在');
					return false;
				}
				
				for ( idx = 0; idx < dsLimitObjsDOWN.length; idx ++ )
				{
					if ( 'edit' == dsmodedownop.value
							&& dsmodedownopidx.value == dsLimitObjsDOWN[idx].objId )
					{
							dsedit_idx = idx;
							continue;
					}

					dstotal_bw += (dsLimitObjsDOWN[idx].speed * 512 * 1024);
					if ( SpeedLimitInterfaceDOWN.value
							== dsLimitObjsDOWN[idx].base )
						{
							alert('LAN端口已经存在');
							return false;
						}
				}
				
				if ( isNaN(parseInt(SpeedLimitDOWNValue.value))
					|| 0 == parseInt(SpeedLimitDOWNValue.value) )
				{
					alert('限速速率不合法');
					return false;
				}

				break;
			default:
				alert('非法提交参数');
				return false;
		}
		
		switch( dsmodedownop.value )
		{
			case 'new':
				var newdslimitObj = new dslimitObj(99, SpeedLimitInterfaceDOWN.value, SpeedLimitDOWNValue.value, parseInt(getRadioVal('SpeedLimitTypeDOWN')));
				dsLimitObjsDOWN.push(newdslimitObj);
				break;
			case 'edit':
				dsLimitObjsDOWN[dsedit_idx].base = SpeedLimitInterfaceDOWN.value;
				dsLimitObjsDOWN[dsedit_idx].speed = parseInt(SpeedLimitDOWNValue.value);
				dsLimitObjsDOWN[dsedit_idx].objType = parseInt(getRadioVal('SpeedLimitTypeDOWN'));
				break;
			default:
				break;
		}
		
		var pairs = new Array();
		var pairs_types = new Array();
		for ( idx = 0; idx < dsLimitObjsDOWN.length; idx ++ )
		{
			pairs.push(dsLimitObjsDOWN[idx].base+'/'+dsLimitObjsDOWN[idx].speed);
			pairs_types.push(dsLimitObjsDOWN[idx].objType);
		}

		dslimitpairdownvalues.value = pairs.join(',');
		dslimitpairdowntypes.value = pairs_types.join(',');

		speedmodechg.value = '5';
	<% elseif tcWebApi_get("DataSpeedLimit_Entry","SpeedLimitModeDOWN","h") = "3" then %>
		switch( dsmodedownop.value )
		{
			case 'new':
			case 'edit':
				if ( !checkLANIP(SpeedLimitIPDOWNStart.value, SpeedLimitIPDOWNEnd.value) )
				{
					return false;
				}
				
				for ( idx = 0; idx < dsLimitObjsDOWN.length; idx ++ )
				{
					if ( 'edit' == dsmodedownop.value
							&& dsmodedownopidx.value == dsLimitObjsDOWN[idx].objId )
					{
							dsedit_idx = idx;
							break;
					}
				}
				
				if ( isNaN(parseInt(SpeedLimitDOWNValue.value))
					|| 0 == parseInt(SpeedLimitDOWNValue.value) )
				{
					alert('限速速率不合法');
					return false;
				}

				break;
			default:
				alert('非法提交参数');
				return false;
		}
		
		switch( dsmodedownop.value )
		{
			case 'new':
				var newdslimitObj = new dslimitObj(99, 
										SpeedLimitIPDOWNStart.value+'-'+SpeedLimitIPDOWNEnd.value
										, SpeedLimitDOWNValue.value, parseInt(getRadioVal('SpeedLimitTypeDOWN')));
				dsLimitObjsDOWN.push(newdslimitObj);
				break;
			case 'edit':
				dsLimitObjsDOWN[dsedit_idx].base = 	SpeedLimitIPDOWNStart.value+'-'+SpeedLimitIPDOWNEnd.value
				dsLimitObjsDOWN[dsedit_idx].speed = parseInt(SpeedLimitDOWNValue.value);
				dsLimitObjsDOWN[dsedit_idx].objType = parseInt(getRadioVal('SpeedLimitTypeDOWN'));
				break;
			default:
				break;
		}

		var pairs = new Array();
		var pairs_types = new Array();
		for ( idx = 0; idx < dsLimitObjsDOWN.length; idx ++ )
		{
			pairs.push(dsLimitObjsDOWN[idx].base+'/'+dsLimitObjsDOWN[idx].speed);
			pairs_types.push(dsLimitObjsDOWN[idx].objType);
		}

		dslimitpairdownvalues.value = pairs.join(',');
		dslimitpairdowntypes.value = pairs_types.join(',');

		speedmodechg.value = '6';
	<% else %>
		alert('非法提交参数');
		return false;
	<% end if %>

		btnobj.disabled = true;
		submit();
	}

	return true;
}
</script>

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
<% if tcwebApi_get("WebCustom_Entry","isCTPONTYLOGOSupported","h") = "Yes" then %>
      <TABLE height=80 cellSpacing=0 cellPadding=0 width=808 
      background=/img/framelogo.jpg border=0>
<% else %>
      <TABLE height=117 cellSpacing=0 cellPadding=0 width=808 
      background=/img/framelogo.jpg border=0>
<% end if %> 
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
        <TR>
          <TD class=LocationDisplay id=LocationDisplay align=middle width=163 
          bgColor=#ef8218 rowSpan=3></TD>
          <TD width=434 bgColor=#427594 height=33>
            <P align=right><FONT face=黑体 color=#ffffff><B><FONT face=黑体 
            color=#ffffff size=6><INPUT id=Selected_Menu type=hidden 
            value="网络->QoS带宽管理" name=Selected_Menu> </FONT></B><SPAN 
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
          <TD class=Item_L1 width=157 bgColor=#ef8218 height=30>
            <P>上行业务限速</P></TD>
          <TD width=7 bgColor=#ef8218>　</TD>
          <TD width=474>　</TD>
          <TD vAlign=top width=170 background=/img/panel4.gif 
          rowSpan='7'>
            <TABLE cellSpacing=0 cellPadding=20 width="100%" border=0 height='100%'>
              <TBODY>
              <TR>
                <TD valign='top'><A href="/cgi-bin/help_content.asp#QoS带宽管理" 
                  target=_blank><IMG height=34 src="/img/help.gif" 
                  width=40 border=0></A></TD></TR>
<% if tcwebApi_get("WebCustom_Entry","isWebTYLOGOSupported","h") = "Yes" then %>
              <TR>
                <TD valign='bottom'><IMG src='/img/tybottom.jpg' height=76 width=112></TD></TR>
<% end if %>
              </TBODY></TABLE></TD></TR>
        <TR>
          <TD vAlign=top width=157 bgColor=#e7e7e7 height=10></TD>
          <TD width=7 background=/img/panel3.gif>　</TD>
          <TD></TD></TR>
        <TR>
          <TD class=Item_L2 vAlign=top width=157 bgColor=#e7e7e7 height=30>
            <P></P></TD>
          <TD width=7 background=/img/panel3.gif>　</TD>
          <TD>
          	<FORM name="ConfigForm" action="/cgi-bin/qos-dslimit.asp" method="post">
					<!--解释头-->
					<div id="PromptPanel">
						    <table width="100%" border="0" cellpadding="0" cellspacing="0">
						        <tr>
						            <td class="prompt">
						                <table width="100%" border="0" cellspacing="0" cellpadding="0">
						                    <tr>
						                        <td width="100%" class="title_01" style="padding-left: 10px;">
						                            开启上行数据限速模式将自动关闭QoS的"SP/WRR"调度策略。
						                        </td>
						                    </tr>
						                </table>
						            </td>
						        </tr>
						        <tr>
						        <td height="5px"></td>
						        </tr>
						
						    </table>
						  
					</div>

					<!--TODO:在这里加入你的具体页面展现-->
					  <table class="table1_bg" border="0" cellspacing="1" style="margin-left: 5px;" width="98%">
					  	<tr class="table_title_up">
						       <td >上行数据限速模式:</td>
						       <td colspan='3' align='left'>
						       	<input type='hidden' name='speedmodechg' value='0'>	
						       	<input type='hidden' name='dsmodeupop' value='0'>
						       	<input type='hidden' name='dsmodedownop' value='0'>
						       	<input type='hidden' name='dsmodeupopidx' value='0'>
						       	<input type='hidden' name='dsmodedownopidx' value='0'>
						      	<input type='hidden' name='dslimitpairupvalues' value='0'>
						      	<input type='hidden' name='dslimitpairdownvalues' value='0'>
						      	<input type='hidden' name='dslimitpairuptypes' value='0'>
						      	<input type='hidden' name='dslimitpairdowntypes' value='0'>
						      	
				              			<select id='dslimitModeUP' size='1' name='dslimitModeUP' onChange='dslimitModeChange(1)'> 
				                          <option value="0" <%if tcWebApi_get("DataSpeedLimit_Entry","SpeedLimitModeUP","h") = "0" then asp_Write("selected") end if%>>关闭</option> 
										  						<option value="1" <%if tcWebApi_get("DataSpeedLimit_Entry","SpeedLimitModeUP","h") = "1" then asp_Write("selected") end if%>>基于用户端口限速</option> 
										  						<option value="3" <%if tcWebApi_get("DataSpeedLimit_Entry","SpeedLimitModeUP","h") = "3" then asp_Write("selected") end if%>>基于用户IP地址限速</option>
										  			</select>
						      &nbsp;&nbsp;&nbsp;&nbsp;
					       					<% if tcWebApi_get("DataSpeedLimit_Entry","SpeedLimitModeUP","h") <> "0" then %>
									  			<input name='newpairup' id='newpairup' type='button' onClick='btnNewPair(1)' value='新 建'/>
									  			<% end if %>
						       </td>
						  </tr>
						  <script language='JavaScript' type='text/javascript'>
						  <% if tcWebApi_get("DataSpeedLimit_Entry","SpeedLimitModeUP","h") = "1" then %>
						  var interfacelimitup = '<%tcWebApi_get("DataSpeedLimit_Entry","InterfaceLimitUP","s")%>';
						  var interfacelimittypeup = '<%tcWebApi_get("DataSpeedLimit_Entry","InterfaceLimitTypeUP","s")%>';
					    document.write( showDSLimitTable(1, interfacelimittypeup, interfacelimitup, dsLimitObjsUP, 1) );
						  <% elseif tcWebApi_get("DataSpeedLimit_Entry","SpeedLimitModeUP","h") = "3" then %>
						  var iplimitup = '<%tcWebApi_get("DataSpeedLimit_Entry","IPLimitUP","s")%>';
						  var iplimittypeup = '<%tcWebApi_get("DataSpeedLimit_Entry","IPLimitTypeUP","s")%>';
					    document.write( showDSLimitTable(3, iplimittypeup, iplimitup, dsLimitObjsUP, 1) );
							<% end if %>
							 </script>
					 </table>

					 <!--Apply和Cancel按钮-->
					  <div id="TableUPInfo" style="display:none">
					  <table cellspacing="1" cellpadding="0" class="table1_bg" style="margin-left: 5px;" width="98%">
					    <tr class="trTabConfigure">  
					    <td align= left colspan='2' class="table1_left">
					    	<label id="TableUPInfoTitle"></label>
					    </td>
					    </tr>
					    <tr class="trTabConfigure">  
					    <td align= left width=25% class="table1_left">限速类型: </td>
					    <td  class="table1_right"> 
					     <% if tcWebApi_get("DataSpeedLimit_Entry","SpeedLimitModeUP","h") = "1" then %>
					     基于端口
					     <% elseif tcWebApi_get("DataSpeedLimit_Entry","SpeedLimitModeUP","h") = "3" then %>
					     基于IP地址
					     <% end if %>
					    </td>
					    </tr>
					    <% if tcWebApi_get("DataSpeedLimit_Entry","SpeedLimitModeUP","h") = "1" then %>
					    <tr class="trTabConfigure">
					    <td  align=left width="25%" class="table1_left">LAN端口:</td><td  class="table1_right">
									<input type=text id='SpeedLimitInterfaceUP' name='SpeedLimitInterfaceUP' style="width:200px" maxlength=16 />
									(如LAN1<% if TCWebApi_get("WebCustom_Entry","isWLanSupported","h" ) = "Yes" then %>或SSID1<% end if %>)
					    </td>
					    </tr>
					    <% elseif tcWebApi_get("DataSpeedLimit_Entry","SpeedLimitModeUP","h") = "3" then %>
					    <tr class="trTabConfigure">
					    <td  align=left width="25%" class="table1_left">起始地址:</td><td  class="table1_right">
									<input type=text id='SpeedLimitIPUPStart' name='SpeedLimitIPUPStart'  style="width:200px" maxlength=64 />
					    </td>
					    </tr>
					    <tr class="trTabConfigure">
					    <td  align=left width="25%" class="table1_left">结束地址:</td><td  class="table1_right">
									<input type=text id='SpeedLimitIPUPEnd' name='SpeedLimitIPUPEnd'  style="width:200px" maxlength=64 />
					    </td>
					    </tr>
					    <% end if %>
					    <tr class="trTabConfigure">
					    <td  align=left width="25%" class="table1_left">类型:</td><td  class="table1_right">
								  <input type='radio' id='SpeedLimitTypeUP' name='SpeedLimitTypeUP' value='1' checked >共享
								  &nbsp;
								  <input type='radio' id='SpeedLimitTypeUP' name='SpeedLimitTypeUP' value='0' >独占
					    </td>
					    </tr>
					    <tr class="trTabConfigure">
					    <td  align=left width="25%" class="table1_left">方向:</td><td class="table1_right">
								  上传
					    </td>
					    </tr>
					    <tr class="trTabConfigure">
					    <td  align=left width="25%" class="table1_left">限速速率:</td><td class="table1_right">
									<input type=text id='SpeedLimitUPValue' style='width:200px' maxlength=4 />(*512Kbps)
					    </td>
					    </tr>
					  </table>

					  <table id="ConfigPanelButtons" width="100%" cellspacing="1" class="table1_button">
					    <tr>
					        <td width="25%">
					        </td>
					        <td class="table1_submit" style="padding-left: 5px">
					            <input id="ButtonApplyUP"  type="button" value="应用" onclick="javascript:return OnApplyButtonClickUP(this);" class="submit" />
					            <input id="ButtonCancelUP" type="button" value="取消" onclick="javascript:OnCancelButtonClick(1);" class="submit" />
					        </td>
					    </tr>
					  </table>
					</div>
            </TD></TR>

				<TR>
          <TD class=Item_L1 width=157 bgColor=#ef8218 height=30>
            <P>下行业务限速</P></TD>
          <TD width=7 bgColor=#ef8218>　</TD>
          <TD width=474>　</TD></TR>
        <TR>
          <TD vAlign=top width=157 bgColor=#e7e7e7 height=10></TD>
          <TD width=7 background=/img/panel3.gif>　</TD>
          <TD></TD></TR>
        <TR>
          <TD vAlign=top width=157 bgColor=#e7e7e7 height=30>
            <P class=Item_L2>&nbsp;</P></TD>
          <TD width=7 background=/img/panel3.gif>　</TD>
          <TD>
					<!--解释头-->
					<div id="PromptPanel">
						    <table width="100%" border="0" cellpadding="0" cellspacing="0">
						        <tr>
						            <td class="prompt">
						                <table width="100%" border="0" cellspacing="0" cellpadding="0">
						                    <tr>
						                        <td width="100%" class="title_01" style="padding-left: 10px;">
						                        </td>
						                    </tr>
						                </table>
						            </td>
						        </tr>
						        <tr>
						        <td height="5px"></td>
						        </tr>
						
						    </table>
						  
					</div>

					<!--TODO:在这里加入你的具体页面展现-->
					  <table class="table1_bg" border="0" cellspacing="1" style="margin-left: 5px;" width="98%">
					  	<tr class="table_title_up">
						       <td >下行数据限速模式:</td>
						       <td colspan='3' align='left'>
				              			<select id='dslimitModeDOWN' size='1' name='dslimitModeDOWN' onChange='dslimitModeChange(2)'> 
				                          <option value="0" <%if tcWebApi_get("DataSpeedLimit_Entry","SpeedLimitModeDOWN","h") = "0" then asp_Write("selected") end if%>>关闭</option> 
										  						<option value="1" <%if tcWebApi_get("DataSpeedLimit_Entry","SpeedLimitModeDOWN","h") = "1" then asp_Write("selected") end if%>>基于用户端口限速</option> 
										  						<option value="3" <%if tcWebApi_get("DataSpeedLimit_Entry","SpeedLimitModeDOWN","h") = "3" then asp_Write("selected") end if%>>基于用户IP地址限速</option>
										  			</select>
						       &nbsp;&nbsp;&nbsp;&nbsp;
										  			<% if tcWebApi_get("DataSpeedLimit_Entry","SpeedLimitModeDOWN","h") <> "0" then %>
										  			<input name='newpairdown' id='newpairdown' type='button' onClick='btnNewPair(2)' value='新 建'/>
										  			<% end if %>
						       </td>
						  </tr>
						  <script language='JavaScript' type='text/javascript'>
						  <% if tcWebApi_get("DataSpeedLimit_Entry","SpeedLimitModeDOWN","h") = "1" then %>
						  var interfacelimitdown = '<%tcWebApi_get("DataSpeedLimit_Entry","InterfaceLimitDOWN","s")%>';
						  var interfacelimittypedown = '<%tcWebApi_get("DataSpeedLimit_Entry","InterfaceLimitTypeDOWN","s")%>';
					    document.write( showDSLimitTable(1, interfacelimittypedown, interfacelimitdown, dsLimitObjsDOWN, 2) );
						  <% elseif tcWebApi_get("DataSpeedLimit_Entry","SpeedLimitModeDOWN","h") = "3" then %>
						  var iplimitdown = '<%tcWebApi_get("DataSpeedLimit_Entry","IPLimitDOWN","s")%>';
						  var iplimittypedown = '<%tcWebApi_get("DataSpeedLimit_Entry","IPLimitTypeDOWN","s")%>';
					    document.write( showDSLimitTable(3, iplimittypedown, iplimitdown, dsLimitObjsDOWN, 2) );
							<% end if %>
							 </script>
					 </table>
	
					 <!--Apply和Cancel按钮-->
					  <div id="TableDOWNInfo" style="display:none">
					  <table cellspacing="1" cellpadding="0" class="table1_bg" style="margin-left: 5px;" width="98%">
					    <tr class="trTabConfigure">  
					    <td align= left colspan='2' class="table1_left">
					    	<label id="TableDOWNInfoTitle"></label>
					    </td>
					    </tr>
					    <tr class="trTabConfigure">  
					    <td align= left width=25% class="table1_left">限速类型: </td>
					    <td  class="table1_right"> 
					     <% if tcWebApi_get("DataSpeedLimit_Entry","SpeedLimitModeDOWN","h") = "1" then %>
					     基于端口
					     <% elseif tcWebApi_get("DataSpeedLimit_Entry","SpeedLimitModeDOWN","h") = "3" then %>
					     基于IP地址
					     <% end if %>
					    </td>
					    </tr>
					    <% if tcWebApi_get("DataSpeedLimit_Entry","SpeedLimitModeDOWN","h") = "1" then %>
					    <tr class="trTabConfigure">
					    <td  align=left width="25%" class="table1_left">LAN端口:</td><td  class="table1_right">
									<input type=text id='SpeedLimitInterfaceDOWN' name='SpeedLimitInterfaceDOWN' style="width:200px" maxlength=16 />
									(如LAN1)
					    </td>
					    </tr>
					    <% elseif tcWebApi_get("DataSpeedLimit_Entry","SpeedLimitModeDOWN","h") = "3" then %>
					    <tr class="trTabConfigure">
					    <td  align=left width="25%" class="table1_left">起始地址:</td><td  class="table1_right">
									<input type=text id='SpeedLimitIPDOWNStart' name='SpeedLimitIPDOWNStart'  style="width:200px" maxlength=64 />
					    </td>
					    </tr>
					    <tr class="trTabConfigure">
					    <td  align=left width="25%" class="table1_left">结束地址:</td><td  class="table1_right">
									<input type=text id='SpeedLimitIPDOWNEnd' name='SpeedLimitIPDOWNEnd'  style="width:200px" maxlength=64 />
					    </td>
					    </tr>
					    <% end if %>
					    <tr class="trTabConfigure">
					    <td  align=left width="25%" class="table1_left">类型:</td><td  class="table1_right">
								  <input type='radio' id='SpeedLimitTypeDOWN' name='SpeedLimitTypeDOWN' value='1' checked >共享
								  &nbsp;
								  <input type='radio' id='SpeedLimitTypeDOWN' name='SpeedLimitTypeDOWN' value='0' >独占
					    </td>
					    </tr>
					    <tr class="trTabConfigure">
					    <td  align=left width="25%" class="table1_left">方向:</td><td class="table1_right">
								  下载
					    </td>
					    </tr>
					    <tr class="trTabConfigure">
					    <td  align=left width="25%" class="table1_left">限速速率:</td><td class="table1_right">
									<input type=text id='SpeedLimitDOWNValue' style='width:200px' maxlength=4 />(*512Kbps)
					    </td>
					    </tr>
					  </table>

					  <table id="ConfigPanelButtons" width="100%" cellspacing="1" class="table1_button">
					    <tr>
					        <td width="25%">
					        </td>
					        <td class="table1_submit" style="padding-left: 5px">
					            <input id="ButtonApplyDOWN"  type="button" value="应用" onclick="javascript:return OnApplyButtonClickDOWN(this);" class="submit" />
					            <input id="ButtonCancelDOWN" type="button" value="取消" onclick="javascript:OnCancelButtonClick(2);" class="submit" />
					        </td>
					    </tr>
					  </table>
					</div>
					
				</TD></TR>  
</FORM>
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
            <P align=center></P></TD>
          <TD width=170 
bgColor=#313031>　</TD></TR></TBODY></TABLE></TD></TR></TBODY></TABLE></BODY></HTML>
