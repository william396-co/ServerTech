all: singletonSingleThreaded singletonMeyers singletonLock singletonAcquireRelease singletonSequentialConsistency singletonCallOnce 

clean:
	rm -rf singletonSingleThreaded singletonMeyers singletonLock singletonAcquireRelease singletonSequentialConsistency singletonCallOnce 

singletonAcquireRelease:
	g++ -std=c++17 -o singletonAcquireRelease singletonAcquireRelease.cpp -Wall -lpthread -O2
   
singletonSequentialConsistency:
	g++ -std=c++17 -o singletonSequentialConsistency singletonSequentialConsistency.cpp -Wall -lpthread -O2

singletonCallOnce:
	g++ -std=c++17 -o singletonCallOnce singletonCallOnce.cpp -Wall -lpthread -O2


singletonLock:
	g++ -std=c++17 -o singletonLock singletonLock.cpp -Wall -lpthread -O2 

singletonMeyers:
	g++ -std=c++17 -o singletonMeyers singletonMeyers.cpp -Wall -lpthread -O2

singletonSingleThreaded:
	g++ -std=c++17 -o singletonSingleThreaded singletonSingleThreaded.cpp -Wall -O2

