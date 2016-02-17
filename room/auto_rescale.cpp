#include <iostream>
#include <room.h>

using namespace std;

/*
 * Shift the rooms coordinates by <amt>, in the x direction (if <dim_x>) or the y direction (otherwise)
 * Shifts north/east when rev(erse) is false, and south/west when true
 */
void Room::shift(int amt, bool dim_x, bool rev){

    cout << "----shifting by " << amt << " dim_x = " << dim_x << endl;

    //shuffle doors
    for(int i = 0; i < MAX_DOORS; i++){
        if(doors[i] != NULL){
            if(dim_x)
                if(rev)
                    doors[i]->xPos -= amt;
                else
                    doors[i]->xPos += amt;
            else
                if(rev)
                    doors[i]->yPos -= amt;
                else
                    doors[i]->yPos += amt;
        }
    }
    //shuffle room bounds
    if(dim_x){
        if(rev){
            eastEdge -= amt;
            westEdge -= amt;
        } else {
            eastEdge += amt;
            westEdge += amt;
        }
    } else {
        if(rev){
            northEdge -= amt;
            southEdge -= amt;
        } else {
            northEdge += amt;
            southEdge += amt;
        }
    }
}

////if dim_x is true, holds west edge constant and shrinks east
////if dim_x is false, holds south edge constant and shrinks north
//void Room::shrink(int amt, bool dim_x){
//    //shuffle doors
//    for(int i = 0; i < MAX_DOORS; i++){
//        if(doors[i] != NULL){
//            float position; //% location of door along wall
//            if(dim_x) {
//                position = (float)(doors[i]->xPos - westEdge)/(eastEdge - westEdge);
//                doors[i]->xPos = westEdge + (eastEdge - westEdge - amt)*position;
//            } else {
//                position = (float)(doors[i]->yPos - southEdge)/(northEdge - southEdge);
//                doors[i]->yPos = southEdge + (northEdge - southEdge - amt)*position;
//            }
//        }
//    }
//    //shuffle room bounds
//    if(dim_x){
//        eastEdge -= amt;
//        xDim -= amt;
//    } else {
//        northEdge -= amt;
//        yDim -= amt;
//    }
//}

// holds <const_wall> constant and shifts its opposite inward by <amt>
// **note that amt must always be positive here (unlike for shift)!!
void Room::shrink(unsigned amt, int const_wall){
    cout << "---Shrinking by " << amt << ": Const wall = " << const_wall << endl;
    //shuffle doors
    for(int i = 0; i < MAX_DOORS; i++){
        if(doors[i] != NULL){

            // if door is linked to another door already, don't shift it
            if(doors[i]->first != NULL && doors[i]->second != NULL){
                /// @TODO: unless not shifting it pushes it outside the room, in which case de-link it
            } else {

                float position; //% location of door along wall
                int door_wall = doors[i]->getWall(this);
                switch(const_wall){
                    case WEST:
                        if(door_wall == EAST){
                            doors[i]->xPos -= amt;
                        } else if(door_wall == NORTH || door_wall == SOUTH){
                            position = (float)(doors[i]->xPos - westEdge)/(eastEdge - westEdge);
                            doors[i]->xPos = westEdge + (eastEdge - westEdge - amt)*position;
                        }
                        break;
                    case EAST:
                        if(door_wall == WEST){
                            doors[i]->xPos += amt;
                        } else if(door_wall == NORTH || door_wall == SOUTH){
                            position = (float)(doors[i]->xPos - westEdge)/(eastEdge - westEdge);
                            doors[i]->xPos = westEdge + amt + (eastEdge - westEdge - amt)*position;
                        }
                        break;
                    case SOUTH:
                        if(door_wall == NORTH){
                            doors[i]->yPos -= amt;
                        } else if(door_wall == EAST || door_wall == WEST){
                            position = (float)(doors[i]->yPos - southEdge)/(northEdge - southEdge);
                            doors[i]->yPos = southEdge + (northEdge - southEdge - amt)*position;
                        }
                        break;
                    case NORTH:
                        if(door_wall == SOUTH){
                            doors[i]->yPos += amt;
                        } else if(door_wall == EAST || door_wall == WEST){
                            position = (float)(doors[i]->yPos - southEdge)/(northEdge - southEdge);
                            doors[i]->yPos = southEdge + amt + (northEdge - southEdge - amt)*position;
                        }
                }
            }
        }
    }
    //shuffle room bounds
    switch(const_wall){
        case NORTH:
            southEdge += amt;
            yDim -= amt;
            return;
        case SOUTH:
            northEdge -= amt;
            yDim -= amt;
            return;
        case EAST:
            westEdge += amt;
            xDim -= amt;
            return;
        case WEST:
            eastEdge -= amt;
            xDim -= amt;
            return;
    }
}
void Room::remove_connections(int wall){
    // beginning at 1: don't want to remove initial entry door
    for(int i = 1; i < MAX_DOORS && doors[i] != NULL; i++){
        if(doors[i]->getWall(this) == wall && doors[i]->first != NULL && doors[i]->second != NULL){
            Room* to_remove_from;
            if(doors[i]->first->id == this->id){
                // was added to second/preexisting room (as a secret door): remove it from there (without deleting it overall; it's still in this rm)
                to_remove_from = doors[i]->second;
            } else {
                // inverse (remove from this room without deleting from the other)
                to_remove_from = this;
            }

            int d = 0;
            while(!(to_remove_from->doors[d]->xPos == doors[i]->xPos && to_remove_from->doors[d]->yPos == doors[i]->yPos)) { d++; }
            if(d < MAX_DOORS) {
                to_remove_from->doors[d] = NULL;

                // Fixes an obscure, realistically-will-never-happen bug:
                //   if that room was at MAX_DOORS and this secret door was added by deleting another secret door,
                //   there could be doors after it in sequence that will need to be shifted down
                //   (otherwise they'll get dropped, since everything loops until you hit the first NULL door)
                for(++d; d < MAX_DOORS && to_remove_from->doors[d] != NULL; d++){
                    to_remove_from->doors[d-1] = to_remove_from->doors[d];
                    to_remove_from->doors[d] = NULL;
                }
            }
        }
    }
}


/*
 * Helper function for linkDoors algorithm.
 * When a door needs to be added to a room in order to connect it to a door from an adjacent room,
 *   this function returns the index into the doors array of the new room (where that door should be added.)
 * The index returned will always point to a NULL door.
 * If can_delete is true, the function will try deleting any unconnected doors if it's impossible to add another door.
 *    Otherwise, it will only delete unconnected secret doors.
 */
int Room::find_overwrite_index(bool can_delete){
    // Try to find a blank door first
    int cur_door;
    for(cur_door = 0; cur_door < MAX_DOORS && doors[cur_door] != NULL; cur_door++){}

    if(doors[cur_door] == NULL){
        cout << "\t- CASE 2: Adding door @ index " << cur_door << endl;
    }

    // If we're already at MAX_DOORS, try to find an unconnected one to overwrite
    if(cur_door == MAX_DOORS){
        for(cur_door = 0; cur_door < MAX_DOORS; cur_door++){
            if(doors[cur_door]->second == NULL && (can_delete || doors[cur_door]->secret)) {
                cout << "\t- CASE 2: Deleting/overwriting door @ index " << cur_door << endl;
                delete doors[cur_door];
                doors[cur_door] = NULL;
                break;
            }
        }
    }

    // Last-ditch error catch
    if(cur_door == MAX_DOORS){
        cout << "Oh dang. This really should not have happened. You somehow managed to make this dungeon non-euclidean. Congrats?" << endl;
        cout << "Downside is, I can't render that. Sorry. Quitting out now, please play again!" << endl << endl;
        exit(EXIT_FAILURE);
    }

    return cur_door;
}

///@TODO: Finish this function!
void Room::linkDoors(Room* adjacent, int wall){
    cout << "## Attempting to link room " << id << " with room " << adjacent->id << "!" << endl;

    // Populate lists of all relevant doors for both rooms
    vector<Door*> new_rm_doors;
    vector<Door*> adj_rm_doors;
    for(int i = 0; i < MAX_DOORS && doors[i] != NULL; i++){
        if(doors[i]->firstWall == wall
           && adjacent->bordering(doors[i]->xPos, doors[i]->yPos)
           && doors[i]->within_bounds(adjacent)){
            new_rm_doors.push_back(doors[i]);
        }
    }
    for(int i = 0; i < MAX_DOORS && adjacent->doors[i] != NULL; i++){
        if(adjacent->doors[i]->getWall(adjacent) == opposite(wall)
           && bordering(adjacent->doors[i]->xPos, adjacent->doors[i]->yPos)
           && adjacent->doors[i]->within_bounds(this))
            {
                adj_rm_doors.push_back(adjacent->doors[i]);
            }
    }

    // Case 0: Easy mode, bare wall between the two rooms
    if(new_rm_doors.empty() && adj_rm_doors.empty()) {
        cout << "\t- Easy mode: no doors to align =]" << endl;
        return;
    }

    // Case 1: Both rooms have connected doors.
    //      - As long as both rooms have connected doors, replace ones from this room with ones from the adjacent room
    //      - If there's an unequal number, you'll run out of one early and be caught by one of the later cases
    while(!new_rm_doors.empty() && !adj_rm_doors.empty()){
        Door* cur = new_rm_doors.back();
        int idx;
        for(idx = 0; idx < MAX_DOORS; idx++){
            if(doors[idx]->xPos == cur->xPos && doors[idx]->yPos == cur->yPos){
                // remove the door from this room that would have gone along that wall
                cout << "\t- CASE 1: Cur door @(" << cur->xPos << ", " << cur->yPos
                     << ") replaced with adj door @(" << adj_rm_doors.back()->xPos << ", " << adj_rm_doors.back()->yPos << ")" << endl;
                delete doors[idx];

                // add the connecting door from the adjacent room back into its spot
                doors[idx] = adj_rm_doors.back();
                adj_rm_doors.back()->second = this;

                // clear those two from our list and move on
                new_rm_doors.pop_back();
                adj_rm_doors.pop_back();
                break;
            }
        }
    }

    // Case 2: The other room has at least one door, but the newly added room doesn't (yet):
    //      just add it to this room's door list. (We'll be at one more door than originally intended... oh well)
    if(new_rm_doors.empty() && !adj_rm_doors.empty()){
        for(vector<Door*>::iterator it = adj_rm_doors.begin(); it != adj_rm_doors.end(); ++it){
            int idx = find_overwrite_index();
            doors[idx] = (*it);
            (*it)->second = this; //to get here, the door in question must have been unconnected before now
        }
    }

    // Case 3: The new room has at least one door, but the other room doesn't.
    //      3a) If there's wallspace the rooms don't share, shift the door(s) down there.
    //      3b) If not, change each connected door's type to <secret>, and add them to the previous room as well (find_overwrite_index with can_delete=false).
    ///@TODO: IMPLEMENT THIS
    if(!new_rm_doors.empty() && adj_rm_doors.empty()){
        cout << "Shit! Haven't implemented this yet. Sorry. " << endl
             << "Short version is, your rooms got weirdly connected and the program can't handle that right now, so, exiting!" << endl
             << "Hope you had fun before now, at least =]" << endl << endl;
        exit(EXIT_FAILURE);
    }
}


//shift on south/west, shrink on north/east
///TODO: one wall needs to be held constant (whichever door you came through). Make sure that happens!
//void Room::adjustPosition(Room* compare){
//    int offset = 0, shrink_amt = 0;
//
//    //south
//    while(compare->withinY(southEdge+offset)){
//        offset++;
//    }
//    if(offset != 0){
//        shift(offset, false);
//        linkDoors(compare, SOUTH);
//    }
//
//    //north
//    while(compare->withinY(northEdge-shrink_amt)){
//        shrink_amt++;
//    }
//    if(shrink_amt != 0){
//        shrink(shrink_amt, false);
//        linkDoors(compare, NORTH);
//    }
//
//    //reset for xDim
//    offset = 0;
//    shrink_amt = 0;
//
//    //west
//    while(compare->withinX(westEdge+offset)){
//        offset++;
//    }
//
//    if(offset != 0){
//        shift(offset, true);
//        linkDoors(compare, WEST);
//    }
//
//    //east
//    while(compare->withinX(eastEdge-shrink_amt)){
//        shrink_amt++;
//    }
//
//    if(shrink_amt != 0){
//        shrink(shrink_amt, true);
//        linkDoors(compare, EAST);
//    }
//
//}

///@TODO: clean these up!
// Returns true iff the parameter room's xth edge is causing a problem (within bounds for) the main caller room
bool Room::issue_north(Room* r, int offset){
    return
        r->northEdge-offset > southEdge &&
        (r->northEdge-offset < northEdge || r->southEdge-offset < southEdge) &&
        !(r->eastEdge <= westEdge || r->westEdge >= eastEdge);
}

bool Room::issue_south(Room* r, int offset){
    return
        r->southEdge+offset < northEdge &&
        (r->southEdge+offset > southEdge || r->northEdge+offset > northEdge) &&
        !(r->eastEdge <= westEdge || r->westEdge >= eastEdge);
}

bool Room::issue_east(Room* r, int offset){
    return
        r->eastEdge-offset > westEdge &&
        (r->eastEdge-offset < eastEdge || r->westEdge-offset < westEdge) &&
        !(r->southEdge >= northEdge || r->northEdge <= southEdge);
}

bool Room::issue_west(Room* r, int offset){
    return
        r->westEdge+offset < eastEdge &&
        (r->westEdge+offset > westEdge || r->eastEdge+offset > eastEdge) &&
        !(r->southEdge >= northEdge || r->northEdge <= southEdge);
}


bool Room::bordering(int x, int y){
    return
        x == eastEdge ||
        x == westEdge ||
        y == northEdge ||
        y == southEdge;
}

bool Room::bordering(Door* d){
    return
        d->xPos == eastEdge ||
        d->xPos == westEdge ||
        d->yPos == northEdge ||
        d->yPos == southEdge;
}
