/*
 * TurnOn.h
 */
#include <string>
#include "rapidxml.hpp"

using namespace std;
using namespace rapidxml;

#ifndef TURNON_H_
#define TURNON_H_

class TurnOn {
public:
	string action;
	string print;

	TurnOn(xml_node<>* root);
	virtual ~TurnOn() {}
};

#endif /* TURNON_H_ */