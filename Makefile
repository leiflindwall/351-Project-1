all: sender recv

sender: sender.o
	g++ sender.o -o sender

recv: recv.o
	g++ recv.o -o recv

sender.o: sender.cpp msg.h
	g++ -g -c sender.cpp

recv.o: recv.cpp msg.h
	g++ -g -c recv.cpp

clean:
	rm -rf *.o recv
	rm -rf *.o sender
	rm -rf recvfile
