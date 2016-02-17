
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