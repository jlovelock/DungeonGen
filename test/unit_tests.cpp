///@TODO REIMPLEMENT THESE ASAP!!

//
//#include <iostream>
//#include <cassert>
//
//#include <tests.h>
//#include <dungeon.h>
//
//using namespace std;
//
////Room linkages / auto-shift: case 2 (new room has no door, adjacent one does)
////Unbarring a door from the other side
//void Tests::unbar_door(){
//    init_input_file(__func__);
//
//    Dungeon* d = new Dungeon();
//    d->add_room(NORTH, 20, 20);     //room 1
//
//    d->cur_room->add_door(EAST);
//    d->cur_room->doors[1]->barred = true;
//    d->cur_room->doors[1]->barredSide = false;
//
//    d->cur_room = d->first_room;
//    d->add_room(EAST, 20, 20);      //room 2
//    d->cur_room->add_door(NORTH);
//    d->add_room(NORTH, 30, 30);     //room 3: should shift position and connect to room 1 (going west)
//
//    //make sure door is barred
//    assert(d->cur_room->get_door_on_wall("west")->barred);
//
//    //go through it
//    d->run();
//
//    //make sure bar was removed
//    assert(!d->cur_room->get_door_on_wall("east")->barred);
//
//    delete d;
//}
//
//
////No doors to align, one wall overlapping
//void Tests::shift_room_basic(){
//    init_input_file(__func__);
//    Dungeon* d = new Dungeon();
//
//    d->add_room(NORTH, 50, 50);     //room 1
//    d->cur_room = d->first_room;
//    d->add_room(EAST, 30, 30);      //room 2; should shift down
//    d->print_map();
//
//    //check room 2 borders
//    assert(d->cur_room->westEdge == d->first_room->eastEdge);
//    assert(d->cur_room->northEdge == d->first_room->next->southEdge);
//    assert(d->cur_room->eastEdge == d->first_room->eastEdge + 30);
//    assert(d->cur_room->southEdge == d->first_room->next->southEdge - 30);
//
//    delete d;
//}
