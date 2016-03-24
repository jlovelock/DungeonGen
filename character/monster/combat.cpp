
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

    if(!w->is_ranged() && !in_melee_with(opponent)){
        cout << "The " << full_name() << " begins to move toward you, but is still too far away to attack." << endl;
        return;
    }

    int atk;
    bool has_advantage = false, has_disadvantage = false;

    cout << "The " << full_name() << " lashes out with a " << w->name() << "." << endl;
    if(is("blinded")){
        cout << "Blinded, he can't target you nearly as effectively." << endl;
        has_disadvantage = true;
    } else if(opponent->is("blinded")){
        cout << "Unable to see your opponent lashing out with their " << w->name() << ", you struggle to avoid the attack." << endl;
        has_advantage = true;
    } else if(w->is_ranged() && in_melee_with(opponent)){
        cout << "However, he struggles to fire his " << w->name() << " from melee." << endl;
        has_disadvantage = true;
    }

    if(has_advantage && !has_disadvantage)
        atk = w->attack_roll(this, "advantage");
    else if(has_disadvantage && !has_advantage)
        atk = w->attack_roll(this, "disadvantage");
    else
        atk = w->attack_roll(this);

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
        w->action_on_hit(opponent, this);
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
