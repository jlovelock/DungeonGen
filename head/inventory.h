
#ifndef INVENTORY_H
#define INVENTORY_H

#include <vector>
#include <string>

class Treasure;
class Potion;
class Scroll;
class Weapon;
class Object;
class Character;

class Inventory {
private:
    std::vector<Potion*> potions;
    std::vector<Scroll*> scrolls;
    std::vector<Weapon*> weapons;
    std::vector<Object*> misc;
    int cp, sp, ep, gp, pp;

public:
    Inventory();
    Inventory(std::string);
    ~Inventory();
    void clear();

    std::vector<Potion*> potion_vector(){ return potions; }
    std::vector<Scroll*> scroll_vector(){ return scrolls; }
    std::vector<Weapon*> weapon_vector(){ return weapons; }
    std::vector<Object*> misc_vector(){ return misc; }

    std::vector<Object*> potion_obj_vector();
    std::vector<Object*> scroll_obj_vector();
    std::vector<Object*> weapon_obj_vector();
    std::vector<Object*> misc_obj_vector(){ return misc; }

    void add(Object*);
    void add_coins(int n, std::string type);
    void transfer(Inventory*, std::string); //parameter inventory emptied, xfer'd to caller
    void remove(Object*);

    Weapon* weapon_select(int distance);
    Object* get_item(std::string);
    void print_weapon_stats(Character*);
    void print(Character*);

    void roll_incidental_treasure();
    void roll_full_treasure();
    void set_monster_treasure(int CR);

    bool is_empty();
    bool has_unidentified_items();
    int num_unidentified_scrolls();
};



#endif // INVENTORY_H
