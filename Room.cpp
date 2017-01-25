#include "Room.h"

//constructor
Room::Room(xml_node<> *node) {
	for(xml_node<> * child = node->first_node(); child; child = child -> next_sibling()) {
		if(string(child->name()) == "name"){
			name = child -> value();
		}
		if(string(child->name()) == "status"){
			status = child -> value();
		}

		if(string(child->name()) == "type"){
			type = child -> value();
		}


		if(string(child->name()) == "description"){
			description = child -> value();
		}

		if(string(child->name()) == "border"){
			B * newborder = new B();
			for (xml_node<> * b_child = child->first_node(); b_child; b_child = b_child -> next_sibling()) {
				string dir;
				if (string(b_child->name()) == "name") {
					newborder->name = b_child->value();
				}
				if(string(b_child->name()) == "direction"){
					dir = b_child -> value();
					if (dir == "north") {dir = "n";}
					if (dir == "south") {dir = "s";}
					if (dir == "east") {dir = "e";}
					if (dir == "west") {dir = "w";}
					newborder->direction = dir;
				}
			}
			border.push_back(newborder);
		}

		if(string(child->name()) == "container"){
		  container.push_back(child -> value());
		}

		if(string(child->name()) == "item"){
		  item.push_back(child -> value());
		}

		if(string(child->name()) == "creature"){
			creature.push_back(child -> value());
		}

		if(string(child->name()) == "trigger"){
		  Trigger * t = new Trigger(child);
		  trigger.push_back(t);
		}
	}

}

