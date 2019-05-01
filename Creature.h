/*
 * Creature.h
 * *      Author: kariitani
 */
#include <string>
#include <vector>
#include "Attack.h"
#include "Trigger.h"
#include "rapidxml.hpp"

using namespace rapidxml;
using namespace std;

#ifndef CREATURE_H_
#define CREATURE_H_

class Creature {
public:
	string description;
	string name;
	string status;
	Attack* attack;
	map<string, string> vulnerability;
	vector<Trigger*> triggers;
	Creature(xml_node<>*);
	virtual ~Creature() {}
};

#endif /* CREATURE_H_ */