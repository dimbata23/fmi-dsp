#include "Student.h"


const bool TABLE_OF_TOLERANCE[][3] = {
//   FMI    TU     UNSS
	true,  true,  false,  // FMI
	false, true,  true,   // TU
	true,  false, true    // UNSS
};





Student::Student(const std::string& name, const std::string& uni) :
	name(name)
{
	for (int i = 0; i < UNI_COUNT; ++i) {
		if (!uni.compare(UNI_NAMES[i])) {
			this->uni = University(i);
			break;
		}
	}
}


inline bool Student::canTolerate(const University& uni)
{
	return TABLE_OF_TOLERANCE[uni, this->uni];
}


void Student::setName(const std::string& name)
{
	this->name = name;
}


void Student::setUni(const std::string& uni)
{
	for (int i = 0; i < UNI_COUNT; ++i) {
		if (!uni.compare(UNI_NAMES[i])) {
			this->uni = University(i);
			break;
		}
	}
}


std::string Student::getName() const
{
	return name;
}


std::string Student::getUni() const
{
	return (uni!=-1) ? UNI_NAMES[uni] : "unknown";
}


std::ostream& operator<<(std::ostream& out, const Student& st)
{
	return out << '(' << st.getName() << ", " << st.getUni() << ')';
}
