#include <character.h>
#include <iostream>
#include <string.h>
#include <limits.h>
#include <spells.h>
#include <math.h>
#include <vector>
#include <weapon.h>

using namespace std;

///**************************************************************************************///
///******************************  CHARACTER CREATION  **********************************///
///**************************************************************************************///

Character::Character(){
    xPos = 0; yPos = 0; ///TODO set properly
    is_monster = false;
    in_melee = false;
    level = 1;
    prof = 2;
    xp = 0;
    main_hand = NULL; off_hand = NULL;

    cout << "New character: default or custom? ";
    string input;
    getline(cin, input);
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

    printCharacterSheet();
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

void Character::fighter_skills(){
    string input;

    int skills = 2;
    cout << "3) SKILLS: ";
    cout << "Select from ACROBATICS, ANIMAL HANDLING, ATHLETICS, HISTORY, INSIGHT, INTIMIDATION, PERCEPTION, or SURVIVAL (all-caps only). " << endl;
    while(skills > 0){
        cout << "\tChoose " << skills << " more: ";
        getline(cin, input);
        if(train(input)) skills--;
    }
}

void Character::default_attributes(){
    race = "Human";
    attribute_scores["STR"] = 16;
    attribute_scores["DEX"] = 14;
    attribute_scores["CON"] = 14;
    attribute_scores["INT"] = 10;
    attribute_scores["WIS"] = 14;
    attribute_scores["CHA"] = 9;
    set_attribute_mods();
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



string Character::base_attribute(string skill){
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

void Character::set_base_skill_mods(){
    for (unsigned i = 0; i < skills_list.size(); i++){
        skill_mods[skills_list[i]] = attribute_mods[base_attribute(skills_list[i])];
    }
}

void Character::default_fighter(){
    PC_class = "fighter";

    //saves
    save_mods["STR"] = attribute_mods["STR"] + prof;
    save_mods["DEX"] = attribute_mods["DEX"];
    save_mods["CON"] = attribute_mods["CON"] + prof;
    save_mods["INT"] = attribute_mods["INT"];
    save_mods["WIS"] = attribute_mods["WIS"];
    save_mods["CHA"] = attribute_mods["CHA"];

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

    //attacks
//    melee_weapon = "greataxe";
//    melee_atk_mod = attribute_mods["STR"] + prof;
//    melee_weapon_die = 12;
//    melee_dmg_bonus = attribute_mods["STR"];
//    ranged_weapon = "hand crossbow";
//    ranged_atk_mod = attribute_mods["DEX"] + prof;
//    ranged_weapon_die = 6;
//    ranged_dmg_bonus = attribute_mods["DEX"];

    //skills
    train("PERCEPTION");
    train("ATHLETICS");

    //misc
    second_wind_available = true;
    _AC = 17;
    init = attribute_mods["DEX"];
}

int Character::AC(){
    if((main_hand && main_hand->name() == "shield") || (off_hand && off_hand->name() == "shield"))
        return _AC+2;
    else
        return _AC;

}

void Character::fighter(){
    PC_class = "fighter";

    //saves
    save_mods["STR"] = attribute_mods["STR"] + prof;
    save_mods["DEX"] = attribute_mods["DEX"];
    save_mods["CON"] = attribute_mods["CON"] + prof;
    save_mods["INT"] = attribute_mods["INT"];
    save_mods["WIS"] = attribute_mods["WIS"];
    save_mods["CHA"] = attribute_mods["CHA"];

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
        getline(cin, input);
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
        getline(cin, input);
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
            getline(cin, input);
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
            getline(cin, input);
            if(input == "y"){
                cout << "\tShield added." << endl;
                Object* s = new Object("shield");
                objects.push_back(s);
                equip(s, true);
            } else cout << "\tNo shield." << endl;
            break;
        } else if(input == "dual"){
            cout << "\tFinesse weapons (dex to wield)? [y/n] ";
            getline(cin, input);
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
        getline(cin, input);

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

void Character::selectRace(){
    cout << "2) RACES" << endl;
    cout << "Select from the following races: " << endl;
    cout << "\tdwarf (+2 CON, 25ft speed, darkvision, dwarven resilience)" << endl;
    cout << "\telf (+2 DEX, 30-35ft speed, darkvision, keen senses, fey ancestry)" << endl;
    cout << "\thuman (+1 to all stats, 30ft speed)" << endl;

    cout << "\tRace: ";
    string input;
    getline(cin, input);

    do {
        if(input == "dwarf"){
            attribute_scores["CON"] += 2;
            speed = 25;
            darkvision = true;
            cout << "\tDwarf selected. Choose a subrace: hill (+1 WIS, +1hp/lvl) or mountain (+2 STR)." << endl;
            do {
                cout << "\tSubrace: ";
                getline(cin, input);

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
                getline(cin, input);

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
    if(race.find("elf") != string::npos){
        train("PERCEPTION");
    }
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
        getline(cin, input);

        if(input == "STR" || input == "DEX" || input == "CON" || input == "INT" || input == "WIS" || input == "CHA"){
            if(attribute_scores[input] == 0) attribute_scores[input] = scores[i];
            else {
                cout << "\t" << input << " already set to " << attribute_scores[input] << "." << endl;
                continue;
            }
        } else {
            cout << "Stat not recognized. (Did you enter it in all caps?)" << endl;
            continue;
        }
        i++;

    } while(i<6);

}



///**************************************************************************************///
///*******************************  MONSTER CREATION  ***********************************///
///**************************************************************************************///

//For spawning monsters
Character::Character(double CR){
    ///TODO add more monster options, random roll for one of the appropriate CR
    xPos = 0; yPos = 0; ///TODO set properly
    is_monster = true;
    searched = false;
    prof = 2;
    if(rand()%2) spawn_cultist(1);
    else spawn_giant_rat(1);
    cur_hp = max_hp;
    temp_hp = 0;
}

///TODO include keen senses, darkvision
void Character::spawn_giant_rat(int group_size){
    _AC = 12;
    max_hp = d6()+d6();

    if(group_size == 1) xp = 38;
    else if(group_size == 2) xp = 50;

    set_attributes(7,15,11,2,10,4);

    Weapon* w = new Weapon("bite", "4/1d4+2 piercing");
    weapons.push_back(w);
    main_hand = w;
    off_hand = NULL;

    race = "rat";
    PC_class = "giant rat";
    pack_tactics = true;
}

//group size represents how many of this monster are in the same room
///TODO: instead of group_size (which assumes identical monsters), have another function
///  handle the monster spawn and therefore set the xp values
void Character::spawn_cultist(int group_size){
    _AC = 12;
    max_hp = d8()+d8();
    speed = 30;

    if(group_size == 1) xp = 38;
    else if(group_size == 2) xp = 50;

    set_attributes(11,12,10,10,11,10);
    train("DECEPTION");
    train("RELIGION");

    Weapon* w = new Weapon("scimitar","3/1d6+1 piercing");
    weapons.push_back(w);
    main_hand = w;
    off_hand = NULL;

    PC_class = "cultist"; /// TODO rename this variable to something more reasonable? =P
    race = "human"; //arbitrary
    pack_tactics = false;
}



///**************************************************************************************///
///***********************************  ATTACKS  ****************************************///
///**************************************************************************************///


Weapon* Character::weapon_select(Character* target){
        if(is_monster) return weapons[0]; //TODO multi-weapon support for monsters??

        map<string, Weapon*> choice_matrix;

        cout << "Which weapon do you want to use?:" << endl;

        while(true) {
            for(vector<Weapon*>::iterator it = weapons.begin(); it != weapons.end(); ++it) {
                if(distance_to(target) < (*it)->range()){
                    choice_matrix[(*it)->name()] = *it;
                    cout << "\t - " << (*it)->name();
                    if(distance_to(target) >= 10 && (*it)->is_thrown())
                        cout << " (thrown)";
                    cout << endl;
                }
                else {
                    cout << "\t(" << (*it)->name() << " out of range.)" << endl;
                }
            }

            cout << endl << ">> ";
            string choice;
            getline(cin, choice);
            map<string, Weapon*>::iterator w = choice_matrix.find(choice);
            if(w == choice_matrix.end()) {
                cout << "Invalid selection. Your choices are as follows: " << endl;
            } else {
                cout << choice_matrix.at(choice)->name() << " selected." << endl;
                return choice_matrix.at(choice);
            }
        }
}

void Character::attack(Character* opponent, int attack_roll, int dmg){
    if(!opponent->is_alive()){
        cout << "Uh, he's already dead, but go right ahead if you really want to." << endl << endl;
        return;
    }
    if(attack_roll >= opponent->AC()){
        if(!is_monster){
            cout << "Your attack connects, dealing " << dmg << " points of damage to the " << opponent->PC_class << "." << endl;
            opponent->take_damage(dmg);
            if(!opponent->is_alive()){
                ///TODO: handle the rest of the cleanup. Delete opponent, set monster in room to false, etc.
                xp += opponent->xp;
                cout << "You have gained " << opponent->xp << " experience points." << endl << endl;
                if(xp > next_levelup()) levelup();
                in_melee = false;
            }
        } else {
            cout << "The " << PC_class << "'s weapon slams into your side, dealing " << dmg << " points of damage." << endl;
            opponent->take_damage(dmg);
        }
    } else {
        if(is_monster) cout << "His "; else cout << "Your ";
        cout << "blow fails to connect." << endl << endl;
    }


}

/*
 * Todos:
 *  - Enforce weapon ranges / character locations
 *  - Distinguish between melee / ranged / etc in flavour
 */
void Character::generic_attack(Character* opponent){
    Weapon* w;
    if(main_hand->is_weapon()) w = (Weapon*)main_hand;
    else if(off_hand->is_weapon()) w = (Weapon*)off_hand;
    else w = weapon_select(opponent);

    if(is_monster) cout << "The " << PC_class << " attacks you with his " << w->name() << " drawn." << endl;
    else cout << "You attack the " << opponent->PC_class << " with your " << w->name() << " drawn." << endl;

    attack(opponent, attack_roll(w), damage(w));
}


//void Character::melee_attack(Character* opponent){
//    Weapon* w = weapon_select(opponent);
//    if(in_melee == false){
//        if(is_monster) cout << "The " << PC_class << " charges you from across the room with his " << w.name() << " drawn." << endl;
//        else cout << "You charge forward to meet the " << opponent->PC_class << " with your " << w.name() << " drawn." << endl;
//    }
//    attack(opponent, attack_roll(w), damage(w));
//    in_melee = true;
//    opponent->in_melee = true; ///TODO FIXME if you attack at range first and melee second, it doesn't set your flag to "in melee", even though this line should have...
//}
//
//void Character::ranged_attack(Character* opponent){
//    Weapon w = weapon_select(opponent);
//    if(!is_monster) cout << "You draw your " << w.name() << " and prepare to strike." << endl;
//    else cout << "The " << PC_class << " draws a " << w.name() << " and readies a shot." << endl;
//    attack(opponent, attack_roll(w), damage(w));
//    in_melee = false;
//    opponent->in_melee = false;
//}

void Character::take_damage(int dmg){
    ///TODO make this whole section more flavourful!
    if(is_monster){
        if(cur_hp > dmg){
            cur_hp -= dmg;
            if((float)cur_hp/max_hp > 0.5) cout << "He still looks ready to fight." << endl << endl;
            else if((float)cur_hp/max_hp > 0.25) cout << "He's not in very good shape." << endl << endl;
            else cout << "He's barely alive at this point." << endl << endl;
        } else {
            cur_hp = 0;
            cout << "He collapses on the ground, dead." << endl;
        }
        return;
    }

    if(temp_hp > 0){
        if(temp_hp >= dmg){
            temp_hp -= dmg;
            cout << "Fortunately, your temporary hitpoints absorb the entire blow. (You have " << temp_hp << " temp hp remaining)." << endl;
            return;
        } else {
            cout << "Fortunately, your temporary hitpoints absorb " << temp_hp << " points of damage, but ";
            dmg -= temp_hp;
            cout << dmg << " points of damage are still dealt directly to you. " << endl;
            cur_hp -= dmg;

            if(cur_hp > 0){
                cout << "You have " << cur_hp << "/" << max_hp << " hitpoints remaining." << endl << endl;
                return;
            } else {
                ///TODO implement death saves, stabilizing, etc
                cur_hp = 0;
                cout << "You have died." << endl << endl;
                return;
            }
        }
    } else {
        if(dmg < cur_hp){
            cur_hp -= dmg;
            cout << "You have " << cur_hp << "/" << max_hp << " hitpoints remaining." << endl << endl;
            return;
        } else { ///TODO same as above
            cur_hp = 0;
            cout << "You have died." << endl << endl;
            return;
        }
    }
}



///**************************************************************************************///
///*************************************  MAGIC  ****************************************///
///**************************************************************************************///

///TODO add flavour
///TODO add support for enemy spellcasters (don't print the same messages based on is_enemy)
///TODO add support for non-damaging spells
void Character::cast(Spell* spell, Character* target){
    if(!within_range(target, spell)){
        cout << "You're too far away to do that: "
        << spell->name << " only has a range of " << spell->range << " feet, and you are "
        << sqrt(pow(xPos-target->xPos, 2)+pow(yPos-target->yPos, 2)) << " feet away from the " << target->PC_class << "." << endl;

        return;
    }
    if(spell->attack_roll_required){
        if(spell_attack() > target->AC()){
            int damage = spell->damage();
            cout << "Your " << spell->name << " hits the " << target->PC_class << " for " << damage << " points of " << spell->damage_type << " damage." << endl;
            target->take_damage(damage);
        } else {
            cout << "The " << target->PC_class << " manages to avoid the effect." << endl;
        }
    } else if(spell->save_allowed){
        if(target->saving_throw(spell) >= spell_save_DC()){
            cout << "The " << target->PC_class << " manages to avoid the effect." << endl;
        } else {
            int damage = spell->damage();
            cout << "Your " << spell->name << " hits the " << target->PC_class << " for " << damage << " points of " << spell->damage_type << " damage." << endl;
            target->take_damage(damage);
        }
    } else {
        int damage = spell->damage();
        cout << "Your " << spell->name << " hits the " << target->PC_class << " for " << damage << " points of " << spell->damage_type << " damage." << endl;
        target->take_damage(damage);
    }

    if(!target->is_alive()){
        ///TODO: handle the rest of the cleanup. Delete opponent, set monster in room to false, etc.
        xp += target->xp;
        cout << "You have gained " << target->xp << " experience points." << endl << endl;
        if(xp > next_levelup()) levelup();
        in_melee = false;
    }
}

int Character::saving_throw(Spell* spell){
    return save(spell->save_stat);
}

int Character::spell_attack(){
    return attribute_chk(casting_stat)+prof;
}

int Character::spell_save_DC(){
    return 8+prof+attribute_mods[casting_stat];
}



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

///**************************************************************************************///
///**********************************  MISCELLANIA  *************************************///
///**************************************************************************************///

void Character::equip(Object* o, bool equip_to_offhand){
    if(o->is_two_handed()){
        if(main_hand == NULL && off_hand == NULL){
            cout << o->name() << " equipped." << endl;
            main_hand = o;
            off_hand = o;
            return;
        }
        else {
            cout << "Unequip the ";
            if(main_hand)
                cout << main_hand->name();
            if(main_hand && off_hand && !main_hand->is_two_handed())
                cout << " and the ";
            if(off_hand && !off_hand->is_two_handed())
                cout << off_hand->name();
            cout << "? [y/n]" << endl;

            string input;
            getline(cin, input);
            if(input == "y") {
                cout << o->name() << " equipped." << endl;
                main_hand = o;
                off_hand = o;
            }
            cout << endl;
            return;
        }

    //One-handed
    } else {
        if(!off_hand && equip_to_offhand){
            cout << o->name() << " equipped (off hand)." << endl;
            off_hand = o;
            return;
        }
        else if(!main_hand){
            cout << o->name() << " equipped (main hand)." << endl;
            main_hand = o;
            return;
        }
        else if(main_hand && !off_hand) {
            cout << o->name() << " and " << main_hand->name() << " equipped (main hand)." << endl;
            cout << "Which in main hand? " << endl;
            string input;
            getline(cin, input);
            while(true){
                if(input == o->name()){
                    off_hand = main_hand;
                    main_hand = o;
                    return;
                } else if(input == main_hand->name()){
                    off_hand = o;
                    return;
                } else {
                    cout << "Invalid entry, try again." << endl;
                }
            }
        }
        //TODO better wording here
        else {
            cout << "You must unequip something first. The ";
            if(main_hand)
                cout << main_hand->name();
            if(main_hand && off_hand && !main_hand->is_two_handed())
                cout << ", the ";
            if(off_hand && !off_hand->is_two_handed())
                cout << off_hand->name();
            cout << ", or cancel?" << endl;

            string input;
            cout << endl << ">> ";
            getline(cin, input);
            while(true){
                if(input == main_hand->name()){
                    cout << main_hand->name() << " unequipped, and " << o->name() << " equipped." << endl << endl;
                    if(main_hand->is_two_handed())
                        off_hand = NULL;
                    main_hand = o;
                    if(o->is_two_handed())
                        off_hand = o;
                    return;
                } else if(input == off_hand->name()){
                    cout << off_hand->name() << " unequipped, and " << o->name() << " equipped." << endl << endl;
                    off_hand = o;
                    return;
                } else if(input == "cancel"){
                    cout << endl;
                    return;
                } else {
                    cout << "Invalid entry, try again." << endl;
                }
            }
        }
    }
}

int Character::distance_to(Character* target){
    return sqrt(pow(xPos-target->xPos, 2)+pow(yPos-target->yPos, 2));
}

bool Character::within_range(Character* target, Spell* spell){
    if(distance_to(target) <= spell->range) return true;
    else return false;
}

void Character::heal(int hp){
    if(cur_hp == max_hp){
        cout << "You are already at full health." << endl << endl;
    } else if(max_hp - cur_hp >= hp){
        cur_hp += hp;
        cout << "You have regained " << hp << " hitpoints, bringing your total to " << cur_hp << "/" << max_hp << "." << endl << endl;
    } else {
        cur_hp = max_hp;
        cout << "Your hitpoints have been fully restored to their maximum value (" << cur_hp << "/" << max_hp << ")." << endl << endl;
    }
}

void Character::levelup(){
    level++;
    cout << "You have reached level " << level << "!" << endl;
    cout << "Wow, didn't expect you to make it this far -- I haven't actually programmed level 2 yet, but stay tuned =]" << endl << endl;
}

void Character::special_action(){
    //sanity check -- this function is just for PCs
    if(is_monster) return;

    cout << "Enter one of the following special action options, or cancel to return to the previous menu." << endl;
    if(second_wind_available){
        cout << "\t- secondwind [bonus action]: regain 1d10+" << level << " hitpoints as a bonus action" << endl;
    }

    cout << endl;
    string input;
    do {
        cout << ">> ";
        getline(cin, input);

        if(input == "secondwind"){
            if(!second_wind_available){
                cout << "You have not taken a short or long rest since you last used this feature. Please make another selection." << endl;
                continue;
            }
            second_wind_available = false;
            if(cur_hp == max_hp) cout << "You are already at full health!" << endl;
            else {
                heal(d10()+level);
                cout << "You may not reuse this feature until you complete a short or long rest." << endl << endl;
            }
            return;
        } else if(input == "cancel"){
            return;
        } else {
            cout << "Input not recognized. Try again?" << endl;
        }
    } while (true);

}

void Character::short_rest(){
    //sanity check -- this function is just for PCs
    if(is_monster) return;

    bool changed = false;
    if(second_wind_available == false){
        second_wind_available = true;
        cout << "You have regained the use of your Second Wind class feature." << endl;
    }
    if(hit_dice > 0){
        if(changed) cout << "Additionally, you"; else cout << "You";
        cout << " may spend any number of your remaining hit dice; each will restore 1d" << hit_die_size << "+" << attribute_mods["CON"] << " hp." << endl;
        cout << " You have " << hit_dice << " hit ";
        if(hit_dice == 1) cout << "die "; else cout << "dice ";
        cout << "remaining. How many would you like to spend?" << endl;

        int num;
        while(true){
            cout << ">> ";
            string tmp;
            getline(cin, tmp);
            num = atoi(tmp.c_str());
            if(num > hit_dice)
                cout << "You do not have that many hit dice remaining. Please re-enter a number." << endl;
            else
                break;
        }
        int restored_hp = 0;
        for(int i = 0; i < num; i++){
            restored_hp += rand()%hit_die_size+1+attribute_mods["CON"];
            hit_dice--;
        }

        if(restored_hp > 0){
            if(max_hp - cur_hp >= restored_hp){
                cout << "You have regained " << restored_hp << " hitpoints, bringing your total to " << cur_hp << "/" << max_hp << "." << endl;
                cout << "You have ";
                if(hit_dice == 0) cout << "no"; else cout << hit_dice;
                cout << " hit ";
                if(hit_dice == 1) cout << "die"; else cout << "dice";
                cout << " remaining." << endl << endl;
            } else {
                cur_hp = max_hp;
                cout << "You have fully restored your hitpoints to their maximum value (" << cur_hp << "/" << max_hp << ")." << endl;
                cout << "You have ";
                if(hit_dice == 0) cout << "no"; else cout << hit_dice;
                cout << " hit ";
                if(hit_dice == 1) cout << "die"; else cout << "dice";
                cout << " remaining." << endl << endl;
            }
        }

    } else cout << "You have no remaining hit dice to spend." << endl << endl;
}

int Character::next_levelup(){
    switch(level){
        case 1: return 300;
        case 2: return 900;
        case 3: return 2700;
        case 4: return 6500;
        case 5: return 14000;
        case 6: return 23000;
        case 7: return 34000;
        case 8: return 48000;
        case 9: return 64000;
        case 10: return 85000;
        case 11: return 100000;
        case 12: return 120000;
        case 13: return 140000;
        case 14: return 165000;
        case 15: return 195000;
        case 16: return 225000;
        case 17: return 265000;
        case 18: return 305000;
        case 19: return 355000;
        default: return INT_MAX;
    }
}
