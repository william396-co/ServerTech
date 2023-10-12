all: ongoingOptimisation ongoingOptimisationLock ongoingOptimisationSequentialConsistency ongoingOptimisationAcquireRelease 

clean:
	rm -rf ongoingOptimisation ongoingOptimisationLock ongoingOptimisationSequentialConsistency ongoingOptimisationAcquireRelease

ongoingOptimisationAcquireRelease:
	g++ -std=c++17 -o ongoingOptimisationAcquireRelease ongoingOptimisationAcquireRelease.cpp -Wall -lpthread

ongoingOptimisationSequentialConsistency:
	g++ -std=c++17 -o ongoingOptimisationSequentialConsistency ongoingOptimisationSequentialConsistency.cpp -Wall -lpthread

ongoingOptimisationLock:
	g++ -std=c++17 -o ongoingOptimisationLock ongoingOptimisationLock.cpp -Wall -lpthread

ongoingOptimisation:
	g++ -std=c++17 -o ongoingOptimisation ongoingOptimisation.cpp -Wall -lpthread 
