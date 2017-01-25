#ifndef _ATTACK_H
#define _ATTACK_H

#include "rapidxml-1.13/rapidxml.hpp"
#include "rapidxml-1.13/rapidxml_utils.hpp"
#include "rapidxml-1.13/rapidxml_print.hpp"
#include<iostream>
#include<sstream>
#include<fstream>
#include<cstdlib>
#include<vector>
#include<string>
#include<list>
#include "Trigger.h"

using namespace std;
using namespace rapidxml;


typedef struct _turnOn{
  string print,action;
} t_on;

class Item {
public:
	string name, writing, description, status;
	t_on turnon;
	bool has_turnon;
	vector <Trigger*> trigger;
	Item(xml_node<>* node);
	//virtual ~Item();
};

#endif
