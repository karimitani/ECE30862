/*
 * Room.h
 *
 */
#include <map>
#include <string>
#include "Trigger.h"
#include "Container.h"
#include "Item.h"
#include "Creature.h"
#include "Border.h"
#include "rapidxml.hpp"

using namespace std;
using namespace rapidxml;

#ifndef ROOM_H_
#define ROOM_H_

class Room{
public:
	string status;
	string type;
	string name;
	string description;
	map<string, Border*> borders;
	map<string, Container*> containers;
	vector<Trigger*> triggers;
	map<string, Item*> items;
	map<string, Creature*> creatures;
	Room (xml_node<char>* name);
	virtual ~Room() {}

};

#endif /* ROOM_H_ */
