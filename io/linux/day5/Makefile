all: server client

clean:
	rm -rf server client

server:
	g++ -std=c++17 src/util.cpp src/Channel.cpp src/InetAddress.cpp src/Socket.cpp src/Epoll.cpp src/EventLoop.cpp src/Server.cpp server.cpp -o server -Wall -g

client:
	g++ -std=c++17 src/util.cpp client.cpp -o client -Wall -g
