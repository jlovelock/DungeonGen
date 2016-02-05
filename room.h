#ifndef ROOM_H
#define ROOM_H
#include <string>
#include <door.h>
#include <defines.h>
#include <character.h>


class Room {
    friend class Dungeon;
    friend class Door;
    Room* next; //linked list, appends whenever a room is generated; used in algorithms that traverse all rooms

    //baddies!
    Character* monsters[MAX_MONSTERS];
    bool has_monsters();
    std::string get_active_monster();
    Character* get_monster(std::string name);
    Character* get_active_monster_char();

    Door* doors[6];

    int level; ///TODO add multilevel support
    int id; //starts at 0, increments with each room added; current id tracked in dungeon class

    //size
    int xDim; //East-West
    int yDim; //North-South

    bool is_passage;
    bool has_treasure;

    std::string hazard;

    ///TODO initialize these as bounds of room - to connect rooms properly
    int northEdge, southEdge, eastEdge, westEdge;
    void setEdgeCoords(int, int, int);
    bool within(int x, int y);
    bool withinX(int);
    bool withinY(int);
    void shift(int amt, bool dim_x);
    void shrink(int amt, bool dim_x);
    void adjustPosition(Room*);
    void linkDoors(Room* adjacent, int wall);

    ///TODO add more descriptions
    std::string shape, purpose_full, purpose_short;

    bool isFirstRoom(Door*);
    void setExits(int entrance, bool large);
    void setPurpose();
    void set_contents();
    ~Room();

    Room(int, int, bool);
    Room();
    Room(int _id, Door* d, bool passage);
    void printInitialDescription(int);
    void printDescription(int);
    void printFullDescription();

    void generateChamber(Door*);
    void generatePassage(Door*);
    void addSecretDoor(int, int);

};

#endif // ROOM_H