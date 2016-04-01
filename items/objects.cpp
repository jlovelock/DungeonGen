
#include <objects.h>
#include <string>
#include <character.h>
#include <spells.h>
#include <iostream>
#include <sstream>

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
    _type = o->_type;
    identified = o->identified;
    consumable = o->consumable;
    _is_weapon = o->_is_weapon;
    _2h = o->_2h;
    if(o->effect_on_hit) effect_on_hit = new Spell(o->effect_on_hit);
    else effect_on_hit = NULL;
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

string Object::get_description_with_article(bool capitalized){
    if(quantity > 1){
        stringstream ss;
        ss << quantity << " " << get_description(true);
        return ss.str();
    } else {
        if(is_vowel(get_description().at(0)))
            if(capitalized) return "An " + get_description();
            else return "an " + get_description();
        else
            if(capitalized) return "A " + get_description();
            else return "a " + get_description();
    }
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
