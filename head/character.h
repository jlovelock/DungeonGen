#ifndef CHARACTER_H
#define CHARACTER_H

#include <string>
#include <cstdlib>
#include <vector>
#include <map>
#include <iostream>

#include <defines.h>

class Weapon;
class Spell;
class Door;
class Object;
class Condition;
class Inventory;

class Character {
    friend class Object;

    protected:
        int xPos, yPos;

        //Misc
        std::string race;
        bool darkvision;
        bool is_monster;
        int prof;
        int speed;
        int xp;

        //Special
        std::vector<std::string> fighting_styles;
        std::vector<std::string> weapon_proficiencies; //"martial", "simple", and/or specific weapon names

        //Equipment
        Object *main_hand, *off_hand;

        //Attributes
        std::map<std::string, int> attribute_scores;
        std::map<std::string, int> attribute_mods;

        //Saving throws
        std::map<std::string, int> save_mods;

        //Skills
        std::map<std::string, int> skill_mods;
        std::string base_attribute(std::string skill);
        bool train(std::string skill);
        bool is_trained(std::string skill){ return skill_mods[skill] != attribute_mods[base_attribute(skill)]; }

        //Combat
        int attack_roll(Weapon* w);
        int damage(Weapon* w, std::string mode = "");
        bool in_melee;
        int _AC;
        int max_hp, cur_hp, temp_hp;
        int hit_dice, hit_die_size;
        int init;

        //Spellcasting
        std::string casting_stat;
        bool within_range(Character* target, Spell* spell);
        Spell* concentrating_on;

        //Character creation
        Character();
        void initialize();
        void set_attributes(std::string mode);
        void set_attributes(int, int, int, int, int, int);
        void stat_boost(std::string stat, int amount);
        void set_base_skill_mods();
        void train_saves(std::string save1="", std::string save2="");

        //ongoing effects
        std::vector<Condition*> affected_conditions;
        std::vector<Condition*> caused_conditions;
        void update_conditions(bool start_of_turn, bool quiet=false);

    public:
        Character(int lvl);
        virtual ~Character();

        /* Stats & Proficiencies */
        int attribute_mod(std::string att){ return attribute_mods[att]; }
        int attribute_score(std::string att){ return attribute_scores[att]; }
        int proficiency_bonus(){ return prof; }
        int skill_check(std::string skill);
        int attribute_chk(std::string att);
        bool has_fighting_style(std::string);
        bool proficient_with(std::string weapon_type);

        /* Combat */
        virtual void attack(Character*, int, int) {}
        virtual void generic_attack(Character*) {}
        bool is_hit(int attack_roll){ return attack_roll >= AC(); }
        int saving_throw(Spell*);
        int save(std::string stat);
        int AC();
        virtual void action_on_kill(Character*)=0;

        /* HP and Damage */
        void take_damage(int dmg);
        bool is_alive(){ return cur_hp > 0; }
        bool is_injured(){ return cur_hp < max_hp; }
        void heal(int hp);
        void adjust_for_resistances(int& dmg, std::string dtype);
        int has_temp_hp(){ return temp_hp > 0; }
        void gain_temp_hp(int n){ if(temp_hp < n) temp_hp = n; }

        /* Positioning */
        std::pair<int, int> get_position();
        void set_position(int, int);
        void move_position(int, int);
        void close_with(Character*);
        bool in_melee_with(Character* opponent);
        int distance_to(Character* target);

        /* Identifiers */
        virtual std::string name() { return ""; }
        virtual std::string full_name() { return ""; }
        std::string get_race(){ return race; }
        bool is_PC(){ return !is_monster; }

        /* Conditions */
        void add_condition(Condition*);
        void remove_condition(std::string, bool quiet=false);
        void cause_condition(Condition*);
        bool is(std::string);

        /* Items and Equipment */
        Inventory* inventory;
        void print_inventory();
        void identify_items();
        void identify_items(std::vector<Object*>, bool&);
        bool is_encumbered();
        bool is_heavily_encumbered();
        bool is_overencumbered();
        bool equip(Object*, bool equip_to_offhand=false);
        std::string equipped_weapon_type();
        bool has_free_hand(){ return main_hand == NULL || off_hand == NULL; }
        void unequip(Object*);

        /* Spellcasting */
        int spell_attack();
        int spell_save_DC();
        bool concentrating(){ return concentrating_on != NULL; }
        void drop_concentration();
        void concentrate_on(Spell* s){ drop_concentration(); concentrating_on = s;}

        /* Misc */
        void start_turn(bool quiet=false);
        void end_turn(bool quiet=false);
        int adjusted_speed();
        int get_xp(){ return xp; }
};



#endif // CHARACTER_H
