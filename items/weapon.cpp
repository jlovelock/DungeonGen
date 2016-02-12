
#include <character.h>
#include <vector>
#include <string>
#include <sstream>

using namespace std;


//For monsters, when fancy stuff don't matter.
//Specifier in the format of "2/1d6+3 piercing" (where the 2/ at the beginning means +2 attack mod)
///TODO range increments?
Weapon::Weapon(string name, string specifier) : Object(name){
    _is_weapon = true;
    _override = true;
    _atk_mod = atoi(specifier.substr(0,specifier.find("/")).c_str());
    _n_dice = atoi(specifier.substr(specifier.find("/")+1, specifier.find("d")-specifier.find("/")-1).c_str());
    _weapon_die = atoi(specifier.substr(specifier.find("d")+1,specifier.find("+")-specifier.find("d")-1).c_str());
    _dmg_mod = atoi(specifier.substr(specifier.find("+")+1, specifier.find(" ")-specifier.find("+")-1).c_str());
    _dtype = specifier.substr(specifier.find(" ")+1);
}

Weapon::Weapon(string name): Object(name){
    _override = false;
    _is_weapon = true;

    if(name == "greataxe"){
        _n_dice = 1;
        _weapon_die = 12;
        _dtype = "slashing";
        _martial = true;
        _2h = true;
        _light = false;
        _heavy = true;
        _range = 5;
        _thrown = false;
        _finesse = false;
        _versatile = false;
        _loading = false;
    }

    else if(name == "hand crossbow"){
        _n_dice = 1;
        _weapon_die = 6;
        _dtype = "piercing";
        _martial = true;
        _2h = false;
        _light = true;
        _heavy = false;
        _range = 30;
        _thrown = false;
        _finesse = false;
        _versatile = false;
        _loading = false;
    }

    else if(name == "longbow"){
        _n_dice = 1;
        _weapon_die = 8;
        _dtype = "piercing";
        _martial = true;
        _2h = true;
        _light = false;
        _heavy = true;
        _range = 150;
        _thrown = false;
        _finesse = false;
        _versatile = false;
        _loading = false;
    }

    else if(name == "rapier"){
        _n_dice = 1;
        _weapon_die = 8;
        _dtype = "piercing";
        _martial = true;
        _2h = false;
        _light = false;
        _heavy = false;
        _range = 5;
        _thrown = false;
        _finesse = true;
        _versatile = false;
        _loading = false;
    }

    else if(name == "longsword"){
        _n_dice = 1;
        _weapon_die = 8;
        _dtype = "slashing";
        _martial = true;
        _2h = false;
        _light = false;
        _heavy = false;
        _range = 5;
        _thrown = false;
        _finesse = false;
        _versatile = true;
        _loading = false;
    }

    else if(name == "shortsword"){
        _n_dice = 1;
        _weapon_die = 6;
        _dtype = "piercing";
        _martial = true;
        _2h = false;
        _light = true;
        _heavy = false;
        _range = 5;
        _thrown = false;
        _finesse = true;
        _versatile = false;
        _loading = false;
    }

    else if(name == "handaxe"){
        _n_dice = 1;
        _weapon_die = 6;
        _dtype = "slashing";
        _martial = false;
        _2h = false;
        _light = true;
        _heavy = false;
        _range = 20;
        _thrown = true;
        _finesse = false;
        _versatile = false;
        _loading = false;
    }

}


int Weapon::proficiency(Character* c){
    if(!_martial) return c->proficiency_bonus(); ///TODO verify accurate
    else return (c->proficiency_bonus())*(c->proficient_with("martial") || c->proficient_with(_name));
}

//TODO: for finesse weapons, allow the user to choose whether they're attacking with STR or DEX,
//  but set a default (so you don't ask every time if irrelevant)
bool Weapon::attr_mod(Character* c){
    if(_range > 10 && !_thrown)
        return c->attribute_mod("DEX");
    else if(_finesse && c->attribute_mod("DEX") > c->attribute_mod("STR"))
        return c->attribute_mod("DEX");
    else
        return c->attribute_mod("STR");
}

int Weapon::base_dmg(Character* c){
    if(_versatile && c->has_free_hand())
        return rand()%(_weapon_die+2)+1;
    else
        return rand()%_weapon_die+1;
}


///TODO implement these
int Weapon::misc_atk_mods(Character* c){
    return 0;
}
int Weapon::misc_dmg_mods(Character* c){
    return 0;
}

int Weapon::get_attack_mod(Character* c){
    if(_override)
        return _atk_mod;

    else return
        attr_mod(c) +
        proficiency(c) +
        misc_atk_mods(c);
}

string Weapon::get_weapon_description(Character* c){
    std::stringstream ss;
    ss << _name << "+" << get_attack_mod(c) << ", d" << get_dmg_die() << "+" << get_dmg_bonus(c) << " " << _dtype << " damage";
    return ss.str();
}

int Weapon::attack_roll(Character* c, string mode){
    int roll;
    if(mode == "advantage")
        roll = max(d20(), d20());
    else if(mode == "disadvantage")
        roll = min(d20(), d20());
    else
        roll = d20();

    return roll + get_attack_mod(c);
}

int Weapon::get_dmg_bonus(Character* c){
    if(_override) return _dmg_mod;
    else return attr_mod(c) + misc_dmg_mods(c);
}


int Weapon::damage_roll(Character* c){
    return base_dmg(c) + get_dmg_bonus(c);
}

