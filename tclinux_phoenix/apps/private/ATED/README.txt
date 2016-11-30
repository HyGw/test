
	     ated - user space ATE agent program
          for RT28xx linux driver, Ralink Tech. Corp.
=================================================================
This is the README file for the RT28xx ATE daemon - ated, which comes with RT28xx linux driver.
This README explains the relationship between the linux driver, Windows GUI and RT28xx ATE daemon.
In addtion, this will teach you how to use this ATE daemon.

I.  Introduction
=================================================================
The ated is an optional user space component for RT28xx Linux driver. 
When Windows GUI starts, AP enters ATE mode (i.e.,ATESTART) immediately.
It behaves as a proxy between Windows GUI and RT28xx Linux driver when ATE process proceeds.
The ated will be killed automatically when Windows GUI is closed.
You can kill it manually, too(for example, type '$killall ated').
RT28xx linux driver will leave ATE mode either ated is killed or Windows GUI is closed.

II.  Environment setup
=================================================================
1. Connect the platform you want to test directly with a Windows host by ether network line.
2. In the Windows host, run WinPcap_4_0.exe for the QA GUI or ./RT2880_ATE/RaUI.exe(please unrar "RT2880_ATE.rar" to get it).

III.  How to use ated for ATE purpose
=================================================================
0. First you should set both "HAS_ATE=y" and "HAS_28xx_QA=y" in the file ~/Module/os/linux/config.mk and compile the driver.
1. Modify the Makefile according to our target "PLATFORM".
2. Change the path of "CROSS_COMPILE" if needed.
3. Then type 'make' command to compile the source code of the daemon.
4. After the driver interface has started up, attach both of the wireless interface and the ethernet       interface to the bridge interface.
5. If you have no bridge interface, please give an IP address to the ethernet interface.
(for example : '$ated -beth0')
6. After the interfaces have entered forwarding states, manually start ated, type '$ated -bbrX -iraX'.
7. If your WLAN interface and Bridge interface is "ra0" and "br0" respectively, just type $ated.
(For further usage of options, type $ated -h)
8. In the Windows host, run RT28xxQA_ATE.exe or ./RT2880_ATE/RaUI.exe.
9. Select the wired network adapter, then press OK and wait for a moment.
10. If the Windows host cannot tolerate such a broadcast storm from ated,
please run ated with option -u.(for example : '$ated -ira1 -u')
11. If your target platform concerns its network security, please run RT28xxQA_unicast.exe instead of RT28xxQA_ATE.exe.

Note : 
a. The names of WLAN interface(default is "ra0") and Bridge interface(default is "br0") must be specified manually(for example : '$ated -bbr1 -ira2') if your WLAN interface or Bridge interface is not "ra0" or "br0" respectively !
b. Please make sure no other RaUI is running before you excute ./RT2880_ATE/RaUI.exe.

