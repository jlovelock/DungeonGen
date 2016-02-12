#include <math.h>
#include <character.h>

using namespace std;

///TODO add flavour
///TODO add support for enemy spellcasters (don't print the same messages based on is_enemy)
///TODO add support for non-damaging spells
void Character::cast(Spell* spell, Character* target){
    if(!within_range(target, spell)){
        cout << "You're too far away to do that: "
        << spell->name << " only has a range of " << spell->range << " feet, and you are "
        << sqrt(pow(xPos-target->xPos, 2)+pow(yPos-target->yPos, 2)) << " feet away from the " << target->PC_class << "." << endl;

        return;
    }
    if(spell->attack_roll_required){
        if(spell_attack() > target->AC()){
            int damage = spell->damage();
            cout << "Your " << spell->name << " hits the " << target->PC_class << " for " << damage << " points of " << spell->damage_type << " damage." << endl;
            target->take_damage(damage);
        } else {
            cout << "The " << target->PC_class << " manages to avoid the effect." << endl;
        }
    } else if(spell->save_allowed){
        if(target->saving_throw(spell) >= spell_save_DC()){
            cout << "The " << target->PC_class << " manages to avoid the effect." << endl;
        } else {
            int damage = spell->damage();
            cout << "Your " << spell->name << " hits the " << target->PC_class << " for " << damage << " points of " << spell->damage_type << " damage." << endl;
            target->take_damage(damage);
        }
    } else {
        int damage = spell->damage();
        cout << "Your " << spell->name << " hits the " << target->PC_class << " for " << damage << " points of " << spell->damage_type << " damage." << endl;
        target->take_damage(damage);
    }

    if(!target->is_alive()){
        ///TODO: handle the rest of the cleanup. Delete opponent, set monster in room to false, etc.
        xp += target->xp;
        cout << "You have gained " << target->xp << " experience points." << endl << endl;
        if(xp > next_levelup()) levelup();
        in_melee = false;
    }
}

int Character::saving_throw(Spell* spell){
    return save(spell->save_stat);
}

int Character::spell_attack(){
    return attribute_chk(casting_stat)+prof;
}

int Character::spell_save_DC(){
    return 8+prof+attribute_mods[casting_stat];
}
