# linuxarms
Automatically exported from code.google.com/p/linuxarms
Real-Time Management System Based on Arm Linux

##项目来源及背景

针对目前的系统，开发人员通常需要许多软件来进行系统开发，例如需要ftp来下载或上传文件到arm目标板上，需要使用minicom来监控系统的运行情况，需要ssh来登陆到arm目标板系统上来进行操作等，如此以来，开发人员就需要安装这些软件，并且在使用的时候需要同时打开这些软件，在这些软件之间切换来使用相应的功能，并且这些软件都是在命令行上使用，这样以来，就增加了arm系统开发的难度。所以我们需要一个集成这些功能的并且有很好的图形界面的软件。针对这个问题，我们开发这个软件来解决这个问题。
项目要达到的目标
本项目设定的目标如下：
* 本系统能提供系统完善的功能，简化arm系统开发人员的开发过程。
* 针对系统，为开发人员提供很好的监控功能，能实时显示arm系统的进程运行情况，系统资源以及arm板的温度等的使用情况。
* 为开发人员系统提供对arm系统的控制功能，开发人员通过在host系统上发送命令控制arm系统，例如关机，停止某些进程等。
* 为开发人员提供从host系统到arm系统的上传和下载功能。
* 为开发人员提供浏览arm系统文件系统的功能。
* 系统host系统上提供友好的用户界面，方便系统开发人员操作。

##系统整体结构

本系统是针对EPC-8000 arm开发板开发的。EPC-8000 arm开发板预装有linux操作系统，内核版本为2.6.10，并且有网卡。所以本系统采用CS架构实现在开发主机(hostclient)上监控目标开发板(armserver)。其中armserver作为服务器，实时接收来自hostclient的命令，然后为hostclient发送相应的数据或进行系统控制。Hostclient作为客户机，开发人员通过图形界面的操作，hostclient会发送相应的操作命令到armserver上，并接收armserver反馈来的数据，为开发人员系统服务。最终加以改造，系统可以实现在两个arm系统上运行，使用一个arm系统监控另一个arm系统的功能。

##友情链接

Google协作平台：http://sites.google.com/site/linuxarms/

西邮Linux兴趣小组主页：http://www.xiyoulinux.cn/

西邮Linux兴趣小组论坛：http://groups.google.com/group/xiyoulinux

Linux内核之旅：http://www.kerneltravel.net/

Linux内核学习小组博客圈：http://blog.chinaunix.net/group/group_1488.html
