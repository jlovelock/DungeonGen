
#include <iostream>
#include <dungeon.h>

using namespace std;

//TODO clean up this function with subroutines, get rid of repetitiveness
///TODO: Ensure this works appropriately when multiple sequential adjustments are necessary! (So many test cases)
///TODO: when one room perfectly overlaps another, no wall will trigger as problematic
void Dungeon::adjust_room_position(Room* rm){

    cout << "...adjusting..." << endl;

    bool north_locked = false, south_locked = false, east_locked = false, west_locked = false;

    switch(rm->doors[0]->getWall(rm)){
        case NORTH:
            north_locked = true;
            break;
        case SOUTH:
            south_locked = true;
            break;
        case EAST:
            east_locked = true;
            break;
        case WEST:
            west_locked = true;
    }

    for(Room* compare = first_room; compare != NULL; compare = compare->next){
        if(compare->id == rm->id) continue;
        cout << "...checking room " << compare->id << ": " << endl;
        int offset = 0, shrink_amt = 0;

        //south
        while(!south_locked && compare->issue_south(rm, offset)){
            offset++;
        }
        if(offset != 0){
            cout << "@@@@@\tCORRECTION along South wall" << endl;
            cout << "@\t- Initial coords: S " << rm->southEdge << " / N " << rm->northEdge << " / W " << rm->westEdge << " / E " << rm->eastEdge << endl;
            cout << "@\t- Offset: " << offset << endl;

            if(south_locked){
                ///FIXME
                cout << "South double-locked! EXITING" << endl;
                exit(EXIT_FAILURE);

            } else if(north_locked){
                rm->shrink(offset, NORTH);
            } else {
                rm->shift(offset, false);
            }
            rm->linkDoors(compare, SOUTH);
            south_locked = true;
            cout << "@\t- Final coords: S " << rm->southEdge << " / N " << rm->northEdge << " / W " << rm->westEdge << " / E " << rm->eastEdge << endl;
            continue;
        }

        //north
        while(!north_locked && compare->issue_north(rm, shrink_amt)){
            shrink_amt++;
        }
        if(shrink_amt != 0){
            cout << "@@@@@\tCORRECTION along North wall" << endl;
            cout << "@\t- Initial coords: S " << rm->southEdge << " / N " << rm->northEdge << " / W " << rm->westEdge << " / E " << rm->eastEdge << endl;
            cout << "@\t- Offset: " << shrink_amt << endl;

            if(north_locked){
                ///FIXME
                cout << "North double-locked! EXITING" << endl;
                exit(EXIT_FAILURE);
            } else if(south_locked){
                rm->shrink(shrink_amt, SOUTH);
            } else {
                rm->shift(shrink_amt, false, true);
            }
            rm->linkDoors(compare, NORTH);
            north_locked = true;
            cout << "@\t- Final coords: S " << rm->southEdge << " / N " << rm->northEdge << " / W " << rm->westEdge << " / E " << rm->eastEdge << endl;
            continue;
        }

        //reset for xDim
        offset = 0;
        shrink_amt = 0;

        //west
        while(!west_locked && compare->issue_west(rm, offset)){
            offset++;
        }

        if(offset != 0){
            cout << "@@@@@\tCORRECTION along West wall" << endl;
            cout << "@\t- Initial coords: S " << rm->southEdge << " / N " << rm->northEdge << " / W " << rm->westEdge << " / E " << rm->eastEdge << endl;
            cout << "@\t- Offset: " << offset << endl;
            if(west_locked){
                ///FIXME
                cout << "West double-locked! EXITING" << endl;
                exit(EXIT_FAILURE);
            } else if(east_locked){
                rm->shrink(offset, EAST);
            } else {
                rm->shift(offset, true);
            }
            rm->linkDoors(compare, WEST);
            west_locked = true;
            cout << "@\t- Final coords: S " << rm->southEdge << " / N " << rm->northEdge << " / W " << rm->westEdge << " / E " << rm->eastEdge << endl;
            continue;
        }

        //east
        while(!east_locked && compare->issue_east(rm, shrink_amt)){
            shrink_amt++;
        }

        if(shrink_amt != 0){
            cout << "@@@@@\tCORRECTION along East wall" << endl;
            cout << "@\t- Initial coords: S " << rm->southEdge << " / N " << rm->northEdge << " / W " << rm->westEdge << " / E " << rm->eastEdge << endl;
            cout << "@\t- Offset: " << shrink_amt << endl;

            if(east_locked){
                ///FIXME
                cout << "East double-locked! EXITING" << endl;
                exit(EXIT_FAILURE);
            } else if(west_locked){
                rm->shrink(shrink_amt, WEST);
            } else {
                rm->shift(shrink_amt, true, true);
            }
            rm->linkDoors(compare, EAST);
            east_locked = true;
            cout << "@\t- Final coords: S " << rm->southEdge << " / N " << rm->northEdge << " / W " << rm->westEdge << " / E " << rm->eastEdge << endl;
            continue;
        }
    }
    cout << "...done" << endl;
}

void Dungeon::add_room(Door* d, bool is_passage){
    Room* next_rm = new Room(dungeon_type, cur_id++, d, is_passage);
    if(!monsters_enabled) next_rm->remove_all_monsters();
    if(treasure_enabled == "ALWAYS") next_rm->has_treasure = true;
    else if(treasure_enabled == "NEVER") next_rm->has_treasure = false;
    last_room->next = next_rm;
    last_room = next_rm;
    cur_room = next_rm;
    cur_room->doors[0] = d;
    d->second = cur_room;
    adjust_room_position(cur_room);
}
