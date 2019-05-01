/*
 * Container.cpp
 *
 *      Author: karim/milind
 */

#include "Container.h"
#include <iostream>

using namespace std;

Container::Container(xml_node<>* root){
	this->status = "closed";
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
		else if (k == "accept"){
			this->accepts.push_back(r);
		}
		else if (k == "trigger"){
			triggers.push_back(new Trigger(root->first_node()));

		}
		else if (k == "item" && r != ""){
			Item* item = new Item(root);
			this->items[item->name] = item;
		}
		else if (k == "item" && r == ""){
			Item* item = new Item(root->first_node());
			this->items[item->name] = item;
		}
		else if (k == "status"){
			this->status = r;
		}
		else if(k == "container"){
			this->name = k;
			return;
		}
		root = root->next_sibling();
	}
}