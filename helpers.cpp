#include "helpers.h"

void help() {
    std::cout << "Guide:\n";
    std::cout << "\tEnter: North, South, East, or West\n";
    std::cout << "\tEnter: Grab <Item>\n";
    std::cout << "\tEnter: Use <Item>\n";
    std::cout << "\tEnter: Use Code <code>\n";
    std::cout << "\tEnter: Drop <Item>\n";
    std::cout << "You can have a maximum of 3 items in your inventory.\n";
    std::cout << "Some items can only be interacted with using items in your inventory, or some not at all.\n";
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
        } else {
            c = std::tolower(c);
        }
    }
    return cap_string;
}


void strip(std::string &str) {
    while (!str.empty() && str[0] == ' ') {
        str.erase(str.begin());
    }

    while (!str.empty() && str[str.length() - 1] == ' ') {
        str.erase(str.end() - 1);
    }
}

void clean(std::string &input) {
    input.erase(std::remove_if(input.begin(), input.end(), [](char c) {return !(std::isalnum(c) || c == ' '); }), input.end());
    strip(input);
    input = capitalise(input);
}
