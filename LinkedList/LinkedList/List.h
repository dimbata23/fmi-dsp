#pragma once
#include <stdexcept>

template <class T>
class List {

public:

	List<T>();
	List<T>(const List<T>& other);
	List<T>& operator=(const List<T> other);
	~List<T>();

private:

	struct Node {

		T data;
		Node* pPrev;
		Node* pNext;

		Node(const T& data, Node* pPrev = nullptr, Node* pNext = nullptr) :
			data(data),
			pPrev(pPrev),
			pNext(pNext)
		{}

	};

public:

	class Iterator {

		friend class List<T>;

	private:

		Node* pNode;
		bool isFwdIter;

	public:

		Iterator(Node* pNode, bool isFwdIter = true) : pNode(pNode), isFwdIter(isFwdIter) {}

		T& operator*() { 
			if (pNode)
				return pNode->data;
			throw std::logic_error("[List::Iterator]: Iterator was a nullptr!");
		}

		T* operator->() { 
			if (pNode)
				return &pNode->data;
			throw std::logic_error("[List::Iterator]: Iterator was a nullptr!");
		}

		Iterator& operator++() {
			if (isFwdIter)
				pNode = pNode->pNext;
			else
				pNode = pNode->pPrev;
			return *this;
		}

		Iterator operator++(int) {
			Iterator result(*this);
			++(*this);
			return result;
		}

		Iterator& operator--() {
			if (isFwdIter)
				pNode = pNode->pPrev;
			else
				pNode = pNode->pNext;
			return *this;
		}

		Iterator operator--(int) {
			Iterator result(*this);
			++(*this);
			return result;
		}

		bool operator==(const Iterator& other) const { return pNode == other.pNode; }
		bool operator!=(const Iterator& other) const { return !(*this == other); }

	};

public:

	Iterator begin() const { return Iterator(head); }
	Iterator end() const { return Iterator(nullptr); }

	Iterator rbegin() const { return Iterator(tail, false); }
	Iterator rend() const { return Iterator(nullptr); }

	void pushBack(const T& elem);
	void pushFront(const T& elem);
	void popBack();
	void popFront();

	T& front();
	const T& front() const;
	T& back();
	const T& back() const;

	size_t getSize() const { return size; }
	bool isEmpty() const { return size == 0; }

	// Searches the list for a given element, if found
	// returns an Iterator to that element, else Iterator to nullptr
	Iterator find(const T& elem) const;

	// Inserts an element after the element pointed by an Iterator
	// returns Iterator pointing to the new element
	Iterator insertAfter(const Iterator& it, const T& elem);

	// Inserts an element before the element pointed by an Iterator
	// returns an Iterator pointing to the new element
	Iterator insertBefore(const Iterator& it, const T& elem);

	// Removes the element pointed by the Iterator
	// Points the given Iterator to the new element in its position (or nullptr if it was the last element)
	Iterator remove(Iterator& it);

	// Removes the element pointed by the Iterator
	// returns an Iterator pointing to the new element in its position (or nullptr if it was the last element)
	Iterator remove(const Iterator& it);

	// Inserts an element at given position
	// returns an Iterator bointing to the new element
	Iterator insertAt(size_t index, const T& elem);

private:

	void copy(const List<T>& other);
	void clear();
	Iterator remove(const Iterator& it, bool moveIt);

private:

	Node* head;
	Node* tail;
	size_t size;

};







template<class T>
List<T>::List() : 
	head(nullptr), 
	tail(nullptr), 
	size(0)
{}


template<class T>
List<T>::List(const List<T>& other)
{
	copy(other);
}


template<class T>
List<T>::~List()
{
	clear();
}


template<class T>
List<T>& List<T>::operator=(const List<T> other)
{
	if (this != &other) {
		clear();
		copy(other);
	}
	return *this;
}


template<class T>
void List<T>::pushBack(const T& elem)
{
	if (tail != nullptr) {
		tail->pNext = new Node(elem, tail);
		tail = tail->pNext;
	} else {
		tail = new Node(elem);
		head = tail;
	}
	++size;
}


template<class T>
void List<T>::pushFront(const T& elem)
{
	if (head != nullptr) {
		head->pPrev = new Node(elem, nullptr, head);
		head = head->pPrev;
	} else {
		head = new Node(elem);
		tail = head;
	}
	++size;
}


template<class T>
void List<T>::popBack()
{
	if (isEmpty())
		throw std::logic_error("[List]: Can not execute popBack() on an empty list!");

	if (size == 1) {
		delete tail;
		tail = nullptr;
		head = nullptr;
		return;
	}

	tail = tail->pPrev;
	delete tail->pNext;
	tail->pNext = nullptr;
	--size;
}


template<class T>
inline void List<T>::popFront()
{
	if (isEmpty())
		throw std::logic_error("[List]: Can not execute popFront() on an empty list!");

	if (size == 1) {
		delete head;
		head = nullptr;
		tail = nullptr;
		return;
	}

	head = head->pNext;
	delete head->pPrev;
	head->pPrev = nullptr;
	--size;
}


template<class T>
T& List<T>::front()
{
	return const_cast<T&>(static_cast<const List&>(*this).front());
}


template<class T>
const T& List<T>::front() const
{
	return head->data;
}


template<class T>
T& List<T>::back()
{
	return const_cast<T&>(static_cast<const List&>(*this).back());
}


template<class T>
const T& List<T>::back() const
{
	return tail->data;
}


template<class T>
typename List<T>::Iterator List<T>::find(const T& elem) const
{
	Iterator pH(head);
	while (pH != nullptr) {
		if (*pH == elem)
			return pH;
		++pH;
	}
	return nullptr;
}


template<class T>
typename List<T>::Iterator List<T>::insertAfter(const Iterator& it, const T& elem)
{
	if (it == Iterator(tail) || it == nullptr) {
		pushBack(elem);
		return tail;
	}

	Node* newNode = new Node(elem);
	newNode->pNext = it.pNode->pNext;
	newNode->pPrev = it.pNode;
	it.pNode->pNext = newNode;
	newNode->pNext->pPrev = newNode;
	++size;
	return newNode;
}


template<class T>
typename List<T>::Iterator List<T>::insertBefore(const Iterator& it, const T& elem)
{
	if (it == begin() || it == nullptr) {
		pushFront(elem);
		return head;
	}

	Node* newNode = new Node(elem);
	newNode->pNext = it.pNode;
	newNode->pPrev = it.pNode->pPrev;
	it.pNode->pPrev = newNode;
	newNode->pPrev->pNext = newNode;
	++size;
	return newNode;
}


template<class T>
typename List<T>::Iterator List<T>::remove(const Iterator& it, bool moveIt) 
{
	if (it.pNode == nullptr)
		return nullptr;

	if (it.pNode == tail) {
		popBack();
		if (moveIt)
			const_cast<Iterator&>(it) = nullptr;
		return tail;
	}
	if (it.pNode == head) {
		popFront();
		if (moveIt)
			const_cast<Iterator&>(it) = head;
		return head;
	}
	it.pNode->pPrev->pNext = it.pNode->pNext;
	it.pNode->pNext->pPrev = it.pNode->pPrev;
	Node* toDel = it.pNode;
	Iterator result(it.pNode->pNext);
	if (moveIt)
		const_cast<Iterator&>(it).pNode = it.pNode->pNext;
	--size;
	delete toDel;
	return result;
}


template<class T>
typename List<T>::Iterator List<T>::remove(Iterator& it)
{
	return remove(it, true);
}


template<class T>
typename List<T>::Iterator List<T>::remove(const Iterator& it)
{
	return remove(it, false);
}


template<class T>
typename List<T>::Iterator List<T>::insertAt(size_t index, const T& elem)
{
	if (index == 0) {
		pushFront(elem);
		return head;
	}
	if (index == size) {
		pushBack(elem);
		return tail;
	}
	if (index > size)
		throw std::logic_error("[List]: Inserting at position larger than the list!");
	
	Iterator it = begin();
	while (index > 1) {
		--index;
		++it;
	}

	insertAfter(it, elem);
}


template<class T>
void List<T>::copy(const List<T>& other)
{
	try {
		List<T>::Iterator ptr = other.begin();
		while (ptr) {
			this->pushBack(*ptr);
			++ptr;
		}
	} catch (const std::bad_alloc&) {
		clear();
		throw;
	}
}


template<class T>
void List<T>::clear()
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
