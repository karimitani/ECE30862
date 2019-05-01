/*
 * Border.h
 *      Author: karim/milind
 */
#include <string>
#include "rapidxml.hpp"
using namespace rapidxml;
using namespace std;

#ifndef BORDER_H_
#define BORDER_H_

class Border{
public:
	string direction;
	string name;
	Border(xml_node<>*);
	virtual ~Border() {}
};
#endif