/*
 * Attack.h
 * *      Author: karim
 */
#include <vector>
#include <map>
#include "Condition.h"
#include "rapidxml.hpp"

using namespace rapidxml;
using namespace std;

#ifndef ATTACK_H_
#define ATTACK_H_

class Attack {
public:
	vector<string> actions;
	vector<Condition*> conditions;
	string print;

	Attack(xml_node<>*);
	virtual ~Attack() {}
};

#endif