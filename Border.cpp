/*
 * Border.cpp
 *
 *      Author: karim/milind
 */
#include <iostream>
#include "Border.h"

Border::Border(xml_node<>* root){
	string r;
	string k;
	while(root != nullptr) {
		r = root->value();
		k = root->name();

		if(k == "name"){
			this->name = r;
		}
		else if (k == "direction"){
			this->direction = r;
		}
		root = root->next_sibling();
	}
}