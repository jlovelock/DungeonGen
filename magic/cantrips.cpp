
#include <spells.h>
#include <defines.h>

using namespace std;

///TODO damage scaling with caster level
void Spell::fire_bolt(){
    _name = "fire bolt";
    level = 0;
    range = 120;

    attack_roll_required = true;

    num_damage_dice = 1;
    damage_die = 10;
    damage_type = "fire";
}

///TODO damage scaling with caster level
void Spell::poison_spray(){
    _name = "poison spray";
    level = 0;
    range = 10;

    save_negates = true;
    save_stat = "CON";

    num_damage_dice = 1;
    damage_die = 12;
    damage_type = "poison";
}
