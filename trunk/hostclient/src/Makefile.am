## Process this file with automake to produce Makefile.in

INCLUDES = \
	-DPACKAGE_DATA_DIR=\""$(datadir)"\" \
	-DPACKAGE_LOCALE_DIR=\""$(prefix)/$(DATADIRNAME)/locale"\" \
	@PACKAGE_CFLAGS@

bin_PROGRAMS = hostclient

#hostclient_SOURCES = \
#	main.c \
#	support.c support.h \
#	interface.c interface.h \
#	callbacks.c callbacks.h
hostclient_SOURCES = \
	main.c \
	linuxarms.h \
	hnet.c hnet.h \
	debug.h \
	protocol.h \
	config.h config.c \
	error.h error.c \
	mwindow.h    imwindow.c   cmwindow.c   \
	menubar.h    imenubar.c   cmenubar.c   \
	toolbar.h    itoolbar.c   ctoolbar.c   \
	ssinfo.h     issinfo.c    cssinfo.c    \
	sprocess.h   isprocess.c  csprocess.c  \
	fileview.h   ifileview.c  cfileview.c  \
	filetrans.h  ifiletrans.c cfiletrans.c \
	sctrl.h      isctrl.c     csctrl.c     \
	statusbar.h  istatusbar.c cstatusbar.c \
	support.h		  support.c    \
	help.h       ihelp.c      chelp.c      \
	about.h      iabout.c     cabout.c     \
	login.h      ilogin.c     clogin.c     \
	hmthread.h   		  hcmthread.c  \
	hsthread.h   		  hcsthread.c  \
	hfthread.h   		  hcfthread.c  \
	hcthread.h		  hccthread.c  \
	htthread.h		  hctthread.c


hostclient_LDADD = @PACKAGE_LIBS@ $(INTLLIBS)

