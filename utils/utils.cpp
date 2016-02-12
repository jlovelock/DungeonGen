
#include <iostream>
#include <string>
#include <cstdlib>
#include <defines.h>

using namespace std;


int d4(){ return rand()%4+1; }
int d6() { return rand()%6+1; }
int d8() { return rand()%8+1; }
int d10() { return rand()%10+1; }
int d12() { return rand()%12+1; }
int d20(){ return rand()%20+1; }
int d100() { return rand()%100+1; }

bool contains(string to_search, string keyword){
    return to_search.find(keyword) != string::npos;
}

void convert_to_lowercase(string& input){
    for(int i = 0; input[i] != '\0'; i++){
        input[i] = tolower(input[i]);
    }
}

void convert_to_uppercase(string& input){
    for(int i = 0; input[i] != '\0'; i++){
        input[i] = toupper(input[i]);
    }
}

void read(string& input){
    getline(cin, input);
    convert_to_lowercase(input);
}

bool is_skill(std::string s){
    convert_to_uppercase(s);
    for(unsigned i = 0; i < skills_list.size(); i++){
        if(skills_list[i] == s) return true;
    }
    return false;
}

//Precondition: list MUST be terminated with an empty string!!
bool in_list(string key, string arr[]){
    for(int i = 0; arr[i] != ""; i++){
        if(key == arr[i]) return true;
    }
    return false;
}
