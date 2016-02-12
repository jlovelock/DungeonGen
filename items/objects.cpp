
#include <objects.h>
#include <string>
#include <character.h>

using namespace std;

Object::Object(string name){
    _name = name;

    if(name == "shield" || name == "focus"){
        _2h = false;
        _is_weapon = false;
    }
}


bool Object::is_equipped_to(Character* c){
    return _name == c->main_hand->name() || _name == c->off_hand->name();
}
