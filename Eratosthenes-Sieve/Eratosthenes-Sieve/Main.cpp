#include <iostream>
#include "BitContainer.h"


// TODO: Using command line arguments
int main() {
	size_t maxNum;

	std::cout << " --- Calculating and printing all primes up to a number using the Sieve of Eratosthenes ---\n";
	std::cout << "Enter upper bound: ";
	std::cin >> maxNum;
	std::cout << "Calculating all primes up to " << maxNum << " ...";

	// The Container is filled with 0's by default
	BitContainer composite(maxNum + 1);
	for (size_t i = 2; i <= maxNum; ++i)
		if (!composite[i])
			for (size_t j = i * i; j <= maxNum; j += i)
				composite[j] = 1;

	std::cout << "\nAll primes up to " << maxNum << " are:\n";
	for (size_t i = 2; i <= maxNum; ++i)
		if (!composite[i])
			std::cout << i << '\t';

	std::cout << '\n';

	return 0;
}
