
#include <iostream>
#include <cstdlib>
#include <room.h>
#include <string>

using namespace std;


//Currently only used for the starting room
Door::Door(Room* room, int _wall, bool passage, bool main){
    initialize_door(room, _wall, passage, main);
}

//Currently used for all rooms except the starting room
///TODO add actual passage support
Door::Door(Room* room, int entrance){

    //position
    int wall;
    int x = d20();
    if(x < 8) wall = opposite(entrance);
    else if(x < 13) wall = left(entrance);
    else if (x < 18) wall = right(entrance);
    else wall = entrance;

    initialize_door(room, wall, rand()%2, false);
}


void Door::set_material(int x){
    if(x < 13) material = "wooden";
    else if (x < 15) material = "stone";
    else if (x < 17) material = "iron";
    else if (x < 19) material = "portcullis";
    else {
        material = "hidden";
        secret = true;
    }
}

//Set the coordinates for the door.
//Precondition: room edge coordinates are set
///TODO: multiple doors along the same wall should have different position coordinates!
void Door::set_position(Room* room){

    switch(firstWall){
        case SOUTH:
            xPos = room->westEdge + (room->eastEdge - room->westEdge)/2;
            yPos = room->southEdge;
            return;
        case NORTH:
            xPos = room->westEdge + (room->eastEdge - room->westEdge)/2;
            yPos = room->northEdge;
            return;
        case EAST:
            xPos = room->eastEdge;
            yPos = room->southEdge + (room->northEdge - room->southEdge)/2;
            return;
        case WEST:
            xPos = room->westEdge;
            yPos = room->southEdge + (room->northEdge - room->southEdge)/2;
    };
}


void Door::initialize_door(Room* room, int _wall, bool is_passage, bool is_main){
    first = room;
    second = NULL;

    //position
    firstWall = _wall;
    set_position(room);

    //defaults
    secret = false;
    barred = false;
    locked = false;

    //set material
    int x = d20();
    if(is_passage){
        material = "empty"; //opens to a passage
        locked = false;
        barred = false;
    } else set_material(x);

    //locked or barred
    switch(x){
        case 18:
            locked = true;
            break;
        case 11: case 12: case 14: case 16: case 20:
            if(rand()%2){
                locked = true;
            } else {
                barred = true;
                if(secret) barredSide = false;
                else barredSide = rand()%2;
            }
    }

    //is main exit
    if(is_main){
        main_exit = true;
        material = "dungeon exit";
        locked = false;
        barred = false;
        secret = false;
    } else {
        main_exit = false;
    }
}

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
    if(material == "wooden") return 15;
    else return 20;
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
