#include <string>
#include <room.h>
#include <monster.h>

using namespace std;


void Room::remove_all_monsters(){
    for(int i = 0; i < MAX_MONSTERS && monsters[i] != NULL; i++){
        delete monsters[i];
        monsters[i] = NULL;
    }
}

bool Room::has_monsters(){
    for(int i = 0; i < MAX_MONSTERS; i++){
        if (monsters[i] && monsters[i]->is_alive()) return true;
    }
    return false;
}

Monster* Room::get_monster(string name){
    for(int i = 0; i < MAX_MONSTERS && monsters[i] != NULL; i++){
        if(contains(name, monsters[i]->name()) || contains(name, monsters[i]->get_race()))
            return monsters[i];
    }
    return NULL;
}

string Room::get_monster(){
    if(monsters[0])
        return monsters[0]->name();
    else
        return "N/A";
}

int Room::num_active_monsters(){
    int count = 0;
    for(int i = 0; i < MAX_MONSTERS; i++){
        if(monsters[i] && monsters[i]->is_alive()){
            count++;
        }
    }
    return count;
}

//redo when multi-monster support gets implemented
Monster* Room::get_monster_char(){
    return monsters[0];
}

string Room::get_active_monster(){
    for(int i = 0; i < MAX_MONSTERS && monsters[i] != NULL; i++){
        if(monsters[i]->is_alive()) return monsters[i]->name();
    }
    return "N/A";
}

Monster* Room::get_active_monster_char(){
    if(!has_monsters()) return NULL;
    else return get_monster(get_active_monster());
}
