
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


/*
 * As a note, no weights for art objects are specified in the DMG,
 *   so these are me making my best guess, using the closest standard
 *   equipment analogue I could find in the PHB where applicable.
 */
void Art::roll_25gp(){
    int x=d10();
    switch(x){
        case 1:
            _name = "silver ewer";
            _description = "silver ewer";
            _weight = 1.5;
            break;
        case 2:
            _name = "carved bone statuette";
            _description = "carved bone statuette";
            _weight = 1;
            break;
        case 3:
            _name = "small gold bracelet";
            _description = "small gold bracelet";
            _weight = 0.1;
            break;
        case 4:
            _name = "vestments";
            _description = "cloth-of-gold vestments";
            _weight = 6; //as "fine clothes"
            break;
        case 5:
            _name = "black velvet mask";
            _description = "black velvet mask stitched with silver thread";
            _weight = 0.5;
            break;
        case 6:
            _name = "copper chalice";
            _description = "copper chalice with silver filigree";
            _weight = 1; //flask or tankard
            break;
        case 7:
            _name = "pair of engraved bone dice";
            _description = "pair of engraved bone dice";
            _weight = 0.1;
            break;
        case 8:
            _name = "small mirror";
            _description = "small mirror set in a painted wooden frame";
            _weight = 0.5; //steel mirror
            break;
        case 9:
            _name = "embroidered silk handkerchief";
            _description = "embroidered silk handkerchief";
            _weight = 0.1;
            break;
        default:
            _name = "gold locket";
            _description = "gold locket with a painted portrait inside";
            _weight = 0.1;
    }
}

