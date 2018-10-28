#ifndef __BIT_CONTAINER_INCLUDED__
#define __BIT_CONTAINER_INCLUDED__

class BitContainer {
private:
	class BitProxy {
	public:
		BitProxy(size_t* data, size_t index);
		BitProxy& operator=(const BitProxy& other);
		BitProxy& operator=(bool bit);
		operator const bool() const;

	private:
		size_t* data;
		size_t mask;
	};
	
public:
	BitContainer();
	BitContainer(const BitContainer& other);
	BitContainer& operator=(const BitContainer& other);
	~BitContainer();

	// Creating a container with an exact size
	BitContainer(size_t size);

	// Changing the capacity of the container
	BitContainer& setCapacity(size_t numOfBits);

	// Setting and Getting bits via the Proxy
	BitProxy operator[](size_t index);

private:
	size_t* data;
	size_t size;
	size_t bitCount;

	void copy(const BitContainer& other);
	void clear();

};

#endif // !__BIT_CONTAINER_INCLUDED__
