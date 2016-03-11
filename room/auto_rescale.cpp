#include <iostream>
#include <room.h>

using namespace std;


void Room::shift(int delta_x, int delta_y){
    shuffle_bounds(delta_y, delta_y, delta_x, delta_x);
}

void Room::shrink(int direction, int amount){
    int dN = (direction == SOUTH ? amount * -1 : 0);
    int dS = (direction == NORTH ? amount      : 0);
    int dE = (direction == WEST  ? amount * -1 : 0);
    int dW = (direction == EAST  ? amount      : 0);
    shuffle_bounds(dN, dS, dE, dW);
}

///@TODO don't shuffle entry door position!
void Room::shuffle_bounds(int dN, int dS, int dE, int dW){
    // Shift room boundary
    northEdge += dN;
    southEdge += dS;
    eastEdge  += dE;
    westEdge  += dW;
    xDim = eastEdge - westEdge;
    yDim = northEdge - southEdge;

    // Shuffle doors
    for(int i = 0; i < MAX_DOORS; i++){
        if(doors[i]){
            // shift along same wall
            if(doors[i]->second) continue;
            switch(doors[i]->getWall(this)){
                case NORTH: case SOUTH:
                    doors[i]->xPos = (1+ (dE-dW)/(eastEdge - westEdge))*(doors[i]->xPos - westEdge)  + westEdge  + dW;
                    break;
                case EAST: case WEST:
                    doors[i]->yPos = (1+ (dN-dS)/(northEdge-southEdge))*(doors[i]->yPos - southEdge) + southEdge + dS;
            }

            // shift with moving wall
            switch(doors[i]->getWall(this)){
                case NORTH: doors[i]->yPos += dN; break;
                case SOUTH: doors[i]->yPos += dS; break;
                case EAST:  doors[i]->xPos += dE; break;
                case WEST:  doors[i]->xPos += dW; break;
            }
        }
    }
}

bool Room::try_shrink(int dir, Room* compare, Door* entry){
    int delta = 0;
    int dN=0, dS=0, dE=0, dW=0;
    while(true) {

        delta++;
        switch(dir){
            case NORTH: dS++;  break;
            case SOUTH: dN--;  break;
            case EAST:  dW++;  break;
            case WEST:  dE--;  break;
        }

        // make sure you haven't shrunk to size 0
        if(northEdge + dN == southEdge + dS || eastEdge + dE == westEdge + dW)
            return false;

        // ensure entry door is still valid
        if(!bordering(entry, dN, dS, dE, dW))
            return false;

        // check if issue resolved
        if(!issue_shrink(compare, dir, delta)){
            if(DEBUG) cout << "shrinking " << to_string(dir) << " by " << delta << "." << endl;
            shrink(dir, delta);
            return true;
        }

    }
    return false;
}

bool Room::try_shift(int dir, Room* compare, Door* entry){
    int delta_x = 0, delta_y = 0;
    while(true) {
        //update deltas
        switch(dir){
            case NORTH: delta_y++; break;
            case SOUTH: delta_y--; break;
            case EAST:  delta_x++; break;
            default:    delta_x--; break;
        }

        // ensure entry door is still valid
        if(!bordering(entry, delta_x, delta_y))
            return false;

        // check if issue resolved
        if(!issue_shift(compare, delta_x, delta_y)){
            if(DEBUG) cout << "shifting (delta_x = " << delta_x << ", delta_y = " << delta_y << "." << endl;
            shift(delta_x, delta_y);
            return true;
        }
    }
    // should never reach, but here to avoid warnings =]
    cout << "Something's messed up. You shouldn't have been able to reach here! (try_shift)" << endl;
    return false;
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
        if(DEBUG) cout << "\t- CASE 2: Adding door @ index " << cur_door << endl;
    }

    // If we're already at MAX_DOORS, try to find an unconnected one to overwrite
    if(cur_door == MAX_DOORS){
        for(cur_door = 0; cur_door < MAX_DOORS; cur_door++){
            if(doors[cur_door]->second == NULL && (can_delete || doors[cur_door]->secret)) {
                if(DEBUG) cout << "\t- CASE 2: Deleting/overwriting door @ index " << cur_door << endl;
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

// returns 0 if rooms are not adjacent, or, NORTH/SOUTH/EAST/WEST otherwise (whichever of the caller room's walls they share)
int Room::shared_wall(Room* r){
    if(northEdge == r->southEdge && !(r->eastEdge <= westEdge || r->westEdge >= eastEdge))
        return NORTH;
    else if(southEdge == r->northEdge && !(r->eastEdge <= westEdge || r->westEdge >= eastEdge))
        return SOUTH;
    else if(eastEdge == r->westEdge && !(r->northEdge <= southEdge || r->southEdge >= northEdge))
        return EAST;
    else if(westEdge == r->eastEdge && !(r->northEdge <= southEdge || r->southEdge >= northEdge))
        return WEST;
    else
        return 0;
}

///@TODO: Finish this function!
// wall = from caller room
//void Room::link_doors(Room* adjacent, int wall){
//    if(wall == 0) return;
//    cout << "-- Attempting to link room " << id << " with room " << adjacent->id << "!" << endl;
//
//    // Populate lists of all relevant doors for both rooms
//    vector<Door*> new_rm_doors;
//    vector<Door*> adj_rm_doors;
//    for(int i = 0; i < MAX_DOORS && doors[i] != NULL; i++){
//        if(doors[i]->firstWall == wall
//           && adjacent->bordering(doors[i]->xPos, doors[i]->yPos)
//           && doors[i]->within_bounds(adjacent)
//           && doors[i]->second == NULL){
//            new_rm_doors.push_back(doors[i]);
//        }
//    }
//    for(int i = 0; i < MAX_DOORS && adjacent->doors[i] != NULL; i++){
//        if(adjacent->doors[i]->getWall(adjacent) == opposite(wall)
//           && bordering(adjacent->doors[i]->xPos, adjacent->doors[i]->yPos)
//           && adjacent->doors[i]->within_bounds(this)
//           && adjacent->doors[i]->second == NULL)
//            {
//                adj_rm_doors.push_back(adjacent->doors[i]);
//            }
//    }
//
//    // Case 0: Easy mode, bare wall between the two rooms
//    if(new_rm_doors.empty() && adj_rm_doors.empty()) {
//        cout << "\t- Easy mode: no doors to align =]" << endl;
//        return;
//    }
//
//    // Case 1: Both rooms have connected doors.
//    //      - As long as both rooms have connected doors, replace ones from this room with ones from the adjacent room
//    //      - If there's an unequal number, you'll run out of one early and be caught by one of the later cases
//    while(!new_rm_doors.empty() && !adj_rm_doors.empty()){
//        Door* cur = new_rm_doors.back();
//        int idx;
//        for(idx = 0; idx < MAX_DOORS; idx++){
//            if(doors[idx]->xPos == cur->xPos && doors[idx]->yPos == cur->yPos){
//                // remove the door from this room that would have gone along that wall
//                cout << "\t- CASE 1: Cur door @(" << cur->xPos << ", " << cur->yPos
//                     << ") replaced with adj door @(" << adj_rm_doors.back()->xPos << ", " << adj_rm_doors.back()->yPos << ")" << endl;
//                delete doors[idx];
//
//                // add the connecting door from the adjacent room back into its spot
//                doors[idx] = adj_rm_doors.back();
//                adj_rm_doors.back()->second = this;
//
//                // clear those two from our list and move on
//                new_rm_doors.pop_back();
//                adj_rm_doors.pop_back();
//                break;
//            }
//        }
//    }
//
//    // Case 2: The other room has at least one door, but the newly added room doesn't (yet):
//    //      just add it to this room's door list. (We'll be at one more door than originally intended... oh well)
//    if(new_rm_doors.empty() && !adj_rm_doors.empty()){
//        for(vector<Door*>::iterator it = adj_rm_doors.begin(); it != adj_rm_doors.end(); ++it){
//            int idx = find_overwrite_index();
//            doors[idx] = (*it);
//            (*it)->second = this; //to get here, the door in question must have been unconnected before now
//        }
//    }
//
//    // Case 3: The new room has at least one door, but the other room doesn't.
//    //      3a) If there's wallspace the rooms don't share, shift the door(s) down there.
//    //      3b) If not, change each connected door's type to <secret>, and add them to the previous room as well (find_overwrite_index with can_delete=false).
//    ///@TODO: IMPLEMENT THIS
//    if(!new_rm_doors.empty() && adj_rm_doors.empty()){
//        cout << "Shit! Haven't implemented this yet. Sorry. " << endl
//             << "Short version is, your rooms got weirdly connected and the program can't handle that right now, so, exiting!" << endl
//             << "Hope you had fun before now, at least =]" << endl << endl;
//        exit(EXIT_FAILURE);
//    }
//}


void Room::link_doors(Room* adjacent, int wall){
    if(wall == 0) return;
    if(DEBUG) cout << "-- Attempting to link room " << id << " with room " << adjacent->id << "!" << endl;

    // Populate lists of all relevant doors for both rooms
    vector<Door*> adj_rm_doors;
    for(int i = 0; i < MAX_DOORS && adjacent->doors[i] != NULL; i++){
        if(adjacent->doors[i]->getWall(adjacent) == opposite(wall)
           && bordering(adjacent->doors[i]->xPos, adjacent->doors[i]->yPos)
           && adjacent->doors[i]->within_bounds(this)
           && adjacent->doors[i]->second == NULL)
            {
                adj_rm_doors.push_back(adjacent->doors[i]);
            }
    }

    // Case 0: Easy mode, bare wall between the two rooms
    if(adj_rm_doors.empty()) {
        if(DEBUG) cout << "\t- Easy mode: no doors to align =]" << endl;
        return;
    }


    // Case 2: The other room has at least one door, but the newly added room doesn't (yet):
    //      just add it to this room's door list. (We'll be at one more door than originally intended... oh well)
    for(vector<Door*>::iterator it = adj_rm_doors.begin(); it != adj_rm_doors.end(); ++it){
        int idx = find_overwrite_index();
        doors[idx] = (*it);
        (*it)->second = this; //to get here, the door in question must have been unconnected before now
    }
}

/*
 * Returns true iff the caller room's xth edge is overlapped by the parameter room.
 * The caller room is shifted by the offset deltas.
 * delta_x is positive when shifting east / negative west; y is positive north / negative south.
 * (It pains me to have so much repeated code like this, but with small-but-nonobvious differences
 *    between them, I actually can't think of a worthwhile way to collapse, given that there's only
 *    four different options total.)
 */
 bool Room::issue(int direction, Room* r){
    switch (direction) {
        case NORTH:  return issue_north(r, 0, 0, 0, 0);
        case SOUTH:  return issue_south(r, 0, 0, 0, 0);
        case EAST:   return issue_east(r, 0, 0, 0, 0);
        case WEST:   return issue_west(r, 0, 0, 0, 0);
        default:     return false;
    }
}

 bool Room::issue_shift(Room* r, int x, int y){
    return issue_north(r, y, y, x, x) ||
           issue_south(r, y, y, x, x) ||
           issue_east(r, y, y, x, x)  ||
           issue_west(r, y, y, x, x);
}

 bool Room::issue_shrink(Room* r, int shrink_direction, int shrink_amt){
    int n = (shrink_direction == SOUTH ? shrink_amt * -1 : 0);
    int s = (shrink_direction == NORTH ? shrink_amt      : 0);
    int e = (shrink_direction == WEST  ? shrink_amt * -1 : 0);
    int w = (shrink_direction == EAST  ? shrink_amt      : 0);

    return issue_north(r, n, s, e, w) ||
           issue_south(r, n, s, e, w) ||
           issue_east(r, n, s, e, w)  ||
           issue_west(r, n, s, e, w);
}

bool Room::issue_north(Room* r, int delta_n, int delta_s, int delta_e, int delta_w){
    return
        r->northEdge >= northEdge + delta_n &&
        r->southEdge < northEdge + delta_n &&
        !(r->eastEdge <= westEdge + delta_w || r->westEdge >= eastEdge + delta_e);
}

bool Room::issue_south(Room* r, int delta_n, int delta_s, int delta_e, int delta_w){
    return
        r->southEdge <= southEdge + delta_s &&
        r->northEdge > southEdge + delta_s &&
        !(r->eastEdge <= westEdge + delta_w || r->westEdge >= eastEdge + delta_e);
}

bool Room::issue_east(Room* r, int delta_n, int delta_s, int delta_e, int delta_w){
    return
        r->eastEdge >= eastEdge + delta_e &&
        r->westEdge < eastEdge + delta_e &&
        !(r->northEdge <= southEdge + delta_s || r->southEdge >= northEdge + delta_n);
}

bool Room::issue_west(Room* r, int delta_n, int delta_s, int delta_e, int delta_w){
    return
        r->westEdge <= westEdge + delta_w &&
        r->eastEdge > westEdge + delta_w &&
        !(r->northEdge <= southEdge + delta_s || r->southEdge >= northEdge + delta_n);
}

// strict lt. means no doors on corners
bool Room::bordering(int x, int y){
    return
        ((x == eastEdge || x == westEdge) && y < northEdge && y > southEdge)
        ||
        ((y == northEdge || y == southEdge) && x < eastEdge && x > westEdge);
}

bool Room::bordering(Door* d, int delta_x, int delta_y){
    return bordering(d, delta_y, delta_y, delta_x, delta_x);
}

bool Room::bordering(Door* d, int delta_n, int delta_s, int delta_e, int delta_w){
    return
        ((d->xPos == eastEdge + delta_e || d->xPos == westEdge + delta_w) && d->yPos < northEdge + delta_n && d->yPos > southEdge + delta_s)
        ||
        ((d->yPos == northEdge + delta_n || d->yPos == southEdge + delta_s) && d->xPos < eastEdge + delta_e && d->xPos > westEdge + delta_w);
}
