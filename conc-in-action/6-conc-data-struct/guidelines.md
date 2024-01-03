guidelines for designing data structure for concurrency

.Ensure that no thread can see a state where the invariantes of the data
	structure have been broken by the actions of another thread

.Take care to avoid race conditions inherent in the interface to the data structure
	by porviding functions for complete operations rather than for operation steps

.Pay attention to how the data structure behaves in the presence of exceptions to 
	ensure that the invariants are not broken.

.Minimize the opportunities for deadlock when using the data structure by restrcting the 
	scope of locks and avoiding nested locks where possible.

