#include "classes.h"
#include "helpers.h"

Items med_bag ("Medical Bag", "");
const int med_bag_heal = 60;
Items shield ("Shield", "");
Items key ("Key", "Chest with a lock");
Items sledgehammer ("Sledgehammer", "Locked Shed (without a lock)");
Items usb ("Usb", "Laptop");
Items code ("Code 000", "Safe (3-digit pin)", 0, false);

Items sword("Sword", "", 50, true);
Items knife("Knife", "", 30, true);
Items revolver("Old Revolver", "", 30, true);
const int revolver_chance = 3;

Monsters std_monster ("Monster", 50, 15, false);
Monsters buff_monster ("Buff Monster", 100, 20, false);
Monsters minion ("Minion", 20, 5, true);

House hall ("Hall", {}, {"Chest with a lock"});
House kitchen ("Kitchen", {}, {});
House dining_room ("Dining Room");
House bedroom ("Bedroom", {key}, {"Laptop", "Safe (3-digit pin)"});
House garden ("Garden", {sledgehammer}, {"Locked Shed (without a lock)"});
House shed ("Shed", {}, {});
House stairwell("Stairwell", {revolver});
House basement("Basement", {}, {"Locked Cellar Doors (you see it is an electronic lock)"});
House escape ("Outside");

//key+chest gives sword/
//usb+laptop opens cellar doors (escape)
//924+safe gives usb (Code 924 is added to inventory (add condition for inventory size) but isn't printed: "Use Code 924")
//drill+lockedshed gives shed

void use_item(Items item, House* current_room_ptr, std::vector <Items> &inventory, int &hp, bool &being_shielded);
void setup();

// Add difficulty levels using command line args
// Make damage per second
int main() {
    House* current_ptr = &hall;
    House* prev_ptr = &hall;
    bool escaped = false;
    int hitpoints = 100;
    int max_inventory_size = 4;
    std::vector<Items> inventory = {code};
    std::vector<Items> hidden_items = {code};
    bool shielded = false;

    setup();

    while (escaped == false && hitpoints > 0) {
        std::cout << '\n';
        std::cout << "Location: " << current_ptr -> name << '\t' << '\t';
        std::cout << "Hitpoints: " << hitpoints << '\t' << '\t';
        std::cout << "Inventory: ";
        print_vector(inventory, hidden_items);
        std::cout << '\n';

        if ((current_ptr -> items).size() != 0) {
            std::cout << "You see the following items: ";
            print_vector(current_ptr -> items, hidden_items);
        }

        if ((current_ptr -> interact).size() != 0) {
            std::cout << "You see the following in the room: ";
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
        }
    }
    std::string a;
    std::cin >> a;
    return 0;
}



void setup() {

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


    const std::vector<House*> spawning_rooms = {&kitchen, &bedroom, &garden, &basement};
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

    std::string pin;
    for (int i = 0; i < length_of_safe_pin; i++) {
        pin[i] = '0' + (std::rand() % 10);
    }
    pin[length_of_safe_pin] = '\0';
    code.set_name("Code " + pin);
    kitchen.add_interact("The number " + std::to_string(pin[0]) + " written on a fridge magnet");
    shed.add_interact("The number " + std::to_string(pin[1]) + std::to_string(pin[2]) + " written on two pieces of paper");
}




void use_item(Items item, House* current_room_ptr, std::vector <Items> &inventory, int &hp, bool &being_shielded) {
    if ((std::find((current_room_ptr -> interact).begin(), (current_room_ptr -> interact).end(), item.dependency) != (current_room_ptr -> interact).end()) || item.dependency == "") {
        if (item.dmg == 0) {
            inventory.erase(std::find(inventory.begin(), inventory.end(), item.name));
            std::cout << item.name << " has been expended.\n";
            (current_room_ptr -> interact).erase(std::remove((current_room_ptr -> interact).begin(), (current_room_ptr -> interact).end(), item.dependency), (current_room_ptr -> interact).end());

        } else if ((current_room_ptr -> monsters).size() != 0) {
            if (item.name == revolver.name) {
                if (std::rand() % revolver_chance == 0) {
                    std::cout << "The old revolver jammed and did not fire...\n";
                    return;
                }
            }
            if ((current_room_ptr -> monsters[0]).kill(item.dmg) == 0) {
                erase_monster(current_room_ptr, (current_room_ptr -> monsters).begin(), current_room_ptr -> monsters[0].name);
            }
        }
    
    } else {
        std::cout << "There's nothing in this room to use this item for.\n";
        return;
    }
    


    if (item.name == key.name) {
        std::cout << "The chest opened! You have received a sword! \n";
        inventory.push_back(sword);
    } else if (item.name == code.name) {
        std::cout << "The safe opened! You have received a USB stick! \n";
        inventory.push_back(usb);
    } else if (item.name == usb.name) {
        basement.add_location("North", &escape);
        basement.add_location("South", &escape);
        basement.add_location("West", &escape);
        std::cout << "Something seems to have opened somewhere...\n";
    } else if (item.name == sledgehammer.name) {
        garden.add_location("East", &shed);
        std::cout << "You've broken the shed door down.\n";
    } else if (item.name == med_bag.name) {
        hp += med_bag_heal;
        if (hp > 100) hp = 100;
        std::cout << "Healed!\n";
    
    } else if (item.name == shield.name) {
        being_shielded = true;
        std::cout << "The next time a monster attacks, you will not take any damage, and any minions will not follow you for one turn!\n";
    }
}



