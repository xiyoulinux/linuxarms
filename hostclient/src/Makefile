CC=gcc
CFLAGS= -Wall -O2 `pkg-config --libs --cflags gtk+-2.0`
LINK=
object=hostclient
OBJ= main.o  \
cabout.o     \
cfiletrans.o \
cfileview.o  \
chelp.o      \
clogin.o     \
cmenubar.o   \
cmwindow.o   \
config.o     \
csctrl.o     \
csprocess.o  \
cssinfo.o    \
cstatusbar.o \
ctoolbar.o   \
error.o      \
hccthread.o  \
hcfthread.o  \
hcmthread.o  \
hcsthread.o  \
hctthread.o  \
hnet.o       \
iabout.o     \
ifiletrans.o \
ifileview.o  \
ihelp.o      \
ilogin.o     \
imenubar.o   \
imwindow.o   \
isctrl.o     \
isprocess.o  \
issinfo.o    \
istatusbar.o \
itoolbar.o   \
support.o    \
message.o

HEADER= about.h \
config.h        \
debug.h         \
error.h         \
filetrans.h     \
fileview.h      \
hcthread.h      \
help.h          \
hfthread.h      \
hmthread.h      \
hnet.h          \
hsthread.h      \
htthread.h      \
linuxarms.h     \
login.h         \
menubar.h       \
mwindow.h       \
protocol.h      \
sctrl.h         \
sprocess.h      \
ssinfo.h        \
statusbar.h     \
support.h       \
toolbar.h       \
message.h

$(object):$(OBJ)
	$(CC) $(CFLAGS) $(OBJ)  -o $(object) $(LINK)
ihelp.o :  ihelp.c    $(HEADER)
	$(CC) $(CFLAGS) -c $<   -o $@  
chelp.o :  chelp.c    $(HEADER)
	$(CC) $(CFLAGS) -c $<   -o $@  
ifileview.o :  ifileview.c    $(HEADER)
	$(CC) $(CFLAGS) -c $<   -o $@  
cfileview.o :  cfileview.c    $(HEADER)
	$(CC) $(CFLAGS) -c $<   -o $@  
ifiletrans.o :  ifiletrans.c    $(HEADER)
	$(CC) $(CFLAGS) -c $<   -o $@  
cfiletrans.o :  cfiletrans.c    $(HEADER)
	$(CC) $(CFLAGS) -c $<   -o $@  
iabout.o :  iabout.c    $(HEADER)
	$(CC) $(CFLAGS) -c $<   -o $@  
cabout.o :  cabout.c    $(HEADER)
	$(CC) $(CFLAGS) -c $<   -o $@  
ilogin.o :  ilogin.c    $(HEADER)
	$(CC) $(CFLAGS) -c $<   -o $@  
clogin.o :  clogin.c    $(HEADER)
	$(CC) $(CFLAGS) -c $<   -o $@  
imenubar.o :  imenubar.c    $(HEADER)
	$(CC) $(CFLAGS) -c $<   -o $@  
imwindow.o :  imwindow.c    $(HEADER)
	$(CC) $(CFLAGS) -c $<   -o $@  
cmwindow.o :  cmwindow.c    $(HEADER)
	$(CC) $(CFLAGS) -c $<   -o $@  
config.o :  config.c    $(HEADER)
	$(CC) $(CFLAGS) -c $<   -o $@  
isctrl.o :  isctrl.c    $(HEADER)
	$(CC) $(CFLAGS) -c $<   -o $@  
csctrl.o :  csctrl.c    $(HEADER)
	$(CC) $(CFLAGS) -c $<   -o $@  
isprocess.o :  isprocess.c    $(HEADER)
	$(CC) $(CFLAGS) -c $<   -o $@  
csprocess.o :  csprocess.c    $(HEADER)
	$(CC) $(CFLAGS) -c $<   -o $@  
issinfo.o :  issinfo.c    $(HEADER)
	$(CC) $(CFLAGS) -c $<   -o $@  
cssinfo.o :  cssinfo.c    $(HEADER)
	$(CC) $(CFLAGS) -c $<   -o $@  
istatusbar.o :  istatusbar.c    $(HEADER)
	$(CC) $(CFLAGS) -c $<   -o $@  
cstatusbar.o :  cstatusbar.c    $(HEADER)
	$(CC) $(CFLAGS) -c $<   -o $@  
itoolbar.o :  itoolbar.c    $(HEADER)
	$(CC) $(CFLAGS) -c $<   -o $@  
ctoolbar.o :  ctoolbar.c    $(HEADER)
	$(CC) $(CFLAGS) -c $<   -o $@  
error.o :  error.c    $(HEADER)
	$(CC) $(CFLAGS) -c $<   -o $@  
hccthread.o :  hccthread.c    $(HEADER)
	$(CC) $(CFLAGS) -c $<   -o $@  
hcfthread.o :  hcfthread.c    $(HEADER)
	$(CC) $(CFLAGS) -c $<   -o $@  
hmthread.o :  hmthread.c    $(HEADER)
	$(CC) $(CFLAGS) -c $<   -o $@  
hcsthread.o :  hcsthread.c    $(HEADER)
	$(CC) $(CFLAGS) -c $<   -o $@  
hctthread.o :  hctthread.c    $(HEADER)
	$(CC) $(CFLAGS) -c $<   -o $@  
hnet.o :  hnet.c    $(HEADER)
	$(CC) $(CFLAGS) -c $<   -o $@  
main.o :  main.c    $(HEADER)
	$(CC) $(CFLAGS) -c $<   -o $@  
support.o :  support.c    $(HEADER)
	$(CC) $(CFLAGS) -c $<   -o $@  
message.o :  message.c    $(HEADER)
	$(CC) $(CFLAGS) -c $<   -o $@  

clean:
	rm -f *.o
	@if [ -e $(object) ]; \
	then \
		echo "rm $(object)" ;\
		rm $(object) ; \
	fi
