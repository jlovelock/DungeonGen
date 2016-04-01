
#include <pc.h>
#include <weapon.h>
#include <conditions.h>

using namespace std;


/*
 * Todos:
 *  - Distinguish between melee / ranged / etc in flavour
 */
void PlayerChar::generic_attack(Character* opponent){
    Weapon* w;
    if(main_hand && main_hand->is_weapon()) w = (Weapon*)main_hand;
    else if(off_hand && off_hand->is_weapon()) w = (Weapon*)off_hand;
    else w = NULL;

    /// TODO FIXME: should only close as far as you need to get in range of the weapon you want to use.
    if(!w || (!w->is_ranged() && !in_melee_with(opponent))){
        close_with(opponent);
    }
    if(!w) w = weapon_select(opponent);
    if(!w || (!w->is_ranged() && !in_melee_with(opponent))) return;

    ///@TODO add thrown weapon support

    int atk;
    bool has_advantage = false, has_disadvantage = false;

    cout << "You attack the " << opponent->full_name() << " with your " << w->name() << " drawn." << endl;
    if(is("blinded")) {
        cout << "Blind and unable to see, you are forced to strike out randomly." << endl;
        has_disadvantage = true;
    } if(opponent->is("blinded")){
        cout << "Blinded, the " << opponent->name() << " can't dodge out of the way of your attack nearly as easily." << endl;
        has_advantage = true;
    } if(w->is_ranged() && in_melee_with(opponent)){
        cout << "However, you struggle to fire the " << w->name() << " from melee." << endl;
        has_disadvantage = true;
    } if(opponent->is("paralyzed")){
        cout << "Paralyzed, he can't stop you from lining up a devastating blow!" << endl;
        has_advantage = true;
    } if(is_heavily_encumbered()){
        cout << "However, you struggle to attack while carrying such a heavy pack." << endl;
        has_disadvantage = true;
    }

    if(has_advantage && !has_disadvantage)
        atk = w->attack_roll(this, "advantage");
    else if(has_disadvantage && !has_advantage)
        atk = w->attack_roll(this, "disadvantage");
    else
        atk = w->attack_roll(this);

    int dmg;
    if(atk == 20 || (opponent->is("paralyzed") && in_melee_with(opponent))){
        cout << "Critical hit!" << endl;
        dmg = damage(w, "crit");
    } else {
        dmg = damage(w);
    }
    opponent->adjust_for_resistances(dmg, w->get_dtype());
    attack(opponent, atk, dmg);
}

void PlayerChar::action_on_kill(Character* opponent){
    ///TODO: handle the rest of the cleanup. Delete opponent, set monster in room to false, etc.
    xp += opponent->get_xp();
    cout << "You have gained " << opponent->get_xp() << " experience points." << endl << endl;
    if(xp > next_levelup()) levelup();
    in_melee = false;

    //end opponent's concentration effects here
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
