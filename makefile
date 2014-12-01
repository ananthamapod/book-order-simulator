COMPILER = gcc
CCFLAGS = -pedantic -Wall -std=c99 -g
all: bookSim

debug:
	make DEBUG=TRUE
bookSim: bookSim.o tknz3r.o hash.o producer.o queue.o consumerThread.o
	$(COMPILER) $(CCFLAGS) -o bookSim bookSim.o tknz3r.o hash.o producer.o queue.o consumerThread.o -lpthread

bookSim.o: bookSim.c
	$(COMPILER) $(CCFLAGS) -c bookSim.c
	
tknz3r.o: tknz3r.c tknz3r.h
	$(COMPILER) $(CCFLAGS) -c tknz3r.c tknz3r.h

hash.o: hash.c hash.h
	$(COMPILER) $(CCFLAGS) -c hash.c hash.h
	
producer.o: producer.c producer.h
	$(COMPILER) $(CCFLAGS) -c producer.c producer.h

queue.o: queue.c queue.h
	$(COMPILER) $(CCFLAGS) -c queue.c queue.c

comsumerThread.o: consumerThread.c consumerThread.h
	$(COMPLIER) $(CCFLAGS) -c consumerThread.c consumerThread.h
	
clean:
	rm -f bookSim
	rm -f *.o
	rm -f *.gch
