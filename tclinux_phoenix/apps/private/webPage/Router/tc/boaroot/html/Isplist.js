var CountryList=new Array();
var ISPList=new Array();

var ispPrtcl;
var ispEncap;
var ispVPI;
var ispVCI;
var ispMTU;

var PPPoEPPPoA=new Array();
var Dynamic=new Array();
var Static=new Array();

var cn=0;
CountryList[cn++]=new Citem("Belgium", 2);
CountryList[cn++]=new Citem("Denmark", 2);
CountryList[cn++]=new Citem("Finland", 2);
CountryList[cn++]=new Citem("France", 2);
CountryList[cn++]=new Citem("Ireland", 3);
CountryList[cn++]=new Citem("Italy", 2);
CountryList[cn++]=new Citem("Netherlands", 2);
CountryList[cn++]=new Citem("Norway", 2);
CountryList[cn++]=new Citem("Spain", 2);
CountryList[cn++]=new Citem("Sweden", 2);
CountryList[cn++]=new Citem("United Kingdom", 2);

var num=0;
//Belgium
ISPList[num++]=new ISPitem("Belgium", "Belgacom", "PPPoE", "LLC", 8, 35, 0);
ISPList[num++]=new ISPitem("Belgium", "Skynet", "PPPoE", "LLC", 8, 35, 0)

//Denmark 
ISPList[num++]=new ISPitem("Denmark", "TDC", "PPPoA", "LLC", 0, 35, 0);
ISPList[num++]=new ISPitem("Denmark", "Foroya", "PPPoA", "LLC", 1, 47, 0)

//Finland
ISPList[num++]=new ISPitem("Finland", "Sonera", "Dynamic IP", "LLC", 0, 33, 0);
ISPList[num++]=new ISPitem("Finland", "Elisa", "Dynamic IP", "LLC", 0, 100, 0);

//France
ISPList[num++]=new ISPitem("France", "Orange(Wanadoo)", "PPPoE", "LLC", 8, 35, 0);
ISPList[num++]=new ISPitem("France", "Free(Degroupe)", "Static IP", "VC Mux", 8, 36, 0);

//Ireland
ISPList[num++]=new ISPitem("Ireland", "BT", "PPPoA", "VC Mux", 0, 38, 0);
ISPList[num++]=new ISPitem("Ireland", "Sky Broadband", "PPPoA", "VC Mux", 0, 38, 1492);
ISPList[num++]=new ISPitem("Ireland", "Eircom", "PPPoE", "LLC", 8, 35, 0);

//Italy
ISPList[num++]=new ISPitem("Italy", "Telecom Italia", "PPPoE", "LLC", 8, 35, 1492);
ISPList[num++]=new ISPitem("Italy", "Infostrada", "PPPoE", "LLC", 8, 35, 0);

//Netherlands
ISPList[num++]=new ISPitem("Netherlands", "KPN", "PPPoA", "VC Mux", 8, 48, 0);
ISPList[num++]=new ISPitem("Netherlands", "Tiscali NL", "PPPoA", "VC Mux", 8, 48, 0);

//Norway
ISPList[num++]=new ISPitem("Norway", "Telenor", "PPPoE", "LLC", 8, 35, 0);
ISPList[num++]=new ISPitem("Norway", "Nexgentel", "PPPoE", "LLC", 1, 32, 0);

//Spain
ISPList[num++]=new ISPitem("Spain", "Telefonica(IP Dinamica)", "PPPoE", "LLC", 8, 32, 0);
ISPList[num++]=new ISPitem("Spain", "ONO", "PPPoA", "VC Mux", 8, 35, 0);

//Sweden 
ISPList[num++]=new ISPitem("Sweden", "Telia", "Dynamic IP", "LLC", 8, 35, 0);
ISPList[num++]=new ISPitem("Sweden", "Bredbandsbolaget", "Dynamic IP", "LLC", 8, 35, 0);

//United Kingdom
ISPList[num++]=new ISPitem("United Kingdom", "BT", "PPPoA", "VC Mux", 0, 38, 0);
ISPList[num++]=new ISPitem("United Kingdom", "Sky Broadband", "PPPoA", "VC Mux", 0, 38, 1492);

function Citem(scountry, iispcount)
{
    this.scountry=scountry;
    this.iispcount=iispcount;
}


function ISPitem(scountry,sname,sprtcl,sencap,ivpi, ivci, imtu)
{
    this.scountry=scountry;
    this.sname=sname;
    this.sprtcl=sprtcl;
    this.sencap=sencap;
    this.ivpi=ivpi;
    this.ivci=ivci;
    this.imtu=imtu;
}

function changeISP(cb1, cb2){

	var value = cb1[cb1.selectedIndex].value;
	
	cb2.options.length=0;
	
	cb2.options[0]=new Option("(Click to select)", "-1");
	if ((value == -1)||(value == -2)){
		cb2.options[1]=new Option("Others", "-2");
		if ((value == -2))
			cb2.selectedIndex = cb2.length - 1;
		return;
	}

	var count =0;
	
	for (i=0; i < ISPList.length; i++){
		if (CountryList[cb1[cb1.selectedIndex].value].scountry == ISPList[i].scountry){
			cb2.options[count+1]=new Option(ISPList[i].sname, i);
			count++;
		}
	
		if (count == CountryList[cb1[cb1.selectedIndex].value].iispcount){
			break;
		}
	}

	cb2.options[count + 1]=new Option("Others", "-2");

}

function createCountry(cb){

	cb.options.length=0;
	
	cb.options[0]=new Option("(Click to select)", "-1");
	
	var count =0;
	
	for (i=0; i < CountryList.length; i++){
		cb.options[i+1]=new Option(CountryList[i].scountry, i);
		count++;
	}

	cb.options[count + 1]=new Option("Others", "-2");
}

function getISPDetails(cb){

	var value = cb[cb.selectedIndex].value;
	if ((value == -1)||(value == -2)){
		ispPrtcl = "";
		ispEncap = "";
		ispVPI = 0;
		ispVCI = 35;
		ispMTU = 0;
		return;
	}
	
	ispPrtcl = ISPList[cb[cb.selectedIndex].value].sprtcl;
	ispEncap = ISPList[cb[cb.selectedIndex].value].sencap;
	ispVPI = ISPList[cb[cb.selectedIndex].value].ivpi;
	ispVCI = ISPList[cb[cb.selectedIndex].value].ivci;
	ispMTU = ISPList[cb[cb.selectedIndex].value].imtu;
	
}
	
function setContype(cb){
	
	cb.options.length = 0;
	
	cb.options[0] = new Option("(Click to Select)", "-1");
	cb.selectedIndex = 0;
	
	if (ispPrtcl == "PPPoA"){
		cb.options[1] = new Option("VC-Mux", "PPPoAVCMux");
		cb.options[2] = new Option("LLC", "PPPoALLC");
		if (ispEncap == "LLC")
		cb.selectedIndex = 2;
		else if (ispEncap == "VC Mux")
		cb.selectedIndex = 1;
	} else if (ispPrtcl == "PPPoE"){
		cb.options[1] = new Option("VC-Mux", "PPPoEVCMux");
		cb.options[2] = new Option("LLC", "PPPoELLC");
		if (ispEncap == "LLC")
		cb.selectedIndex = 2;
		else if (ispEncap == "VC Mux")
		cb.selectedIndex = 1;
	} else if (ispPrtcl == "Dynamic IP"){
		cb.options[1] = new Option("LLC", "DynLLC");
		cb.options[2] = new Option("VC-Mux", "DynVCMux");
		if (ispEncap == "LLC")
		cb.selectedIndex = 1;
		else if (ispEncap == "VC Mux")
		cb.selectedIndex = 2;
	} else if (ispPrtcl == "Static IP"){
		cb.options[1] = new Option("LLC", "MerLLC");
		cb.options[2] = new Option("VC-Mux", "MerVCMux");
		//cb.options[2] = new Option("1483 Routed IP LLC", "IPoALLC");
		//cb.options[3] = new Option("1483 Routed IP VC-Mux", "IPoAVCMux");
		if (ispEncap == "LLC")
		cb.selectedIndex = 1;
		else if (ispEncap == "VC Mux")
		cb.selectedIndex = 2;
	} else if (ispPrtcl == "Bridge"){
		cb.options[1] = new Option("LLC", "MerLLC");
		cb.options[2] = new Option("VC-Mux", "MerVCMux");
		if (ispEncap == "LLC")
		cb.selectedIndex = 1;
		else if (ispEncap == "VC Mux")
		cb.selectedIndex = 2;
	}
}
var mesg;
var atmVpi;
var atmVci;

var wizVpi;
var wizVci;
var ntwkprtcl;
var mtu;
var wizObtGw;
var wizStaticType;
if(mtu == '' || mtu == 0 || mtu == 'undefined')
mtu = 1492;

function prtcl(){// rewite by jack,08.5.19
	var mode;
	with(document.forms[0]){
		if(uiViewEncaps.selectedIndex == 1 || uiViewEncaps.selectedIndex == 2)
			mode = 'PPPoE';
		else if(uiViewEncaps.selectedIndex == 3||uiViewEncaps.selectedIndex == 4)
			mode = 'PPPoA';
		else if(uiViewEncaps.selectedIndex == 5||uiViewEncaps.selectedIndex == 6)
		{
		 	mode = 'Dynamic IP';
		}
		else if(uiViewEncaps.selectedIndex == 7||uiViewEncaps.selectedIndex == 8)
		{
		 	mode = 'Static IP';
		}
		else if(uiViewEncaps.selectedIndex == 9||uiViewEncaps.selectedIndex == 10)
			mode = 'Static IP';
		else 
			mode = '';
	}
	return mode; 
}


function ispAttr(name, rotocol, type, vpi, vci){
	this.name = name;
	this.protocol = protocol;
	this.type = type;
	this.vpi = vpi;
	this.vci = vci;
}

function isValidVpi(value)
{
	if(value == '0' || value == 0)
	return true;
	var i;
	for(i = 0; i < value.length; i++)
	{
		if(value.charAt(i) == '0' && i == 0)
		return false;
	}
	return true;
}

function isValidVci(value)
{
	if(value == '0' || value == 0)
	return true;
	var i;
	for(i = 0; i < value.length; i++)
	{
		if(value.charAt(i) == '0' && i == 0)
		return false;
	}
	return true;
}
	
function getISP(){
	with (document.forms[0]) {
		changeISP(country, isplist);
	}
	disableOption();
}

function disableOption(){
	var ispPrtcl;
	with (document.forms[0]) {
		Next.disabled = false;
		hidemtu(1);
		if ( country.selectedIndex == 0)
		{
		   	Next.disabled = true;
		}
	
		if (isplist.selectedIndex == 0){
			Next.disabled = true;
			vpi.disabled = true;
			vci.disabled = true;
			uiViewEncaps.disabled = true;
		}
		else
		{
			vpi.disabled = false;
			vci.disabled = false;
			uiViewEncaps.disabled = false;
		}
		getISPDetails(isplist);	
		getIspTypeIndex();
		if(uiViewEncaps.disabled ==false)
			ispPrtcl = prtcl();
		else 
			ispPrtcl = "";
		if ((ispPrtcl == "PPPoE") || (ispPrtcl == "PPPoA"))
			hidemtu(0); 
		
		
		vpi.value = ispVPI;
		vci.value = ispVCI;
		
		if((isplist.selectedIndex == (isplist.length - 1))||(isplist.selectedIndex == 0)){
			vpi.value = "(Enter a number)";
			vci.value = "(Enter a number)";
		}
	}
}

function contypeChange()
{
	var ispPrtcl;
	with (document.forms[0]) {
		hidemtu(1);
		hideStaticIP(1);
		ispPrtcl = prtcl();
		if ((ispPrtcl == "PPPoE") || (ispPrtcl == "PPPoA"))
			hidemtu(0);
		if(ispPrtcl == "Static IP")
			hideStaticIP(0);
	}
}

function hideStaticIP(hide){
	var status = 'block';
	with (document.forms[0]) {
		if ( hide == 1 ){
			status = 'none';
		}
		if (document.getElementById){  // DOM3 = IE5, NS6
			document.getElementById("staticIP").style.display = status;
		}
	}
}

function hidemtu(hide){
	var status = 'block';
	with (document.forms[0]) {
		if(ispMTU == 0)
			pppmtu.value = mtu;
		else
			pppmtu.value = ispMTU;
		if (pppmtu.value == 'undefined')
		{
		    pppmtu.value = 1492;
		}
	
		if ( hide == 1 ){
			status = 'none';
			pppmtu.value = '';
		}
		if (document.getElementById){  // DOM3 = IE5, NS6
			document.getElementById("mtuinfo").style.display = status;
			document.getElementById("username").style.display = status;
		}
	}
}

function getIspTypeIndex(){
	with(document.forms[0]){
		if((ispPrtcl == 'PPPoE')&&(ispEncap == 'VC Mux'))
			uiViewEncaps.selectedIndex = 1;
		else if((ispPrtcl == 'PPPoE')&&(ispEncap == 'LLC'))
			uiViewEncaps.selectedIndex = 2;
		else if((ispPrtcl == 'PPPoA')&&(ispEncap == 'VC Mux'))
			uiViewEncaps.selectedIndex = 3;
		else if((ispPrtcl == 'PPPoA')&&(ispEncap == 'LLC'))
			uiViewEncaps.selectedIndex = 4;
		else if((ispPrtcl == 'Dynamic IP')&&(ispEncap == 'LLC'))
			uiViewEncaps.selectedIndex = 5;
		else if((ispPrtcl == 'Dynamic IP')&&(ispEncap == 'VC Mux'))
			uiViewEncaps.selectedIndex = 6;
		else if((ispPrtcl == 'Static IP')&&(ispEncap == 'LLC'))
			uiViewEncaps.selectedIndex = 7;
		else if((ispPrtcl == 'Static IP')&&(ispEncap == 'VC Mux'))
			uiViewEncaps.selectedIndex = 8;
	}
}

function InitIsp_page()
{
	with(document.forms[0])
	{
		createCountry(country);
		getISP();
		disableOption();
		contypeChange();
	}
}
