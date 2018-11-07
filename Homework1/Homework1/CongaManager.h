#pragma once

#include <vector>
#include "List.h"
#include "Student.h"

class CongaManager {
public:

	CongaManager();
	CongaManager(const CongaManager&) = delete;
	CongaManager& operator=(const CongaManager&) = delete;
	~CongaManager();

	void append(const std::string& name, const std::string& uni, int index);
	void removeFirst(int index);
	void removeLast(int index);
	void remove(const std::string& name, int index);
	void print() const;

private:

	void removePerson(int index, bool first);
	void removeLine(int index);

private:

	std::vector<List> lines;
	int size;

};
