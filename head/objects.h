#ifndef OBJECTS_H
#define OBJECTS_H

#include <string>
#include <iostream>

class Spell;
class Character;

class Object{
    public:
        Object(std::string name);
        Object();
        virtual ~Object();
        std::string name(){ return _name; }
        bool is_two_handed(){ return _2h; }
        bool is_weapon(){ return _is_weapon; }
        bool is_equipped_to(Character*);
        virtual std::string get_description(bool plural=true) { return _description; }
        bool is_identified(){ return identified; }
        int value(){ return _value; }
        virtual void use(Character* user=NULL, void* target=NULL){}
        void action_on_hit(Character* target, Character* attacker=NULL);
        void set_effect_on_hit(Spell* s){ if(!effect_on_hit) effect_on_hit = s; }
        bool depleted(){ return quantity == 0; }
        int get_quantity(){ return quantity; }
        void increase_quantity(int x){ quantity += x; }
        std::string type(){ return _type; }
        bool is_lootable(){ return _lootable; }
        void set_lootable(){ _lootable = true; }
        int weight(){ return _weight; }

        virtual void identify();

    protected:
        std::string _name;
        std::string _description;
        std::string _type;
        bool _2h;
        bool _is_weapon;
        int quantity;
        bool _lootable;

        int _value; //in gp; anything <1gp is free
        float _weight; //in lbs

        bool identified;
        bool consumable;
        std::string rarity;

        Spell* effect_on_hit; //template special abilities as spells

        static int max_id; //starts at 0, increments with each room added
        int _id;

};

#endif // OBJECTS_H
