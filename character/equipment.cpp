
#include <character.h>
#include <objects.h>
#include <weapon.h>
#include <pc.h>

using namespace std;


Weapon* PlayerChar::weapon_select(Character* target){
        //if(is_monster) return weapons[0]; //TODO multi-weapon support for monsters??

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

            cout << endl;
            string choice;
            read(choice);
            map<string, Weapon*>::iterator w = choice_matrix.find(choice);
            if(w == choice_matrix.end()) {
                cout << "Invalid selection. Your choices are as follows: " << endl;
            } else {
                cout << choice_matrix.at(choice)->name() << " selected." << endl;
                equip(choice_matrix.at(choice));
                return choice_matrix.at(choice);
            }
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

bool Character::equip(Object* o, bool equip_to_offhand){
    string input;

    if(o->is_equipped_to(this)){
        cout << "You're already wielding the " << o->name();
        if(!o->is_two_handed()){
            if(main_hand && main_hand->name() == o->name()){
                cout << " in your main hand." << endl;
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
                cout << " in your off hand. " << endl;
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
            cout << ", or cancel?" << endl;

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

