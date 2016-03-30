
#include <game.h>
#include <weapon.h>
#include <pc.h>

using namespace std;


void Game::print_inventory(){
    cout << "WEAPONS: " << endl;
    for(auto it = PC->weapons.begin(); it != PC->weapons.end(); ++it){
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
        for(auto it = potions.begin(); it != potions.end(); ++it){
            cout << "\t- ";
            //if(!(*it)->identified) cout << "[?] ";
            cout << (*it)->quantity << " " << (*it)->get_description() << "." << endl;
        }
        cout << endl;
    }

    if(!scrolls.empty()){
        cout << "SPELL SCROLLS:" << endl;
        int num_unidentified_scrolls = 0;
        for(auto it = scrolls.begin(); it != scrolls.end(); ++it){
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
        for(auto it = loot.begin(); it != loot.end(); ++it){
            if(!(*it)->is_identified()) cout << "    [?]\t";
            else {
                int spaces = 3-num_digits((*it)->value());
                for(int i = 0; i < spaces; i++) cout << " ";
                cout << "[" << (*it)->value() << "gp]\t";
            }
            cout << (*it)->quantity << " " << (*it)->get_description() << "." << endl;
        }
        cout << endl;
    }
    cout << endl;

    if(!PC->objects.empty()){
        cout << "OTHER ITEMS: " << endl;
        for(auto it = PC->objects.begin(); it != PC->objects.end(); ++it){
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
    for(auto it = loot.begin(); it != loot.end(); ++it){
        if(!(*it)->is_identified()) return true;
    }
    for(auto it = potions.begin(); it != potions.end(); ++it){
        if(!(*it)->is_identified()) return true;
    }
    for(auto it = scrolls.begin(); it != scrolls.end(); ++it){
        if(!(*it)->is_identified()) return true;
    }

    return false;
}


void Game::identify_items(){
    if(!has_unidentified_items()) return;

    bool first_item_identified = true;
    cout << "As you rest, you look over the items you have gathered more closely." << endl;
    for(auto it = loot.begin(); it != loot.end(); ++it){
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

    for(auto it = potions.begin(); it != potions.end(); ++it){
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

    for(auto it = scrolls.begin(); it != scrolls.end(); ++it){
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
    for(auto it = v.begin(); it != v.end(); ++it){
        if(item->name() == (*it)->name()){
            cout << "You find another " << (*it)->get_description(false) << "." << endl;
            (*it)->quantity += item->quantity;
            delete item;
            return;
        }
    }
    v.push_back(item);
    if(is_vowel(item->get_description().at(0)))
        cout << "You find an " << item->get_description() << "." << endl;
    else
        cout << "You find a " << item->get_description() << "." << endl;
}

void Game::roll_adjusted_treasure(bool full_treasure_amount){
    int x = d100();

    if(!full_treasure_amount){
        int coins;
        if(x < 51){
            coins = (d6()+d6()+d6()+d6()+d6()+d6())*10;
            cout << "You find " << coins << " cp." << endl;
            cp += coins;
            return;
        } else {
            coins = (d6()+d6()+d6())*10;
            cout << "You find " << coins << " sp." << endl;
            sp += coins;
            return;
        }
    }

    /* coins */
    if(x < 49) {
        int coins;
        if(x < 18){
            coins = (d6()+d6()+d6()+d6()+d6()+d6())*35;
            cout << "You find " << coins << " cp." << endl;
            cp += coins;
            return;
        } else if(x < 35){
            coins = (d6()+d6()+d6())*35;
            cout << "You find " << coins << " sp." << endl;
            sp += coins;
            return;
        } else {
            coins = (d6()+d6())*5;
            cout << "You find " << coins << " gp." << endl;
            gp += coins;
            return;
        }

    /* mundane item */
    } else if (x < 87) {
        if (x < 62)
            add(new Gemstone(10), loot);
        else if (x < 79)
            add(new Gemstone(50), loot);
        else
            add(new Art(25), loot);

    /* magic item */
    } else {
        Treasure* t;
        if (x < 93)
            t = magic_item('A');
        else if (x < 96)
            t = magic_item('B');
        else if (x < 98)
            t = magic_item('C');
        else if (x < 100)
            t = magic_item('F');
        else
            t = magic_item('G');

        if(t->type() == "potion")
            add(t, potions);
        else if(t->type() == "scroll")
            add(t, scrolls);
        else
            add(t, loot);
    }
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
    for(auto it = hoard.begin(); it != hoard.end(); ++it){
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

    for(auto it = PC->weapons.begin(); it != PC->weapons.end(); ++it){
        if(contains(input, (*it)->name())){
            to_equip = *it;
        }
    }

    for(auto it = PC->objects.begin(); it != PC->objects.end(); ++it){
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
