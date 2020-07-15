# Makefile for myftp client side

myftp: myftp.c stream.o token.o
	gcc -o myftp myftp.c stream.o 

command.o: command.c command.h stream.h
	gcc -c command.c stream.o

stream.o: stream.c stream.h
	gcc -c stream.c stream.h

