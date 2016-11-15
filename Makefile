#
#Makefile for compiling main.cpp
#
#Instruction:
#
#	1)cd to the directory containing all the game files.
#	2)Type "make" or "make all" or "make main" without quote to compile all the files.
#	3)Type "make main.o" without quote to create the object file for main.cpp
#		You can do the same for AnimatedSprite.o and Animation.o.
#	4)Type "make clean" without quote to remove all .o and the 'game' files
#

CC = g++ #Compiler to use.
CFLAGS = -g -Wall #some useful g++ flags when compiling.

all: main

main: AnimatedSprite.o Animation.o main.o
	$(CC) $(CFLAGS) -o game AnimatedSprite.o Animation.o main.o \
		-lpthread -lsfml-graphics -lsfml-window -lsfml-system

AnimatedSprite.o: AnimatedSprite.cpp AnimatedSprite.hpp
	$(CC) $(CFLAGS) -c AnimatedSprite.cpp

Animation.o: Animation.cpp Animation.hpp
	$(CC) $(CFLAGS) -c Animation.cpp

main.o: main.cpp
	$(CC) $(CFLAGS) -c main.cpp

clean:
	rm -f game *.o *~ *.gch


