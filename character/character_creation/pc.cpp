
#include <pc.h>

using namespace std;

PlayerChar::PlayerChar(int lvl) : Character(){
    xPos = START_X; yPos = START_Y+5;
    is_monster = false;
    level = lvl;
    xp = 0;

    ///@TODO change when more than fighter supported
    weapon_proficiencies.push_back("martial");

    ///@TODO: Class selection
    cout << "New character: default or custom? " << endl;
    string mode;
    read(mode);

    set_attributes(mode);
    select_race(mode);
    set_base_skill_mods();
    if(mode == "custom") fighter(); else default_fighter();

    //debug
    casting_stat = "INT";

    printCharacterSheet();
}


///**************************************************************************************///
///*************************************  RACE  *****************************************///
///**************************************************************************************///

void PlayerChar::select_race(string mode){
    if(mode == "custom") {
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
                stat_boost("CON", 2);
                speed = 25;
                darkvision = true;
                cout << "\tDwarf selected. Choose a subrace: hill (+1 WIS, +1hp/lvl) or mountain (+2 STR)." << endl;
                do {
                    cout << "\tSubrace: ";
                    read(input);

                    if(input == "hill"){
                        cout << "\tHill dwarf selected." << endl;
                        race = "Hill dwarf";
                        stat_boost("WIS", 1);
                        break;
                    } else if(input == "mountain"){
                        cout << "\tMountain dwarf selected." << endl;
                        race = "Mountain dwarf";
                        stat_boost("STR", 2);
                        break;
                    } else { cout << "\tInvalid input." << endl; }
                } while(true);
                break;
            } else if(input == "elf"){
                stat_boost("DEX", 2);
                darkvision = true;
                cout << "\tElf selected. Choose a subrace: high (+1 INT, +1 wizard cantrip, 30ft speed) or wood (+1 wis, 35ft speed)." << endl;
                do {
                    cout << "\tSubrace: ";
                    read(input);

                    if(input == "high"){
                        cout << "\tHigh elf selected." << endl;
                        stat_boost("INT", 1);
                        ///TODO add cantrip
                        race = "High elf";
                        speed = 30;
                        break;
                    } else if(input == "wood"){
                        cout << "\tWood elf selected." << endl;
                        stat_boost("WIS", 1);
                        speed = 35;
                        race = "Wood elf";
                        break;
                    } else cout << "\tInvalid input." << endl;
                } while(true);
                break;
            } else if(input == "human"){
                cout << "\tHuman selected." << endl;
                stat_boost("STR", 1);
                stat_boost("DEX", 1);
                stat_boost("CON", 1);
                stat_boost("INT", 1);
                stat_boost("WIS", 1);
                stat_boost("CHA", 1);
                speed = 30;
                race = "Human";
                darkvision = false;
                break;
            }
        } while(true);

    } else {
        race = "Human";
        stat_boost("STR", 1);
        stat_boost("DEX", 1);
        stat_boost("CON", 1);
        stat_boost("INT", 1);
        stat_boost("WIS", 1);
        stat_boost("CHA", 1);
        speed = 30;
        darkvision = false;
    }
}




