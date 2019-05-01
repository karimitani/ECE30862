/*
 * Game.cpp
 *
 *      Author: karim itani, Milind Shyam
 */
#include <sstream>
#include <map>
#include <fstream>
#include <iostream>
#include <vector>
#include <istream>
#include <string>
#include "Game.h"
#include "rapidxml.hpp"
using namespace std;
using namespace rapidxml;


bool Game::getsetp(string fname) {
	std::ifstream file(fname);

	if(!(file.is_open())){
		std::cout << "Cannot open file!" << std::endl;
		sstatus(false);
		return false;
 	}

	xml_document<> xmldoc;
	xml_node<> * nd;

	vector<char> buffer((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
	buffer.push_back('\0');

	xmldoc.parse<0>(&buffer[0]);

	nd = xmldoc.first_node();

	if (nd != nullptr){
		parsexml(nd->first_node());
	}
	else {
		sstatus(false);
	}

	return getstt();
}

bool Game::getstt() {
	return status;
}

void Game::parsexml(xml_node<> * nd){
	while(nd != nullptr){
		if(string((nd->name())) == "room") {
			Room* room = new Room(nd->first_node());
			rooms[room->name] = room;
		}
		else if(string((nd->name())) == "creature") {
			Creature* creature = new Creature(nd->first_node());
			creatures[creature->name] = creature;
		}
		else if(string((nd->name())) == "container") {
			Container* container = new Container(nd->first_node());
			containers[container->name] = container;
		}
		else if(string((nd->name())) == "item") {
			Item* item = new Item(nd->first_node());
			items[item->name] = item;
		}
		else 
			cout << "Error" << endl;
		nd = nd->next_sibling();
	}
}

void Game::exCom(string command, bool userInput) {
	stringstream strstr(command);
	istream_iterator<string> it(strstr);
	istream_iterator<string> end;
	vector<string> results(it, end);


	if ((command == "n") || command == "north")
		directionN("n");
	else if (command == "s" || command == "south")
		directionN("s");
	else if (command == "e" || command == "east")
		directionN("e");
	else if (command == "w" || command == "west")
		directionN("w");
	else if (command == "i")
		getInt();
	else if (command.find("read") != string::npos)
		readItem(command);
	else if(command.find("attack") != string::npos) 
		atkC(command);
	else if (command.find("turn on") != string::npos){
		turnOn(command);
	}
	else if (command.find("put") != string::npos){
		itemput(command);
	}
	else if (command.find("open") != string::npos){
		openOb(command);
	}
	else if (command.find("drop") != string::npos){
		droppedI(command);
	}
	else if (command.find("take") != string::npos){
		grab(command);
	}
	else if (command.find("open exit") != string::npos){
		if (exitT())
			return;
	}
	
	else if (!userInput && (command.find("Add") != string::npos)){
		addobj(command);
	}
	
	else if (!userInput && (command.find("Update") != string::npos)){
		updateObject(command);
	}
	else if (!userInput && (command.find("Delete") != string::npos)){
		objdel(command);
	}
	else if (!userInput && (command.find("Game Over") != string::npos)){
		std::cout << "Victory!" << std::endl;
		sstatus(false);

	}
	else {

		std::cout << command << "Error" << std::endl;
	}
}

void Game::sstatus(bool s){
	this->status = s;
}

void Game::theRoom(string location) {
	Room* rm = rooms[location];
	for(map<string,Container*>::iterator i = containers.begin(); i!= containers.end(); i++) {
		for(map<string, Item*>::iterator j = items.begin(); j!= items.end(); j++){
			for(map<string, Item*>::iterator k = i->second->items.begin(); k!= i->second->items.end(); k++){
				if(j->second->name == k->second->name){
					k->second = j->second;
				}
			}
		}
		for(map<string,Container*>::iterator k = rm->containers.begin(); k!= rm->containers.end(); k++) {
			if(i->second->name == k->second->name){
				k->second = i->second;
			}

			for(map<string, Item*>::iterator j = k->second->items.begin(); j!= k->second->items.end(); j++) {
				for(map<string, Item*>::iterator k = items.begin(); k!= items.end(); k++) {
					cout << "item: " << k->second->name;
					if(k->second->name == j->second->name){
						j->second = k->second;
					}
				}
			}
		}
	}
	
	for(map<string,Creature*>::iterator i = creatures.begin(); i!= creatures.end(); i++) {
		for(map<string,Creature*>::iterator k = rm->creatures.begin(); k!= rm->creatures.end(); k++) {
			if(i->second->name == k->second->name){
				k->second = i->second;
			}
		}
	}

	for(map<string, Item*>::iterator i = items.begin(); i != items.end(); i++) {
		for(map<string, Item*>::iterator k = rm->items.begin(); k!= rm->items.end(); k++) {
			if(i->second->name == k->second->name){
				k->second = i->second;
			}
		}
	}
}

void Game::runGame(string fname) {
	bool setup = getsetp(fname);
	if(setup) {
		theRoom("Entrance");
		
		this->location = "Entrance";
		cout << rooms[location]->description << endl;

		string command;
		while(this->status){
			bool trigfound = trigCheck(command);

			if (!trigfound){
				getline(cin, command);
				trigCheck(command);
				if(this->status){
					exCom(command, true);
				}
			}
		}
	}
	else{
		return;
	}
}

bool Game::executionT(vector<Trigger*> triggers, string command){
	Trigger* trig;
	bool res;

	for(int i = 0; i < triggers.size(); i++){
		trig = triggers[i];

		if(command == trig->command){

			if(trig->type != "done"){
				res = condI(trig->conditions);

				if(res){
					if(trig->print != ""){
						cout << trig->print << endl;
					}

					if(!(trig->action.empty())){
						int l = trig->action.size();
						for(int j = 0; j < l ; j++){
							exCom(trig->action[j], false);
						}
					}
					if(trig->type == "single"){
						trig->type = "done";
					}
				
					return res;
				}
			}
		}
		else if(trig->command == ""){
			if(trig->type != "done"){
				res = condI(trig->conditions);

				if(res){
					if(trig->print != ""){
						cout << trig->print << endl;
					}
					if(!(trig->action.empty())){
						int l = trig->action.size();
						for(int j = 0; j < l; j++){
							exCom(trig->action[j], false);
						}
					}
					if(trig->type == "single"){
						trig->type = "done";
					}
					return res;
				}
			}
		}
	}
	return false;
}

bool Game::exitT(){
	Room * current_room = rooms[this->location];
	if(current_room->type != "exit"){
		cout << "This is not an exit!" << endl;
		return false;
	}
	else {
		cout << "Game Over" << endl;
		sstatus(false);
		return true;
	}
}

void Game::directionN(string direction){
	map<string, Border*>::iterator it;
	Room* c_room = rooms[this->location];
	string d;

	if(direction == "n") {
		d = "north";
	}
	else if (direction == "s") {
		d = "south";
	}
	else if (direction == "w"){
		d = "west";
	}
	else if (direction == "e") {
		d = "east";
	}
	it = c_room->borders.find(d);

	if(it == c_room->borders.end())
		cout << "Can’t go that way." << endl;
	else {
		this->location = it->second->name;
		theRoom(location);
		cout << rooms[location]->description << endl;
	}
}

void Game::grab(string command){
	vector<string> com = splitCommand(command);
	if(com.size() <= 1) {
		cout << "To take an item, the command must be in the form of 'take (item)'." << endl;
		return;
	}
	string item = com[1];
	Room * c_room = rooms[location];
	Container * c_container;

	if(inventory.find(item) != inventory.end()){
		cout << "Error" << endl;
		return;
	}
	for(map<string,Container*>::iterator it = containers.begin(); it!= containers.end(); it++) {
		c_container = it->second;
		if(c_container->status == "open") {
			if(c_container->items.find(item) != c_container->items.end()) {
				inventory[item] = c_container->items[item];
				c_container->items.erase(item);
				cout << "Item " <<  item << " added to inventory." << endl;
				return;
			}
		}
	}
	if(c_room->items.find(item) != c_room->items.end()) {
		inventory[item] = c_room->items[item];
		c_room->items.erase(item);
		cout << "Item " <<  item << " added to inventory." <<endl;
		return;
	}
	for(map<string,Container*>::iterator it = c_room->containers.begin(); it!= c_room->containers.end(); it++) {
		c_container = it->second;
		if(c_container->status == "open") {
			if(c_container->items.find(item) != c_container->items.end()) {
				inventory[item] = c_container->items[item];
				c_container->items.erase(item);
				cout << "Item " <<  item << " added to inventory." << endl;
				return;
			}
		}
	}

	cout << "Error" << endl;
}

void Game::itemput(string command){
	vector<string> com = splitCommand(command);
	string itemFind = com[1];
	string container = com[3];
	Item* item;
	Room * c_room = rooms[location];

	if(com.size() <= 3) {
		cout << "Error: put command must be in the format of 'put (item) in (container).'" << endl;
		return;
	}
	if(inventory.find(itemFind) == inventory.end()){
		cout << "Error" << endl;
		return;
	}
	else{
		item = inventory[itemFind];
	}
	if(containers.find(container) != containers.end()){
		if(containers[container]->accepts.size() >= 1){
			for(int i = 0; i < containers[container]->accepts.size(); i ++){
				cout << "Item " <<itemFind << " added to " << container << "." << endl;
				containers[container]->items[itemFind] = item;
				inventory.erase(itemFind);
				trigCheck("");
				return;
			}
		}
		else if(containers[container]->status == "open"){
			cout << "Item " <<itemFind << " added to " << container << "." << endl;
			containers[container]->items[itemFind] = item;
			inventory.erase(itemFind);
			trigCheck("");
			return;
		}
		else{
			cout << "Error" << endl;
			return;
		}
	}
	else if(c_room->containers.find(container) != c_room->containers.end()){
		if(rooms[location]->containers[container]->accepts.size() >= 1){
			for(int i = 0; i < rooms[location]->containers[container]->accepts.size(); i ++){
				if(rooms[location]->containers[container]->accepts[i] == itemFind){
					cout << "Item " <<itemFind << " added to " << container << "." << endl;
					rooms[location]->containers[container]->items[itemFind] = item;
					inventory.erase(itemFind);
					trigCheck("");
					return;
				}
			}
		}
		else if(rooms[location]->containers[container]->status == "open"){
			cout << "Item " <<itemFind << " added to " << container << "." << endl;
			rooms[location]->containers[container]->items[itemFind] = item;
			inventory.erase(itemFind);
			trigCheck("");
			return;
		}
		else{
			cout << "Error" << endl;
			return;
		}
	}
	else {
		cout << "Error" << endl;
	}
}

void Game::addobj(string command) {
	vector<string> com = splitCommand(command);
	Room * roomL = nullptr;
	Container* containerL = nullptr;
	Item * itemO = nullptr;
	Container* containerO = nullptr;
	Creature* creatureO = nullptr;
	string loc = com[3];
	string object = com[1];

	if(inventory.find(object) != inventory.end()) 
		itemO = items[object];
	if(rooms.find(loc) != rooms.end())
		roomL = rooms[loc];
	if(items.find(object) != items.end())
		itemO = items[object];
	if(containers.find(loc) != containers.end())
		containerL = containers[loc];
	if(containers.find(object) != containers.end())
		containerO = containers[object];
	if(creatures.find(object) != creatures.end()) 
		creatureO = creatures[object];
	
	for (map<string, Container*>::iterator it = containers.begin(); it != containers.end(); it++){
		if (it->second->items.find(object) != it->second->items.end())
			itemO = it->second->items[object];
	}
	for (map<string, Room*>::iterator it = rooms.begin(); it != rooms.end(); it++)
	{
		if(it->second->creatures.find(object) != it->second->creatures.end())
			creatureO = it->second->creatures[object];
		
		if(it->second->items.find(object) != it->second->items.end())
			itemO = it->second->items[object];
		
		if(it->second->containers.find(loc) != it->second->containers.end())
			containerL = it->second->containers[loc];
		
		if(it->second->containers.find(object) != it->second->containers.end())
			containerO = it->second->containers[object];
		
		for (map<string, Container*>::iterator iter = it->second->containers.begin(); iter != it->second->containers.end(); iter++){
			if (iter->second->items.find(object) != iter->second->items.end())
				itemO = iter->second->items[object];
		}
	}
	if(roomL != nullptr){
		if(itemO != nullptr)
			roomL->items[itemO->name] = itemO;
		else if(containerO != nullptr)
			roomL->containers[containerO->name] = containerO;
		else if(creatureO != nullptr)
			roomL->creatures[creatureO->name] = creatureO;
		else
			cout << "Error: no " << object << " could be found " << endl;
	}

	else if(containerL != nullptr){
		if(itemO != nullptr)
			containerL->items[itemO->name] = itemO;
		else {
			cout << "Error: no " << object << " could be found" << endl;
		}
	}
	else {
		cout << "Error: location must be a room or container" << endl;
	}
}

char Game::hasow(Condition* con){
	if(con->owner == "")
		return 'n';

	Creature* creatureOw;
	Container* containerOw;
	Room* roomOw;

	string owner = con->owner;
	string object = con->object;

	if(owner == "inventory"){
		if(inventory.find(object) != inventory.end())
			return 't';
		else
			return 'f';
	}
	if(rooms.find(owner) != rooms.end())
		roomOw = rooms[owner];
	if(creatures.find(owner) != creatures.end())
		creatureOw = creatures[owner];
	if(containers.find(owner) != containers.end()){
		containerOw = containers[owner];
	}
	for (map<string, Room*>::iterator it = rooms.begin(); it != rooms.end(); it++)
	{
		if(it->second->containers.find(owner) != it->second->containers.end()){
			containerOw = it->second->containers[owner];
		}

		if(it->second->creatures.find(owner) != it->second->creatures.end())
			creatureOw = it->second->creatures[owner];
	}
	if(containerOw != nullptr){
		if(containerOw->items.find(object) != containerOw->items.end())
			return 't';
		else if(containerOw->status == object){
			return 't';
		}
		else{
			return 'f';
		}
	}
	else if(creatureOw != nullptr){
		if(creatureOw->status == object)
			return 't';
		else if(creatureOw->vulnerability.find(object) != creatureOw->vulnerability.end())
			return 't';
		else
			return 'f';
	}
	else if(roomOw != nullptr){
		if(roomOw->items.find(object) != roomOw->items.end())
			return 't';
		else if (roomOw->type == object)
			return 't';
		else if (roomOw->containers.find(object) != roomOw->containers.end())
			return 't';
		else if (roomOw->creatures.find(object) != roomOw->creatures.end())
			return 't';
		else
			return 'f';
	}
	return 'f';
}

void Game::turnOn(string command){
	vector<string> com = splitCommand(command);
	string item = com[2];

	if (com.size() <= 2) {
		cout << "Error! The turn on function command must have the following format: 'turn on (item)'" << endl;
	}

	if(inventory.find(item) == inventory.end()){
		cout << "Error" << endl;
		return;
	}

	if (inventory[item]->turnon == nullptr){
		cout << "Error" << endl;
		return;
	}

	cout << "You turned on the " << item << endl;
	cout << inventory[item]->turnon->print << endl;
	if (inventory[item]->turnon->action == ""){
		return;
	}
	exCom(inventory[item]->turnon->action, false);
}

void Game::getInt(){
	if(inventory.empty()){
		cout << "Inventory: empty" << std::endl;
	}
	else {
		cout << "Inventory: ";
		for (map<string, Item*>::iterator it = inventory.begin(); it != inventory.end(); it++)
		{
			if (it == inventory.begin()){
				cout << it->second->name;
			}
			else{
				cout << ", "<< it->second->name;
			}
		}
		cout << endl;
	}
}

void Game::atkC(string command){
	vector<string> com = splitCommand(command);
	string creature = com[1];
	string item = com[3];

	if(inventory.find(item) == inventory.end()){
		cout << "Error" << endl;
		return;
	}

	if(com.size() <= 3){
		cout << "Error! Attack command should be in the format: 'attack (creature) with (item)'" << endl;
		return;
	}

	if(creatures.find(creature) != creatures.end()){
		if((creatures[creature]->vulnerability.find(item) != creatures[creature]->vulnerability.end()) ||  (creatures[creature]->vulnerability.size() == 0)){
			cout << "You assault " << creature << " with " << item << "." << endl;

			if(creatures[creature]->attack != nullptr){
				bool result = condI(creatures[creature]->attack->conditions);

				if (result) {
					cout << creatures[creature]->attack->print << endl;

					for (int i = 0;  i < creatures[creature]->attack->actions.size(); i++){
						if((creatures[creature]->attack->actions[i].find("Delete") != string::npos) && (creatures[creature]->attack->actions[i].find(creatures[creature]->name) != string::npos)){
							exCom(creatures[creature]->attack->actions[i], false);
							break;
						}
						else {
							exCom(creatures[creature]->attack->actions[i], false);
						}
					}
					trigCheck("");
					return;
				}
				else{
					cout << "Error" << endl;
				}
			}
		}
		else{
			cout << "Error" << endl;
		}
	}
	else{
		cout << "Error" << endl;
	}
}

void Game::readItem(string command) {
	vector<string> com = splitCommand(command);
	string item = com[1];

	if(com.size() <= 1) {
		cout << "Error! The read command should have the following format: 'read (item)'" << endl;
		return;
	}

	if(inventory.find(item) != inventory.end()) {
		if(inventory[item]->writing != ""){
			cout << inventory[item]->writing << endl;
		}
		else{
			cout << "Nothing written" << endl;
		}
	}
	else {
		cout << "Error" << endl;
	}
}

void Game::openOb(string command){
	vector<string> com = splitCommand(command);
	Room * c_room = rooms[location];
	if(com.size() <= 1) {
		cout << "To open a container, the command must be in the form of 'open (container)'." << endl;
		return;
	}

	string cont = com[1];

	if(containers.find(cont) != containers.end()){
		if(containers[cont]->status == "locked"){
			cout << "Error" << endl;
			return;
		}
		containers[cont]->status = "open";
		if(containers[cont]->items.size() <= 0){
			cout << "Error" << endl;
			return;
		}
		cout << cont << " contains: ";
		for (map<string, Item*>::iterator it = containers[cont]->items.begin(); it != containers[cont]->items.end(); it++) {
			if (it == containers[cont]->items.begin()){
				cout << it->second->name;
			}
			else{
				cout << ", "<< it->second->name;
			}
		}
		cout << endl;
	}
	else if (c_room->containers.find(cont) != c_room->containers.end()){
		if (c_room->containers[cont]->status == "locked"){
			cout << "Error" << endl;
		}
		else {
			c_room->containers[cont]->status = "open";
			cout << cont << " contains: ";
			for (map<string, Item*>::iterator it = c_room->containers[cont]->items.begin(); it != c_room->containers[cont]->items.end(); it++)
			{
				if (it == c_room->containers[cont]->items.begin()){
					cout << it->second->name;
				}
				else{
					cout << ", "<< it->second->name;
				}
			}
			cout << endl;
		}
	}
	else {
		cout << "Error" << endl;
	}
}

void Game::objdel(string command){
	vector<string> com = splitCommand(command);
	string object = com[1];

	if(rooms.find(object) != rooms.end()){
		rooms.erase(object);
		return;
	}
	if(creatures.find(object) != creatures.end()) {
		creatures.erase(object);
		return;
	}
	if(containers.find(object) != containers.end()){
		containers.erase(object);
		return;
	}
	if(items.find(object) != items.end()){
		items.erase(object);
		return;
	}
	if(inventory.find(object) != inventory.end()) {
		inventory.erase(object);
		return;
	}
	for (map<string, Container*>::iterator it = containers.begin(); it != containers.end(); it++){
		if (it->second->items.find(object) != it->second->items.end()){
			it->second->items.erase(object);
			return;
		}
	}
	for (map<string, Room*>::iterator it = rooms.begin(); it != rooms.end(); it++)
	{
		if(it->second->items.find(object) != it->second->items.end()){
			it->second->items.erase(object);
			return;
		}
		if(it->second->creatures.find(object) != it->second->creatures.end()){
			it->second->creatures.erase(object);
			return;
		}
		if(it->second->containers.find(object) != it->second->containers.end()){
			it->second->containers.erase(object);
			return;
		}
		for (map<string, Container*>::iterator iter = it->second->containers.begin(); iter != it->second->containers.end(); iter++){
			if (iter->second->items.find(object) != iter->second->items.end()){
				iter->second->items.erase(object);
				return;
			}
		}
	}
	cout << "Error" << endl;
}

void Game::droppedI(string command){
	vector<string> com = splitCommand(command);
	Room* c_room = rooms[location];
	string item = com[1];
	if (com.size() <= 1) {
		cout << "Error! The drop function command must have the following format: 'drop (item)'" << endl;
		return;
	}
	if (inventory.find(item) != inventory.end()){
		c_room->items[item] = inventory[item];
		inventory.erase(item);
		cout << item << " dropped" << endl;
	}
	else {
		cout << "Error" << endl;
	}
}

bool Game::checking(Condition* con){
	if(con->has == "yes" || con->has == "")
		return true;
	else
		return false;
}

void Game::updateObject(string command){
	vector<string> com = splitCommand(command);

	string object = com[1];
	string stat = com[3];

	if(rooms.find(object) != rooms.end()){
		rooms[object]->status = stat;
		trigCheck("");
		return;
	}
	if(creatures.find(object) != creatures.end()) {
		creatures[object]->status = stat;
		trigCheck("");
		return;
	}
	if(containers.find(object) != containers.end()){
		containers[object]->status = stat;
		trigCheck("");
		return;
	}
	if(items.find(object) != items.end()){
		items[object]->status = stat;
		trigCheck("");
		return;
	}
	if(inventory.find(object) != inventory.end()) {
		inventory[object]->status = stat;
		trigCheck("");
		return;
	}
	for (map<string, Container*>::iterator it = containers.begin(); it != containers.end(); it++){
		if (it->second->items.find(object) != it->second->items.end()){
			it->second->items[object]->status = stat;
			trigCheck("");
			return;
		}
	}
	for (map<string, Room*>::iterator it = rooms.begin(); it != rooms.end(); it++)
	{
		if(it->second->containers.find(object) != it->second->containers.end()){
			it->second->containers[object]->status = stat;
			trigCheck("");
			return;
		}
		if(it->second->creatures.find(object) != it->second->creatures.end()){
			it->second->creatures[object]->status = stat;
			trigCheck("");
			return;
		}
		if(it->second->items.find(object) != it->second->items.end()){
			it->second->items[object]->status = stat;
			trigCheck("");
			return;
		}
		for (map<string, Container*>::iterator iter = it->second->containers.begin(); iter != it->second->containers.end(); iter++){
			if (iter->second->items.find(object) != iter->second->items.end()){
				iter->second->items[object]->status = stat;
				trigCheck("");
				return;
			}
		}
	}
	cout << "Error" << endl;
}

vector<string> Game::splitCommand(string command){
	stringstream ss(command);
	string buffer;
	vector<string> words;
	while (ss >> buffer)
		words.push_back(buffer);
	return words;
}

bool Game::trigCheck(string command ){
	bool res = false;
	Room * rm = rooms[location];

	if(!(rm->triggers.empty())){
		res = executionT(rm->triggers, command);
		if(res)
			return res;
	}

	for (map<string, Creature*>::iterator i = rm->creatures.begin(); i != rm->creatures.end(); i++){
		res = executionT(i->second->triggers, command);
		if(res)
			return res;
	}

	for (map<string, Container*>::iterator i = containers.begin(); i != containers.end(); i++){
		if(!(i->second->triggers.empty()))
				res = executionT(i->second->triggers, command);
		if(res)
			return res;
	}

	for (map<string, Item*>::iterator i = items.begin(); i != items.end(); i++){
		if(!(i->second->triggers.empty()))
			res = executionT(i->second->triggers, command);
		if(res)
			return res;
	}

	for (map<string, Creature*>::iterator i = creatures.begin(); i != creatures.end(); i++){
		if(!(i->second->triggers.empty()))
				res = executionT(i->second->triggers, command);
		if(res)
			return res;
	}

	for (map<string, Container*>::iterator i = rm->containers.begin(); i != rm->containers.end(); i++){
		res = executionT(i->second->triggers, command);
		if(res)
			return res;
	}

	for (map<string, Item*>::iterator i = rm->items.begin(); i != rm->items.end(); i++){
		res = executionT(i->second->triggers, command);
		if(res)
			return res;
	}
	return false;
}

char Game::gotIt(Condition* cond) {
	if(cond->status == ""){
		return 'n';
	}

	string stat = cond->status;
	string object = cond->object;

	if(creatures.find(object) != creatures.end()) {
		if(creatures[object]->status != stat)
			return 'f';
		else
			return 't';
	}
	if(items.find(object) != items.end()){
		if(items[object]->status != stat)
			return 'f';
		else
			return 't';
	}
	if(rooms.find(object) != rooms.end()){
		if(rooms[object]->status != stat)
			return 'f';
		else
			return 't';
	}
	if(inventory.find(object) != inventory.end()) {
		if(inventory[object]->status != stat){
			return 'f';
		}
		else
			return 't';
	}
	if(containers.find(object) != containers.end()){
		if(containers[object]->status != stat)
			return 'f';
		else
			return 't';
	}
	for (map<string, Container*>::iterator it = containers.begin(); it != containers.end(); it++){
		if (it->second->items.find(object) != it->second->items.end()){
			if(it->second->items[object]->status != stat)
				return 'f';
			else
				return 't';
		}
	}
	for (map<string, Room*>::iterator it = rooms.begin(); it != rooms.end(); it++)
	{
		if(it->second->items.find(object) != it->second->items.end()){
			if(it->second->items[object]->status != stat)
				return 'f';
			else
				return 't';
		}
		if(it->second->containers.find(object) != it->second->containers.end()){
			if(it->second->containers[object]->status != stat){
				return 'f';
			}
			else{
				return 't';
			}
		}
		if(it->second->creatures.find(object) != it->second->creatures.end()){
			if(it->second->creatures[object]->status != stat){
				return 'f';
			}
			else{
				return 't';
			}
		}
		for (map<string, Container*>::iterator iter = it->second->containers.begin(); iter != it->second->containers.end(); iter++){
			if (iter->second->items.find(object) != iter->second->items.end()){
				if(iter->second->items[object]->status != stat)
					return 'f';
				else
					return 't';
			}
		}
	}
	cout << "Error: object not found." << endl;
	return 'f';
}

bool Game::condI(vector<Condition*> conditions){
	bool res = true;
	Condition* condition;

	if(conditions.empty()){
		return res;
	}

	for (int i = 0; i < conditions.size(); i++){
			condition = conditions[i];
			char hasS = gotIt(condition);
			char hasO = hasow(condition);
			bool hasH = checking(condition);
			if(hasH){
				if(!((hasO == 't' || hasO == 'n') && (hasS == 't' || hasS == 'n'))){
					res = false;
				}
			}
			else {
				if(hasO == 't' || hasS == 't')
					res = false;
			}
	}
	return res;
}