#include "random_util.h"


int random(int start, int end) {
	std::random_device rd;
	std::mt19937 gen(rd());
	uniform_int_distribution<> distrib(start, end);
	return distrib(gen);
}

template<typename T>
const T random(int start, int end) {
	return (T)random(start, end);
}

int random_default() {
	default_random_engine e;
	return e();
}

string random_string( int beg, int end ){

    int sz = random( beg, end );
    string res( (size_t)sz, random( 0, 26 ) + 'a' );
     /*while (sz--) {
          res.push_back(random(0, 26) + 'a');
     }*/
     return res;
}
