 all: hello_thread spinlockAcquireRelease spinlock condition_variable atomic_condition fetch_mult atomic_shared_ptr atomic_shared_ptr_ref producerConsumer transitivity acquireReleaseWithWaiting releaseSequence acquireRelease acquireConsume relaxed acquireReleaseFences atomicSignal

clean:
	rm -rf hello_thread spinlockAcquireRelease  spinlock condition_variable atomic_condition fetch_mult atomic_shared_ptr atomic_shared_ptr_ref producerConsumer transitivity acquireReleaseWithWaiting releaseSequence acquireRelease acquireConsume relaxed acquireReleaseFences atomicSignal

hell_thread:
	g++ -std=c++17 -o hello_thread hello_thread.cpp -Wall -lpthread -g

spinlockAcquireRelease:
	g++ -std=c++17 -o spinlockAcquireRelease spinlockAcquireRelease.cpp -Wall -lpthread -g

spinlock:
	g++ -std=c++17 -o spinlock spinlock.cpp -Wall -lpthread -g

condition_variable:
	g++ -std=c++17 -o condition_variable condition_variable.cpp -Wall -lpthread -g

atomic_condition:
	g++ -std=c++17 -o atomic_condition atomic_condition.cpp -Wall -lpthread -g

fetch_mult:
	g++ -std=c++17 -o fetch_mult fetch_mult.cpp -Wall -lpthread -g

atomic_shared_ptr:
	g++ -std=c++17 -o atomic_shared_ptr atomic_shared_ptr.cpp -Wall -lpthread -g

atomic_shared_ptr_ref:
	g++ -std=c++17 -o atomic_shared_ptr_ref atomic_shared_ptr_ref.cpp -Wall -lpthread -g

producerConsumer:
	g++ -std=c++17 -o producerConsumer producerConsumer.cpp -Wall -lpthread -g

transitivity:
	g++ -std=c++17 -o transitivity transitivity.cpp -Wall -lpthread -g

acquireReleaseWithWaiting:
	g++ -std=c++17 -o acquireReleaseWithWaiting acquireReleaseWithWaiting.cpp -Wall -lpthread -g

releaseSequence:
	g++ -std=c++17 -o releaseSequence releaseSequence.cpp -Wall -lpthread -g

acquireRelease:
	g++ -std=c++17 -o acquireRelease acquireRelease.cpp -Wall -lpthread -g

acquireConsume:
	g++ -std=c++17 -o acquireConsume acquireConsume.cpp -Wall -lpthread -g

relaxed:
	g++ -std=c++17 -o relaxed relaxed.cpp -Wall -lpthread -g

acquireReleaseFences:
	g++ -std=c++17 -o acquireReleaseFences acquireReleaseFences.cpp -Wall -lpthread -g

atomicSignal:
	g++ -std=c++17 -o atomicSignal atomicSignal.cpp -Wall -lpthread -g
