#ifndef CHARACTER_H
#define CHARACTER_H

#include <string>
#include <defines.h>
#include <cstdlib>
#include <spells.h>

class Character {
    friend class Dungeon;
    friend class Room;
    friend class Spell;

    ///TODO change these as the character moves around!
    int xPos, yPos;

    //Misc
    std::string PC_class;
    std::string race;
    bool darkvision;
    bool is_monster;
    int xp;
    int level;
    int prof;
    int speed;

    //Attributes
    int str_score, dex_score, con_score, int_score, wis_score, cha_score;
    int str_mod, dex_mod, con_mod, int_mod, wis_mod, cha_mod;

    //Saving throws
    int str_save_mod, dex_save_mod, con_save_mod, int_save_mod, wis_save_mod, cha_save_mod;
    int str_save(){ return d20()+str_save_mod; }
    int dex_save(){ return d20()+dex_save_mod; }
    int con_save(){ return d20()+con_save_mod; }
    int int_save(){ return d20()+int_save_mod; }
    int wis_save(){ return d20()+wis_save_mod; }
    int cha_save(){ return d20()+cha_save_mod; }

    //Melee attacks
    std::string melee_weapon;
    int melee_atk_mod, melee_weapon_die, melee_dmg_bonus;
    int melee_atk_roll(){ return d20()+melee_atk_mod; }
    int melee_dmg(){ return rand()%melee_weapon_die+1+melee_dmg_bonus; }

    //Ranged attacks
    std::string ranged_weapon;
    int ranged_atk_mod, ranged_weapon_die, ranged_dmg_bonus;
    int ranged_atk_roll(){ return d20()+ranged_atk_mod; }
    int ranged_dmg(){ return rand()%ranged_weapon_die+1+ranged_dmg_bonus; }

    //Misc combat stats
    int AC;
    int max_hp, cur_hp, temp_hp;
    int hit_dice, hit_die_size;
    int init;
    bool in_melee;

    //Combat functions
    void attack(Character*, int, int);
    void melee_attack(Character*);
    void ranged_attack(Character*);
    bool is_hit(int attack_roll){ return attack_roll >= AC; }
    void take_damage(int dmg);
    bool is_alive(){ return cur_hp > 0; }

    //Special abilities
    void special_action();
    bool second_wind_available;

    //Spellcasting
    int casting_stat;
    int spell_attack();
    int spell_save_DC();
    void cast(Spell* spell, Character* target);
    bool within_range(Character* target, Spell* spell);
    int saving_throw(Spell*);

    //Misc other functions
    void short_rest();
    int next_levelup();
    void print_status();
    void heal(int hp);

    //Skill mods
    int athletics_mod, acrobatics_mod, sleight_of_hand_mod, stealth_mod, arcana_mod, history_mod, investigation_mod, nature_mod, religion_mod,
        animal_handling_mod, insight_mod, medicine_mod, perception_mod, survival_mod, deception_mod, intimidation_mod, performance_mod, persuasion_mod, thieves_tools_mod;

    //Skill checks
    int athletics() { return d20()+athletics_mod; }
    int acrobatics(){ return d20()+acrobatics_mod; }
    int sleight_of_hand(){ return d20()+sleight_of_hand_mod; }
    int stealth(){ return d20()+stealth_mod; }
    int arcana(){ return d20()+arcana_mod; }
    int history(){ return d20()+history_mod; }
    int investigation(){ return d20()+investigation_mod; }
    int nature(){ return d20()+nature_mod; }
    int religion(){ return d20()+religion_mod; }
    int animal_handling(){ return d20()+animal_handling_mod; }
    int insight(){ return d20()+insight_mod; }
    int medicine(){ return d20()+medicine_mod; }
    int perception(){ return d20()+perception_mod; }
    int survival(){ return d20()+survival_mod; }
    int deception(){ return d20()+deception_mod; }
    int intimidation(){ return d20()+intimidation_mod; }
    int performance(){ return d20()+performance_mod; }
    int persuasion(){ return d20()+persuasion_mod; }
    int thieves_tools() {return d20()+thieves_tools_mod; }
    int str_chk() { return d20()+str_mod; }
    int dex_chk() { return d20()+dex_mod; }
    int con_chk(){ return d20()+con_mod; }
    int int_chk(){ return d20()+int_mod; }
    int wis_chk(){ return d20()+wis_mod; }
    int cha_chk(){ return d20()+cha_mod; }

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

    //monsters
    Character(double CR);
    void set_attributes(int, int, int, int, int, int);
    bool pack_tactics;
    void spawn_cultist(int group_size);
    void spawn_giant_rat(int group_size);
    bool searched;


    public:
    Character();
    void printCharacterSheet();

};


#endif // CHARACTER_H
