
#include <iostream>
#include <string.h>
#include <vector>

#include <weapon.h>
#include <monster.h>
#include <spells.h>

using namespace std;


Monster::Monster(double CR) : Character(){
    ///TODO add more monster options, random roll for one of the appropriate CR
    is_monster = true;
    searched = false;
    prof = 2;

    int n = rand()%3;
    switch(n){
        case 0: spawn_giant_rat(1); break;
        case 1: spawn_cultist(1); break;
        case 2: spawn_poisonous_snake(1); break;
    }
    cur_hp = max_hp;
    temp_hp = 0;
}

///TODO include keen senses, darkvision
void Monster::spawn_giant_rat(int group_size){
    _AC = 12;
    max_hp = d6()+d6();
    speed = 30;

    if(group_size == 1) xp = 38;
    else if(group_size == 2) xp = 50;

    set_attributes(7,15,11,2,10,4);

    Weapon* w = new Weapon("bite", "4/1d4+2 piercing");
    weapons.push_back(w);
    main_hand = w;
    off_hand = NULL;

    _short_name = "rat";
    _full_name = "giant rat";
    race = "rat";

    features.push_back("pack tactics");
}

//group size represents how many of this monster are in the same room
///TODO: instead of group_size (which assumes identical monsters), have another function
///  handle the monster spawn and therefore set the xp values
void Monster::spawn_cultist(int group_size){
    _AC = 12;
    max_hp = d8()+d8();
    speed = 30;

    if(group_size == 1) xp = 38;
    else if(group_size == 2) xp = 50;

    set_attributes(11,12,10,10,11,10);
    train("DECEPTION");
    train("RELIGION");

    Weapon* w = new Weapon("scimitar","3/1d6+1 piercing");
    weapons.push_back(w);
    main_hand = w;
    off_hand = NULL;

    _short_name = "cultist";
    _full_name = "cultist";
    race = "human"; //arbitrary
}

void Monster::spawn_poisonous_snake(int group_size){
    _AC = 13;
    max_hp = d4();
    speed = 30;
    ///TODO add swim speed 30

    if(group_size == 1) xp = 38;
    else if(group_size == 2) xp = 50;

    set_attributes(1, 16, 11, 1, 10, 3);

    Weapon* w = new Weapon("bite","5/1d0+1 piercing"); //yes, 1d0. deals 1 dmg always (base).
    w->effect_on_hit = new Spell("2d4+0 poison");
    w->effect_on_hit->name = "venom";
    w->effect_on_hit->save_half = true;
    w->effect_on_hit->save_DC = 10;
    w->effect_on_hit->save_stat = "CON";
    weapons.push_back(w);
    main_hand = w;
    off_hand = NULL;

    _short_name = "snake";
    _full_name = "poisonous snake";
    race = "snake"; //arbitrary
}
