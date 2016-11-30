<%
	'tcWebApi_constSet("System_Entry","auto_pvc_trgger","1")
	'tcWebApi_constSet("AutoPVC_Common","Active","1")
	'tcWebApi_constSet("AutoPVC_Common","autoSwitch","1")
	'tcWebApi_Commit("System_Entry")
	'tcWebApi_Commit("AutoPVC_Common")
%>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01//EN" "http://www.w3.org/TR/html4/strict.dtd">
<html lang="pt-br">
    <head>
		<meta http-equiv=Content-Type content="text/html; charset=UTF-8">		
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
			var phonenumtemp = mainFrm.wizardOIRES_Phone.value;
			
			if(mainFrm.wizardOIRES_Phone.value.match("^[(][0-9]{2}[)][0-9]{4}-[0-9]{4}$") != null)
			{
				phonenumtemp = mainFrm.wizardOIRES_Phone.value.charAt(1)+mainFrm.wizardOIRES_Phone.value.charAt(2)+mainFrm.wizardOIRES_Phone.value.charAt(4)+mainFrm.wizardOIRES_Phone.value.charAt(5)+
				mainFrm.wizardOIRES_Phone.value.charAt(6)+mainFrm.wizardOIRES_Phone.value.charAt(7)+mainFrm.wizardOIRES_Phone.value.charAt(9)+mainFrm.wizardOIRES_Phone.value.charAt(10)+
				mainFrm.wizardOIRES_Phone.value.charAt(11)+mainFrm.wizardOIRES_Phone.value.charAt(12);
			}
			else
			{
				<%if tcWebApi_get("WebCustom_Entry","isOIWizardC9Support","h") = "Yes" then%>
				alert("Insira o número do seu Oi Fixo com 2 dígitos de DDD e com 10 dígitos no total (não utilizar zero na frente do número e hífen)."); 
				<%else%>
				alert("Insira o número do seu Oi Fixo com 2 dígitos de DDD e com 10 dígitos no total (não use levando número zero eo hífen)."); 		
				<%end if%>
				return;
			}		

			if (null != mainFrm.wizardOIRES_Phone)
			{
				if (!oiPhoneCheck(phonenumtemp))
					return;
			}
	
			if (null != mainFrm.wizardOI_PPPUserName)
			{
				if (!StringCheck(mainFrm.wizardOI_PPPUserName.value))
				{
					alert("Nome de Usuário não pode ser vazio ou conter espaços.");
					return;
				}
				
				if (quotationCheck(mainFrm.wizardOI_PPPUserName, 71))
					return;
				
				if (key == mainFrm.wizardOI_PPPUserName.value)
				{
					alert("O usuário oivelox@oivelox é provisório. Se você não tem um provedor de internet, clique no link disponível na parte inferior da sua tela.");
					mainFrm.wizardOI_PPPUserName.value="";
					return;
				}

				if(mainFrm.wizardOI_PPPUserName.value.match("(@oiwififon)$") != null)
				{
					alert("Usuário inválido. Por favor digite novamente.");
					mainFrm.wizardOI_PPPUserName.value="";
					return;
				}
			}
			
			if (null != mainFrm.wizardOI_PPPUserPassword)
			{
				if (!StringCheck(mainFrm.wizardOI_PPPUserPassword.value))
				{
					alert("Senha não pode ser vazia ou conter espaços.");
					return;
				}
				
				if (quotationCheck(mainFrm.wizardOI_PPPUserPassword, 71))
					return;
			}
			
			if (null != mainFrm.wizardOI_PPPUserRepeatPassword)
			{
				if (mainFrm.wizardOI_PPPUserRepeatPassword.value != mainFrm.wizardOI_PPPUserPassword.value)
				{
					alert("Repita a senha corretamente.");
					return;
				}
			}
			mainFrm.wizardOIRES_Phone_H.value= phonenumtemp;
			mainFrm.WAN_PPPFlag.value = "1";
			mainFrm.submit();
		}
		
		function phoneNumberCheck()
		{
			var mainFrm = document.forms[0];
			if(mainFrm.wizardOIRES_Phone.value.match("^[(][0-9]{2}[)][0-9]{4}-[0-9]{4}$") != null)
			{
				phonenumtemp = mainFrm.wizardOIRES_Phone.value.charAt(1)+mainFrm.wizardOIRES_Phone.value.charAt(2)+mainFrm.wizardOIRES_Phone.value.charAt(4)+mainFrm.wizardOIRES_Phone.value.charAt(5)+
				mainFrm.wizardOIRES_Phone.value.charAt(6)+mainFrm.wizardOIRES_Phone.value.charAt(7)+mainFrm.wizardOIRES_Phone.value.charAt(9)+mainFrm.wizardOIRES_Phone.value.charAt(10)+
				mainFrm.wizardOIRES_Phone.value.charAt(11)+mainFrm.wizardOIRES_Phone.value.charAt(12);
			}
			else
			{
				<%if tcWebApi_get("WebCustom_Entry","isOIWizardC9Support","h") = "Yes" then%>
				alert("Insira o número do seu Oi Fixo com 2 dígitos de DDD e com 10 dígitos no total (não utilizar zero na frente do número e hífen)."); 
				<%else%>
				alert("Insira o número do seu Oi Fixo com 2 dígitos de DDD e com 10 dígitos no total (não use levando número zero eo hífen)."); 
				<%end if%>			
				return;
			}
		}
		
		function PhoneChange(value)
		{
			if('0' == value.charAt(0))
			{
				alert("Insira o número do seu Oi Fixo com 2 dígitos de DDD e com 10 dígitos no total (não utilizar zero na frente do número e hífen)."); 			
				return;
			}
			else if(10 == value.length && value.match("[^0-9]") == null)
			{
				document.forms[0].wizardOIRES_Phone.value = '('+value.charAt(0)+value.charAt(1)+')'+value.charAt(2)+value.charAt(3)+value.charAt(4)+value.charAt(5)+'-'+value.charAt(6)+value.charAt(7)+value.charAt(8)+value.charAt(9);
				return;
			}
		}
		
		function passwordCheck()
		{
			var mainFrm = document.forms[0];
			if (null != mainFrm.wizardOI_PPPUserPassword)
			{
				if (!StringCheck(mainFrm.wizardOI_PPPUserPassword.value))
				{
					alert("Senha não pode ser vazia ou conter espaços.");
					return;
				}
				
				if (quotationCheck(mainFrm.wizardOI_PPPUserPassword, 71))
					return;
			}
			
			if (null != mainFrm.wizardOI_PPPUserRepeatPassword)
			{
				if (mainFrm.wizardOI_PPPUserRepeatPassword.value != mainFrm.wizardOI_PPPUserPassword.value)
				{
					alert("Repita a senha corretamente.");
					return;
				}
			}
		}
		
		function doload()
		{
			var phonenumloadtemp = "<%tcWebApi_get("Wan_PVC0","USERPHONE","s")%>";
			if(10 == phonenumloadtemp.length && phonenumloadtemp.match("[^0-9]") == null)
			{
				document.forms[0].wizardOIRES_Phone.value = '('+phonenumloadtemp.charAt(0)+phonenumloadtemp.charAt(1)+')'+phonenumloadtemp.charAt(2)+phonenumloadtemp.charAt(3)+phonenumloadtemp.charAt(4)+phonenumloadtemp.charAt(5)+'-'+phonenumloadtemp.charAt(6)+phonenumloadtemp.charAt(7)+phonenumloadtemp.charAt(8)+phonenumloadtemp.charAt(9);
			}
		}		
		</script>
    </head>

    <body onLoad="doload()">
	<form name="configure_modem_residen" method="post" action="/cgi-bin/WizardOi/configurar_wifi.asp" >
        <div id="layout_bg_left" class="layout">
            <div id="layout_bg_right" class="layout">
                &nbsp;
            </div>
        </div>
		 <input type="hidden" name="WAN_PPPFlag" value="0">
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
                    <div id="page_set_modem_residential_content">
                        <div class="page_txt" style="font-size:22px; font-weight:bold">
                            Pra navegar na Internet voc&ecirc; precisa se cadastrar em um provedor de acesso &agrave; Internet.
                        </div>
                        <br>
                        <div class="page_txt" style="font-size:22px; font-weight:bold">
                            Se voc&ecirc; j&aacute; &eacute; cadastrado, preencha os campos abaixo com o n&uacute;mero de telefone da sua linha e os dados do seu provedor.
                        </div>

                        <div class="page_form">
                                <div class="page_form_line">
                                
                                    <div class="page_form_line_name page_form_name_set_modem_residential">
									Oi Fixo</div>
                                    <div class="page_form_line_field page_form_line_field_input">
									<input type="hidden" name="wizardOIRES_Phone_H">
									<input type="text" name="wizardOIRES_Phone" maxlength="10" onkeyup="PhoneChange(this.value)" onblur="phoneNumberCheck()" /></div>
                                    <div class="page_form_line_description">
									Telefone Fixo com DDD vinculado ao seu Oi Velox</div>
                                </div>

                                <div class="page_form_line">
                                    <div class="page_form_line_name page_form_name_set_modem_residential">
									Usu&aacute;rio</div>
                                    <div class="page_form_line_field page_form_line_field_input">
									<input type="text" name="wizardOI_PPPUserName" maxlength="71" 
									value="<%if tcWebApi_get("Wan_PVC0","USERNAME","h") <> "N/A" then  tcWebApi_get("Wan_PVC0","USERNAME","s") end if%>" /></div>
                                    <div class="page_form_line_description">
									Ex: usuario@provedor ou usuario@provedor.com.br</div> 
                                </div>

                                <div class="page_form_line">
                                    <div class="page_form_line_name page_form_name_set_modem_residential">
									Senha</div>
                                    <div class="page_form_line_field page_form_line_field_input">
									<input type="password" name="wizardOI_PPPUserPassword" maxlength="71" 
									value="<%if tcWebApi_get("Wan_PVC0","PASSWORD","h") <> "N/A" then  tcWebApi_get("Wan_PVC0","PASSWORD","s") end if%>"/></div>
                                    <div class="page_form_line_description">
									Informe a senha que voc&ecirc; recebeu do seu Provedor*</div>
                                </div>
                                
                                <div class="page_form_line">
                                    <div class="page_form_line_name page_form_name_set_modem_residential">
									Repetir Senha</div>
                                    <div class="page_form_line_field page_form_line_field_input">
									<input type="password" name="wizardOI_PPPUserRepeatPassword" maxlength="71" onblur="passwordCheck()"
									value="<%if tcWebApi_get("Wan_PVC0","PASSWORD","h") <> "N/A" then  tcWebApi_get("Wan_PVC0","PASSWORD","s") end if%>" /></div>
                                    <div class="page_form_line_description">
									Confirme sua senha</div>
                                </div>
							<script type="text/javascript" language="JavaScript">
							var tempMacAddr = "<% tcWebApi_get("Info_Ether","mac","s") %>";
							var macAddr = tempMacAddr.split(":");
							var OI_Password = macAddr[0] + macAddr[1] + macAddr[2]+ macAddr[3]+ macAddr[4] + macAddr[5];
							var OI_Username = OI_Password +"@oivelox";
							if(document.forms[0].wizardOI_PPPUserName.value == OI_Username)
								document.forms[0].wizardOI_PPPUserName.value = "";
							if(document.forms[0].wizardOI_PPPUserPassword.value == OI_Password)
							{
								document.forms[0].wizardOI_PPPUserPassword.value = "";
								document.forms[0].wizardOI_PPPUserRepeatPassword.value = "";								
							}
							</script>
                        </div>

                        <br>
                        
                        <div class="page_txt" style="font-size:22px; font-weight:bold">
                            Se voc&ecirc; ainda n&atilde;o tem provedor de Internet, <a href="javascript:void(0)" onclick="navPage( 'mobicare' )" >clique aqui</a> para se cadastrar e receber o seu usu&aacute;rio e senha. Ap&oacute;s esse passo preencha os campos acima e clique em avan&ccedil;ar.
                        </div>
                    </div>
                </div>

                <div class="page_btns">
				<%if tcWebApi_get("GUI_Entry0", "OI_USER_ENTERPRISE", "h")= "1" then %>
					<a href="javascript:void(0)" onclick="navPage( 'welcome_comer' )"><img src="../../include/images/page_btn_back.png" width="150" height="30" title="Voltar" alt="Voltar" onmouseover="this.src='../../include/images/page_btn_back_over.png'" onmouseout="this.src='../../include/images/page_btn_back.png'" /></a>
				<%else%>
                    <a href="javascript:void(0)" onclick="navPage( 'welcome_homeuser' )"><img src="../../include/images/page_btn_back.png" width="150" height="30" title="Voltar" alt="Voltar" onmouseover="this.src='../../include/images/page_btn_back_over.png'" onmouseout="this.src='../../include/images/page_btn_back.png'" /></a>
				<%end if%>
                    <a href="javascript:void(0)" onclick="PageSubmit();" ><img src="../../include/images/page_btn_forward.png" width="166" height="30" title="Avan&ccedil;ar" alt="Avan&ccedil;ar" onmouseover="this.src='../../include/images/page_btn_forward_over.png'" onmouseout="this.src='../../include/images/page_btn_forward.png'" /></a>
                </div>
                </div>
                </td>
            </tr>
        </table>
		</form>
    </body>
	<head>
		<meta http-equiv="Pragma" content="no-cache">
		<meta http-equiv="cache-control" content="no-cache">
		<meta http-equiv="Expires" content="-1">
	</head>
</html>
