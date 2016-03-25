
#include <spells.h>
#include <defines.h>
#include <conditions.h>

using namespace std;

///TODO separate attack rolls for each ray
void Spell::scorching_ray(){
    _name = "scorching ray";
    level = 2;
    range = 120;

    attack_roll_required = true;

    num_damage_dice = 6;
    damage_die = 6;
    damage_type = "fire";
}

void Spell::blindness(){
    _name = "blindness";
    level = 2;
    range = 30;

    // Note: save occurs at the end of each of the affected creature's turns.
    condition = new Condition("blinded", 10, "CON", save_DC, true);
}

void Spell::hold_person(){
    _name = "hold person";
    level = 2;
    range = 60;
    concentration = true;

    // Note: save occurs at the end of each of the affected creature's turns.
    condition = new Condition("paralyzed", 10, "WIS", save_DC, true);
}
