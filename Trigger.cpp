#include "Trigger.h"


Trigger::Trigger(xml_node<> *node) {
  has_print = false;
  has_command = false;
  has_action = false;
  for(xml_node<> * child = node->first_node(); child; child = child -> next_sibling()) {
    if(string(child->name()) == "type"){
      type = child -> value();
    }

    if(string(child->name()) == "command"){
      command = child -> value();
      has_command = true;
    }

    if(string(child->name()) == "print"){
          print = child -> value();
          has_print = true;
    }

    if(string(child->name()) == "action"){
          action.push_back(child -> value());
          has_action = true;
    }
    if(string(child->name()) == "condition"){
    	  c_count = condition_count(child);
    	  if (c_count == 2) {
    		  for (xml_node<> * c_child = child->first_node(); c_child; c_child = c_child -> next_sibling()) {
    			  if (string(c_child->name()) == "object") {
    				  stat.object = c_child -> value();
    			  }
       			  if (string(c_child->name()) == "status") {
       				  stat.status = c_child -> value();
       			  }

    		  }
    	  } else if (c_count == 3) {
    		  for (xml_node<> * c_child = child->first_node(); c_child; c_child = c_child -> next_sibling()) {
    			  if (string(c_child->name()) == "has") {
    				  own.has = c_child -> value();
    			  }
       			  if (string(c_child->name()) == "object") {
       				  own.object = c_child -> value();
       			  }

       			  if (string(c_child->name()) == "owner") {
       				  own.owner = c_child -> value();
       			  }
    		  }
    	  }
    }
  }
}

int Trigger::condition_count(xml_node<> * node) {
	int num = 0;
	for(xml_node<> * child = node->first_node(); child; child = child -> next_sibling()) {
		num++;
	 }
     return num;
}
