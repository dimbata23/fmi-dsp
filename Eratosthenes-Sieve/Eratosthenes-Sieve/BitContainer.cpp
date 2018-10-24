#include <iostream>
#include "BitContainer.h"


const size_t BITS_IN_SIZE_T = sizeof(size_t) << 3;


BitContainer::BitContainer() : 
	data(nullptr), 
	size(0), 
	bitCount(0) 
{}


BitContainer::BitContainer(const BitContainer& other)
{
	this->copy(other);
}


BitContainer& BitContainer::operator=(const BitContainer& other)
{
	this->clear();
	this->copy(other);
	return *this;
}


BitContainer::~BitContainer()
{
	this->clear();
}


BitContainer::BitContainer(size_t size) : 
	data(nullptr), 
	size(0), 
	bitCount(0)
{
	this->setCapacity(size);
}


BitContainer& BitContainer::setCapacity(size_t numOfBits)
{
	this->clear();
	this->bitCount = numOfBits;
	this->size = this->bitCount / BITS_IN_SIZE_T + 1;
	this->data = new size_t[this->size];
	for (size_t i = 0; i < this->size; ++i)
		this->data[i] = 0;
	return *this;
}


BitContainer::BitProxy BitContainer::operator[](size_t index)
{
	if (index < bitCount)
		return BitProxy(&this->data[(index + 1) / BITS_IN_SIZE_T], index % BITS_IN_SIZE_T);
	throw std::out_of_range("[BitContainer]: Accessing bit out of range!");
}


void BitContainer::copy(const BitContainer& other)
{
	this->size = other.size;
	this->bitCount = other.bitCount;
	this->data = new size_t[this->size];
	for (size_t i = 0; i < this->size; ++i)
		this->data[i] = other.data[i];
}


void BitContainer::clear()
{
	delete[] this->data;
	this->data = nullptr;
	this->size = 0;
}


BitContainer::BitProxy::BitProxy(size_t* data, size_t index) : 
	data(data), 
	mask(1 << (BITS_IN_SIZE_T - index))
{}


BitContainer::BitProxy& BitContainer::BitProxy::operator=(bool bit)
{
	if (!bit)
		*data &= ~mask;
	else
		*data |= mask;

	return *this;
}


BitContainer::BitProxy::operator const bool() const
{
	return (mask & *data);
}
