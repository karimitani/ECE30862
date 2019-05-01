/*
 * Condition.cpp
 *
 *      Author: karim/milind
 */
#include "Condition.h"
#include <iostream>

using namespace std;

Condition::Condition(xml_node<>* root){
	string r;
	string k;
	this->owner = "";
	this->status = "";
	while(root != nullptr) {
		k = root->name();
		r = root->value();

		if(k == "owner"){
			this->owner = r;
		}
		else if(k == "object"){
			this->object = r;
		}
		else if(k == "has"){
			this->has = r;
		}
		else if (k == "status"){
			this->status = r;
		}
		root = root->next_sibling();
	}
}