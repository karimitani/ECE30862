/*
 * Item.cpp
 *
 */

#include <iostream>
#include <vector>
#include "Item.h"
using namespace std;

Item::Item(xml_node<>* root){
	this->status = "idle";
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
		else if(k == "turnon"){
			this->turnon = new TurnOn(root->first_node());
		}
		else if (k == "writing") {
			this->writing = r;
		}
		else if (k == "trigger"){
			Trigger* trig = new Trigger(root->first_node());
			triggers.push_back(trig);
		}
		else if (k == "status"){
			this->status = r;
		}
		else if (k == "item"){
			this->name = r;
			return;
		}
		root = root->next_sibling();
	}
}


