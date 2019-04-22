SHELL := /bin/bash
CC=gcc

CFLAGSBDDEM= $(CFLAGS) -fPIC -DBP_FREE -O3 -fomit-frame-pointer -Wall -g -O2 -Wextra ${INCDIRS}

prognuplot.o : prognuplot.c
	$(CC) -c $(CFLAGSBDDEM) prognuplot.c -o prognuplot.o