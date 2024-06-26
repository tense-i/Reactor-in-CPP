all: client server

client: client.cpp
	g++ -g -o client client.cpp

server: server.cpp InetAddress.cpp Socket.cpp Epoll.cpp Channel.cpp EventLoop.cpp TcpServer.cpp Connection.cpp Acceptor.cpp Buffer.cpp EchoServer.cpp ThreadPool.h TimeStamp.cpp
	g++ -g -o server server.cpp InetAddress.cpp Socket.cpp Epoll.cpp Channel.cpp EventLoop.cpp TcpServer.cpp Connection.cpp Acceptor.cpp Buffer.cpp EchoServer.cpp ThreadPool.cpp TimeStamp.cpp
clean:
	rm -f client server

.PHONY: all clean
