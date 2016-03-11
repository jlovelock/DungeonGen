
#include <dpg.h>

using namespace std;


int rollExits(bool large){
    int x = d20();
    switch(x){
        case 1: case 2: case 3:
            return 0;
        case 4: case 5:
            return large ? 1 : 0;
        case 6: case 7: case 8:
            return 1;
        case 9: case 10: case 11:
            return large ? 2 : 1;
        case 12: case 13:
            return 2;
        case 14: case 15:
            return large ? 3 : 2;
        case 16: case 17:
            return 3;
        case 18:
            return large ? 4 : 3;
        case 19:
            return large ? 5 : 4;
        case 20:
            return large ? 6 : 4;
    }
    return -1;
}


/*
 * Precondition: the room has no unconnected doors.
 * DPG objects should be instantiated after the room is position-adjusted and existing doors have been linked,
 *   but before new doors are spawned.
 */
DoorPlacementGuide::DoorPlacementGuide(Room* rm, Dungeon* d){

    r = rm;

    if(DEBUG) cout << "Creating new DPG for room " << r->id << "..." << endl;

    //Initially, all along each wall is legal: downgrade/forbid sections later.
    preferred[NORTH].push_back(make_pair(r->westEdge, r->eastEdge));
    preferred[SOUTH].push_back(make_pair(r->westEdge, r->eastEdge));
    preferred[EAST].push_back(make_pair(r->southEdge, r->northEdge));
    preferred[WEST].push_back(make_pair(r->southEdge, r->northEdge));

    allowed[NORTH].push_back(make_pair(r->westEdge, r->eastEdge));
    allowed[SOUTH].push_back(make_pair(r->westEdge, r->eastEdge));
    allowed[EAST].push_back(make_pair(r->southEdge, r->northEdge));
    allowed[WEST].push_back(make_pair(r->southEdge, r->northEdge));

    if(DEBUG)
    {
        cout << "Starting values:" << endl;

        int dir[] = {NORTH, SOUTH, EAST, WEST};
        for(int i = 0; i < 4; i++){
            cout << to_string(dir[i]) << " wall:" << endl;

            cout << "preferred = ";
            for(vector< pair<int,int> >::iterator it = preferred[dir[i]].begin(); it != preferred[dir[i]].end(); ++it){
                cout << " [" << it->first << "," << it->second << "] ";
            }
            cout << "  //  allowed = ";
            for(vector< pair<int,int> >::iterator it = allowed[dir[i]].begin(); it != allowed[dir[i]].end(); ++it){
                cout << " [" << it->first << "," << it->second << "] ";
            }
            cout << endl;
        }
    }


    // loop through each room in the dungeon, forbidding all adjacent areas corresponding to existing rooms
    int w = 0;
    for(vector<Room*>::iterator it = d->rooms.begin(); it != d->rooms.end(); ++it){
        w = r->shared_wall(*it);
        if(w && DEBUG){
            cout << "--Room " << (*it)->id << " adjacent along " << to_string(w) << " wall." << endl;
        }
        if(w == NORTH || w == SOUTH){
            forbid(w, (*it)->westEdge, (*it)->eastEdge);
        } else if(w == EAST || w == WEST){
            forbid(w, (*it)->southEdge, (*it)->northEdge);
        }
    }

    if(DEBUG)
    {
        cout << "After initial pass:" << endl;

        int dir[] = {NORTH, SOUTH, EAST, WEST};
        for(int i = 0; i < 4; i++){
            cout << to_string(dir[i]) << " wall:" << endl;

            cout << "preferred = ";
            for(vector< pair<int,int> >::iterator it = preferred[dir[i]].begin(); it != preferred[dir[i]].end(); ++it){
                cout << " [" << it->first << "," << it->second << "] ";
            }
            cout << "  //  allowed = ";
            for(vector< pair<int,int> >::iterator it = allowed[dir[i]].begin(); it != allowed[dir[i]].end(); ++it){
                cout << " [" << it->first << "," << it->second << "] ";
            }
            cout << endl;
        }
    }

}

int DoorPlacementGuide::roll_wall(){
    int entrance = r->doors[0]->getWall(r);
    int x = d20();
    if(x < 8) return opposite(entrance);
    else if(x < 13) return left(entrance);
    else if (x < 18) return right(entrance);
    else return entrance;
}

bool DoorPlacementGuide::valid_position_exists(){
    return !(allowed[NORTH].empty() && allowed[SOUTH].empty() && allowed[EAST].empty() && allowed[WEST].empty());
}

void DoorPlacementGuide::spawn_door(){

    if(!valid_position_exists()){
        if(DEBUG) cout << "Door will not be added (no valid position exists)." << endl;
        return;
    }

    int wall;
    do {
        wall = roll_wall();
    } while(allowed[wall].empty());

    int x, y;
    switch(wall){
        case NORTH: y = r->northEdge; break;
        case SOUTH: y = r->southEdge; break;
        case EAST:  x = r->eastEdge;  break;
        case WEST:  x = r->westEdge;  break;
    }

    // place at midpoint of first preferred region (if possible) or allowed region (if not)
    int a, b;
    if(!preferred[wall].empty()){
        a = preferred[wall].front().first;
        b = preferred[wall].front().second;
        //pos = (preferred[wall].front().first + preferred[wall].front().second) / 2 + preferred[wall].front().first;
    } else if(!allowed[wall].empty()){
        a = allowed[wall].front().first;
        b = allowed[wall].front().second;
        //pos = (allowed[wall].front().first + allowed[wall].front().second) / 2 + allowed[wall].front().first;
    }
    int pos = (b-a)/2+a;
    if(wall == NORTH || wall == SOUTH) x = pos; else y = pos;
    add_door_at(wall, x, y);
}

void DoorPlacementGuide::spawn_doors(){
    bool large_room = (r->xDim * r->yDim >= 2000); //can't use DMG definitions of large with room rescaling, so this will have to do
    int numExits = rollExits(large_room);
    if(DEBUG) cout << numExits << " total exits (not incl. entrance door)" << endl;

    ///@TODO some sort of sorting? (Don't want to say "door on the east, north, and east")
    for(int i = 1; i <= numExits && i < MAX_DOORS; i++){
        if(!r->doors[i]) spawn_door();
    }
}

void DoorPlacementGuide::add_door_at(int wall, int x, int y){
    for(int i = 1; i < MAX_DOORS; i++){
        if(r->doors[i] == NULL){
            r->doors[i] = new Door(r, wall, x, y);
            int position = ((wall == NORTH || wall == SOUTH) ? x : y);
            forbid(wall, position-4, position+4); // if door is placed at position 45, nearest adjacent can be 40 or 50
            downgrade(wall, position);            // the rest of that 'section' is still allowed, but ideally place a door elsewhere
            return;
        }
    }
}

// if position was a preferred location along that wall, it downgrades to allowed
// otherwise, it has no effect.
void DoorPlacementGuide::downgrade(int wall, int position){
    for(vector< pair<int,int> >::iterator it = preferred[wall].begin(); it != preferred[wall].end(); ++it){
        if(position >= it->first && position <= it->second){
            preferred[wall].erase(it);
            return;
        }
    }
}

//from LB to UB inclusive
void DoorPlacementGuide::forbid(int wall, int LB, int UB){
    remove_region(preferred[wall], LB, UB);
    remove_region(allowed[wall], LB, UB);
}


void DoorPlacementGuide::remove_region(vector< pair<int, int> >& v, int LB, int UB){

    if(DEBUG) cout << "Remove region [" << LB << "," << UB << "]" << endl;

    for(vector< pair<int,int> >::iterator it = v.begin(); !v.empty() && it != v.end(); ++it){

        if(DEBUG) cout << "--Evaluating pair [" << it->first << "," << it->second << "]" << endl;

        if(LB > it->second || UB < it->first) continue; // downgrading an area outside of the current region does nothing
        if(DEBUG) cout << "\tOverlap found! ";

        // add fragments if need be, then erase the base region
        int first = it->first, second = it->second;
        v.erase(it);
        if(UB < second){
            v.push_back(make_pair(UB+1, second));
            if(DEBUG) cout << "+[" << UB+1 << "," << second << "]  ";
        }
        if(LB > first){
            v.push_back(make_pair(first, LB-1));
            if(DEBUG) cout << "+[" << first << "," << LB-1 << "]";
        }

        // After erasing an element, the iterator gets set to the subsequent element, but is then incremented _again_ at the end of the loop.
        // Without any correction, we'd then end up skipping one element each time something was erased. So, how to correct?
        // Can't skip the end step increment, and can't decrement it when we erase (what if we're erasing the first elem?)
        // So, we're left with this slightly awkward cludge.
        it = v.begin();
        if(DEBUG) cout << endl;
    }
}

