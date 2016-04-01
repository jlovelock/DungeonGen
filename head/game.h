
#ifndef GAME_H
#define GAME_H

#include <character.h>
#include <room.h>
#include <treasure.h>
#include <dungeon.h>

class PlayerChar;

class Game {
    friend class Tests;

    Dungeon* dungeon;
    PlayerChar* PC;
    Room* cur_room; //where the player is

    bool parse_open_door(std::string);
    bool can_use_door(Door*);

    bool equip_item(std::string);
    void drop_item(std::string);
    void pick_up_item(std::string);
    void rest();
    void check_encumbrance();
    void look(std::string);
    bool searching(std::string);
    bool combat(std::string);
    bool drink_potion(std::string);
    bool has_unidentified_items();
    void identify_items();
    bool cast_spell(std::string);
    Scroll* select_scroll();

public:
    Game();
    ~Game();
    bool getCommand();
    void run();
};

#endif // GAME_H
