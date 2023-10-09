all: calcWithLoop calcWithLock calcWithAtomic synchronisationWithLock synchronisationWithAtomic synchronisationWithFetchAdd synchronisationWithFetchAddRelaxed 

clean:
	rm -rf calcWithLoop calcWithLock calcWithAtomic synchronisationWithLock synchronisationWithAtomic synchronisationWithFetchAdd synchronisationWithFetchAddRelaxed

synchronisationWithFetchAdd:
	g++ -std=c++17 -o synchronisationWithFetchAdd synchronisationWithFetchAdd.cpp -Wall -lpthread -g

synchronisationWithFetchAddRelaxed:
	g++ -std=c++17 -o synchronisationWithFetchAddRelaxed synchronisationWithFetchAddRelaxed.cpp -Wall -lpthread -g

synchronisationWithAtomic:
	g++ -std=c++17 -o synchronisationWithAtomic synchronisationWithAtomic.cpp -Wall -lpthread -g

synchronisationWithLock:
	g++ -std=c++17 -o synchronisationWithLock synchronisationWithLock.cpp -Wall -lpthread -g

calcWithAtomic:
	g++ -std=c++17 -o calcWithAtomic calcWithAtomic.cpp -Wall -g

calcWithLock:
	g++ -std=c++17 -o calcWithLock calcWithLock.cpp -Wall -g

calcWithLoop:
	g++ -std=c++17 -o calcWithLoop calcWithLoop.cpp -Wall -g

