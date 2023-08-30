#pragma once

template<typename T>
class Singleton {

public:
	static T instance() {
		static T instance;
		return instance;
	}

	Singleton(Singleton const&) = delete;
	~Singleton() {}
protected:
	Singleton() {}
};