
#include <game.h>
using namespace std;


bool Game::can_use_door(Door* d){
    if(!d){
        cout << "There's no door along that wall." << endl;
        return false;
    }

    if(d->barred_from(cur_room)){
        cout << "You remove the bar on the door before opening it." << endl << endl;
        d->unbar();
    }
    if(!PC->can_open(d)) return false;
    return true;
}

bool Game::parse_open_door(string input){

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

    Door* d = cur_room->get_door_on_wall(input);
    if(!can_use_door(d)) return false;

    Room* next_rm = cur_room->connected(d);

    //revisiting a room that is already initialized
    if(next_rm != NULL) {
        cout << "You turn back to the " << next_rm->get_purpose_short() << "." << endl;
        cur_room = next_rm;
        cur_room->printDescription(d);

    //leaving dungeon
    } else if(d->is_main_exit()) {
        cur_room = NULL;
        return true;

    //spawn new room
    } else {
        cur_room = dungeon->add_room(d, d->is_passage()); //debug
        cur_room->printFullDescription(d);
    }

    //update PC's position
    int x = d->get_position().first, y = d->get_position().second;
    int direction_of_travel = opposite(d->getWall(cur_room));
    switch(direction_of_travel){
        case NORTH:
            y += 5; break;
        case SOUTH:
            y -= 5; break;
        case EAST:
            x += 5; break;
        case WEST:
            x -= 5; break;
    }
    PC->set_position(x, y);

    return true;
}



