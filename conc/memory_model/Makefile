all: spinlock condition_variable

clean:
	rm -rf spinlock condition_variable

spinlock:
	g++ -std=c++17 -o spinlock spinlock.cpp -Wall -lpthread -g

condition_variable:
	g++ -std=c++17 -o condition_variable condition_variable.cpp -Wall -lpthread -g

