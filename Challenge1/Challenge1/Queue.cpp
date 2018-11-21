#include "Queue.hpp"



Queue::Queue() :
	head(nullptr),
	tail(nullptr)
{}


Queue::~Queue() {

	Node* next = head;
	while (next != nullptr) {
		next = head->pNext;
		delete head;
		head = next;
	}

}


void Queue::enqueue(const Student& st) {

	Node* curr = head;
	while (curr != tail) {
		if (curr->data.areFriends(st)) {
			curr->pNext = new Node(st, curr->pNext);
			return;
		}

		curr = curr->pNext;
	}

	if (empty()) {
		head = new Node(st);
		tail = head;
		return;
	}

	if (curr == tail) {
		tail->pNext = new Node(st);
		tail = tail->pNext;
	}

}


void Queue::dequeue() {

	if (tail == head) {
		delete tail;
		tail = nullptr;
		head = nullptr;
		return;
	}

	Node* temp = head;
	head = head->pNext;
	delete temp;

}


void Queue::incrementTimes() {

	Node* curr = head;
	while (curr != nullptr) {
		curr->data.incrementTime();
		curr = curr->pNext;
	}

}
