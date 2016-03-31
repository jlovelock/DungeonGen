
#include <inventory.h>
#include <objects.h>
#include <treasure.h>
#include <character.h>
#include <defines.h>
#include <weapon.h>
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <sstream>

using namespace std;

Inventory::Inventory(){
    cp = 0;
    sp = 0;
    ep = 0;
    gp = 0;
    pp = 0;
    weapons.clear();
    potions.clear();
    scrolls.clear();
    misc.clear();
}

void Inventory::clear(){
    while(!weapons.empty()){
        delete weapons.front();
        weapons.erase(weapons.begin());
    }
    while(!potions.empty()){
        delete potions.front();
        potions.erase(potions.begin());
    }
    while(!scrolls.empty()){
        delete scrolls.front();
        scrolls.erase(scrolls.begin());
    }
    while(!misc.empty()){
        delete misc.front();
        misc.erase(misc.begin());
    }

    weapons.clear();
    potions.clear();
    scrolls.clear();
    misc.clear();
    cp = 0; sp = 0; ep = 0; gp = 0; pp = 0;
}

Inventory::~Inventory(){
    clear();
}

bool Inventory::is_empty(){
    return  potions.empty() &&
            scrolls.empty() &&
            weapons.empty() &&
            misc.empty() &&
            cp == 0 &&
            sp == 0 &&
            ep == 0 &&
            gp == 0 &&
            pp == 0;
}

vector<Object*> Inventory::potion_obj_vector(){
    vector<Object*> v;
    v.assign( potions.begin(), potions.end() );
    return v;
}
vector<Object*> Inventory::scroll_obj_vector(){
    vector<Object*> v;
    v.assign( scrolls.begin(), scrolls.end() );
    return v;
}
vector<Object*> Inventory::weapon_obj_vector(){
    vector<Object*> v;
    v.assign( weapons.begin(), weapons.end() );
    return v;
}

void Inventory::add(Object* o){
    vector<Object*> v;
    if(o->type() == "weapon")       v.assign( weapons.begin(), weapons.end() );
    else if(o->type() == "potion")  v.assign( potions.begin(), potions.end() );
    else if(o->type() == "scroll")  v.assign( scrolls.begin(), scrolls.end() );
    else                            v.assign(   misc.begin(),     misc.end() );

    for(unsigned i = 0; i < v.size(); i++){
        if(o->name() == v.at(i)->name()){
            v.at(i)->increase_quantity(o->get_quantity());
            delete o;
            return;
        }
    }

    if(o->type() == "weapon")       weapons.push_back((Weapon*)o);
    else if(o->type() == "potion")  potions.push_back((Potion*)o);
    else if(o->type() == "scroll")  scrolls.push_back((Scroll*)o);
    else                               misc.push_back(o);

//    if(is_vowel(o->get_description().at(0)))
//        cout << "You find an " << o->get_description() << "." << endl;
//    else
//        cout << "You find a " << o->get_description() << "." << endl;
}

/// Todo: clean up all this junk repeated code!!
void Inventory::transfer(Inventory* i, string name){
    stringstream ss;
    vector<string> found;
    for(auto it = i->weapons.begin(); it != i->weapons.end(); it = i->weapons.erase(it)){
        if(!(*it)->is_lootable()) continue;
        if(get_item((*it)->name()) != NULL)                 ss << "another ";
        else if(is_vowel((*it)->get_description().at(0)))   ss << "an ";
        else                                                ss << "a ";
        ss << (*it)->get_description();
        found.push_back(ss.str());
        ss.str("");
        add(*it);
    }

    for(auto it = i->potions.begin(); it != i->potions.end(); it = i->potions.erase(it)){
        if(!(*it)->is_lootable()) continue;
        if(get_item((*it)->name()) != NULL)                 ss << "another ";
        else if(is_vowel((*it)->get_description().at(0)))   ss << "an ";
        else                                                ss << "a ";
        ss << (*it)->get_description();
        found.push_back(ss.str());
        ss.str("");
        add(*it);
    }

    for(auto it = i->scrolls.begin(); it != i->scrolls.end(); it = i->scrolls.erase(it)){
        if(!(*it)->is_lootable()) continue;
        if(get_item((*it)->name()) != NULL)                 ss << "another ";
        else if(is_vowel((*it)->get_description().at(0)))   ss << "an ";
        else                                                ss << "a ";
        ss << (*it)->get_description();
        found.push_back(ss.str());
        ss.str("");
        add(*it);
    }

    for(auto it = i->misc.begin(); it != i->misc.end(); it = i->misc.erase(it)){
        if(!(*it)->is_lootable()) continue;
        if(get_item((*it)->name()) != NULL)                 ss << "another ";
        else if(is_vowel((*it)->get_description().at(0)))   ss << "an ";
        else                                                ss << "a ";
        ss << (*it)->get_description();
        found.push_back(ss.str());
        ss.str("");
        add(*it);
    }

    if(i->cp > 0){
        cp += i->cp;
        ss << i->cp << " copper pieces";
        i->cp = 0;
        found.push_back(ss.str());
        ss.str("");
    }
    if(i->sp > 0){
        sp += i->sp;
        ss << i->sp << " silver pieces";
        i->sp = 0;
        found.push_back(ss.str());
        ss.str("");
    }
    if(i->ep > 0){
        ep += i->ep;
        ss << i->ep << " electrum pieces";
        i->ep = 0;
        found.push_back(ss.str());
        ss.str("");
    }
    if(i->gp > 0){
        gp += i->gp;
        ss << i->gp << " gold pieces";
        i->gp = 0;
        found.push_back(ss.str());
        ss.str("");
    }
    if(i->pp > 0){
        pp += i->pp;
        ss << i->pp << " platinum pieces";
        i->pp = 0;
        found.push_back(ss.str());
        ss.str("");
    }

    if(found.empty()){
        cout << "You get nothing from the " << name << "." << endl;
    } else {
        cout << "You get " << found.at(0);
        for(unsigned i = 1; i < found.size(); i++){
            if(found.size() > 2) cout << ",";
            cout << " ";
            if(i == found.size()-1) cout << "and ";
            cout << found.at(i);
        }
        cout << " from the " << name << "." << endl;
    }
}

void Inventory::add_coins(int n, string type){
    if(type == "cp")        cp += n;
    else if(type == "sp")   sp += n;
    else if(type == "ep")   ep += n;
    else if(type == "gp")   gp += n;
    else                    pp += n;
}

void Inventory::roll_incidental_treasure(){
    int x = d100();
    if(x < 51)
        add_coins((d6()+d6()+d6()+d6()+d6()+d6())*10, "cp");
    else
        add_coins((d6()+d6()+d6())*10, "sp");
}

void Inventory::roll_full_treasure(){
    int x = d100();

    /* coins */
    if(x < 49) {
        if(x < 18)
            add_coins((d6()+d6()+d6()+d6()+d6()+d6())*35, "cp");
        else if(x < 35)
            add_coins((d6()+d6()+d6())*35, "sp");
        else
            add_coins((d6()+d6())*5, "gp");

    /* mundane item */
    } else if (x < 87) {
        if (x < 62)
            add(new Gemstone(10));
        else if (x < 79)
            add(new Gemstone(50));
        else
            add(new Art(25));

    /* magic item */
    } else {
        if (x < 93)
            add(magic_item('A'));
        else if (x < 96)
            add(magic_item('B'));
        else if (x < 98)
            add(magic_item('C'));
        else if (x < 100)
            add(magic_item('F'));
        else
            add(magic_item('G'));
    }
}

// Does not delete the object! (In case you want to add it to another's inventory.)
void Inventory::remove(Object* o){
    vector<Object*> v;
         if(o->type() == "weapon")  v.assign( weapons.begin(), weapons.end() );
    else if(o->type() == "potion")  v.assign( potions.begin(), potions.end() );
    else if(o->type() == "scroll")  v.assign( scrolls.begin(), scrolls.end() );
    else                            v.assign(   misc.begin(),     misc.end() );

    for(unsigned i = 0; i < v.size(); i++){
        if(o->name() == v.at(i)->name()){
                 if(o->type() == "weapon")  weapons.erase( weapons.begin()+i );
            else if(o->type() == "potion")  potions.erase( potions.begin()+i );
            else if(o->type() == "scroll")  scrolls.erase( scrolls.begin()+i );
            else                               misc.erase(    misc.begin()+i );
            return;
        }
    }
    cout << o->name() << " not found in inventory." << endl;
}

Object* Inventory::get_item(string input){
    for(auto it = weapons.begin(); it != weapons.end(); ++it){
        if(contains(input, (*it)->name()) || contains(input, (*it)->get_description()) || contains((*it)->get_description(), input)) return *it;
    }
    for(auto it = potions.begin(); it != potions.end(); ++it){
        if(contains(input, (*it)->name()) || contains(input, (*it)->get_description()) || contains((*it)->get_description(), input)) return *it;
    }
    for(auto it = scrolls.begin(); it != scrolls.end(); ++it){
        if(contains(input, (*it)->name()) || contains(input, (*it)->get_description()) || contains((*it)->get_description(), input)) return *it;
    }
    for(auto it = misc.begin(); it != misc.end(); ++it){
        if(contains(input, (*it)->name()) || contains(input, (*it)->get_description()) || contains((*it)->get_description(), input)) return *it;
    }

    return NULL;
}

Weapon* Inventory::weapon_select(int distance){
    map<string, Weapon*> choice_matrix;
    bool found = false;

    while(true) {
        for(auto it = weapons.begin(); it != weapons.end(); ++it) {
            if(distance <= (*it)->range()){
                if(!found){
                    cout << "Which weapon do you want to use?:" << endl;
                    found = true;
                }
                choice_matrix[(*it)->name()] = (*it);
                cout << "\t - " << (*it)->name();
                if(distance >= 10 && (*it)->is_thrown())
                    cout << " (thrown)";
                cout << endl;
            }
//            else {
//                cout << "\t(" << (*it)->name() << " out of range.)" << endl;
//            }
        }

        if(!found){
            cout << "No weapons within range." << endl;
            return NULL;
        }

        cout << endl;
        string choice;
        read(choice);
        auto w = choice_matrix.find(choice);
        if(w == choice_matrix.end()) {
            cout << "Invalid selection. Your choices are as follows: " << endl;
        } else {
            cout << choice_matrix.at(choice)->name() << " selected." << endl;
            return choice_matrix.at(choice);
        }
    }
}

void Inventory::print(Character* PC){
    cout << "WEAPONS: " << endl;
    for(auto it = weapons.begin(); it != weapons.end(); ++it){
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
            cout << (*it)->get_quantity() << " " << (*it)->get_description() << "." << endl;
        }
        cout << endl;
    }

    if(!scrolls.empty()){
        cout << "SPELL SCROLLS:" << endl;
        for(auto it = scrolls.begin(); it != scrolls.end(); ++it){
            if((*it)->is_identified())
                cout << "\t- " << (*it)->get_quantity() << " " << (*it)->get_description() << "." << endl;
        }
        int n = num_unidentified_scrolls();
        if(n > 0){
            cout << "\t- " << n << " unidentified spell scroll";
            if(n > 1) cout << "s";
            cout << "." << endl;
        }
        cout << endl;
    }

    if(!misc.empty()){
        cout << "MISCELLANIA:" << endl;
        for(auto it = misc.begin(); it != misc.end(); ++it){
            if(!(*it)->is_identified()) cout << "    [?]\t";
            else {
                int spaces = 3-num_digits((*it)->value());
                for(int i = 0; i < spaces; i++) cout << " ";
                cout << "[" << (*it)->value() << "gp]\t";
            }
            cout << (*it)->get_quantity() << " " << (*it)->get_description() << "." << endl;
        }
        cout << endl;
    }
    cout << endl;
}

void Inventory::print_weapon_stats(Character* c){
    cout << "\tWeapons:" << endl;
    for(auto it = weapons.begin(); it != weapons.end(); ++it){
        cout << "\t - " << (*it)->get_weapon_description(c) << endl;
    }
}


void Inventory::set_monster_treasure(int CR){
    int x = d100();
    if(CR <= 4){
        if(x < 31)
            cp = d6()+d6()+d6()+d6()+d6();
        else if(x < 61)
            sp = d6()+d6()+d6()+d6();
        else if(x < 71)
            ep = d6()+d6()+d6();
        else if(x < 96)
            gp = d6()+d6()+d6();
        else
            pp = d6();

    } else {
        cout << "This monster is way too powerful holy shit. How did this happen. You get no money, sorry." << endl;
    }
}

/// deprecated, but if ever needed again, it'll be as a part of this class.
//void Game::roll_treasure_hoard(){
//    vector<Treasure*> hoard;
//
//    string type;
//
//    //Roll money
//    int copper, silver, gold;
//    copper = (d6()+d6()+d6()+d6()+d6()+d6())*100;
//    silver = (d6()+d6()+d6())*100;
//    gold = (d6()+d6())*10;
//    cout << "You find " << copper << "cp, " << silver << "sp, and " << gold << "gp." << endl;
//    cp += copper;
//    sp += silver;
//    gp += gold;
//
//    bool no_magic = (dungeon->magic_items_enabled() == "NEVER");
//    bool force_magic = (dungeon->magic_items_enabled() == "ALWAYS");
//
//    //Roll item category/quantities
//    ///TODO add more magic items / item tables!
//    int x = d100();
//    if(x < 7) {
//        if(force_magic){
//            int N = d6();
//            for(int i = 0; i < N; i++){
//                hoard.push_back(magic_item('A'));
//            }
//        } else {
//            cout << endl;
//            return;
//        }
//    } else if(x < 17){
//        add(new Gemstone(10, d6()), hoard);
//        add(new Gemstone(10, d6()), hoard);
//        if(force_magic){
//            int N = d6();
//            for(int i = 0; i < N; i++){
//                hoard.push_back(magic_item('A'));
//            }
//        }
//    } else if(x < 27){
//        add(new Art(25, d4()), hoard);
//        add(new Art(25, d4()), hoard);
//        if(force_magic){
//            int N = d6();
//            for(int i = 0; i < N; i++){
//                hoard.push_back(magic_item('A'));
//            }
//        }
//    } else if(x < 37){
//        add(new Gemstone(50, d6()), hoard);
//        add(new Gemstone(50, d6()), hoard);
//        if(force_magic){
//            int N = d6();
//            for(int i = 0; i < N; i++){
//                add(magic_item('A'), hoard);
//            }
//        }
//    } else if(x < 45){
//        add(new Gemstone(10, d6()), hoard);
//        add(new Gemstone(10, d6()), hoard);
//        int N = no_magic ? 0 : d6();
//        for(int i = 0; i < N; i++){
//            add(magic_item('A'), hoard);
//        }
//    } else if(x < 53){
//        add(new Art(25, d4()), hoard);
//        add(new Art(25, d4()), hoard);
//        int N = no_magic ? 0 : d6();
//        for(int i = 0; i < N; i++){
//            add(magic_item('A'), hoard);
//        }
//    } else if(x < 61){
//        add(new Gemstone(50, d6()), hoard);
//        add(new Gemstone(50, d6()), hoard);
//        int N = no_magic ? 0 : d6();
//        for(int i = 0; i < N; i++){
//            add(magic_item('A'), hoard);
//        }
//    } else if(x < 66){
//        add(new Gemstone(10, d6()), hoard);
//        add(new Gemstone(10, d6()), hoard);
//        int N = no_magic ? 0 : d4();
//        for(int i = 0; i < N; i++){
//            add(magic_item('B'), hoard);
//        }
//    } else if(x < 71) {
//        add(new Art(25, d4()), hoard);
//        add(new Art(25, d4()), hoard);
//        int N = no_magic ? 0 : d4();
//        for(int i = 0; i < N; i++){
//            add(magic_item('B'), hoard);
//        }
//    } else if(x < 76){
//        add(new Gemstone(50, d6()), hoard);
//        add(new Gemstone(50, d6()), hoard);
//        int N = no_magic ? 0 : d4();
//        for(int i = 0; i < N; i++){
//            add(magic_item('B'), hoard);
//        }
//    } else if(x < 79){
//        add(new Gemstone(10, d6()), hoard);
//        add(new Gemstone(10, d6()), hoard);
//        int N = no_magic ? 0 : d4();
//        for(int i = 0; i < N; i++){
//            add(magic_item('C'), hoard);
//        }
//    } else if(x < 81){
//        add(new Art(25, d4()), hoard);
//        add(new Art(25, d4()), hoard);
//        int N = no_magic ? 0 : d4();
//        for(int i = 0; i < N; i++){
//            add(magic_item('C'), hoard);
//        }
//    } else if(x < 86){
//        add(new Gemstone(50, d6()), hoard);
//        add(new Gemstone(50, d6()), hoard);
//        int N = no_magic ? 0 : d4();
//        for(int i = 0; i < N; i++){
//            add(magic_item('C'), hoard);
//        }
//    } else if(x < 93){
//        add(new Art(25, d4()), hoard);
//        add(new Art(25, d4()), hoard);
//        int N = no_magic ? 0 : d4();
//        for(int i = 0; i < N; i++){
//            add(magic_item('F'), hoard);
//        }
//    } else if(x < 98){
//        add(new Gemstone(50, d6()), hoard);
//        add(new Gemstone(50, d6()), hoard);
//        int N = no_magic ? 0 : d4();
//        for(int i = 0; i < N; i++){
//            add(magic_item('F'), hoard);
//        }
//    } else if(x < 100){
//        add(new Art(25, d4()), hoard);
//        add(new Art(25, d4()), hoard);
//        if(!no_magic) add(magic_item('G'), hoard);
//    } else {
//        add(new Gemstone(50, d6()), hoard);
//        add(new Gemstone(50, d6()), hoard);
//        if(!no_magic) add(magic_item('G'), hoard);
//    }
//
//    //Print loot table
//    cout << "You have also received the following:" << endl;
//    for(auto it = hoard.begin(); it != hoard.end(); ++it){
//        cout << "\t- " << (*it)->get_quantity() << " " << (*it)->get_description() << "." << endl;
//    }
//    cout << endl << endl;
//
//
//    //Add to inventory
//    while(!hoard.empty()){
//        Treasure* t = hoard.front();
//        if(t->type() == "potion")
//            add(t, potions);
//        else if(t->type() == "scroll")
//            add(t, scrolls);
//        else
//            add(t, loot);
//
//        hoard.erase(hoard.begin());
//    }
//
//}


int Inventory::num_unidentified_scrolls(){
    int n = 0;
    for(auto it = scrolls.begin(); it != scrolls.end(); ++it)
        if(!(*it)->is_identified()) ++n;
    return n;
}

bool Inventory::has_unidentified_items(){
    for(auto it = misc.begin(); it != misc.end(); ++it){
        if(!(*it)->is_identified()) return true;
    }
    for(auto it = potions.begin(); it != potions.end(); ++it){
        if(!(*it)->is_identified()) return true;
    }
    for(auto it = scrolls.begin(); it != scrolls.end(); ++it){
        if(!(*it)->is_identified()) return true;
    }
    for(auto it = weapons.begin(); it != weapons.end(); ++it){
        if(!(*it)->is_identified()) return true;
    }

    return false;
}

float Inventory::weight(){
    float w = (cp + sp + ep + gp + pp)/50;
    for(auto it = misc.begin(); it != misc.end(); ++it){
        w += (*it)->weight();
    }
    for(auto it = potions.begin(); it != potions.end(); ++it){
        w += (*it)->weight();
    }
    for(auto it = scrolls.begin(); it != scrolls.end(); ++it){
        w += (*it)->weight();
    }
    for(auto it = weapons.begin(); it != weapons.end(); ++it){
        w += (*it)->weight();
    }
    return w;
}
