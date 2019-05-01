/*
 * Creature.cpp
 *
 *      Author: karim/milind
 */

#include "Creature.h"
#include <iostream>

using namespace std;

Creature::Creature(xml_node<>* root){
	string r;
	string k;

	while(root != nullptr) {
		r = root->value();
		k = root->name();

		if(k == "name"){
			this->name = r;
		}
		else if (k == "description"){
			this->description = r;
		}
		else if (k == "status"){
			this->status = r;
		}
		else if (k == "attack"){
			this->attack = new Attack(root->first_node());
		}
		else if (k == "vulnerability"){
			this->vulnerability[r] = r;
		}
		else if(k == "creature"){
			this->name = r;
			return;
		}
		else if (k == "trigger"){
			triggers.push_back(new Trigger(root->first_node()));
		}
		root = root->next_sibling();
	}
}


