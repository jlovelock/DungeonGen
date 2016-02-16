
#include <iostream>
#include <dungeon.h>

using namespace std;


bool Dungeon::use_locked_door(Door* d){
    cout << "That door is locked. Try to lockpick, ram, or leave? ";
    string input;
    read(input);
    if (input == "lockpick") {
        return d->pick_lock(&PC);
    } else if(input == "ram"){
        return d->break_down(&PC);
    } else {
        cout << endl;
        return false;
    }
}

bool Dungeon::use_barred_door(Door* d){
    if(d->barredSide xor (cur_room->id != d->first->id)) {
        cout << "You remove the bar on the door and head through." << endl << endl;
        d->barred = false;
        return true;

    } else {
        cout << "Something is blocking it from opening. Try to ram, or leave? ";
        string input;
        read(input);

        if(input == "ram"){
           return d->break_down(&PC);
        } else {
            return false;
        }
    }
}

// Checks various complications on the door to see if you actually go through it.
bool Dungeon::can_open(Door* d){
    if(d->locked){
        return use_locked_door(d);

    } else if(d->barred) {
        return use_barred_door(d);

    } else if(d->main_exit) {
        cout << "This leads outside? Leave the dungeon? [y/n] ";
        string input;
        read(input);
        if(input == "y"){
            cout << "You have left the dungeon!" << endl;
            in_dungeon = false;
        }
        return false;

    } else {
        return true;
    }
}

bool Dungeon::parse_open_door(string input){

    // Sanitize and verify input
    if(input == "n" || input == "up") input = "north";
    else if(input == "e" || input == "right") input = "east";
    else if(input == "s" || input == "down") input = "south";
    else if(input == "w" || input == "left") input = "west";

    string allowed_responses[] = {"north", "south", "east", "west", ""};
    if(!in_list(input, allowed_responses)){
        cout << "Unrecognized command" << endl;
        return false;
    }

    if(cur_room->has_monsters()){
        cout << "The " << cur_room->get_active_monster() << " is blocking your exit." << endl << endl;
        return false;
    }

    int doorCopies = 0, doorNum = 0;
    for(int i = 0; i < MAX_DOORS && cur_room->doors[i] != NULL; i++){
        if(contains(input, cur_room->doors[i]->getWallString(cur_room)) && !cur_room->doors[i]->secret){
            doorNum = i;
            doorCopies++;
        }
    }
    if(doorCopies == 1)
        return useDoor(cur_room->doors[doorNum]);

    else if(doorCopies > 1) {

        cout << "Which instance?" << endl;
        int instance = 0;
        for(int i = 0; i < MAX_DOORS && cur_room->doors[i] != NULL; i++){
            if(cur_room->doors[i]->getWallString(cur_room) == input && !cur_room->doors[i]->secret){
                cout << "\t- " << instance++;

                //print description if it leads to a room you've visited
                if(cur_room->doors[i]->second != NULL){
                    cout << " (leading back to the ";
                    if(cur_room->isFirstRoom(cur_room->doors[i]))
                        cout << cur_room->doors[i]->second->purpose_short;
                    else
                        cout << cur_room->doors[i]->first->purpose_short;
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

        for(int i = 0; i < MAX_DOORS && cur_room->doors[i] != NULL; i++){
            if(contains(input, cur_room->doors[i]->getWallString(cur_room)) && cur_room->doors[i]->secret == false){
                if(doorIndex == 0) return useDoor(cur_room->doors[i]);
                else doorIndex--;
            }
        }
    }

    cout << "There's no door along that wall." << endl;
    return false;

}



bool Dungeon::useDoor(Door* d) {
    if(!can_open(d)) return false;

    int door_num = 0;
    //revisiting a room that is already initialized
    if(d->second != NULL){
        cout << "You turn back to the "
             << (cur_room->isFirstRoom(d) ? d->second->purpose_short : d->first->purpose_short)
             << "." << endl;

        cur_room = cur_room->isFirstRoom(d) ? d->second : d->first;
        cur_room->printDescription(cur_room->get_door_id(d));


    //creating (and moving to) a new room
    } else {
        add_room(d, d->material == "passage"); //debug
        cur_room->printFullDescription(door_num);
    }

    //update PC's position
    PC.xPos = d->xPos;
    PC.yPos = d->yPos;

    int direction_of_travel = opposite(d->getWall(cur_room));
    switch(direction_of_travel){
        case NORTH:
            PC.yPos +=5; break;
        case SOUTH:
            PC.yPos -=5; break;
        case EAST:
            PC.xPos +=5; break;
        case WEST:
            PC.xPos -=5; break;
    }

    return true;
}

//void Dungeon::change_rooms(Door* d, Room* new_room){
//
//}

//bool Dungeon::parse_open_door(){
//    string input;
//    read(input);
//    return parse_open_door(input);
//}
