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
		std::cout << "Removed " << name << " from Line" << index;
		if (line->empty()) {
			removeLine(index);
			std::cout << "\nLine" << index << " is now empty and was removed!\n" << "Line" << size << " is now Line" << index << "!\n";
		}
		return;
	}

	if (!line->back().getName().compare(name)) {
		line->pop_back();
		std::cout << "Removed " << name << " from Line" << index;
		if (line->empty()) {
			removeLine(index);
			std::cout << "\nLine" << index << " is now empty and was removed!\n" << "Line" << size << " is now Line" << index << "!\n";
		}
		return;
	}

	while (curr->pNext != nullptr) {
		if (!curr->pNext->data.getName().compare(name)) {
			line->remove(curr->pNext);
			std::cout << "Removed first " << name << " from Line" << index;
			if (!curr->data.canBeToleratedBy(curr->pNext->data.getUni())) {
				lines.push_back_empty_list();
				++size;
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
	for (size_t i = 0; i < len; ++i) {
		std::cout << "\nLine" << i << ": ";
		lines[i]->print();
	}
	std::cout << "\n\n";
}


void CongaManager::merge(int index1, int index2)
{
	if (index1 < 0 || index1 >= size || index2 < 0 || index2 >= size) {
		std::cout << "A line with one of the indexes doesn't exist!\n";
		return;
	}

	List* line1 = lines[index1];
	List* line2 = lines[index2];

	if (!line1->back().canBeToleratedBy(line2->front().getUni())) {
		std::cout << "Incompatible people!\n";
		return;
	}

	line1->end()->pNext = line2->begin();
	line2->begin()->pPrev = line1->end();
	line1->setLast(line2->end());
	line2->setFirst(nullptr);
	line2->setLast(nullptr);
	removeLine(index2);
	std::cout << "Merged Line" << index1 << " and Line" << index2 << "\nLine" << size << " is now Line" << index2 << "!\n";
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
		std::cout << "Removed first person from Line" << index << "\n";
	} else {
		lines[index]->pop_back();
		std::cout << "Removed last person from Line" << index << "\n";
	}
	if (lines[index]->empty() && size > 1) {
		removeLine(index);
		std::cout << "Line" << index << " is now empty and was removed!\n" << "Line" << size << " is now Line" << index << "!\n";
	}
}


void CongaManager::removeLine(int index)
{
	if (size > 1) {
		lines.removeAt(index);
		--size;
	}
}
