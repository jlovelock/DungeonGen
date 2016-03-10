
#ifndef WEAPON_H
#define WEAPON_H

#include <string>
#include <objects.h>
#include <iostream>

class Weapon : public Object {

  public:
    Weapon(){ /* This keeps getting called by the weapon select map thing. It shouldn't be. TODO Debug */ }
    Weapon(std::string name);
    Weapon(std::string name, std::string specifier);
    int damage_roll(Character* c);
    int attack_roll(Character* c, std::string special = "");

    bool is_thrown(){ return _thrown; }
    bool is_finesse(){ return _finesse; }
    bool is_versatile(){ return _versatile; }
    int range(){ return _range; }
    int max_range(){ return _range*3; } ///TODO fix this
    bool is_martial() { return _martial; }

    int get_attack_mod(Character* c);
    int get_dmg_die(){ return _weapon_die; }
    int get_dmg_bonus(Character* c);
    std::string get_dtype(){ return _dtype; }
    std::string get_weapon_description(Character* c);


  private:
    int _range; //Number of feet away you can attack the target with no penalties. Includes thrown weapons.
                // TODO: Thrown weapons. (Should remove themselves from inventory if you throw them, but then add to treasure of room so you can pick them up after)

    bool _martial;
    bool _loading;
    bool _light;
    bool _heavy;
    bool _thrown;
    bool _finesse;
    bool _versatile; ///TODO implement this
    int _weapon_die;
    int _n_dice;

    bool _override;
    bool _dmg_mod;
    bool _atk_mod;
    std::string _dtype;

    int attr_mod(Character* c);
    int base_dmg(Character* c);
    int proficiency(Character* c);
    int misc_atk_mods(Character* c);
    int misc_dmg_mods(Character* c);

//    //Melee attacks
//    std::string melee_weapon;
//    int melee_atk_mod, melee_weapon_die, melee_dmg_bonus;
//    int melee_atk_roll(){ return d20()+melee_atk_mod; }
//    int melee_dmg(){ return rand()%melee_weapon_die+1+melee_dmg_bonus; }
//
//    //Ranged attacks
//    std::string ranged_weapon;
//    int ranged_atk_mod, ranged_weapon_die, ranged_dmg_bonus;
//    int ranged_atk_roll(){ return d20()+ranged_atk_mod; }
//    int ranged_dmg(){ return rand()%ranged_weapon_die+1+ranged_dmg_bonus; }

};


#endif // WEAPON_H
