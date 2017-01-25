#include "Container.h"

Container::Container(xml_node<>*node) {
	has_trig = false;
	for(xml_node<> * child = node->first_node(); child; child = child -> next_sibling()) {
			//cout << child->name() << endl;
			if(string(child->name()) == "name"){
				name = child -> value();
			}
			if(string(child->name()) == "description"){
				description = child -> value();
			}
			if(string(child->name()) == "status"){
				status = child -> value();
			}
			if(string(child->name()) == "accept"){
				//string buffer = child->value;
				accept.push_back(child->value());
			}
			if(string(child->name()) == "item"){
				item.push_back(child->value());
			}
			if(string(child->name()) == "trigger"){
				has_trig = true;
				Trigger * trigz = new Trigger(child);
				trigger.push_back(trigz);
			}
	}
}
