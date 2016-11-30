<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01//EN" "http://www.w3.org/TR/html4/strict.dtd">
<html lang="pt-br">
    <head> 	
		<meta http-equiv=Content-Type content="text/html; charset=UTF-8">
		<meta http-equiv="Pragma" content="no-cache"> 
            
        <title>
		OI VELOX 
		</title>
  
        <link rel="shortcut icon" href="favicon.ico" >
     
        <link href="../../include/css/style.css" rel="stylesheet" type="text/css" />
        <link href="../../include/css/layout.css" rel="stylesheet" type="text/css" />
        <link href="../../include/css/home.css" rel="stylesheet" type="text/css" />
        <link href="../../include/css/pages.css" rel="stylesheet" type="text/css" />    
        <script src="../../include/scripts/main.js" type="text/javascript"></script>      
    </head>  
    <body>   
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
                    <div id="page_user_password_problem_content">
               
                        <div class="page_title page_table_title">
                            Solucionando Problemas
                        </div>
                
                        <div class="page_table">
                         
                            <div class="page_table_problem">
                  
                                <div class="page_table_top">
                                    Problema
                                </div>
                          
                                <div class="page_table_problem_content">
                                    Erro de usu&aacute;rio e<br />
                                    de senha
                                </div>
                       
                            </div>
                   
                            <div class="page_table_steps">
                  
                                <div class="page_table_top">
                                    Antes de chamar o t&eacute;cnico:
                                </div>
                                
                                <div class="page_table_steps_content">
                              
                                    <div class="page_table_steps_list_title">
                                    <%if tcWebApi_get("WebCustom_Entry","isOIWizardC9Support","h") = "Yes" then%>
                                    	<%if tcWebApi_get("GUI_Entry0", "OI_USER_ENTERPRISE", "h") <> "1" then %>
                                    		CLIENTE RESIDENCIAL
                                    	<%else%>
                                    		CLIENTE EMPRESARIAL
                                    	<%end if%>
                                    <%elseif tcWebApi_get("WebCustom_Entry","isOIWizardC7Support","h") = "Yes" then%>
                                    	<%if tcWebApi_get("GUI_Entry0", "OI_USER_ENTERPRISE", "h") <> "1" then %>
                                    		CLIENTE RESIDENCIAL
                                    	<%else%>
                                    		CLIENTE EMPRESARIAL
                                    	<%end if%>
                                    <%else%>
											CLIENTE RESIDENCIAL/EMPRESARIAL
                                    <%end if%>
                                    </div>
                            
                                    <div class="page_table_steps_list">
												<img src="../../include/images/page_table_list_arrow.png" width="5" height="10" alt="Seta" />
                                        Veja se voc&ecirc; digitou corretamente o usu&aacute;rio e a senha do seu provedor de acesso &agrave; internet. <br />
										Se o usu&aacute;rio e a senha n&atilde;o estiverem digitados corretamente, apague e digite novamente. Se n&atilde;o funcionar, entre em contato com o seu provedor.
									</div>	
                                    
                                    <div class="page_table_steps_list">
												<img src="../../include/images/page_table_list_arrow.png" width="5" height="10" alt="Seta" />
                                        Confira se a mensalidade do seu provedor est&aacute; em dia, caso n&atilde;o seja gratuito.
                                    </div>
                        
                                    </div>
                       
                            </div>
                 
                            <div class="page_table_clear"></div>
                 
                        </div>
            
                    </div>
                </div>

                <div class="page_btns">
                    <a href="javascript:void(0)" onclick="navPage( 'authentication_failure' );"><img src="../../include/images/page_btn_back.png" width="150" height="30" title="Voltar" alt="Voltar" onmouseover="this.src='../../include/images/page_btn_back_over.png'" onmouseout="this.src='../../include/images/page_btn_back.png'" /></a>
                </div>

                </div>
                </td>
            </tr>
        </table>
    </body>  
</html>
