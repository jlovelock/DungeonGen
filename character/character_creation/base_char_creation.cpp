
#include <iostream>
#include <string.h>
#include <limits.h>
#include <math.h>
#include <vector>

#include <character.h>
#include <defines.h>

using namespace std;


void Character::train_saves(string save1, string save2){
    convert_to_uppercase(save1); convert_to_uppercase(save2);
    string att_list[6] = {"STR", "DEX", "CON", "INT", "WIS", "CHA"};
    for(int i = 0; i < 6; i++){
        save_mods[att_list[i]] = attribute_mods[att_list[i]];
        if(att_list[i] == save1 || att_list[i] == save2){
            save_mods[att_list[i]] += prof;
        }
    }
}

///**************************************************************************************///
///*********************************  INITIALIZERS  *************************************///
///**************************************************************************************///

void Character::initialize(){
    in_melee = false;
    prof = 2;
    main_hand = NULL; off_hand = NULL;
}

Character::Character(){
    initialize();
}

//Character::Character(int lvl){
//    initialize();
//
//    xPos = START_X; yPos = START_Y+5;
//    is_monster = false;
//    level = lvl;
//    xp = 0;
//
//    ///@TODO change when more than fighter supported
//    weapon_proficiencies.push_back("martial");
//
//    ///@TODO: Class selection
//    cout << "New character: default or custom? " << endl;
//    string mode;
//    read(mode);
//
//    set_attributes(mode);
//    select_race(mode);
//    set_base_skill_mods();
//    if(mode == "custom") fighter(); else default_fighter();
//
//    //debug
//    casting_stat = "INT";
//
//    printCharacterSheet();
//}


///**************************************************************************************///
///***********************************  ATTRIBUTES  *************************************///
///**************************************************************************************///

/*
 * Call this function instead of just updating attribute_scores values directly!
 * If you don't, the mods won't be reflected.
 *
 * Possible TODO: instead of keeping mods in their own vector, just keep track of the scores
 *   Then have a function that calculates/returns the mod 'live', instead.
 */
void Character::stat_boost(string stat, int amount){
    attribute_scores[stat] += amount;
    attribute_mods[stat] = (attribute_scores[stat]-10)/2;
}


void Character::set_attributes(int str, int dex, int con, int intel, int wis, int cha){
    stat_boost("STR", str);
    stat_boost("DEX", dex);
    stat_boost("CON", con);
    stat_boost("INT", intel);
    stat_boost("WIS", wis);
    stat_boost("CHA", cha);
    set_base_skill_mods();
}


void Character::set_attributes(string mode){
    if(mode == "custom"){
        cout << "1) ATTRIBUTES: ";
        cout << "Choose scores ('STR', 'DEX', 'CON', 'INT', 'WIS', and 'CHA') from the standard array." << endl;
        int i = 0;
        string input;
        int scores[] = {15, 14, 13, 12, 10, 8};
        do {
            cout << "\tChoose which stat to put your " << scores[i] << " into: ";
            read(input);
            convert_to_uppercase(input);

            if(input == "STR" || input == "DEX" || input == "CON" || input == "INT" || input == "WIS" || input == "CHA")
                if(!attribute_scores[input])
                    stat_boost(input, scores[i++]); //attribute_scores[input] = scores[i];
                else
                    cout << "\t" << input << " already set to " << attribute_scores[input] << "." << endl;

            else
                cout << "Stat not recognized: re-enter?" << endl;

        } while(i<6);

    } else {
        set_attributes(15, 13, 13, 9, 13, 8);
    }
}




///**************************************************************************************///
///************************************  SKILLS  ****************************************///
///**************************************************************************************///

void Character::set_base_skill_mods(){
    for (unsigned i = 0; i < skills_list.size(); i++){
        skill_mods[skills_list[i]] = attribute_mods[base_attribute(skills_list[i])];
    }

    if(race.find("elf") != string::npos){
        cout << "\tKeen senses (elven racial trait) gives you free training in perception." << endl;
        train("PERCEPTION");
    }
}

bool Character::train(string skill){
    if(!is_skill(skill)) {
        cout << "\"" << skill << "\" is not a skill!" << endl;
        return false;
    } else if(is_trained(skill)){
        cout << "Already trained in this skill!" << endl;
        return false;
    }

    ///TODO verify that it's in the skill list for your class, and if not, return false

    //If you reach here, you're good =]
    skill_mods[skill] += prof;
    return true;
}

string Character::base_attribute(string skill){
    convert_to_uppercase(skill);

    if(
      skill == "ATHLETICS"
    )
        return "STR";

    else if(
      skill == "ACROBATICS" ||
      skill == "SLEIGHT OF HAND" ||
      skill == "THIEVES TOOLS"
    )
        return "DEX";

    else if(
      skill == "ARCANA" ||
      skill == "HISTORY" ||
      skill == "INVESTIGATION" ||
      skill == "NATURE" ||
      skill == "RELIGION"
    )
        return "INT";

    else if(
      skill == "ANIMAL HANDLING" ||
      skill == "INSIGHT" ||
      skill == "MEDICINE" ||
      skill == "PERCEPTION" ||
      skill == "SURVIVAL"
    )
        return "WIS";

    else if(
      skill == "DECEPTION" ||
      skill == "INTIMIDATION" ||
      skill == "PERFORMANCE" ||
      skill == "PERSUASION"
    )
        return "CHA";

    else {
        cout << "Looks like there's a typo somewhere. You're trying to deal with the \"" << skill << "\" skill, which doesn't exist. Exiting program!" << endl;
        exit(EXIT_FAILURE);
    }

}


