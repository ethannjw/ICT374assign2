# Makefile for myftp client side

VPATH = ../

myftp: myftp.c command.o stream.o 
	gcc -Wall -o myftp myftp.c command.o stream.o 

command.o: command.c command.h  
	gcc -Wall -c command.c  

stream.o: stream.c stream.h
	gcc -Wall -c ../stream.c

clean:
	rm *.o myftp
