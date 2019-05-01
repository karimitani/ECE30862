/*
 * main.cpp
 *
 *      Author: karimitani
 */
#include <fstream>
#include <iostream>
#include "Game.h"
#include "rapidxml.hpp"

using namespace std;
using namespace rapidxml;

int main(int argc, char ** argv) {
	if (argc < 2) {
		std::cout << "Error expected input format: Zork filename.xml" << std::endl;
		return EXIT_FAILURE;
	}
	Game* r = new Game(1);
	string file0 = argv[1];
	r->runGame(file0);
	return EXIT_SUCCESS;
}



