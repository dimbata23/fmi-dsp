#include <iostream>
#include "List.h"
#include "Student.h"

List::List() :
	head(nullptr),
	tail(nullptr),
	size(0)
{}


List& List::operator=(List& other)
{
	clear();
	head = other.head;
	tail = other.tail;
	size = other.size;
	other.head = nullptr;
	other.tail = nullptr;
	return *this;
}


List::~List()
{
	clear();
}


void List::push_back(const Student& elem)
{
	if (tail != nullptr) {
		tail->pNext = new Node(elem, tail);
		tail = tail->pNext;
	}
	else {
		tail = new Node(elem);
		head = tail;
	}
	++size;
}


void List::pop_back()
{
	if (size == 1) {
		clear();
		return;
	}

	tail = tail->pPrev;
	delete tail->pNext;
	tail->pNext = nullptr;
	--size;
}


void List::pop_front()
{
	if (size == 1) {
		clear();
		return;
	}

	head = head->pNext;
	delete head->pPrev;
	head->pPrev = nullptr;
	--size;
}


const Student& List::front() const
{
	return head->data;
}


const Student& List::back() const
{
	return tail->data;
}


void List::print() const
{
	for (List::Node* node = this->begin(); node != this->end(); node = node->pNext)
		std::cout << node->data << ((node != this->tail) ? (" - ") : ("\n"));
}


void List::clear()
{
	while (head != tail) {
		head = head->pNext;
		delete head->pPrev;
	}
	delete tail;
	head = nullptr;
	tail = nullptr;
	size = 0;
}
