#include <iostream>
#include <string>

#include "CrashInspector.h"

class Base {
public:
	Base(std::string const& name) :name{ name } {}
	virtual ~Base() {
		std::cout << __FUNCTION__ << "\n";
	}

	virtual void show() {
		std::cout << __FUNCTION__ << " name: " << name << "\n";
	}
private:
	std::string name;
};

class Derived : public Base {
public:
	Derived(std::string const& name, int age) :Base(name), age{ age } {}
	~Derived() { std::cout << __FUNCTION__ << "\n"; }
	virtual void show() {
		Base::show();
		std::cout << __FUNCTION__ << " age: " << age << "\n";
	}
private:
	int age{ 6 };
};


int main() {

	CrashInspector::EnableAutoDump();

	Base* pb = new Derived("d", 21);
	delete pb;

	pb->show();

	return 0;
}