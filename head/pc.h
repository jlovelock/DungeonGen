
#ifndef PC_H
#define PC_H

#include <character.h>

class PlayerChar : public Character {

    std::string PC_class;

    void default_fighter();
    void fighter();
    void fighter_skills();
    void select_race(std::string mode);

    Weapon* weapon_select(Character* target);

    bool second_wind_available;

    int next_levelup();
    void levelup();
    int level;


public:
    PlayerChar(int);
    ~PlayerChar(){}

    std::string name() { return PC_class; }
    std::string full_name() { return PC_class; }

    void generic_attack(Character*);
    void attack(Character* opponent, int attack_roll, int dmg);

    bool pick_lock(Door*);
    bool break_down(Door*);
    bool use_locked_door(Door*);
    bool use_blocked_door(Door*);
    bool can_open(Door*);
    bool find_secret_door(Door*);

    void cast(Spell* spell, Character* target);

    void short_rest();
    void special_action();

    void printCharacterSheet();
    void print_status();
};


#endif // PC_H
