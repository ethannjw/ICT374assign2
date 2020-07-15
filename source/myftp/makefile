# Makefile for myftp client side

VPATH = ../

myftp: myftp.c command.o stream.o 
	gcc -o myftp myftp.c command.o stream.o 

command.o: command.c command.h stream.o 
	gcc -c command.c stream.o 

stream.o: stream.c stream.h
	gcc -c ../stream.c

clean:
	rm *.o
