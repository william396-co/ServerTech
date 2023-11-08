SRC=$(wildcard src/*.cpp)
INCLUDE=$(wildcard include/*.cpp include/*.c)
CLIENT=$(wildcard client/*.cpp)
SERVER=$(wildcard server/*.cpp)

all: multi_client client server test

.PHONY: all clean
clean:
	rm -rf kcp_server kcp_client kcp_test kcp_multi_client

server:
	g++ -std=c++17 -lpthread -g $(INCLUDE) $(SRC) $(SERVER) kcp_server.cpp -o kcp_server

client:
	g++ -std=c++17 -lpthread -g $(INCLUDE) $(SRC) $(CLIENT) kcp_client.cpp -o kcp_client

multi_client:
	g++ -std=c++17 -lpthread -g $(INCLUDE) $(SRC) $(CLIENT) kcp_multi_client.cpp -o kcp_multi_client

test:
	g++ -std=c++17 -lpthread -g $(INCLUDE) test.cpp -o kcp_test

