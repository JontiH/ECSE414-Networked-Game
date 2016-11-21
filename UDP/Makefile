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

main: UDPSystem.o ServerV2.o
	$(CC) $(CFLAGS) -o server UDPSystem.o ServerV2.cpp

ServerV2.o: ServerV2.cpp 
	$(CC) $(CFLAGS) -c ServerV2.cpp

UDPSystem.o : UDPSystem.cpp UDPSystem.hpp
	$(CC) $(CFLAGS) -c UDPSystem.cpp
clean:
	rm -f server *.o *~ *.gch
