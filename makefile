COMPILER = gcc
CCFLAGS = -pedantic -Wall -std=c99 -g
all: bookSim

debug:
	make DEBUG=TRUE
bookSim:
	$(COMPILER) $(CCFLAGS) -o bookSim bookSim.c tknz3r.c tknz3r.h -lpthread

clean:
	rm -f bookSim
	rm -f *.o
	rm -f *.gch
