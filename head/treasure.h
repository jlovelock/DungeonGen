
#ifndef TREASURE_H
#define TREASURE_H

#include <spells.h>
#include <objects.h>

int magic_item_cost(std::string rarity);

class Treasure : public Object {
protected:
    std::string _type;
public:
    Treasure(std::string _ttype, int num, int gp);    //mundane items
    virtual ~Treasure();

    int quantity;
    std::string type(){ return _type; }
    virtual std::string get_description();
};

class Gemstone : public Treasure {
    protected:
        void roll_10gp();
        void roll_50gp();
    public:
        Gemstone(int gp, int num=1);
        ~Gemstone(){}
        std::string get_description();
};

class Art : public Treasure {
    protected:
        void roll_25gp();
    public:
        Art(int gp, int num=1);
        ~Art(){}
};

class Scroll : public Treasure {
    protected:
        Spell* spell;
    public:
        Scroll(int lvl, int num=1);
        ~Scroll(){}
        std::string get_description();
        void use(Character* user, Character* target);
};


class Potion : public Treasure {
    bool is_healing;

    public:
        Potion(std::string name, std::string _rarity, int num=1);
        ~Potion();
        std::string get_description();
        int healing_amount();
        void use(Character* user);

};

Treasure* magic_item(char table);

#endif // TREASURE_H
