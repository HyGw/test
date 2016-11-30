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
		function doPlayVideo()
		{
			var playerContent='<object type="application/x-shockwave-flash" data="../../include/vedio/vcastr3.swf" width="310" height="220" id="vcastr3"><param name="movie" value="../../include/vedio/vcastr3.swf"/><param name="FlashVars" value="xml={vcastr}{channel}{item}{source}../../include/vedio/OiVelox.flv{/source}{/item}{/channel}{config}{controlPanelMode}none{/controlPanelMode}{scaleMode}exactFit{/scaleMode}{/config}{/vcastr}"/></object>';
			document.getElementById("player").innerHTML = playerContent;
			document.getElementById("player").style.display = "";
			document.getElementById("help").className="";
			document.getElementById("help").style.marginTop = "5px";
			setTimeout("dohidden();", 77*1000);
		}
		function dohidden()
		{
			document.getElementById("player").style.display = "none";
			document.getElementById("player").innerHTML = "";
			document.getElementById("help").className="page_authentication_failure_velox_help";
			document.getElementById("help").style.marginTop = "";
		}
		function PageSubmit()
		{
			document.configure_modem_residen.WAN_RetryFlag.value = "2";
			document.configure_modem_residen.submit();
		}

		</script>  
    </head>    
    <body>
	<form name="configure_modem_residen" method="post" action="/cgi-bin/WizardOi/adslTest.asp">
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
                <div class="page_content page_authentication_failure_velox">
                    <div id="page_authentication_failure_velox_video_content">
                        <div class="page_title">
                            Falha na conex&atilde;o do seu Velox.
                        </div>
                        <div class="page_txt">
                        <%if tcWebApi_get("WebCustom_Entry","isOIWizardC9Support","h") = "Yes" then%>	
                            Verifique se os cabos do modem est&atilde;o conectados<br />corretamente de acordo com o esquema ao lado.<br />
							Voc&ecirc; tamb&eacute;m pode consultar seu Guia de Instala&ccedil;&atilde;o<br />
                            ou assistir ao <a href="javascript:void(0)" onclick="doPlayVideo();" >v&iacute;deo Tutorial clicando aqui</a>.<br/>
							O v&iacute;deo possui 1 minuto e 20 segundos.
                        <%elseif	tcWebApi_get("WebCustom_Entry","isOIWizardC7Support","h") = "Yes" then%>
                            Verifique se os cabos do modem est&atilde;o conectados<br />corretamente de acordo com o esquema ao lado.<br />
							Voc&ecirc; tamb&eacute;m pode consultar seu Guia de Instala&ccedil;&atilde;o<br />
                            ou assistir ao <a href="javascript:void(0)" onclick="doPlayVideo();" >v&iacute;deo Tutorial clicando aqui</a>.<br/>
							O v&iacute;deo possui 1 minuto e 20 segundos.
                        <%else%>
                            Verifique se os cabos do modem est&atilde;o conectados<br />corretamente de acordo com o esquema ao lado.<br />
							Voc&ecirc; tamb&eacute;m pode consultar seu Guia de Instala&ccedil;&atilde;o<br />
                            R&aacute;pida ou assistir ao <a href="javascript:void(0)" onclick="doPlayVideo();" >v&iacute;deo Tutorial clicando aqui</a>.<br/>
							O video possui 1 minuto e 20 segundos
												<%end if%>
                        </div>
			<div style="display:none;" id="player"></div>	
                        <div class="page_help page_authentication_failure_velox_help" id="help">
                            <div class="page_help_icon">
                                <img src="../../include/images/page_icon_help.png" width="24" height="25" alt="Ajuda" />
                            </div>
                            <div class="page_help_txt">
                            <%if tcWebApi_get("WebCustom_Entry","isOIWizardC9Support","h") = "Yes" then%>	
                            Quer ajuda? <a href="javascript:void(0)" onclick="navPage( 'adsl_off_problem_em' )" >Veja a solu&ccedil;&atilde;o para alguns problemas aqui</a>.<br />Caso o problema persista, entre em contato com o suporte:<br />
                                Cliente Empresarial: Ligue pra 0800 031 0800 e informe "falha na conex&atilde;o".
                            <%elseif	tcWebApi_get("WebCustom_Entry","isOIWizardC7Support","h") = "Yes" then%>
                            Quer ajuda? <a href="javascript:void(0)" onclick="navPage( 'adsl_off_problem_em' )" >Veja a solu&ccedil;&atilde;o para alguns problemas aqui</a>.<br />Caso o problema persista, entre em contato com o suporte:<br />
                                Cliente Empresarial: Ligue pra 0800 031 0800 e informe "falha na conex&atilde;o".
                            <%else%>
                                Quer ajuda? <a href="javascript:void(0)" onclick="navPage( 'adsl_off_problem_em' )" >Veja a solu&ccedil;&atilde;o para alguns problemas aqui</a>.<br />Cliente Empresarial: Ligue pra 0800 031 0800.<br />Cliente Residencial: Do RJ, ES, MG, BA, SE, AL, PE, PB, RN, CE, PI, MA, PA, AP, RR e AM, ligue pra 103 31. Do RS, SC, PR, MS, GO, TO, MT, AC, RO e do DF, ligue pra 103 14.
                            <%end if%>
                            </div>
                        </div>
                    </div>
                </div>
 				<div> &nbsp;</div>
                <div class="page_btns" 
                	<%if tcWebApi_get("WebCustom_Entry","isOIWizardC9Support","h") = "Yes" then%>	
														style="margin-top:0px"
                 	<%elseif	tcWebApi_get("WebCustom_Entry","isOIWizardC7Support","h") = "Yes" then%>
                            style="margin-top:0px"
                  <%else%>
                  <%end if%>>
                    <a href="javascript:void(0)" onclick="navPage( 'close' )"><img src="../../include/images/page_btn_close.png" width="150" height="30" title="Fechar" alt="Fechar" onmouseover="this.src='../../include/images/page_btn_close_over.png'" onmouseout="this.src='../../include/images/page_btn_close.png'" /></a>
                    <a href="javascript:void(0)" onclick="PageSubmit()" ><img src="../../include/images/page_btn_retry.png" width="180" height="30" title="Tentar Novamente" alt="Tentar Novamente" onmouseover="this.src='../../include/images/page_btn_retry_over.png'" onmouseout="this.src='../../include/images/page_btn_retry.png'" /></a>
                </div>
                </div>
                </td>
            </tr>
        </table>
		<input type="hidden" name="WAN_RetryFlag" value="0">
		</form>
    </body>
</html>
