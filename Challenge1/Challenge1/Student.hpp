#pragma once
#include <string>

class Student {

public:

	Student() = delete;
	Student(const std::string& name, size_t group);
	Student(const Student&) = default;
	Student& operator=(const Student&) = default;
	~Student() = default;

	bool areFriends(const Student& other) const { return this->group == other.group; }
	const std::string& getName() const { return name; }
	void incrementTime() { ++timeSpent; }
	size_t getTime() const { return timeSpent; }

private:

	std::string name;
	size_t group;
	size_t timeSpent;

};



Student::Student(const std::string& name, size_t group) :
	name(name),
	group(group),
	timeSpent(0)
{}
