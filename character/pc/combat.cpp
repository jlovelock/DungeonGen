
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

    cout << "You attack the " << opponent->full_name() << " with your " << w->name() << " drawn." << endl;

    int atk;
    if(w->is_ranged() && in_melee_with(opponent)){
        cout << "However, you struggle to fire the " << w->name() << " from melee." << endl;
        atk = w->attack_roll(this, "disadvantage");
    } else {
        atk = w->attack_roll(this);
    }

    if(atk == 20)
        attack(opponent, atk, damage(w, "crit"));
    else
        attack(opponent, atk, damage(w));
}

void PlayerChar::attack(Character* opponent, int attack_roll, int dmg){
    if(!opponent->is_alive()){
        cout << "Uh, he's already dead, but go right ahead if you really want to." << endl << endl;
        return;
    }

    if(attack_roll >= opponent->AC() || attack_roll == 20){
        cout << "Your attack connects, dealing " << dmg << " points of damage to the " << opponent->full_name() << "." << endl;
        if(attack_roll == 20) cout << "It's a critical hit!" << endl;
        opponent->take_damage(dmg);

        if(!opponent->is_alive()){
            ///TODO: handle the rest of the cleanup. Delete opponent, set monster in room to false, etc.
            xp += opponent->get_xp();
            cout << "You have gained " << opponent->get_xp() << " experience points." << endl << endl;
            if(xp > next_levelup()) levelup();
            in_melee = false;
        }

    } else {
        cout << "Your blow fails to connect." << endl << endl;
    }
}
