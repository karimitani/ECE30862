/*
 * Condition.h
 *
 *      Author: karim/milind
 */
#include <string>
#include "rapidxml.hpp"

using namespace rapidxml;
using namespace std;

#ifndef CONDITION_H_
#define CONDITION_H_

class Condition{
public:
	string has;
	string owner;
	string object;
	string status;

	Condition(xml_node<>*);
	virtual ~Condition() {}
};

#endif 