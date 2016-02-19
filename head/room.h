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

    int level; ///TODO add multilevel support

    static int max_id; //starts at 0, increments with each room added
    int id;

    //size & position
    int xDim; //East-West
    int yDim; //North-South
    int northEdge, southEdge, eastEdge, westEdge;
    void setEdgeCoords(int, int, int);

    Door* doors[MAX_DOORS];

    ///TODO add more descriptions
    std::string shape, purpose_full, purpose_short;
    std::string hazard;
    bool treasure;

    // Utility functions for adjusting room size/placement
    bool within(int x, int y);
    bool withinX(int);
    bool withinY(int);
    bool bordering(int x, int y);
    bool bordering(Door*);
    void shift(int amt, bool dim_x, bool rev=false);
    void shrink(unsigned amt, int const_wall);
    void remove_connections(int wall);
    void adjustPosition(Room*);
    void linkDoors(Room* adjacent, int wall);
    int find_overwrite_index(bool can_delete=true);
    bool issue_east(Room*, int);
    bool issue_west(Room*, int);
    bool issue_north(Room*, int);
    bool issue_south(Room*, int);


    /** Spawning new rooms **/

    Room(int, int, bool);                             //For Starting room
    Room(std::string type, Door* d, bool passage);    //For all other rooms
    Room(Door* d, int x, int y, bool is_large=false); //For testing
    ~Room();

    void generateChamber(std::string, Door*);
    void generatePassage(Door*);

    void setPurpose(std::string);
    void set_purpose_stronghold();
    void set_contents();
    void setExits(int entrance, bool large);

    bool isFirstRoom(Door*);

    void add_door(int);
    void add_exits(int, int[]);
    void addSecretDoor(int, int);

    int get_door_id(Door* d);
    void printDescription(int); //TODO restructure and get rid of this function
    void remove_all_monsters();

public:
    Room();
    bool has_treasure(){ return treasure; }
    void loot_room(){ treasure = false; }

    //baddies!
    Character* monsters[MAX_MONSTERS];
    bool has_monsters();
    std::string get_monster();
    Character* get_monster(std::string name);
    Character* get_active_monster_char();
    Character* get_monster_char();
    std::string get_active_monster();
    void search_for_secret_doors(Character*, bool& found);

    void printFullDescription(int doorNum=0); ///@TODO doorNum should be the entrance door for initial room
    Door* get_door_on_wall(std::string wall);
    Room* connected(Door*);

    void printDescription(Door*);
    void printFullDescription(Door*);
    std::string get_purpose_short(){ return purpose_short; }

};

#endif // ROOM_H
