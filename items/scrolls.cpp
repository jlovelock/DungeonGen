
#include <cstdlib>
#include <treasure.h>
#include <spells.h>
#include <character.h>

using namespace std;

void Scroll::use(Character* user, Character* target){
    cout << "You attempt to cast ";
    if(quantity > 0) cout << "one of ";
    cout << "your " << get_description();
    if(spell->targets_self()){
        cout << " on yourself." << endl;
        spell->cast(user, user);
    } else {
        cout << " on the " << target->full_name() << "." << endl;
        spell->cast(user, target);
    }
    quantity--;
    cout << "The scroll crumbles to dust as the magic leaves it." << endl << endl;
}

Scroll::Scroll(Scroll* o) : Treasure(o) {
    spell = o->spell; ///@TODO confirm this is okay too!
}

Object* Scroll::clone(){
    return new Scroll(this);
}


void Scroll::identify(){
    cout << quantity << " of your scrolls";
    if(quantity == 1)
        cout << " is a scroll of ";
    else
        cout << " are scrolls of ";
    cout << name() << "." << endl;
    identified = true;
}

string Scroll::get_description(bool plural){
    string tmp = "";
    if(identified) {
        tmp += "scroll";
        if(quantity > 1 && plural) tmp += "s";
        tmp += " of ";
        tmp += name();
    } else {
        tmp += "unidentified arcane scroll";
        if(quantity > 1) tmp += "s";
    }
    return tmp;
}

// DMG p200
string scroll_rarity(int lvl){
    switch(lvl){
        case 0: case 1:         return "common";
        case 2: case 3:         return "uncommon";
        case 4: case 5:         return "rare";
        case 6: case 7: case 8: return "very rare";
        case 9:                 return "legendary";
        default:                return "N/A";

    }
}

// DMG p200
int scroll_save_DC(int lvl){
    switch(lvl){
        case 0: case 1: case 2: return 13;
        case 3: case 4:         return 15;
        case 5: case 6:         return 17;
        case 7: case 8:         return 18;
        case 9:                 return 19;
        default:                return -1;
    }
}

// DMG p200
int scroll_attack_mod(int lvl){
    switch(lvl){
        case 0: case 1: case 2: return 5;
        case 3: case 4:         return 7;
        case 5: case 6:         return 9;
        case 7: case 8:         return 10;
        case 9:                 return 11;
        default:                return -1;
    }
}

Scroll::Scroll(int lvl, int num) : Treasure("scroll", num, magic_item_cost(scroll_rarity(lvl))){
    spell = new Spell(lvl);
    _name = spell->name();
    _description = "arcane scroll";
    consumable = true;
    rarity = scroll_rarity(lvl);
    identified = false;
}

Scroll::~Scroll(){
    delete spell;
}
