#include <room.h>
#include <character.h>
#include <treasure.h>
#include <vector>

#ifndef DUNGEON_H
#define DUNGEON_H

class Dungeon{
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

public:
    Dungeon();
    ~Dungeon();
    bool getCommand();
    void run();
};

#endif // DUNGEON_H
