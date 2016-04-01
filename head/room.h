#ifndef ROOM_H
#define ROOM_H
#include <string>
#include <door.h>
#include <inventory.h>
#include <defines.h>

class Monster;
class PlayerChar;

class Room {
    friend class Dungeon;
    friend class Door;
    friend class Tests;
    friend class DoorPlacementGuide;

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

    // Utility functions for adjusting room size/placement
    bool within(int x, int y);
    bool withinX(int);
    bool withinY(int);
    bool bordering(int x, int y);
    bool bordering(Door*, int delta_x=0, int delta_y=0);
    bool bordering(Door* d, int delta_n, int delta_s, int delta_e, int delta_w);
    void shift(int, int);
    void shrink(int, int);
    void shuffle_bounds(int dN, int dS, int dE, int dW);
    void remove_connections(int wall);
    void adjustPosition(Room*);
    void link_doors(Room* adjacent, int wall);
    int find_overwrite_index(bool can_delete=true);
    bool issue(int direction, Room*);
    bool issue_shift(Room* r, int x, int y);
    bool issue_shrink(Room* r, int shrink_direction, int shrink_amt);
    bool issue_east(Room*, int, int, int, int);
    bool issue_west(Room*, int, int, int, int);
    bool issue_north(Room*, int, int, int, int);
    bool issue_south(Room*, int, int, int, int);
    bool try_shift(int dir, Room* compare, Door* entry);
    bool try_shrink(int dir, Room* compare, Door* entry);


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

    Door* add_door(int);
    void add_exits(int, int[]);
    void addSecretDoor(int, int);

    int get_door_id(Door* d);
    void printDescription(int); //TODO restructure and get rid of this function
    void remove_all_monsters();

    int shared_wall(Room*);
    Inventory* hidden_items;
    Inventory* public_items;

public:
    Room();
    bool has_treasure(){ return !hidden_items->is_empty(); }
    void loot(Character*, bool&, bool&);
    void drop_item(Object*);
    Object* pick_up_item(std::string);

    //baddies!
    Monster* monsters[MAX_MONSTERS];
    bool has_monsters();
    std::string get_monster();
    Monster* get_monster(std::string name);
    Monster* get_active_monster_char();
    Monster* get_monster_char();
    std::string get_active_monster();
    void search_for_secret_doors(PlayerChar*, bool& found);
    int num_active_monsters();

    void printFullDescription(int doorNum=0); ///@TODO doorNum should be the entrance door for initial room
    Door* get_door_on_wall(std::string wall, bool use_first=false);
    Room* connected(Door*);

    void printDescription(Door*);
    void printFullDescription(Door*);
    std::string get_purpose_short(){ return purpose_short; }
    std::string location();

    int get_id() { return id; }

    int north(){ return northEdge; }
    int south(){ return southEdge; }
    int east(){ return eastEdge; }
    int west(){ return westEdge; }

};

#endif // ROOM_H
