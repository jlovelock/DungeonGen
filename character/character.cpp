
#include <iostream>
#include <string>
#include <limits.h>
#include <math.h>
#include <character.h>

using namespace std;

void Character::set_position(int x, int y){
    xPos = x;
    yPos = y;
}

void Character::move_position(int x, int y){
    xPos += x;
    yPos += y;
}

pair<int,int> Character::get_position(){
    return make_pair(xPos, yPos);
}

bool Character::find_secret_door(Door* d){
    if (d && d->is_secret() && skill_check("INVESTIGATION") > SECRET_DOOR_DC){
        d->find_door();
        return true;
    }
    return false;
}

bool Character::search_monster(bool print_err_msgs){
    if(is_alive()){
        if(print_err_msgs) cout << "You can't do that while it's still alive!" << endl;
        return false;
    } else if (searched){
        if(print_err_msgs) cout << "You've already done that." << endl;
        return false;
    } else {
        searched = true;
        return true;
    }
}

///@TODO make sure weapon is always in main hand
string Character::equipped_weapon_type(){
    if(main_hand && main_hand->is_two_handed()) return "two handed";
    else if(main_hand && main_hand->is_weapon()){
        if(off_hand && off_hand->is_weapon()) return "dual";
        else return "one handed";
    } else return "none";
}

bool Character::has_fighting_style(string style){
    for(vector<string>::iterator it = fighting_styles.begin(); it != fighting_styles.end(); ++it){
        if((*it) == style) return true;
    }
    return false;
}

bool Character::equip(Object* o, bool equip_to_offhand){
    string input;

    if(o->is_equipped_to(this)){
        cout << "You're already wielding the " << o->name();
        if(!o->is_two_handed()){
            if(main_hand && main_hand->name() == o->name()){
                cout << " in your main hand.";
                if(off_hand){
                    cout << "Switch with the " << off_hand->name() << " in your off hand? [y/n]" << endl;
                    read(input);
                    if(input == "y"){
                        Object* tmp = off_hand;
                        off_hand = main_hand;
                        main_hand = tmp;
                        cout << "Equipped " << main_hand->name() << " to your main hand and " << off_hand->name() << " to your off hand." << endl;
                    }
                    cout << "Switch to your main hand? [y/n]" << endl;
                    read(input);
                    if(input == "y"){
                        main_hand = off_hand;
                        off_hand = NULL;
                        cout << "Switched " << main_hand->name() << " to your main hand." << endl;
                    }
                }
            } else {
                cout << " in your off hand. ";
                if(main_hand) {
                    cout << "Switch with the " << main_hand->name() << " in your main hand? [y/n]" << endl;
                    read(input);
                    if(input == "y"){
                        Object* tmp = off_hand;
                        off_hand = main_hand;
                        main_hand = tmp;
                        cout << "Equipped " << main_hand->name() << " to your main hand and " << off_hand->name() << " to your off hand." << endl;
                    }
                }
            }
        } else { cout << "." << endl; }
        return true;
    }

    if(o->is_two_handed()){
        if(main_hand == NULL && off_hand == NULL){
            cout << o->name() << " equipped." << endl << endl;
            main_hand = o;
            off_hand = o;
            return true;
        }
        else {
            cout << "Unequip the ";
            if(main_hand)
                cout << main_hand->name();
            if(main_hand && off_hand && !main_hand->is_two_handed())
                cout << " and the ";
            if(off_hand && !off_hand->is_two_handed())
                cout << off_hand->name();
            cout << "? [y/n]" << endl;

            read(input);
            if(input == "y") {
                cout << o->name() << " equipped." << endl << endl;
                main_hand = o;
                off_hand = o;
                return true;
            }
            cout << endl;
            return false;
        }

    //One-handed
    } else {
        if(!off_hand && equip_to_offhand){
            cout << o->name() << " equipped (off hand)." << endl;
            off_hand = o;
            return true;
        }
        else if(!main_hand){
            cout << o->name() << " equipped (main hand)." << endl;
            main_hand = o;
            return true;
        }
        else if(main_hand && !off_hand) {
            cout << o->name() << " and " << main_hand->name() << " equipped (main hand)." << endl;
            cout << "Which in main hand? " << endl;
            while(true){
                read(input);
                if(input == o->name()){
                    off_hand = main_hand;
                    main_hand = o;
                    return true;
                } else if(input == main_hand->name()){
                    off_hand = o;
                    return true;
                } else {
                    cout << "Invalid entry, try again." << endl;
                }
            }
        }
        ///@TODO better wording here
        else {
            cout << "You must unequip something first. The ";
            if(main_hand)
                cout << main_hand->name();
            if(main_hand && off_hand && !main_hand->is_two_handed())
                cout << ", the ";
            if(off_hand && !off_hand->is_two_handed())
                cout << off_hand->name();
            cout << ", or cancel?" << endl << endl;

            while(true){
                read(input);
                if(input == main_hand->name()){
                    cout << main_hand->name() << " unequipped, and " << o->name() << " equipped." << endl << endl;
                    if(main_hand->is_two_handed())
                        off_hand = NULL;
                    main_hand = o;
                    if(o->is_two_handed())
                        off_hand = o;
                    return true;
                } else if(input == off_hand->name()){
                    cout << off_hand->name() << " unequipped, and " << o->name() << " equipped." << endl << endl;
                    off_hand = o;
                    return true;
                } else if(input == "cancel"){
                    cout << endl;
                    return false;
                } else {
                    cout << "Invalid entry, try again." << endl;
                }
            }
        }
    }
}

int Character::distance_to(Character* target){
    return sqrt(pow(xPos-target->xPos, 2)+pow(yPos-target->yPos, 2));
}

bool Character::within_range(Character* target, Spell* spell){
    if(distance_to(target) <= spell->range) return true;
    else return false;
}

void Character::heal(int hp){
    if(cur_hp == max_hp){
        cout << "You are already at full health." << endl << endl;
    } else if(max_hp - cur_hp >= hp){
        cur_hp += hp;
        cout << "You have regained " << hp << " hitpoints, bringing your total to " << cur_hp << "/" << max_hp << "." << endl << endl;
    } else {
        cur_hp = max_hp;
        cout << "Your hitpoints have been fully restored to their maximum value (" << cur_hp << "/" << max_hp << ")." << endl << endl;
    }
}

void Character::levelup(){
    level++;
    cout << "You have reached level " << level << "!" << endl;
    cout << "Wow, didn't expect you to make it this far -- I haven't actually programmed level 2 yet, but stay tuned =]" << endl << endl;
}

void Character::special_action(){
    //sanity check -- this function is just for PCs
    if(is_monster) return;

    cout << "Enter one of the following special action options, or cancel to return to the previous menu." << endl;
    if(second_wind_available){
        cout << "\t- secondwind [bonus action]: regain 1d10+" << level << " hitpoints as a bonus action" << endl;
    }

    cout << endl;
    string input;
    do {
        read(input);

        if(input == "secondwind"){
            if(!second_wind_available){
                cout << "You have not taken a short or long rest since you last used this feature. Please make another selection." << endl;
                continue;
            }
            second_wind_available = false;
            if(cur_hp == max_hp) cout << "You are already at full health!" << endl;
            else {
                heal(d10()+level);
                cout << "You may not reuse this feature until you complete a short or long rest." << endl << endl;
            }
            return;
        } else if(input == "cancel"){
            return;
        } else {
            cout << "Input not recognized. Try again?" << endl;
        }
    } while (true);

}

void Character::short_rest(){
    //sanity check -- this function is just for PCs
    if(is_monster) return;

    bool changed = false;
    if(second_wind_available == false){
        second_wind_available = true;
        cout << "You have regained the use of your Second Wind class feature." << endl;
    }
    if(hit_dice > 0){
        if(changed) cout << "Additionally, you"; else cout << "You";
        cout << " may spend any number of your remaining hit dice; each will restore 1d" << hit_die_size << "+" << attribute_mods["CON"] << " hp." << endl;
        cout << " You have " << hit_dice << " hit ";
        if(hit_dice == 1) cout << "die "; else cout << "dice ";
        cout << "remaining. How many would you like to spend?" << endl;

        int num;
        while(true){
            string tmp;
            read(tmp);
            num = atoi(tmp.c_str());
            if(num > hit_dice)
                cout << "You do not have that many hit dice remaining. Please re-enter a number." << endl;
            else
                break;
        }
        int restored_hp = 0;
        for(int i = 0; i < num; i++){
            restored_hp += rand()%hit_die_size+1+attribute_mods["CON"];
            hit_dice--;
        }

        if(restored_hp > 0){
            if(max_hp - cur_hp >= restored_hp){
                cout << "You have regained " << restored_hp << " hitpoints, bringing your total to " << cur_hp << "/" << max_hp << "." << endl;
                cout << "You have ";
                if(hit_dice == 0) cout << "no"; else cout << hit_dice;
                cout << " hit ";
                if(hit_dice == 1) cout << "die"; else cout << "dice";
                cout << " remaining." << endl << endl;
            } else {
                cur_hp = max_hp;
                cout << "You have fully restored your hitpoints to their maximum value (" << cur_hp << "/" << max_hp << ")." << endl;
                cout << "You have ";
                if(hit_dice == 0) cout << "no"; else cout << hit_dice;
                cout << " hit ";
                if(hit_dice == 1) cout << "die"; else cout << "dice";
                cout << " remaining." << endl << endl;
            }
        }

    } else cout << "You have no remaining hit dice to spend." << endl << endl;
}

int Character::next_levelup(){
    switch(level){
        case 1: return 300;
        case 2: return 900;
        case 3: return 2700;
        case 4: return 6500;
        case 5: return 14000;
        case 6: return 23000;
        case 7: return 34000;
        case 8: return 48000;
        case 9: return 64000;
        case 10: return 85000;
        case 11: return 100000;
        case 12: return 120000;
        case 13: return 140000;
        case 14: return 165000;
        case 15: return 195000;
        case 16: return 225000;
        case 17: return 265000;
        case 18: return 305000;
        case 19: return 355000;
        default: return INT_MAX;
    }
}
