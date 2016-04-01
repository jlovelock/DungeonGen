#include <defines.h>
#include <string>

#ifndef SPELLS_H
#define SPELLS_H

class Character;
class Condition;

class Spell {
    friend class Character;
    friend class PlayerChar;
    friend class Treasure;
    friend class Monster;

    std::string _name;
    int level; //cantrips are 0
    int range; //in feet
    int casting_time; //#define'd as ACTION, BONUS_ACTION, or REACTION
    std::string casting_stat; //"INT", "WIS", or "CHA"
    bool concentration;
    bool beneficial; //when true, casting this spell auto-targets self

    bool save_half;
    bool save_negates;
    std::string save_stat;
    int save_DC;

    bool attack_roll_required;

    int num_damage_dice, damage_die, damage_mod;
    std::string damage_type;
    int damage();

    Condition* condition;
    bool condition_save;
    std::string condition_save_stat;

    /// initializers
    // cantrips
    void fire_bolt();
    void poison_spray();
    void blade_ward();

    // lvl 1
    void magic_missile();
    void inflict_wounds();
    void longstrider();
    void armor_of_agathys();

    // lvl 2
    void scorching_ray();
    void blindness();
    void hold_person();

    void set_dmg(std::string);
    void clr();

public:
    Spell();
    Spell(std::string);  //for monster bonus dmg. Formatting example: "2d4+0 poison"
    Spell(int lvl);      //for scrolls only
    Spell(Spell*);
    ~Spell();
    std::string name(){ return _name; }

    void cast(Character* caster, Character* target);
    bool targets_self(){ return beneficial; }

};


#endif // SPELLS_H
