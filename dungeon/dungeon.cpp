
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

bool Dungeon::useDoor(Door* d) {

    string input;
    if(d->locked){
        cout << "That door is locked. Try to lockpick, ram, or leave? ";
        read(input);
        if (input == "lockpick") {
            if(PC.skill_check("THIEVES TOOLS") > LOCKPICK_DC){
                d->locked = false;
                cout << "You successfully pick the lock." << endl << endl;
            } else {
                cout << "You fail to pick the lock." << endl << endl;
                return false;
            }
        } else if(input == "ram"){
            if(PC.attribute_chk("STRENGTH") > d->break_DC()){
                cout << "You successfully break down the door." << endl << endl;
                d->material = "broken " + d->material;
                d->locked = false;
            } else {
                cout << "You fail to break down the door." << endl << endl;
                return false;
            }
        }
        else {
            cout << endl;
            return false;
        }
    } else if(d->barred && (d->barredSide xor (cur_room->id != d->first->id))) {
        cout << "You remove the bar on the door and head through." << endl << endl;
        d->barred = false;
    } else if(d->barred) {
        cout << "Something is blocking it from opening. Try to ram, or leave? ";
        read(input);
        if(input == "ram"){
            if(PC.skill_check("ATHLETICS") > d->break_DC()){
                cout << "You successfully break down the door." << endl << endl;
                d->material = "broken " + d->material;
                d->barred = false;
            } else {
                cout << "You fail to break down the door." << endl << endl;
                return false;
            }
        } else return false;

    } else if(d->main_exit) {
        cout << "This leads outside? Leave the dungeon? [y/n] ";
        read(input);
        if(input == "y"){
            cout << "You have left the dungeon!" << endl;
            in_dungeon = false;
        }
        return false;
    }

    int door_num = 0;

    //revisiting a room that is already initialized
    if(d->second != NULL){
        cout << "You turn back to the ";
        if(cur_room->isFirstRoom(d)){
            cout << d->second->purpose_short << "." << endl;
            for(door_num = 0; door_num < MAX_DOORS; door_num++){
                if(d->second->doors[door_num]->first->id == cur_room->id){
                    break;
                }
            }
            cur_room = d->second;

        } else {
            cout << d->first->purpose_short << "." << endl;
            for(door_num = 0; door_num < MAX_DOORS; door_num++){
                if(d->first->doors[door_num]->second && d->first->doors[door_num]->second->id == cur_room->id){
                    break;
                }
            }

            cur_room = d->first;
        }
        cur_room->printDescription(door_num);


    //creating (and moving to) a new room
    } else {
        add_room(d, d->material == "passage");
        cur_room->printFullDescription(door_num);
    }

    return true;
}




bool Dungeon::parse_open_door(string input){
    if(input != "north" && input != "east" && input != "south" && input != "west"){
        cout << "Unrecognized command" << endl;
        return false;
    }
    if(cur_room->has_monsters()){
        cout << "The " << cur_room->get_active_monster() << " is blocking your exit." << endl << endl;
        return false;
    }

    int doorCopies = 0, doorNum = 0;
    for(int i = 0; i < MAX_DOORS && cur_room->doors[i] != NULL; i++){
        if(cur_room->doors[i]->getWallString(cur_room) == input && !cur_room->doors[i]->secret){
            doorNum = i;
            doorCopies++;
        }
    }
    if(doorCopies == 1)
        return useDoor(cur_room->doors[doorNum]);

    else if(doorCopies > 1) {

        cout << "Which instance?" << endl;
        int instance = 0;
        for(int i = 0; i < MAX_DOORS && cur_room->doors[i] != NULL; i++){
            if(cur_room->doors[i]->getWallString(cur_room) == input && !cur_room->doors[i]->secret){
                cout << "\t- " << instance++;

                //print description if it leads to a room you've visited
                if(cur_room->doors[i]->second != NULL){
                    cout << " (leading back to the ";
                    if(cur_room->isFirstRoom(cur_room->doors[i]))
                        cout << cur_room->doors[i]->second->purpose_short;
                    else
                        cout << cur_room->doors[i]->first->purpose_short;
                    cout << ")";
                } else {
                    cout << " (leading somewhere new)";
                }
                cout << endl;
            }
        }
        cout << endl << ">> ";

        int doorIndex;
        string num;
        read(num);
        doorIndex = atoi(num.c_str());

        for(int i = 0; i < MAX_DOORS && cur_room->doors[i] != NULL; i++){
            if(cur_room->doors[i]->getWallString(cur_room) == input && cur_room->doors[i]->secret == false){
                if(doorIndex == 0) return useDoor(cur_room->doors[i]);
                else doorIndex--;
            }
        }
    }

    cout << "There's no door along that wall." << endl;
    return false;

}

bool Dungeon::parse_open_door(){
    string input;
    read(input);
    return parse_open_door(input);

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
    cout << ">> ";
    string input;
    //getline(cin, input);
    read(input);
    cout << endl;
    if(input == "exit"){
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
    cout << "Thanks for playing!" << endl;
}
