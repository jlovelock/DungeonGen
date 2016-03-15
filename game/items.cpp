
#include <game.h>
#include <weapon.h>
#include <pc.h>

using namespace std;


void Game::print_inventory(){
    cout << "WEAPONS: " << endl;
    for(vector<Weapon*>::iterator it = PC->weapons.begin(); it != PC->weapons.end(); ++it){
        cout << "\t - " << (*it)->get_weapon_description(PC);
        if((*it)->is_equipped_to(PC))
            cout << " (equipped)";
        cout << endl;
    }
    cout << endl;

    if(cp > 0 || sp > 0 || ep > 0 || gp > 0 || pp > 0){
        cout << "COINS: " << endl;
        if(cp > 0) cout << "\t- " << cp << "cp" << endl;
        if(sp > 0) cout << "\t- " << sp << "sp" << endl;
        if(ep > 0) cout << "\t- " << ep << "ep" << endl;
        if(gp > 0) cout << "\t- " << gp << "gp" << endl;
        if(pp > 0) cout << "\t- " << pp << "ep" << endl;
        cout << endl;
    }

    if(!potions.empty()){
        cout << "POTIONS:" << endl;
        for(vector<Treasure*>::iterator it = potions.begin(); it != potions.end(); ++it){
            cout << "\t- ";
            //if(!(*it)->identified) cout << "[?] ";
            cout << (*it)->quantity << " " << (*it)->get_description() << "." << endl;
        }
        cout << endl;
    }

    if(!scrolls.empty()){
        cout << "SPELL SCROLLS:" << endl;
        int num_unidentified_scrolls = 0;
        for(vector<Treasure*>::iterator it = scrolls.begin(); it != scrolls.end(); ++it){
            if(!(*it)->identified)
                num_unidentified_scrolls++;
            else
                cout << "\t- " << (*it)->quantity << " " << (*it)->get_description() << "." << endl;
        }
        if(num_unidentified_scrolls > 0){
            cout << "\t- " << num_unidentified_scrolls << " unidentified spell scroll";
            if(num_unidentified_scrolls > 1) cout << "s";
            cout << "." << endl;
        }
        cout << endl;
    }

    if(!loot.empty()){
        cout << "MISC TREASURE:" << endl;
        for(vector<Treasure*>::iterator it = loot.begin(); it != loot.end(); ++it){
            cout << "\t- ";
            //if(!(*it)->identified) cout << "[?] ";
            cout << (*it)->quantity << " " << (*it)->get_description() << "." << endl;
        }
        cout << endl;
    }
    cout << endl;

    if(!PC->objects.empty()){
        cout << "OTHER ITEMS: " << endl;
        for(vector<Object*>::iterator it = PC->objects.begin(); it != PC->objects.end(); ++it){
            cout << "\t - " << (*it)->name();
            if((*it)->is_equipped_to(PC))
                cout << " (equipped)";
            cout << endl;
        }
        cout << endl;
    }
}



///(long-term) TODO: add the full tables here (by CR)
void Game::rollIndividualTreasure(string monster_name){
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

bool Game::has_unidentified_items(){
    for(vector<Treasure*>::iterator it = loot.begin(); it != loot.end(); ++it){
        if(!(*it)->identified) return true;
    }
    for(vector<Treasure*>::iterator it = potions.begin(); it != potions.end(); ++it){
        if(!(*it)->identified) return true;
    }
    for(vector<Treasure*>::iterator it = scrolls.begin(); it != scrolls.end(); ++it){
        if(!(*it)->identified) return true;
    }

    return false;
}


void Game::identify_items(){
    if(!has_unidentified_items()) return;

    bool first_item_identified = true;
    cout << "As you rest, you look over the items you have gathered more closely." << endl;
    for(vector<Treasure*>::iterator it = loot.begin(); it != loot.end(); ++it){
        if(!(*it)->identified && PC->attribute_chk("INT") > LOOT_IDENTIFY_DC){
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
        if(!(*it)->identified && PC->skill_check("ARCANA") > POTION_IDENTIFY_DC){
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
        if(!(*it)->identified && PC->skill_check("ARCANA") > SCROLL_IDENTIFY_DC){
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


void Game::rollTreasureHoard(){
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

    if(dungeon->magic_items_enabled() == "ALWAYS") {
        num_magic_items = d6();
        magic_item_table = 'A';
    } else if(dungeon->magic_items_enabled() == "NEVER"){
        num_magic_items = 0;
    }



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

//TODO check if item is already equipped, being careful of items with duplicate names (ie equipping a second offhand shortsword is OK)
void Game::equip_item(string input){
    Object* to_equip = NULL;

    for(vector<Weapon*>::iterator it = PC->weapons.begin(); it != PC->weapons.end(); ++it){
        if(contains(input, (*it)->name())){
            to_equip = *it;
        }
    }

    for(vector<Object*>::iterator it = PC->objects.begin(); it != PC->objects.end(); ++it){
        if(contains(input, (*it)->name())){
            to_equip = *it;
        }
    }

    if(to_equip == NULL){
        cout << "Either you don't have that in your inventory, or it's not an equippable item." << endl;
    } else {
        PC->equip(to_equip);
    }
}
