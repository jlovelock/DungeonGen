#ifndef ROOM_H
#define ROOM_H
#include <string>
#include <door.h>
#include <defines.h>
#include <character.h>


class Room {
    friend class Dungeon;
    friend class Door;
    friend class Tests;
    Room* next; //linked list, appends whenever a room is generated; used in algorithms that traverse all rooms

    Door* get_door_on_wall(std::string wall);

    //baddies!
    Character* monsters[MAX_MONSTERS];
    bool has_monsters();
    std::string get_active_monster();
    std::string get_monster();
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
    bool bordering(int x, int y);
    bool bordering(Door*);
    void shift(int amt, bool dim_x, bool rev=false);
    //void shrink(int amt, bool dim_x);
    void shrink(unsigned amt, int const_wall);
    void remove_connections(int wall);
    void adjustPosition(Room*);
    void linkDoors(Room* adjacent, int wall);
    int find_overwrite_index(bool can_delete=true);

    bool issue_east(Room*, int);
    bool issue_west(Room*, int);
    bool issue_north(Room*, int);
    bool issue_south(Room*, int);

    ///TODO add more descriptions
    std::string shape, purpose_full, purpose_short;

    bool isFirstRoom(Door*);
    void setExits(int entrance, bool large);
    void set_purpose_stronghold();
    void setPurpose(std::string);
    void set_contents();
    ~Room();

    Room(int, int, bool);
    Room();
    Room(int _id, Door* d, int x, int y, bool is_large=false); //For testing only
    Room(std::string type, int _id, Door* d, bool passage);
    void printInitialDescription(int);
    void printDescription(int);
    void printFullDescription(int doorNum=0); ///@TODO doorNum should be the entrance door for initial room

    void generateChamber(std::string, Door*);
    void generatePassage(Door*);
    void addSecretDoor(int, int);

    void add_door(int);
    void add_exits(int, int[]);

    int get_door_id(Door* d);
    void remove_all_monsters();

};

#endif // ROOM_H
