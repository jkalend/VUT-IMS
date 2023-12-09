CC=g++
CFLAGS=-g -O2 -Wall
LDFLAGS=-lsimlib -lm
EXECUTABLE=ims
FILES=LaunchPadProcess.cpp BoosterProcess.cpp StarshipProcess.cpp FAAProcess.cpp RocketProcess.cpp TankerProcess.cpp MarsProcess.cpp

default: ims

ims: main.o LaunchPadProcess.o BoosterProcess.o StarshipProcess.o FAAProcess.o RocketProcess.o TankerProcess.o MarsProcess.o
	$(CC) $(CFLAGS)  -o $(EXECUTABLE) main.cpp $(FILES) $(LDFLAGS)

run: ims
	./ims $(ARGS)
	
%.o: %.cpp %.hpp
	$(CC) $(CFLAGS) -c $< $(LDFLAGS)

clean:
	@rm -rf $(EXECUTABLE)* *.o