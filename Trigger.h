/*
 * Trigger.h
 * *      Author: karim/milind
 */
#include <string>
#include <vector>
#include <map>
#include "Condition.h"
#include "rapidxml.hpp"

using namespace std;
using namespace rapidxml;


#ifndef TRIGGER_H_
#define TRIGGER_H_

class Trigger {
public:
	vector<Condition*> conditions;
	string print;
	string command;
	string type;
	vector<string> action;
	Trigger(xml_node<char>* name);
	virtual ~Trigger() {}
};

#endif
