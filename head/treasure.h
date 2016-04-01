
#ifndef TREASURE_H
#define TREASURE_H

#include <spells.h>
#include <objects.h>

int magic_item_cost(std::string rarity);

class Treasure : public Object {
public:
    Treasure(std::string _ttype, int num, int gp);    //mundane items
    Treasure(Treasure*);
    virtual ~Treasure();

    virtual std::string get_description(bool plural=true);
    virtual void use(Character* user=NULL, Character* target=NULL){}
};

class Gemstone : public Treasure {
    protected:
        void roll_10gp();
        void roll_50gp();
    public:
        Gemstone(int gp, int num=1);
        ~Gemstone(){}
        std::string get_description(bool plural=true);
};

class Art : public Treasure {
    protected:
        void roll_25gp();
    public:
        Art(int gp, int num=1);
        ~Art(){}
        void identify();
};

class Scroll : public Treasure {
    protected:
        Spell* spell;
    public:
        Scroll(int lvl, int num=1);
        Scroll(Scroll*);
        ~Scroll();
        std::string get_description(bool plural=true);
        void use(Character* user, Character* target);
        bool targets_self(){ return spell->targets_self(); }
        bool targets_enemy(){ return !spell->targets_self(); }
        void identify();
        Object* clone();
};


class Potion : public Treasure {
    bool is_healing;

    public:
        Potion(std::string name, std::string _rarity, int num=1);
        Potion(Potion*);
        ~Potion();
        std::string get_description(bool plural=true);
        int healing_amount();
        void use(Character* user, Character* target=NULL);
        void identify();
        Object* clone();
};

Treasure* magic_item(char table);

#endif // TREASURE_H
