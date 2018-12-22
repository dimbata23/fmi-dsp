#pragma once

#include <iostream>
#include <string>

const std::string UNI_NAMES[] = {"fmi", "tu", "unss"};

enum University {
	UNI_NOT_SET = -1,
	UNI_FMI,
	UNI_TU,
	UNI_UNSS,
	UNI_COUNT
};



class Student {

public:

	Student() : name(""), uni(UNI_NOT_SET) {}
	Student(const std::string& name, const std::string& uni = "");
	Student(const Student& other) = default;
	Student& operator=(const Student& other) = default;
	~Student() = default;

	bool canBeToleratedBy(const std::string& uni) const;
	void setName(const std::string& name);
	void setUni(const std::string& uni);
	std::string getName() const;
	std::string getUni() const;

private:

	University convertToUni(const std::string& uni) const;

private:

	std::string name;
	University uni;

};

std::ostream& operator<<(std::ostream& out, const Student& st);
