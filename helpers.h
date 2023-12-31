#ifndef HELPERS_H
#define HELPERS_H

#include "classes.h"


template <typename T>
void print_vector(const std::vector<T>& arr, std::vector<T> hidden_items = {}) {
    for (int i = 0; i < arr.size(); i++) {
        if (std::find(hidden_items.begin(), hidden_items.end(), arr[i]) == hidden_items.end()) { //if not a hidden item
            std::cout << arr[i];
            if (i != arr.size() - 1) {
                std::cout << ", ";
            }
        }
    }
    std::cout << '\n';
}

void help();
void erase_monster(House* current_ptr, std::vector<Monsters>::iterator monster_it, std::string monster_name);
std::string capitalise(std::string input);
void strip(std::string &str);
void clean(std::string &input);


#endif