
#include <iostream>
#include <string>
#include <defines.h>
#include <cstdlib>
#include <vector>
#include <treasure.h>

using namespace std;

Treasure::Treasure(string _ttype, int num, int gp) : Object() {
    _type = _ttype;
    identified = false;
    quantity = num;
    _value = gp;
}

Treasure::Treasure(Treasure* t) : Object(t) {}

Treasure::~Treasure(){}

string Treasure::get_description(bool plural){
    string tmp = _description;
    if(identified){
        tmp +=" worth ";
        tmp += to_string(_value);
        tmp += "gp";
        if(quantity > 1 && plural) tmp += " each";
    }
    return tmp;
}


Treasure* magic_item_A(){
    int x = d100();

    ///@TODO FIXME dirty hack! roll again (up to 100x) if you get a value that doesn't exist yet
    for(int i = 0; i < 100; i++) {
        if(x < 51)          return new Potion("healing", "common");
        else if (x < 61)    return new Scroll(0);
        else if (x < 71)    continue; //potion of climbing
        else if (x < 91)    return new Scroll(1);
        else if(x < 95)     return new Scroll(2);
        else if(x < 99)     return new Potion("healing", "uncommon");
        else if(x == 99)    continue; //bag of holding: 500lbs, action to retrieve
        else                continue; //driftglobe
    }

    //if you rolled too many times, just make it a damn heal potion.
    return new Potion("healing", "common");
}

Treasure* magic_item_B(){
    int x = d100();

    ///@TODO FIXME dirty hack! roll again (up to 500x) if you get a value that doesn't exist yet
    for(int i = 0; i < 500; i++) {
        if(x < 16)          return new Potion("healing", "uncommon");
        else if (x < 23)    continue; //potion of firebreath
        else if (x < 30)    continue; //potion of resistance
        else if (x < 35)    continue; //ammunition +1
        else if (x < 40)    continue; //potion of animal friendship
        else if (x < 45)    continue; //potion of hill giant strength
        else if (x < 50)    continue; //potion of growth
        else if (x < 55)    continue; //potion of water breathing
        else if (x < 60)    return new Scroll(2);
        else if (x < 65)    continue; //return new Scroll(3);
        else if (x < 68)    continue; //bag of holding
        else if (x < 71)    continue; //keoghtom's ointment
        else if (x < 74)    continue; //oil of slipperiness
        else if (x < 76)    continue; //dust of disappearance
        else if (x < 78)    continue; //dust of dryness
        else if (x < 80)    continue; //dust of sneezing and choking
        else if (x < 82)    continue; //elemental gem
        else if (x < 84)    continue; //philter of love
        else if (x == 84)   continue; //alchemy jug
        else if (x == 85)   continue; //cap of water breathing
        else if (x == 86)   continue; //cloak of the manta ray
        else if (x == 87)   continue; //driftglobe
        else if (x == 88)   continue; //goggles of night
        else if (x == 89)   continue; //helm of comprehending languages
        else if (x == 90)   continue; //immovable rod
        else if (x == 91)   continue; //lantern of revealing
        else if (x == 92)   continue; //mariner's armor
        else if (x == 93)   continue; //mithral armor
        else if (x == 94)   continue; //potion of poison
        else if (x == 95)   continue; //ring of swimming
        else if (x == 96)   continue; //robe of useful items
        else if (x == 97)   continue; //saddle of the cavalier
        else if (x == 98)   continue; //wand of magic detection
        else if (x == 99)   continue; //wand of secrets
        else                continue;
    }

    //if you rolled too many times, just make it a damn heal potion.
    return new Potion("healing", "uncommon");
}


Treasure* magic_item(char table){
    if(table == 'A')        return magic_item_A();
    else if(table == 'B')   return magic_item_B();
    else {
        if(DEBUG) cout << "@@\tNOTE: Magic item table " << table << " uninitialized, using table A instead." << endl;
        return magic_item_A();
    }
}

// base price in gp. merchants will pay a different (likely lower) amount
// DMG p130
int magic_item_cost(string rarity){
    if(rarity == "common")          return 100;
    else if(rarity == "uncommon")   return 500;
    else if(rarity == "rare")       return 5000;
    else if(rarity == "very rare")  return 50000;
    else                            return -1;
}
