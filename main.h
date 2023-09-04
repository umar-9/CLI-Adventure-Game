#include <vector>
#include <iostream>
#include <map>
#include <string>
#include <algorithm>
#include <random>
#include <time.h>
#include <cctype>


class Items {
public:
    std::string name;
    std::string dependency;
    int dmg;
    
    Items (std::string name_in = "", std::string item_dependency = "", int damage = 0) {
        name = name_in;
        dependency = item_dependency;
        dmg = damage;
    }

    bool operator==(const std::string &x) {
        return (name == x);
    }

};





class Monsters {
public:
    std::string name;
    int hp;
    int dmg;
    bool can_move;

    Monsters(std::string name_in, int hitpoints, int damage_per_sec, bool can_move_room) {
        name = name_in;
        hp = hitpoints;
        dmg = damage_per_sec;
        can_move = can_move_room;
    }

    void action(int &player_hp){
        std::cout << name << " stats: \n";
        std::cout << "\tHitpoints: " << hp << "; Damage: " << dmg << "\n";
        std::string move = (can_move) ? "\tThis monster can move beween rooms, and will follow you!\n" : "\tThis monster cannot move between rooms.\n\n";
        std::cout << move;

        player_hp -= dmg;
    }

    int kill(int damage_to_monster) {
        hp -= damage_to_monster;
        if (hp < 1) {
            std::cout << "You have slain the " << name << "!\n";
            return 0;
        }
        std::cout << "You made the " << name << " take " << damage_to_monster << " damage!\nThe monster now has " << hp << " hitpoints!\n";
        return hp;
    }

    bool operator==(const Monsters& other) const {
        return name == other.name;
    }
};




class House {
public:
    std::string name;
    std::vector<Items> items;
    std::map<std::string, House*> locations;
    std::vector<Monsters> monsters;
    std::vector<std::string> interact;

    House(std::string name_in, std::vector<Items> items_in = {}, std::vector<std::string> interact_with = {}) {
        name = name_in;
        items = items_in;
        interact = interact_with;
    }

    void add_monster (Monsters monster) {
        monsters.push_back(monster);
        interact.push_back(monster.name);
    }

    void add_location (std::string direction, House* room) {
        locations[direction] = room;
    }
};



void print_vector(std::vector<std::string> arr) {
    for (auto i : arr) {
        std::cout << i;
        if (!(i == arr[arr.size() - 1])) {
            std::cout << ", ";
        }
    }
    std::cout << '\n';
}

void print_vector(std::vector<Items> arr){
    for (auto i : arr) {
        std::cout << i.name;
        if (!(i == arr[arr.size() - 1].name)) {
            std::cout << ", ";
        }
    }
    std::cout << '\n';
}



void erase_monster(House* current_ptr, std::vector<Monsters>::iterator monster_it, std::string monster_name) {
    (current_ptr -> monsters).erase(monster_it);
    (current_ptr -> interact).erase(std::remove((current_ptr -> interact).begin(), (current_ptr -> interact).end(), monster_name), (current_ptr -> interact).end());
}



std::string capitalise(std::string input) {
    std::string cap_string = input;
    bool capitalise_next = true;
    for (auto &c : cap_string) {
        if (std::isspace(c)) {
            capitalise_next = true;
        } else if (capitalise_next) {
            c = std::toupper(c);
            capitalise_next = false;
        }
    }
    return cap_string;
}

