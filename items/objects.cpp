
#include <objects.h>
#include <string>
#include <character.h>

using namespace std;

int Object::max_id = 0;

Object::Object(string name){
    _name = name;
    _id = max_id++;

    if(name == "shield" || name == "focus"){
        _2h = false;
        _is_weapon = false;
    }
}


bool Object::is_equipped_to(Character* c){
    return (c->main_hand && _id == c->main_hand->_id) || (c->off_hand && _id == c->off_hand->_id);
}
