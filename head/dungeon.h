#include <room.h>
#include <character.h>
#include <treasure.h>
#include <vector>

#ifndef DUNGEON_H
#define DUNGEON_H

class Dungeon{
    friend class Tests;
    friend class DoorPlacementGuide;

    std::vector<Room*> rooms;
    void adjust_room_position(Room* rm);

    bool completed_known;

    //preferences & settings
    std::string dtype;       // STRONGHOLD[0]
    bool m_ena;          // false[0], true[1]
    std::string t_ena;   //RANDOM[0], ALWAYS[1], or NEVER[2]
    std::string mag_it;   //RANDOM[0], ALWAYS[1], or NEVER[2]
    void read_preferences();
    void write_preferences();

    void link_all_doors(Room*);

public:
    Dungeon();
    ~Dungeon();

    void check_completion();

    Room* add_room(Door*, bool);
    Room* add_room(int wall, int x, int y);
    void print_map(std::pair<int,int>);

    //preferences and settings
    void edit_preferences();
    bool monsters_enabled(){ return m_ena; }
    std::string treasure_enabled(){ return t_ena; }
    std::string dungeon_type(){ return dtype; }
    std::string magic_items_enabled(){ return mag_it; }

    Room* starting_room(){ return rooms.front(); }
};

#endif // DUNGEON_H
