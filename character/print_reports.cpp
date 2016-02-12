
#include <iostream>
#include <character.h>

using namespace std;

///**************************************************************************************///
///*********************************  PRINT REPORTS  ************************************///
///**************************************************************************************///


void Character::print_status(){
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

    cout << endl << "Print character sheet? [y/n] ";
    char input;
    cin.get(input);
    if(input == 'y') printCharacterSheet();

    cout << endl;
}

void Character::printCharacterSheet(){
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

//    cout << "\tSTR " << attribute_scores["STR"] << " (";
//    if(str_mod >= 0) cout << "+";
//    cout << str_mod << "), DEX " << dex_score << " (";
//    if(dex_mod >= 0) cout << "+";
//    cout << dex_mod << "), CON " << con_score << " (";
//    if(con_mod >= 0) cout << "+";
//    cout << con_mod << "), INT " << int_score << " (";
//    if(int_mod >= 0) cout << "+";
//    cout << int_mod << "), WIS " << wis_score << " (";
//    if(wis_mod >= 0) cout << "+";
//    cout << wis_mod << "), CHA " << cha_score << " (";
//    if(cha_mod >= 0) cout << "+";
//    cout << cha_mod << ")" << endl;

    cout << "\tAC " << AC() << ", " << cur_hp << "/" << max_hp << " hp" << endl;

    cout << "\tWeapons:" << endl;
    for(vector<Weapon*>::iterator it = weapons.begin(); it != weapons.end(); ++it){
        cout << "\t - " << (*it)->get_weapon_description(this) << endl;
    }

//    cout << "\tMelee attack: " << melee_weapon << " +" << melee_atk_mod << ", d" << melee_weapon_die << "+" << melee_dmg_bonus << " damage" << endl;
//    cout << "\tRanged attack: " << ranged_weapon << " +" << ranged_atk_mod << ", d" << ranged_weapon_die << "+" << ranged_dmg_bonus << " damage" << endl;


    //trained skills
    cout << "\tTrained skills: ";
    for(unsigned i = 0; i < skills_list.size(); i++){
        if(is_trained(skills_list[i])){
            cout << skills_list[i] << " (+" << skill_mods[skills_list[i]] << ") ";
        }
    }
    cout << endl;

    cout << "======================================================================================" << endl << endl;

}

bool Character::proficient_with(string weapon_type){
    for(vector<string>::iterator it = weapon_proficiencies.begin(); it != weapon_proficiencies.end(); ++it) {
        if (*it == weapon_type) return true;
    }
    return false;
}

