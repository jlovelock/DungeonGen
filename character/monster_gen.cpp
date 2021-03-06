
#include <iostream>
#include <string.h>
#include <vector>

#include <weapon.h>
#include <monster.h>

using namespace std;

string Monster::get_name(){
    return short_name;
}


///TODO include keen senses, darkvision
void Monster::spawn_giant_rat(int group_size){
    short_name = "rat";
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

    race = "rat";
    PC_class = "giant rat";
    pack_tactics = true;
}

//group size represents how many of this monster are in the same room
///TODO: instead of group_size (which assumes identical monsters), have another function
///  handle the monster spawn and therefore set the xp values
void Monster::spawn_cultist(int group_size){
    short_name = "cultist";
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

    PC_class = "cultist"; /// TODO rename this variable to something more reasonable? =P
    race = "human"; //arbitrary
    pack_tactics = false;
}

