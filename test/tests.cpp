
#include <dungeon.h>
#include <defines.h>
#include <tests.h>

Tests::Tests(){
    test_mode = true;
}

Tests::~Tests(){
    test_mode = false;
}

//Unbarring a door from the other side!
void Tests::unbar_door(){
    Dungeon* d = new Dungeon();
    d->add_room(NORTH, 20, 20);

    d->cur_room->add_door(EAST);
    d->cur_room->doors[1]->barred = true;
    d->cur_room->doors[1]->barredSide = false;

    d->cur_room = d->first_room;
    d->add_room(EAST, 20, 20);
    d->cur_room->add_door(NORTH);
    d->add_room(NORTH, 20, 20);

    d->run();

    delete d;
}
