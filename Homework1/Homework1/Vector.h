#pragma once

#include "List.h"

class Vector {

public:

	Vector();
	Vector(const Vector&) = delete;				// Copy ctor and operator=
	Vector operator=(const Vector&) = delete;	// aren't really used
	~Vector();

public:

	List* operator[](size_t index);
	const List* operator[](size_t index) const;
	void push_back_empty_list();
	size_t size() const { return length; }
	List* back() { return data[length - 1]; }
	void removeAt(size_t index);

private:

	void resize();

private:
	
	List** data;
	size_t length;
	size_t capacity;

};
