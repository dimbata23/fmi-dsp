#pragma once
#include "Student.h"

class List {

public:

	List();
	List(const List&) = delete;					// In this case we just want to move
	List& operator=(const List&) = delete;		// the line of people, no need of copying
	~List();

public:

	struct Node {

		Student data;
		Node* pPrev;
		Node* pNext;

		Node(const Student& data, Node* pPrev = nullptr, Node* pNext = nullptr) :
			data(data),
			pPrev(pPrev),
			pNext(pNext)
		{}

	};

public:

	Node* begin() const { return head; }
	Node* end() const { return tail; }

	void push_back(const Student& elem);
	void pop_back();
	void pop_front();

	const Student& front() const;
	const Student& back() const;

	bool empty() const { return head == nullptr; }
	void print() const;

	void remove(Node* node);
	void setLast(Node* node) { tail = node; }
	void setFirst(Node* node) { head = node; }

private:

	void clear();

private:

	Node* head;
	Node* tail;

};
