
#include <string>
#include <iostream>
#include <sstream>

#include <conditions.h>
#include <character.h>
#include <defines.h>

using namespace std;

void Condition::apply(Character* target, Character* caster){

    /*
     * Copy the condition itself, not just the pointer.
     * That way, the original Condition can be deleted safely in the Spell deconstructor,
     *   and these in the Character deconstructor.
     */
    Condition* applied = new Condition(this, target);
    target->add_condition(applied);
    caster->cause_condition(applied);
}


Condition::Condition(string n, int dur, string check, int dc, int time){
    _name = n;
    rounds_remaining = dur;
    DC = dc;
    autopass = !dc;
    test_each_turn = DC;
    check_time = time;

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

Condition::Condition(Condition* c, Character* t){
    _name = c->_name;
    rounds_remaining = c->rounds_remaining;
    autopass = c->autopass;
    DC = c->DC;
    test_each_turn = c->test_each_turn;
    check_type = c->check_type; //attribute, skill, or save
    check_stat = c->check_stat;
    check_time = c->check_time;
    active = true;
    target = t;
}

Condition::~Condition(){}

bool Condition::advance(bool quiet){
    if(--rounds_remaining <= 0 || test_each_turn){
        return test(quiet);
    } else return false;
}

bool Condition::test(bool quiet){
    if(autopass) {
        target->remove_condition(this->name(), quiet);
        return true;
    } else {
        int roll;
        if(check_type == "save")
            roll = target->save(check_stat);
        else if(check_type == "skill")
            roll = target->skill_check(check_stat);
        else //check_type == attribute
            roll = target->attribute_chk(check_stat);

        if(roll >= DC){
            if(target->is_PC())
                cout << "You manage to shrug off being " << _name << "." << endl;
            else
                cout << "The " << target->full_name() << " manages to shrug off being " << _name << "." << endl;
                active = false;
            return true;
        } else if(!quiet) {
            if(target->is_PC())
                cout << "You are still " << _name << "." << endl;
            else
                cout << "The " << target->full_name() << " is still " << _name << "." << endl;
        }
    }
    return false;
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
