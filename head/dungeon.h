#include <room.h>
#include <character.h>
#include <treasure.h>
#include <vector>

#ifndef DUNGEON_H
#define DUNGEON_H

class Dungeon{
    friend class Tests;
    Character PC;
    int cur_id;
    Room* cur_room; //where the player is
    Room* first_room; //where the exit to the dungeon is
    Room* last_room; //the most recent room to be added
    bool useDoor(Door*);
    bool parse_open_door();
    bool parse_open_door(std::string);
    bool in_dungeon;

    void adjust_room_position(Room* rm);
    void print_map();
    void add_room(Door*, bool);
    void add_room(int wall, int x, int y);

    //treasure
    void rollIndividualTreasure(std::string);
    void rollTreasureHoard();

    std::vector<Treasure*> loot, potions, scrolls;
    int cp, sp, ep, gp, pp;
    void print_inventory();

    void equip_item(std::string);
    void rest();
    void look(std::string);
    bool searching(std::string);
    bool combat(std::string);
    //bool combat(std::string input, bool is_ranged);
    bool melee_combat();
    bool ranged_combat();
    bool drink_potion();
    void drink_potion(int index);
    void identify_items();
    bool cast_spell();

    void check_completion();
    bool completed_known;
    void preferences();

    bool use_locked_door(Door*);
    bool use_barred_door(Door*);
    bool can_open(Door*);

    //preferences & settings
    std::string dungeon_type;       // STRONGHOLD[0]
    bool monsters_enabled;          // false[0], true[1]
    std::string treasure_enabled;   //RANDOM[0], ALWAYS[1], or NEVER[2]
    void read_preferences();
    void write_preferences();

public:
    Dungeon();
    ~Dungeon();
    bool getCommand();
    void run();
};

#endif // DUNGEON_H
