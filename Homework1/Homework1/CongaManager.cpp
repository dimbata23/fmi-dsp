#include <iostream>
#include "CongaManager.h"

CongaManager::CongaManager() : size(1)
{
	lines.push_back_empty_list();
}


CongaManager::~CongaManager()
{}


void CongaManager::append(const std::string& name, const std::string& uni, int index)
{
	if (index < 0 || index >= size) {
		std::cout << "No such line index!\n";
		return;
	}

	if (!lines[index]->empty() && !(lines[index]->back().canBeToleratedBy(uni))) {
		std::cout << "Incompatible people!\n";
		return;
	}

	lines[index]->push_back(Student(name, uni));
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

	List* line = lines[index];
	List::Node* curr = line->begin();
	if (!curr->data.getName().compare(name)) {
		line->pop_front();
		return;
	}

	if (!line->back().getName().compare(name)) {
		line->pop_back();
		return;
	}

	while (curr->pNext != nullptr) {
		if (!curr->pNext->data.getName().compare(name)) {
			line->remove(curr->pNext);
			std::cout << "Removed first " << name << " from Line " << index;
			if (!curr->data.canBeToleratedBy(curr->pNext->data.getUni())) {
				lines.push_back_empty_list();
				curr->pNext->pPrev = nullptr;
				lines.back()->setFirst(curr->pNext);
				lines.back()->setLast(line->end());
				curr->pNext = nullptr;
				line->setLast(curr);
				std::cout << " and created a new Line with index " << lines.size() - 1;
			}
			std::cout << '\n';
			return;
		}
		curr = curr->pNext;
	}
	std::cout << name << " not found in Line " << index << "!\n";
}


void CongaManager::print() const
{
	size_t len = lines.size();
	for (size_t i = 0; i < len; ++i)
		lines[i]->print();
}


void CongaManager::removePerson(int index, bool first)
{
	if (index < 0 || index >= size) {
		std::cout << "No such line index!\n";
		return;
	}

	if (lines[index]->empty()) {
		std::cout << "There are no people on Line" << index << "!\n";
		return;
	}

	if (first) {
		lines[index]->pop_front();
		std::cout << "Removed first person from Line" << index << '\n';
	} else {
		lines[index]->pop_back();
		std::cout << "Removed last person from Line" << index << '\n';
	}
	if (lines[index]->empty())
		removeLine(index);
}


void CongaManager::removeLine(int index)
{
	if (size > 1) {
		lines.removeAt(index);
		--size;
	}
}
