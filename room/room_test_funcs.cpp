#include <room.h>

using namespace std;

Room::Room(int _id, Door* d, int x, int y, bool is_large){
    id = _id;
    next = NULL;
    has_treasure = false;
    is_passage = false;
    hazard = "";
    for(int i = 0; i < MAX_MONSTERS; i++)
        monsters[i] = NULL;

    xDim = x;
    yDim = y;
    shape = "rectangular";
    setEdgeCoords(opposite(d->firstWall),d->xPos,d->yPos);

    for(int i = 1; i < MAX_DOORS; i++){
        doors[i] = NULL;
    }
}

void Room::add_door(int wall){
    int i = 0;
    while(i < MAX_DOORS && doors[i] != NULL){ i++; }
    doors[i] = new Door(this, wall, false);
}

void Room::add_exits(int num_exits, int walls[]){
    for(int i = 0; i < num_exits; i++){
        add_door(walls[i]);
    }
}

