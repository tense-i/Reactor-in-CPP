all: client tcpepoll

client: client.cpp
	g++ -g -o client client.cpp

tcpepoll: tcpepoll.cpp inetaddress.cpp Sock.cpp Epoll.cpp Channel.cpp EventLoop.cpp TcpServer.cpp Acceptor.cpp
	g++ -g -o tcpepoll tcpepoll.cpp inetaddress.cpp Sock.cpp Epoll.cpp Channel.cpp EventLoop.cpp TcpServer.cpp Acceptor.cpp

clean:
	rm -f client tcpepoll

.PHONY: all clean

