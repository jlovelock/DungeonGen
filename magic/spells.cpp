
#include <stdlib.h>
#include <spells.h>
#include <defines.h>
#include <conditions.h>
#include <character.h>

using namespace std;

Spell::~Spell(){
    if(condition) delete condition;
}


int Spell::damage(){
    int dmg = 0;
    for(int i = 0; i < num_damage_dice; i++){
        dmg += rand()%damage_die+1;
    }
    return dmg;
}

void Spell::cast(Character* caster, Character* target){

    bool is_PC = !caster->is_monster;
    int dmg = damage();

    // specifics
    if(_name == "hold person" && target->race != "humanoid"){
        cout << "The spell fails: hold PERSON can't affect a " << target->race << "." << endl;
        return;
    }

    // Check range; -4 fudge factor
    if(caster->distance_to(target) - 4 > range){
        cout << "You're too far away to do that: " << _name << " only has a range of " << range << " feet." << endl;
        //cout << "You are << sqrt(pow(xPos-target->xPos, 2)+pow(yPos-target->yPos, 2)) << " feet away from the " << target->full_name() << "." << endl;
        return;
    }

    // Effect negated
    if( ( attack_roll_required && caster->spell_attack() < target->AC()  ) ||
        ( save_negates         && target->saving_throw(this) >= save_DC  ) )  {
        if(is_PC) cout << "Unfortunately, the " << target->full_name() << " manages to avoid the effect." << endl;
        else      cout << "Luckily, you manage to avoid the " << target->full_name() << "'s " << _name << "." << endl;
        return;
    }

    // Save for half damage
    if(save_half && target->saving_throw(this) >= caster->spell_save_DC()){
        dmg /= 2;
        if(is_PC) cout << "The " << target->full_name() << " avoids the brunt of the effect, but it still hits." << endl;
        else      cout << "You avoid the brunt of the " << caster->full_name() << "'s " << _name << " effect, but it still hits." << endl;
    }

    // Deal damage
    if(dmg > 0) {
        target->adjust_for_resistances(dmg, damage_type);
        if(is_PC) cout << "Your " << _name << " hits the " << target->full_name() << " for " << dmg << " points of " << damage_type << " damage." << endl;
        else      cout << "The " << caster->full_name() << "'s " << _name << " deals you " << dmg << " points of " << damage_type << " damage." << endl;
        target->take_damage(dmg);
    }

    // Apply conditions
    if(condition) {
        condition->apply(target, caster);
    }
    if(concentration) caster->concentrate_on(this);

    // Check specific cases
    if(_name == "armor of agathys"){
        target->temp_hp += 5*level;
    }

    cout << endl;

    if(!target->is_alive())
        caster->action_on_kill(target);
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
    _name = "";
    level = 0;
    range = 10; ///TODO FIXME. Default should be adjacency.
    casting_time = ACTION;
    concentration = false;
    beneficial = false;
    attack_roll_required = false;
    save_negates = false;
    save_half = false;
    save_stat = "";
    save_DC = 0;
    num_damage_dice = 0;
    damage_die = 0;
    damage_mod = 0;
    damage_type = "";
    condition = NULL;
    condition_save = false;
    condition_save_stat = "";
}

Spell::Spell(){
    clr();
}

// random scroll of the specified level
Spell::Spell(int lvl){
    clr();
    save_DC = scroll_save_DC(lvl);

    int x;
    switch(lvl){
        case 0:
            x = rand() % 3;
            switch(x){
                case 0: fire_bolt(); return;
                case 1: poison_spray(); return;
                case 2: blade_ward(); return;
            }

        case 1:
            x = rand() % 4;
            switch(x){
                case 0: magic_missile(); return;
                case 1: inflict_wounds(); return;
                case 2: longstrider(); return;
                case 3: armor_of_agathys(); return;
            }

        case 2:
            x = rand() % 3;

            switch(x){
                case 0: scorching_ray(); return;
                case 1: blindness(); return;
                case 2: hold_person(); return;
            }
        }
}

