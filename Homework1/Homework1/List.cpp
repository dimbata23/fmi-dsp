#include <iostream>
#include "List.h"
#include "Student.h"

List::List() :
	head(nullptr),
	tail(nullptr)
{}


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
}


void List::pop_back()
{
	if (head == tail) {
		clear();
		return;
	}

	tail = tail->pPrev;
	delete tail->pNext;
	tail->pNext = nullptr;
}


void List::pop_front()
{
	if (head == tail) {
		clear();
		return;
	}

	head = head->pNext;
	delete head->pPrev;
	head->pPrev = nullptr;
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
	for (List::Node* node = this->begin(); node != nullptr; node = node->pNext)
		std::cout << node->data << ((node != this->tail) ? (" - ") : ("\n"));
}


void List::remove(Node* node)
{
	node->pPrev->pNext = node->pNext;
	node->pNext->pPrev = node->pPrev;
	delete node;
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
}
