
#include <pc.h>
#include <door.h>

using namespace std;

bool PlayerChar::pick_lock(Door* d){
    if(skill_check("THIEVES TOOLS") > LOCKPICK_DC){
        d->unlock();
        cout << "You successfully pick the lock." << endl << endl;
        return true;
    } else {
        cout << "You fail to pick the lock." << endl << endl;
        return false;
    }
}

bool PlayerChar::break_down(Door* d){
    if(attribute_chk("STRENGTH") > d->break_DC()){
        cout << "You successfully break down the door." << endl << endl;
        d->break_down();
        return true;
    } else {
        cout << "You fail to break down the door." << endl << endl;
        return false;
    }
}


bool PlayerChar::use_locked_door(Door* d){
    cout << "That door is locked. Try to lockpick, ram, or leave? " << endl;
    string input;
    read(input);
    if (input == "lockpick") {
        return pick_lock(d);
    } else if(input == "ram"){
        return break_down(d);
    } else {
        cout << endl;
        return false;
    }
}

bool PlayerChar::use_blocked_door(Door* d){
        cout << "Something is blocking it from opening. Try to ram, or leave? " << endl;
        string input;
        read(input);

        if(input == "ram"){
           return break_down(d);
        } else {
            return false;
        }
}

// Checks various complications on the door to see if you actually go through it.
bool PlayerChar::can_open(Door* d){
    if(d->is_locked()){
        return use_locked_door(d);

    // nb. the case of unbarring it from this side was handled by the caller Game functio
    } else if(d->is_barred()) {
        return use_blocked_door(d);

    } else if(d->is_main_exit()) {
        cout << "This leads outside? Leave the dungeon? [y/n] ";
        string input;
        read(input);

        return input == "y";    ///@TODO make sure leaving the dungeon gets properly taken care of elsewhere!
//        if(input == "y"){
//            cout << "You have left the dungeon!" << endl;
//            in_dungeon = false;
//        }
//        return false;

    } else {
        return true;
    }
}

bool PlayerChar::find_secret_door(Door* d){
    if (d && d->is_secret() && skill_check("INVESTIGATION") > SECRET_DOOR_DC){
        d->find_door();
        return true;
    }
    return false;
}
