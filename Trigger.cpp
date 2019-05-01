/*
 * Trigger.cpp
 *
 *      Author: karim
 */

#include "Trigger.h"
#include <iostream>

using namespace std;

Trigger::Trigger(xml_node<>* root){
	this->type = "single";
	this->command = "";
	string r;
	string k;

	while(root != nullptr) {
		r = root->value();
		k = root->name();

		if(k == "command"){
			this->command = r;
		}
		else if (k == "action"){
			action.push_back(r);
		}
		else if (k == "print"){
			this->print = r;
		}
		else if (k == "type"){
			this->type = r;
		}
		else if (k == "condition"){
			Condition* condition = new Condition(root->first_node());
			conditions.push_back(condition);
		}
		root = root->next_sibling();
	}
}