
#include <iostream>
#include <door.h>

using namespace std;

int Door::max_id = 0;

bool Door::within_bounds(Room* r){

    // check x position
    if(firstWall == NORTH || firstWall == SOUTH){
        return xPos >= r->westEdge && xPos <= r->eastEdge;

    // check y position
    } else {
        return yPos >= r->southEdge && yPos <= r->northEdge;
    }
}

