

#include <iostream>
#include <cassert>

#include <tests.h>

using namespace std;


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

