all: spinlock condition_variable atomic_condition fetch_mult atomic_shared_ptr atomic_shared_ptr_ref

clean:
	rm -rf spinlock condition_variable atomic_condition fetch_mult atomic_shared_ptr atomic_shared_ptr_ref

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
