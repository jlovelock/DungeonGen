
#include <spells.h>
#include <defines.h>
#include <conditions.h>

using namespace std;

///TODO should be able to choose different targets for each dart
void Spell::magic_missile(){
    _name = "magic missile";
    level = 1;
    range = 120;

    num_damage_dice = 3;
    damage_die = 4;
    damage_mod = 3;
    damage_type = "force";
}

void Spell::inflict_wounds(){
    _name = "inflict wounds";
    level = 1;
    range = 5;

    attack_roll_required = true;

    num_damage_dice = 3;
    damage_die = 10;
    damage_type = "necrotic";
}

void Spell::longstrider(){
    _name = "longstrider";
    level = 1;
    range = 5;

    condition = new Condition("buffed by longstrider", 600);
    beneficial = true;
}

void Spell::armor_of_agathys(){
    _name = "armor of agathys";
    level = 1;
    range = 0;

    condition = new Condition("enchanted with armor of agathys", 600);
    beneficial = true;
}
