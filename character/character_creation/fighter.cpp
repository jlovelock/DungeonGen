
#include <iostream>
#include <string>

#include <weapon.h>
#include <inventory.h>
#include <pc.h>

using namespace std;


void PlayerChar::default_fighter(){
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
    inventory->add(w1);
    equip(w1);
    Weapon* w2 = new Weapon("hand crossbow");
    inventory->add(w2);

    //skills
    train("PERCEPTION");
    train("ATHLETICS");
    fighting_styles.push_back("defense");

    //misc
    second_wind_available = true;
    _AC = 17;
    init = attribute_mods["DEX"];
}


void PlayerChar::fighter(){
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

    //misc
    fighter_skills();
    second_wind_available = true;
    init = attribute_mods["DEX"];

    string input;

    cout << "4) ARMOR: Choose chainmail or leather. (Leather armor comes with a longbow.)" << endl;
    do {
        read(input);
        if(contains(input, "chainmail")){
            _AC = 16;
            Weapon* w = new Weapon("hand crossbow");
            inventory->add(w);
            break;
        } else if(contains(input, "leather")){
            _AC = 11+attribute_mods["DEX"];
            Weapon* w = new Weapon("longbow");
            inventory->add(w);
            break;
        } else cout << "\tInvalid selection." << endl;
    } while(true);

    cout << "6) WEAPON / SHIELD: Choose two-handed, one-handed, or dual-wielded weapon(s)." << endl;
    do {
        read(input);
        if(contains(input, "two")){
            cout << "\tGreataxe selected." << endl;
            Weapon* w = new Weapon("greataxe");
            inventory->add(w);
            equip(w);
            break;
        } else if(contains(input, "one")){
            cout << "\tFinesse weapon (dex to wield)? [y/n] ";
            read(input);
            if(input == "y"){
               cout << "\tRapier selected." << endl;
               Weapon* w = new Weapon("rapier");
               inventory->add(w);
               equip(w);
            } else if(input == "n"){
                cout << "\tLongsword selected." << endl;
                Weapon* w = new Weapon("longsword");
                inventory->add(w);
                equip(w);
            }
            cout << "\tShield? [y/n] ";
            read(input);
            if(input == "y"){
                cout << "\tShield added." << endl;
                Object* s = new Object("shield");
                inventory->add(s);
                equip(s, true);
            } else cout << "\tNo shield." << endl;
            break;
        } else if(contains(input, "dual")){
            cout << "\tFinesse weapons (dex to wield)? [y/n] ";
            read(input);
            if(input == "y"){
                Weapon* s1 = new Weapon("shortsword");
                Weapon* s2 = new Weapon("shortsword");
                inventory->add(s1);
                inventory->add(s2);
                equip(s1);
                equip(s2, true);
                cout << "\tShortswords selected." << endl;
            } else {
                cout << "\tHandaxes selected." << endl;
                Weapon* s1 = new Weapon("handaxe");
                Weapon* s2 = new Weapon("handaxe");
                inventory->add(s1);
                inventory->add(s2);
                equip(s1);
                equip(s2, true);
            }
            break;
        }
    } while(true);

    //fighting style
    cout << "4) FIGHTING STYLE: ";
    cout << "Choose 'archery', 'defense', or 'dueling'." << endl;
    ///TODO add 'great-weapon' and 'two-weapon'

    do {
        read(input);

        if(input == "archery" || input == "defense" || input == "dueling") {
            fighting_styles.push_back(input);
            break;
        } else {
            cout << "\tInvalid entry." << endl;
        }
    } while(true);


}


void PlayerChar::fighter_skills(){
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
