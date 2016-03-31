
#include <character.h>
#include <objects.h>
#include <weapon.h>
#include <pc.h>
#include <inventory.h>

using namespace std;


Weapon* PlayerChar::weapon_select(Character* target){
        //if(is_monster) return weapons[0]; //TODO multi-weapon support for monsters??

    Weapon* w = inventory->weapon_select(distance_to(target));
    if(!w) return NULL;
    else {
        equip(w);
        return w;
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


void Character::identify_items(vector<Object*> v, bool& item_identified){
    for(auto it = v.begin(); it != v.end(); ++it){
        if(!(*it)->is_identified() && attribute_chk("INT") > LOOT_IDENTIFY_DC){
            if(!item_identified) {
                cout << "You are able to identify the following:" << endl;
                item_identified = true;
            }
            cout << "\t- ";
            (*it)->identify();
        }
    }
}

void Character::identify_items(){
    if(!inventory->has_unidentified_items()) return;

    bool item_identified = false;
    cout << "As you rest, you look over the items you have gathered more closely." << endl;

    identify_items(inventory->scroll_obj_vector(), item_identified);
    identify_items(inventory->potion_obj_vector(), item_identified);
    identify_items(inventory->weapon_obj_vector(), item_identified);
    identify_items(inventory->misc_obj_vector(), item_identified);

    if(!item_identified) cout << "Unfortunately, you are unable to identify anything." << endl;
    cout << endl;
}

bool Character::is_encumbered(){
    return inventory->weight() > 5*attribute_scores["STR"];
}

bool Character::is_heavily_encumbered(){
    return inventory->weight() > 10*attribute_scores["STR"];
}

bool Character::is_overencumbered(){
    return inventory->weight() > 15*attribute_scores["STR"];
}
