# Makefile for myftp client side

VPATH = ../

myftp: myftp.c command.o stream.o 
	gcc -Wall -o myftp myftp.c command.o stream.o 

command.o: command.c command.h  
	gcc -Wall -c command.c -o command.o 

stream.o: stream.c stream.h
	gcc -Wall -c ../stream.c -o stream.o

clean:
	rm *.o myftp
