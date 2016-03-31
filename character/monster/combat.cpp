
#include <monster.h>
#include <weapon.h>
#include <spells.h>

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

    if(!w->is_ranged() && !in_melee_with(opponent)) return;

    int atk;
    bool has_advantage = false, has_disadvantage = false;

    cout << "The " << full_name() << " lashes out with a " << w->name() << "." << endl;
    if(is("blinded")){
        cout << "Blinded, he can't target you nearly as effectively." << endl;
        has_disadvantage = true;
    } if(opponent->is("blinded")){
        cout << "Unable to see your opponent lashing out with their " << w->name() << ", you struggle to avoid the attack." << endl;
        has_advantage = true;
    } if(w->is_ranged() && in_melee_with(opponent)){
        cout << "However, he struggles to fire his " << w->name() << " from melee." << endl;
        has_disadvantage = true;
    } if(opponent->is("paralyzed")){
        cout << "Paralyzed, you can't stop him from lining up a devastating blow!" << endl;
        has_advantage = true;
    }

    if(has_advantage && !has_disadvantage)
        atk = w->attack_roll(this, "advantage");
    else if(has_disadvantage && !has_advantage)
        atk = w->attack_roll(this, "disadvantage");
    else
        atk = w->attack_roll(this);

    int dmg = 0;
    if(atk == 20 || (opponent->is("paralyzed") && in_melee_with(opponent))){
        cout << "Critical hit!" << endl;
        dmg = damage(w, "crit");
    } else if(atk >= opponent->AC()){
        dmg = damage(w);
    } else {
        cout << "His blow fails to connect." << endl << endl;
    }

    if(dmg > 0){
        opponent->adjust_for_resistances(dmg, w->get_dtype());
        cout << "The " << full_name() << "'s " << main_hand->name() << " slams into your side, dealing " << dmg << " points of " << w->get_dtype() << " damage." << endl;
        opponent->take_damage(dmg);
        w->action_on_hit(opponent, this);

        if(in_melee_with(opponent) && opponent->is("enchanted with armor of agathys")){
            cout << "As the " << name() << "'s attack hits you, your armor of agathys hits back, chilling him for 5 points of cold damage." << endl;
            int x = 5;
            adjust_for_resistances(x, "cold");
            take_damage(x);
            if(!opponent->has_temp_hp()){
                opponent->remove_condition("enchanted with armor of agathys");
            }
            if(!is_alive()) opponent->action_on_kill(this);
        }
    }
}

///@TODO
void Monster::action_on_kill(Character* opponent){
}

void Monster::attack(Character* opponent, int attack_roll, int dmg){
    if(attack_roll >= opponent->AC() || attack_roll == 20){
        cout << "The " << full_name() << "'s " << main_hand->name() << " slams into your side, dealing " << dmg << " points of damage." << endl;
        opponent->take_damage(dmg);
//        cast(effect_on_hit, opponent);
    } else {
        cout << "His blow fails to connect." << endl << endl;
    }
}
