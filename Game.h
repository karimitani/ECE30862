/*
 * Game.h
 *      Author: karimitani/milind
 */
#include <string>
#include <vector>
#include <map>
#include "Room.h"
#include "Item.h"
#include "Container.h"
#include "Creature.h"
#include "rapidxml.hpp"

using namespace rapidxml;
using namespace std;

#ifndef GAME_H_
#define GAME_H_

class Game {
private:
	bool status;
	string location;
	map<string, Room*> rooms;
	map<string, Container*> containers;
	map<string, Item*> items;
	map<string, Item*> inventory;
	map<string, Creature*> creatures;

public:
	Game(int i) : status(true) {}
	virtual ~Game() {}
	void addobj(string);
	bool condI(vector<Condition *>);
	bool checking(Condition*);
	void itemput(string);
	void updateObject(string);
	void parsexml(xml_node<> *);
	bool getsetp(string);
	bool getstt();
	void sstatus(bool);
	void droppedI(string);
	void grab(string);
	void objdel(string);
	void runGame(string);
	bool trigCheck(string);
	void turnOn(string);
	void atkC(string);
	void readItem(string);
	bool executionT(vector<Trigger*>, string);
	void exCom(string, bool);
	char hasow(Condition*);
	void openOb(string);
	void getInt(void);
	void directionN(string);
	bool exitT();
	vector<string> splitCommand(string);
	char gotIt(Condition*);
	void theRoom(string);
};

#endif /* GAME_H_ */