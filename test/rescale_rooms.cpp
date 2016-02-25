
#include <iostream>
#include <cassert>

#include <tests.h>

using namespace std;

//Room linkages / auto-shift: case 2 (new room has no door, adjacent one does)
//Unbarring a door from the other side
void Tests::unbar_door(){
    start_test(__func__);

    go("north", 20, 20);            //room 1
    Door* d = add_door("east");
    d->barred = true;
    d->barredSide = false;

    //make sure door is barred from the other side
    assert(d->barred);
    assert(d->barred_from(g->cur_room) == false);

//    g->cur_room = first_room;
    go("south");
    go("east", 20, 20);      //room 2
    go("north", 20, 20);     //room 3: should connect to room 1 (going west)

    //make sure door is barred from this side
    assert(g->cur_room->get_door_on_wall("west")->barred);
    assert(g->cur_room->get_door_on_wall("west")->barred_from(g->cur_room));

    //go through it
    g->run();

    //make sure bar was removed
    assert(!g->cur_room->get_door_on_wall("east")->barred);

    end_test(__func__);
}

// No doors to align, one wall overlapping with one room
void Tests::shift_room_basic(){
    start_test(__func__);

    add_room("north", 50, 50);  //room 1
    add_room("east", 30, 30);   //room 2, should shift down

    assert(g->dungeon->rooms.at(2)->westEdge == g->dungeon->rooms.at(0)->eastEdge);
    assert(g->dungeon->rooms.at(2)->northEdge == g->dungeon->rooms.at(1)->southEdge);

    end_test(__func__);
}

void Tests::doubly_locked_wall_shift(){
    start_test(__func__);

    go("east", 20, 20);
    go("south", 30, 20);
    jump_to_rm(0);
    go("north", 20, 20);
    go("east", 70, 20);
    go("south", 40, 30);

    print_map();

    assert(g->cur_room->westEdge == g->dungeon->rooms.at(2)->eastEdge);

    end_test(__func__);
}

/*
 * Basic shrink test (no door alignment): only one direction possible
 * Shift should fail by disconnecting entry door
 */
void Tests::shrink_room_basic(){
    start_test(__func__);

    go("north", 20, 60);
    go("east", 60, 20);
    go("south", 20, 60);
    jump_to_rm(0);
    go("east", 40, 10);

    print_map();
    assert(g->cur_room->xDim == 20);

    end_test(__func__);
}

/*
 * More advanced test of room geometry (one room overlap, no door alignment)
 * Two-wall overlap (N/E)
 * West shift fails (entry wall locked)
 * South shift/shrink fails (entry door no longer in bounds)
 * Only valid solution: shrink west
 */
void Tests::shrink_room_advanced(){
    start_test(__func__);

    go("north", 20, 60);
    go("east", 20, 20);
    go("east", 20, 110);
    jump_to_rm(0);
    go("east", 30, 40);

    print_map();
    assert(g->cur_room->xDim == 20);

    end_test(__func__);
}

// Will take multiple passes to finalize
// First pass: room 3 OK, room 6 requires a shift down
// Second pass: room 3 overlapping, reqiures shrink
void Tests::shrink_room_sequential(){
    start_test(__func__);

    go("east", 40, 20);
    go("north", 20, 40);
    go("west", 10, 10);
    jump_to_rm(0);
    go("west", 20, 20);
    go("north", 10, 70);
    go("east", 10, 20);
    jump_to_rm(0);
    go("north", 20, 30);

    print_map();

    assert(g->cur_room->xDim == 15 || g->cur_room->yDim == 15);

    end_test(__func__);
}
