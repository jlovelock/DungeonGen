
#include <cstdlib>
#include <treasure.h>
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
            _description = "opake striated light and dark green";
            break;
        case 8:
            _name = "moss agage";
            y = d4();
            if(y==1) _description = "translucent pink with mossy gray markings";
            else if(y==2) _description = "translucent yellow-white with mossy gray markings";
            else if(y==3) _description = "translucent pink with mossy green markings";
            else _description = "translucent yellow-white with mossy green markings";
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
            _description = "translucent brown with a golden centre";
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
            _description = "opaque dark gray with red flecks";
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
            _description = "opaque blue, black, or brown";
            break;
        case 7:
            _name = "moonstone";
            _description = "translucent white with a pale blue glow";
            break;
        case 8:
            _name = "onyx";
            y = rand()%3;
            if(y == 1) _description = "opaque bands of black and white";
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
            _description = "opaque bands of red and white";
            break;
        case 11:
            _name = "star rose quartz";
            _description = "translucent rosy stone with a white star-shaped centre";
            break;
        default:
            _name = "zircon";
            _description = "transparent pale blue-green";
            break;
    }

}

string Gemstone::get_description(){
    string tmp = "";
    if(identified){
        tmp += name();
        tmp += " ";
    }

    tmp += "gemstone";
    if(quantity != 1) tmp += "s";

    if(identified){
        tmp +=" worth ";
        tmp += to_string(_value);
        tmp += "gp";
        if(quantity > 1) tmp += " each";
    }

    tmp += " that ";
    if(quantity != 1) tmp +="are"; else tmp +="is";
    tmp +=" ";

    tmp += _description;

    return tmp;

}
