
#include <character.h>
#include <conditions.h>
#include <string>
using namespace std;

void Character::add_condition(Condition* c){

    if(is(c->name())){
        remove_condition(c->name(), true);

        if(is_monster)
            cout << "The " << full_name() << " was already " << c->name() << ", but it's been refreshed." << endl;
        else
            cout << "You were already " << c->name() << ", but the effect is refreshed." << endl;
    } else {
        if(is_monster)
            cout << "The " << full_name() << " is now " << c->name() << "." << endl;
        else
            cout << "You are now " << c->name() << "." << endl;
    }
    affected_conditions.push_back(c);
}

void Character::cause_condition(Condition* c) {
    caused_conditions.push_back(c);
}

void Character::remove_condition(string name, bool quiet){
    for(auto it = affected_conditions.begin(); it != affected_conditions.end(); ++it){
        if((*it)->name() == name && !(*it)->is_active()) {
            (*it)->deactivate();
            if(!quiet){
                if(is_PC())
                    cout << "You are no longer " << name << "." << endl;
                else
                    cout << "The " << full_name() << " is no longer " << name << "." << endl;
            }
            return;
        }
    }
}

bool Character::is(string effect){
    for(auto it = affected_conditions.begin(); it != affected_conditions.end(); ++it)
        if((*it)->name() == effect && (*it)->is_active()) return true;

    return false;
}

void Character::update_conditions(bool start_of_turn, bool quiet){
    if(DEBUG) cout << "$$\t" << full_name();
    if(DEBUG) { if(start_of_turn) cout << " start turn:" << endl; else cout << " end turn:" << endl; }

    for(int i = 0; i < (int)affected_conditions.size(); i++){
        if((start_of_turn && affected_conditions.at(i)->updates_at(TARGET_START_TURN))
           || (!start_of_turn && affected_conditions.at(i)->updates_at(TARGET_END_TURN)))
        affected_conditions.at(i)->advance(quiet);
    }

    for(int i = 0; i < (int)caused_conditions.size(); i++){
        if((start_of_turn && caused_conditions.at(i)->updates_at(CASTER_START_TURN))
           || (!start_of_turn && caused_conditions.at(i)->updates_at(CASTER_END_TURN)))
        caused_conditions.at(i)->advance(quiet);
    }

    if(DEBUG) cout << "$$\t...done" << endl;
}
