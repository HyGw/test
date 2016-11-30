<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0 Transitional//EN">
<HTML>
    <HEAD>
        <TITLE>
            ����绰����
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
        var dmm = "x��ʾ0-9�����֣�[0-9]��ʾ0��9��������֣�S��ʾ�̶�ʱ����L��ʾ����ʱ����'.'��ʾ�ظ�ǰ���0�����";
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
                        alert('Outbound��������ַ����ȷ.');
                        return false;
                    }
                } else {
                    if (false == isValidCfgStr('The outbound proxy server address', getValue('OutboundProxy'), 256)) {
                        return false;
                    }
                    var OutboundProxy = getValue('OutboundProxy');

                    if ((OutboundProxy.substring(0, 1) <= 9) && (OutboundProxy.substring(0, 1) >= 0)) {
                        alert('Outbound��������Ч.');
                        return false;
                    }
                }
            }
            if ('' != removeSpaceTrim(getValue('OutboundProxyPort'))) {
                if (isValidPort(getValue('OutboundProxyPort')) == false) {
                    alert('Outbound�������˿ں� "' + getValue('OutboundProxyPort') + '" ��Ч.');
                    return false;
                }
            }
            if ('' != removeSpaceTrim(getValue('SecondaryOutboundProxy'))) {
                if (true == isIpAddress(getValue('SecondaryOutboundProxy'))) {
                    if (false == isvalidVoipIpAddress(getValue('SecondaryOutboundProxy'))) {
                        alert('����Outbound��������ַ��Ч.');
                        return false;
                    }
                } else {
                    if (false == isValidCfgStr('The secondary proxy server address', getValue('SecondaryOutboundProxy'), 256)) {
                        return false;
                    }
                    var SecondaryOutboundProxy = getValue('SecondaryOutboundProxy');

                    if ((SecondaryOutboundProxy.substring(0, 1) <= 9) && (SecondaryOutboundProxy.substring(0, 1) >= 0)) {
                        alert('����Outbound��������Ч.');
                        return false;
                    }
                }
            }
            if ('' != removeSpaceTrim(getValue('SecondaryOutboundProxyPort'))) {
                if (isValidPort(getValue('SecondaryOutboundProxyPort')) == false) {
                    alert('����Outbound�������˿ں� "' + getValue('SecondaryOutboundProxyPort') + '" ��Ч.');
                    return false;
                }
            }

            if ('' != removeSpaceTrim(getValue('ProxyServer'))) {
                if (true == isIpAddress(getValue('ProxyServer'))) {
                    if (false == isvalidVoipIpAddress(getValue('ProxyServer'))) {
                        alert('���÷�������ַ��Ч.');
                        return false;
                    }
                } else {
                    if (false == isValidCfgStr('The primary proxy server address', getValue('ProxyServer'), 256)) {
                        return false;
                    }
                    var proxyserv = getValue('ProxyServer');

                    if ((proxyserv.substring(0, 1) <= 9) && (proxyserv.substring(0, 1) >= 0)) {
                        alert('���÷�������Ч��');
                        return false;
                    }

                }
            }

            if ('' != removeSpaceTrim(getValue('ProxyServerPort'))) {
                if (isValidPort(getValue('ProxyServerPort')) == false) {
                    alert('���÷������˿ںš�' + getValue('ProxyServerPort') + '����Ч��');
                    return false;
                }
            }

            if ('' != removeSpaceTrim(getValue('SecondProxyServer'))) {
                if (true == isIpAddress(getValue('SecondProxyServer'))) {
                    if (false == isvalidVoipIpAddress(getValue('SecondProxyServer'))) {
                        alert('���÷�������ַ��Ч��');
                        return false;
                    }
                } else {
                    if (false == isValidCfgStr('The secondary proxy server address', getValue('SecondProxyServer'), 256)) {
                        return false;
                    }
                    var proxysndserv = getValue('SecondProxyServer');

                    if ((proxysndserv.substring(0, 1) <= 9) && (proxysndserv.substring(0, 1) >= 0)) {
                        alert('���÷�������ַ��Ч��');
                        return false;
                    }
                }
            }

            if ('' != removeSpaceTrim(getValue('SecondProxyServerPort'))) {
                if (isValidPort(getValue('SecondProxyServerPort')) == false) {
                    alert('���÷�������ַ�˿ڡ�' + getValue('SecondProxyServerPort') + '����Ч��');
                    return false;
                }
            }

            if ('' != removeSpaceTrim(getValue('HomeDomain'))) {
                if (true == isIpAddress(getValue('HomeDomain'))) {
                    if (false == isvalidVoipIpAddress(getValue('HomeDomain'))) {
                        alert('����������Ч��');
                        return false;
                    }
                } else {
                    if (isValidCfgStr('The SIP home domain', getValue('HomeDomain'), 256) == false) {
                        alert('����������Ч��');
                        return false;
                    }
                }
            }

            if ('' == getValue('UserAgentPort')) {
                alert('SIP���ض˿ںŲ���Ϊ�ա�');
                return false;
            } else {
                if (isValidPort(getValue('UserAgentPort')) == false) {
                    alert('SIP���ض˿ںš�' + getValue('UserAgentPort') + '����Ч��');
                    return false;
                }
            }

            if (getValue('DigitMap').length > 8000) {
                alert('��ͼ���Ȳ��ܳ���8000���ַ���');
                return false;
            }

            if (isSafeStringIn(getValue('DigitMap'), 'ABCDEFXabcdefxT0123456789-[]()*#.T|LSZ') == false) {
                alert('��ͼ���Ϸ����Ϸ���Χ�ǡ�ABCDEFXabcdefxT0123456789-[]()*#.T|LSZ����');
                return false;
            }

            if (false == isInteger(getValue('RegistrationPeriod'))) {
                alert('ע�����ڷǷ�����������һ��1��65534֮�����ֵ��');
                return false;
            }

            if ((getValue('RegistrationPeriod') > 65534) || (getValue('RegistrationPeriod') < 1)) {
                alert('ע�����ڷǷ�����������һ��1��65534֮�����ֵ��');
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
                    alert('��������û�����');
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
                alert("���û���");
                return;
            }

            if (selctIndex == -1) {
                alert("����ɾ�����û������ȱ������û���");
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
                alert('��ѡ���û���');
                return;
            }

            if (confirm("ȷ��Ҫɾ���û�?") == false) {
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
                        alert('URI�Ѿ����ڡ�');
                        return false;
                    }

                    if ((getValue('DirectoryNumber') != "") && (getValue('DirectoryNumber') == Line[i].DirectoryNumber)) {
                        alert('ע���û����Ѿ����ڡ�');
                        return false;
                    }
                    if ((getValue('DirectoryNumber') == "") && (getValue('AuthUserName') != "") && (getValue('AuthUserName') == Line[i].AuthUserName)) {
                        alert('��Ȩ�û����Ѿ����ڡ�');
                        return false;
                    }
                }
            }

            for (var i = 0; i < Line.length; i++) {
                if (selctIndex != i) {
                    if ((getValue('PhyReferenceList') != "") && (getValue('PhyReferenceList') == Line[i].PhyReferenceList)) {
                        if ((getCheckVal('Enable') == 1) && (Line[i].Enable == 1)) {
                            var ulret = confirm('�˿�' + getValue('PhyReferenceList') + '�Ѵ����û���,ȷ��Ҫͬʱ�󶨵��ö˿�?');
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
                                                        ��ӭ����
                                                    </TD>
                                                    <TD vAlign=bottom width=50>
                                                        <A onclick=DoLogout() href="/cgi-bin/logout.cgi" target=_top>
                                                            <SPAN class=logout>
                                                                �˳�
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
                                            <FONT face=���� color=#ffffff>
                                                <B>
                                                    <FONT face=���� color=#ffffff size=6>
                                                        <INPUT id=Selected_Menu type=hidden value="Ӧ��->����绰����" name=Selected_Menu>
                                                    </FONT>
                                                </B>
                                                <SPAN class=GatewayName>
                                                    ��������:
                                                    <SCRIPT language=javascript>
                                                        document.write(top.gateWayName);
                                                    </SCRIPT>
                                                </SPAN>
                                            </FONT>
                                        </P>
                                    </TD>
                                    <TD width=211 bgColor=#ef8218 height=33>
                                        <P class=GatewayType align=center>
                                            �ͺ�:
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
                                            <a href='/cgi-bin/app-VoIP.asp' class=Item_L1 style="text-decoration: none;">����绰��������</a>
                                        </P>
                                    </TD>
                                    <TD width=7 bgColor=#ef8218>
                                        ��
                                    </TD>
                                    <TD width=564>
                                        ��
                                    </TD>
                                    <TD vAlign=top class=table1_help background=/img/panel4.gif rowSpan=8>
                                        <TABLE cellSpacing=0 cellPadding=20 width="100%" border=0 height='100%'>
                                            <TBODY>
                                                <TR>
                                                    <TD valign='top'>
                                                        <A href="/cgi-bin/help_content.asp#����绰����" target=_blank>
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
                                        	<a href='/cgi-bin/app-VoIPUser.asp' style="text-decoration: none;">����绰�߼�����</a>
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
																						                        �ӿڻ�������
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
																						                        Outbound��������ַ:
																						                    </TD>
																						                    <TD class=table1_right width="65%" colSpan=2 align=left>
																						                        <INPUT style="WIDTH: 155px" id=OutboundProxy maxLength=256
																						                        type=text name=OutboundProxy>
																						                        <SPAN class=gray>
																						                            (IP������)
																						                        </SPAN>
																						                    </TD>
																						                </TR>
																						                <TR>
																						                    <TD class=table1_left align=left>
																						                        Outbound�������˿ں�:
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
																						                        ����Outbound��������ַ:
																						                    </TD>
																						                    <TD class=table1_right colSpan=2 align=left>
																						                        <INPUT style="WIDTH: 155px" id=SecondaryOutboundProxy
																						                        maxLength=256 type=text name=SecondaryOutboundProxy>
																						                        <SPAN class=gray>
																						                            (IP������)
																						                        </SPAN>
																						                    </TD>
																						                </TR>
																						                <TR>
																						                    <TD class=table1_left align=left>
																						                        ����Outbound�������˿ں�:
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
																								                        ���÷�������ַ:
																								                    </TD>
																								                    <TD class=table1_right colSpan=2 align=left>
																								                        <INPUT style="WIDTH: 155px" id=ProxyServer maxLength=256 type=text name=ProxyServer>
																								                        <SPAN class=gray>
																								                            (IP������)
																								                        </SPAN>
																								                    </TD>
																								                </TR>
																								                <TR>
																								                    <TD class=table1_left align=left>
																								                        ���ö˿ں�:
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
																								                        ���÷�������ַ:
																								                    </TD>
																								                    <TD class=table1_right width="65%" colSpan=2 align=left>
																								                        <INPUT style="WIDTH: 155px" id=SecondProxyServer
																								                        maxLength=256 type=text name=SecondProxyServer>
																								                        <SPAN class=gray>
																								                            (IP������)
																								                        </SPAN>
																								                    </TD>
																								                </TR>
																								                <TR>
																								                    <TD class=table1_left align=left>
																								                        ���ö˿ں�:
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
																								                            ��������:
																								                        </TD>
																								                        <TD class=table1_right width="65%" colSpan=2 align=left>
																								                            <INPUT style="WIDTH: 155px" id=HomeDomain maxLength=256 type=text name=HomeDomain>
																								                            <SPAN class=gray>
																								                            (IP������)
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
																								                        ���ض˿�:
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
																								                        ��ͼ:
																								                    </TD>
																								                    <TD id=digitmap1 class=table1_right colSpan=2 align=left>
																								                        <TEXTAREA id=DigitMap rows=3 cols=65 style="WIDTH: 100%" name=DigitMap>
																								                        </TEXTAREA>
																								                    </TD>
																								                </TR>
																								                <TR>
																								                    <TD class=table1_left align=left>
																								                        ��ͼƥ��ģʽ:
																								                    </TD>
																								                    <TD class=table1_right colSpan=2 align=left>
																								                        <SELECT style="WIDTH: 75px" id=X_HW_DigitMapMatchMode size=1 name=X_HW_DigitMapMatchMode>
																								                            <OPTION selected value=Min>
																								                                ��Сƥ��
																								                            </OPTION>
																								                            <OPTION value=Max>
																								                                ���ƥ��
																								                            </OPTION>
																								                        </SELECT>
																								                    </TD>
																								                </TR>
																								                <TR>
																								                    <TD class=table1_left align=left>
																								                        ע������:
																								                    </TD>
																								                    <TD class=table1_right colSpan=2 align=left>
																								                        <P>
																								                            <INPUT style="WIDTH: 155px" id=RegistrationPeriod value=600 maxLength=11
																								                            type=text name=RegistrationPeriod>
																								                            <SPAN class=gray>
																								                                (��λ����)(1~65534)
																								                            </SPAN>
																								                        </P>
																								                    </TD>
																								                </TR>
																								                <TR>
																								                    <TD class=table1_left align=left>
																								                        ���Ҽ�����:
																								                    </TD>
																								                    <TD class=table1_right colSpan=2 align=left>
																								                        <SELECT id=Region name=Region>
																								                            <OPTION selected value="">
																								                            </OPTION>
																								                            <OPTION value=AE>
																								                                AE - ����������������
																								                            </OPTION>
																								                            <OPTION value=AR>
																								                                AR - ����͢
																								                            </OPTION>
																								                            <OPTION value=BR>
																								                                BR - ����
																								                            </OPTION>
																								                            <OPTION value=CN>
																								                                CN - �й�
																								                            </OPTION>
																								                            <OPTION value=DE>
																								                                DE - �¹�
																								                            </OPTION>
																								                            <OPTION value=ES>
																								                                ES - ������
																								                            </OPTION>
																								                            <OPTION value=GB>
																								                                GB - Ӣ��
																								                            </OPTION>
																								                            <OPTION value=HK>
																								                                HK - ���
																								                            </OPTION>
																								                            <OPTION value=HR>
																								                                HR - ���޵���
																								                            </OPTION>
																								                            <OPTION value=HU>
																								                                HU - ������
																								                            </OPTION>
																								                            <OPTION value=IT>
																								                                IT - �����
																								                            </OPTION>
																								                            <OPTION value=MK>
																								                                MK - �����
																								                            </OPTION>
																								                            <OPTION value=RU>
																								                                RU - ����˹
																								                            </OPTION>
																								                            <OPTION value=SA>
																								                                SA - ɳ�ذ�����
																								                            </OPTION>
																								                            <OPTION value=SK>
																								                                SK - ˹�工��
																								                            </OPTION>
																								                        </SELECT>
																								                    </TD>
																								                </TR>
																								                <TR>
																								                    <TD class=table1_left align=left>
																								                        ����˿�:
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
																								                            (ѡ����������WAN�˿�����)
																								                        </SPAN>
																								                    </TD>
																								                </TR>
																								                <TR>
																								                    <TD class=table1_left align=left>
																								                        ý��˿�:
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
																								                            (ѡ������ý��WAN�˿�����ý��˿�Ϊ�ձ�ʾ������˿�����ͬ��)
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
																								                                        <INPUT class=submit onclick=SubmitInterface(); value=Ӧ�� type=button name=btnApplySipServer>
																								                                        <INPUT class=submit onclick=CancelConfig(); value=ȡ�� type=button name=cancelValue>
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
																								                        �û���������
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
																								                        ���
																								                    </TD>
																								                    <TD style="WIDTH: 14%">
																								                        URI
																								                    </TD>
																								                    <TD style="WIDTH: 14%">
																								                        ע���û���
																								                    </TD>
																								                    <TD style="WIDTH: 30%">
																								                        ��Ȩ�û���
																								                    </TD>
																								                    <TD style="WIDTH: 10%">
																								                        ����
																								                    </TD>
																								                    <TD style="WIDTH: 12%">
																								                        �����˿ں�
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
																								                            ʹ���û�:
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
																								                            ע���û���:
																								                        </TD>
																								                        <TD class=table1_right width="75%" colSpan=2 align=left>
																								                            <INPUT style="WIDTH: 155px" maxLength=256 type=text name=DirectoryNumber>
																								                            <SPAN class=gray>
																								                                (�绰����)
																								                            </SPAN>
																								                        </TD>
																								                    </TR>
																								                    <TR>
																								                        <TD class=table1_left width="25%" align=left>
																								                            �����˿ں�:
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
																								                            ��Ȩ�û���:
																								                        </TD>
																								                        <TD class=table1_right width="75%" colSpan=2 align=left>
																								                            <INPUT style="WIDTH: 155px" maxLength=256 type=text name=AuthUserName>
																								                            <SPAN class=gray>
																								                                (����0-64)
																								                            </SPAN>
																								                        </TD>
																								                        </TD>
																								                    </TR>
																								                    <TR>
																								                        <TD class=table1_left width="25%" align=left>
																								                            ����:
																								                        </TD>
																								                        <TD class=table1_right width="75%" colSpan=2 align=left>
																								                            <INPUT style="WIDTH: 155px" maxLength=256 type=password name=AuthPassword>
																								                            <SPAN class=gray>
																								                                (����0-64)
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
																								                                            <INPUT class=submit onclick=Submit(); value=Ӧ�� type=button name=btnApplySipUser>
																								                                            <INPUT class=submit onclick=CancelUserConfig(); value=ȡ�� type=button name=cancelValue>
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