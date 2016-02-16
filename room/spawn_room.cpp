#include <room.h>

int rollExits(bool large){
    int x = d20();
    switch(x){
        case 1: case 2: case 3:
            return 0;
        case 4: case 5:
            return large ? 1 : 0;
        case 6: case 7: case 8:
            return 1;
        case 9: case 10: case 11:
            return large ? 2 : 1;
        case 12: case 13:
            return 2;
        case 14: case 15:
            return large ? 3 : 2;
        case 16: case 17:
            return 3;
        case 18:
            return large ? 4 : 3;
        case 19:
            return large ? 5 : 4;
        case 20:
            return large ? 6 : 4;
    }
    return -1;
}

///TODO place exits appropriately (ie not all south doors should be on top of each other!)
void Room::setExits(int entrance, bool large){
    int numExits = rollExits(large);

    ///TODO some sort of sorting? (Don't want to say "door on the east, north, and east")
    for(int i = 1; i < MAX_DOORS; i++){
        if(i < numExits){
            doors[i] = new Door(this, entrance);
        }
        else
            doors[i] = NULL;
    }
}

///For creating the starting area only!!
Room::Room(){
    id = 0;
    is_passage=false;
    next = NULL;
    for(int i = 0; i < MAX_MONSTERS; i++)
        monsters[i] = NULL;
    setPurpose();

    int x = d10();
    switch(x){
        case 1:
        default: ///TODO add the rest of the options!
            shape = "square";
            xDim = 20; yDim = 20;
            setEdgeCoords(SOUTH, START_X, START_Y); //the coordinate of the exit door

            doors[0] = new Door(this, SOUTH, false, true);
            doors[1] = new Door(this, EAST, true, false);
            doors[2] = new Door(this, NORTH, true, false);
            doors[3] = new Door(this, WEST, true, false);
            for(int i = 4; i < MAX_DOORS; i++){
                doors[i] = NULL;
            }
            break;
    }
}

void Room::setEdgeCoords(int entry_wall, int entry_x, int entry_y){
    switch(entry_wall){
        case SOUTH:
            southEdge = entry_y;
            northEdge = entry_y + yDim;
            eastEdge = entry_x + xDim/2;
            westEdge = entry_x - xDim/2;
            return;
        case NORTH:
            northEdge = entry_y;
            southEdge = entry_y - yDim;
            eastEdge = entry_x + xDim/2;
            westEdge = entry_x - xDim/2;
            return;
        case WEST:
            westEdge = entry_x;
            eastEdge = entry_x + xDim;
            northEdge = entry_y + yDim/2;
            southEdge = entry_y - yDim/2;
            return;
        case EAST:
            eastEdge = entry_x;
            westEdge = entry_x - xDim;
            northEdge = entry_y + yDim/2;
            southEdge = entry_y - yDim/2;
            return;
    };
}

//roll contents
///TODO fill in the blanks where there are some, distinguish between monsters as dominant inhabitants / random creatures
void Room::set_contents(){
    int x = d100();
    if(x < 9){
        monsters[0] = new Character(0);
    } else if(x < 16){
        monsters[0] = new Character(0);
        has_treasure = true;
    } else if(x < 28){
        ///pet or allied creature
    } else if(x < 34){
        ///pet or allied creature
        has_treasure = true;
    } else if(x < 43){
        monsters[0] = new Character(0);
    } else if(x < 51){
        monsters[0] = new Character(0);
        has_treasure = true;
    } else if(x < 59){
        int y = d20();
        if(y < 4) hazard = "brown mold";
        else if(y < 9) hazard = "green slime";
        else if(y < 11) hazard = "shrieker";
        else if(y < 16) hazard = "spiderwebs";
        else if(y < 18) hazard = "violet fungus";
        else hazard = "yellow mold";
        ///incidental treasure
    } else if(x < 64){
        ///obstacle
    } else if(x < 74){
        ///trap
    } else if(x < 77){
        ///trap
        has_treasure = true;
    } else if(x < 81){
        ///trick
    } else if(x > 88 && x < 95){
        int y = d20();
        if(y < 4) hazard = "brown mold";
        else if(y < 9) hazard = "green slime";
        else if(y < 11) hazard = "shrieker";
        else if(y < 16) hazard = "spiderwebs";
        else if(y < 18) hazard = "violet fungus";
        else hazard = "yellow mold";
    } else {
        has_treasure = true;
    }

//    ///For debugging non-monster-related stuff: remove me later!!
//    if(monsters[0] != NULL){
//        delete monsters[0];
//        monsters[0] = NULL;
//    }

}

void Room::generateChamber(Door* d){

    ///TODO handle monster spawning properly!
    for(int i = 0; i < MAX_MONSTERS; i++)
        monsters[i] = NULL;

    //defaults
    has_treasure = false;
    is_passage = false;

    setPurpose();
    set_contents();

    ///*********DEBUG, REMOVE ME AFTER
    has_treasure = true;

    //set chamber size, shape, and exits
    bool is_large = false;
    int x = d20();
    switch(x) {
        case 1: case 2:
            shape = "square";
            xDim=20; yDim=20;
            break;
        case 3: case 4:
            shape = "square";
            xDim=30; yDim=30;
            break;
        case 5: case 6:
            shape = "square";
            xDim=40; yDim=40;
            break;
        case 7: case 8: case 9:
            shape = "rectangular";
            if(rand()%2){
                xDim = 20; yDim = 30;
            } else {
                xDim = 30; yDim = 20;
            }
            break;

        case 10: case 11: case 12:
            shape = "rectangular";
            if(rand()%2){
                xDim = 30; yDim = 40;
            } else {
                xDim = 40; yDim = 30;
            }
            break;

        case 13: case 14:
            shape = "rectangular";
            if(rand()%2){
                xDim = 40; yDim = 50;
            } else {
                xDim = 50; yDim = 40;
            }
            is_large = true;
            break;
        case 15:
            shape = "rectangular";
            if(rand()%2){
                xDim = 50; yDim = 80;
            } else {
                xDim = 80; yDim = 50;
            }
            is_large = true;
            break;
        case 16:
            shape = "circular";
            xDim=30; yDim=30;
            break;
        case 17:
            shape = "circular";
            xDim=50; yDim=50;
            is_large = true;
            break;
        case 18:
            shape = "octagonal";
            xDim=40; yDim=40;
            is_large = true;
            break;
        case 19:
            shape = "octagonal";
            xDim=60; yDim=60;
            is_large = true;
            break;
        case 20:
            shape = "trapezoidal";
            if(rand()%2){
                xDim = 40; yDim = 60;
            } else {
                xDim = 60; yDim = 40;
            }
            is_large = true;
            break;
    }

    setEdgeCoords(opposite(d->firstWall),d->xPos,d->yPos);
    setExits(d->firstWall, is_large);
}

Room::Room(int _id, Door* d, bool passage){
    id = _id;

    int x = d20();

    ///TODO fix this to the actual "beyond a door" table contents!
    if(x < 10 || passage)
        generatePassage(d);
    else
        generateChamber(d);

    next = NULL;
}

///TODO add other dungeon types -- currently this is just for the "stronghold" type
void Room::setPurpose(){
    int x = d100();
    switch(x){
        case 1: case 2:
            purpose_short = "antechamber";
            purpose_full = "where visitors seeking access to the stronghold wait.";
            break;
        case 3: case 4: case 5:
            purpose_short = "armory";
            purpose_full = "holding high-quality gear, including light siege weapons (such as ballistas).";
            break;
        case 6:
            purpose_short = "audience chamber";
            purpose_full = "used by the master of the stronghold to recieve visitors.";
            break;
        case 7:
            if(rand()%2){
                purpose_short = "aviary";
                purpose_full = "for keeping exotic birds.";
            } else {
                purpose_short = "zoo";
                purpose_full = "for keeping exotic creatures.";
            }
            break;
        case 8: case 9: case 10: case 11:
            purpose_short = "banquet room";
            purpose_full = "for hosting celebrations and guests.";
            break;
        case 12: case 13: case 14: case 15:
            purpose_short = "barracks";
            purpose_full = "used by elite guards.";
            break;
        case 16:
            purpose_short = "bath";
            purpose_full = "outfitted with a marble floor and other luxurious accoutrements.";
            break;
        case 17:
            purpose_short = "bedroom";
            purpose_full = "for use by the stronghold's master or important guests.";
            break;
        case 18:
            purpose_short = "chapel";
            purpose_full = "dedicated to a deity associated with the stronghold's master.";
            break;
        case 19: case 20: case 21:
            purpose_short = "cistern";
            purpose_full = "providing drinking water for the stronghold.";
            break;
        case 22: case 23: case 24: case 25:
            purpose_short = "dining room";
            purpose_full = "for intimate gatherings or informal meals.";
            break;
        case 26:
            purpose_short = "dressing room";
            purpose_full = "featuring a number of wardrobes.";
            break;
        case 27: case 28: case 29:
            purpose_short = "gallery";
            purpose_full = "for the display of expensive works of art and trophies.";
            break;
        case 30: case 31: case 32:
            purpose_short = "game room";
            purpose_full = "used to entertain visitors.";
            break;
        case 33: case 34: case 35: case 36: case 37: case 38: case 39: case 40: case 41:
        case 42: case 43: case 44: case 45: case 46: case 47: case 48: case 49: case 50:
            purpose_short = "guardroom";
            purpose_full = "for the stronghold's typical guards.";
            break;
        case 51:
            purpose_short = "kennel";
            purpose_full = "where monsters and trained animals that protect the stronghold are kept.";
            break;
        case 52: case 53: case 54: case 55: case 56: case 57:
            purpose_short = "kitchen";
            purpose_full = "designed to prepare exotic foods for large numbers of guests.";
            break;
        case 58: case 59: case 60: case 61:
            purpose_short = "library";
            purpose_full = "with an extensive collection of rare books.";
            break;
        case 62:
            purpose_short = "lounge";
            purpose_full = "used to entertain guests.";
            break;
        case 63: case 64: case 65: case 66: case 67: case 68: case 69: case 70:
            purpose_short = "pantry";
            purpose_full = "including a cellar for wine and spirits.";
            break;
        case 71: case 72: case 73: case 74:
            purpose_short = "sitting room";
            purpose_full = "for family and intimate guests.";
            break;
        case 75: case 76: case 77: case 78:
            purpose_short = "stable";
            purpose_full = "for the stronghold's mounts.";
            break;
        case 79: case 80: case 81: case 82: case 83: case 84: case 85: case 86:
            purpose_short = "storage room";
            purpose_full = "for holding mundane goods and supplies.";
            break;
        case 87: ///TODO FIXME this should have a 75% chance of being behind a secret door!
            purpose_short = "vault";
            purpose_full = "for protecting important treasures.";
            break;
        case 88: case 89: case 90: case 91: case 92:
            purpose_short = "study";
            purpose_full = "including a writing desk.";
            break;
        case 93:
            purpose_short = "throne room";
            purpose_full = "elaborately decorated.";
            break;
        case 94: case 95: case 96:
            purpose_short = "waiting room";
            purpose_full = "where lesser guests are held before recieving an audience.";
            break;
        case 97: case 98:
            purpose_short = "latrine";
            purpose_full = "for public use.";
            break;
        default:
            purpose_short = "crypt";
            purpose_full = "belonging to the stronghold's master (or someone else of importance).";
            break;
    }

}
