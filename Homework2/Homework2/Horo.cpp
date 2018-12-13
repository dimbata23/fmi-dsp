#include <iostream>
#include <fstream>
#include "Horo.hpp"


const int DEFAULT_SIZES[] = { 17, 31, 67, 127, 277, 541, 1373, 2539, 4721, 8581, 16447, 32749 };
const short NUM_OF_DEFAULT_SIZES = sizeof(DEFAULT_SIZES) / sizeof(DEFAULT_SIZES[0]);


dsp::Horo::Horo() :
	capacity(DEFAULT_SIZES[0]),
	size(0),
	first(nullptr),
	currDefaultSize(0),
	disbanded(false)
{
	arr.resize(DEFAULT_SIZES[0], nullptr);
}


dsp::Horo::~Horo() {

	for (ListNode* p : arr) {
		ListNode* curr = p;
		while (curr) {
			delete curr->data;
			curr->data = nullptr;
			ListNode* del = curr;
			curr = curr->pNext;
			delete del;
			del = nullptr;
		}
	}

}


// Really simple hash function
size_t dsp::Horo::hash(const std::string str) const {

	size_t result = 0;

	for (const char& c : str)
		result = (result << 1) ^ (int)c;

	return result;

}


dsp::Horo::Participant* dsp::Horo::insert(const std::string name) {

	++size;
	if (size > 2*capacity)
		if (!resize())
			return nullptr;
	int index = hash(name) % capacity;
	ListNode* newN = new ListNode(name);
	return insertAt(arr[index], newN);

}


dsp::Horo::Participant* dsp::Horo::insertAt(ListNode*& place, ListNode* node) {

	node->pNext = place;
	if (place)
		place->pPrev = node;
	if (size == 1)
		first = node->data;
	node->pPrev = nullptr;
	return (place = node)->data;

}


void dsp::Horo::insert(const std::string name, Participant* leftNeighbor, Participant* rightNeighbor) {

	Participant* inserted = insert(name);
	if (!inserted)
		return;
	inserted->pLeft = leftNeighbor;
	inserted->pRight = rightNeighbor;
	inserted->isHoldingLeft = true;
	inserted->isHoldingRight = true;
	leftNeighbor->pRight = inserted;
	leftNeighbor->isHoldingRight = true;
	rightNeighbor->pLeft = inserted;
	rightNeighbor->isHoldingLeft = true;

}


dsp::Horo::ListNode* dsp::Horo::get(const std::string& name) const {

	ListNode* curr = arr[hash(name) % capacity];

	while (curr) {
		if (!curr->data->name.compare(name))
			return curr;
		curr = curr->pNext;
	}

	return nullptr;
}


void dsp::Horo::changeRelation(const std::string& who, char dir, bool grab)
{
	ListNode* person = get(who);
	if (!person) {
		std::cout << who << " is not participating in the horo!" << std::endl;
		return;
	}

	if (dir == 0) {
		person->data->isHoldingLeft = grab;
		person->data->isHoldingRight = grab;
		std::cout << who << " is no" << (grab ? "w" : " longer") << " holding " << person->data->pLeft->name 
				  << (grab ? " and " : " nor ") << person->data->pRight->name;
	} else if (dir == 1) {
		person->data->isHoldingRight = grab;
		std::cout << who << " is no" << (grab ? "w" : " longer") << " holding " << person->data->pRight->name;
	} else {
		person->data->isHoldingLeft = grab;
		std::cout << who << " is no" << (grab ? "w" : " longer") << " holding " << person->data->pLeft->name;
	}

	std::cout << '.' << std::endl;

}


void dsp::Horo::checkToRemove(Participant* who) {

	ListNode* person = get(who->name);
	checkToRemove(person);

}


void dsp::Horo::checkToRemove(ListNode* person) {

	if (!person->data->isHoldingLeft && !person->data->isHoldingRight) {
		if (!person->data->pLeft->isHoldingRight && !person->data->pRight->isHoldingLeft) {
			std::cout << person->data->name << " is \"Free at last!\"" << std::endl;
			removeFromHoro(person);
			return;
		}
	}

	std::cout << "This won't be so easy!" << std::endl;

}


void dsp::Horo::removeFromHoro(ListNode* who) {

	--size;
	if (size <= 2) {
		disband();
		return;
	}

	if (who->pNext)
		who->pNext->pPrev = who->pPrev;
	if (who->pPrev)
		who->pPrev->pNext = who->pNext;
	if (first == who->data)
		first = who->data->pRight;
	if (!who->pPrev)
		arr[hash(who->data->name) % capacity] = who->pNext;

	who->data->pRight->pLeft = who->data->pLeft;
	who->data->pLeft->pRight = who->data->pRight;

	delete who->data;
	delete who;

}


void dsp::Horo::disband() {

	std::cout << std::endl << "...and the music stops!" << std::endl;
	disbanded = true;

}


bool dsp::Horo::resize() {

	++currDefaultSize;
	if (currDefaultSize > NUM_OF_DEFAULT_SIZES - 1) {
		std::cerr << "You have reached the maximum allowed participants!" << std::endl;
		return false;
	}

	std::vector<ListNode*> newArr;
	int newCapacity = DEFAULT_SIZES[currDefaultSize];
	newArr.resize(newCapacity, nullptr);
	
	for (int i = 0; i < capacity; ++i) {
		ListNode* curr = arr[i];
		while (curr) {
			ListNode* next = curr->pNext;
			int index = hash(curr->data->name) % newCapacity;
			insertAt(newArr[index], curr);
			curr = next;
		}
		arr[i] = nullptr;
	}

	arr = newArr;
	capacity = newCapacity;

	return true;
}


void dsp::Horo::fill(std::ifstream& in) {

	std::string name;
	std::getline(in, name);
	insert(name);
	std::getline(in, name);
	dsp::Horo::Participant* prev = insert(name);
	first->pRight = prev;
	first->isHoldingRight = true;
	prev->pLeft = first;
	prev->isHoldingLeft = true;
	dsp::Horo::Participant* curr = nullptr;

	while (!in.eof()) {
		std::getline(in, name);
		if (name.length() == 0)
			continue;
		curr = insert(name);
		if (!curr)
			return;
		prev->pRight = curr;
		prev->isHoldingRight = true;
		curr->pLeft = prev;
		curr->isHoldingLeft = true;
		prev = curr;
	}

	curr->pRight = first;
	curr->isHoldingRight = true;
	first->pLeft = curr;
	first->isHoldingLeft = true;

}


void dsp::Horo::print() const {

	Participant* curr = first;
	do {
		std::cout << curr->name << std::endl;
		curr = curr->pRight;
	} while (curr && curr != first);

}


void dsp::Horo::checkToRemove(const std::string& who) {

	ListNode* person = get(who);
	if (!person) {
		std::cout << who << " is not participating in the horo!" << std::endl;
		return;
	}

	checkToRemove(person);

}


void dsp::Horo::info(const std::string& who) const {

	ListNode* personNode = get(who);
	if (!personNode) {
		std::cout << who << " is not participating in the horo!" << std::endl;
		return;
	}
	
	Participant* person = personNode->data;

	std::cout  
		<< (person->pRight->name)
		<< (person->isHoldingRight ? " <-" : " --")
		<< (person->pRight->isHoldingLeft ? "-> " : "-- ")
		<< (person->name)
		<< (person->pLeft->isHoldingRight ? " <-" : " --")
		<< (person->isHoldingLeft ? "-> " : "-- ")
		<< (person->pLeft->name)
		<< std::endl;

}


void dsp::Horo::add(const std::string& who, const std::string& neighbor1, const std::string& neighbor2) {

	ListNode* n1Node = get(neighbor1);
	if (!n1Node) {
		std::cout << neighbor1 << " is not participating in the horo!" << std::endl;
		return;
	}

	ListNode* n2Node = get(neighbor2);
	if (!n2Node) {
		std::cout << neighbor2 << " is not participating in the horo!" << std::endl;
		return;
	}

	Participant* leftPerson = n1Node->data;
	Participant* rightPerson = n2Node->data;

	if (leftPerson->pLeft == rightPerson)
		std::swap(leftPerson, rightPerson);

	if (rightPerson->pLeft != leftPerson) {
		std::cout << neighbor1 << " and " << neighbor2 << " are not neighbors!" << std::endl;
		return;
	}

	insert(who, leftPerson, rightPerson);
	std::cout << who << " is now inbetween " << leftPerson->name << " and " << rightPerson->name << '.' << std::endl;

}


void dsp::Horo::swap(const std::string& who1, const std::string& who2) {

	ListNode* n1Node = get(who1);
	if (!n1Node) {
		std::cout << who1 << " is not participating in the horo!" << std::endl;
		return;
	}

	ListNode* n2Node = get(who2);
	if (!n2Node) {
		std::cout << who2 << " is not participating in the horo!" << std::endl;
		return;
	}

	Participant* leftPerson = n1Node->data;
	Participant* rightPerson = n2Node->data;

	if (leftPerson->pLeft == rightPerson)
		std::swap(leftPerson, rightPerson);

	if (rightPerson->pLeft != leftPerson) {
		std::cout << who1 << " and " << who2 << " are not neighbors!" << std::endl;
		return;
	}

	if (leftPerson->isHoldingLeft) {
		std::cout << "Cannot swap " << leftPerson->name << " and " << rightPerson->name << ", because " 
				  << leftPerson->name << " is holding " << leftPerson->pLeft->name << '!' << std::endl;
		return;
	}

	if (rightPerson->isHoldingRight) {
		std::cout << "Cannot swap " << leftPerson->name << " and " << rightPerson->name << ", because "
			<< rightPerson->name << " is holding " << rightPerson->pRight->name << '!' << std::endl;
		return;
	}

	if (rightPerson->pRight->isHoldingLeft) {
		std::cout << "Cannot swap " << leftPerson->name << " and " << rightPerson->name << ", because "
			<< rightPerson->pRight->name << " is holding " << rightPerson->name << '!' << std::endl;
		return;
	}

	if (leftPerson->pLeft->isHoldingRight) {
		std::cout << "Cannot swap " << leftPerson->name << " and " << rightPerson->name << ", because "
			<< leftPerson->pLeft->name << " is holding " << leftPerson->name << '!' << std::endl;
		return;
	}

	Participant* mostLeftPerson = leftPerson->pLeft;
	Participant* mostRightPerson = rightPerson->pRight;
	mostLeftPerson->pRight = rightPerson;
	mostRightPerson->pLeft = leftPerson;
	leftPerson->pLeft = rightPerson;
	rightPerson->pRight = leftPerson;
	rightPerson->pLeft = mostLeftPerson;
	leftPerson->pRight = mostRightPerson;

	rightPerson->isHoldingRight = leftPerson->isHoldingRight;
	leftPerson->isHoldingLeft = rightPerson->isHoldingLeft;
	leftPerson->isHoldingRight = false;
	rightPerson->isHoldingLeft = false;

	std::cout << leftPerson->name << " and " << rightPerson->name << " have been swapped!" << std::endl;

}
