COMPILER = gcc
CCFLAGS = -pedantic -Wall -std=c99 -g
all: bookSim

debug:
	make DEBUG=TRUE
bookSim: bookSim.o tknz3r.o queue.o producer.o
	$(COMPILER) $(CCFLAGS) -o bookSim bookSim.o tknz3r.o queue.o producer.o -lpthread

bookSim.o: bookSim.c
	$(COMPILER) $(CCFLAGS) -c bookSim.c
	
tknz3r.o: tknz3r.c tknz3r.h
	$(COMPILER) $(CCFLAGS) -c tknz3r.c tknz3r.h

queue.o: queue.c queue.h
	$(COMPILER) $(CCFLAGS) -c queue.c queue.h
	
producer.o: producer.c producer.h
	$(COMPILER) $(CCFLAGS) -c producer.c producer.h

makeQueueTable.o: makeQueueTable.c makeQueueTable.h
	$(COMPILER) $(CCFLAGS) -c makeQueueTable.c makeQueueTable.h
	
clean:
	rm -f bookSim
	rm -f *.o
	rm -f *.gch
