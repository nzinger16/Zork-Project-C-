#ifndef _CREATURE_H
#define _CREATURE_H

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

typedef struct _cond{
  string object;
  string status;
} Condition;

class Creature {
public:
	string name, description, status, print;
	vector <string> v, action; //v = vulnerability
	vector <Trigger*> trigger;
	bool has_cond, has_action, has_print, has_attack, has_trig;
	Condition C;

	//vector <Attack*> attack;
	Creature(xml_node<>* node);
	//virtual ~Creature();
};


#endif
