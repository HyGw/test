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
		function doRedirect()
		{
			<%if tcWebApi_get("Info_Adsl","lineState","h") <> "up" then%>			
			<%if request_form("WAN_RetryFlag") = "1"  Then%>
				navPage( 'authentication_failure_velox' );
			<%elseif request_form("WAN_RetryFlag") = "2"  Then%>
				navPage( 'authentication_failure_velox_videoem' );			
			<%end if%>
			<%else%>
			<%if request_form("WAN_RetryFlag") = "1"  Then%>
				document.configure_adslTest.WAN_RetryFlag.value = "1";
				document.configure_adslTest.submit();
			<%elseif request_form("WAN_RetryFlag") = "2"  Then%>
				navPage( 'backhome' );	
			<%end if%>
			<%end if%>
		}		
		</script>
    </head>
<body onLoad="doRedirect()">
	<form name="configure_adslTest" method="post" action="/cgi-bin/WizardOi/configurando.asp">
	<input type="hidden" name="WAN_RetryFlag" value="0">
	</form>
</body>
</html>
