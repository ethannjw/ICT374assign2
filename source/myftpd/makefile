# Makefile for server

myftpd: myftpd.c stream.o commandd.o
	gcc -o myftpd myftpd.c stream.o commandd.o
	
commandd.o: commandd.c commandd.h stream.h
	gcc -c commandd.c stream.o

stream.o: stream.c stream.h
	gcc -c stream.c

clean:
	rm *.o
