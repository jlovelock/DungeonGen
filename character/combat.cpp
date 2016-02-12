
#include <character.h>
using namespace std;


int Character::AC(){
    if((main_hand && main_hand->name() == "shield") || (off_hand && off_hand->name() == "shield"))
        return _AC+2;
    else
        return _AC;
}

Weapon* Character::weapon_select(Character* target){
        if(is_monster) return weapons[0]; //TODO multi-weapon support for monsters??

        map<string, Weapon*> choice_matrix;

        cout << "Which weapon do you want to use?:" << endl;

        while(true) {
            for(vector<Weapon*>::iterator it = weapons.begin(); it != weapons.end(); ++it) {
                if(distance_to(target) < (*it)->range()){
                    choice_matrix[(*it)->name()] = *it;
                    cout << "\t - " << (*it)->name();
                    if(distance_to(target) >= 10 && (*it)->is_thrown())
                        cout << " (thrown)";
                    cout << endl;
                }
                else {
                    cout << "\t(" << (*it)->name() << " out of range.)" << endl;
                }
            }

            cout << endl << ">> ";
            string choice;
            getline(cin, choice);
            map<string, Weapon*>::iterator w = choice_matrix.find(choice);
            if(w == choice_matrix.end()) {
                cout << "Invalid selection. Your choices are as follows: " << endl;
            } else {
                cout << choice_matrix.at(choice)->name() << " selected." << endl;
                return choice_matrix.at(choice);
            }
        }
}

void Character::attack(Character* opponent, int attack_roll, int dmg){
    if(!opponent->is_alive()){
        cout << "Uh, he's already dead, but go right ahead if you really want to." << endl << endl;
        return;
    }
    if(attack_roll >= opponent->AC()){
        if(!is_monster){
            cout << "Your attack connects, dealing " << dmg << " points of damage to the " << opponent->PC_class << "." << endl;
            opponent->take_damage(dmg);
            if(!opponent->is_alive()){
                ///TODO: handle the rest of the cleanup. Delete opponent, set monster in room to false, etc.
                xp += opponent->xp;
                cout << "You have gained " << opponent->xp << " experience points." << endl << endl;
                if(xp > next_levelup()) levelup();
                in_melee = false;
            }
        } else {
            cout << "The " << PC_class << "'s weapon slams into your side, dealing " << dmg << " points of damage." << endl;
            opponent->take_damage(dmg);
        }
    } else {
        if(is_monster) cout << "His "; else cout << "Your ";
        cout << "blow fails to connect." << endl << endl;
    }


}

/*
 * Todos:
 *  - Enforce weapon ranges / character locations
 *  - Distinguish between melee / ranged / etc in flavour
 */
void Character::generic_attack(Character* opponent){
    Weapon* w;
    if(main_hand->is_weapon()) w = (Weapon*)main_hand;
    else if(off_hand->is_weapon()) w = (Weapon*)off_hand;
    else w = weapon_select(opponent);

    if(is_monster) cout << "The " << PC_class << " attacks you with his " << w->name() << " drawn." << endl;
    else cout << "You attack the " << opponent->PC_class << " with your " << w->name() << " drawn." << endl;

    attack(opponent, attack_roll(w), damage(w));
}


//void Character::melee_attack(Character* opponent){
//    Weapon* w = weapon_select(opponent);
//    if(in_melee == false){
//        if(is_monster) cout << "The " << PC_class << " charges you from across the room with his " << w.name() << " drawn." << endl;
//        else cout << "You charge forward to meet the " << opponent->PC_class << " with your " << w.name() << " drawn." << endl;
//    }
//    attack(opponent, attack_roll(w), damage(w));
//    in_melee = true;
//    opponent->in_melee = true; ///TODO FIXME if you attack at range first and melee second, it doesn't set your flag to "in melee", even though this line should have...
//}
//
//void Character::ranged_attack(Character* opponent){
//    Weapon w = weapon_select(opponent);
//    if(!is_monster) cout << "You draw your " << w.name() << " and prepare to strike." << endl;
//    else cout << "The " << PC_class << " draws a " << w.name() << " and readies a shot." << endl;
//    attack(opponent, attack_roll(w), damage(w));
//    in_melee = false;
//    opponent->in_melee = false;
//}

void Character::take_damage(int dmg){
    ///TODO make this whole section more flavourful!
    if(is_monster){
        if(cur_hp > dmg){
            cur_hp -= dmg;
            if((float)cur_hp/max_hp > 0.5) cout << "He still looks ready to fight." << endl << endl;
            else if((float)cur_hp/max_hp > 0.25) cout << "He's not in very good shape." << endl << endl;
            else cout << "He's barely alive at this point." << endl << endl;
        } else {
            cur_hp = 0;
            cout << "He collapses on the ground, dead." << endl;
        }
        return;
    }

    if(temp_hp > 0){
        if(temp_hp >= dmg){
            temp_hp -= dmg;
            cout << "Fortunately, your temporary hitpoints absorb the entire blow. (You have " << temp_hp << " temp hp remaining)." << endl;
            return;
        } else {
            cout << "Fortunately, your temporary hitpoints absorb " << temp_hp << " points of damage, but ";
            dmg -= temp_hp;
            cout << dmg << " points of damage are still dealt directly to you. " << endl;
            cur_hp -= dmg;

            if(cur_hp > 0){
                cout << "You have " << cur_hp << "/" << max_hp << " hitpoints remaining." << endl << endl;
                return;
            } else {
                ///TODO implement death saves, stabilizing, etc
                cur_hp = 0;
                cout << "You have died." << endl << endl;
                return;
            }
        }
    } else {
        if(dmg < cur_hp){
            cur_hp -= dmg;
            cout << "You have " << cur_hp << "/" << max_hp << " hitpoints remaining." << endl << endl;
            return;
        } else { ///TODO same as above
            cur_hp = 0;
            cout << "You have died." << endl << endl;
            return;
        }
    }
}


