
#ifndef MONSTER_H
#define MONSTER_H

#include <character.h>

class Character;

class Monster : public Character {

    std::vector<std::string> features;
    void spawn_cultist(int group_size);
    void spawn_giant_rat(int group_size);
    void spawn_poisonous_snake(int group_size);

    bool searched;
    std::string _short_name;
    std::string _full_name;

public:
    Monster(double CR);
    ~Monster();

    void generic_attack(Character*);
    void attack(Character* opponent, int attack_roll, int dmg);

//    bool search_monster(bool print_err_msgs=false);
    bool was_looted(){ return searched; }
    void mark_as_looted(){ searched = false; }
    bool has(std::string);

    std::string name() { return _short_name; }
    std::string full_name() { return _full_name; }


    void action_on_kill(Character*);

};



#endif // MONSTER_H
