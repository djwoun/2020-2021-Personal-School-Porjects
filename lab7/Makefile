# Makefile for:
# CS130 - Spring 2021
# lab

CC     = g++
CFLAGS = -g -o0 -std=c++11 -Wall -Werror
FILES  = lab.cpp

all: lab

# $@ is "The name of the category"
# $^ is "All of the files after the colon"
lab: $(FILES)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	$(RM) lab++ 
