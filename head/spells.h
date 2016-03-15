#include <defines.h>
#include <string>

#ifndef SPELLS_H
#define SPELLS_H


class Character;

class Spell {
    friend class Character;
    friend class PlayerChar;
    friend class Treasure;
    friend class Monster;

    std::string name;
    int level; //cantrips are 0
    int range; //in feet
    int duration; //in rounds: 1 round = 6 seconds
    int casting_time; //#define'd as ACTION, BONUS_ACTION, or REACTION
    std::string casting_stat; //"INT", "WIS", or "CHA"

    bool save_half;
    bool save_negates;
    std::string save_stat;
    int save_DC;

    bool attack_roll_required;

    int num_damage_dice, damage_die, damage_mod;
    std::string damage_type;
    int damage();

    //specific initializers
    void fire_bolt();
    void magic_missile();
    void scorching_ray();

    void set_dmg(std::string);
    void clr();

public:
    Spell();
    Spell(std::string);
    Spell(int lvl);

};


#endif // SPELLS_H
