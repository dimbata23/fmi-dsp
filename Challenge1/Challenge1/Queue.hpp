#pragma once
#include "Student.hpp"

class Queue {

public:

	Queue();
	Queue(const Queue&) = delete;				// We don't need it
	Queue& operator=(const Queue&) = delete;	// Again not needed
	~Queue();

private:

	struct Node {
		Node(const Student st, Node* next = nullptr) : 
			data(st), pNext(next) {}

		Student data;
		Node* pNext;
	};

public:

	void enqueue(const Student& st);
	void dequeue();
	const Student& peek() const { return head->data; };
	bool empty() const { return head == nullptr; };
	void incrementTimes();

private:

	Node* head;
	Node* tail;

};
