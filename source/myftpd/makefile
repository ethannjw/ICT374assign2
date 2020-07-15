# Makefile for server

VPATH = ../

myftpd: myftpd.c commandd.o
	gcc -Wall -o myftpd myftpd.c commandd.o stream.o
	
commandd.o: commandd.c commandd.h stream.o
	gcc -Wall -c commandd.c stream.o

stream.o: stream.c stream.h
	gcc -Wall -c ../stream.c

clean:
	rm *.o
