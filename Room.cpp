/*
 * Room.cpp
 *
 *      Author: karimm/milind
 */

#include <iostream>
#include "Container.h"
#include "Creature.h"
#include "Trigger.h"
#include "Border.h"
#include "Room.h"
#include <map>
using namespace std;

Room::Room(xml_node<> * root){
	Border* border;
	this->type = "regular";
	string k;
	string r;

	while(root != nullptr) {
		r = root->value();
		k = root->name();

		if(k == "name"){
			this->name = r;
		}
		else if (k == "status"){
			this->status = r;
		}
		else if(k == "creature"){
			Creature* creature = new Creature(root);
			this->creatures[creature->name] = creature;
		}
		else if (k == "description"){
			this->description = r;
		}
		else if (k == "type"){
			this->type = r;
		}
		else if (k == "border"){
			border = new Border(root->first_node());
			this->borders[border->direction] = border;
		}
		else if (k == "creature" && r == ""){
			Creature* creature = new Creature(root->first_node());
			this->creatures[creature->name] = creature;
		}
		else if (k == "trigger"){
			Trigger* trigger = new Trigger(root->first_node());
			triggers.push_back(trigger);
		}
		else if (k == "item"){
			Item* item = new Item(root);
			this->items[item->name] = item;
		}
		else if(k == "container" && r != ""){
			Container* container = new Container(root);
			this->containers[container->name] = container;
		}
		else if (k == "container" && r == "") {
			Container* container = new Container(root->first_node());
			this->containers[container->name] = container;
		}
		root = root->next_sibling();
	}
}
