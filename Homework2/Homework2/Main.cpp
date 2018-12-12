#include <iostream>
#include <fstream>
#include <string>
#include "Horo.hpp"


const std::string INVALID_LABEL = "\"";
void printUsage();
std::string cinLabel();


int main() {

	std::string name;
	std::cout << "Enter filename: ";
	std::getline(std::cin, name);
	std::ifstream in(name);

	if (!in.is_open()) {
		std::cerr << "Couldn't open file " << name << std::endl;
		return -1;
	}

	dsp::Horo horo;
	horo.fill(in);
	
	horo.print();
	printUsage();

	std::string buffer;
	do {

		std::cout << "> ";
		std::cin >> buffer;
		if (!buffer.compare("RELEASE")) {

			std::string who = cinLabel();
			if (!who.compare(INVALID_LABEL)) {
				std::cout << "Please put quotes around the label!" << std::endl;
				continue;
			}
			std::cin >> buffer;
			if (!buffer.compare("left"))
				horo.releaseLeft(who);
			else if (!buffer.compare("right"))
				horo.releaseRight(who);
			else if (!buffer.compare("both"))
				horo.releaseBoth(who);
			else
				std::cout << "Unknown argument \"" << buffer << "\"!" << std::endl;

		} 
		
		else if (!buffer.compare("GRAB")) {

			std::string who = cinLabel();
			if (!who.compare(INVALID_LABEL)) {
				std::cout << "Please put quotes around the label!" << std::endl;
				continue;
			}
			std::cin >> buffer;
			if (!buffer.compare("left"))
				horo.grabLeft(who);
			else if (!buffer.compare("right"))
				horo.grabRight(who);
			else if (!buffer.compare("both"))
				horo.grabBoth(who);
			else
				std::cout << "Unknown argument \"" << buffer << "\"!" << std::endl;

		}

		else if (!buffer.compare("INFO")) {

			std::string who = cinLabel();
			if (!who.compare(INVALID_LABEL)) {
				std::cout << "Please put quotes around the label!" << std::endl;
				continue;
			}
			horo.info(who);

		}

		else if (!buffer.compare("ADD")) {

			std::string who = cinLabel();
			std::string label_left = cinLabel();
			std::string label_right = cinLabel();
			if (!who.compare(INVALID_LABEL) || !label_left.compare(INVALID_LABEL) || !label_right.compare(INVALID_LABEL)) {
				std::cout << "Please put quotes around the labels!" << std::endl;
				continue;
			}
			horo.add(who, label_left, label_right);

		}

		else if (!buffer.compare("REMOVE")) {

			std::string who = cinLabel();
			if (!who.compare(INVALID_LABEL)) {
				std::cout << "Please put quotes around the label!" << std::endl;
				continue;
			}
			horo.checkToRemove(who);

		}

		else if (!buffer.compare("SWAP")) {

			std::string who1 = cinLabel();
			std::string who2 = cinLabel();
			if (!who1.compare(INVALID_LABEL) || !who2.compare(INVALID_LABEL)) {
				std::cout << "Please put quotes around the label!" << std::endl;
				continue;
			}
			horo.swap(who1, who2);

		}

		else if (!buffer.compare("PRINT")) {
			horo.print();
		}

		else if (!buffer.compare("HELP")) {
			printUsage();
		}

		else if (!buffer.compare("EXIT")) {
			break;
		}

		else {
			std::cout << "Unknown command! Type HELP for more info." << std::endl;
		}

		std::cout << std::endl;

	} while (true);
	
	return 0;
	
}





void printUsage() {

	std::cout
		<< std::endl
		<< " ----------------- COMMANDS -----------------" << std::endl
		<< "  RELEASE \"<who>\" [left|right|both]" << std::endl
		<< "  GRAB \"<who>\" [left|right|both]" << std::endl
		<< "  INFO \"<who>\"" << std::endl
		<< "  ADD \"<who>\" \"<label-left>\" \"<label-right>\"" << std::endl
		<< "  REMOVE \"<who>\"" << std::endl
		<< "  SWAP \"<who1>\" \"<who2>\"" << std::endl
		<< "  PRINT" << std::endl
		<< "  HELP" << std::endl
		<< "  EXIT" << std::endl << std::endl;

}


std::string cinLabel() {

	std::string result;
	char ch;
	do {
		std::cin.get(ch);
	} while (ch != '"' && ch != '\n');
	if (ch == '"') {
		do {
			std::cin.get(ch);
			result.push_back(ch);
		} while (ch != '"' && ch != '\n');
		if (ch == '"') {
			result.pop_back();
			std::cin.get();
			return result;
		}
	}

	return INVALID_LABEL;
}
