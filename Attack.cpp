/*
 * Attack.cpp
 *
 *      Author: karim itani
 */
#include "Attack.h"
#include <iostream>

using namespace std;

Attack::Attack(xml_node<>* root){
	string k;
	string r;
	while (root != nullptr){
		k = root->name();
		r = root->value();

		if(k == "print"){
			this->print = r;
		}
		else if (k == "action"){
			this->actions.push_back(r);
		}
		else if(k == "condition"){
			conditions.push_back(new Condition(root->first_node()));
		}
		root = root->next_sibling();
	}
}