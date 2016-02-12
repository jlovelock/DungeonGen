#include <spells.h>
#include <character.h>
#include <defines.h>



int Spell::damage(){
    int dmg = 0;
    for(int i = 0; i < num_damage_dice; i++){
        dmg += rand()%damage_die+1;
    }
    return dmg;
}

///TODO get some random tables going
Spell::Spell(){
    fire_bolt();
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
    save_allowed = false;
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
    save_allowed = false;
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
    save_allowed = false;
    save_stat = "";

    num_damage_dice = 6;
    damage_die = 6;
    damage_mod = 0;
    damage_type = "fire";
}
