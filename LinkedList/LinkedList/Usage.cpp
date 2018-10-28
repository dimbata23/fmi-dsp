#include <iostream>
#include "List.h"

int main() {

	// Create list
	List<int> list;
	list.pushBack(5);
	list.pushBack(2);
	list.pushBack(10);
	list.pushFront(-5);
	list.pushFront(-10);

	std::cout << "Elements: \n";

	// Print List
	for (int p : list)
		std::cout << p << '\t';
	std::cout << '\n';
	
	// Get an Iterator to the element 5
	List<int>::Iterator it = list.find(5);

	std::cout << "Removing all elements after 5:\n";

	// Remove every element after it
	while (it != list.end())
		list.remove(it);

	// Print List
	for (int p : list)
		std::cout << p << '\t';
	std::cout << '\n';

	return 0;
}
