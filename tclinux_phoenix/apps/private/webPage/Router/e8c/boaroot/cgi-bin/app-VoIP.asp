<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0 Transitional//EN">
<HTML>
    <HEAD>
        <TITLE>
            宽带电话设置
        </TITLE>
        <META http-equiv=Content-Language content=zh-cn>
        <META content="MSHTML 6.00.6000.16809" name=GENERATOR>
        <META http-equiv=Content-Type content="text/html; charset=gb2312">
        <LINK href="/JS/stylemain.css" type=text/css rel=stylesheet>
        <SCRIPT language=javascript src="/JS/menu.js"></SCRIPT>
        <SCRIPT language=javascript src="/JS/util.js"></SCRIPT>
        <SCRIPT language=JavaScript src="/JS/printtable.js"></SCRIPT>
        <SCRIPT language=JavaScript type=text/javascript>
        var selctIndex = -1;
        var CfgMode = 'JSCT';

        function stMediaPortName(Domain, X_HW_PortName_RTP) {
            this.Domain = Domain;
            this.X_HW_PortName_RTP = X_HW_PortName_RTP;
        }
        var MediaPortName = new Array(new stMediaPortName("InternetGatewayDevice.Services.VoiceService.1.VoiceProfile.1.RTP", ""), null);

        function stProfile(Domain, Region, X_HW_DigitMapMatchMode, X_HW_PortName, DigitMap) {
            this.Domain = Domain;
            this.Region = Region;
            this.X_HW_DigitMapMatchMode = X_HW_DigitMapMatchMode;
            this.X_HW_PortName = X_HW_PortName;
            var temp = Domain.split('.');
            this.key = '.' + temp[5] + '.';
            this.Relating = new stSIPServer("", "", "5060", "", "5060", "", "5060");

            this.DigitMap = DigitMap;
        }

        var SipDigitMap = '[*#][0-9][0-9*].#|**xx|#*#|*#xx#|#*99|##|010xxxxxxxx|02xxxxxxxxx|0[3-9]xxxxxxxxx|0311xxxxxxxx|037[179]xxxxxxxx|04[15]1xxxxxxxx|043[12]xxxxxxxx|051[0-9]xxxxxxxx|052[37]xxxxxxxx|053[12]xxxxxxxx|057[1345679]xxxxxxxx|059[15]xxxxxxxx|0731xxxxxxxx|075[457]xxxxxxxx|076[09]xxxxxxxx|0898xxxxxxxx|00xxx.|[2-8][1-9]xxxxxx|1[3458]xxxxxxxxx|01[3458]xxxxxxxxx|11[02479]|12[0268]|11[13568]x.|125xx|12[13479]x.|100[015678]x|100[2349]x.|10[1-9]xx.|14xx.|1[79]xx.|160|168xxxxx|16[1-79]x.|[48]00xxxxxxx|[48]0[1-9]x.|[23567]0xx.|1630x|955xx|9699[*#]|95[0-46-9]xxx.|9[0-46-9]xxxx.|x.';
        var AllProfile = new Array(new stProfile("InternetGatewayDevice.Services.VoiceService.1.VoiceProfile.1", "", "Max", ""), null);
        var Profile = new Array();
        for (var i = 0; i < AllProfile.length - 1; i++) {
            AllProfile[i].DigitMap = SipDigitMap;
            Profile[i] = AllProfile[i];
        }

        function stSIPServer(Domain, ProxyServer, ProxyServerPort, OutboundProxy, OutboundProxyPort, X_HW_SecondaryOutboundProxy, X_HW_SecondaryOutboundProxyPort, X_HW_SecondaryProxyServer, X_HW_SecondaryProxyServerPort, RegistrarServer, UserAgentPort, RegistrationPeriod) {
            this.Domain = Domain;
            this.OutboundProxy = OutboundProxy;
            this.OutboundProxyPort = OutboundProxyPort;
            this.X_HW_SecondaryOutboundProxy = X_HW_SecondaryOutboundProxy;
            this.X_HW_SecondaryOutboundProxyPort = X_HW_SecondaryOutboundProxyPort;

            this.ProxyServer = ProxyServer;
            this.ProxyServerPort = ProxyServerPort;
            this.RegistrationPeriod = RegistrationPeriod;
            this.X_HW_SecondaryProxyServer = X_HW_SecondaryProxyServer;
            this.UserAgentPort = UserAgentPort;
            this.RegistrarServer = RegistrarServer;
            if ("0" == ProxyServerPort) {
                this.ProxyServerPort = "";
            } else {
                this.ProxyServerPort = ProxyServerPort;
            }

            if ("0" == X_HW_SecondaryProxyServerPort) {
                this.X_HW_SecondaryProxyServerPort = "";
            } else {
                this.X_HW_SecondaryProxyServerPort = X_HW_SecondaryProxyServerPort;
            }

            if ("0" == OutboundProxyPort) {
                this.OutboundProxyPort = "";
            } else {
                this.OutboundProxyPort = OutboundProxyPort;
            }

            if ("0" == X_HW_SecondaryOutboundProxyPort) {
                this.X_HW_SecondaryOutboundProxyPort = "";
            } else {
                this.X_HW_SecondaryOutboundProxyPort = X_HW_SecondaryOutboundProxyPort;
            }

            var temp = Domain.split('.');
            this.key = '.' + temp[5] + '.';
        }

        var AllSIPServer = new Array(new stSIPServer("InternetGatewayDevice.Services.VoiceService.1.VoiceProfile.1.SIP", "", "5060", "", "5060", "", "5060", "", "5060", "", "5060", "600"), null);
        var SIPServer = new Array();
        for (var i = 0; i < AllSIPServer.length - 1; i++) SIPServer[i] = AllSIPServer[i];

        AssociateParam('Profile', 'SIPServer', 'ProxyServer|ProxyServerPort|OutboundProxy|OutboundProxyPort|X_HW_SecondaryOutboundProxy|X_HW_SecondaryOutboundProxyPort|X_HW_SecondaryProxyServer|X_HW_SecondaryProxyServerPort|RegistrarServer|UserAgentPort|RegistrationPeriod');

        function stDftRoute(domain, autoenable, wandomain) {
            this.domain = domain;
            this.autoenable = autoenable;
            this.wandomain = wandomain;
        }

        function stWanInfo(domain, Enable, CntType, ConnectionStatus, NATEnabled, DefaultGateway, ServiceList, vlanid, WanID, submask) {
            this.domain = domain;
            this.Enable = Enable;
            this.CntType = CntType;
            this.ConnectionStatus = ConnectionStatus;
            this.NATEnabled = NATEnabled;
            this.DefaultGateway = DefaultGateway;
            this.ServiceList = ServiceList;
            this.vlanid = vlanid;
            this.WanID = WanID;
            this.submask = submask;
        }

        var WanIPInfo = new Array(new stWanInfo("InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1", "1", "IP_Routed", "Unconfigured", "0", "0.0.0.0", "TR069", "45", "1", "0.0.0.0"), new stWanInfo("InternetGatewayDevice.WANDevice.1.WANConnectionDevice.5.WANIPConnection.1", "1", "IP_Bridged", "Connected", "0", "", "OTHER", "45", "5", ""), new stWanInfo("InternetGatewayDevice.WANDevice.1.WANConnectionDevice.7.WANIPConnection.1", "1", "IP_Bridged", "Connected", "0", "", "INTERNET", "47", "7", ""), new stWanInfo("InternetGatewayDevice.WANDevice.1.WANConnectionDevice.8.WANIPConnection.1", "1", "IP_Bridged", "Connected", "0", "", "INTERNET", "47", "8", ""), new stWanInfo("InternetGatewayDevice.WANDevice.1.WANConnectionDevice.2.WANIPConnection.1", "0", "IP_Bridged", "Unconfigured", "0", "", "INTERNET", "0", "2", ""), null);
        var WanPPPInfo = new Array(new stWanInfo("InternetGatewayDevice.WANDevice.1.WANConnectionDevice.3.WANPPPConnection.1", "0", "IP_Routed", "Unconfigured", "1", "0.0.0.0", "INTERNET", "1", "3"), new stWanInfo("InternetGatewayDevice.WANDevice.1.WANConnectionDevice.4.WANPPPConnection.1", "0", "IP_Routed", "Unconfigured", "1", "0.0.0.0", "INTERNET", "55", "4"), null);
        var dftRoutes = new Array(new stDftRoute("InternetGatewayDevice.Layer3Forwarding", "0", ""), null);

        var dftRoute = dftRoutes[0];

        var WanInfo = new Array();

        for (i = 0, j = 0; WanIPInfo.length > 0 && i < WanIPInfo.length - 1; i++) {
            if ((true == WanIPInfo[i].Enable) && (("VOIP" == WanIPInfo[i].ServiceList) || ("TR069_VOIP" == WanIPInfo[i].ServiceList) || ("VOIP_INTERNET" == WanIPInfo[i].ServiceList) || ("TR069_VOIP_INTERNET" == WanIPInfo[i].ServiceList))) {
                WanInfo[j] = WanIPInfo[i];
                j++;
            }
        }

        for (i = 0; WanPPPInfo.length > 0 && i < WanPPPInfo.length - 1; i++) {
            if ((true == WanPPPInfo[i].Enable) && (("VOIP" == WanPPPInfo[i].ServiceList) || ("TR069_VOIP" == WanPPPInfo[i].ServiceList) || ("VOIP_INTERNET" == WanPPPInfo[i].ServiceList) || ("TR069_VOIP_INTERNET" == WanPPPInfo[i].ServiceList))) {
                WanInfo[j] = WanPPPInfo[i];
                j++;
            }
        }

        function MakeWanName(wan) {
            var wanInst = 0;
            var wanServiceList = '';
            var wanMode = '';
            var vlanId = 0;
            var WanID = 0;
            var currentWanName = '';

            if ('&nbsp;' != wan) {
                DomainElement = wan.domain.split(".");
                wanInst = DomainElement[4];
                wanServiceList = wan.ServiceList;
                wanMode = (wan.CntType == 'IP_Bridged') ? "B": "R";
                vlanId = wan.vlanid;
                WanID = wan.WanID;
                if (0 != parseInt(vlanId)) {
                    currentWanName = WanID + "_" + wanServiceList + "_" + wanMode + "_VID_" + vlanId;
                } else {
                    currentWanName = WanID + "_" + wanServiceList + "_" + wanMode + "_VID_";
                }

                return currentWanName;
            } else {
                return '&nbsp;';
            }
        }

        function MakeVoipWanName(wan) {
            var wandevInst = 0;
            var wanConInst = 0;
            var wanSsInst = 0;

            var wanServiceList = '';
            var currentWanName = '';

            DomainElement = wan.domain.split(".");
            wandevInst = DomainElement[2];
            wanConInst = DomainElement[4];
            wanSsInst = DomainElement[6];
            wanServiceList = wan.ServiceList;
            if (2 == parseInt(wanSsInst)) {
                currentWanName = "wan" + wandevInst + "." + wanConInst + "." + wanSsInst;
            } else {
                currentWanName = "wan" + wanConInst;
            }

            return currentWanName;
        }

        /*-----------------------handle event-------------------------*/

        /*-----------------------adapte funcion--------------------------*/
        //loadpage
        var dmm = "x表示0-9的数字，[0-9]表示0到9区间的数字，S表示短定时器，L表示长定时器，'.'表示重复前面的0到多次";
        function init() {
            if (Profile[0].Relating.ProxyServerPort == "0") {
                Profile[0].Relating.ProxyServerPort = "";
            }

            if (Profile[0].Relating.X_HW_SecondaryProxyServerPort == "0") {
                Profile[0].Relating.X_HW_SecondaryProxyServerPort = "";
            }

            if (Profile[0].Relating.OutboundProxyPort == "0") {
                Profile[0].Relating.OutboundProxyPort = "";
            }

            if (Profile[0].Relating.X_HW_SecondaryOutboundProxyPort == "0") {
                Profile[0].Relating.X_HW_SecondaryOutboundProxyPort = "";
            }

            if (Profile[0].Relating.X_HW_SecondaryProxyServer == "255.255.255.255") {
                Profile[0].Relating.X_HW_SecondaryProxyServer = "";
            }

            if (Profile[0].Relating.ProxyServer == "255.255.255.255") {
                Profile[0].Relating.ProxyServer = "";
            }
            setText('OutboundProxy', Profile[0].Relating.OutboundProxy);
            setText('OutboundProxyPort', Profile[0].Relating.OutboundProxyPort);
            setText('SecondaryOutboundProxy', Profile[0].Relating.X_HW_SecondaryOutboundProxy);
            setText('SecondaryOutboundProxyPort', Profile[0].Relating.X_HW_SecondaryOutboundProxyPort);
            setText('ProxyServer', Profile[0].Relating.ProxyServer);
            setText('ProxyServerPort', Profile[0].Relating.ProxyServerPort);
            setText('SecondProxyServer', Profile[0].Relating.X_HW_SecondaryProxyServer);
            setText('SecondProxyServerPort', Profile[0].Relating.X_HW_SecondaryProxyServerPort);
            setText('HomeDomain', Profile[0].Relating.RegistrarServer);
            setText('UserAgentPort', Profile[0].Relating.UserAgentPort);
            setText('DigitMap', Profile[0].DigitMap);
            setSelect('Region', Profile[0].Region);
            setSelect('X_HW_DigitMapMatchMode', Profile[0].X_HW_DigitMapMatchMode);
            setSelect('X_HW_PortName', Profile[0].X_HW_PortName);
            setText('RegistrationPeriod', Profile[0].Relating.RegistrationPeriod);
            setSelect('X_HW_PortName_RTP', MediaPortName[0].X_HW_PortName_RTP);
            document.getElementById('digitmap1').title = dmm;

        }

        function LoadFrame() {
            if (Line.length > 0) {
                selectLine('record_0');
                setDisplay('ConfigForm1', 1);
            } else {
                selectLine('record_no');
                setDisplay('ConfigForm1', 0);
            }
            init();

            if (CfgMode.toUpperCase() == "SHCT") {
                setDisplay('ConfigForm2', 0);
            } else {
                setDisplay('ConfigForm2', 1);
            }
        }

        function SubmitInterface() {
            var Form = new webSubmitForm();
            var sndProServerPort;
            var ProServerPort;
            var sndOutboundServerPort;
            var OutboundServerPort;

            var Region = document.getElementById('Region');
            var DigitMap = document.getElementById('DigitMap');
            var X_HW_DigitMapMatchMode = document.getElementById('X_HW_DigitMapMatchMode');
            var X_HW_PortName = document.getElementById('X_HW_PortName');
            var ProxyServer = document.getElementById('ProxyServer');
            var sndProServerPort = document.getElementById('sndProServerPort');
            var UserAgentPort = document.getElementById('UserAgentPort');
            var SecondProxyServer = document.getElementById('SecondProxyServer');
            var HomeDomain = document.getElementById('HomeDomain');

            var ulret = CheckForm1();
            if (ulret != true) {
                return false;
            }
            if (removeSpaceTrim(getValue('SecondProxyServerPort').toString()) == "") {
                sndProServerPort = 0;
            } else {
                sndProServerPort = parseInt(getValue('SecondProxyServerPort'), 10);
            }

            if (removeSpaceTrim(getValue('ProxyServerPort').toString()) == "") {
                ProServerPort = 0;
            } else {
                ProServerPort = parseInt(getValue('ProxyServerPort'), 10);
            }

            if (removeSpaceTrim(getValue('OutboundProxyPort').toString()) == "") {
                OutboundServerPort = 0;
            } else {
                OutboundServerPort = parseInt(getValue('OutboundProxyPort'), 10);
            }
            if (removeSpaceTrim(getValue('SecondaryOutboundProxyPort').toString()) == "") {
                sndOutboundServerPort = 0;
            } else {
                sndOutboundServerPort = parseInt(getValue('SecondaryOutboundProxyPort'), 10);
            }

            Form.addParameter('y.OutboundProxy', getValue('OutboundProxy'));
            Form.addParameter('y.OutboundProxyPort', parseInt(OutboundServerPort));
            Form.addParameter('y.X_HW_SecondaryOutboundProxy', getValue('SecondaryOutboundProxy'));
            Form.addParameter('y.X_HW_SecondaryOutboundProxyPort', parseInt(sndOutboundServerPort));
            Form.addParameter('x.Region', getValue('Region'));
            Form.addParameter('z.DigitMap', getValue('DigitMap'));
            Form.addParameter('x.X_HW_DigitMapMatchMode', getValue('X_HW_DigitMapMatchMode'));
            Form.addParameter('x.X_HW_PortName', getValue('X_HW_PortName'));
            Form.addParameter('y.ProxyServer', getValue('ProxyServer'));
            Form.addParameter('y.ProxyServerPort', parseInt(ProServerPort));
            Form.addParameter('y.RegistrationPeriod', parseInt(getValue('RegistrationPeriod'), 10));
            Form.addParameter('y.X_HW_SecondaryProxyServerPort', parseInt(sndProServerPort));
            Form.addParameter('y.UserAgentPort', parseInt(getValue('UserAgentPort'), 10));
            Form.addParameter('y.X_HW_SecondaryProxyServer', getValue('SecondProxyServer'));
            Form.addParameter('y.RegistrarServer', getValue('HomeDomain'));
            Form.addParameter('a.X_HW_PortName', getValue('X_HW_PortName_RTP'));

            Form.setAction('app-VoIP.asp?x=' + Profile[0].Domain + '&y=' + Profile[0].Domain + '.SIP' + '&z=' + Profile[0].Domain + '.SIP.X_HW_SIPDigitmap.1' + '&a=' + Profile[0].Domain + '.RTP' + '&RequestFile=html/voip/voipinterface.asp');

            setDisable('btnApplySipServer', 1);
            setDisable('cancelValue', 1);

            Form.submit();
        }

        var g_Index = -1;

        function isIpAddress(address) {
            var i = 0;

            var addrParts = address.split('.');
            if (addrParts.length != 4) {
                return false;
            }

            for (i = 0; i < 4; i++) {
                if (isNaN(addrParts[i]) || addrParts[i] == "" || addrParts[i].charAt(0) == '+' || addrParts[i].charAt(0) == '-') {
                    return false;
                }
                if (!isInteger(addrParts[i]) || addrParts[i] < 0) {
                    return false;
                }
            }
            return true;
        }

        function getIpAddress(address) {
            var i = 0;
            var addrParts = address.split('.');
            var IpAddress = parseInt(addrParts[0], 10) + '.' + parseInt(addrParts[1], 10) + '.' + parseInt(addrParts[2], 10) + '.' + parseInt(addrParts[3], 10);
            return IpAddress
        }

        function isvalidVoipIpAddress(address) {
            var i = 0;
            if (address == '255.255.255.255') {
                return false;
            }

            if (address == '0.0.0.0') {
                return true;
            }

            var addrParts = address.split('.');
            if (addrParts.length != 4) return false;
            for (i = 0; i < 4; i++) {
                if (isNaN(addrParts[i]) || addrParts[i] == "" || addrParts[i].charAt(0) == '+' || addrParts[i].charAt(0) == '-') return false;

                if (addrParts[i].length > 3 || addrParts[i].length < 1) {
                    return false;
                }

                if (addrParts[i].length > 1 && addrParts[i].charAt(0) == '0') {
                    return false;
                }
                if (!isInteger(addrParts[i]) || addrParts[i] < 0) {
                    return false;
                }
                num = parseInt(addrParts[i]);
                /* END:   Added by c00147909, 2009/2/6 */
                if (num < 0 || num > 255) return false;
            }
            return true;
        }

        //Checkout
        function CheckForm1() {
            if ('' != removeSpaceTrim(getValue('OutboundProxy'))) {
                if (true == isIpAddress(getValue('OutboundProxy'))) {
                    if (false == isvalidVoipIpAddress(getValue('OutboundProxy'))) {
                        alert('Outbound服务器地址不正确.');
                        return false;
                    }
                } else {
                    if (false == isValidCfgStr('The outbound proxy server address', getValue('OutboundProxy'), 256)) {
                        return false;
                    }
                    var OutboundProxy = getValue('OutboundProxy');

                    if ((OutboundProxy.substring(0, 1) <= 9) && (OutboundProxy.substring(0, 1) >= 0)) {
                        alert('Outbound服务器无效.');
                        return false;
                    }
                }
            }
            if ('' != removeSpaceTrim(getValue('OutboundProxyPort'))) {
                if (isValidPort(getValue('OutboundProxyPort')) == false) {
                    alert('Outbound服务器端口号 "' + getValue('OutboundProxyPort') + '" 无效.');
                    return false;
                }
            }
            if ('' != removeSpaceTrim(getValue('SecondaryOutboundProxy'))) {
                if (true == isIpAddress(getValue('SecondaryOutboundProxy'))) {
                    if (false == isvalidVoipIpAddress(getValue('SecondaryOutboundProxy'))) {
                        alert('备用Outbound服务器地址无效.');
                        return false;
                    }
                } else {
                    if (false == isValidCfgStr('The secondary proxy server address', getValue('SecondaryOutboundProxy'), 256)) {
                        return false;
                    }
                    var SecondaryOutboundProxy = getValue('SecondaryOutboundProxy');

                    if ((SecondaryOutboundProxy.substring(0, 1) <= 9) && (SecondaryOutboundProxy.substring(0, 1) >= 0)) {
                        alert('备用Outbound服务器无效.');
                        return false;
                    }
                }
            }
            if ('' != removeSpaceTrim(getValue('SecondaryOutboundProxyPort'))) {
                if (isValidPort(getValue('SecondaryOutboundProxyPort')) == false) {
                    alert('备用Outbound服务器端口号 "' + getValue('SecondaryOutboundProxyPort') + '" 无效.');
                    return false;
                }
            }

            if ('' != removeSpaceTrim(getValue('ProxyServer'))) {
                if (true == isIpAddress(getValue('ProxyServer'))) {
                    if (false == isvalidVoipIpAddress(getValue('ProxyServer'))) {
                        alert('主用服务器地址无效.');
                        return false;
                    }
                } else {
                    if (false == isValidCfgStr('The primary proxy server address', getValue('ProxyServer'), 256)) {
                        return false;
                    }
                    var proxyserv = getValue('ProxyServer');

                    if ((proxyserv.substring(0, 1) <= 9) && (proxyserv.substring(0, 1) >= 0)) {
                        alert('主用服务器无效。');
                        return false;
                    }

                }
            }

            if ('' != removeSpaceTrim(getValue('ProxyServerPort'))) {
                if (isValidPort(getValue('ProxyServerPort')) == false) {
                    alert('主用服务器端口号“' + getValue('ProxyServerPort') + '”无效。');
                    return false;
                }
            }

            if ('' != removeSpaceTrim(getValue('SecondProxyServer'))) {
                if (true == isIpAddress(getValue('SecondProxyServer'))) {
                    if (false == isvalidVoipIpAddress(getValue('SecondProxyServer'))) {
                        alert('备用服务器地址无效。');
                        return false;
                    }
                } else {
                    if (false == isValidCfgStr('The secondary proxy server address', getValue('SecondProxyServer'), 256)) {
                        return false;
                    }
                    var proxysndserv = getValue('SecondProxyServer');

                    if ((proxysndserv.substring(0, 1) <= 9) && (proxysndserv.substring(0, 1) >= 0)) {
                        alert('备用服务器地址无效。');
                        return false;
                    }
                }
            }

            if ('' != removeSpaceTrim(getValue('SecondProxyServerPort'))) {
                if (isValidPort(getValue('SecondProxyServerPort')) == false) {
                    alert('备用服务器地址端口“' + getValue('SecondProxyServerPort') + '”无效。');
                    return false;
                }
            }

            if ('' != removeSpaceTrim(getValue('HomeDomain'))) {
                if (true == isIpAddress(getValue('HomeDomain'))) {
                    if (false == isvalidVoipIpAddress(getValue('HomeDomain'))) {
                        alert('归属域名无效。');
                        return false;
                    }
                } else {
                    if (isValidCfgStr('The SIP home domain', getValue('HomeDomain'), 256) == false) {
                        alert('归属域名无效。');
                        return false;
                    }
                }
            }

            if ('' == getValue('UserAgentPort')) {
                alert('SIP本地端口号不能为空。');
                return false;
            } else {
                if (isValidPort(getValue('UserAgentPort')) == false) {
                    alert('SIP本地端口号“' + getValue('UserAgentPort') + '”无效。');
                    return false;
                }
            }

            if (getValue('DigitMap').length > 8000) {
                alert('数图长度不能超过8000个字符。');
                return false;
            }

            if (isSafeStringIn(getValue('DigitMap'), 'ABCDEFXabcdefxT0123456789-[]()*#.T|LSZ') == false) {
                alert('数图不合法。合法范围是“ABCDEFXabcdefxT0123456789-[]()*#.T|LSZ”。');
                return false;
            }

            if (false == isInteger(getValue('RegistrationPeriod'))) {
                alert('注册周期非法，它必须是一个1到65534之间的数值。');
                return false;
            }

            if ((getValue('RegistrationPeriod') > 65534) || (getValue('RegistrationPeriod') < 1)) {
                alert('注册周期非法，它必须是一个1到65534之间的数值。');
                return false;
            }

            return true;
        }

        function CancelConfig() {
            init();
        }

        var selctIndex = -1;

        function stPhyInterface(Domain, InterfaceID) {
            this.Domain = Domain;
        }

        var AllPhyInterface = new Array(new stPhyInterface("InternetGatewayDevice.Services.VoiceService.1.PhyInterface.1", "1"), new stPhyInterface("InternetGatewayDevice.Services.VoiceService.1.PhyInterface.2", "2"), null);

        function stLine(Domain, DirectoryNumber, Enable, PhyReferenceList) {
            this.Domain = Domain;
            this.DirectoryNumber = DirectoryNumber;
            this.PhyReferenceList = PhyReferenceList;

            if (Enable.toLowerCase() == 'enabled') {
                this.Enable = 1;
            } else {
                this.Enable = 0;
            }
            this.AuthUserName = '';
            this.AuthPassword = '';
            this.URI = '';
            var temp = Domain.split('.');
            this.key = '.' + temp[7] + '.';
        }

        var AllLine = new Array(new stLine("InternetGatewayDevice.Services.VoiceService.1.VoiceProfile.1.Line.1", "", "Disabled", "1"), new stLine("InternetGatewayDevice.Services.VoiceService.1.VoiceProfile.1.Line.2", "", "Disabled", "2"), null);
        var Line = new Array();
        for (var i = 0; i < AllLine.length - 1; i++) Line[i] = AllLine[i];

        function stAuth(Domain, AuthUserName, AuthPassword, URI) {
            this.Domain = Domain;
            this.AuthUserName = AuthUserName;
            this.AuthPassword = AuthPassword;
            this.URI = URI;
            var temp = Domain.split('.');
            this.key = '.' + temp[7] + '.';
        }

        var AllAuth = new Array(new stAuth("InternetGatewayDevice.Services.VoiceService.1.VoiceProfile.1.Line.1.SIP", "", "", ""), new stAuth("InternetGatewayDevice.Services.VoiceService.1.VoiceProfile.1.Line.2.SIP", "", "", ""), null);
        var Auth = new Array();
        for (var i = 0; i < AllAuth.length - 1; i++) Auth[i] = AllAuth[i];

        AssociateParam('Line', 'Auth', 'AuthUserName|AuthPassword|URI');

        /*-----------------------handle event-------------------------*/

        /*-----------------------adapte funcion--------------------------*/
        //loadpage
        var recordDirectoryNumber;

        function SelectLineRecord(recordId) {
            selectLine(recordId);
            recordDirectoryNumber = getElement("DirectoryNumber").value;
        }

        function AddSubmitParam(Form, type) {
            Form.addParameter('x.DirectoryNumber', getValue('DirectoryNumber'));
            Form.addParameter('x.PhyReferenceList', getValue('PhyReferenceList'));

            Form.addParameter('y.AuthUserName', getValue('AuthUserName'));
            Form.addParameter('y.AuthPassword', getValue('AuthPassword'));
            Form.addParameter('y.URI', getValue('URI'));

            if (getCheckVal('Enable') == 1) {
                Form.addParameter('x.Enable', 'Enabled');
            } else {
                Form.addParameter('x.Enable', 'Disabled');
            }

            if (selctIndex == -1) {
                Form.setAction('addvoipinfo.cgi?x=InternetGatewayDevice.Services.VoiceService.1.VoiceProfile.1.Line' + '&y=SIP' + '&RequestFile=html/voip/voipinterface.asp');
            } else {
                Form.setAction('app-VoIP.asp?x=' + Line[selctIndex].Domain + '&y=' + Line[selctIndex].Domain + '.SIP' + '&RequestFile=html/voip/voipinterface.asp');
            }
            setDisable('btnApplySipUser', 1);
            setDisable('cancelValue', 1);
        }

        function setCtlDisplay(record) {
            setText('URI', record.URI);
            setText('DirectoryNumber', record.DirectoryNumber);
            setText('AuthUserName', record.AuthUserName);
            setText('AuthPassword', record.AuthPassword);
            setSelect('PhyReferenceList', record.PhyReferenceList);
            setCheck('Enable', record.Enable);
        }

        var g_Index = -1;

        function setControl(index) {
            var record;
            selctIndex = index;

            if (index == -1) {
                if (Line.length >= ((AllPhyInterface.length - 1) * 17)) {
                    setDisplay('ConfigForm1', 0);
                    alert('超过最大用户数。');
                    return false;
                }

                record = new stLine("", "", "Disabled", "");
                setDisplay('ConfigForm1', 1);
                setCtlDisplay(record);
            } else if (index == -2) {
                setDisplay('ConfigForm1', 0);
            } else {
                record = Line[index];
                setDisplay('ConfigForm1', 1);
                setCtlDisplay(record);
            }
            g_Index = index;
        }

        function clickRemove() {
            if (Line.length == 0) {
                alert("无用户。");
                return;
            }

            if (selctIndex == -1) {
                alert("不能删除此用户，请先保存新用户。");
                return;
            }
            var rml = getElement('rml');
            var noChooseFlag = true;
            if (rml.length > 0) {
                for (var i = 0; i < rml.length; i++) {
                    if (rml[i].checked == true) {
                        noChooseFlag = false;
                    }
                }
            } else if (rml.checked == true) //for one connection
            {
                noChooseFlag = false;
            }
            if (noChooseFlag) {
                alert('请选择用户。');
                return;
            }

            if (confirm("确定要删除用户?") == false) {
                return;
            }
            setDisable('btnApplySipUser', 1);
            setDisable('cancelValue', 1);
            removeInst('html/voip/voipinterface.asp');
        }

        //Checkout
        function CheckForm(type) {
            if ('' != removeSpaceTrim(getValue('DirectoryNumber'))) {
                if (isValidCfgStr('The SIP public user name', getValue('DirectoryNumber'), 64) == false) {
                    return false;
                }
            }

            if ('' != removeSpaceTrim(getValue('URI'))) {
                if (isValidCfgStr('The SIP URI', getValue('URI'), 389) == false) {
                    return false;
                }
            }

            if ('' != removeSpaceTrim(getValue('AuthUserName'))) {
                if (isValidCfgStr('The SIP auth user name', getValue('AuthUserName'), 64) == false) {
                    return false;
                }

                if (isValidCfgStr('The SIP auth password', getValue('AuthPassword'), 64) == false) {
                    return false;
                }
            }

            for (var i = 0; i < Line.length; i++) {
                if (selctIndex != i) {
                    if ((getValue('URI') != "") && (getValue('URI') == Line[i].URI)) {
                        alert('URI已经存在。');
                        return false;
                    }

                    if ((getValue('DirectoryNumber') != "") && (getValue('DirectoryNumber') == Line[i].DirectoryNumber)) {
                        alert('注册用户名已经存在。');
                        return false;
                    }
                    if ((getValue('DirectoryNumber') == "") && (getValue('AuthUserName') != "") && (getValue('AuthUserName') == Line[i].AuthUserName)) {
                        alert('鉴权用户名已经存在。');
                        return false;
                    }
                }
            }

            for (var i = 0; i < Line.length; i++) {
                if (selctIndex != i) {
                    if ((getValue('PhyReferenceList') != "") && (getValue('PhyReferenceList') == Line[i].PhyReferenceList)) {
                        if ((getCheckVal('Enable') == 1) && (Line[i].Enable == 1)) {
                            var ulret = confirm('端口' + getValue('PhyReferenceList') + '已存在用户绑定,确定要同时绑定到该端口?');
                            return ulret;
                        }

                        return true;
                    }
                }
            }

            return true;
        }

        function CancelUserConfig() {
            if (selctIndex == -1) {
                var tableRow = getElement("voipUserTable");

                if (tableRow.rows.length == 1) {
                    selectLine('record_no');
                } else if (tableRow.rows.length == 2) {
                    addNullInst('VOIP User');
                } else {
                    tableRow.deleteRow(tableRow.rows.length - 1);
                    selectLine('record_0');
                }
            } else {
                var record = Line[selctIndex];
                setCtlDisplay(record);
            }
        }
    </SCRIPT>
    </HEAD>
    
    <BODY style="TEXT-ALIGN: center" vLink=#000000 aLink=#000000 link=#000000
    leftMargin=0 topMargin=0 onload="DisplayLocation(getElement('Selected_Menu').value);FinishLoad(); LoadFrame();"
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
                                    <TD>
                                        &nbsp;
                                    </TD>
                                    <TD vAlign=bottom align=right width=358>
                                        <TABLE id=table8 cellSpacing=0 cellPadding=0 border=0>
                                            <TBODY>
                                                <TR>
                                                    <TD vAlign=bottom align=right>
                                                        <SPAN class=curUserName>
                                                            &nbsp;
                                                        </SPAN>
                                                    </TD>
                                                    <TD class=welcom vAlign=bottom align=middle width=120>
                                                        欢迎您！
                                                    </TD>
                                                    <TD vAlign=bottom width=50>
                                                        <A onclick=DoLogout() href="/cgi-bin/logout.cgi" target=_top>
                                                            <SPAN class=logout>
                                                                退出
                                                            </SPAN>
                                                        </A>
                                                    </TD>
                                                </TR>
                                            </TBODY>
                                        </TABLE>
                                    </TD>
                                </TR>
                            </TBODY>
                        </TABLE>
                        <TABLE id=table2 height=100 cellSpacing=0 cellPadding=0 width=808 border=0>
                            <TBODY>
                                <TR>
                                    <TD class=LocationDisplay id=LocationDisplay align=middle width=163 bgColor=#ef8218
                                    rowSpan=3>
                                    </TD>
                                    <TD width=434 bgColor=#427594 height=33>
                                        <P align=right>
                                            <FONT face=黑体 color=#ffffff>
                                                <B>
                                                    <FONT face=黑体 color=#ffffff size=6>
                                                        <INPUT id=Selected_Menu type=hidden value="应用->宽带电话设置" name=Selected_Menu>
                                                    </FONT>
                                                </B>
                                                <SPAN class=GatewayName>
                                                    网关名称:
                                                    <SCRIPT language=javascript>
                                                        document.write(top.gateWayName);
                                                    </SCRIPT>
                                                </SPAN>
                                            </FONT>
                                        </P>
                                    </TD>
                                    <TD width=211 bgColor=#ef8218 height=33>
                                        <P class=GatewayType align=center>
                                            型号:
                                            <SCRIPT language=javascript>
                                                document.write(top.ModelName);
                                            </SCRIPT>
                                        </P>
                                    </TD>
                                </TR>
                                <TR>
                                    <TD id=MenuArea_L1 vAlign=bottom bgColor=#ef8218 colSpan=2 height=43>
                                        &nbsp;
                                    </TD>
                                </TR>
                                <TR>
                                    <TD id=MenuArea_L2 bgColor=#427594 colSpan=2 height=24>
                                    </TD>
                                </TR>
                            </TBODY>
                        </TABLE>
                        <SCRIPT language=javascript>
                            MakeMenu(getElById('Selected_Menu').value);
                        </SCRIPT>
                        <TABLE id=table3 height=15 cellSpacing=0 cellPadding=0 width=808 border=0>
                            <TBODY>
                                <TR>
                                    <TD height=15>
                                        <IMG height=15 src="/img/panel1.gif" width=164 border=0>
                                    </TD>
                                    <TD>
                                        <IMG height=15 src="/img/panel2.gif" width=645 border=0>
                                    </TD>
                                </TR>
                            </TBODY>
                        </TABLE>
                    </TD>
                </TR>
                <TR>
                    <TD vAlign=top>
                        <TABLE height="100%" cellSpacing=0 cellPadding=0 border=0>
                            <TBODY>
                                <TR>
                                    <TD class=Item_L1 width=157 bgColor=#ef8218 height=30>
                                        <P>
                                            <a href='/cgi-bin/app-VoIP.asp' class=Item_L1 style="text-decoration: none;">宽带电话基本设置</a>
                                        </P>
                                    </TD>
                                    <TD width=7 bgColor=#ef8218>
                                        　
                                    </TD>
                                    <TD width=564>
                                        　
                                    </TD>
                                    <TD vAlign=top class=table1_help background=/img/panel4.gif rowSpan=8>
                                        <TABLE cellSpacing=0 cellPadding=20 width="100%" border=0 height='100%'>
                                            <TBODY>
                                                <TR>
                                                    <TD valign='top'>
                                                        <A href="/cgi-bin/help_content.asp#宽带电话设置" target=_blank>
                                                            <IMG height=34 src="/img/help.gif" width=40 border=0>
                                                        </A>
                                                    </TD>
                                                </TR>
<% if tcwebApi_get("WebCustom_Entry","isWebTYLOGOSupported","h") = "Yes" then %>
                                                <TR>
                                                    <TD valign='bottom'>
                                                        <IMG src='/img/tybottom.jpg' height=76 width=112>
                                                    </TD>
                                                </TR>
<% end if %>
                                            </TBODY>
                                        </TABLE>
                                    </TD>
                                </TR>
                                <TR>
                                    <TD class=Item_L2 vAlign=top width=157 bgColor=#e7e7e7 height=30>
                                        <P style='font-weight:normal; padding-top: 10px;'>
                                        	<a href='/cgi-bin/app-VoIPUser.asp' style="text-decoration: none;">宽带电话高级设置</a>
                                        </P>
                                    </TD>
                                    <TD width=7 background=/img/panel3.gif></TD>
                                    <TD>
                                        <TABLE cellSpacing=0 cellPadding=0 width="100%" border=0>
                                            <TBODY>
                                                <TR>
                                                    <TD width=10>
                                                        &nbsp;
                                                    </TD>
                                                    <TD>
                                                    <!-- start -->

																										<TABLE border=0 cellSpacing=0 cellPadding=0 width="100%">
																						            <TBODY>
																						                <TR>
																						                    <TD class=table1_head>
																						                        接口基本参数
																						                    </TD>
																						                </TR>
																						            </TBODY>
																						        </TABLE>
																						        <TABLE border=0 cellSpacing=0 cellPadding=0 width="100%">
																						            <TBODY>
																						                <TR>
																						                    <TD class=prompt>
																						                        <TABLE border=0 cellSpacing=0 cellPadding=0 width="100%">
																						                            <TBODY>
																						                                <TR>
																						                                    <TD style="PADDING-LEFT: 10px" width="100%"></TD>
																						                                </TR>
																						                            </TBODY>
																						                        </TABLE>
																						                    </TD>
																						                </TR>
																						            </TBODY>
																						        </TABLE>
																						        <TABLE border=0 cellSpacing=0 cellPadding=0 width="100%">
																						            <TBODY>
																						                <TR>
																						                    <TD height=5></TD>
																						                </TR>
																						            </TBODY>
																						        </TABLE>
																						        <TABLE class=table1_bg border=0 cellSpacing=1 cellPadding=0 width="100%">
																						            <TBODY>
																						                <TR>
																						                    <TD class=table1_left width="35%" align=left>
																						                        Outbound服务器地址:
																						                    </TD>
																						                    <TD class=table1_right width="65%" colSpan=2 align=left>
																						                        <INPUT style="WIDTH: 155px" id=OutboundProxy maxLength=256
																						                        type=text name=OutboundProxy>
																						                        <SPAN class=gray>
																						                            (IP或域名)
																						                        </SPAN>
																						                    </TD>
																						                </TR>
																						                <TR>
																						                    <TD class=table1_left align=left>
																						                        Outbound服务器端口号:
																						                    </TD>
																						                    <TD class=table1_right colSpan=2 align=left>
																						                        <INPUT style="WIDTH: 155px" id=OutboundProxyPort maxLength=256 type=text
																						                        name=OutboundProxyPort>
																						                        <SPAN class=gray>
																						                            (1-65535)
																						                        </SPAN>
																						                    </TD>
																						                </TR>
																						                <TR>
																						                    <TD class=table1_left align=left>
																						                        备用Outbound服务器地址:
																						                    </TD>
																						                    <TD class=table1_right colSpan=2 align=left>
																						                        <INPUT style="WIDTH: 155px" id=SecondaryOutboundProxy
																						                        maxLength=256 type=text name=SecondaryOutboundProxy>
																						                        <SPAN class=gray>
																						                            (IP或域名)
																						                        </SPAN>
																						                    </TD>
																						                </TR>
																						                <TR>
																						                    <TD class=table1_left align=left>
																						                        备用Outbound服务器端口号:
																						                    </TD>
																						                    <TD class=table1_right colSpan=2 align=left>
																						                        <INPUT style="WIDTH: 155px" id=SecondaryOutboundProxyPort
																						                        maxLength=256 type=text name=SecondaryOutboundProxyPort>
																						                        <SPAN class=gray>
																						                            (1-65535)
																								                        </SPAN>
																								                    </TD>
																								                </TR>
																								                <TR>
																								                    <TD class=table1_left align=left>
																								                        主用服务器地址:
																								                    </TD>
																								                    <TD class=table1_right colSpan=2 align=left>
																								                        <INPUT style="WIDTH: 155px" id=ProxyServer maxLength=256 type=text name=ProxyServer>
																								                        <SPAN class=gray>
																								                            (IP或域名)
																								                        </SPAN>
																								                    </TD>
																								                </TR>
																								                <TR>
																								                    <TD class=table1_left align=left>
																								                        主用端口号:
																								                    </TD>
																								                    <TD class=table1_right colSpan=2 align=left>
																								                        <INPUT style="WIDTH: 155px" id=ProxyServerPort value=5060 maxLength=11
																								                        type=text name=ProxyServerPort>
																								                        <SPAN class=gray>
																								                            (1-65535)
																								                        </SPAN>
																								                    </TD>
																								                </TR>
																								            </TBODY>
																								        </TABLE>
																								        <TABLE class=table1_bg border=0 cellSpacing=1 cellPadding=0 width="100%">
																								            <TBODY>
																								                <TR>
																								                    <TD class=table1_left width="35%" align=left>
																								                        备用服务器地址:
																								                    </TD>
																								                    <TD class=table1_right width="65%" colSpan=2 align=left>
																								                        <INPUT style="WIDTH: 155px" id=SecondProxyServer
																								                        maxLength=256 type=text name=SecondProxyServer>
																								                        <SPAN class=gray>
																								                            (IP或域名)
																								                        </SPAN>
																								                    </TD>
																								                </TR>
																								                <TR>
																								                    <TD class=table1_left align=left>
																								                        备用端口号:
																								                    </TD>
																								                    <TD class=table1_right colSpan=2 align=left>
																								                        <INPUT style="WIDTH: 155px" id=SecondProxyServerPort value=5060 maxLength=11
																								                        type=text name=SecondProxyServerPort>
																								                        <SPAN class=gray>
																								                            (1-65535)
																								                        </SPAN>
																								                    </TD>
																								                </TR>
																								            </TBODY>
																								        </TABLE>
																								        <DIV id=ConfigForm2>
																								            <TABLE class=table1_bg border=0 cellSpacing=1 cellPadding=0 width="100%">
																								                <TBODY>
																								                    <TR>
																								                        <TD class=table1_left width="35%" align=left>
																								                            归属域名:
																								                        </TD>
																								                        <TD class=table1_right width="65%" colSpan=2 align=left>
																								                            <INPUT style="WIDTH: 155px" id=HomeDomain maxLength=256 type=text name=HomeDomain>
																								                            <SPAN class=gray>
																								                            (IP或域名)
																								                            </SPAN>
																								                        </TD>
																								                    </TR>
																								                </TBODY>
																								            </TABLE>
																								        </DIV>
																								        <TABLE class=table1_bg border=0 cellSpacing=1 cellPadding=0 width="100%">
																								            <TBODY>
																								                <TR>
																								                    <TD class=table1_left height=21 width="35%" align=left>
																								                        本地端口:
																								                    </TD>
																								                    <TD class=table1_right width="65%" colSpan=2 align=left>
																								                        <INPUT style="WIDTH: 155px" id=UserAgentPort value=5060 maxLength=11 type=text
																								                        name=UserAgentPort>
																								                        <STRONG style="COLOR: #ff0033">
																								                            *
																								                        </STRONG>
																								                        <SPAN class=gray>
																								                            (1-65535)
																								                        </SPAN>
																								                    </TD>
																								                </TR>
																								                <TR>
																								                    <TD class=table1_left align=left>
																								                        数图:
																								                    </TD>
																								                    <TD id=digitmap1 class=table1_right colSpan=2 align=left>
																								                        <TEXTAREA id=DigitMap rows=3 cols=65 style="WIDTH: 100%" name=DigitMap>
																								                        </TEXTAREA>
																								                    </TD>
																								                </TR>
																								                <TR>
																								                    <TD class=table1_left align=left>
																								                        数图匹配模式:
																								                    </TD>
																								                    <TD class=table1_right colSpan=2 align=left>
																								                        <SELECT style="WIDTH: 75px" id=X_HW_DigitMapMatchMode size=1 name=X_HW_DigitMapMatchMode>
																								                            <OPTION selected value=Min>
																								                                最小匹配
																								                            </OPTION>
																								                            <OPTION value=Max>
																								                                最大匹配
																								                            </OPTION>
																								                        </SELECT>
																								                    </TD>
																								                </TR>
																								                <TR>
																								                    <TD class=table1_left align=left>
																								                        注册周期:
																								                    </TD>
																								                    <TD class=table1_right colSpan=2 align=left>
																								                        <P>
																								                            <INPUT style="WIDTH: 155px" id=RegistrationPeriod value=600 maxLength=11
																								                            type=text name=RegistrationPeriod>
																								                            <SPAN class=gray>
																								                                (单位：秒)(1~65534)
																								                            </SPAN>
																								                        </P>
																								                    </TD>
																								                </TR>
																								                <TR>
																								                    <TD class=table1_left align=left>
																								                        国家及地区:
																								                    </TD>
																								                    <TD class=table1_right colSpan=2 align=left>
																								                        <SELECT id=Region name=Region>
																								                            <OPTION selected value="">
																								                            </OPTION>
																								                            <OPTION value=AE>
																								                                AE - 阿拉伯联合酋长国
																								                            </OPTION>
																								                            <OPTION value=AR>
																								                                AR - 阿根廷
																								                            </OPTION>
																								                            <OPTION value=BR>
																								                                BR - 巴西
																								                            </OPTION>
																								                            <OPTION value=CN>
																								                                CN - 中国
																								                            </OPTION>
																								                            <OPTION value=DE>
																								                                DE - 德国
																								                            </OPTION>
																								                            <OPTION value=ES>
																								                                ES - 西班牙
																								                            </OPTION>
																								                            <OPTION value=GB>
																								                                GB - 英国
																								                            </OPTION>
																								                            <OPTION value=HK>
																								                                HK - 香港
																								                            </OPTION>
																								                            <OPTION value=HR>
																								                                HR - 克罗地亚
																								                            </OPTION>
																								                            <OPTION value=HU>
																								                                HU - 匈牙利
																								                            </OPTION>
																								                            <OPTION value=IT>
																								                                IT - 意大利
																								                            </OPTION>
																								                            <OPTION value=MK>
																								                                MK - 马其顿
																								                            </OPTION>
																								                            <OPTION value=RU>
																								                                RU - 俄罗斯
																								                            </OPTION>
																								                            <OPTION value=SA>
																								                                SA - 沙特阿拉伯
																								                            </OPTION>
																								                            <OPTION value=SK>
																								                                SK - 斯洛伐克
																								                            </OPTION>
																								                        </SELECT>
																								                    </TD>
																								                </TR>
																								                <TR>
																								                    <TD class=table1_left align=left>
																								                        信令端口:
																								                    </TD>
																								                    <TD class=table1_right colSpan=2 align=left>
																								                        <SELECT id=X_HW_PortName name=X_HW_PortName maxlength="30">
																								                            <SCRIPT language=JavaScript type=text/javascript>
																								                                document.write('<option value=' + " " + ' >' + '<\/option>');
																								                                document.write('<option value=' + "br0" + ' id="br0">' + "br0" + '<\/option>');
																								
																								                                for (i = 0; i < WanInfo.length; i++) {
																								                                    document.write('<option id="wan_' + i + '" value="' + MakeVoipWanName(WanInfo[i]) + '">' + MakeWanName(WanInfo[i]) + '<\/option>');
																								                                }
																								                            </SCRIPT>
																								                        </SELECT>
																								                        <SPAN class=gray>
																								                            (选择语音信令WAN端口名。)
																								                        </SPAN>
																								                    </TD>
																								                </TR>
																								                <TR>
																								                    <TD class=table1_left align=left>
																								                        媒体端口:
																								                    </TD>
																								                    <TD class=table1_right colSpan=2 align=left>
																								                        <SELECT name=X_HW_PortName_RTP maxlength="30">
																								                            <SCRIPT language=JavaScript type=text/javascript>
																								                                document.write('<option value="" id="null">' + '' + '<\/option>');
																								                                document.write('<option value=' + "br0" + ' id="br0">' + "br0" + '<\/option>');
																								                                for (i = 0; i < WanInfo.length; i++) {
																								                                    document.write('<option id="wan_' + i + '" value="' + MakeVoipWanName(WanInfo[i]) + '">' + MakeWanName(WanInfo[i]) + '<\/option>');
																								                                }
																								                            </SCRIPT>
																								                        </SELECT>
																								                        <SPAN class=gray>
																								                            (选择语音媒体WAN端口名。媒体端口为空表示与信令端口名相同。)
																								                        </SPAN>
																								                    </TD>
																								                </TR>
																								            </TBODY>
																								        </TABLE>
																								        <TABLE border=0 cellSpacing=1 cellPadding=0 width="100%">
																								            <TBODY>
																								                <TR>
																								                    <TD>
																								                        <TABLE border=0 cellSpacing=0 cellPadding=0 width="100%">
																								                            <TBODY>
																								                                <TR>
																								                                    <TD class=table1_submit width="25%">
																								                                    </TD>
																								                                    <TD class=table1_submit>
																								                                        <INPUT class=submit onclick=SubmitInterface(); value=应用 type=button name=btnApplySipServer>
																								                                        <INPUT class=submit onclick=CancelConfig(); value=取消 type=button name=cancelValue>
																								                                    </TD>
																								                                </TR>
																								                            </TBODY>
																								                        </TABLE>
																								                    </TD>
																								                </TR>
																								            </TBODY>
																								        </TABLE>
																								        <TABLE border=0 cellSpacing=0 cellPadding=0 width="100%">
																								            <TBODY>
																								                <TR>
																								                    <TD height=15>
																								                    </TD>
																								                </TR>
																								            </TBODY>
																								        </TABLE>
																								        <TABLE border=0 cellSpacing=0 cellPadding=0 width="100%">
																								            <TBODY>
																								                <TR>
																								                    <TD class=table1_head>
																								                        用户基本参数
																								                    </TD>
																								                </TR>
																								            </TBODY>
																								        </TABLE>
																								        <TABLE border=0 cellSpacing=0 cellPadding=0 width="100%">
																								            <TBODY>
																								                <TR>
																								                    <TD class=prompt>
																								                        <TABLE border=0 cellSpacing=0 cellPadding=0 width="100%">
																								                            <TBODY>
																								                                <TR>
																								                                    <TD style="PADDING-LEFT: 10px" class=title_01 width="100%">
																								                                        &nbsp;
																								                                    </TD>
																								                                </TR>
																								                            </TBODY>
																								                        </TABLE>
																								                    </TD>
																								                </TR>
																								            </TBODY>
																								        </TABLE>
																								        <SCRIPT language=JavaScript type=text/javascript>
																								            writeTabCfgHeader('VOIP User', "100%");
																								        </SCRIPT>
																								        <TABLE style="TABLE-LAYOUT: fixed; WORD-BREAK: break-all; margin-top:0px;" id=voipUserTable
																								        class=table1_bg border=0 cellSpacing=1 cellPadding=0 width="100%">
																								            <TBODY>
																								                <TR class=table1_title>
																								                    <TD style="WIDTH: 8%">
																								                        &nbsp;
																								                    </TD>
																								                    <TD style="WIDTH: 8%">
																								                        编号
																								                    </TD>
																								                    <TD style="WIDTH: 14%">
																								                        URI
																								                    </TD>
																								                    <TD style="WIDTH: 14%">
																								                        注册用户名
																								                    </TD>
																								                    <TD style="WIDTH: 30%">
																								                        鉴权用户名
																								                    </TD>
																								                    <TD style="WIDTH: 10%">
																								                        密码
																								                    </TD>
																								                    <TD style="WIDTH: 12%">
																								                        关联端口号
																								                    </TD>
																								                </TR>
																								                <SCRIPT language=JavaScript type=text/javascript>
																				                        for (var i = 0; i < Line.length; i++)
																				                        {
																				                            var html = '';
																				                            if (i % 2 == 0) {
																				                                html += '<tr id="record_' + i + '" class="table1_01" ' + 'onclick="SelectLineRecord(this.id);">';
																				                            } else {
																				                                html += '<tr id="record_' + i + '" class="table1_02" ' + 'onclick="SelectLineRecord(this.id);">';
																				                            }
																				
																				                            html += '<td align="center">' + '<input name="rml" type="checkbox" value="' + Line[i].Domain + '";></td>';
																				
																				                            html += '<td align="left">' + (i + 1) + '</td>';
																				
																				                            if (Line[i].URI == "") {
																				                                html += '<td align="center">' + '--' + '&nbsp;</td>';
																				                            } else {
																				                                html += '<td align="left">' + Line[i].URI + '&nbsp;</td>';
																				                            }
																				
																				                            if (Line[i].DirectoryNumber == "") {
																				                                html += '<td align="center">' + '--' + '&nbsp;</td>';
																				                            } else {
																				                                html += '<td align="left">' + Line[i].DirectoryNumber + '&nbsp;</td>';
																				                            }
																				                            if (Line[i].AuthUserName == "") {
																				                                html += '<td align="center">' + '--' + '&nbsp;</td>';
																				                            } else {
																				                                html += '<td align="left">' + Line[i].AuthUserName + '&nbsp;</td>';
																				                            }
																				                            html += '<td align="center">' + '&nbsp  *******</td>' + '<td align="left">' + Line[i].PhyReferenceList + '&nbsp;</td>';
																				
																				                            html += '</tr>';
																				
																				                            document.write(html);
																				
																				                        }
																								                </SCRIPT>
																								            </TBODY>
																								        </TABLE>
																								        <SCRIPT language=JavaScript type=text/javascript>
																								        writeTabTail()
																								        </SCRIPT>
																								        
																								        <TABLE border=0 cellSpacing=0 cellPadding=0 width="100%">
																								            <TBODY>
																								                <TR>
																								                    <TD height=5>
																								                    </TD>
																								                </TR>
																								            </TBODY>
																								        </TABLE>
																								        <DIV id=ConfigForm1>
																								            <TABLE class=table1_bg border=0 cellSpacing=1 cellPadding=0 width="100%">
																								                <TBODY>
																								                    <TR>
																								                        <TD class=table1_left width="25%" align=left>
																								                            使能用户:
																								                        </TD>
																								                        <TD class=table1_right width="75%" colSpan=2 align=left>
																								                            <INPUT size=18 CHECKED type=checkbox name=Enable>
																								                        </TD>
																								                    </TR>
																								                    <TR>
																								                        <TD class=table1_left width="25%" align=left>
																								                            URI:
																								                        </TD>
																								                        <TD class=table1_right width="75%" colSpan=2 align=left>
																								                            <INPUT style="WIDTH: 155px" maxLength=389 type=text name=URI>
																								                            <SPAN class=gray>
																								                                (URI)
																								                            </SPAN>
																								                        </TD>
																								                    </TR>
																								                    <TR>
																								                        <TD class=table1_left width="25%" align=left>
																								                            注册用户名:
																								                        </TD>
																								                        <TD class=table1_right width="75%" colSpan=2 align=left>
																								                            <INPUT style="WIDTH: 155px" maxLength=256 type=text name=DirectoryNumber>
																								                            <SPAN class=gray>
																								                                (电话号码)
																								                            </SPAN>
																								                        </TD>
																								                    </TR>
																								                    <TR>
																								                        <TD class=table1_left width="25%" align=left>
																								                            关联端口号:
																								                        </TD>
																								                        <TD class=table1_right width="75%" colSpan=2 align=left>
																								                            <SELECT style="WIDTH: 40px" name=PhyReferenceList>
																								                                <SCRIPT language=JavaScript type=text/javascript>
																								                                    var k;
																								                                    document.write('<option value="" id="null">' + '' + '<\/option>');
																								                                    for (k = 1; k < AllPhyInterface.length; k++) {
																								                                        document.write('<option value="' + k + '">' + k + '</option>');
																								                                    }
																								                                </SCRIPT>
																								                            </SELECT>
																								                        </TD>
																								                    </TR>
																								                    <TR>
																								                        <TD class=table1_left width="25%" align=left>
																								                            鉴权用户名:
																								                        </TD>
																								                        <TD class=table1_right width="75%" colSpan=2 align=left>
																								                            <INPUT style="WIDTH: 155px" maxLength=256 type=text name=AuthUserName>
																								                            <SPAN class=gray>
																								                                (长度0-64)
																								                            </SPAN>
																								                        </TD>
																								                        </TD>
																								                    </TR>
																								                    <TR>
																								                        <TD class=table1_left width="25%" align=left>
																								                            密码:
																								                        </TD>
																								                        <TD class=table1_right width="75%" colSpan=2 align=left>
																								                            <INPUT style="WIDTH: 155px" maxLength=256 type=password name=AuthPassword>
																								                            <SPAN class=gray>
																								                                (长度0-64)
																								                            </SPAN>
																								                        </TD>
																								                    </TR>
																								                </TBODY>
																								            </TABLE>
																								            <TABLE border=0 cellSpacing=1 cellPadding=0 width="100%">
																								                <TBODY>
																								                    <TR>
																								                        <TD>
																								                            <TABLE border=0 cellSpacing=0 cellPadding=0 width="100%">
																								                                <TBODY>
																								                                    <TR>
																								                                        <TD class=table1_submit width="25%">
																								                                        </TD>
																								                                        <TD class=table1_submit>
																								                                            <INPUT class=submit onclick=Submit(); value=应用 type=button name=btnApplySipUser>
																								                                            <INPUT class=submit onclick=CancelUserConfig(); value=取消 type=button name=cancelValue>
																								                                        </TD>
																								                                    </TR>
																								                                </TBODY>
																								                            </TABLE>
																								                        </TD>
																								                    </TR>
																								                </TBODY>
																								            </TABLE>
																								            <TABLE border=0 cellSpacing=0 cellPadding=0 width="100%">
																								                <TBODY>
																								                    <TR>
																								                        <TD height=10>
																								                        </TD>
																								                    </TR>
																								                </TBODY>
																								            </TABLE>
																								        </DIV>

																										<!-- end -->
                                                    </TD>
                                                    <TD width=10>
                                                        &nbsp;
                                                    </TD>
                                                </TR>
                                                <TR>
                                                    <TD width=10>
                                                        &nbsp;
                                                    </TD>
                                                    <TD>
                                                    </TD>
                                                    <TD width=10>
                                                        &nbsp;
                                                    </TD>
                                                </TR>
                                            </TBODY>
                                        </TABLE>
                                    </TD>
                                </TR>
                                <TR>
                                    <TD vAlign=top width=157 bgColor=#e7e7e7 height="11px">
                                    </TD>
                                    <TD width=7 background=/img/panel3.gif>
                                    </TD>
                                    <TD>
                                    </TD>
                                </TR>                                
                                <TR>
                                    <TD vAlign=top width=157 bgColor=#e7e7e7>
                                    </TD>
                                    <TD width=7 background=/img/panel3.gif></TD>
                                    <TD></TD>
                                </TR>
                            </TBODY>
                        </TABLE>
                    </TD>
                </TR>
                <TR>
                    <TD height=1>
                        <TABLE id=table7 height=35 cellSpacing=0 cellPadding=0 width=808 border=0>
                            <TBODY>
                                <TR>
                                    <TD width=162 bgColor=#ef8218>
                                    	&nbsp;
                                    </TD>
                                    <TD bgColor=#427594>&nbsp;</TD>
                                    <TD bgColor=#427594>
                                        <P align=center>&nbsp;</P>
                                    </TD>
                                    <TD class=table1_help bgColor=#313031>&nbsp;</TD>
                                </TR>
                            </TBODY>
                        </TABLE>
                    </TD>
                </TR>
            </TBODY>
        </TABLE>
    </BODY>

</HTML>