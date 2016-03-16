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

class Character {
    friend class Dungeon;
    friend class Room;
    friend class Spell;
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
        int save(std::string stat){ return d20() + save_mods[stat]; }

        //Skills
        std::map<std::string, int> skill_mods;
        std::string base_attribute(std::string skill);
        bool train(std::string skill);
        bool is_trained(std::string skill){ return skill_mods[skill] != attribute_mods[base_attribute(skill)]; }

        //Melee attacks
        int distance_to(Character* target);
        int attack_roll(Weapon* w);
        int damage(Weapon* w, std::string mode = "");
        bool in_melee;

        int _AC;
        int max_hp, cur_hp, temp_hp;
        int hit_dice, hit_die_size;
        int init;

        //Spellcasting
        std::string casting_stat;
        int spell_attack();
        int spell_save_DC();
        bool within_range(Character* target, Spell* spell);

        //Character creation
        Character();
        void initialize();
        void set_attributes(std::string mode);
        void set_attributes(int, int, int, int, int, int);
        void stat_boost(std::string stat, int amount);
        void set_base_skill_mods();
        void train_saves(std::string save1="", std::string save2="");

    public:
        Character(int lvl);
        virtual ~Character();

        int attribute_mod(std::string att){ return attribute_mods[att]; }
        int proficiency_bonus(){ return prof; }
        int skill_check(std::string skill){ return d20()+skill_mods[skill]; }
        int attribute_chk(std::string att){ return d20() + attribute_mods[att]; }
        bool has_fighting_style(std::string);

        //Combat functions
        virtual void attack(Character*, int, int) {}
        virtual void generic_attack(Character*) {}
        bool is_hit(int attack_roll){ return attack_roll >= AC(); }
        int saving_throw(Spell*);
        int AC();

        void take_damage(int dmg);
        bool is_alive(){ return cur_hp > 0; }
        bool is_injured(){ return cur_hp < max_hp; }
        void heal(int hp);

        //Equipment
        std::vector<Weapon*> weapons; //in inventory
        std::vector<Object*> objects;
        bool equip(Object*, bool equip_to_offhand=false);
        std::string equipped_weapon_type();
        bool has_free_hand(){ return main_hand == NULL || off_hand == NULL; }
        bool proficient_with(std::string weapon_type);

        //Positioning
        std::pair<int, int> get_position();
        void set_position(int, int);
        void move_position(int, int);
        void close_with(Character*);
        bool in_melee_with(Character* opponent);

        virtual std::string name() { return ""; }
        virtual std::string full_name() { return ""; }

        void cast(Spell* spell, Character* target);

        int get_xp(){ return xp; }
        virtual void action_on_kill(Character*) {}

};



#endif // CHARACTER_H
