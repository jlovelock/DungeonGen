
#include <iostream>
#include <dungeon.h>

using namespace std;


///(long-term) TODO: add the full tables here (by CR)
void Dungeon::rollIndividualTreasure(string monster_name){
    int x = d100();
    int reward;
    if(x < 31){
        reward = d6()+d6()+d6()+d6()+d6();
        cout << "The " << monster_name << " was carrying " << reward << " copper pieces." << endl;
        cp += reward;
    } else if(x < 61){
        reward = d6()+d6()+d6()+d6();
        cout << "The " << monster_name << " was carrying " << reward << " silver pieces." << endl;
        sp += reward;
    } else if(x < 71){
        reward = d6()+d6()+d6();
        cout << "The " << monster_name << " was carrying " << reward << " electrum pieces." << endl;
        ep += reward;
    } else if(x < 96){
        reward = d6()+d6()+d6();
        cout << "The " << monster_name << " was carrying " << reward << " gold pieces." << endl;
        gp += reward;
    } else {
        reward = d6();
        cout << "The " << monster_name << " was carrying " << reward << " platinum ";
        if(reward == 1) cout << "piece." << endl;
        else cout << "pieces." << endl;
        pp += reward;
    }
}


void Dungeon::rollTreasureHoard(){
    vector<Treasure*> hoard;

    int num_objects, value;
    string type;

    //Roll money
    int copper, silver, gold;
    copper = (d6()+d6()+d6()+d6()+d6()+d6())*100;
    silver = (d6()+d6()+d6())*100;
    gold = (d6()+d6())*10;
    cout << "You find " << copper << "cp, " << silver << "sp, and " << gold << "gp." << endl;
    cp += copper;
    sp += silver;
    gp += gold;

    int num_magic_items;
    char magic_item_table;

    //Roll item category/quantities
    ///TODO add magic items!
    int x = d100();
    if(x < 7) {
        cout << endl;
        return;
    } else if(x < 17){
        type = "gemstones";
        num_objects = d6()+d6();
        value = 10;
    } else if(x < 27){
        type = "art";
        num_objects = d4()+d4();
        value = 25;
    } else if(x < 37){
        type = "gemstones";
        num_objects = d6()+d6();
        value = 50;
    } else if(x < 45){
        type = "gemstones";
        num_objects = d6()+d6();
        value = 10;
        num_magic_items = d6();
        magic_item_table = 'A';
    } else if(x < 53){
        type = "art";
        num_objects = d4()+d4();
        value = 25;
        num_magic_items = d6();
        magic_item_table = 'A';
    } else if(x < 61){
        type = "gemstones";
        num_objects = d6()+d6();
        value = 50;
        num_magic_items = d6();
        magic_item_table = 'A';
    } else if(x < 66){
        type = "gemstones";
        num_objects = d6()+d6();
        value = 10;
    } else if(x < 71) {
        type = "art";
        num_objects = d4()+d4();
        value = 25;
    } else if(x < 76){
        type = "gemstones";
        num_objects = d6()+d6();
        value = 50;
    } else if(x < 79){
        type = "gemstones";
        num_objects = d6()+d6();
        value = 10;
    } else if(x < 81){
        type = "art";
        num_objects = d4()+d4();
        value = 25;
    } else if(x < 86){
        type = "gemstones";
        num_objects = d6()+d6();
        value = 50;
    } else if(x < 93){
        type = "art";
        num_objects = d4()+d4();
        value = 25;
    } else if(x < 98){
        type = "gemstones";
        num_objects = d6()+d6();
        value = 50;
    } else if(x < 100){
        type = "art";
        num_objects = d4()+d4();
        value = 25;
    } else {
        type = "gemstones";
        num_objects = d6()+d6();
        value = 50;
    }

    ///**********REMOVE ME AFTER
    num_magic_items = d6();
    magic_item_table = 'A';



    //Roll each mundane item
    for(int i = 0; i < num_objects; i++){
        Treasure* next = new Treasure (type, 1, value);
        bool found = false;
        for(vector<Treasure*>::iterator it = hoard.begin(); it != hoard.end(); ++it){
            if((*it)->name == next->name){
                delete next;
                found = true;
                (*it)->quantity++;
                break;
            }
        }
        if(!found) hoard.push_back(next);
    }

    //roll each magical item
    for(int i = 0; i < num_magic_items; i++){
        Treasure* next = new Treasure (magic_item_table);
        bool found = false;
        for(vector<Treasure*>::iterator it = hoard.begin(); it != hoard.end(); ++it){
            if((*it)->name == next->name){
                delete next;
                found = true;
                (*it)->quantity++;
                break;
            }
        }
        if(!found) hoard.push_back(next);
    }
    //Print loot table
    cout << "You have also received the following:" << endl;
    for(vector<Treasure*>::iterator it = hoard.begin(); it != hoard.end(); ++it){
        cout << "\t- " << (*it)->quantity << " " << (*it)->get_description() << "." << endl;
    }
    cout << endl << endl;


    //Merge duplicate items from hoard to dungeon loot / potion inventory
    bool flag = false;
    for(vector<Treasure*>::iterator h_it = hoard.begin(); h_it != hoard.end(); ++h_it){
        if(flag){
            flag = false;
            --h_it;
        }
        for(vector<Treasure*>::iterator l_it = loot.begin(); l_it != loot.end(); ++l_it){
            if((*h_it)->name == (*l_it)->name){
                (*l_it)->quantity += (*h_it)->quantity;
                delete *h_it;
                h_it = hoard.erase(h_it);
                flag = true;
                break;
            }
        }
        for(vector<Treasure*>::iterator p_it = potions.begin(); p_it != potions.end(); ++p_it){
            if((*h_it)->name == (*p_it)->name){
                (*p_it)->quantity += (*h_it)->quantity;
                delete *h_it;
                h_it = hoard.erase(h_it);
                flag = true;
                break;
            }
        }
        for(vector<Treasure*>::iterator s_it = scrolls.begin(); s_it != scrolls.end(); ++s_it){
            if((*h_it)->name == (*s_it)->name){
                (*s_it)->quantity += (*h_it)->quantity;
                delete *h_it;
                h_it = hoard.erase(h_it);
                flag = true;
                break;
            }
        }
    }

    //Transfer non-duplicate items from hoard to dungeon loot
    while(!hoard.empty()){
        if(hoard.back()->type == "potion")
            potions.push_back(hoard.back());
        else if(hoard.back()->type == "scroll")
            scrolls.push_back(hoard.back());
        else
            loot.push_back(hoard.back());
        hoard.pop_back();
    }
}

void Dungeon::identify_items(){
    bool first_item_identified = true;
    cout << "As you rest, you look over the items you have gathered more closely." << endl;
    for(vector<Treasure*>::iterator it = loot.begin(); it != loot.end(); ++it){
        if(!(*it)->identified && PC.attribute_chk("INT") > LOOT_IDENTIFY_DC){
            if(first_item_identified) cout << "You are able to identify the following:" << endl;
            first_item_identified = false;
            cout << "\t- Your " << (*it)->get_description();
            if((*it)->type=="art"){
                if((*it)->quantity == 1) cout << " is";
                else cout << " are";
            }
            else if((*it)->quantity == 1)
                cout << " is a " << (*it)->name;
            else
                cout << " are " << (*it)->name << "s";
            cout << " worth " << (*it)->value << "gp";
            if((*it)->quantity > 1) cout << " each";
            cout << "." << endl;
            (*it)->identified = true;
        }
    }

    for(vector<Treasure*>::iterator it = potions.begin(); it != potions.end(); ++it){
        if(!(*it)->identified && PC.skill_check("ARCANA") > POTION_IDENTIFY_DC){
            if(first_item_identified) cout << "Looking over the items you have gathered in more detail, you are able to identify the following:" << endl;
            first_item_identified = false;
            cout << "\t- Your " << (*it)->get_description();
            if((*it)->quantity == 1)
                cout << " is a potion of ";
            else
                cout << " are potions of ";
            cout << (*it)->name << "." << endl;
            (*it)->identified = true;
        }
    }

    for(vector<Treasure*>::iterator it = scrolls.begin(); it != scrolls.end(); ++it){
        if(!(*it)->identified && PC.skill_check("ARCANA") > SCROLL_IDENTIFY_DC){
            if(first_item_identified) cout << "Looking over the items you have gathered in more detail, you are able to identify the following:" << endl;
            first_item_identified = false;
            cout << "\t- " << (*it)->quantity << " of your scrolls";
            if((*it)->quantity == 1)
                cout << " is a scroll of ";
            else
                cout << " are scrolls of ";
            cout << (*it)->name << "." << endl;
            (*it)->identified = true;
        }
    }


    if(first_item_identified) cout << "Unfortunately, you are unable to identify anything." << endl;
    cout << endl;
}


void Dungeon::drink_potion(int index){
    potions.at(index)->quantity--;
    if(PC.cur_hp == PC.max_hp)
        cout << "Nothing appears to happen." << endl << endl;
    else
        PC.heal(potions.at(index)->healing_amount());

    if(potions.at(index)->quantity == 0){
        delete potions.at(index);
        potions.erase(potions.begin()+index);
    }
}

//returns true iff a spell was cast
bool Dungeon::cast_spell(){
    if(scrolls.empty()){
        cout << "You have no scrolls from which to cast." << endl << endl;
        return false;
    }
    if(!cur_room->has_monsters()){
        cout << "There are no enemies in the room to cast spells at." << endl << endl;
        return false;
    }

    bool flag = true;
    for(vector<Treasure*>::iterator it = scrolls.begin(); it != scrolls.end(); ++it){
        char idx = 'A';
        if((*it)->identified){
            if(flag){
                cout << "Choose which scroll you would like to cast, or enter 'cancel' to exit this menu." << endl;
                flag = false;
            }
            cout << "\t(" << idx << "): " << (*it)->get_description() << endl;
            ++idx;
        }
    }
    if(flag){
        cout << "You can't cast a scroll you haven't identified. You can attempt to identify them while you rest." << endl;
        return false;
    }

    cout << endl;
    char input;
    cin.get(input);

    for(vector<Treasure*>::iterator it = scrolls.begin(); it != scrolls.end(); ++it){
        char idx = 'A';
        if((*it)->identified){
            if(idx == input){
                PC.cast((*it)->spell, cur_room->get_active_monster_char());
                (*it)->quantity--;
                if((*it)->quantity == 0){
                    delete *it;
                    scrolls.erase(it);
                }
                return true;
            }
            ++idx;
        }
    }
    return false;

}


//returns true iff you drank a potion
bool Dungeon::drink_potion(){
    if(potions.empty()){
        cout << "You have no potions to drink!" << endl;
        return false;
    }
    string input;
    if(potions.size() == 1){
        if(potions.front()->identified) cout << "Drink the " << potions.front()->name << " potion? [y/n] (" << potions.front()->quantity << " in inventory)" << endl;
        else cout << "Drink the unidentified " << potions.front()->description << " potion? [y/n] (" << potions.front()->quantity << " in inventory)" << endl;


        read(input);
        if(input == "y"){
            drink_potion(0);
            return true;
        } else {
            cout << "Alright, what do you want to do instead?" << endl;
            return false;
        }

    } else {
        char inputChar;
        do {
            cout << "Which potion?" << endl;
            char idx = 'A';
            for(vector<Treasure*>::iterator it = potions.begin(); it != potions.end(); ++it){
                cout << "\t(" << idx << "): ";
                if((*it)->identified) cout << (*it)->name << " potion" << endl;
                else cout << "unidentified " << (*it)->description << " potion" << endl;
                idx++;
            }
            cout << endl;
            cin.get(inputChar);
        } while((unsigned)(inputChar - 'A') > potions.size());

        drink_potion(inputChar-'A');
        return true;
    }
}

//TODO check if item is already equipped, being careful of items with duplicate names (ie equipping a second offhand shortsword is OK)
void Dungeon::equip_item(string input){
    Object* to_equip = NULL;

    for(vector<Weapon*>::iterator it = PC.weapons.begin(); it != PC.weapons.end(); ++it){
        if(contains(input, (*it)->name())){
            to_equip = *it;
        }
    }

    for(vector<Object*>::iterator it = PC.objects.begin(); it != PC.objects.end(); ++it){
        if(contains(input, (*it)->name())){
            to_equip = *it;
        }
    }

    if(to_equip == NULL){
        cout << "Either you don't have that in your inventory, or it's not an equippable item." << endl;
    } else {
        PC.equip(to_equip);
    }
}
