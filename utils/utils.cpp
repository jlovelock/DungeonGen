
#include <iostream>
#include <string>
#include <cstdlib>
#include <fstream>
#include <cstdarg>
#include <defines.h>

using namespace std;


int d4(){ return rand()%4+1; }
int d6() { return rand()%6+1; }
int d8() { return rand()%8+1; }
int d10() { return rand()%10+1; }
int d12() { return rand()%12+1; }
int d20(){ return rand()%20+1; }
int d100() { return rand()%100+1; }
int d20(string mode){
    int x = d20(), y = d20();
    if(mode == "disadvantage")
        return (x < y ? x : y);
    else
        return (x > y ? x : y);
}

int num_digits(int x){
    int n = 0;
    while(x > 0){
        x = x/10;
        n++;
    }
    return n;
}

bool is_vowel(char c){
    c = tolower(c);
    return c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u';
}

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
    cout << endl << ">> ";

    istream& s = test_mode ? testfile_input : cin;
    getline(s, input);
    convert_to_lowercase(input);

    if(contains(input, "exit") || contains(input, "quit")){
        cout << endl << "Thanks for playing!" << endl;
        exit(EXIT_SUCCESS);
    }
}

bool is_number(char c){
    return
        c == '1' ||
        c == '2' ||
        c == '3' ||
        c == '4' ||
        c == '5' ||
        c == '6' ||
        c == '7' ||
        c == '8' ||
        c == '9' ||
        c == '0';
}

/*
 * returns NOT_FOUND if no number was found
 * positive #s only (entering -10 will return 10)
 * if multiple numbers are present, returns the first one
 * decimals are rounded ("32.5" returns 32)
 */
int extract_num_from_string(string s){
    int start = NOT_FOUND, end = NOT_FOUND;
    for(unsigned i = 0; i < s.length(); i++){
        if(is_number(s.at(i)) && start == NOT_FOUND) {
            start = i;
        } else if(!is_number(s.at(i)) && start != NOT_FOUND) {
            end = i;
            break;
        }
    }
    if(start != NOT_FOUND){
        int len = (end == NOT_FOUND ? s.length()-start : end-start);
        return atoi(s.substr(start, len).c_str());
    }
    return NOT_FOUND;
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
        if(contains(key, arr[i])) return true;
    }
    return false;
}
