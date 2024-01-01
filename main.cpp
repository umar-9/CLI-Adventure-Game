#include "classes.h"
#include "helpers.h"

Items med_bag ("Medical Bag", "", 0, true, 60);
Items shield ("Shield", "");
Items key ("Key", "Chest With A Lock");
Items sledgehammer ("Sledgehammer", "Locked Shed (without a lock)");
Items usb ("Usb", "Laptop");
Items code ("Code 000", "Safe (3-digit pin)", 0, false);

Items sword("Sword", "", 50, false);
Items knife("Knife", "", 30, false);
Items revolver("Old Revolver", "", 30, false, 0, 3);
Items grenade("Grenade", "", 70, true, -10);

Monsters std_monster ("Monster", 50, 15, false);
Monsters buff_monster ("Buff Monster", 100, 15, false);
Monsters minion ("Minion", 20, 5, true);

House hall ("Hall", {}, {"Chest With A Lock"});
House kitchen ("Kitchen", {}, {});
House dining_room ("Dining Room");
House bedroom ("Bedroom", {key}, {"Laptop", "Safe (3-digit pin)"});
House garden ("Garden", {sledgehammer}, {"Locked Shed (without a lock)"});
House shed ("Shed", {grenade}, {});
House stairwell("Stairwell", {revolver});
House basement("Basement", {}, {"Locked Cellar Doors"});
House escape ("Outside");


void use_item(Items item, House* current_room_ptr, std::vector <Items> &inventory, int &hp, bool &being_shielded);
std::string setup();


int main() {
    House* current_ptr = &hall;
    House* prev_ptr = &hall;
    bool escaped = false;
    int hitpoints = 100;
    const int max_inventory_size = 4;
    std::vector<Items> inventory;
    bool shielded = false;

    code.set_name("Code " + setup());
    inventory.push_back(code);
    std::vector<Items> hidden_items = {code};

    int start_time = time(NULL);

    while (escaped == false && hitpoints > 0) {
        std::cout << '\n';
        std::cout << "Location: " << current_ptr -> name << '\t' << '\t';
        std::cout << "Hitpoints: " << hitpoints << '\t' << '\t';
        std::cout << "Inventory: ";
        print_vector(inventory, hidden_items);
        std::cout << '\n';

        if ((current_ptr -> items).size() != 0) {
            std::cout << "The following items are in the room: ";
            print_vector(current_ptr -> items, hidden_items);
        }

        if ((current_ptr -> interact).size() != 0) {
            std::cout << "You see the following things in the room: ";
            print_vector(current_ptr -> interact);
        }

        if ((current_ptr -> monsters).size() != 0 && (!shielded)){
            for (auto monster : (current_ptr -> monsters)) {
                monster.action(hitpoints);
            }
        } else if ((current_ptr -> monsters).size() != 0 && shielded){
            shielded = false;
        } else if (shielded) {
            std::cout << "You are shielded!\n";
        }


        prev_ptr = current_ptr;   
        
        std::cout << "\nChoose your next action: ";
        std::string input;
        std::getline(std::cin, input);
        clean(input);
        system("CLS");

        if (input.find("Help") != std::string::npos) {
            help();
        } else if (input == "North" || input == "East" || input == "South" || input == "West") { 
            if ((current_ptr -> locations).count(input)) {
                current_ptr = (current_ptr -> locations).at(input);
                std::cout << "You are now moving to: " << current_ptr -> name << '\n';
            } else {
                std::cout << "You cannot go that way\n";
            }
        } else if (input.substr(0,4) == "Grab") {
            std::string item_req = capitalise(input.substr(5,input.size()-4));
            auto it = std::find((current_ptr -> items).begin(), (current_ptr -> items).end(), item_req);

            if (it != (current_ptr -> items).end()) {
                int index = std::distance((current_ptr -> items).begin(), it);
                Items current_item = (current_ptr -> items)[index];
                if (inventory.size() != max_inventory_size) {
                    std::cout << "Nice find! \n";
                    inventory.push_back(current_item);
                    (current_ptr -> items).erase(std::find((current_ptr -> items).begin(), (current_ptr -> items).end(), item_req));
                } else {
                    std::cout << "You already have the maximum inventory size\n";
                }
            } else {
                std::cout << "This item does not exist\n";
            }
        
        } else if (input.substr(0,3) == "Use") {
            std::string item_req = capitalise(input.substr(4,input.size()-3));
            auto it = std::find(inventory.begin(), inventory.end(), item_req);
            if (it != inventory.end()) {
                int index = std::distance(inventory.begin(), it);
                use_item(inventory[index], current_ptr, inventory, hitpoints, shielded);
            } else {
                std::cout << "You don't have this item\n";
            }
        
        } else if (input.substr(0,4) == "Drop") {
            std::string item_req = capitalise(input.substr(5,input.size()-4));
            auto it = std::find(inventory.begin(), inventory.end(), item_req);
            if (it != inventory.end()) {
                int index = std::distance(inventory.begin(), it);
                Items current_item = inventory[index];
                current_ptr -> items.push_back(current_item);
                inventory.erase(std::remove(inventory.begin(), inventory.end(), item_req), inventory.end());
                std::cout << item_req << " has been dropped." << "\n";
            } else {
                std::cout << "You don't have this item\n";
            }
        

        } else {
            std::cout << "Invalid action or Syntax Error (Type 'help' if you need a list of allowed actions).\n";
        }
            
        auto it = (std::find_if((prev_ptr -> monsters).begin(), (prev_ptr -> monsters).end(), [] (Monsters i) {return i.can_move;}));
        if (it != (prev_ptr -> monsters).end()) {
            std::cout << "You hear a monster's footsteps!\n";
            Monsters monster = (prev_ptr -> monsters)[it - (prev_ptr -> monsters).begin()];
            current_ptr->add_monster(monster);
            erase_monster(prev_ptr, std::remove((prev_ptr -> monsters).begin(), (prev_ptr -> monsters).end(), monster), monster.name);
        }

        if (hitpoints < 1) {
            std::cout << "\n\nYou died...\n\n";
        }
        else if (current_ptr -> name == "Outside") {
            std::cout << "\nCongratulations! You have escaped!\n\n";
            escaped = true;
            int time_to_escape = time(NULL) - start_time;
            std::cout << "You took " << time_to_escape << " seconds to escape!\n";
            std::string str_personal_best;
            int personal_best;
            std::string filename = "record.txt";
            if (!std::filesystem::exists(filename)) {
                std::ofstream Record_file(filename);
                if (Record_file.is_open()) {
                    std::cout << "Record file created successfully.\n";
                    Record_file << "999999999\n";
                    Record_file.close();
                } else {
                    std::cerr << "Error creating the record file.\n";
                    return 1;
                }
            }

            std::ifstream Record_file("record.txt");
            if (!Record_file.is_open()) {
                std::cout << "Record file could not be opened.\n";
                return 1;
            }
            std::getline (Record_file, str_personal_best);
            try {
                personal_best = std::stoi(str_personal_best);
            } catch (const std::out_of_range& oor) {
                std::cerr << "You took too long to escape...\n";
                return 0;
            }
            Record_file.close();
            if (time_to_escape < personal_best) {
                std::ofstream Record_file("record.txt");
                if (!Record_file.is_open()) {
                    std::cout << "Record file could not be opened.\n";
                    return 1;
                }
                Record_file << time_to_escape;
                Record_file.close();
                std::cout << "You beat your personal record of " << personal_best << " by " << (personal_best - time_to_escape) << " seconds!\n";
            } else {
                std::cout << "Your personal best is " << personal_best << "\n";
            }
        }
    }
    system("PAUSE");
    return 0;
}



std::string setup() {

    std::cout << "\nWelcome to the House of Horrors.\nIf you can escape, you earn your freedom. Good Luck!\nType 'help' for a guide.\n";
    help();
    system("PAUSE");
    const std::vector<std::string> compass = {"North", "East", "South", "West"};
    srand(time(NULL));
    int shift = std::rand() % compass.size();

    hall.add_location(compass[(0+shift) % compass.size()], &kitchen);
    kitchen.add_location(compass[(2+shift) % compass.size()], &hall);
    hall.add_location(compass[(1+shift) % compass.size()], &bedroom);
    hall.add_location(compass[(3+shift) % compass.size()], &stairwell);
    stairwell.add_location(compass[(1+shift) % compass.size()], &hall);
    stairwell.add_location(compass[(3+shift) % compass.size()], &basement);
    basement.add_location(compass[(1+shift) % compass.size()], &stairwell);
    kitchen.add_location(compass[(1+shift) % compass.size()], &garden);
    bedroom.add_location(compass[(3+shift) % compass.size()], &hall);
    garden.add_location(compass[(3+shift) % compass.size()], &kitchen);
    dining_room.add_location(compass[(1+shift) % compass.size()], &kitchen);
    dining_room.add_location(compass[(2+shift) % compass.size()], &stairwell);
    stairwell.add_location(compass[(0+shift) % compass.size()], &dining_room);
    kitchen.add_location(compass[(3+shift) % compass.size()], &dining_room);
    shed.add_location(compass[(3+shift) % compass.size()], &garden);

    const std::vector<House*> spawning_rooms = {&kitchen, &dining_room, &bedroom, &garden, &basement};
    const std::vector<Monsters> monsters = {std_monster, buff_monster, minion};
    const std::vector<Items> items = {knife, shield, med_bag, med_bag};
    const int length_of_safe_pin = 3;

    for (int i = 0; i < monsters.size(); i++) {
        int monster_rand = std::rand() % spawning_rooms.size();
        spawning_rooms[monster_rand] -> add_monster(monsters[i]);
    }

    for (int i = 0; i < items.size(); i++) {
        int item_rand = std::rand() % spawning_rooms.size();
        spawning_rooms[item_rand] -> add_item(items[i]);
    }

    std::string pin = "";
    for (int i = 0; i < length_of_safe_pin; i++) {
        pin += '0' + (std::rand() % 10);
    }

    kitchen.add_interact("The number " + std::to_string(pin[0] - '0') + " written on a fridge magnet");
    shed.add_interact("The number " + std::to_string(pin[1] - '0') + std::to_string(pin[2] - '0') + " written on two pieces of paper");
    return pin;
}




void use_item(Items item, House* current_room_ptr, std::vector <Items> &inventory, int &hp, bool &being_shielded) {
    
    if ((std::find((current_room_ptr -> interact).begin(), (current_room_ptr -> interact).end(), item.dependant) == (current_room_ptr -> interact).end()) && item.dependant != "") {
        std::cout << "There's nothing in this room to use this item for.\n";
        return;
    }

    if (std::rand() % item.chance_failure == 0) {
        std::cout << "The " << item.name << " jammed and failed to work.\n";
        return;
    }

    if (item.is_expendable == true) {
        inventory.erase(std::find(inventory.begin(), inventory.end(), item.name));
        std::cout << item.name << " has been expended.\n";
    }

    (current_room_ptr -> interact).erase(std::remove((current_room_ptr -> interact).begin(), (current_room_ptr -> interact).end(), item.dependant), (current_room_ptr -> interact).end());
    item.health(hp);

    int total_number_of_monsters = (current_room_ptr->monsters).size();
    int number_of_monsters_affected;
    if (item == grenade) {
        number_of_monsters_affected = total_number_of_monsters;
    } else if (item == sword) {
        number_of_monsters_affected = 2;
    } else {
        number_of_monsters_affected = 1;
    }
    int x = 0;
    for (int i = 0; i < total_number_of_monsters && i < number_of_monsters_affected; i++) {
        if ((current_room_ptr->monsters)[x].kill(item.dmg) == 0) {
            auto monster_it = std::find((current_room_ptr -> monsters).begin(), (current_room_ptr -> monsters).end(), (current_room_ptr->monsters)[x]);
            erase_monster(current_room_ptr, monster_it, (current_room_ptr->monsters)[x].name);
        } else x++;
    }

    const std::vector<std::string> compass = {"North", "East", "South", "West"};

    if (item == key) {
        std::cout << "The chest opened! You have received a sword! \n";
        inventory.push_back(sword);
    } else if (item == code) {
        std::cout << "The safe opened! You have received a USB stick! \n";
        inventory.push_back(usb);
    } else if (item == usb) {
        for (auto i : compass) {
            basement.add_location(i, &escape);
        }
        std::cout << "Something seems to have opened somewhere...\n";
        basement.interact.erase(std::find(basement.interact.begin(), basement.interact.end(), "Locked Cellar Doors"));
    } else if (item == sledgehammer) {
        for (int i = 0; i < compass.size(); i++) {
            if (shed.locations.count(compass[i]) && shed.locations[compass[i]] == &garden) {
                if (i+2 > compass.size()) garden.add_location(compass[i-2], &shed);
                else garden.add_location(compass[i+2], &shed);
            }
        }
        std::cout << "You've broken the shed door down.\n";
    } else if (item == shield) {
        being_shielded = true;
        std::cout << "The next time a monster attacks, you will not take any damage, and any minions will not follow you for one turn!\n";
    }
}



