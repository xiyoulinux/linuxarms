GTK_THREAD = 1
ifdef GTK_THREAD
THREAD_LINK = -lgthread-2.0
else
THREAD_LINK = -lpthread
endif
CC = gcc

GTKFLAGS = -I/usr/include/gtk-2.0 \
	   -I/usr/lib/gtk-2.0/include \
	   -I/usr/include/atk-1.0 \
	   -I/usr/include/cairo \
	   -I/usr/include/pango-1.0 \
	   -I/usr/include/glib-2.0 \
	   -I/usr/lib/glib-2.0/include \
	   -I/usr/include/pixman-1 \
	   -I/usr/include/freetype2 \
	   -I/usr/include/libpng12

CFLAGS = -Wall -O2  $(GTKFLAGS)

LINK = $(THREAD_LINK) -lgtk-x11-2.0 -lgdk-x11-2.0 -latk-1.0 \
       -lpangoft2-1.0 -lgdk_pixbuf-2.0 -lm -lpangocairo-1.0 \
       -lgio-2.0 -lcairo -lpango-1.0 -lfreetype -lz \
       -lfontconfig -lgobject-2.0 -lgmodule-2.0 -lglib-2.0

object = hostclient

OBJS = main.o cabout.o cfiletrans.o cfview.o chelp.o clogin.o cmenubar.o \
       cmwindow.o config.o csctrl.o csprocess.o cssinfo.o cstatusbar.o \
       ctoolbar.o error.o hccthread.o hcfthread.o hcmthread.o hcsthread.o \
       hctthread.o hnet.o iabout.o ifiletrans.o ifview.o ihelp.o ilogin.o \
       imenubar.o imwindow.o isctrl.o isprocess.o issinfo.o istatusbar.o \
       itoolbar.o support.o message.o thread.o

HEADER = about.h config.h debug.h error.h filetrans.h fview.h hcthread.h \
	 help.h hfthread.h hmthread.h hnet.h hsthread.h htthread.h login.h \
	 linuxarms.h menubar.h mwindow.h protocol.h sctrl.h sprocess.h \
	 ssinfo.h statusbar.h support.h toolbar.h message.h thread.h

.SUFFIXES: .c .h

$(object):$(OBJS) $(HEADER)
	$(CC) -Wall -O2 $(OBJS) -o $(object) $(LINK)

%.o :  %.c $(HEADER)
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY : clean
clean:
	-rm -f $(OBJS) $(boject)

