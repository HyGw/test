<%
	tcWebApi_constSet("GUI_Entry0","wzOISetted","1")
	if request_form("fonSubmit_Flag") = "No"  Then
	if tcWebApi_get("FonNet_Entry","Enable","h") = "Yes" Then
	tcWebApi_Set("FonNet_Entry","Enable","fonSubmit_Flag")
	tcWebApi_Commit("FonNet")
	end if
	end if
	tcWebApi_save()
%>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01//EN" "http://www.w3.org/TR/html4/strict.dtd">
<html lang="pt-br">   
    <head>	
        <!-- METAS -->
        <meta charset="utf-8">
		<meta http-equiv="Pragma" content="no-cache">
		<meta http-equiv="Expires" content="-1">
        <!-- TITLE -->
        <title>OI VELOX</title>
    
        <!-- FAVICON -->
        <link rel="shortcut icon" href="favicon.ico" >
      
        <!-- CSS -->
        <link href="../../include/css/style.css" rel="stylesheet" type="text/css" />
        <link href="../../include/css/layout.css" rel="stylesheet" type="text/css" />
        <link href="../../include/css/home.css" rel="stylesheet" type="text/css" />
        <link href="../../include/css/pages.css" rel="stylesheet" type="text/css" />
    
        <!-- MAIN -->
        <script src="../../include/scripts/main.js" type="text/javascript"></script>
		<script language="JavaScript">
		<%if request_form("fonSubmit_Flag") = "No"  Then %>
			navPage( 'configured' );
		<%end if%>
		var xmlHttp; 
		var t1;  
		function fonActiveDeactive()
		{
			if(document.WLANForm.rede_fon[0].checked)
			{
				var userFlag = "<% tcWebApi_get("GUI_Entry0","OI_USER_ENTERPRISE","s") %>";
				if(userFlag==1)
				{
					document.WLANForm.action="cadastrado_comercial.asp";
					document.WLANForm.fonSubmit_Flag.value = "Yes";
					document.WLANForm.submit();		
				}
				else
				{
					document.WLANForm.action="cadastrado_residencial.asp";
					document.WLANForm.fonSubmit_Flag.value = "Yes";
					document.WLANForm.submit();		
				}
			}
			else
			{
				document.WLANForm.fonSubmit_Flag.value = "No";
				document.WLANForm.submit();		
			}		
		}
		
   		function doFonPost()
		{
				t1 = setTimeout("doFonFail(0);", 6*1000);
				if(document.WLANForm.rede_fon[0].checked)				
					loadXML("/cgi-bin/FonPost.cgi?fonAccept=YES",true);
				else
					loadXML("/cgi-bin/FonPost.cgi?fonAccept=NO",true);
		}
		
		function doFonEntry()
		{
			var postResultDiv =  document.getElementById('postResult'); 
			if (xmlHttp.readyState == 4)
			{  
				if (xmlHttp.status == 200)
				{  
			        var response = xmlHttp.responseText;  
			        postResultDiv.innerHTML = response;
			        
			        var result=postResultDiv.getElementsByTagName('div')[0]; 
			        if(result.innerHTML == "OK")
					{
						if(t1) clearTimeout(t1);
						fonActiveDeactive();
					}
					else if(result.innerHTML != "")
					{
						if(t1)clearTimeout(t1);
						doFonFail(1);
					}
					else
					{
						if(t1)clearTimeout(t1);
						doFonFail(0);
					}
			    }
			    else
			    {
					if(t1) clearTimeout(t1);
			      	doFonFail(1);
			    } 
    		}   			
		}
		
		function loadXML(fileName,asynFlag)  
		{ 			
			try
			{
				// Safari, chrome
				xmlHttp=new XMLHttpRequest();
			}
			catch (e)
			{
				// Internet Explorer
				try
				{
					xmlHttp=new ActiveXObject("Msxml2.XMLHTTP");
				}
				catch (e)
				{
					try
					{
					 	xmlHttp=new ActiveXObject("Microsoft.XMLHTTP");
					}
					catch (e)
					{
						alert('Your browser can\'t handle this script');
						return;
					}
				}
			}		
		
			xmlHttp.open("GET", fileName, asynFlag);
			xmlHttp.onreadystatechange = doFonEntry;
			xmlHttp.send(null);	
		}
		
		function doFonFail(flag)
		{
				if(xmlHttp) xmlHttp.abort();
				alert("Ocorreu um erro durante o seu registo na FON, tente novamente mais tarde através do endereço http://192.168.1.1/fon");
				<%if tcWebApi_get("GUI_Entry0","FonSetted","h") <> "1" then %>
				navPage( 'configured' );
				<%else%>
				if(flag==1)
				{
					navPage( 'site' );
				}
				else
				{
					<%if tcWebApi_get("GUI_Entry0", "OI_USER_ENTERPRISE", "h") <> "1" then %>
					navPage( 'browsingu' );
					<%else%>
					navPage( 'browsingem' );
					<%end if%>
				}
				<%end if%>
		}
		var updated = false;
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
			if((typeof(iFrameDiv) != "undefined") && (iFrameDiv.innerHTML != "") )
			{
				document.getElementById('dynText').innerHTML=iFrameDiv.innerHTML;
				updated = true;
			}
		} 
		function doJump()
		{
			//if(updated == true)
				navPage( 'contract' );
		} 
		
		<%if tcWebApi_get("WebCustom_Entry","isOIWizardC9Support","h") = "Yes" then%>
		if ( top.location.pathname != "/fon")
		{
			top.location.href = "/fon";
			top.location.pathname = "/fon";
		}
		<%elseif tcWebApi_get("WebCustom_Entry","isOIWizardC7Support","h") = "Yes" then%>
		if ( top.location.pathname != "/fon")
		{
			top.location.href = "/fon";
			top.location.pathname = "/fon";
		}
		<%else%>
		<%end if%>
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
                <div class="page_content" style="height:500px">
                    <div id="page_rede_fon_content">
                  
                        <!--BEGIN TXT-->
                        <div class="page_txt" id="dynText">						
                            <p><span class="page_txt_highlights">Acesso WiFi Ilimitado com a Rede Fon.</span> Com Oi Velox voc&ecirc; faz parte da Rede Fon, a maior rede WiFi comunit&aacute;ria do mundo. S&atilde;o mais de 5 milh&otilde;es de pontos de acesso para voc&ecirc; se conectar  e aproveitar a internet tamb&eacute;m fora de casa.<br/></p>
                            <p><span class="page_txt_highlights">Participe e tenha Internet dentro e fora de casa.</span> Fazendo parte da Rede Fon voc&ecirc; compartilha uma parte da sua conex&atilde;o de banda larga e se torna um ponto de acesso WiFi. Assim, quando estiver fora de casa, voc&ecirc; tem acesso aos outros milhares de pontos WiFi da Rede Fon no Brasil e no mundo.<br/></p>
                            <p><span class="page_txt_highlights">Compartilhar &eacute; seguro.</span> Voc&ecirc; ter&aacute; duas redes sem fio independentes: uma particular - protegida por uma senha de seguran&ccedil;a que s&oacute; voc&ecirc; conhece - e outra comunit&aacute;ria, que somente outros usu&aacute;rios registrados na Rede Fon ter&atilde;o acesso.</p>     
                            <p><span class="page_txt_highlights">S&oacute; a Oi oferece esse servi&ccedil;o no Brasil. Clique em avan&ccedil;ar e fa&ccedil;a parte!</span></p>
                        </div>
                        <!--END TXT-->
                 
                        <!--BEGIN FORM-->
                        <div class="page_form page_form_rede_fon">                   
                            <form  name="WLANForm"  method="post">
                        	<input type="hidden" name="fonSubmit_Flag" value=""> 
                               <!--BEGIN FORM RADIO BUTTON-->
                                <div class="page_form_radio_button">
                                <%if tcWebApi_get("WebCustom_Entry","isOIWizardC9Support","h") = "Yes" then%>
                                <input name="rede_fon" type="radio" value="yes" checked /> Aceito o <a href="javascript:void(0)" onclick="doJump();">contrato de ades&atilde;o</a> e quero participar da Rede Fon.<br />
                                <input name="rede_fon" type="radio" value="no" /> N&atilde;o quero ter acesso gr&aacute;tis aos Milh&otilde;es de pontos WiFi da Rede Fon.
                                 <!--BEGIN PAGE BTNS-->
                                <div class="page_btns" style="margin-top: -36px;">
                                <a href="javascript:void(0)" onclick="doFonPost()" ><img style="margin-right: 60px;" src="../../include/images/page_btn_forward.png" width="166" height="30" title="Avan&ccedil;ar" alt="Avan&ccedil;ar" onmouseover="this.src='../../include/images/page_btn_forward_over.png'" onmouseout="this.src='../../include/images/page_btn_forward.png'" /></a>
                                </div>
                               <!--END PAGE BTNS-->
                                <%elseif tcWebApi_get("WebCustom_Entry","isOIWizardC7Support","h") = "Yes" then%>
                                <input name="rede_fon" type="radio" value="yes" checked /> Aceito o <a href="javascript:void(0)" onclick="doJump();">contrato de ades&atilde;o</a> e quero participar da Rede Fon.<br />
                                <input name="rede_fon" type="radio" value="no" /> N&atilde;o quero ter acesso gr&aacute;tis aos Milh&otilde;es de pontos WiFi da Rede Fon.
                                 <!--BEGIN PAGE BTNS-->
                                <div class="page_btns" style="margin-top: -36px;">
                                <a href="javascript:void(0)" onclick="doFonPost()" ><img style="margin-right: 60px;" src="../../include/images/page_btn_forward.png" width="166" height="30" title="Avan&ccedil;ar" alt="Avan&ccedil;ar" onmouseover="this.src='../../include/images/page_btn_forward_over.png'" onmouseout="this.src='../../include/images/page_btn_forward.png'" /></a>
                                </div>
                               <!--END PAGE BTNS-->
                                <%else%>
                                <input name="rede_fon" type="radio" value="yes" checked /> Aceito o <a href="javascript:void(0)" onclick="doJump();">contrato de ades&atilde;o</a> e quero participar da Rede Fon<br />
                                    <input name="rede_fon" type="radio" value="no" /> N&atilde;o quero ter acesso gr&aacute;tis aos Milh&otilde;es de pontos Wi-Fi da Rede Fon.                                  
                                <%end if%>
                             </div>
                                <!--END FORM RADIO BUTTON-->
                                
                          </form>
                      </div>
                        <!--END FORM-->
                 </div>
                </div>
                <!--END PAGE CONTENT-->
        <div class="page_table_clear"></div> 
				<div style="display:none">
				<div id="postResult"></div>                    
                 	        <iframe id="fonStringIframe" name="fonStringIframe" frameborder="0"  src="/cgi-bin/getOutString.cgi?index=1"  onload="GetFonString();"></iframe>
				</div>
                <%if tcWebApi_get("WebCustom_Entry","isOIWizardC9Support","h") = "Yes" then%>
                <%elseif tcWebApi_get("WebCustom_Entry","isOIWizardC7Support","h") = "Yes" then%>
                <%else%>
                <!--BEGIN PAGE BTNS-->
                <div class="page_btns">
                   <a href="javascript:void(0)" onclick="doFonPost()" ><img src="../../include/images/page_btn_forward.png" width="166" height="30" title="Avan&ccedil;ar" alt="Avan&ccedil;ar" onmouseover="this.src='../../include/images/page_btn_forward_over.png'" onmouseout="this.src='../../include/images/page_btn_forward.png'" /></a>
             </div>
                <!--END PAGE BTNS-->
                <%end if%>

                </div>
                <!--BEGIN LAYOUT PAGES CONTENT-->
     
                </td>
            </tr>
        </table>
    </body>   
</html>
