
#include <cstdlib>
#include <treasure.h>
#include <iostream>
using namespace std;


Art::Art(int gp, int num) : Treasure("art", num, gp) {
    consumable = false;

    switch(gp){
        case 25: roll_25gp(); break;
    }
}

void Art::identify(){
    cout << "\t- Your " << get_description();

    if(quantity == 1)
        cout << " is worth " << value() << " gp." << endl;
    else
        cout << " are worth " << value() << " gp each." << endl;

    identified = true;
}


void Art::roll_25gp(){
    int x=d10();
    switch(x){
        case 1:
            _name = "silver ewer";
            _description = "silver ewer";
            break;
        case 2:
            _name = "carved bone statuette";
            _description = "carved bone statuette";
            break;
        case 3:
            _name = "small gold bracelet";
            _description = "small gold bracelet";
            break;
        case 4:
            _name = "vestments";
            _description = "cloth-of-gold vestments";
            break;
        case 5:
            _name = "black velvet mask";
            _description = "black velvet mask stitched with silver thread";
            break;
        case 6:
            _name = "copper chalice";
            _description = "copper chalice with silver filigree";
            break;
        case 7:
            _name = "pair of engraved bone dice";
            _description = "pair of engraved bone dice";
            break;
        case 8:
            _name = "small mirror";
            _description = "small mirror set in a painted wooden frame";
            break;
        case 9:
            _name = "embroidered silk handkerchief";
            _description = "embroidered silk handkerchief";
            break;
        default:
            _name = "gold locket";
            _description = "gold locket with a painted portrait inside";
    }
}

