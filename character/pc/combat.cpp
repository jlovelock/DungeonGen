
#include <pc.h>
#include <weapon.h>

using namespace std;


/*
 * Todos:
 *  - Distinguish between melee / ranged / etc in flavour
 */
void PlayerChar::generic_attack(Character* opponent){
    Weapon* w;
    if(main_hand->is_weapon()) w = (Weapon*)main_hand;
    else if(off_hand->is_weapon()) w = (Weapon*)off_hand;
    else w = weapon_select(opponent);

    if(!w->is_ranged() && !in_melee_with(opponent)){
        close_with(opponent);
    }
    ///@TODO add thrown weapon support

    if(!w->is_ranged() && !in_melee_with(opponent)){
        weapon_select(opponent);
    }

    int atk;
    bool has_advantage = false, has_disadvantage = false;

    cout << "You attack the " << opponent->full_name() << " with your " << w->name() << " drawn." << endl;
    if(is("blinded")) {
        cout << "Blind and unable to see, you are forced to strike out randomly." << endl;
        has_disadvantage = true;
    } else if(opponent->is("blinded")){
        cout << "Blinded, the " << opponent->name() << " can't dodge out of the way of your attack nearly as easily." << endl;
        has_advantage = true;
    } else if(w->is_ranged() && in_melee_with(opponent)){
        cout << "However, you struggle to fire the " << w->name() << " from melee." << endl;
        has_disadvantage = true;
    }

    if(has_advantage && !has_disadvantage)
        atk = w->attack_roll(this, "advantage");
    else if(has_disadvantage && !has_advantage)
        atk = w->attack_roll(this, "disadvantage");
    else
        atk = w->attack_roll(this);

    if(atk == 20){
        cout << "Critical hit!" << endl;
        attack(opponent, atk, damage(w, "crit"));
    } else
        attack(opponent, atk, damage(w));
}

void PlayerChar::action_on_kill(Character* opponent){
    ///TODO: handle the rest of the cleanup. Delete opponent, set monster in room to false, etc.
    xp += opponent->get_xp();
    cout << "You have gained " << opponent->get_xp() << " experience points." << endl << endl;
    if(xp > next_levelup()) levelup();
    in_melee = false;
}

void PlayerChar::attack(Character* opponent, int attack_roll, int dmg){
    if(attack_roll >= opponent->AC() || attack_roll == 20){
        cout << "Your attack connects, dealing " << dmg << " points of damage to the " << opponent->full_name() << "." << endl;
        opponent->take_damage(dmg);

        if(!opponent->is_alive()) action_on_kill(opponent);

    } else {
        cout << "Your blow fails to connect." << endl << endl;
    }
}
