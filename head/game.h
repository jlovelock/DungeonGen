
#ifndef GAME_H
#define GAME_H

#include <character.h>
#include <room.h>
#include <treasure.h>
#include <dungeon.h>

class Game {
    friend class Tests;

    Dungeon* dungeon;
    Character* PC;
    Room* cur_room; //where the player is

    bool parse_open_door(std::string);
    bool can_use_door(Door*);

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
    bool drink_potion();
    void drink_potion(int index);
    bool has_unidentified_items();
    void identify_items();
    bool cast_spell();

public:
    Game();
    ~Game();
    bool getCommand();
    void run();
};

#endif // GAME_H
