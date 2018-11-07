#pragma once
#include "Student.h"

struct List {

	List();
	List(const List&) = default;	//
	List& operator=(List&);			// Just moving the object
	~List();



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



	Node* begin() const { return head; }
	Node* end() const { return nullptr; }

	void push_back(const Student& elem);
	void pop_back();
	void pop_front();

	const Student& front() const;
	const Student& back() const;

	bool empty() const { return size == 0; }
	void print() const;

	// TODO:
	//Node* remove(Node* node);
	//Node* find(const Student& elem);

	void clear();



	Node* head;
	Node* tail;
	size_t size;

};
