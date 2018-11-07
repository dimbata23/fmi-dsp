#include <iostream>
#include "CongaManager.h"

CongaManager::CongaManager() : size(1)
{
	lines.push_back(List());
}


CongaManager::~CongaManager()
{}


void CongaManager::append(const std::string& name, const std::string& uni, int index)
{
	if (index < 0 || index >= size) {
		std::cout << "No such line index!\n";
		return;
	}

	if (!lines[index].empty() && !(lines[index].back().canBeToleratedBy(uni))) {
		std::cout << "Incompatible people!\n";
		return;
	}

	lines[index].push_back(Student(name, uni));
	std::cout << "Added " << name << " to Line" << index << '\n';
}

void CongaManager::removeFirst(int index)
{
	removePerson(index, true);
}


void CongaManager::removeLast(int index)
{
	removePerson(index, false);
}


void CongaManager::remove(const std::string& name, int index)
{
	if (index < 0 || index >= size) {
		std::cout << "No such line index!\n";
		return;
	}

	// TODO: Remove person

}


void CongaManager::print() const
{
	for (const List& i : lines)
		i.print();
}


void CongaManager::removePerson(int index, bool first)
{
	if (index < 0 || index >= size) {
		std::cout << "No such line index!\n";
		return;
	}

	if (lines[index].empty()) {
		std::cout << "There are no people on Line" << index << "!\n";
		return;
	}

	if (first) {
		lines[index].pop_front();
		std::cout << "Removed first person from Line" << index << '\n';
	} else {
		lines[index].pop_back();
		std::cout << "Removed last person from Line" << index << '\n';
	}
	if (lines[index].empty())
		removeLine(index);
}


void CongaManager::removeLine(int index)
{
	if (size > 1) {
		lines[index] = lines[size - 1];
		--size;
	}
}
