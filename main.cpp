#include "Zork.hpp"
#include <stdlib.h>

int main(int argc, char*argv[]) {
	//cout << argc;
	if (argc < 2) {
		 cout << "Error. Please enter: filename.out filename.xml" <<endl;
		 return EXIT_FAILURE;
	}

	//Parsing input xml file
	file<> xmlFile(argv[1]);
	xml_document<> doc;
	doc.parse<0>(xmlFile.data());
	xml_node<> *node = doc.first_node();

	//List of xml nodes
	vector <xml_node<>*> room_list;
	vector <xml_node<>*> container_list;
	vector <xml_node<>*> item_list;
	vector <xml_node<>*> creature_list;


	for(xml_node<>*child = node -> first_node(); child; child = child -> next_sibling()){
		if(string(child->name()) == "room"){
			room_list.push_back(child);
		}
		if(string(child->name()) == "container"){

			container_list.push_back(child);
		}

		if(string(child->name()) == "item"){
			item_list.push_back(child);
		}
		if(string(child->name()) == "creature"){
			creature_list.push_back(child);
		}
	}



	//size of loops to create object lists
	int room_size = room_list.size();
	int container_size = container_list.size();
	//cout << container_size <<endl;
	int item_size = item_list.size();
	int creature_size = creature_list.size();

	int i;

	//loops to populate object lists
	for (i = 0; i < room_size; i++) {
		rooms.push_back(new Room(room_list[i]));
	}
	for (i = 0; i < container_size; i++) {
		containers.push_back(new Container(container_list[i]));
	}

	for (i = 0; i < item_size; i++) {
		items.push_back(new Item(item_list[i]));
	}

	for (i = 0; i < creature_size; i++) {
		creatures.push_back(new Creature(creature_list[i]));

	}

	//Start game
	curr_room = rooms[0];
	//Print inital Room/description
	cout <<  curr_room->description << endl;

	string input;
	while (true) {
		getline(cin, input);
		if(check_input(input)) {
			cout << "Game Over" << endl;
			break;
		} else if (END) {
			cout << "Victory!" << endl;
			break;
		}
	}




	return 0;
}
