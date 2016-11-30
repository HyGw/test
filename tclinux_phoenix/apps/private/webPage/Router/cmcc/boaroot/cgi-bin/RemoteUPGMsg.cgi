#!/bin/sh

echo "Content-Type: text/html"
echo ""

FILE="/tmp/cwmpfirmwareupgrade"
echo "<html>
<head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=gb2312\" /></head>
<body onload=\"setInterval('msgJudge()', 3000)\">
<div id=\"div0\">"

#文件存在时，表明正在下载
if [ -f "$FILE" ];
then
    echo "<center><b><font size=5>终端正在进行版本升级，请勿下电。</font></b></center><br /><br />
&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp<font color=red>提示</font>:<br />
&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbspITMS平台正在对设备进行远程升级，且系统启动后自动配置业务(10分钟内)，请注意以下几点:<br />
&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp1、在升级过程中，不要下电。<br />
&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp2、在升级过程中，不要拔插光纤。<br />
&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp3、在升级过程中不要关闭该页面。<br />"

else
	echo "<center><b><font size=5>终端升级成功，可正常使用业务，谢谢您的耐心等待。</font></b></center>"      
fi

echo "<br />
</div>
<div id=\"div1\"></div>
<script type=\"text/javascript\" language=javascript>

//全局变量，保存状态、标识、计数等
var bias = 1;
var biasOfBrowser = 5;
var cycleCount = 0;
var onloadCount = 0;
var countAfterErase = 0;
var eraseFlashBeginFlag = 0;
var onloadCountWhenEraseFlash = 0;
var eraseFlashCount = 20, connectWanCount = 3;
var rebootCount = 10;

function onloadIncrease(){
  onloadCount+=1;
}

//显示网关的三种状态，包括正在下载、正在重启、已经正常。
function msgJudge()
{ 
  var elem, img, msg, time, elem1;

  elem = document.getElementById(\"div0\");
  elem1 = document.getElementById(\"div1\");
  time = new Date().getTime();
  msg  = elem.innerHTML;  

  if (cycleCount > (onloadCount+bias)){
      if (onloadCountWhenEraseFlash == 0){
		  onloadCountWhenEraseFlash = onloadCount;
		  eraseFlashBeginFlag = 1;  
      }    
      if (cycleCount > (onloadCountWhenEraseFlash+eraseFlashCount)){
	  	  msg=\"<center><b><font size=5>终端正在重启，请等待。</font></b></center>\";
      }
  }

  if (eraseFlashBeginFlag == 1){
      countAfterErase+=1;
      if ((onloadCount > (onloadCountWhenEraseFlash+connectWanCount+biasOfBrowser)) && (countAfterErase > (eraseFlashCount+rebootCount))){
  		  msg=\"<center><b><font size=5>终端升级成功，可正常使用业务，谢谢您的耐心等待。</font></b></center>\";
  	  }
  }

  img=\"<img width='1' height='1' style='display:none' onload='onloadIncrease()'\" +
      \"src='/img/ok.gif?query0=\"
      + time + \"'></img>\";
  elem1.innerHTML=img;
  elem.innerHTML=msg;
  cycleCount+=1;
}
</script>
</body>
</html>"
