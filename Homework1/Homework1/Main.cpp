#include <iostream>
#include <string>
#include "Student.h"
#include "CommandHandler.h"

int main() {

	std::string command;
	std::getline(std::cin, command);

	while (command.compare("quit")) {
		CommandHandler::handleCommand(command);
		std::getline(std::cin, command);
	}

	return 0;
}