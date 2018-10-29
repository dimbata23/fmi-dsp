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

	// First way of Printing
	List<int>::Iterator startIt = list.begin();
	List<int>::Iterator endIt = list.end();
	while (startIt != endIt) {
		std::cout << *startIt << '\t';
		++startIt;
	}
	std::cout << '\n';
	
	// Get an Iterator to the element 5
	List<int>::Iterator it = list.find(5);

	std::cout << "\nRemoving all elements after 5 (including 5):\n";

	// Remove every element after it (including 5)
	List<int>::Iterator lastIt = list.end();
	while (it != lastIt)
		list.remove(it);

	// Second way of Printing
	for (int p : list)
		std::cout << p << '\t';
	std::cout << '\n';

	// Adding  elements
	list.pushBack(8);
	list.pushBack(13);
	list.pushBack(45);

	std::cout << "\nNew elements:\n";

	// Print List
	for (int p : list)
		std::cout << p << '\t';
	std::cout << '\n';

	std::cout << "\nIterating backwards:\n";

	// Backwards iterator and printing
	List<int>::Iterator iter = list.rbegin();
	List<int>::Iterator lastIter = list.rend();
	while (iter != lastIter) {
		std::cout << *iter << '\t';
		++iter;
	}
	std::cout << '\n';

	return 0;
}
