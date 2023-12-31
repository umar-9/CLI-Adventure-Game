#ifndef CLASSES_H
#define CLASSES_H


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
    bool expendable;
    
    Items (std::string name_in = "", std::string item_dependency = "", int damage = 0, bool is_expendable = true) {
        name = name_in;
        dependency = item_dependency;
        dmg = damage;
        expendable = is_expendable;
    }

    bool operator==(const Items& other) {
        return (name == other.name);
    }

    friend std::ostream& operator<<(std::ostream& os, const Items& item) {
        os << item.name;
        return os;
    }

    void set_name(std::string name_in) {
        name = name_in;
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
        std::string move = (can_move) ? "\tThis monster can move between rooms, and will follow you!\n" : "\tThis monster cannot move between rooms.\n\n";
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

    void add_item (Items item) {
        items.push_back(item);
    }

    void add_interact (std::string new_interaction) {
        interact.push_back(new_interaction);
    }
};



#endif