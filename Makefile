ROOTDIR=.

CC	= gcc
CFLAGS	= -Wall 
CFLAGS  += -g -m32 -pthread
INCS    = -I$(ROOTDIR)/include
LIBS    = -L$(ROOTDIR)/lib -lhardware

ping_pong: ping_pong.o ctx.o
	$(CC) $(CFLAGS) -o ping_pong ping_pong.o ctx.o $(LIBS)
ping_pong.o: ping_pong.c
	$(CC) $(CFLAGS) $(INCS) -c -o ping_pong.o ping_pong.c
ctx.o: ctx.c
	$(CC) $(CFLAGS) $(INCS) -c -o ctx.o ctx.c

clean:
	$(RM) *.o ping_pong
