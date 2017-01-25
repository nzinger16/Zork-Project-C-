#ifndef _ROOM_H
#define _ROOM_H

#include<iostream>
#include<sstream>
#include<fstream>
#include<cstdlib>
#include<vector>
#include<string>
#include<list>
#include "rapidxml-1.13/rapidxml.hpp"
#include "rapidxml-1.13/rapidxml_utils.hpp"
#include "rapidxml-1.13/rapidxml_print.hpp"
#include "Trigger.h"
using namespace std;
using namespace rapidxml;

typedef struct _border{
  string direction;
  string name;
} B;



class Room {
public:
	string name;
	string status;
	string type;
	string description;
	vector<B *> border;
	vector<string> container;
	vector<string> item;
	vector<string> creature;
	vector<Trigger*> trigger;
	Room(xml_node<>* node);
	//virtual ~Room();

};

#endif
