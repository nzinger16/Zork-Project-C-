#ifndef _ZORK_H
#define _ZORK_H
#include<iostream>
#include<fstream>
#include<cstdlib>
#include<vector>
#include<map>
#include<string>
#include<list>
#include "rapidxml-1.13/rapidxml.hpp"
#include "rapidxml-1.13/rapidxml_utils.hpp"
#include "rapidxml-1.13/rapidxml_print.hpp"
#include "Room.h"
#include "Container.h"
#include "Item.h"
#include "Creature.h"
#include "Trigger.h"

using namespace std;
using namespace rapidxml;
vector <Room *> rooms;
vector <Container *> containers;
vector <Item *> items;
vector <Creature *> creatures;

vector<string> inventory;

//Exit to end game
bool END;

//int room_ind, container_ind, item_ind, creature_ind;
Room * curr_room;
bool check_input(string);
bool check_input_with_no_trig(string);



void update_action(vector<string> actions) {
	string act;
	int space_pos;
	string object, status;
	for (unsigned int i = 0; i < actions.size(); i++) {
		space_pos = actions[i].find(" ");
		act = actions[i].substr(0, space_pos);
		actions[i].erase(0, space_pos + 1);
		if (act == "Add") {
			space_pos = actions[i].find(" ");
			object = actions[i].substr(0, space_pos);
			curr_room->item.push_back(object);
		}
		else if (act == "Delete") {
			//Deleting creatures
			for (unsigned int j = 0; j < curr_room->creature.size(); j++) {
				if (curr_room->creature[j] == actions[i]) {
					curr_room->creature.erase(curr_room->creature.begin() + j);
				}
			}
		}

		else if (act == "Update") {
			space_pos = actions[i].find(" ");
			object = actions[i].substr(0, space_pos);
			actions[i].erase(0, space_pos + 4);
			status = actions[i];
			//Updating Containers
			for (unsigned int i = 0; i < containers.size(); i++) {
				if (containers[i]->name == object) {
					containers[i]->status = status;
				}
			}

			//Updating Items
			for (unsigned int i = 0; i < items.size(); i++) {
				if (items[i]->name == object) {
					items[i]->status = status;
				}
			}
		}

		else {
			act.append(" ");
			act.append(actions[i]);
			END = check_input_with_no_trig(act);
		}


	}
}

void drop(string input) {
	for (unsigned int i = 0; i < inventory.size(); i++) {
		if (inventory[i] == input) {
			inventory.erase(inventory.begin() + i);
			curr_room->item.push_back(input);
			cout << input << " dropped" << endl;
			return;
		}
	}
	cout << "Error: item " << input << " is not found in inventory to drop" << endl;
	return;
}
void take(string input) {
	for (unsigned int i = 0; i < curr_room->item.size(); i++) {
		if (curr_room->item[i] == input) {
			inventory.push_back(input);
			curr_room->item[i] = "";
			cout << "item " << input << " added to inventory" << endl;
			for (unsigned int j = 0; j < containers.size(); j++) {
				for (unsigned int k = 0; k < containers[j]->item.size(); k++) {
					if (input == containers[j]->item[k]) {
						containers[j]->item.erase(containers[j]->item.begin() + k);
						return;
					}
				}
			}
			return;
		}
	}
	cout << "Sorry fam that item isn't here" << endl;
}

void read(string input) {
	for (unsigned int i = 0; i < inventory.size(); i++) {
		if (inventory[i] == input) {
			for (unsigned int ind = 0; ind < items.size(); ind++) {
				if (items[ind]->name == input) {
					if (items[ind]->writing != "") {
						cout << items[ind]->writing << endl;
					} else {
						cout << "Nothing is written." << endl;
					}
					return;
				}
			}
		}
	}
	cout << "No such item to read in inventory" << endl;
	return;
}



//False = no trigger, True = trigger
bool status_trigger(Trigger * t) {

	string object = t->stat.object;
	string status = t->stat.status;
	//cout << object << " " << status << endl;
	//check item objects
	for (unsigned int i = 0; i < items.size(); i++) {
		//cout << items[i]->name << " Status: " << items[i]->status << endl;
		if (items[i]->name == object && status == items[i]->status) {
			return true;
		}
	}

	//check container objects
	for (unsigned int i = 0; i < containers.size(); i++) {
		if (containers[i]->name == object && status == containers[i]->status) {
			return true;
		}
	}

	return false;
}
bool owner_trigger(Trigger * t) {
	string owner = t->own.owner;
	string has = t->own.has;
	string object = t->own.object;
	//for inventory (player needs this item to pass trigger)
	if (owner == "inventory") {
		if (has == "no") {
			for (unsigned int i = 0; i < inventory.size(); i++) {
				if (inventory[i] == object) {
					return false;
				}
			}
			return true;
		}
		if (has == "yes") {
			for (unsigned int i = 0; i < inventory.size(); i++) {
				if (inventory[i] == object) {
					return true;;
				}
			}
			return false;
		}
	}

	//for Container
	for (unsigned int i = 0; i < containers.size(); i++) {
		if (containers[i]->name == owner) {
			if (has == "yes") {
				for (unsigned int j = 0; j < containers[i]->item.size(); j++) {
					if (containers[i]->item[j] == object) {
						return true;
					}
				}
				return false;
			}
		}
	}
	return false;
}
bool check_command_trigger(string input) {
	bool trig_true = false;


	//check room

	for (unsigned int i = 0; i < curr_room->trigger.size(); i++) {
		if (curr_room->trigger[i]->command == input) {
			if (curr_room->trigger[i]->c_count == 2) {
				trig_true = status_trigger(curr_room->trigger[i]);
			} else if (curr_room->trigger[i]->c_count == 3) {
				trig_true = owner_trigger(curr_room->trigger[i]);
			}
			if (trig_true) {
				if (curr_room->trigger[i]->has_print) {
					cout << curr_room->trigger[i]->print << endl;
				}

				if (curr_room->trigger[i]->has_action) {
					update_action(curr_room->trigger[i]->action);
				}

				if (curr_room->trigger[i]->type == "single") {
					curr_room->trigger.erase(curr_room->trigger.begin() + i);
				}
			}
			//return trig_true;
			//break;
		}
	}

	//check creatures in room
	for (unsigned int i = 0; i < curr_room->creature.size(); i++) {
		for (unsigned int j = 0; j < creatures.size(); j++) {
			if (creatures[j]->name == curr_room->creature[i] && creatures[j]->has_trig) {
				for (unsigned int k = 0; k < creatures[j]->trigger.size(); k++) {
					if (creatures[j]->trigger[k]->command == input) {
						if (creatures[j]->trigger[k]->c_count == 2) {
							//cout << "got in here" << endl;
							trig_true = status_trigger(creatures[j]->trigger[k]);
						} else if (creatures[j]->trigger[k]->c_count == 3) {
							trig_true = owner_trigger(creatures[j]->trigger[k]);
						}
						if (trig_true) {

								if (creatures[j]->trigger[k]->has_print) {
									cout << creatures[j]->trigger[k]->print << endl;
								}

								if (creatures[j]->trigger[k]->has_action) {
									update_action(creatures[j]->trigger[k]->action);
								}

								if (creatures[j]->trigger[k]->type == "single") {
									creatures[j]->trigger.erase(creatures[j]->trigger.begin()+k);
								}
						}
						//return trig_true;
						//break;
					}
				}
			}
		}
	}

	return trig_true;
}
void check_condition_trigger(string type) {
	bool trig_true = false; //false means user does not satisfy trigger conditions
	bool has_c;
	if (type == "creature") {
			for (unsigned int i = 0; i < curr_room->creature.size(); i++) {
				for (unsigned int j = 0; j < creatures.size(); j++) {
					if (creatures[j]->name == curr_room->creature[i] && creatures[j]->has_trig) {
						for (unsigned int k = 0; k < creatures[j]->trigger.size(); k++) {
							has_c = creatures[j]->trigger[k]->has_command;
							if (creatures[j]->trigger[k]->c_count == 2 && !has_c) {
								trig_true = status_trigger(creatures[j]->trigger[k]);
							} else if (creatures[j]->trigger[k]->c_count == 3 && !has_c) {
								trig_true = owner_trigger(creatures[j]->trigger[k]);
							}
							if (trig_true) {

								if (creatures[j]->trigger[k]->has_print) {
									cout << creatures[j]->trigger[k]->print << endl;
								}

								if (creatures[j]->trigger[k]->has_action) {
									update_action(creatures[j]->trigger[k]->action);
								}

								if (creatures[j]->trigger[k]->type == "single") {
									creatures[j]->trigger.erase(creatures[j]->trigger.begin()+k);
								}
							}
						}
					}
				}
			}
	}

	//Check container triggers

	if (type == "container") {
			for (unsigned int i = 0; i < curr_room->container.size(); i++) {
				for (unsigned int j = 0; j < containers.size(); j++) {
					if (containers[j]->name == curr_room->container[i] && containers[j]->has_trig) {
						for (unsigned int k = 0; k < containers[j]->trigger.size(); k++) {
							if (containers[j]->trigger[k]->c_count == 2) {
								trig_true = status_trigger(containers[j]->trigger[k]);
							} else if (containers[j]->trigger[k]->c_count == 3) {
								trig_true = owner_trigger(containers[j]->trigger[k]);
							}
							if (trig_true) {

								if (containers[j]->trigger[k]->has_print) {
									cout << containers[j]->trigger[k]->print << endl;
								}

								if (containers[j]->trigger[k]->has_action) {
									update_action(containers[j]->trigger[k]->action);
								}

								if (containers[j]->trigger[k]->type == "single") {
									containers[j]->trigger.erase(containers[j]->trigger.begin()+k);
								}
							}
						}
					}
				}
			}
	}
	//Room triggers
	/*if (type == "room") {
		for (unsigned int i = 0; i < curr_room->trigger.size(); i++) {
				if (curr_room->trigger[i]->command == input) {
					if (curr_room->trigger[i]->c_count == 2) {
						trig_true = status_trigger(curr_room->trigger[i]);
					} else if (curr_room->trigger[i]->c_count == 3) {
						trig_true = owner_trigger(curr_room->trigger[i]);
					}
					if (trig_true) {
						//cout << curr_room->trigger[i]->print << endl;
						return true;
					}
				}
		}
	}*/

	//Container triggers
	/*if (type == "container") {
		for (unsigned int i = 0; i < containers.size(); i++) {
			if (containers[i]->has_trig && containers[i]->name == input) {
				for (unsigned int j = 0; j < containers[i]->trigger.size(); j++) {
					if (containers[i]->trigger[j]->c_count == 2) {
						trig_true = status_trigger(containers[i]->trigger[j]);
					} else if (containers[i]->trigger[j]->c_count == 3) {
						trig_true = owner_trigger(containers[i]->trigger[j]);
					}
					if (trig_true) {
						if (containers[i]->trigger[j]->has_print) {cout << containers[i]->trigger[j]->print << endl;}
						if (containers[i]->trigger[j]->has_action) {update_action(containers[i]->trigger[j]->action);}
					}
				}
			}
		}
	}*/

	//Creature triggers

}

bool open(string input) {
	if (input == "exit") {
		if (curr_room->type == input) {
			return true;
		} else {
			cout << "Sorry broh, this room isn't the exit. Keep playing to find it :)" << endl;
		}
		return false;
	}
	for (unsigned int i = 0; i < curr_room->container.size(); i++) {
		if (curr_room->container[i] == input) {
			for (unsigned int j = 0; j < containers.size(); j++) {
				if (input == containers[j]->name) {
					if (containers[j]->item.empty()) {
						cout << containers[j]->name << " is empy" << endl;
						return false;
					}
					cout << containers[j]->name << " contains ";
					for (unsigned int k = 0; k < containers[j]->item.size(); k++) {
						curr_room->item.push_back(containers[j]->item[k]);
						if (k == 0) {
							cout << containers[j]->item[k];
						} else {
							cout << ", " << containers[j]->item[k];
						}
						//containers[j]->item.erase(containers[j]->item.begin() + k);
					}
					cout << endl;
					return false;
				}
			}
		}
	}
	return false;
}
void turn_on(string input) {
	string new_stat;
	size_t pos;
	for (unsigned int i = 0; i < inventory.size(); i++) {
		if (input == inventory[i]) {
			cout << "You activate the " << input << endl;
			for (unsigned int ind = 0; ind < items.size(); ind++) {
				if(input == items[ind]->name) {
					cout << items[ind]->turnon.print << endl;
					pos = items[ind]->turnon.action.find_last_of(" ");
					new_stat = items[ind]->turnon.action.substr(pos + 1);
					items[ind]->status = new_stat;
					return;
				}
			}
		}
	}
	cout << "You don't have this object to turn on bruh" << endl;
	return;
}
void put(string object, string location) {
	//objects to be assigned for easier logic
	//Item * i;
	//Container * c;
	bool has_item = false;
	bool has_container = false;
	bool accept_item = false;

	//Check if player has item
	for (unsigned int i = 0; i < inventory.size(); i++) {
		if (object == inventory[i]) {
			has_item = true;
		}
	}
	if (!has_item) {
		cout << "Error: Don't have item " << object << " in inventory to put in " << location << endl;
		return;
	}

	//Check if container exists
	for (unsigned int i = 0; i < curr_room->container.size(); i++) {
		if (location == curr_room->container[i]) {
			has_container = true;
		}
	}

	if (!has_container) {
		cout << "Error: There is no " << location << " to put " << object << " in" << endl;
		return;
	}

	//check if container accepts item

	for (unsigned int i = 0; i < containers.size(); i++) {
		if (containers[i]->name == location) {
			if (!containers[i]->accept.empty()) {
				for (unsigned int j = 0; j < containers[i]->accept.size(); j++) {
					if (object == containers[i]->accept[j]) {
						accept_item = true;
					}
				}
			} else {
				accept_item = true;
			}
		}

	}

	if (!accept_item) {
		cout << "Error: " << "Can't put this " << object << " in " << location << endl;
		return;
	}

	cout << "Item " << object << " added to " << location << endl;

	//remove item from inventory
	for (unsigned int i = 0; i < inventory.size(); i++) {
		if (object == inventory[i]) {
			inventory.erase(inventory.begin() + i);
		}
	}

	//add item back to container and to room
	for (unsigned int i = 0; i < containers.size(); i++) {
		if(containers[i]->name == location) {
			containers[i]->item.push_back(object);
		}
	}
	curr_room->item.push_back(object);
	//check_trigger(location, "container");
	return;
}
void analyze_room(string input) {
	for (unsigned int ind = 0; ind < curr_room->border.size(); ind++) {
		if (input == curr_room->border[ind]->direction) {
			for (unsigned int i = 0; i < rooms.size(); i++) {
				if (rooms[i]->name == curr_room->border[ind]->name) {
					curr_room = rooms[i];
					break;
				}
			}
			cout << curr_room->description << endl;
			return;
		}
	}
	cout << "Can't go that way." << endl;
}


void attack(string monster, string weapon) {
	//check if creature is in room
	bool monster_exists = false;
	for (unsigned int i = 0; i < curr_room->creature.size(); i++) {
		if (curr_room->creature[i] == monster) {
			monster_exists = true;
		}
	}
	if (!monster_exists) {
		cout << "no such creature " << monster << " exists" << endl;
		return;
	}

	//check if user has the weapon to attack with
	bool weapon_exists = false;
	for (unsigned int i = 0; i < inventory.size(); i++) {
		if (inventory[i] == weapon) {
			weapon_exists = true;
		}
	}
	if (!weapon_exists) {
		cout << "no such item " << weapon << " exists in inventory" << endl;
		return;
	}

	//Access Creature and Weapon properties to analyze attack
	Creature * c_properties;
	Item * w_properties;

	for (unsigned int i = 0; i < creatures.size(); i++) {
		if (creatures[i]->name == monster) {
			c_properties = creatures[i];
		}
	}
	/*if (!c_properties->has_attack) {
		cout << "Can't attack " << c_properties->name << endl;
		return;
	}*/

	for (unsigned int i = 0; i < items.size(); i++) {
		if (items[i]->name == weapon) {
			w_properties = items[i];
		}
	}

	if (!c_properties->has_cond) {
		bool assault = false;
		for (unsigned int i = 0; i < c_properties->v.size(); i++) {
			if (c_properties->v[i] == weapon) {
				assault = true;
			}
		}
		if (assault) {
			cout << "You assault the " << monster << " with the " << weapon << endl;
		} else {
			cout << "Error: Cannot attack " << monster << "with this item" << endl;
		}
	} else {
		if (c_properties->C.object == weapon && c_properties->C.status == w_properties->status) {
			if (c_properties->has_print) {cout << c_properties->print << endl;}
			update_action(c_properties->action);

		} else {
			cout << "Error: " << weapon << " does not meet conditions to assault " << monster << endl;
		}
	}


	return;

}

bool check_input_with_no_trig(string input) {
	if (input == "n" || input == "s" || input == "w" || input == "e") {

			analyze_room(input);
			check_condition_trigger("creature");
			check_condition_trigger("container");

		return false;
	}

	if (input == "Game Over") {
		return true;
	}

	if(input == "i") {
		if (inventory.empty()) {
			cout << "inventory is empty" << endl;
		} else {
			cout << "Inventory: ";
			for (unsigned int i = 0; i < inventory.size(); i++) {
				if (i == 0) {
					cout << inventory[i];
				} else {
					cout << ", " << inventory[i];
				}
			}
			cout << endl;
		}
		return false;
	}

	if(input == "take"){
		cout<<"What do you want to take?" << endl;
		getline(cin, input);
		take(input);
		check_condition_trigger("creature");
		check_condition_trigger("container");
		return false;
	}
	if(input.find("take") != string::npos){

			input.erase(0,5);
			take(input);
			check_condition_trigger("creature");
			check_condition_trigger("container");


		return false;
	}

	if(input == "turn on"){
		cout<<"What item do you want to turn on?" << endl;
		getline(cin, input);
		turn_on(input);
		check_condition_trigger("creature");
		check_condition_trigger("container");
		return false;
	}
	if(input.find("turn on") != string::npos){

			input.erase(0,8);
			cout << input << endl;
			turn_on(input);
			check_condition_trigger("creature");
			check_condition_trigger("container");

		return false;
	}

	if(input == "read"){
		cout<<"What item do you want to turn on?" << endl;
		getline(cin, input);
		read(input);
		check_condition_trigger("creature");
		check_condition_trigger("container");
		return false;
	}
	if(input.find("read") != string::npos){

			input.erase(0,5);
			read(input);
			check_condition_trigger("creature");
			check_condition_trigger("container");
			return false;

	}
	if(input == "open"){
		cout<<"What do you want to open?" << endl;
		bool end_game = false;
		getline(cin, input);
		end_game = open(input);
		check_condition_trigger("creature");
		check_condition_trigger("container");
		return end_game;
	}
	if(input.find("open") != string::npos){
		bool end_game = false;

			input.erase(0,5);
			end_game = open(input);
			check_condition_trigger("creature");
			check_condition_trigger("container");

		return end_game;
	}

	if(input == "drop"){
		cout<<"What item do you want to drop?" << endl;
		getline(cin, input);
		drop(input);
		check_condition_trigger("creature");
		check_condition_trigger("container");
		return false;
	}
	if(input.find("drop") != string::npos){

			input.erase(0,5);
			drop(input);
			check_condition_trigger("creature");
			check_condition_trigger("container");

		return false;
	}

	if(input == "put"){
		string object, location;
		cout<<"What do you want to put?" << endl;
		getline(cin, object);
		cout <<"Where do you want to put it?" << endl;
		getline(cin, location);
		put(object, location);
		check_condition_trigger("creature");
		check_condition_trigger("container");
		return false;
	}
	if(input.find("put") != string::npos){

			input.erase(0,4);
			int space_pos = input.find(" ");
			string object = input.substr(0, space_pos);
			input.erase(0, space_pos + 1);
			if(input.substr(0, 2) != "in") {
				cout << "Error: to place item: \"put (item name) in (container)\"" << endl;
				return false;
			}
			input.erase(0, 3);
			string location = input;
			put(object, location);
			check_condition_trigger("creature");
			check_condition_trigger("container");

		return false;
	}
	if(input == "attack"){
		cout << "What do you want to attack? ";
		getline(cin, input);
		string monster = input;
		cout << "What do you want to attack with? ";
		getline(cin, input);
		string weapon = input;
		attack(monster, weapon);
		check_condition_trigger("creature");
		check_condition_trigger("container");
		return false;
	}
	if(input.find("attack") != string::npos){

			input.erase(0,7);
			int space_pos = input.find(" ");
			string monster = input.substr(0, space_pos);
			input.erase(0, space_pos + 1);
			if(input.substr(0, 4) != "with") {
				cout << "Error: to attack type: \"Attack (Creature name) with (object)\"" << endl;
				return false;
			}
			input.erase(0, 5);
			string weapon = input;
			attack(monster, weapon);
			check_condition_trigger("creature");
			check_condition_trigger("container");

		return false;
	}

	cout << "Error." << endl;
	return false;
}


bool check_input(string input) {
	if (input == "n" || input == "s" || input == "w" || input == "e") {
		if (!check_command_trigger(input)) {
			analyze_room(input);
			check_condition_trigger("creature");
			check_condition_trigger("container");
		}
		return false;
	}

	if(input == "i") {
		if (inventory.empty()) {
			cout << "inventory is empty" << endl;
		} else {
			cout << "Inventory: ";
			for (unsigned int i = 0; i < inventory.size(); i++) {
				if (i == 0) {
					cout << inventory[i];
				} else {
					cout << ", " << inventory[i];
				}
			}
			cout << endl;
		}
		return false;
	}

	if(input == "take"){
		cout<<"What do you want to take?" << endl;
		getline(cin, input);
		take(input);
		check_condition_trigger("creature");
		check_condition_trigger("container");
		return false;
	}
	if(input.find("take") != string::npos){
		if (!check_command_trigger(input)) {
			input.erase(0,5);
			take(input);
			check_condition_trigger("creature");
			check_condition_trigger("container");
		}

		return false;
	}

	if(input == "turn on"){
		cout<<"What item do you want to turn on?" << endl;
		getline(cin, input);
		turn_on(input);
		check_condition_trigger("creature");
		check_condition_trigger("container");
		return false;
	}
	if(input.find("turn on") != string::npos){
		if (!check_command_trigger(input)) {
			input.erase(0,8);
			cout << input << endl;
			turn_on(input);
			check_condition_trigger("creature");
			check_condition_trigger("container");
		}
		return false;
	}

	if(input == "read"){
		cout<<"What item do you want to turn on?" << endl;
		getline(cin, input);
		read(input);
		check_condition_trigger("creature");
		check_condition_trigger("container");
		return false;
	}
	if(input.find("read") != string::npos){
		if (!check_command_trigger(input)) {
			input.erase(0,5);
			read(input);
			check_condition_trigger("creature");
			check_condition_trigger("container");
			return false;
		}
	}
	if(input == "open"){
		cout<<"What do you want to open?" << endl;
		bool end_game = false;
		getline(cin, input);
		end_game = open(input);
		check_condition_trigger("creature");
		check_condition_trigger("container");
		return end_game;
	}
	if(input.find("open") != string::npos){
		bool end_game = false;
		if (!check_command_trigger(input)) {
			input.erase(0,5);
			end_game = open(input);
			check_condition_trigger("creature");
			check_condition_trigger("container");
		}
		return end_game;
	}

	if(input == "drop"){
		cout<<"What item do you want to drop?" << endl;
		getline(cin, input);
		drop(input);
		check_condition_trigger("creature");
		check_condition_trigger("container");
		return false;
	}
	if(input.find("drop") != string::npos){
		if (!check_command_trigger(input)) {
			input.erase(0,5);
			drop(input);
			check_condition_trigger("creature");
			check_condition_trigger("container");
		}
		return false;
	}

	if(input == "put"){
		string object, location;
		cout<<"What do you want to put?" << endl;
		getline(cin, object);
		cout <<"Where do you want to put it?" << endl;
		getline(cin, location);
		put(object, location);
		check_condition_trigger("creature");
		check_condition_trigger("container");
		return false;
	}
	if(input.find("put") != string::npos){
		if (!check_command_trigger(input)) {
			input.erase(0,4);
			int space_pos = input.find(" ");
			string object = input.substr(0, space_pos);
			input.erase(0, space_pos + 1);
			if(input.substr(0, 2) != "in") {
				cout << "Error: to place item: \"put (item name) in (container)\"" << endl;
				return false;
			}
			input.erase(0, 3);
			string location = input;
			put(object, location);
			check_condition_trigger("creature");
			check_condition_trigger("container");
		}
		return false;
	}
	if(input == "attack"){
		cout << "What do you want to attack? ";
		getline(cin, input);
		string monster = input;
		cout << "What do you want to attack with? ";
		getline(cin, input);
		string weapon = input;
		attack(monster, weapon);
		check_condition_trigger("creature");
		check_condition_trigger("container");
		return false;
	}
	if(input.find("attack") != string::npos){
		if (!check_command_trigger(input)) {
			input.erase(0,7);
			int space_pos = input.find(" ");
			string monster = input.substr(0, space_pos);
			input.erase(0, space_pos + 1);
			if(input.substr(0, 4) != "with") {
				cout << "Error: to attack type: \"Attack (Creature name) with (object)\"" << endl;
				return false;
			}
			input.erase(0, 5);
			string weapon = input;
			attack(monster, weapon);
			check_condition_trigger("creature");
			check_condition_trigger("container");
		}
		return false;
	}

	cout << "Error." << endl;
	return false;
}







#endif
