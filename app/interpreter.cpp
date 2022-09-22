#include <iostream>
#include <set>
#include <vector>
#include <sstream>
#include "ra/map.hpp"
#include <string>

//avl-tree map interpreter for interaction

using iterator = map<std::string, int>::iterator;

//available commands
std::set<std::string> command_set{
    "map_create",
    "map_destroy",
    "map_move_assign",
    "map_move",
    "map_copy_assign",
    "map_copy",
    "map_at",
    "map_empty",
    "map_size",
    "map_max_size",
    "map_clear",
    "map_insert",
    "map_erase",
    "map_swap",
    "map_print",
    "map_print_all",
    "map_destroy_all",

    "map_begin",
    "map_end",
    "map_find",

    "iter_next",
    "iter_prev",
    "iter_print",
    "iter_data_erase",

    "iter_data_compare",
    "iter_destroy"
};

//list of map tag, pointer to map object pairs for keeping track of which map number refers to which map
std::vector<std::pair<int, map<std::string, int>*>> map_list;

//keeps track of the map numbers in use
std::set<int> map_numbers;

//list of iterator number, iterator object pairs for keeping track of which iterator number refers to which iterator
std::vector<std::pair<int, iterator>> iter_list;

//map and iterator numbers are 0 and the natural numbers

//select map number based on available numbers
int sel_num(){
    for(int i = 0; i < std::numeric_limits<int>::max(); i++){
        if(!map_numbers.contains(i)){
            return i;
        }
    }
    return 0;
}

//checks if our command arguments are numeric
bool only_digits(const std::string s){
    return s.find_first_not_of( "0123456789" ) == std::string::npos;
}

//tokenize input line and return it as a vector of tokens
std::vector<std::string> parse_line(std::string line){
    std::vector<std::string> tokens;

    std::stringstream ss(line);
    std::string s;

    while (getline(ss, s, ' ')) {
        tokens.push_back(s);
    }  
    return tokens;
}

//find map in map list given the tag
map<std::string, int>* find_map(int tag){
    for(auto pair: map_list){
        if(pair.first == tag){
            return pair.second;
        }
    }
    return nullptr;
}

//check if iterator exists currently in list given number
bool iter_exists(int num){

    for(auto pair: iter_list){
        if(pair.first == num){
            return true;
        }
    }
    return false;
}

//get a copy of the iterator with number
iterator get_iter_copy(int num){
    iterator i;

    for(auto pair: iter_list){
        if(pair.first == num){
            i = pair.second;
            break;
        }
    }
    return i;
}

//increment iterator
bool increment_iter(int num){
    iterator copy;

    for(int i = 0; i < iter_list.size(); i++){
        if(iter_list[i].first == num){
            copy = iter_list[i].second; //copy the iterator
            copy++;                     //increment copy

            if(copy.equals_end()){  //check if copy is at end
                return false;       //return false, since we don't won't to increment then
            }
            iter_list[i].second++;  //increment original iterator only if we're not point to the last map element
            break;
        }
    }
    return true;
}

//decrement iterator
bool decrement_iter(int num){

    for(int i = 0; i < iter_list.size(); i++){
        if(iter_list[i].first == num){

           if(iter_list[i].second.equals_start()){  //if iterator points to start
                return false;                       //return false, since there is no point to decrement since we're at the start
            }
            iter_list[i].second--;  //otherwise, decrement pointer
            break;
        }
    }
    return true;
}

void remove_map_pair(int map_num){
    int i = 0;

    while(i < map_list.size()){
        if(map_list[i].first == map_num){
            map_list.erase(map_list.begin() + i);
            break;
        }else{
            i++;
        }
    }
}

//command handler
void run_command(std::vector<std::string> command){
    int num;        //number for creating map numbers
    int map_num;    //holds a map number
    int val;        //holds value from key value pair
    bool success;   //measures particular operations are successful
    std::string key;//holds key from key value pair
    int iter_num;   //holds iterator number
    int new_map_num;

    //check if first token in command is in the list of commands
    if(command_set.contains(command[0])){

        //map creation
        if(command[0] == "map_create"){
        
            //possible input:
            //map_create
            //map_create tag_number(natural number + 0)
            if(command.size() > 2){
                std::cout << "Only 2 arguments maximum are accepted\n";

            //map tag provided as second argument
            }else if(command.size() == 2){

                if(only_digits(command[1])){
                    num = stoi(command[1]);
                    
                    if(!map_numbers.contains(num)){

                        map_numbers.insert(num);
                        map_list.emplace_back(num, new map<std::string, int>());
                        std::cout << "Created map with tag: " << num << "\n";

                    }else{
                        std::cout << "Map " << num << " already exists\n";
                    }

                }else{
                    std::cout << "Tag is non-integer\n";
                }

            //no map tag provided as second argument
            //create a tag for the new map
            }else{
                num = sel_num();
                map_numbers.insert(num);
                map_list.emplace_back(num, new map<std::string, int>());
                std::cout << "Created map with tag: " << num << "\n";
            }

        //map insertion
        //possible input: map_insert key(string) value(natural number + 0) map_tag(natural number + 0)
        }else if(command[0] == "map_insert"){

            if(command.size() != 4){
                std::cout << "Only 4 arguments for map_insert\n";

            }else{
                if(only_digits(command[2]) && only_digits(command[3])){

                    map_num = stoi(command[3]);

                    if(map_numbers.contains(map_num)){
                        val = stoi(command[2]);

                        map<std::string, int>* dest = find_map(map_num);
                        success = dest->insert(make_pair(command[1], val)).second;

                        if(success){
                            std::cout << command[1] << ", " << val << " inserted into map " << map_num << "\n";
                        }else{
                            std::cout << command[1] << ", " << val << " not inserted into map " << map_num << "\n";
                        }

                    }else{
                        std::cout << "No map corresponding to tag: " << map_num << "\n";
                    }

                }else{
                    std::cout << "The 3rd and 4th arguments must be integers\n";
                }
            }
        //map at (find value of key)
        //possible input:
        //map_at key(string) map_tag(natural number + 0)  
        }else if(command[0] == "map_at"){

            if(command.size() != 3){
                std::cout << "Only 3 arguments for map_at\n";

            }else{
                if(only_digits(command[2])){

                    map_num = stoi(command[2]);

                    if(map_numbers.contains(map_num)){
                        key = command[1];
                        map<std::string, int>* src = find_map(map_num);

                        try{
                            std::cout << "Value corresponding to key: " << key << ": " << src->at(key) << "\n";

                        }catch(const std::out_of_range e){
                            std::cout << "Does not exist\n";
                        }
                    }else{
                        std::cout << "No map corresponding to tag: " << map_num << "\n";
                    }
                }else{
                    std::cout << "The 2nd arguments must be an integer\n";
                }    
            }
        //map begin (iterator pointing to start of map)
        //possible input:
        //map_begin iterator_number(natural number + 0) map_tag(natural number + 0)
        }else if(command[0] == "map_begin"){
            
            if(command.size() != 3){
                std::cout << "Only 3 arguments for map_begin\n";
            }else{

                if(only_digits(command[1]) && only_digits(command[2])){

                    iter_num = stoi(command[1]);

                    if(!iter_exists(iter_num)){
                        map_num = stoi(command[2]);

                        if(map_numbers.contains(map_num)){
                            map<std::string, int>* src = find_map(map_num);
                            iter_list.emplace_back(iter_num, src->begin());

                            std::cout << "Created iterator " << iter_num << " pointing to beginning of map " << map_num << "\n";
                            
                        }else{
                            std::cout << "No map corresponding to tag: " << map_num << "\n";
                        }

                    }else{
                        std::cout << "Iterator " << iter_num << " already exists\n";
                    }
                    
                }else{
                    std::cout << "The 2nd and 3rd arguments must be integers\n";
                }
            }
        //iterator print (print iterator key value pair)
        //possible input:
        //iter_print iterator_number(nat num + 0)
        }else if(command[0] == "iter_print"){

            if(command.size() != 2){
                std::cout << "Only 2 arguments for iter_print\n";

            }else{
                if(only_digits(command[1])){
                    iter_num = stoi(command[1]);

                    if(iter_exists(iter_num)){

                        iterator i = get_iter_copy(iter_num);

                        //if iterator points to a nullptr then do not print anything
                        if(i.equals_end()){
                            std::cout << "No data for iterator " << iter_num << "\n";

                        }else{
                            std::cout << "Data for iterator " << iter_num << " is key " << i->data_.first << ", value " << i->data_.second << "\n";
                        }
                        
                    }else{
                        std::cout << "No iterator corresponding to number: " << iter_num << "\n";
                    }
                }else{
                    std::cout << "Argument must be an integer\n";
                }
            }
        //iterator next (next map element from iterator)
        //possible input:
        //iter_next iterator_number(nat num + 0)
        }else if(command[0] == "iter_next"){

            if(command.size() != 2){
                std::cout << "Only 2 arguments for iter_next\n";

            }else{
                if(only_digits(command[1])){
                    iter_num = stoi(command[1]);

                    if(iter_exists(iter_num)){
                        success = increment_iter(iter_num); //check if we are at the end of the map. do not increment if so

                        if(success){
                            std::cout << "Iterator " << iter_num << " incremented\n";
                        }else{
                            std::cout << "Cannot go any farther, we are at the end of the map\n";
                        }

                    }else{
                        std::cout << "No iterator corresponding to number: " << iter_num << "\n";
                    }

                }else{
                    std::cout << "The argument must be an integer\n";
                }
            }    
        //iterator previous (previous map element from iterator)
        //possible input:
        //iter_prev iterator_num(nat num + 0)
        }else if(command[0] == "iter_prev"){

            if(command.size() != 2){
                std::cout << "Only 2 arguments for iter_prev\n";

            }else{
                if(only_digits(command[1])){
                    iter_num = stoi(command[1]);

                    if(iter_exists(iter_num)){
                        success = decrement_iter(iter_num); //check if we are at the start of the map. do not decrement if so

                        if(success){
                            std::cout << "Iterator " << iter_num << " decremented\n";
                        }else{
                            std::cout << "Cannot go any farther, we are at the start of the map\n";
                        }

                    }else{
                        std::cout << "No iterator corresponding to number: " << iter_num << "\n";
                    }

                }else{
                    std::cout << "The argument must be an integer\n";
                }
            }    
        }else if(command[0] == "map_size"){

            if(command.size() != 2){
                std::cout << "Only 2 arguments for map_size\n";

            }else{
                if(only_digits(command[1])){

                    map_num = stoi(command[1]);

                    if(map_numbers.contains(map_num)){
                        map<std::string, int>* src = find_map(map_num);
                        std::cout << "Map " << map_num << " has size: " << src->size() << "\n";

                    }else{
                        std::cout << "No map corresponding to tag: " << map_num << "\n";
                    }
                }
            }
        }else if(command[0] == "map_max_size"){

            if(command.size() != 2){
                std::cout << "Only 2 arguments for map_max_size\n";

            }else{
                if(only_digits(command[1])){

                    map_num = stoi(command[1]);

                    if(map_numbers.contains(map_num)){
                        map<std::string, int>* src = find_map(map_num);
                        std::cout << "Map " << map_num << " has max size: " << src->max_size() << "\n";

                    }else{
                        std::cout << "No map corresponding to tag: " << map_num << "\n";
                    }
                }
            }
        }else if(command[0] == "map_empty"){

            if(command.size() != 2){
                std::cout << "Only 2 arguments for map_empty\n";

            }else{
                if(only_digits(command[1])){

                    map_num = stoi(command[1]);

                    if(map_numbers.contains(map_num)){
                        map<std::string, int>* src = find_map(map_num);

                        if(src->empty()){
                            std::cout << "Map " << map_num << " is empty\n";
                        }else{
                            std::cout << "Map " << map_num << " is not empty\n";
                        }
                        
                    }else{
                        std::cout << "No map corresponding to tag: " << map_num << "\n";
                    }
                }
            }
        }else if(command[0] == "map_clear"){

            if(command.size() != 2){
                std::cout << "Only 2 arguments for map_clear\n";

            }else{
                if(only_digits(command[1])){

                    map_num = stoi(command[1]);

                    if(map_numbers.contains(map_num)){
                        map<std::string, int>* src = find_map(map_num);

                        src->clear();

                        std::cout << "Cleared map: " << map_num << "\n";
                        
                    }else{
                        std::cout << "No map corresponding to tag: " << map_num << "\n";
                    }
                }
            }
        }else if(command[0] == "map_destroy"){

            if(command.size() != 2){
                std::cout << "Only 2 arguments for map_destroy\n";

            }else{
                if(only_digits(command[1])){

                    map_num = stoi(command[1]);

                    if(map_numbers.contains(map_num)){
                        map<std::string, int>* src = find_map(map_num);

                        delete(src);
                        remove_map_pair(map_num);
                        map_numbers.erase(map_num);

                        std::cout << "Destroyed " << map_num << "\n";
                        
                    }else{
                        std::cout << "No map corresponding to tag: " << map_num << "\n";
                    }
                }
            }
        }else if(command[0] == "map_move_assign"){

            if(command.size() != 3){
                std::cout << "Only 3 arguments for map_move_assign\n";

            }else{
                if(only_digits(command[1]) && only_digits(command[2])){

                    new_map_num = stoi(command[1]);
                    map_num = stoi(command[2]);

                    if(map_numbers.contains(map_num)){
                        map<std::string, int>* src = find_map(map_num);

                        if(map_numbers.contains(new_map_num)){
                            map<std::string, int>* dest = find_map(new_map_num);
                            *dest = std::move(*src);

                            std::cout << "Move assigned existing map " << new_map_num << " to map " << map_num <<"\n";

                        }else{
                            map_numbers.insert(new_map_num);
                            map_list.emplace_back(new_map_num, new map<std::string, int>());
                            map<std::string, int>* dest = find_map(new_map_num);
                            *dest = std::move(*src);

                            std::cout << "Move assigned new map " << new_map_num << " to map " << map_num <<"\n";
                        }
                        
                    }else{
                        std::cout << "No map corresponding to tag: " << map_num << "\n";
                    }
                }
            }
        }else if(command[0] == "map_copy_assign"){

            if(command.size() != 3){
                std::cout << "Only 3 arguments for map_copy_assign\n";

            }else{
                if(only_digits(command[1]) && only_digits(command[2])){

                    new_map_num = stoi(command[1]);
                    map_num = stoi(command[2]);

                    if(map_numbers.contains(map_num)){
                        map<std::string, int>* src = find_map(map_num);

                        if(map_numbers.contains(new_map_num)){
                            map<std::string, int>* dest = find_map(new_map_num);
                            *dest = *src;

                            std::cout << "Copy assigned existing map " << new_map_num << " to map " << map_num <<"\n";

                        }else{
                            map_numbers.insert(new_map_num);
                            map_list.emplace_back(new_map_num, new map<std::string, int>());
                            map<std::string, int>* dest = find_map(new_map_num);
                            *dest = *src;

                            std::cout << "Copy assigned new map " << new_map_num << " to map " << map_num <<"\n";
                        }
                        
                    }else{
                        std::cout << "No map corresponding to tag: " << map_num << "\n";
                    }
                }
            }
        }else if(command[0] == "map_copy"){

            if(command.size() != 3){
                std::cout << "Only 3 arguments for map_copy\n";

            }else{
                if(only_digits(command[1]) && only_digits(command[2])){

                    new_map_num = stoi(command[1]);
                    map_num = stoi(command[2]);

                    if(map_numbers.contains(map_num)){
                        map<std::string, int>* src = find_map(map_num);

                        if(!map_numbers.contains(new_map_num)){

                            map_numbers.insert(new_map_num);
                            map_list.emplace_back(new_map_num, new map<std::string, int>(*src));
                            
                            std::cout << "Copied map " << map_num << " to new map " << new_map_num <<"\n";

                        }else{
                            std::cout << "Copy failed, map " << new_map_num << " already exists\n";
                        }
                        
                    }else{
                        std::cout << "No map corresponding to tag: " << map_num << "\n";
                    }
                }
            }
        }else if(command[0] == "map_move"){

            if(command.size() != 3){
                std::cout << "Only 3 arguments for map_move\n";

            }else{
                if(only_digits(command[1]) && only_digits(command[2])){

                    new_map_num = stoi(command[1]);
                    map_num = stoi(command[2]);

                    if(map_numbers.contains(map_num)){
                        map<std::string, int>* src = find_map(map_num);

                        if(!map_numbers.contains(new_map_num)){

                            map_numbers.insert(new_map_num);
                            map_list.emplace_back(new_map_num, new map<std::string, int>(std::move(*src)));
                            
                            std::cout << "Moved map " << map_num << " to new map " << new_map_num <<"\n";

                        }else{
                            std::cout << "Move failed, map " << new_map_num << " already exists\n";
                        }
                        
                    }else{
                        std::cout << "No map corresponding to tag: " << map_num << "\n";
                    }
                }
            }
        }

    //first token in commands is not a valid command
    }else{
        std::cout << command[0] << " is an invalid command\n";
    }
}


int main(int argc, char** argv){
    std::string line;                   //for taking stdin
    std::vector<std::string> tokens;    //for taking tokens of the parsed line and passing to the command handler
    
    std::cout << "ENTER COMMANDS:\n";

    while(std::cin) {
        
        if(std::getline(std::cin, line)){

            if(line == "exit"){
                break;

            }else if(line.size() == 0){
                std::cout << "\n";
                continue;

            }else{
                tokens = parse_line(line);  //parse line
                run_command(tokens);        //run the command
            }
        }
    }
    for(auto pair: map_list){   //delete the maps created via new
        delete pair.second;
    }
    return 0;
}