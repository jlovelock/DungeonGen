
#include <objects.h>
#include <string>
#include <character.h>
#include <spells.h>
#include <iostream>

using namespace std;

int Object::max_id = 0;

Object::~Object(){
    if(effect_on_hit){
        delete effect_on_hit;
    }
}

Object::Object(){
    _name = "??";
    _description = "--";
    _id = max_id++;

    _value = 0;
    identified = true;
    consumable = false;
    _is_weapon = false;
    _2h = false;
    effect_on_hit = NULL;
    rarity = "mundane";
    _lootable = true;
}

Object::Object(string name){
    _name = name;
    _description = name; ///@TODO placeholder here
    _id = max_id++;

    _value = 0;
    identified = true;
    consumable = false;
    _is_weapon = false;
    _2h = false;
    effect_on_hit = NULL;
    rarity = "mundane";
    _type = "object";
    quantity = 1;
    _lootable = true;
}

void Object::identify(){
    cout << "\t- Your " << get_description();

    if(quantity == 1)
        cout << " is a " << name() << " worth " << value() << " gp." << endl;
    else
        cout << " are " << name() << "s worth " << value() << " gp each." << endl;

    identified = true;
}

void Object::action_on_hit(Character* target, Character* attacker){
    if(effect_on_hit){
        effect_on_hit->cast(attacker, target);
    }
}


bool Object::is_equipped_to(Character* c){
    return (c->main_hand && _id == c->main_hand->_id) || (c->off_hand && _id == c->off_hand->_id);
}
