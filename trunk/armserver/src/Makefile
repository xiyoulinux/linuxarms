CC=gcc
CFLAGS= -Wall -O2
LINK=-lpthread
object=armserver

OBJ= main.o         \
	error.o     \
	acmthread.o \
	accthread.o \
	acfthread.o \
	acsthread.o \
	actthread.o \
	proc.o      \
	log.o       \
	anet.o      \
	acsprocess.o\
	acssinfo.o  \
	clogin.o    \
	support.o   \
	arm.o

HEADER=linuxarms.h \
       protocol.h  \
       arm.h       \
       error.h     \
       debug.h     \
       amthread.h  \
       acthread.h  \
       afthread.h  \
       asthread.h  \
       proc.h      \
       log.h       \
       anet.h      \
       asprocess.h \
       assinfo.h   \
       login.h     \
       support.h   \
       beepdrv.h   \
       leddrv.h

$(object):$(OBJ)
	$(CC) $(CFLAGS) $(OBJ)  -o $(object) $(LINK)
main.o:       main.c       $(HEADER)
	$(CC) $(CFLAGS) -c $<   -o $@ 
error.o :     error.c      $(HEADER)
	$(CC) $(CFLAGS) -c $<   -o $@  
acmthread.o : acmthread.c  $(HEADER) 
	$(CC) $(CFLAGS) -c $<   -o $@  
accthread.o : accthread.c  $(HEADER) 
	$(CC) $(CFLAGS) -c $<   -o $@ 
acfthread.o : acfthread.c  $(HEADER)
	$(CC) $(CFLAGS) -c $<   -o $@  
actthread.o : actthread.c  $(HEADER)
	$(CC) $(CFLAGS) -c $<   -o $@  
proc.o      : proc.c       $(HEADER)
	$(CC) $(CFLAGS) -c $<   -o $@  
log.o :       log.c        $(HEADER)
	$(CC) $(CFLAGS) -c $<   -o $@  
anet.o :      anet.c       $(HEADER)
	$(CC) $(CFLAGS) -c $<   -o $@  
acsprocess.o: acsprocess.c $(HEADER)
	$(CC) $(CFLAGS) -c $<   -o $@  
acssinfo.o :  acssinfo.c   $(HEADER)
	$(CC) $(CFLAGS) -c $<   -o $@  
clogin.o :   clogin.c    $(HEADER)
	$(CC) $(CFLAGS) -c $<   -o $@  
support.o :   support.c    $(HEADER)
	$(CC) $(CFLAGS) -c $<   -o $@  
arm.o :   arm.c    $(HEADER)
	$(CC) $(CFLAGS) -c $<   -o $@  

clean:
	rm -f *.o
	@if [ -e $(object) ] ; \
	then \
		echo "rm \"$(object)\" " ;\
		rm $(object) ;\
	fi

