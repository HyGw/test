function doPageclose()
{
	if (window.navigator.userAgent.indexOf("MSIE")>=1)
	{
		window.top.close();
		return;
	}
	
	if (window.navigator.userAgent.indexOf("Firefox")>=1)
	{
		window.top.opener=self;
		window.top.open('','_self');
		window.top.close();
		alert("Por favor feche o browser.");
		return;
	}
	
	if (window.navigator.userAgent.indexOf("Safari")>=1)
	{
		window.top.opener=self;
		window.top.open('','_self');
		window.top.close();
		return;
	}
	
	//others
	window.open('','_parent','');
	window.close(); 
}

function navPage( page )
{
	var addr = null;

	// DEFINE PAGE	
	switch( page )
	{
		
		// PAGES
		case "home":
	  		navPageLink( "index.asp" );
	  		break;
	  		
	  	case "welcome_homeuser":
	  		navPageLink( "bem_vindo_usr.asp" );
	  		break;

	  	case "welcome_comer":
	  		navPageLink( "bem_vindo_comer.asp" );
	  		break;
		
	  	case "detecting":
	  		navPageLink( "detectando.asp" );
	  		break;
	  		
		case "set_modem_residential":
	  		navPageLink( "configurar_modem_residencial.asp" );
	  		break;
	  		
		case "set_modem_commercial":
	  		navPageLink( "configurar_modem_comercial.asp" );
	  		break;
	  			
	  	case "host":
	  		navPageLink( "provedor.asp" );
	  		break;
	  		
		case "authentication_failure":
	  		navPageLink( "falha_autenticacao.asp" );
	  		break;
	  		
		case "authentication_failureem":
	  		navPageLink( "falha_autenticacao_em.asp" );
	  		break;	
	  			
		case "authentication_failure_velox":
	  		navPageLink( "falha_conexao_velox_video.asp" );
	  		break;
	  		
		case "authentication_failure_velox_video":
	  		navPageLink( "falha_conexao_velox_video.asp" );
	  		break;
			
		case "authentication_failure_velox_videoem":
	  		navPageLink( "falha_conexao_velox_videoem.asp" );
	  		break;
	  		
		case "interface_failure":
	  		navPageLink( "falha_interface.asp" );
	  		break;
	  			
		case "troubleshooting":
	  		navPageLink( "solucionando_problemas.asp" );
	  		break;
			
		case "modem_off_problem":
	  		navPageLink( "problema_modem_apagado.asp" );
	  		break;
	  			
		case "adsl_off_problem":
	  		navPageLink( "problema_adsl_apagada.asp" );
	  		break;
	  		
		case "adsl_off_problem_em":
	  		navPageLink( "problema_adsl_apagada_em.asp" );
	  		break;	  		
	  			
		case "ethernet_shield_problem":
	  		navPageLink( "problema_placa_de_rede.asp" );
	  		break;
	  			
		case "user_password_problem":
	  		navPageLink( "problema_usuario_senha.asp" );
	  		break;
	  			
		case "connectionless_problem":
	  		navPageLink( "problema_sem_conexao.v" );
	  		break;
	  		
		case "no_navigation_problem":
	  		navPageLink( "problema_sem_navegacao.asp" );
	  		break;
	  			
		case "slow_browsing_problem":
	  		navPageLink( "problema_navegacao_lenta.asp" );
	  		break;
	  		
	  	case "configure_wifi":
	  		navPageLink( "configurar_wifi.asp" );
	  		break;
	  		
		case "setting":
	  		navPageLink( "configurando.asp" );
	  		break;
			
		case "setting_em":
	  		navPageLink( "configurandoem.asp" );
	  		break;
	  		
		case "configured":
	  		navPageLink( "configurado.asp" );
	  		break;
	  		
		case "configuredem":
	  		navPageLink( "configuradoem.asp" );
	  		break;
	  		
		case "blank":
	  		navPageLink( "em_branco.asp" );
	  		break;
	  		
		case "rede_fon":
	  		navPageLink( "rede_fon.asp" );
	  		break;
	  		
		case "registered_residential":
	  		navPageLink( "cadastrado_residencial.asp" );
	  		break;
	  		
		case "registered_commercial":
	  		navPageLink( "cadastrado_comercial.asp" );
	  		break;
	  		
		case "modem_installed":
	  		navPageLink( "modem_instalado.asp" );
	  		break;

		case "mobicare":
			navPageLinkTarget( "/oiWzRedirect", "_blank" );
	  		break;

		// BTN ACTION
		case "close":
			doPageclose();
	  		break;
		case "retry":
	  		navPageLink( "configurando.asp" );		
	  		break;
	  			
		case "browsing":
	  		break;

		case "browsingu":
			window.top.open("http://www.oi.com.br/boasvindas_oipravoce","_self");
	  		break;
			
		case "browsingem":
			window.top.open("http://www.oi.com.br/boasvindas_oipranegocios","_self");
	  		break;
			
		case "backhome":
			navPageLink( "wizardoi.asp" );
	  		break;			
		case "back":
			navPageLink( "solucionando_problemas.asp" );
	  		break;

		// LINKS	
		case "video":
	  		navPageLink( "http://" );
	  		break;
	  		
		case "contract":
	  		window.top.open( "http://fon-instalacao.oi.com.br/contrato/contrato_de_adesao.html", "_blank");
	  		break;
			
		case "provider":
	  		window.top.open( "http://primeiroacesso.oi.com.br", "_self");
	  		break;			
	  			
		case "site":
	  		navPageLink( "http://www.oi.com.br/" );
	  		break;
	  		
		case "siteem":
	  		navPageLink( "http://www.oi.com.br/oipranegocios" );
	  		break;	  		
	}
}

function navPageLink( addr )
{
	// WINDOW TARGET
	var wTarget = "_self";
	if( addr.indexOf( "http://" ) > -1 ) wTarget = "_blank";

	// NAV
	if( addr ) window.open( addr, wTarget );
}

function navPageLinkTarget( addr, _wTarget)
{
	// WINDOW TARGET
	var wTarget = _wTarget;

	// NAV
	if( addr ) window.open( addr, wTarget );
}

function StringCheck(val)
{
    re = /^[^\s]+$/;
    if( re.test(val) )
        return true;
    else
        return false;
}

function quotationCheck(object, limit_len)
{
	var len = object.value.length;
	var c;
	var i, j = 0;
    for (i = 0; i < len; i++)
    {
	 	var c = object.value.charAt(i);
      
	  	if (c == '"')
		{
			j += 6;
		}
		else
			j++;
    }
   	if (j > limit_len)
	{
    alert('usuário / senha inválido.');   	  								    	    	   		
		return true;
	}	
	return false;
}

function oiPhoneCheck(value)
{
	var prefixLen;

	if(0 == value.length || value.match("[^0-9]") != null)
	{
				alert("Número de telefone OI Fixo tem de conter somente dígitos.");
        return false;
    }

	if ( '0' == value.charAt(0) )
	{
		alert("Insira o número do seu Oi Fixo com 2 dígitos de DDD e com 10 dígitos no total (não utilizar zero na frente do número e hífen)."); 
		return false;
	}

	return true;
}


