/*
 * TurnOn.cpp
 *
 *      Author: karim/milind
 */
#include <iostream>
#include "TurnOn.h"

using namespace std;

TurnOn::TurnOn(xml_node<>* root){
	string r;
	string k;
	while(root != nullptr) {
		r = root->value();
		k = root->name();

		if(k == "action"){
			this->action = r;
		}
		else if (k == "print"){
			this->print = r;
		}
		root = root->next_sibling();
	}
}
