all: calcWithLoop calcWithLock calcWithAtomic synchronisationWithLock synchronisationWithAtomic synchronisationWithFetchAdd synchronisationWithFetchAddRelaxed localVariable localVariableAtomic localVariableAtomicRelaxed threadlocalSummation taskSummation 

clean:
	rm -rf calcWithLoop calcWithLock calcWithAtomic synchronisationWithLock synchronisationWithAtomic synchronisationWithFetchAdd synchronisationWithFetchAddRelaxed localVariable localVariableAtomic localVariableAtomicRelaxed threadlocalSummation taskSummation

taskSummation:
	g++ -std=c++17 -o taskSummation taskSummation.cpp -Wall -lpthread -g

threadlocalSummation:
	g++ -std=c++17 -o threadlocalSummation threadlocalSummation.cpp -Wall -lpthread -g

localVariableAtomicRelaxed:
	g++ -std=c++17 -o localVariableAtomicRelaxed localVariableAtomicRelaxed.cpp -Wall -lpthread -g

localVariableAtomic:
	g++ -std=c++17 -o localVariableAtomic localVariableAtomic.cpp -Wall -lpthread -g

localVariable:
	g++ -std=c++17 -o localVariable localVariable.cpp -Wall -lpthread -g

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

