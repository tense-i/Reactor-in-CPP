all: client tcpepoll

client: client.cpp
	g++ -g -o client client.cpp

tcpepoll: tcpepoll.cpp inetaddress.cpp Sock.cpp Epoll.cpp Channel.cpp
	g++ -g -o tcpepoll tcpepoll.cpp inetaddress.cpp Sock.cpp Epoll.cpp Channel.cpp

clean:
	rm -f client tcpepoll

.PHONY: all clean
