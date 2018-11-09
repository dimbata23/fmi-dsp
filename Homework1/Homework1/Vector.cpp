#include "Vector.h"


const size_t DEFAULT_CAPACITY = 4;


Vector::Vector() :
	data(new List*[DEFAULT_CAPACITY]),
	length(0),
	capacity(DEFAULT_CAPACITY)
{}


Vector::~Vector()
{
	for (size_t i = 0; i < length; ++i)
		delete data[i];
	delete[] data;
}


List* Vector::operator[](size_t index)
{
	return data[index];
}


const List* Vector::operator[](size_t index) const
{
	return data[index];
}


void Vector::push_back_empty_list()
{
	if (length >= capacity)
		resize();

	data[length] = new List;

	++length;
}


void Vector::removeAt(size_t index)
{
	delete data[index];
	if (index < length - 1)
		data[index] = data[length - 1];
	--length;
}


void Vector::resize()
{
	List** temp = new List*[length * 2];
	for (size_t i = 0; i < length; ++i)
		temp[i] = data[i];

	delete[] data;
	data = temp;
}
