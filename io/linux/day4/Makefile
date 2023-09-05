all: server client

clean:
	rm -rf server client

server:
	g++ -std=c++17 util.cpp Channel.cpp InetAddress.cpp Socket.cpp Epoll.cpp server.cpp -o server -Wall -g

client:
	g++ -std=c++17 util.cpp client.cpp -o client -Wall -g
