#include <iostream>
#include <fstream>
#include <string>
#include "DataManager.hpp"

int main() {

	std::string name;
	std::cout << "Filepath: ";
	std::getline(std::cin, name);
	std::ifstream in(name);
	if (!in) {
		std::cerr << "Couldn't open/find file \"" << name << "\"!" << std::endl;
		return -1;
	}

	DataManager sys;
	sys.fill(in);
	in.close();

	std::cout << "Enter starting point: ";
	std::getline(std::cin, name);
	if (name[0] == '\"')
		name = name.substr(1, name.length() - 2);

	if (!sys.zoneExists(name)) {
		std::cerr << "Zone \"" << name << "\" doesn't exist in the file!" << std::endl;
		return -2;
	}

	sys.exploreZones(name);
	sys.generateDotFile();
	std::cout << "Generated output.gv!" << std::endl;

	return 0;
}
