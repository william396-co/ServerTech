#pragma once
#include <future>

extern std::promise<double> some_promise;

double square_root( double x );

double calc_value();

void future_exception_example();

void shared_future_example();

