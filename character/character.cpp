
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

using namespace std;


Character::~Character(){
    for(vector<Weapon*>::iterator it = weapons.begin(); it != weapons.end(); ++it){
        delete *it;
    }
    weapons.clear();

    for(vector<Condition*>::iterator it = conditions.begin(); it != conditions.end(); ++it){
        delete *it;
    }
    conditions.clear();
}

bool Character::proficient_with(string weapon_type){
    for(vector<string>::iterator it = weapon_proficiencies.begin(); it != weapon_proficiencies.end(); ++it) {
        if (*it == weapon_type) return true;
    }
    return false;
}

int Character::skill_check(std::string skill){
    if(skill == "PERCEPTION" && is("blinded")) return 0;
    return d20()+skill_mods[skill];
}

int Character::attribute_chk(std::string att){
    return d20() + attribute_mods[att];
}

int Character::save(string stat){
    if(is("paralyzed") && (stat == "STR" || stat == "DEX")){
        cout << "Paralyzed and helpless, ";
        if(is_monster) cout << "he is"; else cout << "you are";
        cout << " entirely unable to avoid the effect." << endl;
        return 0;
    }
    return d20() + save_mods[stat];
}

void Character::end_of_turn_cleanup(bool quiet){
    if(DEBUG) cout << "$$ EOT: " << name() << "...";
    update_conditions(quiet);
    if(DEBUG) cout << "done" << endl;
}


bool Character::has_fighting_style(string style){
    for(vector<string>::iterator it = fighting_styles.begin(); it != fighting_styles.end(); ++it){
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




