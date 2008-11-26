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
	aclogin.o

HEADER = linuxarms.h \
       protocol.h  \  
       arms.h      \
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
       alogin.h   

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
actthread.o : acthread.c  $(HEADER)
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
aclogin.o :   aclogin.c    $(HEADER)
	$(CC) $(CFLAGS) -c $<   -o $@  

clean:
	rm -f *.o

