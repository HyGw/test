<%
	tcWebApi_constSet("GUI_Entry0","FonSetted","1")
	tcWebApi_save()
%>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01//EN" "http://www.w3.org/TR/html4/strict.dtd">
<html lang="pt-br">
    <head>
		<meta http-equiv=Content-Type content="text/html; charset=iso-8859-1">
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
                <div id="layout_content">
                <div class="page_content">
                    <div id="page_configured_content">
                        <div class="page_configured_title">
                            <img src="../../include/images/page_congratulations_title.png" width="224" height="40" alt="Configurado" />
                   	  </div>
                        
                        <div class="page_subtitle_special page_configured_txt">
                            Seu modem foi instalado com sucesso.
                        </div>
                    
                    </div>
                </div>

                <div class="page_btns">
				<%if tcWebApi_get("GUI_Entry0", "OI_USER_ENTERPRISE", "h") <> "1" then %>
                    <a href="javascript:void(0)" onclick="navPage( 'browsingu' )" ><img src="../../include/images/page_btn_navigate.png" width="176" height="30" title="Iniciar navega&ccedil;&atilde;o" alt="Iniciar navega&ccedil;&atilde;o" onmouseover="this.src='../../include/images/page_btn_navigate_over.png'" onmouseout="this.src='../../include/images/page_btn_navigate.png'" /></a>
				<%else%>
					<a href="javascript:void(0)" onclick="navPage( 'browsingem' )" ><img src="../../include/images/page_btn_navigate.png" width="176" height="30" title="Iniciar navega&ccedil;&atilde;o" alt="Iniciar navega&ccedil;&atilde;o" onmouseover="this.src='../../include/images/page_btn_navigate_over.png'" onmouseout="this.src='../../include/images/page_btn_navigate.png'" /></a>
				<%end if%>
				</div>
                </div>
                </td>
            </tr>
        </table>
    </body>
</html>
