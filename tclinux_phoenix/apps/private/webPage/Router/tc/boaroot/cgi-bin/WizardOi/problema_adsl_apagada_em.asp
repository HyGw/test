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
                     
                <div style="width:850px; *padding: 45px 0 45px 0;">
                <div>
                    <div id="page_adsl_off_problem_content">
                                   
                        <div class="page_title page_table_title">
                            Solucionando Problemas
                        </div>
                                 
                        <div class="page_table">
                                            
                            <div class="page_table_problem">
                            
                                <div class="page_table_top">
                                    Problema
                                </div>
                                
                                <div class="page_table_problem_content">
                                     Luz ADSL <br />do modem apagada <br />ou piscando
                                </div>
                            </div>
                   
                            <div class="page_table_steps">
                   
                                <div class="page_table_top">
									Nem sempre &eacute; necessária a visita de um t&eacute;cnico:
                                </div>
                      
                                <div class="page_table_steps_content">     
                                		<div class="page_table_steps_list">
											Acredite: vários problemas podem ser resolvidos por voc&eacute; mesmo, de forma fácil e rápida, apenas com alguns procedimentos. Verifique abaixo a solução para o seu problema.
                                    </div>
									
									<div class="page_table_steps_list">
											<img src="../../include/images/page_table_list_arrow.png" width="5" height="10" alt="Seta" />
											Verifique se o sinal do Oi Velox j&aacute; foi liberado pra sua linha, ele &eacute; liberado em 72 horas após adquirir o Oi Velox.
                                    </div>
									<%if tcWebApi_get("WebCustom_Entry","isOIWizardC9Support","h") = "Yes" then%>	
									<%elseif	tcWebApi_get("WebCustom_Entry","isOIWizardC7Support","h") = "Yes" then%>
									<%else%>
									<div class="page_table_steps_list">
											<img src="../../include/images/page_table_list_arrow.png" width="5" height="10" alt="Seta" />
											Veja se o cabo Ethernet est&aacute; conectado ao computador (placa de rede Ethernet)e ao modem (verifique se a luz ethernet est&aacute; acesa).
                                    </div>
									<%end if%>
                                		<div class="page_table_steps_list">
											<img src="../../include/images/page_table_list_arrow.png" width="5" height="10" alt="Seta" />
											Veja se o cabo de telefone est&aacute; conectado ao modem e &agrave; linha de telefone.
                                    </div>	
                                    
                                            
                                    <div class="page_table_steps_list">
											<img src="../../include/images/page_table_list_arrow.png" width="5" height="10" alt="Seta" />
                                        Veja se o microfiltro est&aacute; conectado ao telefone.
                                    </div>
                                    
                                    
                                    <div class="page_table_steps_list">
											<img src="../../include/images/page_table_list_arrow.png" width="5" height="10" alt="Seta" />
                                        Aguarde cerca de 60 segundos ap&oacute;s ligar o modem e verifique se a luz ADSL est&aacute; apagada ou piscando em intervalos regulares.
                                    </div>
                                    
                                    <div class="page_table_steps_list page_table_steps_list_bold">
											<img src="../../include/images/page_table_list_arrow.png" width="5" height="10" alt="Seta" />
                                        Se estiver, verifique:<br />
                                        <div class="page_table_steps_sublist">
                                            &middot; Se o cabo de telefone est&aacute; enferrujado ou em m&aacute;s condi&ccedil;&otilde;es. Caso seja necess&aacute;rio, efetue a troca do cabo ou da tomada de telefone.<br />
                                            &middot; Se os microfiltros est&atilde;o conectados a todos os aparelhos de telefone, secret&aacute;rias eletr&ocirc;nicas e ao Set Top Box (decodificador de TV a cabo).<br />
                                            &middot; Desligue o modem e ligue-o novamente.
                                        </div>
                                    </div>
                                </div>
                            </div>
                            <div class="page_table_clear"></div>                     
                        </div>
                    </div>
                </div>
     
                <div class="page_btns">    
                    <a href="javascript:void(0)" onclick="navPage( 'authentication_failure_velox_videoem' );"><img src="../../include/images/page_btn_back.png" width="150" height="30" title="Voltar" alt="Voltar" onmouseover="this.src='../../include/images/page_btn_back_over.png'" onmouseout="this.src='../../include/images/page_btn_back.png'" /></a>
                </div>
                </div>
                </td>
            </tr>
        </table>
    </body>   
</html>
