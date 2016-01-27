
#include <iostream>
#include <string>
#include <defines.h>
#include <cstdlib>
#include <vector>
#include <treasure.h>

using namespace std;

Treasure::Treasure(){
    identified = false;
    quantity = 0;
}

Treasure::~Treasure(){
    if(spell != NULL) delete spell;
}

string Treasure::get_description(){
    string tmp = "";
    if(type == "gemstones"){
        if(identified){
            tmp += name;
            tmp += " ";
        }

        tmp += "gemstone";
        if(quantity != 1) tmp += "s";

        if(identified){
            tmp +=" worth ";
            char tmp_str[10];
            tmp += string(itoa(value, tmp_str, 10));
            tmp += "gp";
            if(quantity > 1) tmp += " each";
        }

        tmp += " that ";
        if(quantity != 1) tmp +="are"; else tmp +="is";
        tmp +=" ";

        tmp +=description;

    }
    else if(type == "potion"){
        tmp +=description;
        tmp += " potion";
        if(quantity != 1) tmp += "s";
        if(identified){
            tmp +=" of ";
            tmp += name;
        }
    }
    else if(type == "scroll"){
        if(identified) {
            tmp += "scroll";
            if(quantity > 1) tmp += "s";
            tmp += " of ";
            tmp += name;
        } else {
            tmp += "unidentified arcane scroll";
            if(quantity > 1) tmp += "s";
        }
    } else {
        tmp = description;
        if(identified){
            tmp +=" worth ";
            char tmp_str[10];
            tmp += string(itoa(value, tmp_str, 10));
            tmp += "gp";
            if(quantity > 1) tmp += " each";
        }

    }

    return tmp;

}

//For healing potions. Returns the number of hp
int Treasure::healing_amount(){
    if(!consumable) return 0;

    int n;
    if(rarity == COMMON)
        n=2;
    else if(rarity == UNCOMMON)
        n = 4;
    else if(rarity == RARE)
        n = 8;
    else if(rarity == VERY_RARE)
        n = 10;
    else
        n = 0;

    int total = 0;
    for(int i = 0; i < n; i++){
        total += d4()+1;
        if(rarity == VERY_RARE) total++;
    }
    return total;
}

void Treasure::set_gemstones(int gp){
    int x=d12(), y;
    consumable = false;

    if(gp == 10){
        switch(x){
            case 1:
                name = "azurite";
                description = "opaque mottled deep blue";
                break;
            case 2:
                 name = "banded agate";
                y = d4();
                if(y==1) description = "translucent striped brown";
                else if(y==2) description = "translucent striped blue";
                else if(y==3) description = "translucent striped white";
                else description = "translucent striped red";
                break;
            case 3:
                name = "blue quartz";
                description = "transparent pale blue";
                break;
            case 4:
                name = "eye agate";
                y = rand()%5+1;
                if(y==1) description = "translucent circles of gray";
                else if(y==2) description = "translucent circles of white";
                else if(y==3) description = "translucent circles of brown";
                else if(y==4) description = "translucent circles of blue";
                else description = "translucent circles of green";
                break;
            case 5:
                name = "hematite";
                description = "opaque gray-black";
                break;
            case 6:
                name = "lapis lazuli";
                description = "opaque light and dark blue with yellow flecks";
                break;
            case 7:
                name = "malachite";
                description = "opake striated light and dark green";
                break;
            case 8:
                name = "moss agage";
                y = d4();
                if(y==1) description = "translucent pink with mossy gray markings";
                else if(y==2) description = "translucent yellow-white with mossy gray markings";
                else if(y==3) description = "translucent pink with mossy green markings";
                else description = "translucent yellow-white with mossy green markings";
                break;
            case 9:
                name = "obsidian";
                description = "opaque black";
                break;
            case 10:
                name = "rhodochrosite";
                description = "opaque light pink";
                break;
            case 11:
                name = "tiger eye";
                description = "translucent brown with a golden centre";
                break;
            default:
                name = "turquoise";
                description = "opaque light blue-green";
                break;
        }

    } else if (gp == 50){
        switch(x){
            case 1:
                name = "bloodstone";
                description = "opaque dark gray with red flecks";
                break;
            case 2:
                name = "carnelian";
                description = "opaque orange to red-brown";
                break;
            case 3:
                name = "chalcedony";
                description = "opaque white";
                break;
            case 4:
                name = "chrysoprase";
                description = "translucent green";
                break;
            case 5:
                name = "citrine";
                description = "transparent pale yellow-brown";
                break;
            case 6:
                name = "jasper";
                description = "opaque blue, black, or brown";
                break;
            case 7:
                name = "moonstone";
                description = "translucent white with a pale blue glow";
                break;
            case 8:
                name = "onyx";
                y = rand()%3;
                if(y == 1) description = "opaque bands of black and white";
                else if(y==2) description = "pure white";
                else description = "pure black";
                break;
            case 9:
                name = "quartz";
                y = rand()%3;
                if(y == 1) description = "transparent white";
                else if(y==2) description = "transparent smoky gray";
                else description = "transparent yellow";
                break;
            case 10:
                name = "sardonyx";
                description = "opaque bands of red and white";
                break;
            case 11:
                name = "star rose quartz";
                description = "translucent rosy stone with a white star-shaped centre";
                break;
            default:
                name = "zircon";
                description = "transparent pale blue-green";
                break;
        }
    } else {
        cout << "Whoops. You were supposed to get a hella expensive gemstone, but I didn't add those tables yet. Sorry!" << endl;
    }
}

void Treasure::set_art(int gp){
    int x;
    consumable = false;
    if(gp == 25){
        x=d10();
        switch(x){
            case 1:
                name = "silver ewer";
                description = "silver ewer";
                break;
            case 2:
                name = "carved bone statuette";
                description = "carved bone statuette";
                break;
            case 3:
                name = "small gold bracelet";
                description = "small gold bracelet";
                break;
            case 4:
                name = "vestments";
                description = "cloth-of-gold vestments";
                break;
            case 5:
                name = "black velvet mask";
                description = "black velvet mask stitched with silver thread";
                break;
            case 6:
                name = "copper chalice";
                description = "copper chalice with silver filigree";
                break;
            case 7:
                name = "pair of engraved bone dice";
                description = "pair of engraved bone dice";
                break;
            case 8:
                name = "small mirror";
                description = "small mirror set in a painted wooden frame";
                break;
            case 9:
                name = "embroidered silk handkerchief";
                description = "embroidered silk handkerchief";
                break;
            default:
                name = "gold locket";
                description = "gold locket with a painted portrait inside";
        }
    }

}

Treasure::Treasure(char table){
    if(table != 'A'){
        cout << "I haven't initialized these magic items yet! Sorry." << endl;
        return;
    }

    quantity = 1;
    identified = false;

    int x = d100();
    bool properly_initialized = true; ///temp variable, to make sense of things until the full table is inputted properly
    is_scroll = false; spell = NULL;

    int i = 0;
    do {
        //potion of healing
        if(x < 51){
            consumable = true;
            name = "healing";
            description = "red, glimmering";
            type = "potion";
            rarity = COMMON;

        //spell scroll (cantrip)
        } else if (x < 61){
            consumable = true;
            is_scroll = true;
            spell = new Spell(0);
            name = spell->name;
            description = "arcane scroll";
            type = "scroll";
            rarity = COMMON;

        ///TODO: potion of climbing
        } else if(x < 71){
            consumable = true;
            properly_initialized = false;

        ///TODO: spell scroll (lvl 1)
        } else if(x < 91){
            consumable = true;
            is_scroll = true;
            spell = new Spell(1);
            name = spell->name;
            description = "arcane scroll";
            type = "scroll";
            rarity = COMMON;

        ///TODO: spell scroll (lvl 2)
        } else if(x < 95){
            consumable = true;
            is_scroll = true;
            spell = new Spell(2);
            name = spell->name;
            description = "arcane scroll";
            type = "scroll";
            rarity = COMMON;

        //potion of greater healing
        } else if(x < 99){
            type = "potion";
            consumable = true;
            name = "greater healing";
            description = "deep red, glimmering";
            rarity = UNCOMMON;

        ///TODO: bag of holding
        } else if(x == 99){
            properly_initialized = false;

        ///TODO: driftglobe
        } else {
            properly_initialized = false;
        }

        i++;

    ///TODO FIXME dirty hack! roll again if you got a value that doesn't exist yet
    } while (!properly_initialized && i < 100);

    ///TODO FIXME if you rolled too many times just make it a damn heal potion
    if(i == 100){
        consumable = true;
        name = "healing";
        description = "red, glimmering";
        type = "potion";
        rarity = COMMON;
    }
}

//this function is called on common items only: magic ones use the other constructor
Treasure::Treasure(string _type, int num, int gp){
    type = _type;
    identified = false;
    quantity = num;
    value = gp;
    is_scroll = false; spell = NULL;

    if(type == "gemstones"){
        set_gemstones(gp);
    } else if(type == "art"){
        set_art(gp);
    }
}
