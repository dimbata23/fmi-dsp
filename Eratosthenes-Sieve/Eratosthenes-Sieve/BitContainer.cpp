#include <iostream>
#include "BitContainer.h"


const size_t BITS_IN_SIZE_T = sizeof(size_t) << 3;


BitContainer::BitContainer() : data(nullptr), size(0), bitCount(0) {}


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


BitContainer::BitContainer(size_t size) : data(nullptr), size(0), bitCount(0)
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


BitContainer& BitContainer::setBit(size_t index, bool bit)
{
	if (index >= bitCount)
		throw std::out_of_range("[BitContainer]: Accessing index out of bounds!");

	size_t element = (index + 1) / BITS_IN_SIZE_T;
	index %= BITS_IN_SIZE_T;
	size_t mask = 1 << (BITS_IN_SIZE_T - index);

	if (!bit) {
		mask = ~mask;
		this->data[element] &= mask;
	} else {
		this->data[element] |= mask;
	}

	return *this;
}


const bool BitContainer::operator[](size_t index) const
{
	if (index >= bitCount)
		throw std::out_of_range("[BitContainer]: Accessing index out of bounds!");

	size_t element = (index + 1) / BITS_IN_SIZE_T;
	index %= BITS_IN_SIZE_T;
	size_t mask = 1 << (BITS_IN_SIZE_T - index);
	return this->data[element] & mask;
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
