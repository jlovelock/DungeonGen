
#include <game.h>
using namespace std;

Game::Game(){
    PC = new Character();
    dungeon = new Dungeon();
    cur_room = dungeon->starting_room();
    cp = 0; sp = 0; ep = 0; gp = 0; pp = 0;
}

Game::~Game(){
    loot.clear();
    delete dungeon;
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

    //combat
    } else if(contains(input, "attack") || contains(input, "fight")) {
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
        if(!searching(input)) return true;
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
        dungeon->edit_preferences();
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
        cur_room->get_active_monster_char()->generic_attack(PC);
    }

    if(!PC->is_alive()) return false;
    return true;
}




void Game::rest(){
    if(cur_room->has_monsters())
        cout << "You can't rest when there's a " << cur_room->get_active_monster() << " in the room!" << endl;
    else {
        PC->short_rest();
        identify_items();
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
    if(contains(input, "with")){
        bool found = false, equipped = false;
        for(vector<Weapon*>::iterator it = PC->weapons.begin(); it != PC->weapons.end(); ++it){
            if(contains(input, (*it)->name())){
                found = true;
                equipped = PC->equip(*it);
                break;
            }
        }
        if(!found) cout << "That's not a weapon you can attack with." << endl << endl;
        if(!equipped) return false;
    }

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
        if(cur_room->monsters[0]->search_monster(true)){
            rollIndividualTreasure(cur_room->monsters[0]->get_name());
            return true;
        }
        return false;

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
            if(cur_room->monsters[i]->search_monster(false)){
                rollIndividualTreasure(cur_room->monsters[0]->get_name());
                found_small = true;
            }
        }

        //search for treasure
        if((cur_room->has_treasure() || dungeon->treasure_enabled() == "ALWAYS")
           && !(dungeon->treasure_enabled() == "NEVER")
           && PC->skill_check("INVESTIGATION") > FIND_TREASURE_DC){

            cout << "You find a treasure hoard!" << endl;
            cur_room->loot_room();
            rollTreasureHoard();
            found = true;
        }

        cur_room->search_for_secret_doors(PC, found);

        if(!found){
            cout << "You find nothing ";
            if(found_small) cout << "else ";
            cout << "of interest." << endl << endl;
        }
    }
    return true;
}



void Game::drink_potion(int index){
    potions.at(index)->quantity--;
    if(!PC->is_injured())
        cout << "Nothing appears to happen." << endl << endl;
    else
        PC->heal(potions.at(index)->healing_amount());

    if(potions.at(index)->quantity == 0){
        delete potions.at(index);
        potions.erase(potions.begin()+index);
    }
}

//returns true iff a spell was cast
bool Game::cast_spell(){
    if(scrolls.empty()){
        cout << "You have no scrolls from which to cast." << endl << endl;
        return false;
    }
    if(!cur_room->has_monsters()){
        cout << "There are no enemies in the room to cast spells at." << endl << endl;
        return false;
    }

    bool flag = true;
    for(vector<Treasure*>::iterator it = scrolls.begin(); it != scrolls.end(); ++it){
        char idx = 'A';
        if((*it)->identified){
            if(flag){
                cout << "Choose which scroll you would like to cast, or enter 'cancel' to exit this menu." << endl;
                flag = false;
            }
            cout << "\t- " << (*it)->get_description() << endl;
            ++idx;
        }
    }
    if(flag){
        cout << "You can't cast a scroll you haven't identified. You can attempt to identify them while you rest." << endl;
        return false;
    }

    cout << endl;
    string input;
    read(input);

    for(vector<Treasure*>::iterator it = scrolls.begin(); it != scrolls.end(); ++it){
        if((*it)->identified && (contains(input, (*it)->get_description()) || contains((*it)->get_description(), input))){
            PC->cast((*it)->spell, cur_room->get_active_monster_char());
            (*it)->quantity--;
            if((*it)->quantity == 0){
                delete *it;
                scrolls.erase(it);
            }
            return true;
        }
    }
    return false;
}



//returns true iff you drank a potion
bool Game::drink_potion(){
    if(potions.empty()){
        cout << "You have no potions to drink!" << endl;
        return false;
    }
    string input;
    if(potions.size() == 1){
        if(potions.front()->identified) cout << "Drink the " << potions.front()->name << " potion? [y/n] (" << potions.front()->quantity << " in inventory)" << endl;
        else cout << "Drink the unidentified " << potions.front()->description << " potion? [y/n] (" << potions.front()->quantity << " in inventory)" << endl;


        read(input);
        if(input == "y"){
            drink_potion(0);
            return true;
        } else {
            cout << "Alright, what do you want to do instead?" << endl;
            return false;
        }

    } else {
        char inputChar;
        do {
            cout << "Which potion?" << endl;
            char idx = 'A';
            for(vector<Treasure*>::iterator it = potions.begin(); it != potions.end(); ++it){
                cout << "\t(" << idx << "): ";
                if((*it)->identified) cout << (*it)->name << " potion" << endl;
                else cout << "unidentified " << (*it)->description << " potion" << endl;
                idx++;
            }
            cout << endl;
            cin.get(inputChar);
        } while((unsigned)(inputChar - 'A') > potions.size());

        drink_potion(inputChar-'A');
        return true;
    }
}
