<%
	if request_form("fonSubmit_Flag") = "Yes"  Then
	tcWebApi_constSet("GUI_Entry0","FonSetted","1")
	if tcWebApi_get("FonNet_Entry","Enable","h") = "No" Then
	tcWebApi_Set("FonNet_Entry","Enable","fonSubmit_Flag")
	tcWebApi_Commit("FonNet")
	else
		tcWebApi_save()	
	end if
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
        <script language="JavaScript">
         function GetFonString()
		 {
			var iFrame =  document.getElementById('fonStringIframe'); 
			if ( iFrame.contentDocument )
			{ // FF
				  iFrameDiv = iFrame.contentDocument.getElementsByTagName('div')[0]; 
			}else if ( iFrame.contentWindow )
			{ // IE
				  iFrameDiv = iFrame.contentWindow.document.getElementsByTagName('div')[0];  
			} 
			if(iFrameDiv.innerHTML != "")
				document.getElementById('dynText').innerHTML=iFrameDiv.innerHTML;
		} 
	    </script> 
	      
    </head>
    
    
    <body>
    
    
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
                    <div id="page_registered_commercial_content">
                    
                        
                        <!--BEGIN TITLE-->
                        <div class="page_congratulations_title">
                        	<img src="../../include/images/page_congratulations_title.png" width="224" height="40" alt="Parab&eacute;ns" />
                        </div>
                        <!--END TITLE-->
                        
                        
                        <!--BEGIN SUBTITLE-->
                        <div class="page_subtitle_special">
                            Seu cadastro foi realizado com sucesso. 
                        </div>
                        <!--END SUBTITLE-->
                        
                        
                        <!--BEGIN TXT-->
                        <div class="page_txt">
                        	<br /><br />
                        <%if tcWebApi_get("WebCustom_Entry","isOIWizardC9Support","h") = "Yes" then%>
                        <span class="page_txt_highlights">Importante: Anote seus dados para acessar a Rede Fon:</span><br />
                        Login: n&uacute;mero do seu Oi Fixo com DDD. Se tiver mais de um Oi Fixo, 
                        digite o n&uacute;mero daquele que est&aacute; vinculado ao seu Oi Velox<br />
                        Senha: n&uacute;mero do CNPJ.<br /><br /><br />
                        <%elseif tcWebApi_get("WebCustom_Entry","isOIWizardC7Support","h") = "Yes" then%>
                        <span class="page_txt_highlights">Importante: Anote seus dados para acessar a Rede Fon:</span><br />
                        Login: n&uacute;mero do seu Oi Fixo com DDD. Se tiver mais de um Oi Fixo, 
                        digite o n&uacute;mero daquele que est&aacute; vinculado ao seu Oi Velox<br />
                        Senha: n&uacute;mero do CNPJ.<br /><br /><br />
                        <%else%>
                        	<span class="page_txt_highlights">Importante: Anote seus dados para acessar a Rede Fon:</span><br />
              <b>Login: n&uacute;mero do seu Oi Fixo com DDD.</b> Se tiver mais de um Oi Fixo, 
              digite o n&uacute;mero daquele que est&aacute; vinculado ao seu Oi Velox<br />
              <b>Senha: n&uacute;mero do CNPJ</b><br />
              <br /><br />
                        <%end if%>
                         <div class="page_txt"  id="dynText">
                            <p>Em seu primeiro acesso a qualquer um dos pontos Oi WiFi da Rede Fon, voc&ecirc; dever&aacute; alterar sua senha e cadastrar um e-mail v&aacute;lido.<br />
                            Mais informa&ccedil;&otilde;es em <a href="http://www.oi.com.br/oipranegocios" target="_blank">http://www.oi.com.br/oipranegocios</a>.</p>
                        </div>
                        </div>
                        <!--END TXT-->
                        
                    
                    </div>
                </div>
                <!--END PAGE CONTENT-->
        
        	<div style="display:none">
                 <iframe id="fonStringIframe" name="fonStringIframe" frameborder="0"  src="/cgi-bin/getOutString.cgi?index=3"  onload="GetFonString();"></iframe>
		</div>
                <!--BEGIN PAGE BTNS-->
                <div class="page_btns">               
                    <a href="javascript:void(0)" onclick="navPage( 'browsingem' )" ><img src="../../include/images/page_btn_navigate.png" width="166" height="30" title="Navegar" alt="Navegar" onmouseover="this.src='../../include/images/page_btn_navigate_over.png'" onmouseout="this.src='../../include/images/page_btn_navigate.png'" /></a>
                </div>
                <!--END PAGE BTNS-->
                </div>
                <!--BEGIN LAYOUT PAGES CONTENT-->
                </td>
            </tr>
        </table>
    </body>
</html>
