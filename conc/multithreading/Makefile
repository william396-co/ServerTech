all: createThread threadWithoutJoin threadWithJoin threadArguments threadMethods coutUnsynchronised coutSynchronised deadlock deadlockResolved deadlockResolvedScopedLock readerWriterLock

clean:
	rm -rf createThread threadWithoutJoin threadWithJoin threadArguments threadMethods coutUnsynchronised coutSynchronised deadlock deadlockResolved deadlockResolvedScopedLock readerWriterLock


readerWriterLock:
	g++ -std=c++17 -o readerWriterLock readerWriterLock.cpp -Wall -lpthread -g

createThread:
	g++ -std=c++17 -o createThread createThread.cpp -Wall -lpthread -g

threadWithoutJoin:
	g++ -std=c++17 -o threadWithoutJoin threadWithoutJoin.cpp -Wall -lpthread -g

threadWithJoin:
	g++ -std=c++17 -o threadWithJoin threadWithJoin.cpp -Wall -lpthread -g

threadArguments:
	g++ -std=c++17 -o threadArguments threadArguments.cpp -Wall -lpthread -g

threadMethods:
	g++ -std=c++17 -o threadMethods threadMethods.cpp -Wall -lpthread -g

coutUnsynchronised:
	g++ -std=c++17 -o coutUnsynchronised coutUnsynchronised.cpp -Wall -lpthread -g

coutSynchronised:
	g++ -std=c++17 -o coutSynchronised coutSynchronised.cpp -Wall -lpthread -g

deadlock:
	g++ -std=c++17 -o deadlock deadlock.cpp -Wall -lpthread -g

deadlockResolved:
	g++ -std=c++17 -o deadlockResolved deadlockResolved.cpp -Wall -lpthread -g

deadlockResolvedScopedLock:
	g++ -std=c++17 -o deadlockResolvedScopedLock deadlockResolvedScopedLock.cpp -Wall -lpthread -g
