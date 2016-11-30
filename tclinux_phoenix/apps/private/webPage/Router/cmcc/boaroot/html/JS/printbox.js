// JavaScript Document
function webClickAdd(pszDomainName,szUrl)
{  
     top.preAddDomain = pszDomainName;
     location.replace('/' + szUrl);
}

function webClickEdit(szDomainName,uwInstanceID,szUrl)
{
     var hdFrm = new webSubmitForm();
     var strDomainName = szDomainName+'.'+uwInstanceID
     hdFrm.setAction("../"+szUrl+'?DomainNamePrefix='+strDomainName);
     hdFrm.submit();	
}

function doDel(szArrayName)
{
	var form = document.StaticRouteForm;
	var aArrayInfo = eval(szArrayName);
    	var strDomainName = aArrayInfo[1][2];
     //从第3行开始确认复选框有没有被选中
    for(k = 3;k < (aArrayInfo.length - 1);k++)
    { 
        var elementId = aArrayInfo[0][1] + "_" + k;
     	var checkbox = getElById(elementId);	

	var temp = "hSrouteFlag" + aArrayInfo[k][2];
     	if(true == checkbox.checked)
     	{
		setText(temp, aArrayInfo[k][2]);
		
     	}
	else
	{
		setText(temp,"17");
	}
     }
}

function webClickRemove()
{
	var form = document.StaticRouteForm;
	form.delFlag.value = "1";
	form.submit();
	/*var aArrayInfo = eval(szArrayName);
    var strDomainName = aArrayInfo[1][2];
    //var hdFrm = new webSubmitForm();
   // hdFrm.setAction('del.cgi?RequestFile='+aArrayInfo[0][0]);
     //从第3行开始确认复选框有没有被选中
    for(k = 3;k < (aArrayInfo.length - 1);k++)
    { //alert(k)
        var elementId = aArrayInfo[0][1] + "_" + k;
     	//var instenceID;
     	var checkbox = getElById(elementId);	
		//alert(szArrayName)
		//alert(checkbox.checked)
     	if(true == checkbox.checked)
     	{
     	    
     		//hdFrm.addParameter(strDomainName+'.'+aArrayInfo[k][2],"");
			//window.location = '/cgi-bin/net-routeset.asp?del_num='+aArrayInfo[k][2];
			form.del_num.value = aArrayInfo[k][2];
			return;
     	}
     }
	
 	// if(!confirm(shErr(1006, ERR_ONLYVALUE)))
 	// {
 	//	 return;
 	// }     
    //hdFrm.submit();*/
}

function printInstEditBox(arrMsgInfoName,TabWidth)
{	
	var i;	
	var j;
	var strTemp;
	var strSub;
	var strName;
	var backColor = ' class=\"table_title\"';	
	var strTDStart;
	var width;
	var arrMsgInfo = eval(arrMsgInfoName);
    var strDomainName = arrMsgInfo[1][2];
    var strUrl = arrMsgInfo[1][0];
    var transformfunction;
    var edit_url = arrMsgInfo[1][1];
    var PrintBuf = "";
	
	if (TabWidth == null)
	{
	   TabWidth = 450;
    }
    
    PrintBuf += '<TABLE border=\'1\' cellspacing=\'0\' cellpadding=\'0\' width=' + TabWidth + '>';
    
	var tempArray = arrMsgInfo[0];

	//输出提示行
	PrintBuf+=("<TR>");
	for(j=3;j<tempArray.length-2;j++)
    {    
     //   backColor = 'style=\"background: #c2c2bf\"';
	    width = ' width=\"' + arrMsgInfo[0][j] + '\" ';
		strTDStart = '<TD align=\"center\"'  + width + backColor + '>';
	    
	    strTemp = strTDStart + arrMsgInfo[1][j];
	    strTemp += '</TD>';
	    PrintBuf+=(strTemp);
    }

	PrintBuf+=("<TD align=\"center\" width=\"" + arrMsgInfo[0][tempArray.length-2] + " \"" +backColor +">");
	PrintBuf+=("删除");
	PrintBuf+=("</TD>");	
	PrintBuf+=("<TD align=\"center\" width =\"" + arrMsgInfo[0][tempArray.length-1] + " \"" +backColor +">");
	PrintBuf+=("编辑");
	PrintBuf+=("</TD>");	
	
	PrintBuf+=("</TR>");

	//从第3行开始输出
	for( i = 3; i < (arrMsgInfo.length - 1); i++ )
	{		
		PrintBuf+=("<TR>");

		for(j=3;j<tempArray.length-2;j++)
		{
		    width = ' width =\"' + arrMsgInfo[0][j] + '\"';
		    strTDStart = '<TD align=\"center\"' + width + '>';
			if (arrMsgInfo[2][j] != "")
    	    {
        	    transformfunction = eval(arrMsgInfo[2][j]);
        	    strTemp = strTDStart + transformfunction(arrMsgInfo[i][j],arrMsgInfo,strDomainName+arrMsgInfo[i][2]);
    	    }
    		else
    		{
				if (arrMsgInfo[i][j] != '')
    		    {
					strTemp = strTDStart + arrMsgInfo[i][j];
				}
			    else
			    {
			        strTemp = strTDStart +'&nbsp;';
			    }
    		}
		    
		    strTemp += '</TD>';
		    PrintBuf += (strTemp);
	    }
   
	    width = ' align="center" width = \"' + arrMsgInfo[0][tempArray.length-2] + '\"';
		strTDStart = '<TD align=\"center\"' + width +'>';
        strTemp = strTDStart + '<INPUT type=\"checkbox\" id=\"' + 'checkId_' + i + '\"></TD>';	
		PrintBuf += (strTemp);

        width = ' align="center" width = \"' + arrMsgInfo[0][tempArray.length-1] + '\"';
        strTDStart = '<TD align=\"center\"' + width +'>';
        strTemp = strTDStart + '<INPUT type=\"button\"  class="btns"  value = \"编辑\" id=\"'+ 'editbuttonId' + i + '\" onclick = \"webClickEdit('+'\''+strDomainName+'\''+','+'\''+arrMsgInfo[i][2]+'\''+','+'\''+edit_url+'\''+');\"></TD>';	
		PrintBuf += (strTemp);
		PrintBuf+=("</TR>");
	}
	
	PrintBuf += ('</TABLE>');
	//PrintBuf+=('</FORM>');
    PrintBuf =  PrintBuf + '<BR><INPUT type=\"button\"  class="btns"  value = \"添加\" onclick = \"webClickAdd('+'\''+strDomainName+'\''+','+'\''+strUrl+'\''+');\" id=\"button\" name=\"button\">';
	PrintBuf =  PrintBuf + '&nbsp;<INPUT type=\"button\" class="btns"  value = \"删除\" onclick = \"webClickRemove();\" id=\"button\" name=\"button\">';

	document.write(PrintBuf);
	return;
}

/*start of HGW 2008.1.10 For A36D04370 FireFox 显示 V100R001C02B013 by w00104696*/
function getDisplayText(val,lineLength)
{
   if (lineLength == null)
      lineLength = 20;
   var content = '';
   var index = 0;
   var len = val.length;
   
   while (len > lineLength)
   {
	  content += val.substr(index,lineLength) + '<br>';   
	  len -= lineLength;
	  index += lineLength;
   }
   content += val.substr(index);    
   
   return content; 
}
/*end of HGW 2008.1.10 For A36D04370 FireFox 显示 V100R001C02B013 by w00104696*/

function printInstNoEditBox(arrMsgInfoName, TabWidth)
{	
	var i;	
	var j;
	var strTemp;
	var strSub;
	var strName;
	var backColor = ' class=\"table_title\"';
	var strTDStart;
	var width;
	var arrMsgInfo = eval(arrMsgInfoName);
    var strDomainName = arrMsgInfo[1][2];
    var strUrl = arrMsgInfo[1][0];//alert(strUrl)
    var transformfunction;
    var PrintBuf = "";
    var strVal;
    
    if (TabWidth == null)
	{
	   TabWidth = 450;
    }
//style=\"word-wrap:break-word\"
	PrintBuf+=('<TABLE border=\'1\' cellspacing=\'0\' cellpadding=\'0\' width=\"' + TabWidth + '\">');
	
	var tempArray = arrMsgInfo[0];	

	//输出提示行
	PrintBuf+=("<TR>");
	for(j=3;j<tempArray.length-2;j++)
    {    
	    width = ' width=\"' + arrMsgInfo[0][j] + '\" ';
		strTDStart = '<TD align=\"center\"'  + width + backColor + '>';
	    
	    strTemp = strTDStart + arrMsgInfo[1][j];
	    strTemp += '</TD>';
	    PrintBuf+=(strTemp);
    }
	PrintBuf+=("<TD align=\"center\" width=\"" + arrMsgInfo[0][tempArray.length-2] + " \""
			   + backColor +">");
	PrintBuf+=("删除");
	PrintBuf+=("</TD>");	
	PrintBuf+=("</TR>");

	//从第3行开始输出	
	for( i = 3; i < (arrMsgInfo.length - 1); i++ )
	{	
		PrintBuf += ("<TR>");

		for(j=3;j<tempArray.length-2;j++)
		{
			width = ' align="center" width =\"' + arrMsgInfo[0][j] + '\"';
		    strTDStart = '<TD align=\"center\"' +  width + '>';
						
			if (arrMsgInfo[2][j] != "")
    	    {
        	    transformfunction = eval(arrMsgInfo[2][j]);
				strVal = transformfunction(arrMsgInfo[i][j],arrMsgInfo,strDomainName+arrMsgInfo[i][2]);
			    strTemp = strTDStart + getDisplayText(strVal, 12);
    	    }
    		else
    		{
				if (arrMsgInfo[i][j] != '')
    		    {
					strTemp = strTDStart + getDisplayText(arrMsgInfo[i][j], 12);
				}
			    else
			    {
			        strTemp = strTDStart +'&nbsp;';
			    }
    		}
		    
		    strTemp += '</TD>';
		    PrintBuf+=(strTemp);
	    }

        width = ' align="center" width = \"' + arrMsgInfo[0][tempArray.length-2] + '\"';    
        strTDStart = '<TD align=\"center\"' +  width + '>';
        strTemp = strTDStart + '<INPUT type=\"checkbox\" id=\"' + arrMsgInfoName + '_' + i + '\" onclick=\"doDel('+arrMsgInfoName+');\"></TD>';	
		PrintBuf += (strTemp);
       
		PrintBuf+=("</TR>");
	}

	PrintBuf += ('</TABLE>');
    PrintBuf =  PrintBuf + '<BR><table><tr><td><INPUT type=\"button\"  class="btns"  value = \"添加\" onclick = \"webClickAdd('+'\''+strDomainName+'\''+','+'\''+strUrl+'\''+');\">';
	PrintBuf =  PrintBuf + '&nbsp;<INPUT type=\"button\" class="btns"  value = \"删除\" onclick = \"webClickRemove();\"></td></tr></table>';

	document.write(PrintBuf);
	return;
}
