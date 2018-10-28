#include <iostream>
#include "List.h"

int main() {

	// Create list
	List<int> test;
	test.pushBack(5);
	test.pushBack(2);
	test.pushBack(10);
	test.pushFront(-5);
	test.pushFront(-10);

	std::cout << "Elements: \n";

	// Print List
	for (int p : test)
		std::cout << p << '\t';
	std::cout << '\n';
	
	// Get an Iterator to the element 5
	List<int>::Iterator it = test.find(5);

	std::cout << "Removing all elements after 5:\n";

	// Remove every element after it
	while (it != test.end())
		test.remove(it);

	// Print List
	for (int p : test)
		std::cout << p << '\t';
	std::cout << '\n';

	return 0;
}
