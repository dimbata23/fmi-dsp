#pragma once

#include <algorithm>
#include "CongaManager.h"
CongaManager sys;

namespace CommandHandler {

	void handleCommand(const std::string& command) {

		size_t numOfArguments = std::count(command.begin(), command.end(), ' ') + 1;
		std::string* arguments = new std::string[numOfArguments];

		size_t cLen = command.length();
		size_t lastPosition = 0;
		size_t currArgument = 0;
		for (size_t i = 0; i < cLen; ++i)
		{
			if (command[i] == ' ') {
				arguments[currArgument] = command.substr(lastPosition, i - lastPosition);
				lastPosition = i+1;
				++currArgument;
			}
			arguments[numOfArguments - 1] = command.substr(lastPosition, cLen - lastPosition);
		}

		if (!arguments[0].compare("append")) {

			if (numOfArguments != 4)
				std::cout << "Wrong number of arguments!\nUsage: append NAME UNI LINEINDEX\n";
			else
				sys.append(arguments[1], arguments[2], atoi(arguments[3].c_str()));

		} else if (!arguments[0].compare("removeLast")) {

			if (numOfArguments != 2)
				std::cout << "Wrong number of arguments!\nUsage: removeLast LINEINDEX\n";
			else
				sys.removeLast(atoi(arguments[1].c_str()));

		} else if (!arguments[0].compare("removeFirst")) {

			if (numOfArguments != 2)
				std::cout << "Wrong number of arguments!\nUsage: removeFirst LINEINDEX\n";
			else
				sys.removeFirst(atoi(arguments[1].c_str()));

		} else if (!arguments[0].compare("remove")) {

			if (numOfArguments != 3)
				std::cout << "Wrong number of arguments!\nUsage: remove NAME LINEINDEX\n";
			else
				sys.remove(arguments[1], atoi(arguments[2].c_str()));

		} else if (!arguments[0].compare("merge")) {

			if (numOfArguments != 3)
				std::cout << "Wrong number of arguments!\nUsage: merge LINEINDEX1 LINEINDEX2\n";
			else
				sys.merge(atoi(arguments[1].c_str()), atoi(arguments[2].c_str()));

		} else if (!arguments[0].compare("print")) {

			if (numOfArguments != 1)
				std::cout << "Wrong number of arguments!\nUsage: print\n";
			else
				sys.print();

		} else if (!arguments[0].compare("help")) {

			if (numOfArguments != 1)
				std::cout << "Wrong number of arguments!\nUsage: help\n";
			else
				std::cout
				<< "\n ----------- Commands ----------- ---------- Description ----------\n"
				<< "  append NAME UNI LINEINDEX        Appends a student to a line\n"
				<< "  removeLast LINEINDEX             Removes last student in a line\n"
				<< "  removeFirst LINEINDEX            Removes first student in a line\n"
				<< "  remove NAME LINEINDEX            Removes certain student from a line\n"
				<< "  merge LINEINDEX1 LINEINDEX2      Merges two lines together\n"
				<< "  print                            Prints all the lines\n"
				<< "  help                             Prints out this helpful message\n"
				<< "  quit                             End program\n"
				<< " -------------------------------- ---------------------------------\n\n";

		} else {

			std::cout << "Unknown command!\nTry: help\n";

		}

		delete[] arguments;

	}

}
