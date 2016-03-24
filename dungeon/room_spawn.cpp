
#include <iostream>
#include <dungeon.h>
#include <dpg.h>
#include <monster.h>

using namespace std;


/*
 * Nb. This cannot adjust rooms that are perfectly overlapping others.
 *   However, if doors are connected properly on other room spawns, this should never be the case.
 */
void Dungeon::adjust_room_position(Room* rm){

    if(DEBUG) cout << "Adjusting position (original " << rm->location() << ")" << endl;
    int dir[] = {NORTH, SOUTH, EAST, WEST};

    map<int, bool> locked;
    for(int i = 0; i < 4; i++){ locked[ dir[i] ] = false; }
    locked[rm->doors[0]->getWall(rm)] = true;

    bool adjustments_needed;
    do {
        adjustments_needed = false;
        for(vector<Room*>::iterator it = rooms.begin(); it != rooms.end(); ++it){
            if((*it)->id == rm->id) continue;
            if(DEBUG) cout << "...checking room " << (*it)->id << ": ";

            // See which walls of the room to be adjusted (if any) are overlapped by *it
            bool issue_found = false;
            map<int, bool> overlap;
            for(int i = 0; i < 4; i++){
                if(DEBUG) cout << " " << to_string(dir[i]);
                overlap[ dir[i] ] = rm->issue(dir[i], *it);
                issue_found = issue_found || overlap[ dir[i] ];

                if(DEBUG) if(overlap[dir[i]]) cout << "**";
            }
            if(DEBUG) cout << " ";
            if(!issue_found) {
                if(DEBUG) cout << "OK" << endl;
                continue;
            }

            if(overlap[dir[0]] && overlap[dir[1]] && overlap[dir[2]] && overlap[dir[3]]){
                cout << "ERROR: perfectly overlapping room provided. Cannot adjust. Exiting." << endl;
                exit(EXIT_FAILURE);
            }

            adjustments_needed = true;
            bool fixed = false;
            Door* entry_door = rm->doors[0];

            // Try shifts in each direction without an overlapping wall
            for(int i = 0; i < 4; i++){
                if( !overlap[ dir[i] ] && !locked[ dir[i] ]){
                    if(rm->try_shift(dir[i], *it, entry_door)){
                       locked[ opposite(dir[i]) ] = true;
                       fixed = true;
                       break;
                    }
                }
            }

            // If still not fixed, need to shrink instead of shifting
            // note that "shrinking north" means the north wall is held constant while the south wall is moved north (&etc)
            if(!fixed){
                for(int i = 0; i < 4 && !fixed; i++){
                    if( !overlap[ dir[i] ] ){
                        if(rm->try_shrink(dir[i], *it, entry_door)){
                            locked[ opposite(dir[i]) ] = true;
                            fixed = true;
                            break;
                        }
                    } // end if
                } // end for
            } // end if (!fixed)
        } // end for (each room)
    } while(adjustments_needed);

    link_all_doors(rm);

}

void Dungeon::link_all_doors(Room* new_room){
    for(int i = 1; i < MAX_DOORS; i++){
        new_room->doors[i] = NULL;
    }

    for(vector<Room*>::iterator it = rooms.begin(); it != rooms.end(); ++it){
        if(new_room->id == (*it)->id) continue;
        new_room->link_doors(*it, new_room->shared_wall(*it));
    }
}


Room* Dungeon::add_room(Door* d, bool is_passage){
    Room* next_rm = new Room(dtype, d, is_passage);
    rooms.push_back(next_rm);
    if(!monsters_enabled()) next_rm->remove_all_monsters();
    next_rm->doors[0] = d;
    d->second = next_rm;
    adjust_room_position(next_rm);

    ///@TODO multi-monster support
    if(next_rm->monsters[0]){
        next_rm->monsters[0]->xPos = next_rm->westEdge + rand()%(next_rm->eastEdge-next_rm->westEdge-1) + 1;
        next_rm->monsters[0]->yPos = next_rm->southEdge + rand()%(next_rm->northEdge-next_rm->southEdge-1) + 1;
        if(DEBUG) cout << "Room " << next_rm->id << " -- " << next_rm->location();
        if(DEBUG) cout << " -- monster spawn @(" << next_rm->monsters[0]->xPos << "," << next_rm->monsters[0]->yPos << ")" << endl;
    }

    DoorPlacementGuide dpg(next_rm, this);
    dpg.spawn_doors();

    return next_rm;
}
