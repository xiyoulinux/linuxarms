#ARM = 1
ifdef ARM
CROSS = iwmmxt_le-
ARMGTK = /usr/local/arm/iwmmxt_le/target
else
CROSS =
ARMGTK = 
endif

GTK_THREAD = 1
ifdef GTK_THREAD
THREAD_LINK = -lgthread-2.0
else
THREAD_LINK = -lpthread
endif

CC=$(CROSS)gcc
GTKFLAGSS = -I$(ARMGTK)/usr/include/gtk-2.0 \
	    -I$(ARMGTK)/usr/lib/gtk-2.0/include
GLIBFLAGS = -I$(ARMGTK)/usr/include/glib-2.0 \
	    -I$(ARMGTK)/usr/lib/glib-2.0/include
IINCLUDE =  -I$(ARMGTK)/usr/include
CFLAGS = -Wall -O2 $(GTKFLAGS) $(GLIBFLAGS) $(IINCLUDE)
LINK =  $(THREAD_LINK) -lglib-2.0 -lgmodule-2.0
object = armserver

OBJS = main.o error.o acmthread.o accthread.o acfthread.o acsthread.o \
       actthread.o proc.o anet.o acsprocess.o acssinfo.o clogin.o \
       support.o arm.o config.o acfview.o thread.o
#	log.o       

HEADER = linuxarms.h protocol.h arm.h error.h debug.h amthread.h \
	 acthread.h afthread.h asthread.h atthread.h proc.h anet.h \
	 asprocess.h assinfo.h login.h support.h beepdrv.h leddrv.h \
	 config.h afview.h thread.h
#       log.h
.SUFFIXES: .c .h
$(object):$(OBJS) $(HEADER)
	$(CC) -Wall -O2 $(OBJS) -o $(object) $(LINK)
%.o : %.c $(HEADER)
	$(CC) $(CFLAGS) -c $< -o $@  
.PHONY : clean
clean:
	-rm -f $(OBJS) $(object)

