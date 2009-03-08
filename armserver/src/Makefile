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
GLIBFLAGS =-I$(ARMGTK)/usr/include/glib-2.0 \
	    -I$(ARMGTK)/usr/lib/glib-2.0/include
IINCLUDE =  -I$(ARMGTK)/usr/include
srcdir = -I.
CFLAGS =$(GTKFLAGS) $(GLIBFLAGS) $(IINCLUDE) $(srcdir)
LINK =  $(THREAD_LINK) -lglib-2.0 -lgmodule-2.0 -lcrypt
object = armserver
sources = main.c error.c acmthread.c accthread.c acfthread.c acsthread.c \
       actthread.c proc.c anet.c acsprocess.c acssinfo.c clogin.c \
       support.c arm.c config.c acfview.c thread.c log.c init.c
headers = acthread.h afthread.h afview.h amthread.h anet.h arm.h asprocess.h \
	  assinfo.h asthread.h atthread.h beepdrv.h config.h debug.h error.h \
	  leddrv.h linuxarms.h log.h login.h proc.h protocol.h support.h \
	  thread.h init.h
OBJS = $(sources:.c=.o)
.SUFFIXES: .c .h
%.o:%.c $(headers)
	$(CC) -Wall -O2 $(CFLAGS) -c $< -o $@
#%.deps:%.c
#	$(CC) -M  $(CFLAGS) $< > $@.$$$$; sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; rm -f $@.$$$$
$(object):$(OBJS) $(headers)
	$(CC) -Wall -O2 $(OBJS) -o $(object) $(LINK)
#sinclude $(sources:.c=.deps)
.PHONY : clean
clean:
	-rm -f $(OBJS) $(object)

