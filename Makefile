CC=g++
CFLAGS=-g -O2 -Wall -std=c++11
LDFLAGS=-lsimlib -lm
SOURCES=main.cc
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=ims

default: ims

ims: main.cc
	$(CC) $(CFLAGS) main.cc -o $(EXECUTABLE)  $(LDFLAGS)
run: ims
	./ims
%.o: %.c %.h
	$(CC) $(CFLAGS) -c $<

clean:
	@rm -rf $(EXECUTABLE)* *.o* ./experiments