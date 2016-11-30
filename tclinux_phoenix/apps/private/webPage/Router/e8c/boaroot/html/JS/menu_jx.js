// JavaScript Document
var curUserName = parent.curUser;
if (typeof(curUserName) == "undefined")
	curUserName = '0';
var vPageMap =  parent.pageMap;
if (typeof(vPageMap) == "undefined")
	top.window.location.href="/cgi-bin/content.asp";
var sysUserName = '1';
var sptUserName = '0';
var ctcqdUserName = 'ctcqd';
var bandRegName = 'regAcc';
var usrUserName = 0;
var iCount = 0;
var MenuArray = new Array();

//QS->0
//Sta->1
if(vPageMap[1][0] == '1'){
	if(vPageMap[1][1] == '1')
		MenuArray[iCount++] = new MenuNodeConstruction(1, "状态", "/cgi-bin/sta-device.asp", "");
	else if(vPageMap[1][2] == '1')
		MenuArray[iCount++] = new MenuNodeConstruction(1, "状态", "/cgi-bin/sta-network.asp", "");
	else if(vPageMap[1][3] == '1')
		MenuArray[iCount++] = new MenuNodeConstruction(1, "状态", "/cgi-bin/sta-user.asp", "");
	else if(vPageMap[1][4] == '1')
		MenuArray[iCount++] = new MenuNodeConstruction(1, "状态", "/cgi-bin/sta-VoIP.asp", "");	
	else if(vPageMap[1][5] == '1')
		MenuArray[iCount++] = new MenuNodeConstruction(1, "状态", "/cgi-bin/sta-acs.asp", "");							
}
if(vPageMap[1][1] == '1')
	MenuArray[iCount++] = new MenuNodeConstruction(2, "状态总览", "/cgi-bin/sta-device.asp", "");
if(vPageMap[1][2] == '1')
	MenuArray[iCount++] = new MenuNodeConstruction(2, "网络侧信息", "/cgi-bin/sta-network.asp", "");
if(vPageMap[1][3] == '1')
	MenuArray[iCount++] = new MenuNodeConstruction(2, "用户侧信息", "/cgi-bin/sta-user.asp", "");
if(vPageMap[1][4] == '1')
	MenuArray[iCount++] = new MenuNodeConstruction(2, "宽带语音信息", "/cgi-bin/sta-VoIP.asp", "");
if(vPageMap[1][5] == '1')
	MenuArray[iCount++] = new MenuNodeConstruction(2, "远程管理状态", "/cgi-bin/sta-acs.asp", "");
//Net->2
if(vPageMap[2][0] == '1'){
	if(vPageMap[2][1] == '1')
		MenuArray[iCount++] = new MenuNodeConstruction(1, "网络", "/cgi-bin/net-wanset.asp", "");
	else if(vPageMap[2][2] == '1')
		MenuArray[iCount++] = new MenuNodeConstruction(1, "网络", "/cgi-bin/net-binding.asp", "");
	else if(vPageMap[2][3] == '1')
		MenuArray[iCount++] = new MenuNodeConstruction(1, "网络", "/cgi-bin/net-dhcp.asp", "");
	else if(vPageMap[2][4] == '1')
		MenuArray[iCount++] = new MenuNodeConstruction(1, "网络", "/cgi-bin/net-wlan.asp", "");
	else if(vPageMap[2][5] == '1')
		MenuArray[iCount++] = new MenuNodeConstruction(1, "网络", "/cgi-bin/net-tr069.asp", "");
	else if(vPageMap[2][9] == '1')
		MenuArray[iCount++] = new MenuNodeConstruction(1, "网络", "/cgi-bin/net-phoneapp.asp", "");
	else if(vPageMap[2][6] == '1')
		MenuArray[iCount++] = new MenuNodeConstruction(1, "网络", "/cgi-bin/net-qos.asp", "");
	else if(vPageMap[2][7] == '1')
		MenuArray[iCount++] = new MenuNodeConstruction(1, "网络", "/cgi-bin/net-time.asp", "");
	else if(vPageMap[2][8] == '1')
		MenuArray[iCount++] = new MenuNodeConstruction(1, "网络", "/cgi-bin/net-routeset.asp", "");										
	else if(vPageMap[2][10] == '1')
		MenuArray[iCount++] = new MenuNodeConstruction(1, "网络", "/cgi-bin/qos-dslimit.asp", "");								
}
if(vPageMap[2][1] == '1')
	MenuArray[iCount++] = new MenuNodeConstruction(2, "宽带设置", "/cgi-bin/net-wanset.asp", "");
if(vPageMap[2][2] == '1')
	MenuArray[iCount++] = new MenuNodeConstruction(2, "绑定设置", "/cgi-bin/net-binding.asp", "");
if(vPageMap[2][3] == '1')
	MenuArray[iCount++] = new MenuNodeConstruction(2, "LAN侧地址配置", "/cgi-bin/net-dhcp.asp", "");
if(vPageMap[2][4] == '1')
	MenuArray[iCount++] = new MenuNodeConstruction(2, "WLAN配置", "/cgi-bin/net-wlan.asp", "");
if(vPageMap[2][5] == '1')
	MenuArray[iCount++] = new MenuNodeConstruction(2, "远程管理", "/cgi-bin/net-tr069.asp", "");
if(vPageMap[2][9] == '1')
	MenuArray[iCount++] = new MenuNodeConstruction(2, "手机连接管理", "/cgi-bin/net-phoneapp.asp", "");
if(vPageMap[2][6] == '1')
	MenuArray[iCount++] = new MenuNodeConstruction(2, "QoS", "/cgi-bin/net-qos.asp", "");
if(vPageMap[2][7] == '1')
	MenuArray[iCount++] = new MenuNodeConstruction(2, "时间管理", "/cgi-bin/net-time.asp", "");
if(vPageMap[2][8] == '1')
	MenuArray[iCount++] = new MenuNodeConstruction(2, "路由配置", "/cgi-bin/net-routeset.asp", "");
if(vPageMap[2][10] == '1')
	MenuArray[iCount++] = new MenuNodeConstruction(2, "QoS带宽管理", "/cgi-bin/qos-dslimit.asp", "");
//Sec->3
if(vPageMap[3][0] == '1'){
	if(vPageMap[3][1] == '1')
		MenuArray[iCount++] = new MenuNodeConstruction(1, "安全", "/cgi-bin/sec-urlfilter.asp", "");
	else if(vPageMap[3][2] == '1')
		MenuArray[iCount++] = new MenuNodeConstruction(1, "安全", "/cgi-bin/sec-firewall.asp", "");
	else if(vPageMap[3][3] == '1')
		MenuArray[iCount++] = new MenuNodeConstruction(1, "安全", "/cgi-bin/sec-macfilter.asp", "");
	else if(vPageMap[3][4] == '1')
		MenuArray[iCount++] = new MenuNodeConstruction(1, "安全", "/cgi-bin/sec-portfilter.asp", "");				
}
if(vPageMap[3][1] == '1')
	MenuArray[iCount++] = new MenuNodeConstruction(2, "广域网访问设置", "/cgi-bin/sec-urlfilter.asp", "");
if(vPageMap[3][2] == '1')
	MenuArray[iCount++] = new MenuNodeConstruction(2, "防火墙", "/cgi-bin/sec-firewall.asp", "");
if(vPageMap[3][3] == '1')
	MenuArray[iCount++] = new MenuNodeConstruction(2, "MAC过滤", "/cgi-bin/sec-macfilter.asp", "");
if(vPageMap[3][4] == '1')
	MenuArray[iCount++] = new MenuNodeConstruction(2, "端口过滤", "/cgi-bin/sec-portfilter.asp", "");
//App->4
if(vPageMap[4][0] == '1'){
	if(vPageMap[4][1] == '1')
		MenuArray[iCount++] = new MenuNodeConstruction(1, "应用", "/cgi-bin/app-ddns.asp", "");
	else if(vPageMap[4][2] == '1')
		MenuArray[iCount++] = new MenuNodeConstruction(1, "应用", "/cgi-bin/app-natset.asp", "");
	else if(vPageMap[4][3] == '1')
		MenuArray[iCount++] = new MenuNodeConstruction(1, "应用", "/cgi-bin/app-upnp.asp", "");
	else if(vPageMap[4][4] == '1')
		MenuArray[iCount++] = new MenuNodeConstruction(1, "应用", "/cgi-bin/app-VoIP.asp", "");
	else if(vPageMap[4][5] == '1')
		MenuArray[iCount++] = new MenuNodeConstruction(1, "应用", "/cgi-bin/app-igmpset.asp", "");
	else if(vPageMap[4][6] == '1')
		MenuArray[iCount++] = new MenuNodeConstruction(1, "应用", "/cgi-bin/app-daily.asp", "");						
}
if(vPageMap[4][1] == '1')
	MenuArray[iCount++] = new MenuNodeConstruction(2, "DDNS配置", "/cgi-bin/app-ddns.asp", "");
if(vPageMap[4][2] == '1')
	MenuArray[iCount++] = new MenuNodeConstruction(2, "高级NAT配置", "/cgi-bin/app-natset.asp", "");
if(vPageMap[4][3] == '1')
	MenuArray[iCount++] = new MenuNodeConstruction(2, "UPNP配置", "/cgi-bin/app-upnp.asp", "");
if(vPageMap[4][4] == '1')
	MenuArray[iCount++] = new MenuNodeConstruction(2, "宽带电话设置", "/cgi-bin/app-VoIP.asp", "");
if(vPageMap[4][5] == '1')
	MenuArray[iCount++] = new MenuNodeConstruction(2, "IGMP/MLD设置", "/cgi-bin/app-igmpset.asp", "");
if(vPageMap[4][6] == '1')
	MenuArray[iCount++] = new MenuNodeConstruction(2, "日常应用", "/cgi-bin/app-daily.asp", "");
//Mag->5
if(vPageMap[5][0] == '1'){
	if(vPageMap[5][1] == '1')
		MenuArray[iCount++] = new MenuNodeConstruction(1, "管理", "/cgi-bin/mag-account.asp", "");
	else if(vPageMap[5][2] == '1')
		MenuArray[iCount++] = new MenuNodeConstruction(1, "管理", "/cgi-bin/mag-reset.asp", "");
	else if(vPageMap[5][3] == '1')
		MenuArray[iCount++] = new MenuNodeConstruction(1, "管理", "/cgi-bin/mag-syslogmanage.asp", "");		
}
if(vPageMap[5][1] == '1')
	MenuArray[iCount++] = new MenuNodeConstruction(2, "用户管理", "/cgi-bin/mag-account.asp", "");
if(vPageMap[5][2] == '1')
	MenuArray[iCount++] = new MenuNodeConstruction(2, "设备管理", "/cgi-bin/mag-reset.asp", "");
if(vPageMap[5][3] == '1')
	MenuArray[iCount++] = new MenuNodeConstruction(2, "日志文件管理", "/cgi-bin/mag-syslogmanage.asp", "");
//Diag->6
if(vPageMap[6][0] == '1')
	MenuArray[iCount++] = new MenuNodeConstruction(1, "诊断", "/cgi-bin/diag-quickdiagnose.asp", "");
if(vPageMap[6][1] == '1')
	MenuArray[iCount++] = new MenuNodeConstruction(2, "网络诊断", "/cgi-bin/diag-quickdiagnose.asp", "");
//Help->7
if(vPageMap[7][0] == '1')
	MenuArray[iCount++] = new MenuNodeConstruction(1, "帮助", "/cgi-bin/help.asp", "");
if(vPageMap[7][1] == '1')
	MenuArray[iCount++] = new MenuNodeConstruction(2, "使用帮助", "/cgi-bin/help.asp", "");

MenuArray[iCount++] = new MenuNodeConstruction(0, "", "", "");

function MenuNodeConstruction(Level, Text, Link, Target)
{
    this.Level = Level;
    this.Text = Text;
    this.Link = Link;
    this.Target = Target;
}

function MakeMenu(Selected_Menu)
{
	var Menu = Selected_Menu.split("->");
	MakeMenu_L2(Menu[1], MakeMenu_L1(Menu[0]));
}

function MakeMenu_L1(Menu_Text)
{
	var Menu_L2_Start;
	var Code = '<table border="0" cellpadding="0" cellspacing="0" height="43px">';
	
	for (iCount = 0; MenuArray[iCount].Level != 0; iCount++)
	{
		if (MenuArray[iCount].Level == 1)
		{
			if (MenuArray[iCount].Text != Menu_Text)
			{
				Code += '<td height="15px"><img src="/img/back_button.jpg"></td>';
			}
			else
			{
				Menu_L2_Start = iCount + 1;
				Code += '<td height="15px"><img src="/img/selected_button.jpg"></td>';
			}
		}
	}
	Code += '</tr><tr>';
	for (iCount = 0; MenuArray[iCount].Level != 0; iCount++)
	{
		var str = 'LoadPage(\'' + iCount + '\')';
		if (MenuArray[iCount].Level == 1)
		{
			if (MenuArray[iCount].Text != Menu_Text)
			{
				Code += '<td bgcolor="#EF8218" height="30px"><a href="javascript:' + str + '" target="' + MenuArray[iCount].Target + '" class="Menu_L1_Link"><p align="center">' + MenuArray[iCount].Text + '</p></a></td>';
			}
			else
			{
				Code += '<td bgcolor="#427594" height="30px"><a href="javascript:' + str + '" target="' + MenuArray[iCount].Target + '" class="Menu_L1_Active"><p align="center">' + MenuArray[iCount].Text + '</p></a></td>';
			}
		}
	}
	Code += '</tr></table>';
	getElement('MenuArea_L1').innerHTML = Code;
	return Menu_L2_Start;
}

function LoadPage(strIndex)
{
	var index = parseInt(strIndex);
	location = MenuArray[index].Link;
}

function MakeMenu_L2(Menu_Text, Start)
{
	var Code = '<table border="0" cellpadding="0" cellspacing="0" height="15px"><tr><td width="10px"><td width="7px" class="Menu_L2_Link"><p>|</p></td>';
	for (var iCount = Start; (MenuArray[iCount].Level != 0) && (MenuArray[iCount].Level != 1); iCount++)
	{
		var str = 'LoadPage(\'' + iCount + '\')';
		if (MenuArray[iCount].Level == 2)
		{
			if (MenuArray[iCount].Text != Menu_Text)
			{				
				Code += '<td height="30px"><p align="center"><a href="javascript:' + str + '" target="' + MenuArray[iCount].Target + '" class="Menu_L2_Link"><p> ' + MenuArray[iCount].Text + ' </p></a></td>';
			}
			else
			{
				Code += '<td height="30px"><p align="center"><a href="javascript:' + str + '" target="' + MenuArray[iCount].Target + '" class="Menu_L2_Active"><p> ' + MenuArray[iCount].Text + ' </p></a></td>';
			}
			Code += '<td width="7px" class="Menu_L2_Link"><p>|</p></td>';
		}
	}
	Code += '</tr></table>';
	getElement('MenuArea_L2').innerHTML = Code;
}
 
function DisplayLocation(Selected_Menu)
{
	var Menu = Selected_Menu.split("->");
	getElement('LocationDisplay').innerHTML = Menu[0];
}
