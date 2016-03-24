
#include <string>
#include <iostream>
#include <sstream>

#include <conditions.h>
#include <character.h>
#include <defines.h>

using namespace std;


Condition::Condition(string n, int dur, string check, int dc, bool tet){
    _name = n;
    rounds_remaining = dur;
    DC = dc;
    autopass = !dc;
    test_each_turn = tet;

    if(contains(check, "save")){
        check_type = "save";
        if(contains(check, "STR"))      check_stat = "STR";
        else if(contains(check, "DEX")) check_stat = "DEX";
        else if(contains(check, "CON")) check_stat = "CON";
        else if(contains(check, "INT")) check_stat = "INT";
        else if(contains(check, "WIS")) check_stat = "WIS";
        else if(contains(check, "CHA")) check_stat = "CHA";

    } else if(is_skill(check))
        check_type = "skill";
    else
        check_type = "attribute";
}

Condition::Condition(Condition* c){
    _name = c->_name;
    rounds_remaining = c->rounds_remaining;
    autopass = c->autopass;
    DC = c->DC;
    test_each_turn = c->test_each_turn;
    check_type = c->check_type; //attribute, skill, or save
    check_stat = c->check_stat;
}

Condition::~Condition(){}

bool Condition::advance(Character* c){
    if(--rounds_remaining <= 0 || test_each_turn){
        return test(c);
    } else return false;
}

bool Condition::test(Character* c){
    if(autopass) {
        if(c->is_PC())
            cout << "You are no longer " << _name << "." << endl;
        else
            cout << "The " << c->full_name() << " is no longer " << _name << "." << endl;
        return true;
    } else {
        int roll;
        if(check_type == "save")
            roll = c->save(check_stat);
        else if(check_type == "skill")
            roll = c->skill_check(check_stat);
        else //check_type == attribute
            roll = c->attribute_chk(check_stat);

        if(roll >= DC){
            if(c->is_PC())
                cout << "You manage to shrug off being " << _name << "." << endl;
            else
                cout << "The " << c->full_name() << " manages to shrug off being " << _name << "." << endl;
            return true;
        } else {
            if(c->is_PC())
                cout << "You are still " << _name << "." << endl;
            else
                cout << "The " << c->full_name() << " is still " << _name << "." << endl;
            return false;
        }
    }
}

void Condition::set_DC(int value){
    DC = value;
    autopass = !value;
}


string Condition::time_remaining(){
    stringstream ss;

    ss << rounds_remaining << " rounds (";
    if(rounds_remaining < 10){
        ss << rounds_remaining*6 << " seconds)";
    } else if(rounds_remaining < 600) {
        ss << rounds_remaining/10 << " minutes)";
    } else {
        ss << rounds_remaining/600 << " hours)";
    }
    return ss.str();
}
