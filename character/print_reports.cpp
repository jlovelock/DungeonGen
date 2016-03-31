
#include <iostream>
#include <pc.h>
#include <weapon.h>
#include <inventory.h>
#include <conditions.h>

using namespace std;

///**************************************************************************************///
///*********************************  PRINT REPORTS  ************************************///
///**************************************************************************************///


void PlayerChar::print_status(){
    //sanity check -- this function is just for PCs
    if(is_monster) return;

    cout << "You have " << cur_hp << " of " << max_hp << " hitpoints remaining";
    if(temp_hp > 0) cout << ", along with a buffer of " << temp_hp << " temporary hitpoints.";
    cout << "." << endl;
    cout << "You have " << hit_dice << " hit";
    if(hit_dice == 1) cout << " die"; else cout << " dice";
    cout << " remaining";
    if(hit_dice > 0){
        cout << ": during a short rest ('rest' command), you can spend ";
        if(hit_dice == 1) cout << "it"; else cout << "them";
        cout << " to restore 1d" << hit_die_size << "+" << attribute_mods["CON"] << " hitpoints";
        if(hit_dice > 1) cout << "each";
    }
    cout << "." << endl;

    if(second_wind_available) cout << "You have your Second Wind available ('special' command)." << endl;
    else cout << "You must take a short rest before you can use your Second Wind feature again." << endl;

    cout << endl << "Print character sheet? [y/n]" << endl;
    string input;
    read(input);
    if(input == "y") printCharacterSheet();

    cout << endl;
}

void PlayerChar::printCharacterSheet(){
    cout << endl << "======================================================================================" << endl;
    cout << "Level " << level << " " << race << " " << PC_class << "  [" << xp << "/" << next_levelup() << " xp to level " << level + 1 << "]" << endl;

    //Attributes
    string atts[6] = {"STR", "DEX", "CON", "INT", "WIS", "CHA"};
    cout << "\t";
    for (int i = 0; i < 6; i++){
        cout << atts[i] << " " << attribute_scores[atts[i]] << " (";
        if(attribute_mods[atts[i]] >= 0) cout << "+";
        cout << attribute_mods[atts[i]] << ")";
        if(i<5) cout << ", ";
    }
    cout << endl;

    cout << "\tAC " << AC() << ", " << cur_hp << "/" << max_hp << " hp";
    if(temp_hp > 0) cout << " [+" << temp_hp << " temp hp]";
    cout << endl;

    inventory->print_weapon_stats(this);

    cout << "\tWeapon proficiencies: simple weapons";
    for(auto it = weapon_proficiencies.begin(); it != weapon_proficiencies.end(); ++it){
        cout << ", " << (*it);
        if((*it) == "martial") cout << " weapons";
    }
    cout << endl;


    //trained skills
    cout << "\tTrained skills: ";
    for(unsigned i = 0; i < skills_list.size(); i++){
        if(is_trained(skills_list[i])){
            cout << skills_list[i] << " (+" << skill_mods[skills_list[i]] << ") ";
        }
    }
    cout << endl;

    cout << "\tFighting style";
    if(fighting_styles.size() > 1) cout << "s";
    cout << ": ";
    for(auto it = fighting_styles.begin(); it != fighting_styles.end(); ++it){
        if(it != fighting_styles.begin()) cout << ", ";
        cout << (*it);
    }
    cout << endl;

    cout << "\tCurrent conditions:";
    bool found = false;
    for(auto it = affected_conditions.begin(); it != affected_conditions.end(); ++it){
        if((*it)->is_active()){
            if(!found){
                found = true;
                cout << endl;
            }
            cout << "\t- " << (*it)->name() << " -- max time remaining = " << (*it)->time_remaining() << "." << endl;
        }
    }
    if(!found) cout << " (none)" << endl;


    cout << "======================================================================================" << endl << endl;
}



