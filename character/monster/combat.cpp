
#include <monster.h>
#include <weapon.h>

using namespace std;


/*
 * Todos:
 *  - Enforce weapon ranges / character locations
 *  - Distinguish between melee / ranged / etc in flavour
 */
void Monster::generic_attack(Character* opponent){
    ///@TODO opponents with multiple weapon options
    Weapon* w = (Weapon*)main_hand;

    if(!w->is_ranged() && !in_melee_with(opponent)){
        close_with(opponent);
    }
    ///@TODO add thrown weapon support

    if(!w->is_ranged() && !in_melee_with(opponent)){
        cout << "The " << full_name() << " begins to move toward you, but is still too far away to attack." << endl;
        return;
    }

    int atk;
    if(w->is_ranged() && in_melee_with(opponent)){
        cout << "The " << full_name() << " struggles to fire his " << w->name() << " from melee." << endl;
        atk = w->attack_roll(this, "disadvantage");
    } else {
        cout << "The " << full_name() << " lashes out with a " << w->name() << "." << endl;
        atk = w->attack_roll(this);
    }

    int dmg = 0;
    if(atk == 20){
        cout << "Critical hit!" << endl;
        dmg = damage(w, "crit");
    } else if(atk >= opponent->AC()){
        dmg = damage(w);
    } else {
        cout << "His blow fails to connect." << endl << endl;
    }

    if(dmg > 0){
        cout << "The " << full_name() << "'s " << main_hand->name() << " slams into your side, dealing " << dmg << " points of damage." << endl;
        opponent->take_damage(dmg);
        cast(w->effect_on_hit, opponent);
    }

//    if(atk == 20)
//        attack(opponent, atk, damage(w, "crit"));
//    else
//        attack(opponent, atk, damage(w));
}

///@TODO
void Monster::action_on_kill(Character* opponent){
}
//
void Monster::attack(Character* opponent, int attack_roll, int dmg){
    if(attack_roll >= opponent->AC() || attack_roll == 20){
        cout << "The " << full_name() << "'s " << main_hand->name() << " slams into your side, dealing " << dmg << " points of damage." << endl;
        if(attack_roll == 20) cout << "It's a critical hit!" << endl;
        opponent->take_damage(dmg);
//        cast(effect_on_hit, opponent);
    } else {
        cout << "His blow fails to connect." << endl << endl;
    }
}
