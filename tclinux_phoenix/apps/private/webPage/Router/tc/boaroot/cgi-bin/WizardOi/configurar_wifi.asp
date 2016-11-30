<%
	tcWebApi_constSet("WebCurSet_Entry","wan_pvc","0")
	if request_form("WAN_PPPFlag") = "1" Then
		tcWebApi_set("Wan_PVC","USERNAME","wizardOI_PPPUserName")
		tcWebApi_set("Wan_PVC","PASSWORD","wizardOI_PPPUserPassword")
		tcWebApi_set("Wan_PVC","USERPHONE","wizardOIRES_Phone_H")		
	end if
	if request_form("WAN_NoWanIpFlag") = "1" Then
		tcWebApi_set("Wan_PVC","USERNAME","wizardOIAuth_PPPUserName")
		tcWebApi_set("Wan_PVC","PASSWORD","wizardOIAuth_PPPUserPassword")		
	end if		
%>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01//EN" "http://www.w3.org/TR/html4/strict.dtd">
<html lang="pt-br">
    <head>	
	<meta http-equiv=Content-Type content="text/html; charset=UTF-8">
	<meta http-equiv="Pragma" content="no-cache">
	<meta http-equiv="Expires" content="-1">
        <title>
		OI VELOX 
		</title>
        <link rel="shortcut icon" href="../../include/images/favicon.ico" >  
        <!-- CSS -->
        <link href="../../include/css/style.css" rel="stylesheet" type="text/css" />
        <link href="../../include/css/layout.css" rel="stylesheet" type="text/css" />
        <link href="../../include/css/home.css" rel="stylesheet" type="text/css" />
        <link href="../../include/css/pages.css" rel="stylesheet" type="text/css" />  
        <!-- MAIN -->
        <script src="../../include/scripts/main.js" type="text/javascript"></script>
		
		<script type="text/javascript" language="JavaScript">
		function doHexCheck(c)
		{
			if ( (c >= "0") && (c <= "9") )
				return 1;
			else if ( (c >= "A") && (c <= "F") )
				return 1;
			else if ( (c >= "a") && (c <= "f") )
				return 1;
			
			return -1;
		}
		function doWepCheck()
		{
			var KeyValue = document.forms[0].password.value;
			var len = KeyValue.length;
			if((len != 5) && (len != 13) && (len != 10) && (len != 26))
			{
				return false;	
			}else if((len == 10) || (len == 26))
			{
				if(KeyValue.match("[^0-9,a-f,A-F]"))
				{
					return false;
				}
			}
		}
		function wpapskCheck(WPAPSK)
		{
			var WPAPSKKey=WPAPSK.value;
			var len=WPAPSKKey.length;
			if(len < 8){
				return false;		
			}else if(len == 64){
				for(i=0; i<64; i++){
					var c=WPAPSKKey.charAt(i);
					if(doHexCheck(c)<0){
						return false;
					}
				}
			}else if(len > 64){
				return false;
			}
			return true;
		}
		function PageSubmit()
		{
			var isOIWizardC9Support = "<%tcWebApi_get("WebCustom_Entry","isOIWizardC9Support","s")%>";
			var isOIWizardC7Support = "<%tcWebApi_get("WebCustom_Entry","isOIWizardC7Support","s")%>";
					
			if(document.forms[0].encryption.value == "OPEN")
			{
					document.forms[0].submit();
					return;
				 }
					
			if(document.forms[0].encryption.selectedIndex == 1 ){
				if(doWepCheck() == false){
					alert("Senha inválida. Deve conter exatamente 5 ou 13 caracteres.");
					if(isOIWizardC9Support == "Yes" || isOIWizardC7Support == "Yes") 
					{
						document.forms[0].password.value = "";
						document.forms[0].repeat_password.value = "";
					}
					return;
				}
				if(document.forms[0].password.value.length == 13 || document.forms[0].password.value.length == 26)
					document.forms[0].encryption.options[1].value="WEP-128Bits";
				else
					document.forms[0].encryption.options[1].value="WEP-64Bits";
			}		
			
			if (document.forms[0].encryption.value == "WPAPSK" || document.forms[0].encryption.value == "WPA2PSK" || document.forms[0].encryption.value == "WPAPSKWPA2PSK"){
				if(wpapskCheck(document.forms[0].password) == false){
					alert("Senha inválida. Deve conter de 8 a 63 caracteres.");
					if(isOIWizardC9Support == "Yes" || isOIWizardC7Support == "Yes") 
					{
						document.forms[0].password.value = "";
						document.forms[0].repeat_password.value = "";
					}
					return;
				}
			}

			if(document.forms[0].password.value != document.forms[0].repeat_password.value)
			{
				alert("Repita a senha corretamente.");
				if(isOIWizardC9Support == "Yes" || isOIWizardC7Support == "Yes") 
				{
					document.forms[0].password.value = "";
					document.forms[0].repeat_password.value = "";
				}
				return;
			}

			document.forms[0].submit();
		}   

		function doInit()
		{
			var tempMacAddr = "<% tcWebApi_get("Info_Ether","mac","s") %>";
			var macAddr = tempMacAddr.split(":");
			document.forms[0].network.value = "Oi_Velox_WiFi_"+ macAddr[4].toUpperCase() + macAddr[5].toUpperCase();
			document.forms[0].encryption.selectedIndex = 3;
			doEncryptionChange();
		}

		function doEncryptionChange()
		{
			var passwd = document.getElementById("wifiPass");
			var repasswd = document.getElementById("reWifiPass");
			var descripObj = document.getElementById("dynDescrip");
			if(document.forms[0].encryption.value == "OPEN")
			{
				 if(confirm("Você selecionou uma opção não segura. É recomendado cadastrar uma senha para proteger sua rede. Deseja prosseguir mesmo assim? ")== true)
				 {			
				passwd.style.display="none";
				repasswd.style.display="none";
			}
			else
			{
					document.forms[0].encryption.selectedIndex = 3;
					doEncryptionChange();
				}
				return;
			}
			else
			{
				passwd.style.display="";
				repasswd.style.display="";
				if(document.forms[0].encryption.selectedIndex == 1)
				{
					descripObj.innerHTML = "5 ou 13 caracteres";
				}
				else
				{
					descripObj.innerHTML = "8 a 63 caracteres";
				}
			}
		}
		function checkpassword()
		{
				var isOIWizardC9Support = "<%tcWebApi_get("WebCustom_Entry","isOIWizardC9Support","s")%>";
				var isOIWizardC7Support = "<%tcWebApi_get("WebCustom_Entry","isOIWizardC7Support","s")%>";

		 	  if((document.forms[0].password.value.length != 0) && (document.forms[0].repeat_password.value.length != 0)
		 	  && (document.forms[0].password.value != document.forms[0].repeat_password.value))
			  {
					alert("Repita a senha corretamente.");
					if(isOIWizardC9Support == "Yes" || isOIWizardC7Support == "Yes") 
					{
						document.forms[0].password.value = "";
						document.forms[0].repeat_password.value = "";
					}
					return;
			  }
				
			  if(document.forms[0].encryption.selectedIndex == 1 ){
					if(doWepCheck() == false){
					alert("Senha inválida. Deve conter exatamente 5 ou 13 caracteres.");
					if(isOIWizardC9Support == "Yes" || isOIWizardC7Support == "Yes") 
					{
						document.forms[0].password.value = "";
						document.forms[0].repeat_password.value = "";
					}
					return;
					}
			  }		
			
			  if (document.forms[0].encryption.value == "WPAPSK" || document.forms[0].encryption.value == "WPA2PSK" || document.forms[0].encryption.value == "WPAPSKWPA2PSK"){
					if(wpapskCheck(document.forms[0].password) == false){
					alert("Senha inválida. Deve conter de 8 a 63 caracteres.");
					if(isOIWizardC9Support == "Yes" || isOIWizardC7Support == "Yes") 
					{
						document.forms[0].password.value = "";
						document.forms[0].repeat_password.value = "";
					}
					return;
					}
			  }			
		}		
			
		</script>
    </head>    
    <body onLoad="doInit()">
  
        <!--BEGIN LAYOUT BG-->
        <div id="layout_bg_left" class="layout">
            <div id="layout_bg_right" class="layout">
                &nbsp;
            </div>
        </div>
        <!--END LAYOUT BG-->

         <table border="0" cellpadding="0" cellspacing="0" width="100%" height="100%" class="layout_pages">
             <tr>
                <td height="94" valign="top" align="left">

                <!--BEGIN LAYOUT TOP-->
                <div id="layout_top">
          
                    <!--BEGIN CONTENT-->
                    <div id="layout_top_content">
                        <img src="../../include/images/layout_top.png" width="181" height="60" title="OI VELOX" alt="OI VELOX">
                    </div>
                    <!--END CONTENT-->
                
                </div>
                <!--END LAYOUT TOP-->
   
            	</td>
            </tr>
            <tr>
            	<td valign="middle">

                <!--BEGIN LAYOUT PAGES CONTENT-->
                <div id="layout_content">

                <!--BEGIN PAGE CONTENT-->
                <div class="page_content">
                    <div id="page_configure_wifi_content">
					
                  <!--BEGIN TITLE-->
                        <div class="page_title">
                            Para configurar a sua rede Wi-Fi, preencha os campos abaixo:
                        </div>
                        <!--END TITLE-->
                     
                        <!--BEGIN FORM-->
                        <div class="page_form">                 
                            <form name="configure_modem_residen" method="post" action="/cgi-bin/WizardOi/configurando.asp" >                   
                                <!--BEGIN FORM NETWORK-->
                                <div class="page_form_line">              
                                    <div class="page_form_line_name page_form_name_configure_wifi">Nome da Rede</div>
                                    <div class="page_form_line_field page_form_line_field_input"><input type="text" name="network" id="network" maxlength="32" size="32" value="<%if tcWebApi_get("WLan_Entry0","SSID","h") <> "N/A" then  tcWebApi_get("WLan_Entry0","SSID","s") end if%>" /></div>
									<div class="page_form_line_description">Escolha um nome para sua rede Wi-Fi*</div>                      
                                </div>
                                <!--END FORM NETWORK-->
                     
                                <!--BEGIN FORM ENCRYPTION-->
                                <div class="page_form_line">                         
                                    <div class="page_form_line_name page_form_name_configure_wifi">Criptografia</div>
                                    <div class="page_form_line_field">
                                    	<select id="encryption" name="encryption" class="page_form_encryption" onchange="doEncryptionChange()">
                                            <option  value="OPEN" <%if tcWebApi_get("WLan_Entry","AuthMode","h") = "OPEN" then asp_Write("selected") end if%> >Sem seguran&ccedil;a</option>
											<option  value="WEP-64Bits" <%if tcWebApi_get("WLan_Entry","AuthMode","h") = "WEP-64Bits" then asp_Write("selected") elseif tcWebApi_get("WLan_Entry","AuthMode","h") = "WEP-128Bits" then asp_Write("selected")end if%>>WEP-64/WEP-128</option>
											<option  value="WPAPSK" <%if tcWebApi_get("WLan_Entry","AuthMode","h") = "N/A" then asp_Write("selected") elseif tcWebApi_get("WLan_Entry","AuthMode","h") = "WPAPSK" then asp_Write("selected") end if %>>WPA-PSK</option>
											<option   value="WPA2PSK" <%if tcWebApi_get("WLan_Entry","AuthMode","h") = "WPA2PSK" then asp_Write("selected") end if %>>WPA2-PSK</option>
											<option   value="WPAPSKWPA2PSK" <%if tcWebApi_get("WLan_Entry","AuthMode","h") = "WPAPSKWPA2PSK" then asp_Write("selected") end if %>>WPA-PSK/WPA2-PSK</option>

                                        </select>
                                    </div>
                                    <div class="page_form_line_description">Escolha a chave de seguran&ccedil;a da sua Rede</div>                                                                
                                </div>
                                <!--END FORM ENCRYPTION-->
                           
                                <!--BEGIN FORM PASSWORD-->
                                <div class="page_form_line" id="wifiPass">                        
                                    <div class="page_form_line_name page_form_name_configure_wifi">Senha</div>
                                    <div class="page_form_line_field page_form_line_field_input"><input type="password" name="password" id="password" size="28" maxlength="64" /></div>

                                    <div class="page_form_line_description">Escolha uma senha para a sua rede Wi-Fi (<span id="dynDescrip">X a X caracteres</span>)</div>                    
                                </div>
                                <!--END FORM PASSWORD-->
                       
                                <!--BEGIN FORM REPEAT PASSWORD-->
                                <div class="page_form_line" id="reWifiPass">                           
                                    <div class="page_form_line_name page_form_name_configure_wifi">Repetir Senha</div>
                                    <div class="page_form_line_field page_form_line_field_input"><input type="password" name="repeat_password" id="repeat_password" size="28" maxlength="64" onblur=checkpassword() /></div>                           
                                </div>
                                <!--END FORM REPEAT PASSWORD-->  
                            </form>                        
                        </div>
                        <!--END FORM-->
                      
                        <!--BEGIN HELP-->
                        <div class="page_help">
                       
                            <!--BEGIN HELP ICON-->
                            <div class="page_help_icon">
                                <img src="../../include/images/page_icon_help.png" width="24" height="25" alt="Ajuda" />
                            </div>
                            <!--END HELP ICON-->  
                        
                            <!--BEGIN HELP TXT-->
                            <div class="page_help_txt" 
                            <%if tcWebApi_get("WebCustom_Entry","isOIWizardC9Support","h") = "Yes" then%>	
                            	style="font-family:TradeGothicBoldTwo; font-size: 16px;"
                            <%elseif	tcWebApi_get("WebCustom_Entry","isOIWizardC7Support","h") = "Yes" then%>
                            	style="font-family:TradeGothicBoldTwo; font-size: 16px;"
                            <%else%>
                            <%end if%>
                            >
                                *Para mais informa&ccedil;&otilde;es consulte seu Guia de Instala&ccedil;&atilde;o.
                            </div>
                            <!--END HELP TXT-->   
                        
                        </div>
                        <!--END HELP-->                
                    </div>
                </div>
                <!--END PAGE CONTENT-->
 
                <!--BEGIN PAGE BTNS-->
                <div class="page_btns"> 
                    <a href="javascript:void(0)" onclick="navPage( 'set_modem_residential' )"><img src="../../include/images/page_btn_back.png" width="150" height="30" title="Voltar" alt="Voltar" onmouseover="this.src='../../include/images/page_btn_back_over.png'" onmouseout="this.src='../../include/images/page_btn_back.png'" /></a>                                     
                    <a href="javascript:void(0)" onclick="PageSubmit()" ><img src="../../include/images/page_btn_forward.png" width="166" height="30" title="Avan&ccedil;ar" alt="Avan&ccedil;ar" onmouseover="this.src='../../include/images/page_btn_forward_over.png'" onmouseout="this.src='../../include/images/page_btn_forward.png'" /></a>           
                </div>
                <!--END PAGE BTNS-->

                </div>
                <!--BEGIN LAYOUT PAGES CONTENT-->
   
                </td>
            </tr>
        </table>
    </body>   
</html>
