#include "Item.h"

Item::Item(xml_node<>*node) {
	has_turnon = false;
	for(xml_node<> * child = node->first_node(); child; child = child -> next_sibling()) {
			if(string(child->name()) == "name"){
				name = child -> value();
			}
			if(string(child->name()) == "description"){
				description = child -> value();
			}
			if(string(child->name()) == "writing"){
				writing = child -> value();
			}
			if(string(child->name()) == "status"){
				status = child -> value();
			}
			if(string(child->name()) == "turnon"){
				has_turnon = true;
				for (xml_node<> * t_child = child->first_node(); t_child; t_child = t_child -> next_sibling()) {
					if (string(t_child -> name()) == "print") {
						turnon.print = t_child -> value();
					}
					if (string(t_child -> name()) == "action") {
						turnon.action = t_child -> value();
					}
				}

			}
			if(string(child->name()) == "trigger"){
				Trigger * trigz = new Trigger(child);
				trigger.push_back(trigz);
			}
	}
}
