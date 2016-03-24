
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
            if(!(*it)->is_identified())
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
        if(!(*it)->is_identified()) return true;
    }
    for(vector<Treasure*>::iterator it = potions.begin(); it != potions.end(); ++it){
        if(!(*it)->is_identified()) return true;
    }
    for(vector<Treasure*>::iterator it = scrolls.begin(); it != scrolls.end(); ++it){
        if(!(*it)->is_identified()) return true;
    }

    return false;
}


void Game::identify_items(){
    if(!has_unidentified_items()) return;

    bool first_item_identified = true;
    cout << "As you rest, you look over the items you have gathered more closely." << endl;
    for(vector<Treasure*>::iterator it = loot.begin(); it != loot.end(); ++it){
        if(!(*it)->is_identified() && PC->attribute_chk("INT") > LOOT_IDENTIFY_DC){
            if(first_item_identified) cout << "You are able to identify the following:" << endl;
            first_item_identified = false;
            cout << "\t- Your " << (*it)->get_description();
            if((*it)->type()=="art"){
                if((*it)->quantity == 1) cout << " is";
                else cout << " are";
            }
            else if((*it)->quantity == 1)
                cout << " is a " << (*it)->name();
            else
                cout << " are " << (*it)->name() << "s";
            cout << " worth " << (*it)->value() << "gp";
            if((*it)->quantity > 1) cout << " each";
            cout << "." << endl;
            (*it)->identify();
        }
    }

    for(vector<Treasure*>::iterator it = potions.begin(); it != potions.end(); ++it){
        if(!(*it)->is_identified() && PC->skill_check("ARCANA") > POTION_IDENTIFY_DC){
            if(first_item_identified) cout << "Looking over the items you have gathered in more detail, you are able to identify the following:" << endl;
            first_item_identified = false;
            cout << "\t- Your " << (*it)->get_description();
            if((*it)->quantity == 1)
                cout << " is a potion of ";
            else
                cout << " are potions of ";
            cout << (*it)->name() << "." << endl;
            (*it)->identify();
        }
    }

    for(vector<Treasure*>::iterator it = scrolls.begin(); it != scrolls.end(); ++it){
        if(!(*it)->is_identified() && PC->skill_check("ARCANA") > SCROLL_IDENTIFY_DC){
            if(first_item_identified) cout << "Looking over the items you have gathered in more detail, you are able to identify the following:" << endl;
            first_item_identified = false;
            cout << "\t- " << (*it)->quantity << " of your scrolls";
            if((*it)->quantity == 1)
                cout << " is a scroll of ";
            else
                cout << " are scrolls of ";
            cout << (*it)->name() << "." << endl;
            (*it)->identify();
        }
    }


    if(first_item_identified) cout << "Unfortunately, you are unable to identify anything." << endl;
    cout << endl;
}

void Game::add(Treasure* item, vector<Treasure*>& v){
    if(DEBUG) cout << "$$\tadding " << item->name() << "...";
    for(vector<Treasure*>::iterator it = v.begin(); it != v.end(); ++it){
        if(item->name() == (*it)->name()){
            (*it)->quantity += item->quantity;
            delete item;
            if(DEBUG) cout << "updated existing.";
            return;
        }
    }
    v.push_back(item);
    if(DEBUG) cout << "done." << endl;
}


void Game::rollTreasureHoard(){
    vector<Treasure*> hoard;

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

    bool no_magic = (dungeon->magic_items_enabled() == "NEVER");
    bool force_magic = (dungeon->magic_items_enabled() == "ALWAYS");

    //Roll item category/quantities
    ///TODO add more magic items / item tables!
    int x = d100();
    if(x < 7) {
        if(force_magic){
            int N = d6();
            for(int i = 0; i < N; i++){
                hoard.push_back(magic_item('A'));
            }
        } else {
            cout << endl;
            return;
        }
    } else if(x < 17){
        add(new Gemstone(10, d6()), hoard);
        add(new Gemstone(10, d6()), hoard);
        if(force_magic){
            int N = d6();
            for(int i = 0; i < N; i++){
                hoard.push_back(magic_item('A'));
            }
        }
    } else if(x < 27){
        add(new Art(25, d4()), hoard);
        add(new Art(25, d4()), hoard);
        if(force_magic){
            int N = d6();
            for(int i = 0; i < N; i++){
                hoard.push_back(magic_item('A'));
            }
        }
    } else if(x < 37){
        add(new Gemstone(50, d6()), hoard);
        add(new Gemstone(50, d6()), hoard);
        if(force_magic){
            int N = d6();
            for(int i = 0; i < N; i++){
                add(magic_item('A'), hoard);
            }
        }
    } else if(x < 45){
        add(new Gemstone(10, d6()), hoard);
        add(new Gemstone(10, d6()), hoard);
        int N = no_magic ? 0 : d6();
        for(int i = 0; i < N; i++){
            add(magic_item('A'), hoard);
        }
    } else if(x < 53){
        add(new Art(25, d4()), hoard);
        add(new Art(25, d4()), hoard);
        int N = no_magic ? 0 : d6();
        for(int i = 0; i < N; i++){
            add(magic_item('A'), hoard);
        }
    } else if(x < 61){
        add(new Gemstone(50, d6()), hoard);
        add(new Gemstone(50, d6()), hoard);
        int N = no_magic ? 0 : d6();
        for(int i = 0; i < N; i++){
            add(magic_item('A'), hoard);
        }
    } else if(x < 66){
        add(new Gemstone(10, d6()), hoard);
        add(new Gemstone(10, d6()), hoard);
        int N = no_magic ? 0 : d4();
        for(int i = 0; i < N; i++){
            add(magic_item('B'), hoard);
        }
    } else if(x < 71) {
        add(new Art(25, d4()), hoard);
        add(new Art(25, d4()), hoard);
        int N = no_magic ? 0 : d4();
        for(int i = 0; i < N; i++){
            add(magic_item('B'), hoard);
        }
    } else if(x < 76){
        add(new Gemstone(50, d6()), hoard);
        add(new Gemstone(50, d6()), hoard);
        int N = no_magic ? 0 : d4();
        for(int i = 0; i < N; i++){
            add(magic_item('B'), hoard);
        }
    } else if(x < 79){
        add(new Gemstone(10, d6()), hoard);
        add(new Gemstone(10, d6()), hoard);
        int N = no_magic ? 0 : d4();
        for(int i = 0; i < N; i++){
            add(magic_item('C'), hoard);
        }
    } else if(x < 81){
        add(new Art(25, d4()), hoard);
        add(new Art(25, d4()), hoard);
        int N = no_magic ? 0 : d4();
        for(int i = 0; i < N; i++){
            add(magic_item('C'), hoard);
        }
    } else if(x < 86){
        add(new Gemstone(50, d6()), hoard);
        add(new Gemstone(50, d6()), hoard);
        int N = no_magic ? 0 : d4();
        for(int i = 0; i < N; i++){
            add(magic_item('C'), hoard);
        }
    } else if(x < 93){
        add(new Art(25, d4()), hoard);
        add(new Art(25, d4()), hoard);
        int N = no_magic ? 0 : d4();
        for(int i = 0; i < N; i++){
            add(magic_item('F'), hoard);
        }
    } else if(x < 98){
        add(new Gemstone(50, d6()), hoard);
        add(new Gemstone(50, d6()), hoard);
        int N = no_magic ? 0 : d4();
        for(int i = 0; i < N; i++){
            add(magic_item('F'), hoard);
        }
    } else if(x < 100){
        add(new Art(25, d4()), hoard);
        add(new Art(25, d4()), hoard);
        if(!no_magic) add(magic_item('G'), hoard);
    } else {
        add(new Gemstone(50, d6()), hoard);
        add(new Gemstone(50, d6()), hoard);
        if(!no_magic) add(magic_item('G'), hoard);
    }

    //Print loot table
    cout << "You have also received the following:" << endl;
    for(vector<Treasure*>::iterator it = hoard.begin(); it != hoard.end(); ++it){
        cout << "\t- " << (*it)->quantity << " " << (*it)->get_description() << "." << endl;
    }
    cout << endl << endl;


    //Add to inventory
    while(!hoard.empty()){
        Treasure* t = hoard.front();
        if(t->type() == "potion")
            add(t, potions);
        else if(t->type() == "scroll")
            add(t, scrolls);
        else
            add(t, loot);

        hoard.erase(hoard.begin());
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
