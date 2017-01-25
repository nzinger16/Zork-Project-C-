#ifndef _CONTAINER_H
#define _CONTAINER_H

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



class Container {
public:
	bool has_trig;
	string name, description, status;
	vector<Trigger*> trigger;
	vector <string> accept, item;
	Container(xml_node<>* node);
	//virtual ~Container();
};

#endif
