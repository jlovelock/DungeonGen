#ifndef OBJECTS_H
#define OBJECTS_H

#include <string>

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
        virtual std::string get_description() { return _description; }
        bool is_identified(){ return identified; }
        void identify(){ identified = true; }
        int value(){ return _value; }
        virtual void use(Character* user=NULL, void* target=NULL){}
        void action_on_hit(Character* target, Character* attacker=NULL);
        void set_effect_on_hit(Spell* s){ if(!effect_on_hit) effect_on_hit = s; }

    protected:
        std::string _name;
        std::string _description;
        bool _2h;
        bool _is_weapon;

        int _value; //in gp; anything <1gp is free

        bool identified;
        bool consumable;
        std::string rarity;

        Spell* effect_on_hit; //template special abilities as spells

        static int max_id; //starts at 0, increments with each room added
        int _id;

};

#endif // OBJECTS_H
