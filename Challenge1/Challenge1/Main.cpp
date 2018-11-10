/*
	A solution to the Challenge #1 of the DSP course 2018/19
	
	Start time: 05:29PM
	End time: 06:16PM
	Total time spent: 47 min.

	Author: Alexander Dimitrov
*/

#include <iostream>
#include <fstream>
#include <string>
#include "Queue.hpp"

int main() {

	std::ifstream in("students.txt");
	std::string name;
	Queue queue;
	size_t group;
	size_t globalTime = 0;

	do {

		if (!in.eof()) {
			in >> name;
			if (!in.eof()) {
				in >> group;
				queue.enqueue(Student(name, group));
			}
		}

		if (globalTime > 0 && globalTime % 2 == 0) {
			std::cout << queue.peek().getName() << ' ' << queue.peek().getTime() << "m\n";
			queue.dequeue();
		}

		queue.incrementTimes();
		++globalTime;

	} while (!queue.empty());

	return 0;

}
