#include <iostream>
#include <string>
#include "Student.h"
#include "CongaManager.h"

int main() {

	CongaManager sys;
	sys.append("pesho", "fmi", 0);
	sys.append("gesho", "unss", 0);
	sys.append("tosho", "fmi", 0);
	sys.append("mosho", "tu", 0);
	sys.print();
	sys.removeLast(0);
	sys.print();
	sys.removeFirst(0);
	sys.print();
	sys.removeFirst(0);
	sys.print();
	sys.removeLast(0);
	sys.print();

	return 0;
}