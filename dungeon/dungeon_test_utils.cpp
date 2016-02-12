
#include <iostream>
#include <dungeon.h>

using namespace std;


//for testing: adds a room to the first unattached door along the specified wall
void Dungeon::add_room(int wall, int x, int y){
    Door* d = NULL;
    for(int i = 0; i < MAX_DOORS && cur_room->doors[i] != NULL; i++){
        if(cur_room->doors[i]->getWall(cur_room) == wall && (cur_room->doors[i]->first == NULL || cur_room->doors[i]->second == NULL)){
            d = cur_room->doors[i];
            break;
        }
    }
    if(d != NULL){
        Room* next_rm = new Room(cur_id++, d, x, y);
        last_room->next = next_rm;
        last_room = next_rm;
        cur_room = next_rm;
        cur_room->doors[0] = d;
        d->second = cur_room;
        adjust_room_position(cur_room);
    } else {
        cout << "ERROR door not found" << endl;
        exit(EXIT_FAILURE);
    }
}
