ROOTDIR=.

CC	= gcc
CFLAGS	= -Wall 
CFLAGS  += -g -m32 -pthread
INCS    = -I$(ROOTDIR)/include
LIBS    = -L$(ROOTDIR)/lib -lhardware

philo: philo.o ctx.o
	$(CC) $(CFLAGS) -o philo philo.o ctx.o $(LIBS)
philo.o: philo.c
	$(CC) $(CFLAGS) $(INCS) -c -o philo.o philo.c
ctx.o: ctx.c
	$(CC) $(CFLAGS) $(INCS) -c -o ctx.o ctx.c

clean:
	$(RM) *.o philo
