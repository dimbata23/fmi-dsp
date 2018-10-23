#ifndef __BIT_CONTAINER_INCLUDED__
#define __BIT_CONTAINER_INCLUDED__

class BitContainer {

public:
	BitContainer();
	BitContainer(const BitContainer& other);
	BitContainer& operator=(const BitContainer& other);
	~BitContainer();

	// Creating a container with an exact size
	BitContainer(size_t size);

	// Changing the capacity of the container
	BitContainer& setCapacity(size_t numOfBits);

	// Setting a bit
	BitContainer& setBit(size_t index, bool bit);

	// Reading from the Container
	const bool operator[](size_t index) const;

private:
	size_t* data;
	size_t size;
	size_t bitCount;


	void copy(const BitContainer& other);
	void clear();

};

#endif // !__BIT_CONTAINER_INCLUDED__
