
#include <stdlib.h>
#include <spells.h>
#include <defines.h>

using namespace std;


int Spell::damage(){
    int dmg = 0;
    for(int i = 0; i < num_damage_dice; i++){
        dmg += rand()%damage_die+1;
    }
    return dmg;
}

//eg. 1d4+5 fire
void Spell::set_dmg(string specifier){
    num_damage_dice = atoi(specifier.substr(0, specifier.find("d")).c_str());
    damage_die = atoi(specifier.substr(specifier.find("d")+1,specifier.find("+")-specifier.find("d")-1).c_str());
    damage_mod = atoi(specifier.substr(specifier.find("+")+1, specifier.find(" ")-specifier.find("+")-1).c_str());
    damage_type = specifier.substr(specifier.find(" ")+1);
}

Spell::Spell(string specifier){
    clr();
    set_dmg(specifier);
}

void Spell::clr(){
    name = "";
    level = 0;
    range = 10; ///@TODO FIXME
    duration = 0;
    casting_time = 0;
    attack_roll_required = false;
    save_negates = false;
    save_half = false;
    save_stat = "";
    num_damage_dice = 0;
    damage_die = 0;
    damage_mod = 0;
    damage_type = "";
    save_DC = 0;
}

Spell::Spell(){
    clr();
}

Spell::Spell(int lvl){
    if(lvl == 0) fire_bolt();
    else if(lvl == 1) magic_missile();
    else if(lvl == 2) scorching_ray();

}

///TODO should be able to choose different targets for each dart
void Spell::magic_missile(){
    name = "magic missile";
    level = 1;
    range = 120;
    duration = 0;
    casting_time = ACTION;

    attack_roll_required = false;
    save_negates = false;
    save_half = false;
    save_stat = "";

    num_damage_dice = 3;
    damage_die = 4;
    damage_mod = 3;
    damage_type = "force";
}

void Spell::fire_bolt(){
    name = "fire bolt";
    level = 0;
    range = 120;
    duration = 0;
    casting_time = ACTION;

    attack_roll_required = true;
    save_negates = false;
    save_half = false;
    save_stat = "";

    num_damage_dice = 1;
    damage_die = 10;
    damage_mod = 0;
    damage_type = "fire";
}

///TODO separate attack rolls for each ray
void Spell::scorching_ray(){
    name = "scorching ray";
    level = 2;
    range = 120;
    duration = 0;
    casting_time = ACTION;

    attack_roll_required = true;
    save_negates = false;
    save_half = false;
    save_stat = "";

    num_damage_dice = 6;
    damage_die = 6;
    damage_mod = 0;
    damage_type = "fire";
}
