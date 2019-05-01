/*
 * Item.h
 *
 *      Author: karim/milind
 */

#include <string>
#include "TurnOn.h"
#include "Trigger.h"
#include "rapidxml.hpp"

using namespace rapidxml;
using namespace std;

#ifndef ITEM_H_
#define ITEM_H_

class Item{
public:
	string description;
	string writing;
	string name;
	string status;
	TurnOn* turnon;
	vector<Trigger*> triggers;
	Item(xml_node<>* node);
	virtual ~Item() {}
};

#endif /* ITEM_H_ */
