
#include <iostream>
#include <cstdlib>
#include <string>
#include <room.h>

using namespace std;

int Room::max_id = 0;

Room* Room::connected(Door* d){
    return isFirstRoom(d) ? d->second : d->first;
}


Door* Room::get_door_on_wall(string input, bool use_first){
    int doorCopies = 0, doorNum = 0;
    for(int i = 0; i < MAX_DOORS && doors[i] != NULL; i++){
        if(contains(input, doors[i]->getWallString(this)) && !doors[i]->secret){
            if(use_first){
                return doors[i];
            } else {
                doorNum = i;
                doorCopies++;
            }
        }
    }
    if(doorCopies == 1)
        return doors[doorNum];

    else if(doorCopies > 1) {

        cout << "Which instance?" << endl;
        int instance = 0;
        for(int i = 0; i < MAX_DOORS && doors[i] != NULL; i++){
            if(doors[i]->getWallString(this) == input && !doors[i]->secret){
                cout << "\t- " << instance++;

                //print description of room it leads to
                if(doors[i]->second != NULL){
                    cout << " (leading back to the ";
                    if(isFirstRoom(doors[i]))
                        cout << doors[i]->second->purpose_short;
                    else
                        cout << doors[i]->first->purpose_short;
                    cout << ")";
                } else {
                    cout << " (leading somewhere new)";
                }
                cout << endl;
            }
        }
        cout << endl;

        int doorIndex;
        string num;
        read(num);
        doorIndex = atoi(num.c_str());

        for(int i = 0; i < MAX_DOORS && doors[i] != NULL; i++){
            if(contains(input, doors[i]->getWallString(this)) && doors[i]->secret == false){
                if(doorIndex == 0) return doors[i];
                else doorIndex--;
            }
        }
    }

    ///@TODO sanitize input / loop until a valid door # is entered by user, instead of crapping out here
    return NULL;
}

///@TODO: get doors properly spaced out along walls, so this works as expected
int Room::get_door_id(Door* d){
    for(int i = 0; i < MAX_DOORS; i++){
        if(doors[i] && doors[i]->id == d->id) return i;
    }
    cout << "ERROR: Door #" << d->id << " @(" << d->xPos << "," << d->yPos << ") was not found in room #" << id << ".";
    return -1;
}

Room::~Room(){
    for(int i = 0; i < MAX_DOORS; i++){
        if(doors[i] != NULL){

            //don't double delete! only delete from first room - set the other to NULL
            if(isFirstRoom(doors[i]) && doors[i]->second){
                doors[i]->second->doors[0] = NULL;
                delete doors[i];
            } else if(!isFirstRoom(doors[i]) && doors[i]->first){
                doors[i] = NULL;
            }

        }
    }
    remove_all_monsters();
}


bool Room::isFirstRoom(Door* door){
    return door->first->id == this->id;
}





//bool Room::within(int x, int y){
//    return
//        x < eastEdge &&
//        x > westEdge &&
//        y < northEdge &&
//        y > southEdge;
//}
//bool Room::withinX(int x){
//    return x < eastEdge && x > westEdge;
//}
//
//bool Room::withinY(int y){
//    return y < northEdge && y > southEdge;
//}

void Room::addSecretDoor(int door_num, int entrance){
    doors[door_num] = new Door(this, entrance);
    doors[door_num]->secret=true;
    doors[door_num]->material="hidden";
    ///TODO FIXME make sure it's not barred from the outside
}



//void Room::printInitialDescription(int doorNum){
//    cout << "You enter a " << xDim << " by " << yDim << " foot " << shape << " ";
//    cout << purpose_short << ", " << purpose_full << endl;
//    printDescription(doorNum);
//
//    ///TODO FIXME
//    //if(is_passage) cout << " corridor, ";
//    //else cout << " room, ";
//}

void Room::printFullDescription(Door* d){
    printFullDescription(get_door_id(d));
}

void Room::printDescription(Door* d){
    printDescription(get_door_id(d));
}

void Room::printFullDescription(int doorNum){
    cout << "You are in a " << xDim << " by " << yDim << " foot " << shape << " ";
    cout << purpose_short << ", " << purpose_full << endl;

//    cout << endl << " ### NORTH WALL: " << northEdge << endl;
//    cout << " ### SOUTH WALL: " << southEdge << endl;
//    cout << " ### EAST WALL: " << eastEdge << endl;
//    cout << " ### WEST WALL: " << westEdge << endl;

    printDescription(doorNum);
}


void Room::printDescription(int doorNum){
    //cout << "ROOM " << id << endl;

    if(DEBUG) cout << "@@ " << location() << endl;

    ///TODO multi-monster support
    if(monsters[0] != NULL){
        if(has_monsters())
            cout << "There is an angry " << get_active_monster() << " charging towards you!" << endl;
        else
            cout << "The corpse of a " << monsters[0]->PC_class << " lies on the floor." << endl; ///TODO FIXME
    }

    if(!hazard.empty()){
        cout << "Toxic " << hazard << " covers the walls." << endl;
    }

    cout << "There is ";
    int i;
    bool doorFound = false;
    for(i = 0; i < MAX_DOORS && doors[i] != NULL; i++) {
        if(i == doorNum || doors[i]->secret) continue;
        if((i == MAX_DOORS-1 || (i != 0 && doors[i+1] == NULL))&&doorFound) cout << "and ";
        doorFound = true;

        //Door description
        if(doors[i]->material == "empty" || doors[i]->material == "passage")
            cout << "an opening to a corridor";
        else
            cout << "a " << doors[i]->material << " door";

        cout << " on the " << doors[i]->getWallString(this) << " wall";

        if(doors[i]->barred && (doors[i]->barredSide &&
             (doors[i]->barredSide xor (this->id != doors[i]->first->id))))
           cout << " that is barred from this side";

        if(doors[i]->second != NULL){
            cout << " (leading back to the ";
            if(isFirstRoom(doors[i]))
                cout << doors[i]->second->get_purpose_short();
            else
                cout << doors[i]->first->get_purpose_short();
            cout << ")";
        }

        if(DEBUG) cout << "--[ID #" << doors[i]->id << " @(" << doors[i]->xPos << "," << doors[i]->yPos << ")]--";

        cout << ", ";
    }
    if(doorFound) cout << "as well as ";
    cout << "the ";

    if(doors[doorNum]->material == "empty" || doors[doorNum]->material == "passage")
        cout << "opening to the corridor you entered to the ";
    else
        cout << doors[doorNum]->material << " door you entered to the ";

    cout << doors[doorNum]->getWallString(this);

    if(doors[doorNum]->main_exit){
        cout << " (leading back outside";
    } else {
        cout << " (leading back to the ";
        if(isFirstRoom(doors[doorNum]))
            cout << doors[doorNum]->second->get_purpose_short();
        else
            cout << doors[doorNum]->first->get_purpose_short();
    }

     cout << ") ";

     if(DEBUG) cout << "--[ID #" << doors[doorNum]->id << " @(" << doors[doorNum]->xPos << "," << doors[doorNum]->yPos << ")]--";

     cout << "." << endl << endl;

}


void Room::search_for_secret_doors(Character* c, bool& found){
    for(int i = 0; i < MAX_DOORS; i++){
        if(c->find_secret_door(doors[i])){
        //if(cur_room->doors[i] != NULL && cur_room->doors[i]->secret && PC.skill_check("INVESTIGATION") > SECRET_DOOR_DC){
            //cur_room->doors[i]->secret = false;
            if(found) cout << "You also find a secret door along the ";
            else cout << "You find a secret door along the ";
            cout << to_string(doors[i]->getWall(this)) << " wall." << endl << endl;
            found = true;
        }
    }
}


