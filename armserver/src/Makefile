#ARM = 1
#GTK_THREAD = 1
ifdef ARM
CROSS = iwmmxt_le-
ARMGTK = /usr/local/arm/iwmmxt_le/target
else
CROSS =
ARMGTK = 
endif

ifdef GTK_THREAD
THREAD_LINK = -lgthread-2.0 -lglib-2.0 -lgmodule-2.0
else
THREAD_LINK = -lpthread
endif

CC=$(CROSS)gcc
IINCLUDE =  -I$(ARMGTK)/usr/include
srcdir = .
topdir = ..
CFLAGS = $(IINCLUDE) -I$(srcdir) -I$(topdir)
LINK =  $(THREAD_LINK) -lcrypt
object = armserver
sources = main.c error.c acmthread.c accthread.c acfthread.c acsthread.c \
       actthread.c proc.c anet.c acsprocess.c acssinfo.c clogin.c \
       support.c arm.c fconfig.c acfview.c thread.c log.c init.c
headers = acthread.h afthread.h afview.h amthread.h anet.h arm.h asprocess.h \
	  assinfo.h asthread.h atthread.h beepdrv.h fconfig.h debug.h error.h \
	  leddrv.h linuxarms.h log.h login.h proc.h protocol.h support.h \
	  thread.h init.h $(topdir)/config.h
OBJS = $(sources:.c=.o)
.SUFFIXES: .c .h
%.o:%.c $(headers)
	$(CC) -Wall -O2 $(CFLAGS) -c $< -o $@
$(object):$(OBJS) $(headers)
	$(CC) -Wall -O2 $(OBJS) -o $(object) $(LINK)
.PHONY : clean
clean:
	-rm -f $(OBJS) $(object)

