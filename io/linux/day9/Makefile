all: server client threadTest

clean:
	rm -rf server client threadTest

threadTest:
	g++ -std=c++17 -pthread src/util.cpp src/ThreadPool.cpp ThreadPoolTest.cpp -o ThreadPoolTest

server:
	g++ -std=c++17  server.cpp  src/Buffer.cpp src/ThreadPool.cpp \
		 	src/util.cpp src/Channel.cpp src/InetAddress.cpp src/Socket.cpp src/Epoll.cpp \
			src/Connection.cpp src/EventLoop.cpp src/Acceptor.cpp src/Server.cpp -o server -Wall -g -lpthread

client:
	g++ -std=c++17 src/util.cpp src/Socket.cpp src/InetAddress.cpp src/Buffer.cpp \
   	client.cpp -o client -Wall -g
