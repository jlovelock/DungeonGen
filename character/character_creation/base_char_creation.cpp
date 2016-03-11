
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

Character::Character(){
    xPos = START_X; yPos = START_Y+5;
    is_monster = false;
    in_melee = false;
    level = 1;
    prof = 2;
    xp = 0;
    main_hand = NULL; off_hand = NULL;


    weapon_proficiencies.push_back("martial");

    ///@TODO: Class selection
    cout << "New character: default or custom? " << endl;
    string input;
    read(input);
    if(input == "custom"){
        cout << "Custom character selected." << endl;
        standard_array();
        selectRace();
        set_base_skill_mods();
        fighter();
    } else {
        cout << "Standard character selected." << endl;
        default_attributes();
        set_base_skill_mods();
        default_fighter();
    }

    //debug
    casting_stat = "INT";

    printCharacterSheet();
}


///**************************************************************************************///
///***********************************  ATTRIBUTES  *************************************///
///**************************************************************************************///

void Character::default_attributes(){
    race = "Human";
    speed = 30;
    attribute_scores["STR"] = 16;
    attribute_scores["DEX"] = 14;
    attribute_scores["CON"] = 14;
    attribute_scores["INT"] = 10;
    attribute_scores["WIS"] = 14;
    attribute_scores["CHA"] = 9;
    set_attribute_mods();
}

void Character::standard_array(){
    attribute_scores["STR"] = 0;
    attribute_scores["DEX"] = 0;
    attribute_scores["CON"] = 0;
    attribute_scores["INT"] = 0;
    attribute_scores["WIS"] = 0;
    attribute_scores["CHA"] = 0;

    cout << "1) ATTRIBUTES: ";
    cout << "Choose scores ('STR', 'DEX', 'CON', 'INT', 'WIS', and 'CHA') from the standard array." << endl;
    int i = 0;
    string input;
    int scores[] = {15, 14, 13, 12, 10, 8};
    do {
        cout << "\tChoose which stat to put your " << scores[i] << " into: ";
        read(input);
        convert_to_uppercase(input);

        if(input == "STR" || input == "DEX" || input == "CON" || input == "INT" || input == "WIS" || input == "CHA"){
            if(attribute_scores[input] == 0) attribute_scores[input] = scores[i];
            else {
                cout << "\t" << input << " already set to " << attribute_scores[input] << "." << endl;
                continue;
            }
        } else {
            cout << "Stat not recognized: re-enter?" << endl;
            continue;
        }
        i++;

    } while(i<6);

}

void Character::set_attribute_mods(){
    attribute_mods["STR"] = (attribute_scores["STR"]-10)/2;
    attribute_mods["DEX"] = (attribute_scores["DEX"]-10)/2;
    attribute_mods["CON"] = (attribute_scores["CON"]-10)/2;
    attribute_mods["INT"] = (attribute_scores["INT"]-10)/2;
    attribute_mods["WIS"] = (attribute_scores["WIS"]-10)/2;
    attribute_mods["CHA"] = (attribute_scores["CHA"]-10)/2;
}

void Character::set_attributes(int str, int dex, int con, int intel, int wis, int cha){
    attribute_scores["STR"] = str;
    attribute_scores["DEX"] = dex;
    attribute_scores["CON"] = con;
    attribute_scores["INT"] = intel;
    attribute_scores["WIS"] = wis;
    attribute_scores["CHA"] = cha;

    set_attribute_mods();
    set_base_skill_mods();
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


///**************************************************************************************///
///*************************************  RACE  *****************************************///
///**************************************************************************************///

void Character::selectRace(){
    cout << "2) RACES" << endl;
    cout << "Select from the following races: " << endl;
    cout << "\tdwarf (+2 CON, 25ft speed, darkvision, dwarven resilience)" << endl;
    cout << "\telf (+2 DEX, 30-35ft speed, darkvision, keen senses, fey ancestry)" << endl;
    cout << "\thuman (+1 to all stats, 30ft speed)" << endl;

    cout << "\tRace: ";
    string input;
    read(input);

    do {
        if(input == "dwarf"){
            attribute_scores["CON"] += 2;
            speed = 25;
            darkvision = true;
            cout << "\tDwarf selected. Choose a subrace: hill (+1 WIS, +1hp/lvl) or mountain (+2 STR)." << endl;
            do {
                cout << "\tSubrace: ";
                read(input);

                if(input == "hill"){
                    cout << "\tHill dwarf selected." << endl;
                    race = "Hill dwarf";
                    attribute_scores["WIS"] += 1;
                    break;
                } else if(input == "mountain"){
                    cout << "\tMountain dwarf selected." << endl;
                    race = "Mountain dwarf";
                    attribute_scores["STR"] += 2;
                    break;
                } else { cout << "\tInvalid input." << endl; }
            } while(true);
            break;
        } else if(input == "elf"){
            attribute_scores["DEX"] += 2;
            darkvision = true;
            cout << "\tElf selected. Choose a subrace: high (+1 INT, +1 wizard cantrip, 30ft speed) or wood (+1 wis, 35ft speed)." << endl;
            do {
                cout << "\tSubrace: ";
                read(input);

                if(input == "high"){
                    cout << "\tHigh elf selected." << endl;
                    attribute_scores["INT"] += 1;
                    ///TODO add cantrip
                    race = "High elf";
                    speed = 30;
                    break;
                } else if(input == "wood"){
                    cout << "\tWood elf selected." << endl;
                    attribute_scores["WIS"] += 1;
                    speed = 35;
                    race = "Wood elf";
                    break;
                } else cout << "\tInvalid input." << endl;
            } while(true);
            break;
        } else if(input == "human"){
            cout << "\tHuman selected." << endl;
            attribute_scores["STR"]++;
            attribute_scores["DEX"]++;
            attribute_scores["CON"]++;
            attribute_scores["INT"]++;
            attribute_scores["WIS"]++;
            attribute_scores["CHA"]++;
            speed = 30;
            race = "Human";
            darkvision = false;
            break;
        }
    } while(true);

    set_attribute_mods();
}




