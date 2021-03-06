
#include <iostream>
#include <string>
#include <limits.h>
#include <math.h>

#include <character.h>
#include <door.h>
#include <spells.h>
#include <objects.h>
#include <weapon.h>
#include <conditions.h>
#include <inventory.h>

using namespace std;


void Character::print_inventory(){ inventory->print(this); }

Character::~Character(){
    delete inventory;

    /*
     * Only delete from caused conditions vector. Same condition is spread across 2 chars, shouldn't double delete!
     */
    for(auto it = caused_conditions.begin(); it != caused_conditions.end(); ++it){
        delete *it;
    }
    affected_conditions.clear();
    caused_conditions.clear();
}

bool Character::proficient_with(string weapon_type){
    for(auto it = weapon_proficiencies.begin(); it != weapon_proficiencies.end(); ++it) {
        if (*it == weapon_type) return true;
    }
    return false;
}

int Character::skill_check(std::string skill){
    if(skill == "PERCEPTION" && is("blinded")) return 0;
    return d20()+skill_mods[skill];
}

int Character::attribute_chk(std::string att){
    if(is_heavily_encumbered() && (att == "STR" || att == "DEX" || att == "CON")){
        cout << "The crushing weight of your pack on your back makes it difficult for you to execute smoothly." << endl;
        return d20("disadvantage") + attribute_mods[att];
    }
    return d20() + attribute_mods[att];
}

int Character::save(string stat){
    if(is("paralyzed") && (stat == "STR" || stat == "DEX")){
        cout << "Paralyzed and helpless, ";
        if(is_monster) cout << "he is"; else cout << "you are";
        cout << " entirely unable to avoid the effect." << endl;
        return 0;
    }
    if(is_heavily_encumbered() && (stat == "STR" || stat == "DEX" || stat == "CON")){
        cout << "The crushing weight of your pack on your back makes it difficult for you to keep steady." << endl;
        return d20("disadvantage") + save_mods[stat];
    }
    return d20() + save_mods[stat];
}

void Character::start_turn(bool quiet){
    update_conditions(true, quiet);
}

void Character::end_turn(bool quiet){
    if(DEBUG) cout << "EOT: " << name() << "...";
    update_conditions(false, quiet);

    if(is_heavily_encumbered() && is_PC()) cout << "You are heavily encumbered. You may want to drop some items." << endl;
    else if(is_encumbered() && is_PC()) cout << "You are encumbered. You may want to drop some items." << endl;

    if(DEBUG) cout << "done" << endl;
}


bool Character::has_fighting_style(string style){
    for(auto it = fighting_styles.begin(); it != fighting_styles.end(); ++it){
        if((*it) == style) return true;
    }
    return false;
}


void Character::heal(int hp){
    if(cur_hp == max_hp){
        cout << "You are already at full health." << endl << endl;
    } else if(max_hp - cur_hp > hp){
        cur_hp += hp;
        cout << "You have regained " << hp << " hitpoints, bringing your total to " << cur_hp << "/" << max_hp << "." << endl << endl;
    } else {
        cur_hp = max_hp;
        cout << "Your hitpoints have been fully restored to their maximum value (" << cur_hp << "/" << max_hp << ")." << endl << endl;
    }
}




