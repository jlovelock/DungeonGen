#include <spells.h>

class Treasure {
public:
    int quantity;
    int value; //in gp
    std::string name;
    std::string description;
    std::string type;
    bool identified;
    bool consumable;
    int rarity;

    int healing_amount();

    Treasure();
    Treasure(std::string _type, int num, int gp);    //mundane items
    Treasure(char);                                  //magic items
    ~Treasure();

    void set_gemstones(int gp);
    void set_art(int gp);

    std::string get_description();

    bool is_scroll;
    Spell* spell;
};

