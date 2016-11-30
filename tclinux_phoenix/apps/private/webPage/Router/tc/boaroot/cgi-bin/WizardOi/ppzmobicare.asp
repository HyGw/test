<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01//EN" "http://www.w3.org/TR/html4/strict.dtd">
<html lang="pt-br">
    <head>	
		<meta http-equiv=Content-Type content="text/html; charset=UTF-8">
		<meta http-equiv="Pragma" content="no-cache">
		<meta http-equiv="Expires" content="-1">
        <title>
		OI VELOX 
		</title>

        <link rel="shortcut icon" href="favicon.ico" >

        <link href="../../include/css/style.css" rel="stylesheet" type="text/css" />
        <link href="../../include/css/layout.css" rel="stylesheet" type="text/css" />
        <link href="../../include/css/home.css" rel="stylesheet" type="text/css" />
        <link href="../../include/css/pages.css" rel="stylesheet" type="text/css" />

        <script src="../../include/scripts/main.js" type="text/javascript"></script>
		<script type="text/javascript" language="JavaScript">
		function PageLoad()
		{
			var tempMacAddr = "<% tcWebApi_get("Info_Ether","mac","s") %>";
			var macAddr = tempMacAddr.split(":");
			var OI_Password = ""+ macAddr[0] + macAddr[1] + macAddr[2]+ macAddr[3]+ macAddr[4] + macAddr[5];
			var OI_Username = OI_Password +"@oivelox";
			var url = "/cgi-bin/wanDefAccount.cgi?pppusername="+OI_Username+"&ppppassword="+OI_Password;
			window.frames["wanDefAccount"].location.href=url;
			intervalFlag = setInterval("refreshWizardPage();", 3*1000);			
		}
		</script>
    </head>
    <body onLoad="PageLoad()">
	<FORM name=ConfigForm  method="post" action="/cgi-bin/WizardOi/ppzmobicare.asp">
        <div id="layout_bg_left" class="layout">
            <div id="layout_bg_right" class="layout">
                &nbsp;
            </div>
        </div>
         <table border="0" cellpadding="0" cellspacing="0" width="100%" height="100%" class="layout_pages">
             <tr>
                <td height="94" valign="top" align="left">
                <div id="layout_top">
                    <div id="layout_top_content">
                        <img src="../../include/images/layout_top.png" width="181" height="60" title="OI VELOX" alt="OI VELOX">
                    </div>
                </div>
            	</td>
            </tr>
            <tr>
            	<td valign="middle">
                <div id="layout_content">
                <div class="page_content">
                    <div id="page_setting_content"> 
                            <div class="page_txt_load" id="page_notice_content">
                               Aguarde enquanto redirecionamos ao Portal de Provedores Oi Velox
                            </div>

                            <div class="page_load" id="divLoading">
                                <img src="../../include/images/page_load.gif" width="140" height="18" alt="Load" />
                            </div>
                    </div>
                </div>
                <div class="page_btns" id="closeBtn" style="display:none">
                    <a href="javascript:void(0)" onclick="navPage( 'close' )"><img src="../../include/images/page_btn_close.png" width="150" height="30" title="Fechar" alt="Fechar" onmouseover="this.src='../../include/images/page_btn_close_over.png'" onmouseout="this.src='../../include/images/page_btn_close.png'" /></a>
                </div>
                </div>
				<DIV style="DISPLAY: none"><iframe id="wanDefAccount" name="wanDefAccount" frameborder="0" width="0" height="0"></iframe></DIV>
				<DIV style="DISPLAY: none"><iframe id="wanUpStatus" name="wanUpStatus" frameborder="0" width="0" height="0"></iframe></DIV>
                </td>
            </tr>
        </table>
	</FORM>
		<script type="text/javascript" language="JavaScript">
		var refreshLimit = 40; //2 miniute
		var wanip = "N/A";
		var intervalFlag= null;
		function refreshWizardPage()
		{
			window.frames["wanUpStatus"].location.href="wanUpStatus.asp?jump=true";
			refreshLimit--;
			if((refreshLimit == 0) && (null != intervalFlag))
				clearInterval(intervalFlag);			
			setTimeout("checkConnect()",1000);
		}
		function checkConnect()
		{
			wanip = window.frames["wanUpStatus"].wanip;
			if( (wanip != "N/A") && (typeof(wanip) != "undefined"))
			{
				if(null != intervalFlag)
					clearInterval(intervalFlag);
				//navPage( 'provider' );
			}
			else
			{
				if(refreshLimit == 0)
				{
					document.getElementById("divLoading").style.display = "none";
					document.getElementById("page_notice_content").innerHTML = "Desculpe, o Portal de Provedores est&aacute; fora do ar temporariamente.</br> Tente novamente mais tarde. Se o problema persistir, entre em contato com o suporte Oi Velox.";
					document.getElementById("closeBtn").style.display = "";
				}
			}
		}		
		</script>
    </body>
</html>
