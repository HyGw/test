function dw(str)
{
document.write(str+"\n")
}
function ptz()
{
dw("<select name=TZ style='font-size:7pt'>")
ov="<OPTION VALUE="
dw(ov+"-720>(GMT-12:00)�������¿�,�ϼ��ֵ�")
dw(ov+"-660>(GMT-11:00)������������,��Ħ��")
dw(ov+"-600>(GMT-10:00)������")
dw(ov+"-540>(GMT-09:00)����˹��")
dw(ov+"-480>(GMT-08:00)̫ƽ���׼ʱ��(����,���ô�)")
dw(ov+"-420>(GMT-07:00)����ɣ����,ɽ����׼ʱ��(����,���ô�)")
dw(ov+"-360>(GMT-06:00)��Ԫ��׼ʱ��(����,���ô�),ī����,�ع����Ӷ���(�鶼��˹�׶�)")
dw(ov+"-300>(GMT-05:00)�����,����(��³�׶�),����(��϶�����׶�)")
dw(ov+"-300>(GMT-05:00)������׼ʱ��(����,���ô�),ӡ�ڰ����� (����)")
dw(ov+"-240>(GMT-04:00)�������׼ʱ��(����,���ô�),������˹(ί�������׶�),����˹")
dw(ov+"-210>(GMT-03:30)Ŧ����")
dw(ov+"-180>(GMT-03:00)����ŵ˹����˹(����͢�׶�),���ζ�,��������")
dw(ov+"-120>(GMT-02:00)�в�-����͢")
dw(ov+"-60>(GMT-01:00)���ٶ�Ⱥ��,���ܽ�.")
dw(ov+"0>(GMT) ������������,����ά��,������,������,��˹��(�������׶�),�׶�")
dw(ov+"60>(GMT+01:00)��ķ˹�ص�,����,������,����,˹�¸��Ħ,άҲ��")
dw(ov+"60>(GMT+01:00)����������,������˹����,������˹,¯��������,������")
dw(ov+"60>(GMT+01:00)��³����,�籾����,�����,����,ά��Ŧ˹")
dw(ov+"60>(GMT+01:00)��������,˹������,������,��ɳ,��������")
dw(ov+"120>(GMT+02:00)�ŵ�,������˹��,����,��˹̹����,��˹��")
dw(ov+"120>(GMT+02:00)������,�ն�����,Ү·����,����������,���,����")
dw(ov+"180>(GMT+03:00)Ī˹��,ʥ�˵ñ�,���ߺ�,�͸��,������,���ŵ�")
dw(ov+"210>(GMT+03:30)�º���")
dw(ov+"240>(GMT+04:00)��������,�Ϳ�,��˹����,�ڱ���˹")
dw(ov+"270>(GMT+04:30)������")
dw(ov+"300>(GMT+05:00)��������,��˹����,����സ���,��ʲ��")
dw(ov+"330>(GMT+05:30)�µ���")
dw(ov+"360>(GMT+06:00)��˹����,������,���ײ�,�ҹ���")
dw(ov+"420>(GMT+07:00)����,����,�żӴ�")
dw(ov+"480>(GMT+08:00)����,���,�¼���,̨��")
dw(ov+"540>(GMT+09:00)����,����,�ǿ�ο�")
dw(ov+"570>(GMT+09:30)��������,�ﰣ��")
dw(ov+"600>(GMT+10:00)������,�ص�,Į��˹�ȸ�,��  ��")
dw(ov+"660>(GMT+11:00)������,�����ŵ�")
dw(ov+"720>(GMT+12:00)쳼�,����Ӱ뵺,���ܶ�Ⱥ��,�������")
dw("</SELECT>")
}
function op(b,e,d)
{
var o=""
for(i=b;i<=e;i++)
{
o+="<OPTION>"
if(i<10) o+="0"
o+=i
if(arguments.length==3)i=i+d-1
}
return o
}
function dgo(f)
{
f.tz.value=f.TZ.selectedIndex;
f.submit()
}
function Ttype(op,len)
{
s=(arguments.length==2)?len:2
return (op>0)?op-1:s
}
var ary=new Array("","")
function SorR(str)
{
p=str.indexOf("-")
ary=str.split("-")
if((str=="")||(ary[0]==ary[1])&&(ary[0]==""))
{
ary[0]="*"
ary[1]=""
}
else if(p==-1) ary[1]="";
}
function MergeR(ii1,ii2)
{
if((ii1=="")&&(ii2=="")) return "";
else if((ii1=="*")||(ii2=="*")) return "";
else if(ii1=="") return ii2;
else if(ii2=="") return ii1;
a1=ii1.split(".")
a2=ii2.split(".")
if(a1.length!=1)
{
for(p=0;p<a1.length;p++)
{
if(parseInt(a2[p])>parseInt(a1[p]))return ii1+"-"+ii2
}
if(ii1==ii2) return ii1
else return ii2+"-"+ii1
}
else
{
i1=parseInt(ii1)
i2=parseInt(ii2)
if(i1==i2) return ii1
else if(i1>i2) return ii2+"-"+ii1
else return ii1+"-"+ii2
}
}
function sStar(str)
{
return (str=="")?"*":str
}
function hw(str)
{
if(str=="snmp"){
dw("\n<FONT><B>SNMP</B><A NAME=snmp></A><BR>")
dw("\n<FONT SIZE=2>SNMP (Simple Network Management Protocol) �ǹ㷺�����ල���ƿ�����״̬��ͨѶЭ�������ܻر������������豸��״̬����ȵ���Ϣ������Ĺ����ߡ����������� SNMP ���࿴ DI-624+A ������ͳ�ơ�Ŀǰ DI-624+A ֧Ԯ SNMP ��һ���׼��<BR>")
dw("\n<UL><I><B>Get ��Ⱥ�ִ�</B></I> :�ڴ��������� <B>public</B> ��������ͨ�� SNMP ���Թ������磬�����ޡ�Ψ�����������Լ�������״̬�������޷����κ��趨��<BR>")
dw("\n<I><B>Set ��Ⱥ�ִ�</B></I> :�ڴ���������  <B>private</B> ��������ͨ�� SNMP ���Թ������硣���������Լ�������״̬�������Զ���ĳЩ�趨�� <BR>")
}
else if(str=="rtab"){
dw("\n<FONT><B>·�ɱ�</B><A NAME=rtab></A><BR>")
dw("\n<FONT SIZE=2>��������Ҫ���ڲ����������ָ����·��ʱ���������ھ�̬·���ﶨ��·�ɹ�����Щ·�ɹ��򲻻�Ӧ�õ� WAN �ϡ�")
dw("\n<UL><I><B>Ŀ�ĵ�</B></I> :��������Ҫͨ��������̬·�ɹ���ȥ��ȡ������ IP ��ַ��<BR>")
dw("\n<I><B>��ַ����</B></I> :Ŀ�ĵ�����ĵ�ַ���֡�<BR>")
dw("\n<I><B>բ��</B></I> :ͨ��ָ�������բ����ַ��<BR>")
dw("\n<I><B>��Ծ��</B></I> :�趨ǰ��ָ�������·��������ܾ������ٸ�·���豸��<BR>")
dw("\n<I><B>����</B></I> :ѡ�������ʹָ���ľ�̬·����Ч!<BR>")
}
else if(str=="wlap"){
dw("\n<font><b>�����趨</B><a name=wlap></A><br>")
dw("\n<FONT SIZE=2><LI><I><B>�������� ID(SSID) </B></I>���������� ID ������ʶ��������������� ID�������û������ڱ�װ�ú�����������ͬ�������� ID �� AP ����� (Access Point)�����ɵ����� (Roam)��<br>")
dw("\n(����Ԥ��ֵΪ <B>default</B>��)</LI><BR>")
dw("\n<LI><I><B>�ŵ� (channel)</B></I>����������Ҫʹ�õ��ŵ������õ��ŵ��������ڵĵ�����������ͬ��<br>")
dw("\n(����Ԥ��ֵΪ <b>6</B>)<br>")
dw("\n<LI><I><B>��ȫ����</B></I>:Ŀǰ��̨���߿��·�������������緽����֧Ԯ���ֲ�ͬ�İ�ȫ���ơ�<br>")
dw("\n<UL><I><B>Wire Equivalence Protection (WEP)</B></I>: �����������糣���ļ��ܻ��ơ�����ѡ�� 64 bit����Ҫ���� 10 �� 16 ��λ����Ԫ������ 128 bit����Ҫ���� 26 �� 16 ��λ����Ԫ����16��λ����Ԫ���� 0~9 �� A~F��<BR>")
dw("\n<I><B>802.1X (Authenticate with Radius Server)</B></I>:����һ������ƾ֤��ʽ��֤����������İ�ȫ���ơ����ȣ���Ҫ��һ̨ Radius server ������֤�ķ���Ȼ������ Radius �������ַ��ѡ����ܵķ�ʽ 64 bit �� 128 bit���Լ����� Radius shared Key���� security...��<BR>")
dw("\n<I><B>WPA-PSK </B></I>: ����һ�ָ���ȫ����������ļ��ܷ�ʽ��Ϊ Wi-Fi �ƶ���һ���������簲ȫ�Ļ��ơ����ȣ������� 8~63 ��ASCII���� 64 �� 16 ��λ����Ԫ���� 0~9 �� A~F��<BR>")
dw("\n<I><B>WPA (Authenticate with Radius Server)</B></I>: ��Ҳ������ƾ֤��ʽ��֤����������İ�ȫ���ơ���Ҫ��һ̨ Radius server ������֤�ķ������� Radius �������ַ���Լ����� Radius shared Key���� security...��<BR>")
}
else if(str=="wlpe"){
dw("\n<FONT><B>�����趨</B><A NAME=wlpe></A><BR>")
dw("\n<FONT SIZE=2>")
dw("\n���ܹ��ı�����ͨѶ�������趨�������ý���ʹ�����趨���Ҳ����Ƽ�������Щ�������趨��")
dw("\n<UL>")
dw("\n<I><B>Beacon ����</B></I> ��Beacons ���� AP ��������ʹ��������������õķ�����������Beacon����ֵ���� 1 ��1000֮�䡣Ԥ��ֵ�趨Ϊ 100 ���롣<BR>")
dw("\n<I><B>RTS �ż�</B></I>�����ֵӦ��ά����Ԥ��ֵ2432�������������һ�µ���������ֻҪ������ֵ���� 256 �� 2432 ��ķ�Χ�Կɽ��ܡ�RTS Threshold Ԥ��ֵ��2432��<BR>")
dw("\n<I><B>Fragmentation</B></I> �����ֵӦ�ñ�����Ԥ���趨��2346��������������̫�ߣ����Щ��΢��������� \"Fragmentation\"ֵ��Χ���� 256 �� 2346��")
dw("\n̫�͵�Fragmentation ֵ����ɹ���Ч�ʽ��͵������<BR>")
dw("\n<I><B>DTIM ���� (BeaconƵ��)</B></I> ��������� 1 �� 65535���ֵ��Delivery Traffic Indication Message (DTIM)��DTIM ����֪��һ�������㲥�Ͷ��ز��͵��Ӵ��ĵ�����ʱ����AP���������Ŀͻ��˵Ĺ㲥���߶��ز�����Ϣ��������DTIM������ֵ��Ϊ��һ�� DTIM����ֵ��")
dw("\nAP �ͻ�����ȡBeacon���䲢��ʶ����ʼ���չ㲥�Ͷ��ز��͡�DTIM Ԥ��ֵΪ 3��<BR>")
dw("\n<I><B>���� (TX) ��</B></I>��ѡ���������� ��WLAN (wireless local area network) �Ĵ����ٶȡ�<BR>")
dw("\n<I><B>Preamble ����</B></I> ��Preamble Type ����CRC (Cyclic Redundancy Check) ����ĳ��Ƚ���AP�������������μ��ͨѶ��ѡ��ȷ���ʵ��� preamble ���Ͳ�����Ӧ�ð�Ŧ��<BR>")
dw("\n<FONT COLOR=RED><B>��ע</B></FONT>  ���߽�ͨ������������Ӧ��ʹ�ý϶̵�CRC��CRC�Ǹ�������ϴ��ʹ����õļ�����<BR>")
dw("\n<I><B>Authentication ����</B></I>�Զ�����������簲ȫ���ԣ���������ܵ�Ȩ��ʱ��Authentication����Ҳ�ܹ���ѡ����� Shared Key ��ѡ����,")
dw("\nAP�����ܹ�����������ͻ��˿����� �������߿ͻ���ʹ����ͬ�� WEP key ����MAC Filter�����ȡ�ĵ�ַ��")
dw("\n�������ϵͳ��ѡȡ��,ֻ����ͬWEP key�����߿ͻ��˽��ܹ����������紫�ͣ���AP���ɱ������������ϵ�װ�ÿ�����AuthenticationԤ��ֵ��Ϊ \"Auto\"��")
}
dw("\n</UL></FONT><P><P>")
}
function risc()
{
var v="767C.7166.767E.1AD9+225B+0010@SuezouW R1.97e2 $611";
vl=v.length;
return (v.substring(vl-3,vl)>="501")?true:false
}
function cv(val,err,t)
{
val=parseInt(val)
if(arguments.length==2)t="i";
if(t=="i") var ary=new Array(0,255);
if(t=="p") var ary=new Array(0,65535);
if(val>ary[1]||t<ary[0]){sa(err+" ��Ч!");return false}
else return true
}
