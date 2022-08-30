#include <iostream>
#include <cstdio>
#include <string>

class Counter {

private:
	int count;
public:
	Counter();
	void Reset();
	void increase();
	void PrintCounter();
	//virtual ~Counter();
	Counter operator++();
	Counter operator+(Counter x);
	Counter operator*(Counter xy);
};

Counter::Counter() {
	count = 0;
}

void Counter::Reset() {
	count = 0;
}

void Counter::increase() {
	count++;
}

void Counter::PrintCounter() {
	printf("Present Counter value is = %d\n", count);
}
/*
*/
Counter Counter::operator++() {
	Counter temp;
	++count;
	temp.count = count;
	return temp;
}

Counter Counter::operator+(Counter x) {
	Counter temp1;
	temp1.count = count + x.count;
	return temp1;
}

Counter Counter::operator*(Counter xy) {
	Counter mul;
	mul.count = count * xy.count;
	return mul;
}


void main() {

	Counter a, b, c;
	b.increase();
	++b;
	c.increase();
	++c;
	a = b.operator*(c);

	a.PrintCounter();
}