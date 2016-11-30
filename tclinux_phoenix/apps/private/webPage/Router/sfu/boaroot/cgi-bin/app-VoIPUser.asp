<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0 Transitional//EN">
<HTML>
    
    <HEAD>
        <TITLE>
            宽带电话高级设置
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
            var selctLindex = 0;

            function stProfile1(Domain, Region) {
                this.Domain = Domain;
                this.Region = Region;
                var temp = Domain.split('.');
                this.key = '.' + temp[5] + '.';
            }

            var AllProfile1 = new Array(new stProfile1("InternetGatewayDevice.Services.VoiceService.1.VoiceProfile.1", ""), null);
            var Profile1 = new Array();
            for (var i = 0; i < AllProfile1.length - 1; i++) {
                Profile1[i] = AllProfile1[i];
            }
            function onInputUserid(elem) {
                var directoryNumber = document.getElementById("DirectoryNumber");
                var authUserName = document.getElementById("AuthUserName");

                directoryNumber.value = elem.value;
                authUserName.value = elem.value;
            }

            function stPhyInterface(Domain, InterfaceID) {
                this.Domain = Domain;
                this.InterfaceID = InterfaceID;
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
                var temp = Domain.split('.');
                this.key = '.' + temp[7] + '.';
            }

            var AllLine = new Array(new stLine("InternetGatewayDevice.Services.VoiceService.1.VoiceProfile.1.Line.1", "", "Disabled", "1"), new stLine("InternetGatewayDevice.Services.VoiceService.1.VoiceProfile.1.Line.2", "", "Disabled", "2"), null);
            var Line = new Array();
            for (var i = 0; i < AllLine.length - 1; i++) Line[i] = AllLine[i];

            function stLineSIP(Domain, URI) {
                this.Domain = Domain;
                this.URI = URI;
                var temp = Domain.split('.');
                this.key = '.' + temp[7] + '.';
            }

            var AllLineSIP = new Array(new stLineSIP("InternetGatewayDevice.Services.VoiceService.1.VoiceProfile.1.Line.1.SIP", ""), new stLineSIP("InternetGatewayDevice.Services.VoiceService.1.VoiceProfile.1.Line.2.SIP", ""), null);

            function stAuth(Domain, AuthUserName, AuthPassword) {
                this.Domain = Domain;
                this.AuthUserName = AuthUserName;
                this.AuthPassword = AuthPassword;
                var temp = Domain.split('.');
                this.key = '.' + temp[7] + '.';
            }

            var AllAuth = new Array(new stAuth("InternetGatewayDevice.Services.VoiceService.1.VoiceProfile.1.Line.1.SIP", "", ""), new stAuth("InternetGatewayDevice.Services.VoiceService.1.VoiceProfile.1.Line.2.SIP", "", ""), null);
            var Auth = new Array();
            for (var i = 0; i < AllAuth.length - 1; i++) Auth[i] = AllAuth[i];

            AssociateParam('Line', 'Auth', 'AuthUserName|AuthPassword');

            /*-----------------------handle event-------------------------*/

            /*-----------------------adapte funcion--------------------------*/
            //loadpage
            var recordDirectoryNumber;

            function SelectLineRecord(recordId) {
                var temp = recordId.split('_');
                selctLindex = temp[1];
                selectLine(recordId);
                setPhyList("PhyList");
                setPhyInterfaceParams();
                setDspTemplatePara();
            }
            function stVoiceTransmode(Domain, FaxT38_Enable) {
                this.Domain = Domain;
                this.FaxT38_Enable = FaxT38_Enable;
            }
            var VoiceTransmode = new Array(new stVoiceTransmode("InternetGatewayDevice.Services.VoiceService.1.VoiceProfile.1.FaxT38", "0"), null);

            function stVoiceTransSwitch(Domain, X_HW_FaxModem_FaxNego) {
                this.Domain = Domain;
                this.X_HW_FaxModem_FaxNego = X_HW_FaxModem_FaxNego;
            }
            var VoiceTransswitch = new Array(new stVoiceTransSwitch("InternetGatewayDevice.Services.VoiceService.1.VoiceProfile.1.X_HW_FaxModem", "1"), null);

            function stProfileBody(Domain, X_HW_SIPProfile_Body) {
                this.Domain = Domain;
                this.X_HW_SIPProfile_Body = X_HW_SIPProfile_Body;
            }
            var ProfileBody = new Array(new stProfileBody("InternetGatewayDevice.Services.VoiceService.1.VoiceProfile.1.SIP.X_HW_SIPProfile", "1=4294967295;2=1;3=1;4=0;5=0;6=0;7=1;8=600;9=1;10=0;11=0;12=0;13=1;14=1;15=0;16=0;17=0;18=0;19=0;20=1;21=1;22=0;23=64;24=60;25=60;26=32;27=120;28=120;29=30;30=60;31=40;32=60;33=500;34=45;35=0;36=4202735;37=24575;38=532615;39=15;40=33007;41=1025;42=0;43=0;44=4294967295;45=1;46=1;47=0;48=4294967295;49=0;50=1;51=0;52=2;53=0;54=4294967295;55=0;56=0;57=1;58=1;59=1;60=1;61=2;62=500;63=360;64=360;65=8;66=1;67=1;68=4294967295;69=0;70=0;71=4294967295;72=2;73=4294967295;74=4294967295;75=4294967295;76=4294967295;77=4294967295;78=4294967295;79=4294967295;80=0;81=2;82=0;83=4294967295;84=2;85=1;86=3;87=1;88=180;89=10;90=4;91=20;92=30;93=30;94=180;95=4;96=90;97=30;98=6;99=4;100=120;101=100;102=30;103=30;104=4;105=10;106=10;107=16;108=8;109=2;110=0;111=1;112=2;113=2;114=1;115=2;116=1;117=0;118=0;119=1;120=0;121=1;122=2;123=0;124=0;125=0;126=0;127=0;128=0;129=0;130=1;131=0;132=0;133=1;134=1;135=0;136=0;137=1;138=0;139=0;140=0;141=1;142=1;143=0;144=0;145=1;146=1;147=2;148=1;149=0;150=36000;151=500;152=2;153=1;154=8194;155=0;156=0;157=50;158=0;159=0;160=0;161=1;162=1;163=0;164=0;165=0;166=1;167=1;168=0;169=0;170=0;171=0;172=180;173=90;174=1;175=0;176=0;177=30;178=0;179=0;180=1;181=1;182=0;183=0;184=1;185=1;186=2;187=60000;188=1;189=0;190=0;191=0;192=0;193=0;194=21600;195=0;196=0;197=0;198=0;199=0;200=0;201=0;202=0;203=4294967295;204=480;205=486;206=486;207=0;208=30;209=0;210=0;211=0;212=0;213=0;214=0;215=0;216=0;217=0;218=1;219=0;220=0;221=90;222=0;223=1;224=0;225=20;226=20;227=4;228=0;229=0;230=0;231=0;232=0;233=4294967295;234=0;235=0;236=0;237=0;238=1800;239=0;240=4294967295;241=0;242=1200;243=1;244=1;245=0;246=5;247=0;248=0;249=1;250=4294967295;251=1000;252=0;253=1;254=0;255=0;256=1;257=1;258=0;259=0;260=4294967295;261=0;262=0;263=5;264=0;265=1;266=100;267=1;268=0;269=0;270=0;271=0;272=0;273=0;274=0;275=0;276=0;277=1;278=1;279=0;280=16383;281=0;282=1;283=0;284=0;285=0;286=0;287=0;288=0;289=4294967295;290=4294967295;291=4294967295;292=4294967295;293=4294967295;294=4294967295;295=4294967295;296=4294967295;297=4294967295;298=4294967295;299=4294967295;300=4294967295;301=0;302=0;303=4294967295;304=4294967295;305=0;306=0;307=0"), null);

            function stSoftwarePara(Domain, X_HW_SIPExtend_SoftwarePara) {
                this.Domain = Domain;
                this.X_HW_SIPExtend_SoftwarePara = X_HW_SIPExtend_SoftwarePara;
            }
            var SoftwarePara = new Array(new stSoftwarePara("InternetGatewayDevice.Services.VoiceService.1.VoiceProfile.1.SIP.X_HW_SIPExtend", "8=0;"), null);

            function stEchoCancellation(Domain, X_HW_RTPExtend_EchoCancellationEnable) {
                this.Domain = Domain;
                this.X_HW_RTPExtend_EchoCancellationEnable = X_HW_RTPExtend_EchoCancellationEnable;
            }
            var EchoCancellation = new Array(new stEchoCancellation("InternetGatewayDevice.Services.VoiceService.1.VoiceProfile.1.RTP.X_HW_Extend", "1"), null);

            function stCodec(Domain, EntryID, Codecs, PacketizationPeriod, Priority, Enable) {
                this.Domain = Domain;
                this.EntryID = EntryID;
                this.Codecs = Codecs;
                this.PacketizationPeriod = PacketizationPeriod;
                this.Priority = Priority;
                this.Enable = Enable;
                var temp = Domain.split('.');
                this.key = '.' + temp[7] + '.';
            }

            var AllCodecList = new Array(new stCodec("InternetGatewayDevice.Services.VoiceService.1.VoiceProfile.1.Line.1.Codec.List.1", "1", "G.711MuLaw", "20", "4", "1"), new stCodec("InternetGatewayDevice.Services.VoiceService.1.VoiceProfile.1.Line.1.Codec.List.2", "2", "G.711ALaw", "20", "1", "1"), new stCodec("InternetGatewayDevice.Services.VoiceService.1.VoiceProfile.1.Line.1.Codec.List.3", "3", "G.729", "20", "3", "1"), new stCodec("InternetGatewayDevice.Services.VoiceService.1.VoiceProfile.1.Line.1.Codec.List.4", "4", "G.722", "20", "2", "1"), new stCodec("InternetGatewayDevice.Services.VoiceService.1.VoiceProfile.1.Line.2.Codec.List.1", "1", "G.711MuLaw", "20", "4", "1"), new stCodec("InternetGatewayDevice.Services.VoiceService.1.VoiceProfile.1.Line.2.Codec.List.2", "2", "G.711ALaw", "20", "1", "1"), new stCodec("InternetGatewayDevice.Services.VoiceService.1.VoiceProfile.1.Line.2.Codec.List.3", "3", "G.729", "20", "3", "1"), new stCodec("InternetGatewayDevice.Services.VoiceService.1.VoiceProfile.1.Line.2.Codec.List.4", "4", "G.722", "20", "2", "1"), null);
            var CodecList = new Array();
            for (var i = 0; i < AllCodecList.length - 1; i++) CodecList[i] = AllCodecList[i];

            function stHotLine(Domain, X_HW_HotlineEnable, X_HW_HotlineNumber, X_HW_HotlineTimer) {
                this.Domain = Domain;
                this.X_HW_HotlineEnable = X_HW_HotlineEnable;
                this.X_HW_HotlineNumber = X_HW_HotlineNumber;
                this.X_HW_HotlineTimer = X_HW_HotlineTimer;
            }

            var AllHotLine = new Array(new stHotLine("InternetGatewayDevice.Services.VoiceService.1.VoiceProfile.1.Line.1.CallingFeatures", "0", "", "5"), new stHotLine("InternetGatewayDevice.Services.VoiceService.1.VoiceProfile.1.Line.2.CallingFeatures", "0", "", "5"), null);

            var HotLineList = new Array();
            for (var i = 0; i < AllHotLine.length - 1; i++) HotLineList[i] = AllHotLine[i];

            var SipDigitMap = '[*#][0-9][0-9*].#|**xx|#*#|*#xx#|#*99|##|010xxxxxxxx|02xxxxxxxxx|0[3-9]xxxxxxxxx|0311xxxxxxxx|037[179]xxxxxxxx|04[15]1xxxxxxxx|043[12]xxxxxxxx|051[0-9]xxxxxxxx|052[37]xxxxxxxx|053[12]xxxxxxxx|057[1345679]xxxxxxxx|059[15]xxxxxxxx|0731xxxxxxxx|075[457]xxxxxxxx|076[09]xxxxxxxx|0898xxxxxxxx|00xxx.|[2-8][1-9]xxxxxx|1[3458]xxxxxxxxx|01[3458]xxxxxxxxx|11[02479]|12[0268]|11[13568]x.|125xx|12[13479]x.|100[015678]x|100[2349]x.|10[1-9]xx.|14xx.|1[79]xx.|160|168xxxxx|16[1-79]x.|[48]00xxxxxxx|[48]0[1-9]x.|[23567]0xx.|1630x|955xx|9699[*#]|95[0-46-9]xxx.|9[0-46-9]xxxx.|x.';
            var AllProfile = new Array(null);
            var Profile = new Array();
            for (var i = 0; i < AllProfile.length - 1; i++) {
                AllProfile[i].DigitMap = SipDigitMap;
                Profile[i] = AllProfile[i];
            }

            function stSipDigitMap(domain, DigitMapShortTimer, DigitMapLongTimer) {
                this.domain = domain;
                this.DigitMapShortTimer = DigitMapShortTimer;
                this.DigitMapLongTimer = DigitMapLongTimer;
            }
            var SipDigitMapPara = new Array(new stSipDigitMap("InternetGatewayDevice.Services.VoiceService.1.VoiceProfile.1.SIP.X_HW_SIPDigitmap.1", "2", "5"), null);

            function stPhyInterfaceParam(Domain, HookFlashDownTime, HookFlashUpTime, OnhookConfirmTime, Impedance, Current, RingFrequency, RingVoltage, SendGain, ReceiveGain, FskTime, ClipTransWhen, ClipFormat, RingDCVoltageOverlapped, UserDefineRingVoltage, ReversePoleOnAnswer, ClipSendDateTime) {
                this.Domain = Domain;
                this.HookFlashDownTime = HookFlashDownTime;
                this.HookFlashUpTime = HookFlashUpTime;
                this.OnhookConfirmTime = OnhookConfirmTime;
                this.Impedance = Impedance;
                this.Current = Current;
                this.RingFrequency = RingFrequency;
                this.RingVoltage = RingVoltage;
                this.SendGain = SendGain;
                this.ReceiveGain = ReceiveGain;
                this.FskTime = FskTime;
                this.ClipTransWhen = ClipTransWhen;
                this.ClipFormat = ClipFormat;
                this.RingDCVoltageOverlapped = RingDCVoltageOverlapped;
                this.UserDefineRingVoltage = UserDefineRingVoltage;
                this.ReversePoleOnAnswer = ReversePoleOnAnswer;
                this.ClipSendDateTime = ClipSendDateTime;
            }
            var PhyInterfaceParams = new Array(new stPhyInterfaceParam("InternetGatewayDevice.Services.VoiceService.1.PhyInterface.1.X_HW_Extend", "100", "500", "0", "2", "25", "1", "0", "0db", "0db", "800", "AfterRing", "Sdmf-fsk", "0", "65", "1", "0"), new stPhyInterfaceParam("InternetGatewayDevice.Services.VoiceService.1.PhyInterface.2.X_HW_Extend", "100", "500", "0", "2", "25", "1", "0", "0db", "0db", "800", "AfterRing", "Sdmf-fsk", "0", "65", "1", "0"), null);

            function stDspTemplateParam(Domain, Enable, EchoCancellationEnable, SilenceSuppression, JbMode, NLP, WorkMode) {
                this.Domain = Domain;
                this.Enable = Enable;
                this.EchoCancellationEnable = EchoCancellationEnable;
                this.SilenceSuppression = SilenceSuppression;
                this.JbMode = JbMode;
                this.NLP = NLP;
                this.WorkMode = WorkMode;
            }
            var DspTemplateParams = new Array(new stDspTemplateParam("InternetGatewayDevice.Services.VoiceService.1.PhyInterface.1.X_HW_DspTemplate", "0", "0", "0", "Static", "Closed", "Voice"), new stDspTemplateParam("InternetGatewayDevice.Services.VoiceService.1.PhyInterface.2.X_HW_DspTemplate", "0", "0", "0", "Static", "Closed", "Voice"), null);

            function LoadFrame() {
                var X_HW_RTPExtend_EchoCancellationEnable = document.getElementById('X_HW_RTPExtend_EchoCancellationEnable');
                var FaxT38_Enable = document.getElementById('FaxT38_Enable');
                var X_HW_FaxModem_FaxNego = document.getElementById('X_HW_FaxModem_FaxNego');
                var X_HW_SIPProfile_Body = document.getElementById('X_HW_SIPProfile_Body');
                var SoftwareParameters = document.getElementById('SoftwareParameters');
                var ProfileBody_exp = '参数格式为“序号＝值”，中间以分号分隔。序号从1开始，必须是连续的。';
                var Software_exp = '参数格式为“序号＝值”，中间以分号分隔。';

                setSelect('FaxT38_Enable', VoiceTransmode[0].FaxT38_Enable);
                //Fax Switchmode
                setSelect('X_HW_FaxModem_FaxNego', VoiceTransswitch[0].X_HW_FaxModem_FaxNego);
                //Profile Body
                setText('X_HW_SIPProfile_Body', ProfileBody[0].X_HW_SIPProfile_Body);
                //SoftwarePara
                setText('X_HW_SIPExtend_SoftwarePara', SoftwarePara[0].X_HW_SIPExtend_SoftwarePara);
                //Echo Cancellation
                setCheck('X_HW_RTPExtend_EchoCancellationEnable', EchoCancellation[0].X_HW_RTPExtend_EchoCancellationEnable);
                //setCheck('ClipSendTimeEable',AllPhyInterfaceExtend[0].ClipSendDateTime);
                if (SoftwarePara[0].X_HW_SIPExtend_SoftwarePara == '') {
                    setSelect("SelectSoftwarePara", 0);
                    setDisplay("X_HW_SIPExtend_SoftwarePara", 0);
                } else {
                    setSelect("SelectSoftwarePara", 1);
                    setDisplay("X_HW_SIPExtend_SoftwarePara", 1);
                }
                document.getElementById('X_HW_SIPProfile_Body').title = ProfileBody_exp;
                document.getElementById('X_HW_SIPExtend_SoftwarePara').title = Software_exp;
                if (CodecList.length != 0) {
                    setSelect('PacketizationPeriod1', CodecList[selctLindex * 4 + 0].PacketizationPeriod);
                    setSelect('PacketizationPeriod2', CodecList[selctLindex * 4 + 1].PacketizationPeriod);
                    setSelect('PacketizationPeriod3', CodecList[selctLindex * 4 + 2].PacketizationPeriod);
                    setSelect('PacketizationPeriod4', CodecList[selctLindex * 4 + 3].PacketizationPeriod);

                    setText('Priority1', CodecList[selctLindex * 4 + 0].Priority);
                    setText('Priority2', CodecList[selctLindex * 4 + 1].Priority);
                    setText('Priority3', CodecList[selctLindex * 4 + 2].Priority);
                    setText('Priority4', CodecList[selctLindex * 4 + 3].Priority);

                    setCheck('EnableCodec1', CodecList[selctLindex * 4 + 0].Enable);
                    setCheck('EnableCodec2', CodecList[selctLindex * 4 + 1].Enable);
                    setCheck('EnableCodec3', CodecList[selctLindex * 4 + 2].Enable);
                    setCheck('EnableCodec4', CodecList[selctLindex * 4 + 3].Enable);
                } else {
                    return;
                }
                if (Line.length > 0) {
                    selectLine('record_0');
                    setDisplay('ConfigForm1', 1);
                } else {
                    selectLine('record_no');
                    setDisplay('ConfigForm1', 0);
                }
                setText('shorttimer', SipDigitMapPara[0].DigitMapShortTimer);
                setText('longtimer', SipDigitMapPara[0].DigitMapLongTimer);
                setPhyList("PhyList");
                setPhyInterfaceParams();
                setDspTemplatePara();
            }

            function Submit2() {
                var Form = new webSubmitForm();

                //Check Parameter
                var ulret = CheckForm1();
                if (ulret != true) {
                    return false;
                }

                Form.addParameter('x.Enable', getValue('FaxT38_Enable'));
                Form.addParameter('y.FaxNego', getValue('X_HW_FaxModem_FaxNego'));
                Form.addParameter('z.ProfileBody', getValue('X_HW_SIPProfile_Body'));
                if (getSelectVal("SelectSoftwarePara") == 0) {
                    Form.addParameter('a.SoftwareParameters', "");
                } else {
                    Form.addParameter('a.SoftwareParameters', getValue('X_HW_SIPExtend_SoftwarePara'));
                }
                Form.addParameter('b.EchoCancellationEnable', getCheckVal('X_HW_RTPExtend_EchoCancellationEnable'));

                Form.setAction('app-VoIPUser.asp?x=' + Profile1[0].Domain + '.FaxT38' + '&y=' + Profile1[0].Domain + '.X_HW_FaxModem' + '&z=' + Profile1[0].Domain + '.SIP' + '.X_HW_SIPProfile' + '&a=' + Profile1[0].Domain + '.SIP' + '.X_HW_SIPExtend' + '&b=' + Profile1[0].Domain + '.RTP' + '.X_HW_Extend' + '&RequestFile=html/voip/voipuser.asp');
                setDisable('btnApplySipServer2', 1);
                setDisable('cancelValue2', 1);

                Form.submit();
            }

            function CheckForm1() {
                var ProfileBody_ex = document.getElementById('X_HW_SIPProfile_Body');
                var SoftwarePara_ex = document.getElementById('X_HW_SIPExtend_SoftwarePara');
                var FaxT38_Enable = document.getElementById('FaxT38_Enable');
                var X_HW_FaxModem_FaxNego = document.getElementById('X_HW_FaxModem_FaxNego');

                if (FaxT38_Enable.value > 2) {
                    return false;
                }

                if (X_HW_FaxModem_FaxNego.value > 2) {
                    return false;
                }

                if (ProfileBody_ex.value.length > 8194) {
                    alert("控制点字符超长，长度必须小于8194。");
                    return false;
                }

                if (SoftwarePara_ex.value.length > 8194) {
                    alert("软件参数字符超长，长度必须小于8194。");
                    return false;
                }

                return true;
            }

            function CancelConfig2() {
                //Fax Transmode
                setSelect('FaxT38_Enable', VoiceTransmode[0].FaxT38_Enable);
                //Fax Switchmode
                setSelect('X_HW_FaxModem_FaxNego', VoiceTransswitch[0].X_HW_FaxModem_FaxNego);
                //Profile Body
                setText('X_HW_SIPProfile_Body', ProfileBody[0].X_HW_SIPProfile_Body);
                //SoftwarePara
                setText('X_HW_SIPExtend_SoftwarePara', SoftwarePara[0].X_HW_SIPExtend_SoftwarePara);
                //Echo Cancellation
                setCheck('X_HW_RTPExtend_EchoCancellationEnable', EchoCancellation[0].X_HW_RTPExtend_EchoCancellationEnable);

                if (SoftwarePara[0].X_HW_SIPExtend_SoftwarePara == '') {
                    setSelect("SelectSoftwarePara", 0);
                    setDisplay("X_HW_SIPExtend_SoftwarePara", 0);
                } else {
                    setSelect("SelectSoftwarePara", 1);
                    setDisplay("X_HW_SIPExtend_SoftwarePara", 1);
                }

            }

            function onChangeSoftware() {
                var index = getSelectVal("SelectSoftwarePara");
                if (index == 0) {
                    setDisplay("X_HW_SIPExtend_SoftwarePara", 0);
                } else {
                    setDisplay("X_HW_SIPExtend_SoftwarePara", 1);
                }
            }

            function AddSubmitParam(Form, type) {
                var PhyListLength = getElement('PhyList').options.length;
                var domain;

                Form.addParameter('a.PacketizationPeriod', getSelectVal('PacketizationPeriod1'));
                Form.addParameter('b.PacketizationPeriod', getSelectVal('PacketizationPeriod2'));
                Form.addParameter('c.PacketizationPeriod', getSelectVal('PacketizationPeriod3'));
                Form.addParameter('d.PacketizationPeriod', getSelectVal('PacketizationPeriod4'));

                Form.addParameter('a.Priority', getSelectVal('Priority1'));
                Form.addParameter('b.Priority', getSelectVal('Priority2'));
                Form.addParameter('c.Priority', getSelectVal('Priority3'));
                Form.addParameter('d.Priority', getSelectVal('Priority4'));

                Form.addParameter('a.Enable', getCheckVal('EnableCodec1'));
                Form.addParameter('b.Enable', getCheckVal('EnableCodec2'));
                Form.addParameter('c.Enable', getCheckVal('EnableCodec3'));
                Form.addParameter('d.Enable', getCheckVal('EnableCodec4'));

                Form.addParameter('e.X_HW_HotlineEnable', getCheckVal('HotlineEnable'));
                Form.addParameter('e.X_HW_HotlineNumber', getSelectVal('HotlineNumber'));
                Form.addParameter('e.X_HW_HotlineTimer', getSelectVal('HotlineTimer'));

                if (PhyListLength > 0) {
                    Form.addParameter('f.RingVoltage', getSelectVal('RingVoltage'));
                    Form.addParameter('f.RingDCVoltageOverlapped', getValue('RingDCVoltageOverlapped'));
                    Form.addParameter('f.UserDefineRingVoltage', getValue('UserDefineRingVoltage'));
                    Form.addParameter('f.SendGain', getSelectVal('SendGain'));
                    Form.addParameter('f.ReceiveGain', getSelectVal('ReceiveGain'));
                    Form.addParameter('f.HookFlashDownTime', getValue('HookFlashDownTime'));
                    Form.addParameter('f.HookFlashUpTime', getValue('HookFlashUpTime'));
                    Form.addParameter('f.OnhookConfirmTime', getValue('OnhookConfirmTime'));
                    Form.addParameter('f.Impedance', getSelectVal('Impedance'));
                    Form.addParameter('f.Current', getValue('Current'));
                    Form.addParameter('f.ClipFormat', getSelectVal('ClipFormat'));
                    Form.addParameter('f.FskTime', getValue('FskTime'));
                    Form.addParameter('f.ClipTransWhen', getSelectVal('ClipTransWhen'));
                    Form.addParameter('f.ReversePoleOnAnswer', getCheckVal('EnablePotsReversePole'));
                    Form.addParameter('f.ClipSendDateTime', getCheckVal('ClipSendDateTime'));
                    Form.addParameter('g.Enable', getCheckVal('EnableDspTemplate'));
                    Form.addParameter('g.WorkMode', getSelectVal('WorkMode'));

                    Form.addParameter('h.DigitMapShortTimer', getValue('shorttimer'));
                    Form.addParameter('h.DigitMapLongTimer', getValue('longtimer'));

                    domain = 'a=' + CodecList[selctLindex * 4 + 0].Domain + '&b=' + CodecList[selctLindex * 4 + 1].Domain + '&c=' + CodecList[selctLindex * 4 + 2].Domain + '&d=' + CodecList[selctLindex * 4 + 3].Domain + '&e=' + HotLineList[selctLindex].Domain + '&f=' + PhyInterfaceParams[parseInt(getSelectVal('PhyList')) - 1].Domain + '&g=' + DspTemplateParams[parseInt(getSelectVal('PhyList')) - 1].Domain + '&h=' + SipDigitMapPara[0].domain;

                } else {
                    domain = 'a=' + CodecList[selctLindex * 4 + 0].Domain + '&b=' + CodecList[selctLindex * 4 + 1].Domain + '&c=' + CodecList[selctLindex * 4 + 2].Domain + '&d=' + CodecList[selctLindex * 4 + 3].Domain + '&e=' + HotLineList[selctLindex].Domain;
                }

                if (selctIndex == -1) {
                    Form.setAction('add.cgi?x=InternetGatewayDevice.Services.VoiceService.1.VoiceProfile.1.Line.1.Codec.List.1' + '&RequestFile=html/voip/voipuser.asp');

                } else {
                    Form.setAction('app-VoIPUser.asp?' + domain + '&RequestFile=html/voip/voipuser.asp');

                }
                setDisable('btnApplyVoipUser3', 1);
                setDisable('cancelValue3', 1);
            }

            function setCtlDisplay(record) {
                //alert("selctLindex:"+selctLindex);
                // setText('codec1', CodecList[selctLindex * 4 + 0].Codecs);
                // setText('codec2', CodecList[selctLindex * 4 + 1].Codecs);
                // setText('codec3', CodecList[selctLindex * 4 + 2].Codecs);
                // setText('codec4', CodecList[selctLindex * 4 + 3].Codecs);
                setSelect('PacketizationPeriod1', CodecList[selctLindex * 4 + 0].PacketizationPeriod);
                setSelect('PacketizationPeriod2', CodecList[selctLindex * 4 + 1].PacketizationPeriod);
                setSelect('PacketizationPeriod3', CodecList[selctLindex * 4 + 2].PacketizationPeriod);
                setSelect('PacketizationPeriod4', CodecList[selctLindex * 4 + 3].PacketizationPeriod);

                setText('Priority1', CodecList[selctLindex * 4 + 0].Priority);
                setText('Priority2', CodecList[selctLindex * 4 + 1].Priority);
                setText('Priority3', CodecList[selctLindex * 4 + 2].Priority);
                setText('Priority4', CodecList[selctLindex * 4 + 3].Priority);

                setCheck('EnableCodec1', CodecList[selctLindex * 4 + 0].Enable);
                setCheck('EnableCodec2', CodecList[selctLindex * 4 + 1].Enable);
                setCheck('EnableCodec3', CodecList[selctLindex * 4 + 2].Enable);
                setCheck('EnableCodec4', CodecList[selctLindex * 4 + 3].Enable);

                setCheck('HotlineEnable', HotLineList[selctLindex].X_HW_HotlineEnable);
                setText('HotlineNumber', HotLineList[selctLindex].X_HW_HotlineNumber);
                setText('HotlineTimer', HotLineList[selctLindex].X_HW_HotlineTimer);
            }

            var g_Index = -1;

            function setControl(index) {
                var record;
                selctIndex = index;

                if (index == -1) {
                    if (Line.length >= ((AllPhyInterface.length - 1) * 17)) {
                        setDisplay('ConfigForm1', 0);
                        alert('用户数量过多。');
                        return false;
                    }

                    record = new stLine("", "", "Enabled", "1");
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
                    alert("You cannot delete the user because there is no user.");
                    return;
                }

                if (selctIndex == -1) {
                    alert("You cannot delete the user. Please save the new user first.");
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
                    alert('No user has been chosen. Please choose one.');
                    return;
                }

                if (confirm("Are you sure to delete the current user?") == false) {
                    return;
                }
                setDisable('btnApplySipUser', 1);
                setDisable('cancelValue', 1);
                removeInst('html/voip/voipuser.asp');
            }

            //Checkout
            function CheckForm(type) {
                var PhyList = document.getElementById('PhyList');
                var EnableCodec1 = document.getElementById('EnableCodec1').checked;
                var EnableCodec2 = document.getElementById('EnableCodec2').checked;
                var EnableCodec3 = document.getElementById('EnableCodec3').checked;
                var EnableCodec4 = document.getElementById('EnableCodec4').checked;

                var RingDCVoltageOverlapped = getValue('RingDCVoltageOverlapped');
                var UserDefineRingVoltage = getValue('UserDefineRingVoltage');
                var HookFlashDownTime = getValue('HookFlashDownTime');
                var HookFlashUpTime = getValue('HookFlashUpTime');
                var OnhookConfirmTime = getValue('OnhookConfirmTime');
                var Current = getValue('Current');
                var FskTime = getValue('FskTime');

                for (var i = 1; i < 5; i++) {
                    if (getValue('Priority' + i) > 100) {
                        alert("优先级不合法,请重新设置(范围为1-100)。");
                        return false;
                    }
                }
                for (var i = 1; i < 5; i++) {
                    for (var j = i + 1; j < 5; j++) {
                        if ((getSelectVal('Priority' + i)) == (getSelectVal('Priority' + j))) {
                            alert('优先级有冲突。');
                            return false;
                        }
                    }
                }

                if ((EnableCodec1 == false) && (EnableCodec2 == false) && (EnableCodec3 == false) && (EnableCodec4 == false)) {
                    alert('使能开关不能全部去使能。');
                    return false;
                }

                if ((getValue('HotlineTimer') != "") && ((false == isInteger(getValue('HotlineTimer'))) || (getValue('HotlineTimer') > 255) || (getValue('HotlineTimer') < 0))) {
                    alert('热线时间不合法,请重新设置(范围为0-255)。');
                    return false;
                }

                if (PhyList.options.length == 0) {
                    return true;
                }

                if (parseInt(RingDCVoltageOverlapped) < 0 || RingDCVoltageOverlapped > 25) {
                    alert('直流电压设置不合法，请重新设置(范围0-25)');
                    return false;
                }

                if (parseInt(UserDefineRingVoltage) < 0 || parseInt(UserDefineRingVoltage) > 74) {
                    alert('自定义振铃电压不合法，请重新设置(范围0-74)');
                    return false;
                }

                if (parseInt(HookFlashDownTime) < 0 || parseInt(HookFlashDownTime) > 1400) {
                    alert('拍叉下限不合法，请重新设置(范围0-1400)');
                    return false;
                }

                if (parseInt(HookFlashUpTime) < 0 || parseInt(HookFlashUpTime) > 1400) {
                    alert('拍叉上限不合法，请重新设置(范围0-1400)');
                    return false;
                }

                if (parseInt(OnhookConfirmTime) < 0 || parseInt(OnhookConfirmTime) > 1400) {
                    alert('挂机确认时间不合法，请重新设置(范围0-1400)');
                    return false;
                }

                if (((parseInt(HookFlashUpTime) < parseInt(HookFlashDownTime)) || (parseInt(OnhookConfirmTime) < parseInt(HookFlashUpTime))) && (parseInt(OnhookConfirmTime) != 0)) {
                    alert('拍叉上限必须大于拍叉下限，挂机确认时间必须大于拍叉上限');
                    return false;
                }

                if (parseInt(Current) < 16 || parseInt(Current) > 49) {
                    alert('馈电电流不合法，请重新设置(范围16-49)');
                    return false;
                }

                if (parseInt(FskTime) < 0 || parseInt(FskTime) > 2000) {
                    alert('发送FSK间隔不合法，请重新设置(范围0-2000)');
                    return false;
                }

                return true;
            }
            function ChangeEnable() {
                var Form = new webSubmitForm();
                var EchoEnable = getElement("X_HW_RTPExtend_EchoCancellationEnable");

                if (EchoEnable.checked == true) {
                    setDisplay('X_HW_RTPExtend_EchoCancellationEnable', 1);
                    Form.addParameter('b.EchoCancellationEnable', 1);
                } else {
                    setDisplay('X_HW_RTPExtend_EchoCancellationEnable', 1);
                    Form.addParameter('b.EchoCancellationEnable', 0);
                }

                Form.setAction('app-VoIPUser.asp?b=InternetGatewayDevice.Services.VoiceService.1.VoiceProfile.1.RTP.X_HW_Extend' + '&RequestFile=html/voip/voipuser.asp');
                Form.submit();
            }

            function CancelConfig() {
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
                    setPhyInterfaceParams();
                }
            }

            function setPhyList(objname) {
                var PhyPortList = Line[selctLindex].PhyReferenceList;
                var selectObj = getElement(objname);
                var tempList;

                removeAllOption(objname);

                if (PhyPortList.length == 0) {
                    setDisplay("DivPhyParameter", 0);
                    return;
                }

                tempList = PhyPortList.split(',');
                if (tempList.length > 1) {
                    setDisable('PhyList', 0);
                } else {
                    setDisable('PhyList', 1);
                }
                setDisplay("DivPhyParameter", 1);
                if (selectObj != null) {
                    for (var i = 0; i < tempList.length; i++) {
                        var opt = document.createElement("option");
                        opt.setAttribute("value", tempList[i]);
                        opt.innerHTML = tempList[i];
                        selectObj.appendChild(opt);
                    }
                }

                if (selectObj.options.length > 0) {
                    try {
                        selectObj.options[0].selected = true;
                    } catch(ex) {

}
                }

            }

            function setRingVoltagePara() {
                var RingVoltage = getElement("RingVoltage");

                if (3 == RingVoltage.selectedIndex) {
                    setDisplay("TrUserDefineRingVoltage", 1);
                } else {
                    setDisplay("TrUserDefineRingVoltage", 0);
                }
            }

            function setDspTemplatePara() {
                var PhyID;
                var selectObj = getElement("PhyList");
                if (selectObj.options.length == 0) {
                    return;
                }
                PhyID = parseInt((getSelectVal("PhyList") - 1));
                if (1 == getCheckVal("EnableDspTemplate")) {
                    setDisplay("TrWorkMode", 1);
                } else {
                    setDisplay("TrWorkMode", 0);
                }
                setSelect("WorkMode", DspTemplateParams[PhyID].WorkMode);
            }

            function setFskClipPara() {
                var clipFormat = getSelectVal("ClipFormat");
                if (("Sdmf-fsk" == clipFormat) || ("Mdmf-fsk" == clipFormat)) {
                    setDisplay("TrFskTime", 1);
                } else {
                    setDisplay("TrFskTime", 0);
                }
            }

            function setPhyInterfaceParams() {
                var PhyID;
                var selectObj = getElement("PhyList");
                if (selectObj.options.length == 0) {
                    return;
                }

                PhyID = parseInt(getSelectVal("PhyList")) - 1;
                setSelect("RingVoltage", PhyInterfaceParams[PhyID].RingVoltage);
                setText("UserDefineRingVoltage", PhyInterfaceParams[PhyID].UserDefineRingVoltage);
                setText("RingDCVoltageOverlapped", PhyInterfaceParams[PhyID].RingDCVoltageOverlapped);
                setSelect("SendGain", PhyInterfaceParams[PhyID].SendGain);
                setSelect("ReceiveGain", PhyInterfaceParams[PhyID].ReceiveGain);
                setText("HookFlashDownTime", PhyInterfaceParams[PhyID].HookFlashDownTime);
                setText("HookFlashUpTime", PhyInterfaceParams[PhyID].HookFlashUpTime);
                setText("OnhookConfirmTime", PhyInterfaceParams[PhyID].OnhookConfirmTime);
                setSelect("Impedance", PhyInterfaceParams[PhyID].Impedance);
                setText("Current", PhyInterfaceParams[PhyID].Current);
                setSelect("ClipFormat", PhyInterfaceParams[PhyID].ClipFormat);
                setText("FskTime", PhyInterfaceParams[PhyID].FskTime);
                setSelect("ClipTransWhen", PhyInterfaceParams[PhyID].ClipTransWhen);
                setCheck("EnableDspTemplate", DspTemplateParams[PhyID].Enable);
                setRingVoltagePara();
                setDspTemplatePara();
                setCheck("EnablePotsReversePole", PhyInterfaceParams[PhyID].ReversePoleOnAnswer);
                setCheck('ClipSendDateTime', PhyInterfaceParams[PhyID].ClipSendDateTime);
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
                        <TABLE height=117 cellSpacing=0 cellPadding=0 width=808 background=/img/framelogo.jpg
                        border=0>
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
                                    <TD width=434 bgColor=#427594 height=33 class=CucBgColorTD_1>
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
                                    <TD width=211 bgColor=#ef8218 height=33 class=CucBgColorTD_1>
                                        <P class=GatewayType align=center>
                                            型号:
                                            <SCRIPT language=javascript>
                                                document.write(top.ModelName);
                                            </SCRIPT>
                                        </P>
                                    </TD>
                                </TR>
                                <TR>
                                    <TD id=MenuArea_L1 vAlign=bottom bgColor=#ef8218 colSpan=2 height=43 class=CucBgColorTD_1>
                                        &nbsp;
                                    </TD>
                                </TR>
                                <TR>
                                    <TD id=MenuArea_L2 bgColor=#427594 colSpan=2 height=24 class=CucBgColorTD_3>
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
                                    <TD class=Item_L2 width=157 bgColor=#e7e7e7 height=30>
                                        <P>
                                            <a href='/cgi-bin/app-VoIP.asp' class=Item_L2 style="font-weight:normal;text-decoration: none;">
                                                宽带电话基本设置
                                            </a>
                                        </P>
                                    </TD>
                                    <TD width=7 bgColor=#e7e7e7>
                                        　
                                    </TD>
                                    <TD width=564>
                                        　
                                    </TD>
                                    <TD vAlign=top class=table1_help background=/img/panel4.gif rowSpan=8>
                                        <TABLE cellSpacing=0 cellPadding=20 width="100%" border=0>
                                            <TBODY>
                                                <TR>
                                                    <TD>
                                                        <A href="/cgi-bin/help_content.asp#宽带电话设置" target=_blank>
                                                            <IMG height=34 src="/img/help.gif" width=40 border=0>
                                                        </A>
                                                    </TD>
                                                </TR>
                                            </TBODY>
                                        </TABLE>
                                    </TD>
                                </TR>
                                <TR>
                                    <TD class=Item_L1 width=157 bgColor=#ef8218 height=30>
                                        <P>
                                            <a href='/cgi-bin/app-VoIPUser.asp' class=Item_L1 style="text-decoration: none;">
                                                宽带电话高级设置
                                            </a>
                                        </P>
                                    </TD>
                                    <TD width=7 bgColor=#ef8218 class=CucBgColorTD_2>
                                    </TD>
                                    <TD>
                                            <TABLE cellSpacing=0 cellPadding=0 width="100%" border=0>
                                            <TBODY>
                                                <TR>
                                                    <TD width=10>
                                                        &nbsp;
                                                    </TD>
                                                    <TD>
                                                        <!-- start -->
                                                        <TABLE>
                                                            <TBODY>
                                                                <TR>
                                                                    <TD class=table1_head colSpan=11>
                                                                     接口高级参数
                                                                    </TD>
                                                                </TR>
                                                             </TBODY>
                                                        </TABLE>                                            
                                                        <!-- end -->

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
                                    <TD class=Item_L2 vAlign=top width=157 bgColor=#e7e7e7 height=30>
                                    </TD>
                                    <TD width=7 background=/img/panel3.gif>
                                    </TD>
                                    <TD>
                                        <TABLE cellSpacing=0 cellPadding=0 width="100%" border=0>
                                            <TBODY>
                                                <TR>
                                                    <TD width=10>
                                                        &nbsp;
                                                    </TD>
                                                    <TD>
                                                        <!-- start -->
                                                        

                                                        <TABLE class=table1_bg border=0 cellSpacing=1 cellPadding=0 width="100%" style='margin-top:0px;'>
                                                            <TBODY>
                                                                <TR>
                                                                    <TD class=table1_left width="20%">
                                                                        使能回声抑制：
                                                                    </TD>
                                                                    <TD class=table1_right width="80%" colSpan=10>
                                                                        <INPUT id=X_HW_RTPExtend_EchoCancellationEnable value=1 type=checkbox
                                                                        name=X_HW_RTPExtend_EchoCancellationEnable>
                                                                    </TD>
                                                                </TR>
                                                                <TR>
                                                                    <TD class=table1_left>
                                                                        传真模式：
                                                                    </TD>
                                                                    <TD class=table1_right colSpan=10>
                                                                        <SELECT style="WIDTH: 150px" id=FaxT38_Enable size=1 name=FaxT38_Enable>
                                                                            <OPTION selected value=0>
                                                                                透传
                                                                            </OPTION>
                                                                            <OPTION value=1>
                                                                                T.38
                                                                            </OPTION>
                                                                        </SELECT>
                                                                    </TD>
                                                                </TR>
                                                                <TR>
                                                                    <TD class=table1_left >
                                                                        传真切换模式：
                                                                    </TD>
                                                                    <TD class=table1_right colSpan=10>
                                                                        <SELECT style="WIDTH: 150px" id=X_HW_FaxModem_FaxNego size=1 name=X_HW_FaxModem_FaxNego>
                                                                            <OPTION value=0>
                                                                                自切换
                                                                            </OPTION>
                                                                            <OPTION selected value=1>
                                                                                协商
                                                                            </OPTION>
                                                                        </SELECT>
                                                                    </TD>
                                                                </TR>
                                                                <TR>
                                                                    <TD class=table1_left>
                                                                        Profile参数:
                                                                    </TD>
                                                                    <TD class=table1_right colSpan=10>
                                                                        <TEXTAREA style="WIDTH: 383px" id=X_HW_SIPProfile_Body rows=3 cols=67
                                                                        name=X_HW_SIPProfile_Body>
                                                                            0=0;1=2;2=1;3=0;4=0;5=0;6=0;7=0;8=0;9=0;10=0;11=1;12=0;13=0;14=0;15=0;16=0;17=0;18=0;19=0;20=0;21=0;22=0;23=0;24=0;25=0;26=0;27=0;28=0;29=0;30=0;31=0;32=0;33=0;34=0;35=0;36=0;37=0;38=0;39=0;40=1;41=4;42=0;43=0;44=0;45=0;46=0;47=2000;48=0;49=60;50=0;51=0;52=0;53=0;54=0;55=1;56=1;57=7;58=3;59=0;60=0;61=0;62=0;63=0;64=4;65=400;66=1;67=400;68=1;69=400;70=1;71=400;72=1;73=400;74=0;75=20;76=0;77=0;78=0;79=1;80=1;81=0;82=0;83=0;84=1;85=0;86=180;87=2;88=0;89=0;90=0;91=0;92=0;93=1;94=1;95=40;96=1;97=2;98=0;99=0;100=0;101=0;102=2000;103=0;104=60;105=60;106=0;107=0;108=0;109=0;110=500;111=0;112=0;113=1;114=0;115=0;116=0;117=65534;118=65534;119=65534;
                                                                        </TEXTAREA>
                                                                    </TD>
                                                                </TR>
                                                                <TR>
                                                                    <TD class=table1_left>
                                                                        软件参数
                                                                    </TD>
                                                                    <TD class=table1_right colSpan=10>
                                                                        <SELECT style="WIDTH: 100px" id=SelectSoftwarePara onchange=onChangeSoftware()
                                                                        name=SelectSoftwarePara>
                                                                            <OPTION selected value=0>
                                                                                默认
                                                                            </OPTION>
                                                                            <OPTION value=1>
                                                                                自定义
                                                                            </OPTION>
                                                                        </SELECT>
                                                                        <INPUT style="WIDTH: 280px" id=X_HW_SIPExtend_SoftwarePara type=text name=X_HW_SIPExtend_SoftwarePara>
                                                                    </TD>
                                                                </TR>
                                                            </TBODY>
                                                        </TABLE>
                                                        <TABLE border=0 cellSpacing=1 cellPadding=0 width="100%">
                                                            <TBODY>
                                                                <TR>
                                                                    <TD class=table1_submit width="20%">
                                                                    </TD>
                                                                    <TD class=table1_submit width="80%" align=left>
                                                                        <INPUT id=btnApplySipServer2 class=submit onclick=Submit2() value=" 应用 "
                                                                        type=button name=btnApplySipServer2>
                                                                        <INPUT id=cancelValue2 class=submit onclick=CancelConfig2(); value=" 取消 "
                                                                        type=button name=cancelValue2>
                                                                    </TD>
                                                                </TR>
                                                            </TBODY>
                                                        </TABLE>
                                                        <TABLE>
                                                            <TBODY>
                                                                <TR>
                                                                    <TD class=table1_head>
                                                                        <FONT face=Arial>
                                                                            用户高级参数
                                                                        </FONT>
                                                                    </TD>
                                                                </TR>
                                                            </TBODY>
                                                        </TABLE>
                                                        <TABLE id=voipUserTable class=table1_bg border=0 cellSpacing=1 cellPadding=0
                                                        width="100%">
                                                            <TBODY>
                                                                <TR class=table1_title>
                                                                    <TD width="14%" align=middle>
                                                                        编号
                                                                    </TD>
                                                                    <TD width="56%">
                                                                        用户名（电话号码）
                                                                    </TD>
                                                                    <TD width="30%">
                                                                        关联端口号
                                                                    </TD>
                                                                </TR>
                                                                <SCRIPT language=JavaScript type=text/javascript>
                                                                    if (Line.length == 0) {
                                                                        selctLindex = 0;
                                                                        document.write('<tr id="record_no"' + ' class="trTabContent" onclick="selectLine(this.id);">');
                                                                        document.write('<td align="center">----</td>');
                                                                        document.write('<td align="center">----</td>');
                                                                        document.write('<td align="center">----</td>');
                                                                        document.write('</tr>');
                                                                    } else {
                                                                        for (var i = 0; i < Line.length; i++) {
                                                                            var html = '';
                                                                            if (i % 2 == 0) {
                                                                                html += '<tr id="record_' + i + '" class="table1_01" ' + 'onclick="SelectLineRecord(this.id);">';
                                                                            } else {
                                                                                html += '<tr id="record_' + i + '" class="table1_02" ' + 'onclick="SelectLineRecord(this.id);">';
                                                                            }

                                                                            html += '<td align="left">' + (i + 1) + '</td>';
                                                                            if (AllLineSIP[i].URI != "") {
                                                                                html += '<td align="left">' + AllLineSIP[i].URI + '&nbsp;</td>';
                                                                            } else if (Line[i].DirectoryNumber != "") {
                                                                                html += '<td align="left">' + Line[i].DirectoryNumber + '&nbsp;</td>';
                                                                            } else if (Line[i].AuthUserName != "") {
                                                                                html += '<td align="left">' + Line[i].AuthUserName + '&nbsp;</td>';
                                                                            } else {
                                                                                html += '<td align="center">' + '--' + '&nbsp;</td>';
                                                                            }
                                                                            html += '<td align="left">' + Line[i].PhyReferenceList + '&nbsp;</td>';
                                                                            html += '</tr>';
                                                                            document.write(html);
                                                                        }
                                                                    }
                                                                </SCRIPT>
                                                            </TBODY>
                                                        </TABLE>
                                                        <TABLE border=0 cellSpacing=0 cellPadding=0 width="100%" height=15>
                                                        </TABLE>
                                                        <DIV id=ConfigForm1>
                                                            <TABLE id=CodecInfo class=table1_bg border=0 cellSpacing=1 cellPadding=0
                                                            width="100%">
                                                                <TBODY>
                                                                    <TR class=table1_title align=left>
                                                                        <TD class=table1_title rowSpan=5 width="14%" align=middle>
                                                                            编解码
                                                                        </TD>
                                                                        <TD width="12%">
                                                                            使能
                                                                        </TD>
                                                                        <TD width="30%">
                                                                            编解码方式
                                                                        </TD>
                                                                        <TD width="19%">
                                                                            打包时长(ms)
                                                                        </TD>
                                                                        <TD width="25%">
                                                                            优先级
                                                                        </TD>
                                                                    </TR>
                                                                    <TR class=table1_left align=left>
                                                                        <TD align=middle>
                                                                            <INPUT id=EnableCodec1 onclick="" value=1 type=checkbox name=EnableCodec1>
                                                                        </TD>
                                                                        <SCRIPT language=javascript type=text/javascript>
                                                                            if (CodecList.length != 0) {
                                                                                document.write("<td>" + CodecList[0].Codecs + "</td>");
                                                                            } else {
                                                                                document.write("<td>" + '---- ' + "</td>");
                                                                            }
                                                                        </SCRIPT>
                                                                        <TD>
                                                                            <SELECT style="WIDTH: 70px" id=PacketizationPeriod1 name=PacketizationPeriod1>
                                                                                <OPTION selected value=10>
                                                                                    10
                                                                                </OPTION>
                                                                                <OPTION value=20>
                                                                                    20
                                                                                </OPTION>
                                                                                <OPTION value=30>
                                                                                    30
                                                                                </OPTION>
                                                                            </SELECT>
                                                                        </TD>
                                                                        <TD>
                                                                            <INPUT style="WIDTH: 50px" id=Priority1 maxLength=256 type=text name=Priority1>
                                                                            <SPAN class=gray>
                                                                                (1-100)
                                                                            </SPAN>
                                                                        </TD>
                                                                    </TR>
                                                                    <TR class=table1_left>
                                                                        <TD align=middle>
                                                                            <INPUT id=EnableCodec2 onclick="" value=1 type=checkbox name=EnableCodec2>
                                                                        </TD>
                                                                        <SCRIPT language=javascript type=text/javascript>
                                                                            if (CodecList.length != 0) {
                                                                                document.write("<td>" + CodecList[1].Codecs + "</td>");
                                                                            } else {
                                                                                document.write("<td>" + '---- ' + "</td>");
                                                                            }
                                                                        </SCRIPT>
                                                                        <TD>
                                                                            <SELECT style="WIDTH: 70px" id=PacketizationPeriod2 name=PacketizationPeriod2>
                                                                                <OPTION selected value=10>
                                                                                    10
                                                                                </OPTION>
                                                                                <OPTION value=20>
                                                                                    20
                                                                                </OPTION>
                                                                                <OPTION value=30>
                                                                                    30
                                                                                </OPTION>
                                                                            </SELECT>
                                                                        </TD>
                                                                        <TD>
                                                                            <INPUT style="WIDTH: 50px" id=Priority2 maxLength=256 type=text name=Priority2>
                                                                            <SPAN class=gray>
                                                                                (1-100)
                                                                            </SPAN>
                                                                        </TD>
                                                                    </TR>
                                                                    <TR class=table1_left>
                                                                        <TD align=middle>
                                                                            <INPUT id=EnableCodec3 onclick="" value=1 type=checkbox name=EnableCodec3>
                                                                        </TD>
                                                                        <SCRIPT language=javascript type=text/javascript>
                                                                            if (CodecList.length != 0) {
                                                                                document.write("<td>" + CodecList[2].Codecs + "</td>");
                                                                            } else {
                                                                                document.write("<td>" + '---- ' + "</td>");
                                                                            }
                                                                        </SCRIPT>
                                                                        <TD>
                                                                            <SELECT style="WIDTH: 70px" id=PacketizationPeriod3 name=PacketizationPeriod3>
                                                                                <OPTION selected value=10>
                                                                                    10
                                                                                </OPTION>
                                                                                <OPTION value=20>
                                                                                    20
                                                                                </OPTION>
                                                                                <OPTION value=30>
                                                                                    30
                                                                                </OPTION>
                                                                            </SELECT>
                                                                        </TD>
                                                                        <TD>
                                                                            <INPUT style="WIDTH: 50px" id=Priority3 maxLength=256 type=text name=Priority3>
                                                                            <SPAN class=gray>
                                                                                (1-100)
                                                                            </SPAN>
                                                                        </TD>
                                                                    </TR>
                                                                    <TR class=table1_left>
                                                                        <TD align=middle>
                                                                            <INPUT id=EnableCodec4 onclick="" value=1 type=checkbox name=EnableCodec4>
                                                                        </TD>
                                                                        <SCRIPT language=javascript type=text/javascript>
                                                                            if (CodecList.length != 0) {
                                                                                document.write("<td>" + CodecList[3].Codecs + "</td>");
                                                                            } else {
                                                                                document.write("<td>" + '---- ' + "</td>");
                                                                            }
                                                                        </SCRIPT>
                                                                        <TD>
                                                                            <SELECT style="WIDTH: 70px" id=PacketizationPeriod4 name=PacketizationPeriod4>
                                                                                <OPTION selected value=10>
                                                                                    10
                                                                                </OPTION>
                                                                                <OPTION value=20>
                                                                                    20
                                                                                </OPTION>
                                                                                <OPTION value=30>
                                                                                    30
                                                                                </OPTION>
                                                                            </SELECT>
                                                                        </TD>
                                                                        <TD>
                                                                            <INPUT style="WIDTH: 50px" id=Priority4 maxLength=256 type=text name=Priority4>
                                                                            <SPAN class=gray>
                                                                                (1-100)
                                                                            </SPAN>
                                                                        </TD>
                                                                    </TR>
                                                                </TBODY>
                                                            </TABLE>
                                                            <TABLE class=table1_bg border=0 cellSpacing=1 cellPadding=0 width="100%">
                                                                <TBODY>
                                                                    <TR>
                                                                        <TD style="TEXT-ALIGN: center" class=table1_left rowSpan=3>
                                                                            热线
                                                                        </TD>
                                                                        <TD class=table1_right align=left>
                                                                            使能:
                                                                        </TD>
                                                                        <TD class=table1_right colSpan=2 align=left>
                                                                            <INPUT id=HotlineEnable value=1 type=checkbox name=HotlineEnable>
                                                                        </TD>
                                                                    </TR>
                                                                    <TR>
                                                                        <TD class=table1_right align=left>
                                                                            号码:
                                                                        </TD>
                                                                        <TD class=table1_right colSpan=2 align=left>
                                                                            <INPUT style="WIDTH: 150px" id=HotlineNumber maxLength=32 type=text name=HotlineNumber>
                                                                            <SPAN class=gray>
                                                                                (长度0-32)
                                                                            </SPAN>
                                                                        </TD>
                                                                    </TR>
                                                                    <TR>
                                                                        <TD class=table1_right width="13%" align=left>
                                                                            延迟时长:
                                                                        </TD>
                                                                        <TD class=table1_right width="73%" colSpan=2 align=left>
                                                                            <INPUT style="WIDTH: 150px" id=HotlineTimer type=text name=HotlineTimer>
                                                                            <SPAN class=gray>
                                                                                (单位:秒)(0~255)
                                                                            </SPAN>
                                                                        </TD>
                                                                    </TR>
                                                                </TBODY>
                                                            </TABLE>
                                                            <TABLE class=table1_bg border=0 cellSpacing=1 cellPadding=0 width="100%" style='margin-top:0px;'>
                                                            </TABLE>
                                                            <DIV id=DivPhyParameter>
                                                                <TABLE style="WIDTH: 100%" border=0 cellSpacing=1 cellPadding=0>
                                                                    <TBODY>
                                                                        <TR>
                                                                            <TD class=table1_head colSpan=4>
                                                                                物理端口参数
                                                                            </TD>
                                                                        </TR>
                                                                    </TBODY>
                                                                </TABLE>
                                                                <TABLE class=table1_bg border=0 cellSpacing=1 cellPadding=0 width="100%">
                                                                    <TBODY>
                                                                        <TR>
                                                                            <TD style="WIDTH: 20%" class=table1_left>
                                                                                端口编号:
                                                                            </TD>
                                                                            <TD class=table1_right colSpan=3>
                                                                                <SELECT style="WIDTH: 125px" id=PhyList>
                                                                                </SELECT>
                                                                            </TD>
                                                                        </TR>
                                                                        <TR>
                                                                            <TD style="WIDTH: 20%" class=table1_left>
                                                                                振铃电压:
                                                                            </TD>
                                                                            <TD class=table1_right colSpan=3>
                                                                                <SELECT style="WIDTH: 125px" id=RingVoltage onchange=setRingVoltagePara()
                                                                                size=1>
                                                                                    <OPTION selected value=0>
                                                                                        74Vrms
                                                                                    </OPTION>
                                                                                    <OPTION value=1>
                                                                                        65Vrms
                                                                                    </OPTION>
                                                                                    <OPTION value=2>
                                                                                        50Vrms
                                                                                    </OPTION>
                                                                                    <OPTION value=3>
                                                                                        自定义
                                                                                    </OPTION>
                                                                                </SELECT>
                                                                            </TD>
                                                                        </TR>
                                                                        <TR id=TrUserDefineRingVoltage>
                                                                            <TD style="WIDTH: 20%" class=table1_left>
                                                                                自定义振铃电压:
                                                                            </TD>
                                                                            <TD class=table1_right colSpan=3>
                                                                                <INPUT style="WIDTH: 125px" id=UserDefineRingVoltage type=text>
                                                                                &nbsp; (单位：Vrms)
                                                                            </TD>
                                                                        </TR>
                                                                        <TR>
                                                                            <TD style="WIDTH: 20%" class=table1_left>
                                                                                直流电压:
                                                                            </TD>
                                                                            <TD class=table1_right colSpan=3>
                                                                                <INPUT style="WIDTH: 125px" id=RingDCVoltageOverlapped type=text>
                                                                                &nbsp; (单位：V)
                                                                            </TD>
                                                                        </TR>
                                                                        <TR>
                                                                            <TD style="WIDTH: 20%" class=table1_left>
                                                                                发送增益:
                                                                            </TD>
                                                                            <TD class=table1_right colSpan=3>
                                                                                <SELECT style="WIDTH: 125px" id=SendGain>
                                                                                    <OPTION value=-6db>
                                                                                        -6db
                                                                                    </OPTION>
                                                                                    <OPTION value=-5.5db>
                                                                                        -5.5db
                                                                                    </OPTION>
                                                                                    <OPTION value=-5db>
                                                                                        -5db
                                                                                    </OPTION>
                                                                                    <OPTION value=-4.5db>
                                                                                        -4.5db
                                                                                    </OPTION>
                                                                                    <OPTION value=-4db>
                                                                                        -4db
                                                                                    </OPTION>
                                                                                    <OPTION value=-3.5db>
                                                                                        -3.5db
                                                                                    </OPTION>
                                                                                    <OPTION value=-3db>
                                                                                        -3db
                                                                                    </OPTION>
                                                                                    <OPTION value=-2.5db>
                                                                                        -2.5db
                                                                                    </OPTION>
                                                                                    <OPTION value=-2db>
                                                                                        -2db
                                                                                    </OPTION>
                                                                                    <OPTION value=-1.5db>
                                                                                        -1.5db
                                                                                    </OPTION>
                                                                                    <OPTION value=-1db>
                                                                                        -1db
                                                                                    </OPTION>
                                                                                    <OPTION value=-0.5db>
                                                                                        -0.5db
                                                                                    </OPTION>
                                                                                    <OPTION selected value=0db>
                                                                                        0db
                                                                                    </OPTION>
                                                                                    <OPTION value=0.5db>
                                                                                        0.5db
                                                                                    </OPTION>
                                                                                    <OPTION value=1db>
                                                                                        1db
                                                                                    </OPTION>
                                                                                    <OPTION value=1.5db>
                                                                                        1.5db
                                                                                    </OPTION>
                                                                                    <OPTION value=2db>
                                                                                        2db
                                                                                    </OPTION>
                                                                                    <OPTION value=2.5db>
                                                                                        2.5db
                                                                                    </OPTION>
                                                                                    <OPTION value=3db>
                                                                                        3db
                                                                                    </OPTION>
                                                                                    <OPTION value=3.5db>
                                                                                        3.5db
                                                                                    </OPTION>
                                                                                    <OPTION value=4db>
                                                                                        4db
                                                                                    </OPTION>
                                                                                    <OPTION value=4.5db>
                                                                                        4.5db
                                                                                    </OPTION>
                                                                                    <OPTION value=5db>
                                                                                        5db
                                                                                    </OPTION>
                                                                                </SELECT>
                                                                            </TD>
                                                                        </TR>
                                                                        <TR>
                                                                            <TD style="WIDTH: 20%" class=table1_left>
                                                                                接收增益:
                                                                            </TD>
                                                                            <TD class=table1_right colSpan=3>
                                                                                <SELECT style="WIDTH: 125px" id=ReceiveGain>
                                                                                    <OPTION value=0db>
                                                                                        0db
                                                                                    </OPTION>
                                                                                    <OPTION value=-0.5db>
                                                                                        -0.5db
                                                                                    </OPTION>
                                                                                    <OPTION value=-1db>
                                                                                        -1db
                                                                                    </OPTION>
                                                                                    <OPTION value=-1.5db>
                                                                                        -1.5db
                                                                                    </OPTION>
                                                                                    <OPTION value=-2db>
                                                                                        -2db
                                                                                    </OPTION>
                                                                                    <OPTION value=-2.5db>
                                                                                        -2.5db
                                                                                    </OPTION>
                                                                                    <OPTION value=-3db>
                                                                                        -3db
                                                                                    </OPTION>
                                                                                    <OPTION selected value=-3.5db>
                                                                                        -3.5db
                                                                                    </OPTION>
                                                                                    <OPTION value=-4db>
                                                                                        -4db
                                                                                    </OPTION>
                                                                                    <OPTION value=-4.5db>
                                                                                        -4.5db
                                                                                    </OPTION>
                                                                                    <OPTION value=-5db>
                                                                                        -5db
                                                                                    </OPTION>
                                                                                    <OPTION value=-5.5db>
                                                                                        -5.5db
                                                                                    </OPTION>
                                                                                    <OPTION value=-6db>
                                                                                        -6db
                                                                                    </OPTION>
                                                                                    <OPTION value=-6.5db>
                                                                                        -6.5db
                                                                                    </OPTION>
                                                                                    <OPTION value=-7db>
                                                                                        -7db
                                                                                    </OPTION>
                                                                                    <OPTION value=-7.5db>
                                                                                        -7.5db
                                                                                    </OPTION>
                                                                                    <OPTION value=-8db>
                                                                                        -8db
                                                                                    </OPTION>
                                                                                    <OPTION value=-8.5db>
                                                                                        -8.5db
                                                                                    </OPTION>
                                                                                    <OPTION value=-9db>
                                                                                        -9db
                                                                                    </OPTION>
                                                                                    <OPTION value=-9.5db>
                                                                                        -9.5db
                                                                                    </OPTION>
                                                                                    <OPTION value=-10db>
                                                                                        -10db
                                                                                    </OPTION>
                                                                                    <OPTION value=-10.5db>
                                                                                        -10.5db
                                                                                    </OPTION>
                                                                                    <OPTION value=-11db>
                                                                                        -11db
                                                                                    </OPTION>
                                                                                    <OPTION value=-11.5db>
                                                                                        -11.5db
                                                                                    </OPTION>
                                                                                    <OPTION value=-12db>
                                                                                        -12db
                                                                                    </OPTION>
                                                                                </SELECT>
                                                                            </TD>
                                                                        </TR>
                                                                        <TR>
                                                                            <TD style="WIDTH: 20%" class=table1_left>
                                                                                拍叉下限:
                                                                            </TD>
                                                                            <TD class=table1_right colSpan=3>
                                                                                <INPUT style="WIDTH: 125px" id=HookFlashDownTime type=text>
                                                                                &nbsp;（单位：毫秒）
                                                                            </TD>
                                                                        </TR>
                                                                        <TR>
                                                                            <TD style="WIDTH: 20%" class=table1_left>
                                                                                拍叉上限:
                                                                            </TD>
                                                                            <TD class=table1_right colSpan=3>
                                                                                <INPUT style="WIDTH: 125px" id=HookFlashUpTime type=text>
                                                                                &nbsp;（单位：毫秒）
                                                                            </TD>
                                                                        </TR>
                                                                        <TR>
                                                                            <TD style="WIDTH: 20%" class=table1_left>
                                                                                挂机确认时间:
                                                                            </TD>
                                                                            <TD class=table1_right colSpan=3>
                                                                                <INPUT style="WIDTH: 125px" id=OnhookConfirmTime type=text>
                                                                                &nbsp;（单位：毫秒）
                                                                            </TD>
                                                                        </TR>
                                                                        <TR>
                                                                            <TD style="WIDTH: 20%" class=table1_left>
                                                                                阻抗:
                                                                            </TD>
                                                                            <TD class=table1_right colSpan=3>
                                                                                &nbsp;
                                                                            </TD>
                                                                        </TR>
                                                                        <TR>
                                                                            <TD class=table1_right colSpan=4 style='text-align:right' >
                                                                                <SELECT id=Impedance >
                                                                                    <OPTION value=0>
                                                                                        中国局用机(200+680|| 100nf)
                                                                                    </OPTION>
                                                                                    <OPTION value=1>
                                                                                        中国用户机(200+560|| 100nf)
                                                                                    </OPTION>
                                                                                    <OPTION selected value=2>
                                                                                        600Ω接口
                                                                                    </OPTION>
                                                                                    <OPTION value=3>
                                                                                        俄罗斯接口(150+510|| 47nf)
                                                                                    </OPTION>
                                                                                    <OPTION value=4>
                                                                                        (220+820|| 115nf )
                                                                                    </OPTION>
                                                                                    <OPTION value=5>
                                                                                        (220+820|| 120nf )
                                                                                    </OPTION>
                                                                                    <OPTION value=6>
                                                                                        900Ω接口
                                                                                    </OPTION>
                                                                                    <OPTION value=7>
                                                                                        巴西接口(800|| 50nf)
                                                                                    </OPTION>
                                                                                    <OPTION value=8>
                                                                                        BT0(Zin=87+1052||228nF+229||28.4nF, Zload=93+615|| 471nF+179||495nF+244||32nF)
                                                                                    </OPTION>
                                                                                    <OPTION value=9>
                                                                                        HK_BT3(Zin=370+620|| 310nf,Zload=600)
                                                                                    </OPTION>
                                                                                    <OPTION value=10>
                                                                                        HK_BT5(Zin=270+264|| 357nf+1434|| 265nf,Zload=600)
                                                                                    </OPTION>
                                                                                    <OPTION value=11>
                                                                                        BT1(BT0 without AGC)
                                                                                    </OPTION>
                                                                                    <OPTION value=12>
                                                                                        BT2(Zin=87+1052||228nF+229||28.4nF, Zload=270+264|| 357nF+1434||265nF)
                                                                                    </OPTION>
                                                                                    <OPTION value=13>
                                                                                        BT3(Zin=87+1052||228nF+229||28.4nF, Zload=164+162|| 363nF+1227||350nF)
                                                                                    </OPTION>
                                                                                    <OPTION value=14>
                                                                                        欧洲通用(Zin= 270+750|| 150nf)
                                                                                    </OPTION>
                                                                                    <OPTION value=15>
                                                                                        新西兰(Zin= 370+620|| 310nf )
                                                                                    </OPTION>
                                                                                </SELECT>
                                                                            </TD>
                                                                        </TR>
                                                                        <TR>
                                                                            <TD style="WIDTH: 20%" class=table1_left>
                                                                                馈电电流:
                                                                            </TD>
                                                                            <TD class=table1_right colSpan=3>
                                                                                <INPUT style="WIDTH: 125px" id=Current type=text>
                                                                                &nbsp;（单位：毫安）
                                                                            </TD>
                                                                        </TR>
                                                                        <TR>
                                                                            <TD style="WIDTH: 20%" class=table1_left>
                                                                                来电显示格式:
                                                                            </TD>
                                                                            <TD class=table1_right colSpan=3>
                                                                                <SELECT style="WIDTH: 125px" id=ClipFormat onchange=setFskClipPara()>
                                                                                    <OPTION selected value=Sdmf-fsk>
                                                                                        Sdmf-fsk
                                                                                    </OPTION>
                                                                                    <OPTION value=Mdmf-fsk>
                                                                                        Mdmf-fsk
                                                                                    </OPTION>
                                                                                    <OPTION value=Dtmf>
                                                                                        Dtmf
                                                                                    </OPTION>
                                                                                    <OPTION value=R1.5>
                                                                                        R1.5
                                                                                    </OPTION>
                                                                                    <OPTION value=etsi>
                                                                                        Etsi
                                                                                    </OPTION>
                                                                                </SELECT>
                                                                            </TD>
                                                                        </TR>
                                                                        <TR id=TrFskTime>
                                                                            <TD style="WIDTH: 20%" class=table1_left>
                                                                                发送FSK间隔:
                                                                            </TD>
                                                                            <TD class=table1_right colSpan=3>
                                                                                <INPUT style="WIDTH: 125px" id=FskTime type=text>
                                                                                &nbsp;（单位：毫秒）
                                                                            </TD>
                                                                        </TR>
                                                                        <TR>
                                                                            <TD style="WIDTH: 20%" class=table1_left>
                                                                                来电显示流程:
                                                                            </TD>
                                                                            <TD class=table1_right colSpan=3>
                                                                                <SELECT style="WIDTH: 125px" id=ClipTransWhen>
                                                                                    <OPTION selected value=AfterRing>
                                                                                        振铃后
                                                                                    </OPTION>
                                                                                    <OPTION value=BeforeRing>
                                                                                        振铃前
                                                                                    </OPTION>
                                                                                </SELECT>
                                                                            </TD>
                                                                        </TR>
                                                                        <TR>
                                                                            <TD style="WIDTH: 20%" class=table1_left>
                                                                                启用DSP模板:
                                                                            </TD>
                                                                            <TD class=table1_right colSpan=3>
                                                                                <INPUT id=EnableDspTemplate onclick=setDspTemplatePara() type=checkbox>
                                                                            </TD>
                                                                        </TR>
                                                                        <TR id=TrWorkMode>
                                                                            <TD style="WIDTH: 20%" class=table1_left>
                                                                                工作模式:
                                                                            </TD>
                                                                            <TD class=table1_right colSpan=3>
                                                                                <SELECT style="WIDTH: 125px" id=WorkMode>
                                                                                    <OPTION selected value=Voice>
                                                                                        语音
                                                                                    </OPTION>
                                                                                    <OPTION value=Fax>
                                                                                        传真
                                                                                    </OPTION>
                                                                                    <OPTION value=Modem>
                                                                                        Modem
                                                                                    </OPTION>
                                                                                </SELECT>
                                                                            </TD>
                                                                        </TR>
                                                                        <TR>
                                                                            <TD style="WIDTH: 20%" class=table1_left>
                                                                                使能端口反极:
                                                                            </TD>
                                                                            <TD class=table1_right colSpan=3>
                                                                                <INPUT id=EnablePotsReversePole type=checkbox name=EnablePotsReversePole>
                                                                            </TD>
                                                                        </TR>
                                                                        <TR>
                                                                            <TD style="WIDTH: 20%" class=table1_left>
                                                                                短定时器:
                                                                            </TD>
                                                                            <TD class=table1_right colSpan=3>
                                                                                <INPUT style="WIDTH: 125px" id=shorttimer type=text>
                                                                                &nbsp;（单位：秒）
                                                                            </TD>
                                                                        </TR>
                                                                        <TR>
                                                                            <TD style="WIDTH: 20%" class=table1_left>
                                                                                长定时器:
                                                                            </TD>
                                                                            <TD class=table1_right colSpan=3>
                                                                                <INPUT style="WIDTH: 125px" id=longtimer type=text>
                                                                                &nbsp;（单位：秒）
                                                                            </TD>
                                                                        </TR>
                                                                        <TR>
                                                                            <TD style="WIDTH: 20%" class=table1_left>
                                                                                话机时间同步:
                                                                            </TD>
                                                                            <TD class=table1_right colSpan=3>
                                                                                <INPUT id=ClipSendDateTime type=checkbox name=ClipSendDateTime>
                                                                            </TD>
                                                                        </TR>
                                                                    </TBODY>
                                                                </TABLE>
                                                            </DIV>
                                                            <TABLE style="WIDTH: 100%" border=0 cellSpacing=1 cellPadding=0>
                                                                <TBODY>
                                                                    <TR>
                                                                        <TD style="WIDTH: 20%" class=table1_submit>
                                                                        </TD>
                                                                        <TD style="HEIGHT: 50px" class=table1_submit colSpan=3>
                                                                            <INPUT id=btnApplyVoipUser3 class=submit onclick=Submit(); value=" 应用 "
                                                                            type=button name=btnApplyVoipUser3>
                                                                            <INPUT id=cancelValue3 class=submit onclick=CancelConfig(); value=" 取消 "
                                                                            type=button name=cancelValue3>
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
                                    <TD width=7 background=/img/panel3.gif>
                                    </TD>
                                    <TD>
                                    </TD>
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
                                    <TD width=162 bgColor=#ef8218 class=CucBgColorTD_2>
                                        &nbsp;
                                    </TD>
                                    <TD bgColor=#427594 class=CucBgColorTD_2>
                                        &nbsp;
                                    </TD>
                                    <TD bgColor=#427594 class=CucBgColorTD_2>
                                        <P align=center>
                                            &nbsp;
                                        </P>
                                    </TD>
                                    <TD class=table1_help bgColor=#FFB403>
                                        &nbsp;
                                    </TD>
                                </TR>
                            </TBODY>
                        </TABLE>
                    </TD>
                </TR>
            </TBODY>
        </TABLE>
    </BODY>

</HTML>