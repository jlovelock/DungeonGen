
#include <monster.h>
#include <room.h>

using namespace std;


Monster::~Monster(){

}


bool Monster::has(string feature){
    for(auto it = features.begin(); it != features.end(); ++it){
        if((*it) == feature) return true;
    }
    return false;
}

void Monster::set_position_in(Room* r){
    xPos = r->west() + rand()%(r->east() - r->west() - 1) + 1;
    xPos = r->south() + rand()%(r->north() - r->south() - 1) + 1;
}
