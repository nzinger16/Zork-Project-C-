#ifndef _TRIGGER_H
#define _TRIGGER_H

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
using namespace std;
using namespace rapidxml;

typedef struct _status {
	string object, status;
} status ;

typedef struct _owner {
	string object, has, owner;
} owner ;


class Trigger {
public:
	string type;
	string command;
	string print;
	vector<string> action;
	int c_count;
	bool has_command;
	bool has_print;
	bool has_action;
	status stat;
	owner own;
	Trigger(xml_node<> * node);
	//virtual ~Trigger();
	int condition_count(xml_node<> * node);

};

#endif
