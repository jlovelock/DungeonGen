
#include <iostream>
#include <door.h>

using namespace std;

bool Door::pick_lock(Character* c){
    if(c->skill_check("THIEVES TOOLS") > LOCKPICK_DC){
        locked = false;
        cout << "You successfully pick the lock." << endl << endl;
        return true;
    } else {
        cout << "You fail to pick the lock." << endl << endl;
        return false;
    }
}

bool Door::break_down(Character* c){
    if(c->attribute_chk("STRENGTH") > break_DC()){
        cout << "You successfully break down the door." << endl << endl;
        material = "broken " + material;
        locked = false;
        return true;
    } else {
        cout << "You fail to break down the door." << endl << endl;
        return false;
    }
}
