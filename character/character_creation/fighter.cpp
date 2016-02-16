
#include <iostream>
#include <string>

#include <weapon.h>
#include <character.h>

using namespace std;


void Character::default_fighter(){
    PC_class = "fighter";

    //saves
    train_saves("STR", "CON");

    //hp
    max_hp = 10+attribute_mods["CON"];
    cur_hp = max_hp;
    temp_hp = 0;
    hit_dice = level;
    hit_die_size = 10;

    //weapons
    Weapon* w1 = new Weapon("greataxe");
    weapons.push_back(w1);
    equip(w1);
    Weapon* w2 = new Weapon("hand crossbow");
    weapons.push_back(w2);

    //skills
    train("PERCEPTION");
    train("ATHLETICS");
    fighting_styles.push_back("defense");

    //misc
    second_wind_available = true;
    _AC = 17;
    init = attribute_mods["DEX"];
}


void Character::fighter(){
    PC_class = "fighter";

    //saves
    train_saves("STR", "CON");

    //hp
    max_hp = 10+attribute_mods["CON"];
    if(race.find("Hill dwarf")!=string::npos) max_hp++;
    cur_hp = max_hp;
    temp_hp = 0;
    hit_dice = level;
    hit_die_size = 10;

    //skills
    fighter_skills();

    //attacks
//    melee_atk_mod = attribute_mods["STR"] + prof;
//    melee_dmg_bonus = attribute_mods["STR"];
//    ranged_atk_mod = attribute_mods["DEX"] + prof;
//    ranged_dmg_bonus = attribute_mods["DEX"];
    init = attribute_mods["DEX"];

    //misc
    second_wind_available = true;

    string input;

    cout << "4) ARMOR: Choose chainmail or leather. (Leather armor comes with a longbow.)" << endl;
    do {
        cout << "\tArmor choice: ";
        read(input);
        if(input == "chainmail"){
            _AC = 16;
            Weapon* w = new Weapon("hand crossbow");
            weapons.push_back(w);
            //ranged_weapon = "hand crossbow";
            //ranged_weapon_die = 6;
            break;
        } else if(input == "leather"){
            _AC = 11+attribute_mods["DEX"];
            Weapon* w = new Weapon("longbow");
            weapons.push_back(w);
//            ranged_weapon = "longbow";
//            ranged_weapon_die = 8;
            break;
        } else cout << "\tInvalid selection." << endl;
    } while(true);

    cout << "6) WEAPON / SHIELD: Choose two-handed, one-handed, or dual-wielded weapon(s)." << endl;
    do {
        cout << "\ttwo-handed, one-handed, or dual: ";
        read(input);
        if(input == "two-handed"){
            cout << "\tGreataxe selected." << endl;
            Weapon* w = new Weapon("greataxe");
            weapons.push_back(w);
            equip(w);
//            melee_weapon = "greataxe";
//            melee_weapon_die = 12;
            break;
        } else if(input == "one-handed"){
            cout << "\tFinesse weapon (dex to wield)? [y/n] ";
            read(input);
            if(input == "y"){
               cout << "\tRapier selected." << endl;
               Weapon* w = new Weapon("rapier");
               weapons.push_back(w);
               equip(w);
//               melee_weapon = "rapier";
//               melee_atk_mod = attribute_mods["DEX"] + prof;
//               melee_weapon_die = 8;
            } else if(input == "n"){
//                melee_weapon = "longsword";
                cout << "\tLongsword selected." << endl;
                Weapon* w = new Weapon("longsword");
                weapons.push_back(w);
                equip(w);
//                melee_weapon_die = 8;
            }
            cout << "\tShield? [y/n] ";
            read(input);
            if(input == "y"){
                cout << "\tShield added." << endl;
                Object* s = new Object("shield");
                objects.push_back(s);
                equip(s, true);
            } else cout << "\tNo shield." << endl;
            break;
        } else if(input == "dual"){
            cout << "\tFinesse weapons (dex to wield)? [y/n] ";
            read(input);
            if(input == "y"){
//                melee_weapon = "shortswords";
                Weapon* s1 = new Weapon("shortsword");
                Weapon* s2 = new Weapon("shortsword");
                weapons.push_back(s1);
                weapons.push_back(s2);
                equip(s1);
                equip(s2, true);
                cout << "\tShortswords selected." << endl;
//                melee_atk_mod = attribute_mods["DEX"] + prof;
//                melee_weapon_die = 6;
            } else {
//                melee_weapon = "handaxes";
                cout << "\tHandaxes selected." << endl;
                Weapon* s1 = new Weapon("handaxe");
                Weapon* s2 = new Weapon("handaxe");
                weapons.push_back(s1);
                weapons.push_back(s2);
                equip(s1);
                equip(s2, true);
//                melee_weapon_die = 6;
            }
            break;
        }
    } while(true);

    //fighting style
    cout << "4) FIGHTING STYLE: ";
    cout << "Choose 'archery', 'defense', or 'dueling'." << endl;
    ///TODO add 'great-weapon' and 'two-weapon'

    do {
        cout << "\tStyle: ";
        read(input);

        if(input == "archery" || input == "defense" || input == "dueling") {
            fighting_styles.push_back(input);
            break;
//        if(input == "archery"){
//            ranged_atk_mod += 2;
//            break;
//        } else if(input == "defense") {
//            AC++;
//            break;
//        } else if(input == "dueling") {
//            melee_dmg_bonus += 2;
//            break;
        } else {
            cout << "\tInvalid entry." << endl;
        }
    } while(true);


}


void Character::fighter_skills(){
    string input;

    int skills = 2;
    cout << "3) SKILLS: ";
    cout << "Select from acrobatics, animal handling, athletics, history, insight, intimidation, perception, or survival. " << endl;
    while(skills > 0){
        cout << "\tChoose " << skills << " more: ";
        read(input);
        convert_to_uppercase(input);
        if(train(input)) skills--;
    }
}
