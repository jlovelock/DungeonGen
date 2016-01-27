#include <character.h>
#include <defines.h>

#ifndef SPELLS_H
#define SPELLS_H


class Character;

class Spell {
    friend class Character;
    friend class Treasure;

    std::string name;
    int level; //cantrips are 0
    int range; //in feet
    int duration; //in rounds: 1 round = 6 seconds
    int casting_time;
    int casting_stat;

    bool save_allowed;
    int save_stat; //#defined as STR_STAT, etc

    bool attack_roll_required;

    int num_damage_dice, damage_die, damage_mod;
    std::string damage_type;
    int damage();

    //specific initializers
    void fire_bolt();
    void magic_missile();
    void scorching_ray();

public:
    Spell();
    Spell(int lvl);

};


#endif // SPELLS_H
