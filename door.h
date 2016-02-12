#include <room.h>

#ifndef DOOR_H
#define DOOR_H


class Room;

class Door {

    friend class Room;
    friend class Dungeon;
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
    int getWall(Room*);
    Door(Room* room, int _wall, bool passage, bool main);
    void initialize_door(Room* room, int _wall, bool passage, bool main);
    int break_DC();

};

#endif // DOOR_H
