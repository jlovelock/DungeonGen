
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

    return useDoor(cur_room->get_door_on_wall(input));
}



bool Dungeon::useDoor(Door* d) {
    if(!d){
        cout << "There's no door along that wall." << endl;
        return false;
    }
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
