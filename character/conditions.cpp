
#include <character.h>
#include <conditions.h>
#include <string>
using namespace std;

void Character::add_condition(string name, int duration, int DC, string check){
    add_condition(new Condition(name, duration, check, DC));
}

void Character::add_condition(Condition* c){
    /*
     * Copy the condition itself, not just the pointer.
     * That way, the original Condition can be deleted safely in the Spell deconstructor,
     *   and these in the Character deconstructor.
     */
    conditions.push_back(new Condition(c));

    if(is_monster)
        cout << "The " << full_name() << " is now " << c->name() << "." << endl;
    else
        cout << "You are now " << c->name() << "." << endl;
}

void Character::remove_condition(string name){
    for(vector<Condition*>::iterator it = conditions.begin(); it != conditions.end(); ++it){
        if((*it)->name() == name) {
            delete *it;
            conditions.erase(it);
            return;
        }
    }
}

bool Character::is(string effect){
    for(vector<Condition*>::iterator it = conditions.begin(); it != conditions.end(); ++it)
        if((*it)->name() == effect) return true;

    return false;
}

void Character::update_conditions(){
    for(int i = 0; i < (int)conditions.size(); i++){
        if(conditions.at(i)->advance(this)){
            delete conditions.at(i);
            conditions.erase(conditions.begin()+i);
            i--;
        }
    }
}
