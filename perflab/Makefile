# Student's Makefile for the CS:APP Performance Lab

CC = gcc
CFLAGS = -Wall -O2 -m32
LIBS = -lm
TURNIN = submission.tar

OBJS = driver.o kernels.o fcyc.o clock.o

all: driver

driver: $(OBJS) fcyc.h clock.h defs.h config.h
	$(CC) $(CFLAGS) $(OBJS) $(LIBS) -o driver

clean: 
	-rm -f $(OBJS) driver core *~ *.o

submit:
	@rm -f $(TURNIN)
	@tar czf $(TURNIN) kernels.c
	@./submit.pl
	@rm -f $(TURNIN)
