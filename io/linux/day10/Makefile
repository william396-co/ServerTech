src=$(wildcard src/*.cpp)

all: server client threadTest test

clean:
	rm -rf server client ThreadPoolTest test

server:
	g++ -std=c++17 -lpthread -g $(src) server.cpp -o server

client:
	g++ -std=c++17 -lpthread -g $(src) client.cpp -o client

threadTest:
	g++ -std=c++17 -lpthread -g $(src) ThreadPoolTest.cpp -o ThreadPoolTest

test:
	g++ -std=c++17 -lpthread -g $(src) test.cpp -o test
