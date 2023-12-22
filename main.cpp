#include "main.h"


Items med_bag ("Medical Bag", "");
const int med_bag_heal = 50;
Items shield ("Shield", "");
Items key ("Key", "Chest");
Items drill ("Drill", "Safe");

Items sword("Sword", "", 50);
Items knife("Knife", "", 20);
Items revolver("Old Revolver", "", 30);
const int revolver_chance = 3;

Monsters std_monster ("Monster", 50, 15, false);
Monsters buff_monster ("Buff Monster", 100, 20, false);
Monsters minion ("Minion", 20, 5, true);

House hall ("Hall", {med_bag}, {"Chest"});
House kitchen ("Kitchen", {knife, shield, med_bag});
House bedroom ("Bedroom", {drill});
House garden ("Garden", {key}, {"Safe"});
House stairwell("Stairwell", {revolver});
House basement("Basement", {}, {"Cellar Doors"});
House escape ("Outside");


void use_item(Items item, House* current_room_ptr, std::vector <Items> &inventory, int &hp, bool &being_shielded);
void setup();


int main() {
    House* current_ptr = &hall;
    House* prev_ptr = &hall;
    bool escaped = false;
    int hitpoints = 100;
    std::vector<Items> inventory;
    bool shielded = false;

    setup();

    while (escaped == false && hitpoints > 0) {
        std::cout << "\nYou are in the " << current_ptr -> name << '\n';
        std::cout << "Hitpoints: " << hitpoints << '\n';
        std::cout << "Inventory: ";
        print_vector(inventory);

        if ((current_ptr -> items).size() != 0) {
            std::cout << "You see the following items: ";
            print_vector(current_ptr -> items);
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
        
        std::cout << "\n-------------------------------------------------------\n";    
        std::cout << "\nChoose your next action: ";
        std::string input;
        std::getline(std::cin, input);
        input = capitalise(input);

        if (input == "help" or input == "Help") {
            std::cout << "\nActions (case sensitive):\n";
            std::cout << "\tEnter: North, South, East, or West\n";
            std::cout << "\tEnter: Grab <Item>\n";
            std::cout << "\tEnter: Use <Item>\n";
            std::cout << "\tEnter: Drop <Item>\n";
            std::cout << "You can have a maximum of 3 items in your inventory.\n";

        } else if ((current_ptr -> locations).count(input)) {
            current_ptr = (current_ptr -> locations).at(input);
            std::cout << "You are now moving to: " << current_ptr -> name <<'\n';
        
        } else if (input.substr(0,4) == "Grab" or input.substr(0,4) == "grab") {
            std::string item_req = input.substr(5,input.size()-4);
            auto it = std::find((current_ptr -> items).begin(), (current_ptr -> items).end(), item_req);

            if (it != (current_ptr -> items).end()) {
                int index = std::distance((current_ptr -> items).begin(), it);
                Items current_item = (current_ptr -> items)[index];
                if (inventory.size() != 3) {
                    std::cout << "Nice find! \n";
                    inventory.push_back(current_item);
                    (current_ptr -> items).erase(std::remove((current_ptr -> items).begin(), (current_ptr -> items).end(), item_req), (current_ptr -> items).end());
                } else {
                    std::cout << "You already have the maximum inventory size\n";
                }
            } else {
                std::cout << "This item does not exist\n";
            }
        
        } else if (input.substr(0,3) == "Use" or input.substr(0,3) == "use") {
            std::string item_req = input.substr(4,input.size()-3);
            auto it = std::find(inventory.begin(), inventory.end(), item_req);
            if (it != inventory.end()) {
                int index = std::distance(inventory.begin(), it);
                use_item(inventory[index], current_ptr, inventory, hitpoints, shielded);
            } else {
                std::cout << "You don't have this item\n";
            }
        
        } else if (input.substr(0,4) == "Drop" or input.substr(0,4) == "drop") {
            std::string item_req = input.substr(5,input.size()-4);
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
            std::cout << "Invalid action or direction (there may not be a door there).\n";
            std::cout << "\t(Type 'help' if you need a list of allowed actions).";
        }
            
        auto it = (std::find_if((prev_ptr -> monsters).begin(), (prev_ptr -> monsters).end(), [] (Monsters i) {return i.can_move;}));
        if (it != (prev_ptr -> monsters).end()) {
            std::cout << "You hear a monster's footsteps!\n";
            Monsters monster = (prev_ptr -> monsters)[it - (prev_ptr -> monsters).begin()];
            current_ptr->add_monster(monster);
            erase_monster(prev_ptr, std::remove((prev_ptr -> monsters).begin(), (prev_ptr -> monsters).end(), monster), monster.name);
        }

        if (hitpoints < 1) {
            std::cout << "\nYou died...\n\n";
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
    std::cout << "You will see various items in different rooms. \nType Grab <Item> to pick it up, Drop <Item> to drop it (you only have 3 spaces in your inventory), and Use <Item> to use it. \nSome items in rooms cannot be picked up, but can be interacted with using items in your inventory.\n";
    std::cout << "Enter: North, South, East, or West to move rooms.\n";
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

    std::vector<House*> rooms = {&kitchen, &bedroom, &garden, &basement};
    const std::vector<Monsters> monsters = {std_monster, buff_monster, minion};

    srand(time(NULL));
    for (int i = 0; i < monsters.size(); i++) {
        int monster_rand = std::rand() % rooms.size();
        rooms[monster_rand] -> add_monster(monsters[i]);
    }
}




void use_item(Items item, House* current_room_ptr, std::vector <Items> &inventory, int &hp, bool &being_shielded) {
    if ((std::find((current_room_ptr -> interact).begin(), (current_room_ptr -> interact).end(), item.dependency) != (current_room_ptr -> interact).end()) or item.dependency == "") {
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
        std::cout << "You have received a sword! \n";
        inventory.push_back(sword);

    } else if (item.name == drill.name) {
        basement.add_location("North", &escape);
        std::cout << "Something seems to have opened somewhere...\n";
    
    } else if (item.name == med_bag.name) {
        hp += med_bag_heal;
        if (hp > 100) hp = 100;
        std::cout << "Healed!\n";
    
    } else if (item.name == shield.name) {
        being_shielded = true;
        std::cout << "The next time a monster attacks, you will not take any damage, and any minions will not follow you for one turn!\n";
    }
}



