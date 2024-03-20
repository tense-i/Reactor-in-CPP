all:client tcpepoll
client:client.cpp
  g++ -g -o client client.cpp
tcpepoll:tcpepoll.cpp inetaddress.cpp Sock.cpp
  g++ -g -o tcpepoll tcpepoll.cpp inetaddress.cpp Sock.cpp

clean:
rm -f client tcpepoll