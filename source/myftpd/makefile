# Makefile for server

#VPATH=./:../

myftpd: myftpd.c commandd.o stream.o
	gcc -Wall -o myftpd myftpd.c commandd.o stream.o
	
stream.o: ../stream.c ../stream.h
	gcc -Wall -c ../stream.c -o stream.o

commandd.o: commandd.c commandd.h
	gcc -Wall -c commandd.c -o commandd.o

clean:
	rm *.o myftpd
