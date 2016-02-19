#include <room.h>
#include <character.h>

#ifndef DOOR_H
#define DOOR_H


class Room;

class Door {

    friend class Room;
    friend class Dungeon;
    friend class Tests;
    std::string material;
    bool locked;
    bool secret;

    bool barred;
    bool barredSide; //only initialized if the door is barred; true iff the bar is on the same side as First
    bool main_exit; //true iff door leads outside the dungeon

    int xPos, yPos;
    void set_position(Room*);

    void set_material(int);

    Room* first;
    Room* second; //doors connect the rooms!

    int firstWall; //NORTH, SOUTH, etc; for the first room (second room has opposite)
    bool isFirstRoom(Room*);

    Door(Room* room, int entrance);
    std::string getWallString(Room*);
    Door(Room* room, int _wall, bool passage, bool main=false);
    void initialize_door(Room* room, int _wall, bool passage, bool main);

    //bool break_down(Character*);

    bool within_bounds(Room*);

public:
    bool is_main_exit(){ return main_exit; }
    bool is_secret(){ return secret; }
    bool is_locked(){ return locked; }
    bool is_barred(){ return barred; }
    bool is_passage(){ return material == "passage"; }
    void unbar(){ barred = false; }
    void find_door(){ secret = false; }
    void unlock(){ locked = false; }
    int break_DC();
    void break_down();
    bool pick_lock(Character*);
    bool barred_from(Room*);
    int getWall(Room*);
    std::pair<int, int> get_position(){ return std::make_pair(xPos, yPos); }
};

#endif // DOOR_H
