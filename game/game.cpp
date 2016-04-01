
#include <game.h>
#include <weapon.h>
#include <pc.h>
#include <fstream>
#include <string>
using namespace std;

ofstream unrecognized_input;

Game::Game(){
    PC = new PlayerChar(1);
    dungeon = new Dungeon();
    cur_room = dungeon->starting_room();

// ------------ Testing ---------------
    for(int lvl = 0; lvl <= 2; lvl++){
        for(int i = 0; i < 20; i++){
            Scroll* s = new Scroll(lvl, 2);
            s->identify();
            PC->inventory->add(s);
        }
    }

    unrecognized_input.open("utils/input_log.txt", ifstream::app);
    if(!unrecognized_input.is_open()) {
        cout << "Could not open input log file. Exiting." << endl;
        exit(EXIT_FAILURE);
    }
}

Game::~Game(){
    delete dungeon;
    delete PC;
    unrecognized_input.close();
}

void Game::run() {
    cur_room->printFullDescription(0);
    dungeon->print_map(PC->get_position());
    while (cur_room && getCommand()){
        dungeon->print_map(PC->get_position());
        dungeon->check_completion();
    }
    cout << "Restarting with a new dungeon..." << endl;
}

bool Game::getCommand() {
    string input;
    read(input);
    cout << endl;
    bool new_room=false;
    if(contains(input, "new") || contains(input, "restart") || contains(input, "reset")){
        cout << "Reset this dungeon and start a new game? [y/n]" << endl;
        read(input);
        if(input == "y") return false;
        else return true;
    } else if(input == "special"){
        PC->special_action();
        return true;
    } else if (input == "rest"){
        rest();
        return true;
    } else if(contains(input, "look")|| contains(input, "examine") || contains(input,"inspect")){
        look(input);
        return true;
    } else if(input == "status"){
        PC->print_status();
        return true;
    } else if(contains(input, "drop")){
        drop_item(input);
        return true;
    } else if(contains(input, "pick up") || contains(input, "grab")){
        pick_up_item(input);
        return true;

    //combat
    } else if(contains(input, "attack") || contains(input, "fight") || contains(input, "shoot") || contains(input, "stab")) {
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

    } else if(contains(input, "kill") || contains(input, "murder")){
        cout << "That's a bit presumptuous, but you can certainly try!" << endl;
        if(!combat(input)) return true;
    } else if(contains(input,"search") || contains(input, "loot")){
        if(!searching(input)) return true;
    } else if(input == "inventory"){
        PC->print_inventory();
        return true;
    } else if(contains(input, "drink") || contains(input, "potion")){
        if(!drink_potion(input)) return true;
    } else if(contains(input, "identify") || contains(input, "inspect") || contains(input, "examine")){
        cout << "Inspecting things in that much detail takes a while. Maybe you should rest for a bit?" << endl << endl;
        return true;
    } else if(contains(input, "cast") || contains(input, "read") || contains(input, "scroll")){
        if(!cast_spell(input)) return true;

    } else if(contains(input, "settings") || contains(input, "preferences")){
        dungeon->edit_preferences();
        return true;

    } else if(contains(input, "equip")){
        equip_item(input);
        return true;

    } else {
        parse_open_door(input);
        new_room = true;
    }

    PC->end_turn();
    check_encumbrance();

    //moneter's turn to attack
    if(cur_room->has_monsters() && !new_room){
    ///TODO robustness here
    ///TODO FIXME multiple monster support
        Character* m = cur_room->get_active_monster_char();
        m->start_turn();
        if(m->is("paralyzed")){
            cout << "The " << m->full_name() << " tries to attack, you, but can't move!" << endl;
        } else {
            cout << "The " << m->full_name() << " turns to attack you!" << endl;
            m->generic_attack(PC);
        }

        if(m->is_alive())
            m->end_turn();
    }

    if(!PC->is_alive()) return false;
    PC->start_turn();
    return true;
}

///@TODO: be able to drop / pick up coins
void Game::pick_up_item(string input){
    Object* o = cur_room->pick_up_item(input);
    if(!o) {
        cout << "You don't see that on the floor." << endl;
        return;
    }

    cout << o->get_quantity() << " " << o->get_description() << " added to inventory." << endl << endl;
    PC->inventory->add(o);
}

///@TODO: be able to drop coins
///@TODO: if PC has an item equipped, dropping it should unequip
void Game::drop_item(string input){
    /* Find item */
    Object* o = PC->inventory->get_item(input);
    if(!o){
        cout << "Sorry, which item did you want to drop?" << endl;
        read(input);
        o = PC->inventory->get_item(input);
    }
    if(!o) {
        cout << "You aren't carrying that item." << endl;
        return;
    }

    /* Figure out how many to drop */
    int num = NOT_FOUND;
    if(o->get_quantity() > 1){
        if(contains(input, "all")) num = o->get_quantity();
        else num = extract_num_from_string(input);
        while(num == NOT_FOUND){
            cout << "How many? (" << o->get_quantity() << " in inventory)" << endl;
            read(input);
            if(contains(input, "all"))
                num = o->get_quantity();
            else
                num = extract_num_from_string(input);
        }
    } else {
        num = 1;
    }

    /* Unequip if need be */
    ///@TODO: fix minor bug here when dual wielding two different weapons
    int num_remaining = o->get_quantity() - num;
    if(num_remaining == 0){
        while(o->is_equipped_to(PC)) PC->unequip(o);
    } else if(num_remaining == 1){
        if(PC->equipped_weapon_type() == "dual" && o->is_equipped_to(PC))
            PC->unequip(o);
    }

    /* Drop it! */
    Object* dropped = PC->inventory->remove(o, num);
    cout << dropped->get_quantity() << " " << dropped->get_description() << " dropped." << endl << endl;
    cur_room->drop_item(dropped);
}

void Game::check_encumbrance(){
    bool looped = false;
    string input;

    while(PC->is_overencumbered()){
        if(!looped){
            cout << "You are overencumbered [" << PC->inventory->weight() << " of " << PC->attribute_score("STR")*15 << " lbs]! Inventory:" << endl;
            PC->print_inventory();
            looped = true;
        } else {
            cout << "You are still overencumbered [" << PC->inventory->weight() << " of " << PC->attribute_score("STR")*15 << " lbs]!" << endl;
        }
        cout << "Which item to drop?" << endl;
        read(input);
        drop_item(input);
    }
}


void Game::rest(){
    if(cur_room->has_monsters())
        cout << "You can't rest when there's a " << cur_room->get_active_monster() << " in the room!" << endl;
    else {
        PC->short_rest();
        PC->identify_items();
    }
}


///COME BACK HERE AND FIX THIS YOU DOLT
void Game::look(string input){
    //room description
    if(contains(input, "room") || contains(input, "around") || input == "look"){
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
bool Game::combat(string input)
{
    // attack <monster> with <weapon>
    // find and equip that weapon first
    if(contains(input, "with"))
        equip_item(input);

    ///@TODO multi-monster support
    ///@TODO add more options here
    if(contains(input, cur_room->get_active_monster()) || cur_room->num_active_monsters() == 1){
        PC->generic_attack(cur_room->get_active_monster_char());
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

    ///@TODO multi-monster support
    } else if(cur_room->monsters[0]){
        cout << "Uh, he's already dead, but go right ahead if you really want to." << endl << endl;
        return false;

    } else {
        cout << "That's not something you can attack." << endl;
        return false;
    }
}

//returns false iff no action was taken
bool Game::searching(string input){
    //search a single monster
    ///TODO multi-monster support
    if(contains(input, cur_room->get_monster()) || input == "loot"){
        return PC->search_monster(cur_room->monsters[0], true);

    //search the whole room
    // Default option (just "search" will go here")
    } else if(contains(input, "room") || contains(input, "area") || input == "search"){
        if(cur_room->has_monsters()){
            cout << "You can't do that with the " << cur_room->get_active_monster() << " around!" << endl;
            return false;
        }

        bool found = false, found_small = false;

        //search monster corpses
        for(int i = 0; i < MAX_MONSTERS && cur_room->monsters[i] != NULL; i++){
            if(PC->search_monster(cur_room->monsters[0], false))
                found_small = true;
        }

        cur_room->loot(PC, found, found_small);
        cur_room->search_for_secret_doors(PC, found);

        if(!found){
            cout << "You find nothing ";
            if(found_small) cout << "else ";
            cout << "of interest." << endl << endl;
        }
    }
    return true;
}


