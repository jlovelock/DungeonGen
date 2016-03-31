
#include <vector>
#include <string>
#include <sstream>

#include <weapon.h>
#include <character.h>

using namespace std;


void Weapon::set_defaults(){
    _is_weapon = true;
    _martial = false;
    _loading = false;
    _light = false;
    _heavy = false;
    _thrown = false;
    _finesse = false;
    _versatile = false;
    _weapon_die = 0;
    _n_dice = 0;
    _dmg_mod = 0;
    _atk_mod = 0;
    _dtype = "weapon";
    _range = 5;
    effect_on_hit = NULL;
    _type = "weapon";
    quantity = 1;
    _lootable = true;
}


//For monsters, when fancy stuff don't matter.
//Specifier in the format of "2/1d6+3 piercing" (where the 2/ at the beginning means +2 attack mod)
///TODO range increments?
Weapon::Weapon(string name, string specifier) : Object(name){
    set_defaults();
    _override = true;
    _atk_mod = atoi(specifier.substr(0,specifier.find("/")).c_str());
    _n_dice = atoi(specifier.substr(specifier.find("/")+1, specifier.find("d")-specifier.find("/")-1).c_str());
    _weapon_die = atoi(specifier.substr(specifier.find("d")+1,specifier.find("+")-specifier.find("d")-1).c_str());
    _dmg_mod = atoi(specifier.substr(specifier.find("+")+1, specifier.find(" ")-specifier.find("+")-1).c_str());
    _dtype = specifier.substr(specifier.find(" ")+1);
    _lootable = false;
}

Weapon::Weapon(string name): Object(name){
    _override = false;
    set_defaults();
    identified = true;

    if(name == "greataxe"){
        _n_dice = 1;
        _weapon_die = 12;
        _dtype = "slashing";
        _martial = true;
        _2h = true;
        _heavy = true;
        _value = 30;
        _weight = 7;
    }

    else if(name == "hand crossbow"){
        _n_dice = 1;
        _weapon_die = 6;
        _dtype = "piercing";
        _martial = true;
        _light = true;
        _range = 30;
        _value = 75;
        _weight = 3;
    }

    else if(name == "longbow"){
        _n_dice = 1;
        _weapon_die = 8;
        _dtype = "piercing";
        _martial = true;
        _2h = true;
        _heavy = true;
        _range = 150;
        _value = 50;
        _weight = 2;
    }

    else if(name == "rapier"){
        _n_dice = 1;
        _weapon_die = 8;
        _dtype = "piercing";
        _martial = true;
        _finesse = true;
        _value = 25;
        _weight = 2;
    }

    else if(name == "longsword"){
        _n_dice = 1;
        _weapon_die = 8;
        _dtype = "slashing";
        _martial = true;
        _versatile = true;
        _value = 15;
        _weight = 3;
    }

    else if(name == "shortsword"){
        _n_dice = 1;
        _weapon_die = 6;
        _dtype = "piercing";
        _martial = true;
        _light = true;
        _finesse = true;
        _value = 10;
        _weight = 2;
    }

    else if(name == "handaxe"){
        _n_dice = 1;
        _weapon_die = 6;
        _dtype = "slashing";
        _light = true;
        _range = 20;
        _thrown = true;
        _value = 5;
        _weight = 2;
    }

}


int Weapon::proficiency(Character* c){
    if(!_martial) return c->proficiency_bonus(); ///TODO verify accurate
    else return (c->proficiency_bonus())*(c->proficient_with("martial") || c->proficient_with(_name));
}

//TODO: for finesse weapons, allow the user to choose whether they're attacking with STR or DEX,
//  but set a default (so you don't ask every time if irrelevant)
int Weapon::attr_mod(Character* c){
    if(_range > 10 && !_thrown) {
        return c->attribute_mod("DEX");
    } else if(_finesse && c->attribute_mod("DEX") > c->attribute_mod("STR")) {
        return c->attribute_mod("DEX");
    } else {
        return c->attribute_mod("STR");
    }
}

int Weapon::base_dmg(Character* c){
    if(_weapon_die == 0)
        return 0;
    else if(_versatile && c->has_free_hand())
        return rand()%(_weapon_die+2)+1;
    else
        return rand()%_weapon_die+1;
}


int Weapon::misc_atk_mods(Character* c){
    int sum = 0;

    // Nb: yes, the archery fighting style applies to thrown weapons by RAW.
    if(c->has_fighting_style("archery") && _range > 10){
        sum += 2;
    }
    return sum;
}
int Weapon::misc_dmg_mods(Character* c){
    int sum = 0;

    /*
     * Say you have a pair of unequipped shortswords. Do they both deal +2 damage, because you _could_ equip them individually?
     * It's a strange question, and likely to be misleading whichever way you answer.
     * Here, I've chosen to only give a weapon bonuses from the dueling fighting style if the weapon is actually equipped.
     * In the future, I might try to print extra clarifying info: for now, just trust that naming the fighting style covers it.
     */
    if(c->has_fighting_style("dueling")
       && c->equipped_weapon_type() == "one handed"
       && is_equipped_to(c)){
        sum += 2;
    }
    return sum;
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
    ss << _name << " +" << get_attack_mod(c) << ", d" << get_dmg_die() << "+" << get_dmg_bonus(c) << " " << _dtype << " damage";
    return ss.str();
}

int Weapon::attack_roll(Character* c, string mode){
    int roll;

    if(c->is("blinded")){
        if(mode == "advantage") mode = "";
        else mode = "disadvantage";
    }

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


int Weapon::damage_roll(Character* c, string mode){
    if(mode == "crit")
        return base_dmg(c) + base_dmg(c) + get_dmg_bonus(c);
    else
        return base_dmg(c) + get_dmg_bonus(c);
}
