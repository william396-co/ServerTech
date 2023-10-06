all: createThread threadWithoutJoin threadWithJoin threadArguments 

clean:
	rm -rf createThread threadWithoutJoin threadWithJoin threadArguments

createThread:
	g++ -std=c++17 -o createThread createThread.cpp -Wall -lpthread -g

threadWithoutJoin:
	g++ -std=c++17 -o threadWithoutJoin threadWithoutJoin.cpp -Wall -lpthread -g

threadWithJoin:
	g++ -std=c++17 -o threadWithJoin threadWithJoin.cpp -Wall -lpthread -g

threadArguments:
	g++ -std=c++17 -o threadArguments threadArguments.cpp -Wall -lpthread -g
