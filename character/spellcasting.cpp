
#include <math.h>

#include <character.h>
#include <spells.h>
#include <pc.h>

using namespace std;

///TODO add flavour
///TODO add support for enemy spellcasters (don't print the same messages based on is_enemy)
///TODO add support for non-damaging spells
void Character::cast(Spell* spell, Character* target){
    if(!spell) return;

    if(!within_range(target, spell)){
        cout << "You're too far away to do that: "
        << spell->name() << " only has a range of " << spell->range << " feet." << endl;
        //<< sqrt(pow(xPos-target->xPos, 2)+pow(yPos-target->yPos, 2)) << " feet away from the " << target->full_name() << "." << endl;

        return;
    }
    if(spell->attack_roll_required){
        if(spell_attack() > target->AC()){
            int damage = spell->damage();
            if(!is_monster)
                cout << "Your " << spell->name() << " hits the " << target->full_name() << " for " << damage << " points of " << spell->damage_type << " damage." << endl;
            else
                cout << "The " << full_name() << "'s " << spell->name() << " deals you " << damage << " points of " << spell->damage_type << " damage." << endl;
            target->take_damage(damage);
        } else {
            if(!is_monster)
                cout << "Unfortunately, the " << target->full_name() << " manages to avoid the effect." << endl;
            else
                cout << "Luckily, you manage to avoid the " << full_name() << "'s " << spell->name() << "." << endl;
        }
    }

    int damage = spell->damage();
    if(spell->save_negates && target->saving_throw(spell) >= spell_save_DC()){
        if(!is_monster)
            cout << "Unfortunately, the " << target->full_name() << " manages to avoid the effect." << endl;
        else
            cout << "Luckily, you manage to avoid the " << full_name() << "'s " << spell->name() << "." << endl;
    } else if(spell->save_half && target->saving_throw(spell) >= spell_save_DC()){
        damage /= 2;
        if(!is_monster)
            cout << "The " << target->full_name() << " avoids the brunt of the effect, but your " << spell->name() << " still hits him for " << damage << " points of " << spell->damage_type << " damage." << endl;
        else
            cout << "You avoid the brunt of the " << full_name() << "'s " << spell->name() << " effect, but it still deals you " << damage << " points of " << spell->damage_type << " damage." << endl;
        target->take_damage(damage);
    } else {
        if(!is_monster)
            cout << "Your " << spell->name() << " hits the " << target->full_name() << " for " << damage << " points of " << spell->damage_type << " damage." << endl;
        else
            cout << "The " << full_name() << "'s " << spell->name() << " hits deals you " << damage << " points of " << spell->damage_type << " damage." << endl;
        target->take_damage(damage);
    }

    if(!target->is_alive()) action_on_kill(target);
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
