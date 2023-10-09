all: createThread threadWithoutJoin threadWithJoin threadArguments threadMethods coutUnsynchronised coutSynchronised deadlock deadlockResolved deadlockResolvedScopedLock readerWriterLock constexpr callOnce singletonCallOnce meyerSingleton threadLocal threadLocalState conditionVariable asyncVsThread asyncLazy fireAndForgetFutures dotProductAsync packagedTask packagedTaskReuse promiseFuture waitFor sharedFuture promiseFutureException 

clean:
	rm -rf createThread threadWithoutJoin threadWithJoin threadArguments threadMethods coutUnsynchronised coutSynchronised deadlock deadlockResolved deadlockResolvedScopedLock readerWriterLock constexpr callOnce singletonCallOnce meyerSingleton threadLocal threadLocalState conditionVariable asyncVsThread asyncLazy fireAndForgetFutures dotProductAsync packagedTask packagedTaskReuse promiseFuture waitFor sharedFuture promiseFutureException


waitFor:
	g++ -std=c++17 -o waitFor waitFor.cpp -Wall -lpthread -g

promiseFuture:
	g++ -std=c++17 -o promiseFuture promiseFuture.cpp -Wall -lpthread -g

packagedTaskReuse:
	g++ -std=c++17 -o packagedTaskReuse packagedTaskReuse.cpp -Wall -lpthread -g

promiseFutureException:
	g++ -std=c++17 -o promiseFutureException promiseFutureException.cpp -Wall -lpthread -g

sharedFuture:
	g++ -std=c++17 -o sharedFuture sharedFuture.cpp -Wall -lpthread -g

packagedTask:
	g++ -std=c++17 -o packagedTask packagedTask.cpp -Wall -lpthread -g


dotProductAsync:
	g++ -std=c++17 -o dotProductAsync dotProductAsync.cpp -Wall -lpthread -g

fireAndForgetFutures:
	g++ -std=c++17 -o fireAndForgetFutures fireAndForgetFutures.cpp -Wall -lpthread -g

asyncLazy:
	g++ -std=c++17 -o asyncLazy asyncLazy.cpp -Wall -lpthread -g

asyncVsThread:
	g++ -std=c++17 -o asyncVsThread asyncVsThread.cpp -Wall -lpthread -g

conditionVariable:
	g++ -std=c++17 -o conditionVariable conditionVariable.cpp -Wall -lpthread -g

threadLocalState:
	g++ -std=c++17 -o threadLocalState threadLocalState.cpp -Wall -lpthread -g

threadLocal:
	g++ -std=c++17 -o threadLocal threadLocal.cpp -Wall -lpthread -g

meyerSingleton:
	g++ -std=c++17 -o meyerSingleton meyerSingleton.cpp -Wall -lpthread -g

singletonCallOnce:
	g++ -std=c++17 -o singletonCallOnce singletonCallOnce.cpp -Wall -lpthread -g

callOnce:
	g++ -std=c++17 -o callOnce callOnce.cpp -Wall -lpthread -g

constexpr:
	g++ -std=c++17 -o constexpr constexpr.cpp -Wall

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
