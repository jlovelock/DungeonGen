
#include <iostream>
#include <string>
#include <door.h>

using namespace std;


int opposite(int wall){
    if(wall == NORTH) return SOUTH;
    if(wall == SOUTH) return NORTH;
    if(wall == EAST) return WEST;
    if(wall == WEST) return EAST;
    return -1;
}

int left(int wall){
    if(wall == NORTH) return EAST;
    if(wall == EAST) return SOUTH;
    if(wall == SOUTH) return WEST;
    if(wall == WEST) return NORTH;
    return -1;
}

int right(int wall){
    if(wall == NORTH) return WEST;
    if(wall == WEST) return SOUTH;
    if(wall == SOUTH) return EAST;
    if(wall == EAST) return NORTH;
    return -1;
}

int Door::break_DC(){
    if(material == "wooden") return 10;
    else return 15;
}

string Door::getWallString(Room* rm){
    if(firstWall == NORTH) return isFirstRoom(rm) ? "north" : "south";
    else if(firstWall == SOUTH) return isFirstRoom(rm) ? "south" : "north";
    else if(firstWall == EAST) return isFirstRoom(rm) ? "east" : "west";
    else return isFirstRoom(rm) ? "west" : "east";
}

int Door::getWall(Room* rm){
    if(firstWall == NORTH) return isFirstRoom(rm) ? NORTH : SOUTH;
    else if(firstWall == SOUTH) return isFirstRoom(rm) ? SOUTH : NORTH;
    else if(firstWall == EAST) return isFirstRoom(rm) ? EAST : WEST;
    else return isFirstRoom(rm) ? WEST : EAST;
}

bool Door::isFirstRoom(Room* rm){
    return this->first->id == rm->id;
}

void Door::break_down(){
    material = "broken " + material;
    locked = false;
}

bool Door::obstructed_from(Room* r){
    return locked || (barred && (barredSide xor (r->id == first->id)));
}

// returns true iff it's barred from the side of r
// (ie, you can remove the bar if you're inside r)
bool Door::barred_from(Room* r){
    return barred && (barredSide xor (r->id != first->id));
}

string to_string(int dir){
    switch(dir){
        case NORTH: return "north";
        case SOUTH: return "south";
        case EAST: return "east";
        case WEST: return "west";
        default: return "undefined";
    }
}


void Door::set_defaults(){
    // defaults
    locked = false;
    barred = false;
    secret = false;
}
