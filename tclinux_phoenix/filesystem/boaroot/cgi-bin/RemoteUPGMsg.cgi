#!/bin/sh

echo "Content-Type: text/html"
echo ""

FILE="/tmp/cwmpfirmwareupgrade"
echo "<html>
<head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=gb2312\" /></head>
<body onload=\"setInterval('msgJudge()', 3000)\">
<div id=\"div0\">"

#�ļ�����ʱ��������������
if [ -f "$FILE" ];
then
    echo "<center><b><font size=5>�ն����ڽ��а汾�����������µ硣</font></b></center><br /><br />
&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp<font color=red>��ʾ</font>:<br />
&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbspITMSƽ̨���ڶ��豸����Զ����������ϵͳ�������Զ�����ҵ��(10������)����ע�����¼���:<br />
&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp1�������������У���Ҫ�µ硣<br />
&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp2�������������У���Ҫ�β���ˡ�<br />
&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp3�������������в�Ҫ�رո�ҳ�档<br />"

else
	echo "<center><b><font size=5>�ն������ɹ���������ʹ��ҵ��лл�������ĵȴ���</font></b></center>"      
fi

echo "<br />
</div>
<div id=\"div1\"></div>
<script type=\"text/javascript\" language=javascript>

//ȫ�ֱ���������״̬����ʶ��������
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

//��ʾ���ص�����״̬�������������ء������������Ѿ�������
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
	  	  msg=\"<center><b><font size=5>�ն�������������ȴ���</font></b></center>\";
      }
  }

  if (eraseFlashBeginFlag == 1){
      countAfterErase+=1;
      if ((onloadCount > (onloadCountWhenEraseFlash+connectWanCount+biasOfBrowser)) && (countAfterErase > (eraseFlashCount+rebootCount))){
  		  msg=\"<center><b><font size=5>�ն������ɹ���������ʹ��ҵ��лл�������ĵȴ���</font></b></center>\";
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
