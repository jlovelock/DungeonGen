
#include <string>
#include <iostream>
#include <dungeon.h>

using namespace std;

Dungeon::Dungeon(){
    cur_id = 1;
    cur_room = new Room();
    last_room = cur_room;
    first_room = cur_room;
    in_dungeon = true;
    cp = 0; sp = 0; ep = 0; gp = 0; pp = 0;
    completed_known = false;
    read_preferences();
}

Dungeon::~Dungeon(){
    loot.clear();
    Room* rm = cur_room = first_room;
    Room* tmp;
    for(int i = 0; i < cur_id; i++){
        tmp = rm;
        rm = rm->next;
        delete tmp;
    }
}

void Dungeon::read_preferences(){
    ifstream sfile("save/settings.conf");

	char c;

    // Dungeon type: STRONGHOLD[0]
    sfile.get(c);
    switch(c){
        case '0':
            dungeon_type = "STRONGHOLD";
            break;
        default:
            cout << "Corrupt settings file detected [dtype=" << c << "]. Exiting." << endl;
            exit(EXIT_FAILURE);
    }

    // Monsters enabled: false[0], true[1]
    sfile.get(c);
    switch(c){
        case '0':
            monsters_enabled = false;
            break;
        case '1':
            monsters_enabled = true;
            break;
        default:
            cout << "Corrupt settings file detected [m_ena=" << c << "]. Exiting." << endl;
            exit(EXIT_FAILURE);
    }

    //treasure_enabled: RANDOM[0], ALWAYS[1], or NEVER[2]
    sfile.get(c);
    switch(c){
        case '0':
            treasure_enabled = "RANDOM";
            break;
        case '1':
            treasure_enabled = "ALWAYS";
            break;
        case '2':
            treasure_enabled = "NEVER";
            break;
        default:
            cout << "Corrupt settings file detected [t_ena=" << c << "]. Exiting." << endl;
            exit(EXIT_FAILURE);
    }

	sfile.close();
}

void Dungeon::write_preferences(){
    ofstream sfile("save/settings.conf");

    // Dungeon type: STRONGHOLD[0]
    if(dungeon_type == "STRONGHOLD"){
        sfile.put('0');
    }

    // Monsters enabled: false[0], true[1]
    if(monsters_enabled)
        sfile.put('0');
    else
        sfile.put('1');

    //treasure_enabled: RANDOM[0], ALWAYS[1], or NEVER[2]
    if(treasure_enabled == "RANDOM")
        sfile.put('0');
    else if(treasure_enabled == "ALWAYS")
        sfile.put('1');
    else
        sfile.put('2');

    sfile.close();
}


void Dungeon::preferences(){
    cout << "Your current settings:" << endl;
    cout << "\tDungeon type: " << dungeon_type << endl;
    cout << "\tMonster spawning: " << (monsters_enabled ? "ON" : "OFF") << endl;
    cout << "\tTreasure hoards: " << treasure_enabled << endl << endl;
    cout << "Enter the setting you'd like to change, or 'cancel' to go back to the previous menu." << endl;
    string input;
    read(input);

    if(contains(input, "dungeon") || contains(input, "type")){
        cout << "Sorry, only the STRONGHOLD dungeon type is supported at this time." << endl;
        return;
    } else if(contains(input, "monster") || contains(input, "spawn")){
        cout << "Monsters ON or OFF? " << endl;
        read(input);
        if(contains(input, "off")){
            monsters_enabled = false;
            cout << "Monster spawning set to OFF." << endl;
        } else if(contains(input, "on")){
            monsters_enabled = true;
            cout << "Monster spawning set to ON." << endl;
        } else {
            cout << "Error, unrecognized input." << endl;
        }
        return;
    } else if(contains(input, "treasure") || contains(input, "hoard")){
        cout << "Treasure hoard spawning: RANDOM, ALWAYS, or NEVER?" << endl;
        read(input);
        if(contains(input, "random")){
            treasure_enabled = "RANDOM";
            cout << "Treasure hoards will spawn randomly." << endl;
        } else if(contains(input, "always")){
            treasure_enabled = "ALWAYS";
            cout << "There will be a treasure hoard in every room." << endl;
        } else if(contains(input, "never")){
            treasure_enabled = "NEVER";
            cout << "Treasure hoards will never spawn." << endl;
        } else {
            cout << "Input not recognized." << endl;
        }
    }
    write_preferences();
}

void Dungeon::check_completion(){

    if(completed_known) return;
    for(Room* r = first_room; r != NULL; r = r->next){
        for(int i = 0; i < MAX_DOORS && r->doors[i] != NULL; i++){
            if(r->doors[i]->main_exit) continue;
            if(r->doors[i]->second == NULL) return;
        }
    }
    cout << endl << "   *** ALERT: All rooms of this dungeon have been generated! ***" << endl << endl;
    completed_known = true;

}


void Dungeon::rest(){
    if(cur_room->has_monsters())
        cout << "You can't rest when there's a " << cur_room->get_active_monster() << " in the room!" << endl;
    else {
        PC.short_rest();
        identify_items();
    }
}

///COME BACK HERE AND FIX THIS YOU DOLT
void Dungeon::look(string input){
    //room description
    if(contains(input, "room") || contains(input, "around")){
        cur_room->printFullDescription();

    //looking at a monster
    ///TODO add multi-monster support
    ///TODO FIXME since changing how input worked
    } else if(contains(input,cur_room->get_monster())) {
        if(cur_room->has_monsters())
           cout << "He looks like a bad guy. You should kill him. What did you expect?" << endl << endl;
        else
            cout << "He looks like a bad guy. A dead bad guy. Good thing you killed him." << endl << endl;

    //looking at your weapons
    ///TODO fixme
//    } else if(input == PC.melee_weapon || input == PC.ranged_weapon){
//        cout << "It's a little rusty, but it's served you well over the years." << endl << endl;

    } else {
        ///TODO check if it's an item in inventory, give description on it
        cout << "I get that you want to look at something, but I'm not sure what... sorry. Try another command?" << endl << endl;
    }
}

//returns true iff fighting happened
bool Dungeon::combat(string input)
{
    ///@TODO multi-monster support
    ///@TODO add more options here
    if(contains(input, cur_room->get_active_monster())){
        PC.generic_attack(cur_room->get_active_monster_char());
        return true;

//        if(is_ranged){
//            if(PC.in_melee){
//                ///TODO FIXME multi-monster support (make sure the right monster(s) get(s) the AOO)
//                ///add Character* in_melee_with to the Character class? (maybe even an array?)
//                cout << "As you step away from the " << cur_room->get_active_monster() << ", it turns to attack you!" << endl;
//                cur_room->get_monster(input)->melee_attack(&PC);
//            }
//            PC.ranged_attack(cur_room->get_monster(input));
//        } else {
//            PC.melee_attack(cur_room->get_monster(input));
//        }
//        return true;
    } else {
        cout << "That's not something you can attack." << endl;
        return false;
    }
}
//
////returns true iff fighting happened
//bool Dungeon::combat(){
//    string monster_name;
//    cin >> monster_name;
//
//
////    cout << "Melee or ranged?" << endl;
////    cout << ">> ";
////    string input;
////    cin >> input;
////    cout << endl;
////    if(input == "ranged")
//        return combat(monster_name, true);
////    else
////        return combat(monster_name, false);
//
//}

//returns false iff no action was taken
bool Dungeon::searching(string input){
    //search a single monster
    ///TODO multi-monster support
    if(contains(input, cur_room->get_monster())){
        if(cur_room->has_monsters()){
            cout << "You can't do that while it's still alive!" << endl;
            return false;
        } else if (cur_room->monsters[0]->searched){
            cout << "You've already done that." << endl;
            return false;
        } else {
            cur_room->monsters[0]->searched = true;
            rollIndividualTreasure(cur_room->monsters[0]->PC_class);
            return true;
        }

    //search the whole room
    } else if(contains(input, "room") || contains(input, "area")){
        if(cur_room->has_monsters()){
            cout << "You can't do that with the " << cur_room->get_active_monster() << " around!" << endl;
            return false;
        }

        bool found = false, found_small = false;

        //search monster corpses
        for(int i = 0; i < MAX_MONSTERS; i++){
            if(cur_room->monsters[i] != NULL && cur_room->monsters[i]->is_alive() == false && cur_room->monsters[i]->searched == false){
                cur_room->monsters[0]->searched = true;
                rollIndividualTreasure(cur_room->monsters[0]->PC_class);
                found_small = true;
            }
        }

        //search for treasure
        if(cur_room->has_treasure && PC.skill_check("INVESTIGATION") > FIND_TREASURE_DC){
            cout << "You find a treasure hoard!" << endl;
            cur_room->has_treasure = false;
            rollTreasureHoard();
            found = true;
        }

        //search for secret doors
        for(int i = 0; i < MAX_DOORS; i++){
            if(cur_room->doors[i] != NULL && cur_room->doors[i]->secret && PC.skill_check("INVESTIGATION") > SECRET_DOOR_DC){
                cur_room->doors[i]->secret = false;
                if(found) cout << "You also find a secret door along the ";
                else cout << "You find a secret door along the ";
                cout << cur_room->doors[i]->getWall(cur_room) << " wall." << endl << endl;
                found = true;
            }
        }
        if(!found){
            cout << "You find nothing ";
            if(found_small) cout << "else ";
            cout << "of interest." << endl << endl;
        }
    }
    return true;
}



bool Dungeon::getCommand() {
    string input;
    //getline(cin, input);
    read(input);
    cout << endl;
    if(contains(input, "new") || contains(input, "restart")){
        return false;
    } else if(input == "special"){
        PC.special_action();
        return true;
    } else if (input == "rest"){
        rest();
        return true;
    } else if(contains(input, "look")|| contains(input, "examine") || contains(input,"inspect")){
        look(input);
        return true;
    } else if(input == "status"){
        PC.print_status();
        return true;

    //combat
    } else if(contains(input, "attack")) {
        if(!combat(input)) return true;
//    } else if(input == "melee"){
//        cin >> input; //get monster name
//        if(input == "attack") cin >> input;
//        if(!combat(input, false)) return true;
//    } else if(input == "ranged"){
//        cin >> input; //get monster name
//        if(input == "attack") cin >> input;
//        if(!combat(input, true)) return true;
//    } else if(input == "attack" || input == "fight"){
//        if(!combat()) return true;

    } else if(contains(input,"search") || contains(input, "loot")){
        searching(input);
    } else if(input == "inventory"){
        print_inventory();
        return true;
    } else if(contains(input, "drink") || contains(input, "potion")){
        if(!drink_potion()) return true;

    } else if(contains(input, "identify")){
        if(loot.empty() && potions.empty()) cout << "Sorry, not sure what you're referring to there." << endl << endl;
        else cout << "Inspecting things in that much detail takes a while. Maybe you should rest for a bit?" << endl << endl;
        return true;
    } else if(contains(input, "cast") || contains(input, "read")){
        if(!cast_spell()) return true;

    } else if(contains(input, "settings") || contains(input, "preferences")){
        preferences();
        return true;

    } else if(contains(input, "equip")){
        equip_item(input);
        return true;

    //switch rooms
//    } else if(input == "open" || input == "go"){
//        parse_open_door();
//        return true;
    } else {
        parse_open_door(input);
        return true;
    }

    //moneter's turn to attack
    if(cur_room->has_monsters()){
    ///TODO robustness here
    ///TODO FIXME multiple monster support
        cout << "The " << cur_room->get_active_monster() << " turns to attack you!" << endl;
        cur_room->get_active_monster_char()->generic_attack(&PC);
    }

    if(!PC.is_alive()) return false;
    return true;
}

void Dungeon::run() {
    cur_room->printFullDescription(0);
    print_map();
    while (in_dungeon && getCommand()){
        print_map();
        check_completion();
    }
    cout << "Restarting with a new dungeon..." << endl;
}
