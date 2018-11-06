#include <iostream>
#include <string>
#include "Student.h"

int main() {

	Student st1, st2;
	std::string buffer;
	std::cout << "Entering info for 2 students...\n";
	std::cout << "Enter a name for Student1: ";
	std::cin >> buffer;
	st1.setName(buffer);
	std::cout << "Enter a uni for Student1: ";
	std::cin >> buffer;
	st1.setUni(buffer);
	std::cout << "Enter a name for Student2: ";
	std::cin >> buffer;
	st2.setName(buffer);
	std::cout << "Enter a uni for Student2: ";
	std::cin >> buffer;
	st2.setUni(buffer);

	std::cout << "\nThe students are " << st1 << " and " << st2 << '\n';

}