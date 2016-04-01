
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
    _weight = 0;
    quantity = 1;
    identified = true;
    consumable = false;
    _is_weapon = false;
    _2h = false;
    effect_on_hit = NULL;
    rarity = "mundane";
    _lootable = true;
}

Object::Object(Object* o){
    _name = o->_name;
    _description = o->_description;
    _id = max_id++;

    _value = o->_value;
    _weight = o->_weight;
    identified = o->identified;
    consumable = o->consumable;
    _is_weapon = o->_is_weapon;
    _2h = o->_2h;
    effect_on_hit = o->effect_on_hit; ///@TODO CONFIRM THIS IS OKAY
    rarity = o->rarity;
    _lootable = o->_lootable;
    quantity = o->quantity;
}

Object* Object::clone(){
    return new Object(this);
}

Object::Object(string name){
    _name = name;
    _description = name; ///@TODO placeholder here
    _id = max_id++;

    _value = 0;
    _weight = 0;
    identified = true;
    consumable = false;
    _is_weapon = false;
    _2h = false;
    effect_on_hit = NULL;
    rarity = "mundane";
    _type = "object";
    quantity = 1;
    _lootable = true;

    if(name == "shield") _weight = 6;
}

void Object::identify(){
    cout << "Your " << get_description();

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
