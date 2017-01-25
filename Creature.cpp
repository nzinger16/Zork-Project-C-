#include "Creature.h"

Creature::Creature(xml_node<>* node) {
	has_cond = false;
	has_print = false;
	has_action = false;
	has_attack = false;
	has_trig = false;
	for(xml_node<> * child = node->first_node(); child; child = child -> next_sibling()) {

		if(string(child->name()) == "name"){
			name = child -> value();
		}
		if(string(child->name()) == "description"){
			description = child -> value();
		}
		if(string(child->name()) == "status"){
			status = child -> value();
		}

		if(string(child->name()) == "trigger"){
			has_trig = true;
			Trigger * trigz = new Trigger(child);
			trigger.push_back(trigz);
		}
		if(string(child->name()) == "attack"){
			has_attack = true;
			for(xml_node<> * a_child = child->first_node(); a_child; a_child = a_child -> next_sibling()) {
				if(string(a_child->name()) == "print"){
					print = a_child -> value();
					has_print = true;
				}
				if(string(a_child->name()) == "action"){
					action.push_back(a_child -> value());
					has_print = true;
				}
				if(string(a_child->name()) == "condition"){
					has_cond = true;
					for(xml_node<> * c_child = a_child->first_node(); c_child; c_child = c_child -> next_sibling()) {
						if(string(c_child->name()) == "object"){
							C.object = c_child -> value();
						}
						if(string(c_child->name()) == "status"){
							C.status = c_child -> value();
						}
					}
				}
			}
		}
		if(string(child->name()) == "vulnerability"){
			v.push_back(child -> value());
		}
	}
}
