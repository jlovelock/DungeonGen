

#include <iostream>
#include <cassert>

#include <tests.h>

using namespace std;

// Both new and adj room have a door on X wall, link them
void Tests::merge_doors_basic(){
    start_test(__func__);

    go("north", 20, 20);
    add_door("east");
    go("south");
    go("east", 20, 20);

    int extra_doors[MAX_DOORS] = {WEST};
    add_room("north", 20, 20, extra_doors);

    assert(g->dungeon->rooms.at(3)->get_door_on_wall("west")->id ==
           g->dungeon->rooms.at(1)->get_door_on_wall("east")->id);

    print_map();

    end_test(__func__);
}

// Adj room has door, new room does not
void Tests::extra_door_adj(){
    start_test(__func__);

    go("north", 20, 20);
    add_door("east");
    go("south");
    go("east", 20, 20);

    go("north", 20, 20);
    print_map();

    assert(g->cur_room->get_door_on_wall("west"));

    end_test(__func__);
}


// New room has door, adj room does not
void Tests::extra_door_new(){
    start_test(__func__);

    go("north", 20, 20);
    go("south");
    go("east", 20, 20);

    int extra_doors[MAX_DOORS] = {WEST};
    add_room("north", 20, 20, extra_doors);

    print_map();

    end_test(__func__);
}
