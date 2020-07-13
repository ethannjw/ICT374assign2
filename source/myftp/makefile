# Makefile for c_ex2

ex2: c_ex2.c command.o token.o
	gcc -o ex2 c_ex2.c command.o token.o

command.o: command.c command.h
	gcc -c command.c command.h

token.o: token.c token.h
	gcc -c token.c token.h
