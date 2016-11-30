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
		function PageSubmit()
		{
			var mainFrm = document.forms[0];
			var subdomain;
			var key = "oivelox@oivelox";
			var domain = "@telemar.com.br";
			
			if (null != mainFrm.wizardOIAuth_PPPUserName)
			{
				if (!StringCheck(mainFrm.wizardOIAuth_PPPUserName.value))
				{
					alert("Nome de Usuário não pode ser vazio ou conter espaços.");
					return;
				}
				
				if (quotationCheck(mainFrm.wizardOIAuth_PPPUserName, 71))
					return;

				if (key == mainFrm.wizardOIAuth_PPPUserName.value)
				{
					alert("O usuário oivelox@oivelox é provisório. Utilize o usuário e senha do seu provedor de internet.");
					mainFrm.wizardOIAuth_PPPUserName.value="";
					return;
				}
				if(mainFrm.wizardOIAuth_PPPUserName.value.match("(@oiwififon)$") != null)
				{
					alert("Usuário inválido. Por favor digite novamente.");
					mainFrm.wizardOIAuth_PPPUserName.value="";
					return;
				}
			}
			
			if (null != mainFrm.wizardOIAuth_PPPUserPassword)
			{
				if (!StringCheck(mainFrm.wizardOIAuth_PPPUserPassword.value))
				{
					alert("Senha não pode ser vazia ou conter espaços.");
					return;
				}
				
				if (quotationCheck(mainFrm.wizardOIAuth_PPPUserPassword, 71))
					return;
			}
			
			if (null != mainFrm.wizardOIAuth_PPPUserRepeatPassword)
			{
				if (mainFrm.wizardOIAuth_PPPUserRepeatPassword.value != mainFrm.wizardOIAuth_PPPUserPassword.value)
				{
					alert("Repita a senha corretamente.");
					return;
				}
			}
			mainFrm.WAN_NoWanIpFlag.value = "1";
			mainFrm.submit();
		}
		</script>
    </head>
    <body>
       <form name="ppp_failauthen" method="post" action="/cgi-bin/WizardOi/configurando.asp">
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
                    <div id="page_authentication_failure_content">
                        <div class="page_title">
                            Falha na autentica&ccedil;&atilde;o.
                        </div>

                        <div class="page_txt">
                            Verifique se seu usu&aacute;rio e senha est&atilde;o corretos e digite novamente nos campos abaixo.
                        </div>

                        <div class="page_form">

                                
                                <div class="page_form_line">
                                    <div class="page_form_line_name page_form_name_set_modem_residential">
									Usu&aacute;rio</div>
                                    <div class="page_form_line_field page_form_line_field_input">
                                    <%if tcWebApi_get("WebCustom_Entry","isOIWizardC9Support","h") = "Yes" then%>
                                    <input type="text" name="wizardOIAuth_PPPUserName" maxlength="71" value="" />
                                    <%elseif tcWebApi_get("WebCustom_Entry","isOIWizardC7Support","h") = "Yes" then%>
                                    <input type="text" name="wizardOIAuth_PPPUserName" maxlength="71" value="" />
                                    <%else%>
									<input type="text" name="wizardOIAuth_PPPUserName" maxlength="71"
									value="<%if tcWebApi_get("Wan_PVC0","USERNAME","h") <> "N/A" then  tcWebApi_get("Wan_PVC0","USERNAME","s") end if%>" /><%end if%></div>
                                    <div class="page_form_line_description">
									Ex: usuario@provedor ou usuario@provedor.com.br</div>
                                </div>

                                <div class="page_form_line">
                                    <div class="page_form_line_name page_form_name_set_modem_residential">
									Senha</div>
                                    <div class="page_form_line_field page_form_line_field_input">
                                    <%if tcWebApi_get("WebCustom_Entry","isOIWizardC9Support","h") = "Yes" then%>
                                    <input type="password" name="wizardOIAuth_PPPUserPassword" maxlength="71" value=""/>
                                    <%elseif tcWebApi_get("WebCustom_Entry","isOIWizardC7Support","h") = "Yes" then%>
                                    <input type="password" name="wizardOIAuth_PPPUserPassword" maxlength="71" value=""/>
                                    <%else%>
									<input type="password" name="wizardOIAuth_PPPUserPassword" maxlength="71"
									value="<%if tcWebApi_get("Wan_PVC0","PASSWORD","h") <> "N/A" then  tcWebApi_get("Wan_PVC0","PASSWORD","s") end if%>"/><%end if%></div>
                                </div>

                                <div class="page_form_line">
                                    <div class="page_form_line_name page_form_name_set_modem_residential">
									Repetir Senha</div>
                                    <div class="page_form_line_field page_form_line_field_input">
                                    <%if tcWebApi_get("WebCustom_Entry","isOIWizardC9Support","h") = "Yes" then%>
                                    <input type="password" name="wizardOIAuth_PPPUserRepeatPassword" maxlength="71" value=""/>
                                    <%elseif tcWebApi_get("WebCustom_Entry","isOIWizardC7Support","h") = "Yes" then%>
                                    <input type="password" name="wizardOIAuth_PPPUserRepeatPassword" maxlength="71" value=""/>
                                    <%else%>
									<input type="password" name="wizardOIAuth_PPPUserRepeatPassword" maxlength="71"
									value="<%if tcWebApi_get("Wan_PVC0","PASSWORD","h") <> "N/A" then  tcWebApi_get("Wan_PVC0","PASSWORD","s") end if%>"/><%end if%></div>
                                </div>
								<input type="hidden" name="WAN_NoWanIpFlag" value="0">
                        </div>
                        <div class="page_authentication_failure_description">
                            Caso o problema persista, entre em contato com o seu provedor.
                        </div>

                        <div class="page_help">
                            
                            <div class="page_help_icon">
                                <img src="../../include/images/page_icon_help.png" width="24" height="25" alt="Ajuda" />
                            </div>
                            
                            <div class="page_help_txt">
                                Quer ajuda? <a href="javascript:void(0)" onclick="navPage( 'user_password_problem' )" >Veja a solu&ccedil;&atilde;o para alguns problemas aqui</a>.
						    </div>
                        </div>
                    </div>
                </div>
                <div class="page_btns">
                    <a href="javascript:void(0)" onclick="navPage( 'close' )"><img src="../../include/images/page_btn_close.png" width="150" height="30" title="Fechar" alt="Fechar" onmouseover="this.src='../../include/images/page_btn_close_over.png'" onmouseout="this.src='../../include/images/page_btn_close.png'" /></a>
                    <a href="javascript:void(0)" onclick="PageSubmit();" ><img src="../../include/images/page_btn_forward.png" width="166" height="30" title="Avan&ccedil;ar" alt="Avan&ccedil;ar" onmouseover="this.src='../../include/images/page_btn_forward_over.png'" onmouseout="this.src='../../include/images/page_btn_forward.png'" /></a>
                </div>
                </div>
                </td>
            </tr>
        </table>
	</form>
    </body>
</html>
