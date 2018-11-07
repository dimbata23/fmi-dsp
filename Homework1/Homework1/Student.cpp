#include "Student.h"


const bool TABLE_OF_TOLERANCE[][3] = {
//   FMI    TU     UNSS
	true,  true,  false,  // FMI
	false, true,  true,   // TU
	true,  false, true    // UNSS
};





Student::Student(const std::string& name, const std::string& uni) :
	name(name),
	uni(convertToUni(uni))
{}


bool Student::canBeToleratedBy(const std::string& uni) const
{
	return TABLE_OF_TOLERANCE[convertToUni(uni)][this->uni];
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


University Student::convertToUni(const std::string& uni) const
{
	for (int i = 0; i < UNI_COUNT; ++i)
		if (!uni.compare(UNI_NAMES[i]))
			return University(i);
	return University(-1);
}


std::ostream& operator<<(std::ostream& out, const Student& st)
{
	return out << '(' << st.getName() << ", " << st.getUni() << ')';
}
