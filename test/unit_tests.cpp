
#include <iostream>
#include <assert.h>

#include <tests.h>
#include <dungeon.h>

using namespace std;

//Room linkages / auto-shift: case 2 (new room has no door, adjacent one does)
//Unbarring a door from the other side
void Tests::unbar_door(){
    init_input_file("unbar_door");

    Dungeon* d = new Dungeon();
    d->add_room(NORTH, 20, 20);     //room 1

    d->cur_room->add_door(EAST);
    d->cur_room->doors[1]->barred = true;
    d->cur_room->doors[1]->barredSide = false;

    d->cur_room = d->first_room;
    d->add_room(EAST, 20, 20);      //room 2
    d->cur_room->add_door(NORTH);
    d->add_room(NORTH, 30, 30);     //room 3: should shift position and connect to room 1 (going west)

    //make sure door is barred
    assert(d->cur_room->get_door_on_wall("west")->barred);

    //go through it
    d->run();

    //make sure bar was removed
    assert(!d->cur_room->get_door_on_wall("east")->barred);

    delete d;
}
