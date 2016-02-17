#include <string>
#include <room.h>

using namespace std;


bool Room::has_monsters(){
    for(int i = 0; i < MAX_MONSTERS; i++){
        if (monsters[i] == NULL) return false;
        else if(monsters[i]->is_alive()) return true;
    }
    return false;
}

Character* Room::get_monster(string name){
    for(int i = 0; i < MAX_MONSTERS && monsters[i] != NULL; i++){
        if(monsters[i]->PC_class == name || monsters[i]->race == name)
            return monsters[i];
    }
    return NULL;
}

string Room::get_monster(){
    if(monsters[0])
        return monsters[0]->PC_class;
    else
        return "N/A";
}

string Room::get_active_monster(){
    for(int i = 0; i < MAX_MONSTERS && monsters[i] != NULL; i++){
        if(monsters[i]->is_alive()) return monsters[i]->PC_class;
    }
    return "N/A";
}

Character* Room::get_active_monster_char(){
    return get_monster(get_active_monster());
}