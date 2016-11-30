<%
	tcWebApi_constSet("GUI_Entry0","OI_USER_ENTERPRISE","0")
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

        <link href="../../include/css/style.css" rel="stylesheet" type="text/css" />
        <link href="../../include/css/layout.css" rel="stylesheet" type="text/css" />
        <link href="../../include/css/home.css" rel="stylesheet" type="text/css" />
        <link href="../../include/css/pages.css" rel="stylesheet" type="text/css" />

        <script src="../../include/scripts/main.js" type="text/javascript"></script>    
		 <script type="text/javascript" language="JavaScript">
		 function doSubmit()
		 {
				navPage( 'set_modem_residential' );
		 }
		 </script>  		
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
                    <div id="page_welcome_content">
                        <div class="page_orador_oi_title">
						<img src="../../include/images/page_welcome_title.png" width="57" height="34" alt="Oi" />
                      </div>
                        <div class="page_subtitle_special">
                        	<%if tcWebApi_get("WebCustom_Entry", "isCZH108LSupported", "h") = "Yes" then %>
                        		Seja bem-vindo ao assistente de configura&ccedil;&atilde;o do modem ZXHN H108L do seu Oi Velox.<br />
														Vamos come&ccedil;ar? Feche os demais programas que estiverem abertos pra iniciar a instala&ccedil;&atilde;o.
													<%else%>	
                            Seja bem-vindo ao assistente de configura&ccedil;&atilde;o do modem ZTE W300S do seu Oi Velox.<br />
														Vamos come&ccedil;ar? Feche os demais programas que estiverem abertos pra iniciar a instala&ccedil;&atilde;o.
													<%end if%>	
                        </div>
                    </div>
                </div>

                <div class="page_btns">
						<a href="javascript:void(0)" onclick="navPage( 'backhome' )"><img src="../../include/images/page_btn_back.png" width="150" height="30" title="Voltar" alt="Voltar" onmouseover="this.src='../../include/images/page_btn_back_over.png'" onmouseout="this.src='../../include/images/page_btn_back.png'" /></a>
						<a href="javascript:void(0)" onclick="doSubmit()" ><img src="../../include/images/page_btn_forward.png" width="166" height="30" title="Avan&ccedil;ar" alt="Avan&ccedil;ar" onmouseover="this.src='../../include/images/page_btn_forward_over.png'" onmouseout="this.src='../../include/images/page_btn_forward.png'" /></a>
                </div>
                </div>
                </td>
            </tr>
        </table>
    </body>
</html>
