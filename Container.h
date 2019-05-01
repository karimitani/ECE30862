/*
 * Container.h
 *
 *      Author: karimitani
 */
#include <string>
#include <map>
#include <vector>
#include "Item.h"
#include "Trigger.h"
#include "rapidxml.hpp"

using namespace rapidxml;
using namespace std;

#ifndef CONTAINER_H_
#define CONTAINER_H_

class Container{
public:
	string status;
	string name;
	vector<string> accepts;
	string description;
	vector<Trigger*> triggers;
	map<string, Item*> items;

	Container(xml_node<>*);
	virtual ~Container() {}
};



#endif /* CONTAINER_H_ */
