#ifndef CHARACTER_H
#define CHARACTER_H

#include <string>
#include <cstdlib>
#include <vector>
#include <map>
#include <iostream>

#include <defines.h>
#include <objects.h>
#include <spells.h>
#include <weapon.h>
#include <door.h>

class Character {
    friend class Dungeon;
    friend class Room;
    friend class Spell;
    friend class Object;

    ///TODO change these as the character moves around!
    int xPos, yPos;

    //Misc
    std::string PC_class;
    std::string race;
    std::string short_name; //for monsters
    bool darkvision;
    bool is_monster;
    int xp;
    int level;
    int prof;
    int speed;

    //equipped stuff
    Object *main_hand, *off_hand;

    //Attributes
    std::map<std::string, int> attribute_scores;
    std::map<std::string, int> attribute_mods;

    //Saving throws
    std::map<std::string, int> save_mods;
    int save(std::string stat){ return d20() + save_mods[stat]; }

    //Skills
    std::map<std::string, int> skill_mods;
    std::string base_attribute(std::string skill);
    bool train(std::string skill);
    bool is_trained(std::string skill){ return skill_mods[skill] != attribute_mods[base_attribute(skill)]; }

    //Melee attacks
    int distance_to(Character* target);
    std::vector<std::string> weapon_proficiencies; //"martial", "simple", and/or specific weapon names

    Weapon* weapon_select(Character* target);

    int attack_roll(Weapon* w){
        return w->attack_roll(this);
    }

    ///TODO add class features &etc
    int damage(Weapon* w){
        return w->damage_roll(this);
    }

//    std::string melee_weapon;
//    int melee_atk_mod, melee_weapon_die, melee_dmg_bonus;
//    int melee_atk_roll(){ return d20()+melee_atk_mod; }
//    int melee_dmg(){ return rand()%melee_weapon_die+1+melee_dmg_bonus; }
//
//    //Ranged attacks
//    std::string ranged_weapon;
//    int ranged_atk_mod, ranged_weapon_die, ranged_dmg_bonus;
//    int ranged_atk_roll(){ return d20()+ranged_atk_mod; }
//    int ranged_dmg(){ return rand()%ranged_weapon_die+1+ranged_dmg_bonus; }

    //Misc combat stats
    std::vector<std::string> fighting_styles;

    int _AC;
    int AC();
    int max_hp, cur_hp, temp_hp;
    int hit_dice, hit_die_size;
    int init;
    bool in_melee;

    //Special abilities
    bool second_wind_available;

    //Spellcasting
    std::string casting_stat;
    int spell_attack();
    int spell_save_DC();
    bool within_range(Character* target, Spell* spell);
    int saving_throw(Spell*);

    //Misc other functions
    int next_levelup();

    //Character creation functions
    void standard_array();
    void set_attribute_mods();
    void default_attributes();
    void set_base_skill_mods();
    void default_fighter();
    void fighter();
    void fighter_skills();
    void selectRace();
    void levelup();

    void train_saves(std::string, std::string);

    //monsters
    Character(double CR);
    void set_attributes(int, int, int, int, int, int);
    bool pack_tactics;
    void spawn_cultist(int group_size);
    void spawn_giant_rat(int group_size);
    bool searched;

    bool pick_lock(Door*);
    bool break_down(Door*);
    bool use_locked_door(Door*);
    bool use_blocked_door(Door*);



    public:
        Character();
        void printCharacterSheet();
        bool proficient_with(std::string weapon_type);
        int attribute_mod(std::string att){ return attribute_mods[att]; }
        int proficiency_bonus(){ return prof; }
        bool has_free_hand(){ return main_hand == NULL || off_hand == NULL; }
        int skill_check(std::string skill){ return d20()+skill_mods[skill]; }
        int attribute_chk(std::string att){ return d20() + attribute_mods[att]; }
        bool has_fighting_style(std::string);
        std::string equipped_weapon_type();
        void short_rest();
        bool search_monster(bool print_err_msgs=false);

    //Combat functions
    void attack(Character*, int, int);
    void generic_attack(Character*);
    std::string get_name();
//    void melee_attack(Character*);
//    void ranged_attack(Character*);
    bool is_hit(int attack_roll){ return attack_roll >= AC(); }
    void take_damage(int dmg);
    bool is_alive(){ return cur_hp > 0; }

    bool find_secret_door(Door*);
    void special_action();
    void print_status();

    bool is_injured(){ return cur_hp < max_hp; }
    void heal(int hp);

        void cast(Spell* spell, Character* target);

    std::vector<Weapon*> weapons; //in inventory
    std::vector<Object*> objects;

    void equip(Object*, bool equip_to_offhand=false);
        bool can_open(Door*);

    std::pair<int, int> get_position();
    void set_position(int, int);
    void move_position(int, int);
};



#endif // CHARACTER_H
