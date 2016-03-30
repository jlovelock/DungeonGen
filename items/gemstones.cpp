
#include <cstdlib>
#include <treasure.h>
#include <iostream>
#include <string>
using namespace std;

Gemstone::Gemstone(int gp, int num) : Treasure("gemstone", num, gp) {
    consumable = false;
    switch(gp){
        case 10: roll_10gp(); break;
        case 50: roll_50gp(); break;
    }
}

void Gemstone::roll_10gp(){
    int x = d12(), y;
    switch(x){
        case 1:
            _name = "azurite";
            _description = "opaque mottled deep blue";
            break;
        case 2:
            _name = "banded agate";
            y = d4();
            if(y==1) _description = "translucent striped brown";
            else if(y==2) _description = "translucent striped blue";
            else if(y==3) _description = "translucent striped white";
            else _description = "translucent striped red";
            break;
        case 3:
            _name = "blue quartz";
            _description = "transparent pale blue";
            break;
        case 4:
            _name = "eye agate";
            y = rand()%5+1;
            if(y==1) _description = "translucent circles of gray";
            else if(y==2) _description = "translucent circles of white";
            else if(y==3) _description = "translucent circles of brown";
            else if(y==4) _description = "translucent circles of blue";
            else _description = "translucent circles of green";
            break;
        case 5:
            _name = "hematite";
            _description = "opaque gray-black";
            break;
        case 6:
            _name = "lapis lazuli";
            _description = "opaque light and dark blue with yellow flecks";
            break;
        case 7:
            _name = "malachite";
            _description = "opaque striated light and dark green";
            break;
        case 8:
            _name = "moss agage";
            y = d4();
            if(y==1) _description = "translucent pink and mossy gray marked";
            else if(y==2) _description = "translucent yellow-white and mossy gray marked";
            else if(y==3) _description = "translucent pink and mossy green marked";
            else _description = "translucent yellow-white and mossy green marked";
            break;
        case 9:
            _name = "obsidian";
            _description = "opaque black";
            break;
        case 10:
            _name = "rhodochrosite";
            _description = "opaque light pink";
            break;
        case 11:
            _name = "tiger eye";
            _description = "golden centred translucent brown";
            break;
        default:
            _name = "turquoise";
            _description = "opaque light blue-green";
            break;
    }
}

void Gemstone::roll_50gp(){
    int x = d12(), y;
    switch(x){
        case 1:
            _name = "bloodstone";
            _description = "opaque dark gray red flecked";
            break;
        case 2:
            _name = "carnelian";
            _description = "opaque orange to red-brown";
            break;
        case 3:
            _name = "chalcedony";
            _description = "opaque white";
            break;
        case 4:
            _name = "chrysoprase";
            _description = "translucent green";
            break;
        case 5:
            _name = "citrine";
            _description = "transparent pale yellow-brown";
            break;
        case 6:
            _name = "jasper";
            y = d6()/2;
            if(y==0) _description = "opaque blue";
            else if(y==1) _description = "opaque black";
            else _description = "opaque brown";
            break;
        case 7:
            _name = "moonstone";
            _description = "pale blue glowing translucent white";
            break;
        case 8:
            _name = "onyx";
            y = rand()%3;
            if(y == 1) _description = "opaque banded black and white";
            else if(y==2) _description = "pure white";
            else _description = "pure black";
            break;
        case 9:
            _name = "quartz";
            y = rand()%3;
            if(y == 1) _description = "transparent white";
            else if(y==2) _description = "transparent smoky gray";
            else _description = "transparent yellow";
            break;
        case 10:
            _name = "sardonyx";
            _description = "opaque banded red and white";
            break;
        case 11:
            _name = "star rose quartz";
            _description = "white star-shaped centred translucent rosy";
            break;
        default:
            _name = "zircon";
            _description = "transparent pale blue-green";
            break;
    }

}

string Gemstone::get_description(bool plural){
    string tmp = _description;
    tmp += " ";

    if(identified)
        tmp += name();
    else
        tmp += "unidentified";
    tmp += " gemstone";
    if(quantity != 1 && plural) tmp += "s";

    if(identified){
        tmp +=" worth ";
        tmp += to_string(_value);
        tmp += "gp";
        if(quantity > 1) tmp += " each";
    }

    return tmp;

}
