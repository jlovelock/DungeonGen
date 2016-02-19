
#include <string>
#include <iostream>
#include <dungeon.h>

using namespace std;

Dungeon::Dungeon(){
    Room* starting_room = new Room();
    rooms.push_back(starting_room);
    completed_known = false;
    read_preferences();
}

Dungeon::~Dungeon(){
    for(vector<Room*>::iterator it = rooms.begin(); it != rooms.end(); ++it){
        delete *it;
    }
    rooms.clear();
}

void Dungeon::check_completion(){

    if(completed_known) return;
    for(vector<Room*>::iterator it = rooms.begin(); it != rooms.end(); ++it){
    //for(Room* r = first_room; r != NULL; r = r->next){
        for(int i = 0; i < MAX_DOORS && (*it)->doors[i] != NULL; i++){
            if((*it)->doors[i]->main_exit) continue;
            if((*it)->doors[i]->second == NULL) return;
        }
    }
    cout << endl << "   *** ALERT: All rooms of this dungeon have been generated! ***" << endl << endl;
    completed_known = true;

}
