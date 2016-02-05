#include <dungeon.h>
#include <string>
#include <iostream>
#include <spells.h>
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

///TODO clean up this function with subroutines, get rid of repetitiveness
void Dungeon::adjust_room_position(Room* rm){

    for(Room* compare = first_room; compare != NULL; compare = compare->next){

        int offset = 0, shrink_amt = 0;

        //south
        while(compare->withinY(rm->southEdge+offset)){
            offset++;
        }
        if(offset != 0){
            rm->shift(offset, false);
            rm->linkDoors(compare, SOUTH);
        }

        //north
        while(compare->withinY(rm->northEdge-shrink_amt)){
            shrink_amt++;
        }
        if(shrink_amt != 0){
            rm->shrink(shrink_amt, false);
            rm->linkDoors(compare, NORTH);
        }

        //reset for xDim
        offset = 0;
        shrink_amt = 0;

        //west
        while(compare->withinX(rm->westEdge+offset)){
            offset++;
        }

        if(offset != 0){
            rm->shift(offset, true);
            rm->linkDoors(compare, WEST);
        }

        //east
        while(compare->withinX(rm->eastEdge-shrink_amt)){
            shrink_amt++;
        }

        if(shrink_amt != 0){
            rm->shrink(shrink_amt, true);
            rm->linkDoors(compare, EAST);
        }


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

    if(d->locked){
        cout << "That door is locked. Try to lockpick, ram, or leave? ";
        string input;
        cin >> input;
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
        string input;
        cin >> input;
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
        char input;
        cin >> input;
        if(input == 'y'){
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
        if(d->material == "passage") d->second = new Room(cur_id, d, true);
        else d->second = new Room(cur_id, d, false);
        adjust_room_position(d->second);
        cur_id++;
        last_room->next = d->second;
        last_room = d->second;
        cur_room = d->second;
        cur_room->doors[0] = d;
        cur_room->printInitialDescription(door_num);
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
        if(cur_room->doors[i]->getWall(cur_room) == input){
            if(!cur_room->doors[i]->secret){
                doorNum = i;
                doorCopies++;
            }
        }
    }
    if(doorCopies == 1)
        return useDoor(cur_room->doors[doorNum]);

    else if(doorCopies > 1) {

        cout << "Which instance?" << endl;
        int instance = 0;
        for(int i = 0; i < MAX_DOORS && cur_room->doors[i] != NULL; i++){
            if(cur_room->doors[i]->getWall(cur_room) == input && !cur_room->doors[i]->secret){
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
        cin >> doorIndex;
        for(int i = 0; i < MAX_DOORS && cur_room->doors[i] != NULL; i++){
            if(cur_room->doors[i]->getWall(cur_room) == input && cur_room->doors[i]->secret == false){
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
    cin >> input;
    return parse_open_door(input);

}

///(long-term) TODO: add the full tables here (by CR)
void Dungeon::rollIndividualTreasure(string monster_name){
    int x = d100();
    int reward;
    if(x < 31){
        reward = d6()+d6()+d6()+d6()+d6();
        cout << "The " << monster_name << " was carrying " << reward << " copper pieces." << endl;
        cp += reward;
    } else if(x < 61){
        reward = d6()+d6()+d6()+d6();
        cout << "The " << monster_name << " was carrying " << reward << " silver pieces." << endl;
        sp += reward;
    } else if(x < 71){
        reward = d6()+d6()+d6();
        cout << "The " << monster_name << " was carrying " << reward << " electrum pieces." << endl;
        ep += reward;
    } else if(x < 96){
        reward = d6()+d6()+d6();
        cout << "The " << monster_name << " was carrying " << reward << " gold pieces." << endl;
        gp += reward;
    } else {
        reward = d6();
        cout << "The " << monster_name << " was carrying " << reward << " platinum ";
        if(reward == 1) cout << "piece." << endl;
        else cout << "pieces." << endl;
        pp += reward;
    }
}

void Dungeon::print_inventory(){
    cout << "WEAPONS: " << endl;
    cout << "\t- " << PC.melee_weapon << endl;
    cout << "\t- " << PC.ranged_weapon << endl << endl;

    if(cp > 0 || sp > 0 || ep > 0 || gp > 0 || pp > 0){
        cout << "COINS: " << endl;
        if(cp > 0) cout << "\t- " << cp << "cp" << endl;
        if(sp > 0) cout << "\t- " << sp << "sp" << endl;
        if(ep > 0) cout << "\t- " << ep << "ep" << endl;
        if(gp > 0) cout << "\t- " << gp << "gp" << endl;
        if(pp > 0) cout << "\t- " << pp << "ep" << endl;
        cout << endl;
    }

    if(!potions.empty()){
        cout << "POTIONS:" << endl;
        for(vector<Treasure*>::iterator it = potions.begin(); it != potions.end(); ++it){
            cout << "\t- ";
            //if(!(*it)->identified) cout << "[?] ";
            cout << (*it)->quantity << " " << (*it)->get_description() << "." << endl;
        }
        cout << endl;
    }

    if(!scrolls.empty()){
        cout << "SPELL SCROLLS:" << endl;
        int num_unidentified_scrolls = 0;
        for(vector<Treasure*>::iterator it = scrolls.begin(); it != scrolls.end(); ++it){
            if(!(*it)->identified)
                num_unidentified_scrolls++;
            else
                cout << "\t- " << (*it)->quantity << " " << (*it)->get_description() << "." << endl;
        }
        if(num_unidentified_scrolls > 0){
            cout << "\t- " << num_unidentified_scrolls << " unidentified spell scroll";
            if(num_unidentified_scrolls > 1) cout << "s";
            cout << "." << endl;
        }
        cout << endl;
    }

    if(!loot.empty()){
        cout << "MISC:" << endl;
        for(vector<Treasure*>::iterator it = loot.begin(); it != loot.end(); ++it){
            cout << "\t- ";
            //if(!(*it)->identified) cout << "[?] ";
            cout << (*it)->quantity << " " << (*it)->get_description() << "." << endl;
        }
        cout << endl;
    }
    cout << endl;
}

void Dungeon::rollTreasureHoard(){
    vector<Treasure*> hoard;

    int num_objects, value;
    string type;

    //Roll money
    int copper, silver, gold;
    copper = (d6()+d6()+d6()+d6()+d6()+d6())*100;
    silver = (d6()+d6()+d6())*100;
    gold = (d6()+d6())*10;
    cout << "You find " << copper << "cp, " << silver << "sp, and " << gold << "gp." << endl;
    cp += copper;
    sp += silver;
    gp += gold;

    int num_magic_items;
    char magic_item_table;

    //Roll item category/quantities
    ///TODO add magic items!
    int x = d100();
    if(x < 7) {
        cout << endl;
        return;
    } else if(x < 17){
        type = "gemstones";
        num_objects = d6()+d6();
        value = 10;
    } else if(x < 27){
        type = "art";
        num_objects = d4()+d4();
        value = 25;
    } else if(x < 37){
        type = "gemstones";
        num_objects = d6()+d6();
        value = 50;
    } else if(x < 45){
        type = "gemstones";
        num_objects = d6()+d6();
        value = 10;
        num_magic_items = d6();
        magic_item_table = 'A';
    } else if(x < 53){
        type = "art";
        num_objects = d4()+d4();
        value = 25;
        num_magic_items = d6();
        magic_item_table = 'A';
    } else if(x < 61){
        type = "gemstones";
        num_objects = d6()+d6();
        value = 50;
        num_magic_items = d6();
        magic_item_table = 'A';
    } else if(x < 66){
        type = "gemstones";
        num_objects = d6()+d6();
        value = 10;
    } else if(x < 71) {
        type = "art";
        num_objects = d4()+d4();
        value = 25;
    } else if(x < 76){
        type = "gemstones";
        num_objects = d6()+d6();
        value = 50;
    } else if(x < 79){
        type = "gemstones";
        num_objects = d6()+d6();
        value = 10;
    } else if(x < 81){
        type = "art";
        num_objects = d4()+d4();
        value = 25;
    } else if(x < 86){
        type = "gemstones";
        num_objects = d6()+d6();
        value = 50;
    } else if(x < 93){
        type = "art";
        num_objects = d4()+d4();
        value = 25;
    } else if(x < 98){
        type = "gemstones";
        num_objects = d6()+d6();
        value = 50;
    } else if(x < 100){
        type = "art";
        num_objects = d4()+d4();
        value = 25;
    } else {
        type = "gemstones";
        num_objects = d6()+d6();
        value = 50;
    }

    ///**********REMOVE ME AFTER
    num_magic_items = d6();
    magic_item_table = 'A';



    //Roll each mundane item
    for(int i = 0; i < num_objects; i++){
        Treasure* next = new Treasure (type, 1, value);
        bool found = false;
        for(vector<Treasure*>::iterator it = hoard.begin(); it != hoard.end(); ++it){
            if((*it)->name == next->name){
                delete next;
                found = true;
                (*it)->quantity++;
                break;
            }
        }
        if(!found) hoard.push_back(next);
    }

    //roll each magical item
    for(int i = 0; i < num_magic_items; i++){
        Treasure* next = new Treasure (magic_item_table);
        bool found = false;
        for(vector<Treasure*>::iterator it = hoard.begin(); it != hoard.end(); ++it){
            if((*it)->name == next->name){
                delete next;
                found = true;
                (*it)->quantity++;
                break;
            }
        }
        if(!found) hoard.push_back(next);
    }
    //Print loot table
    cout << "You have also received the following:" << endl;
    for(vector<Treasure*>::iterator it = hoard.begin(); it != hoard.end(); ++it){
        cout << "\t- " << (*it)->quantity << " " << (*it)->get_description() << "." << endl;
    }
    cout << endl << endl;


    //Merge duplicate items from hoard to dungeon loot / potion inventory
    bool flag = false;
    for(vector<Treasure*>::iterator h_it = hoard.begin(); h_it != hoard.end(); ++h_it){
        if(flag){
            flag = false;
            --h_it;
        }
        for(vector<Treasure*>::iterator l_it = loot.begin(); l_it != loot.end(); ++l_it){
            if((*h_it)->name == (*l_it)->name){
                (*l_it)->quantity += (*h_it)->quantity;
                delete *h_it;
                h_it = hoard.erase(h_it);
                flag = true;
                break;
            }
        }
        for(vector<Treasure*>::iterator p_it = potions.begin(); p_it != potions.end(); ++p_it){
            if((*h_it)->name == (*p_it)->name){
                (*p_it)->quantity += (*h_it)->quantity;
                delete *h_it;
                h_it = hoard.erase(h_it);
                flag = true;
                break;
            }
        }
        for(vector<Treasure*>::iterator s_it = scrolls.begin(); s_it != scrolls.end(); ++s_it){
            if((*h_it)->name == (*s_it)->name){
                (*s_it)->quantity += (*h_it)->quantity;
                delete *h_it;
                h_it = hoard.erase(h_it);
                flag = true;
                break;
            }
        }
    }

    //Transfer non-duplicate items from hoard to dungeon loot
    while(!hoard.empty()){
        if(hoard.back()->type == "potion")
            potions.push_back(hoard.back());
        else if(hoard.back()->type == "scroll")
            scrolls.push_back(hoard.back());
        else
            loot.push_back(hoard.back());
        hoard.pop_back();
    }
}

void Dungeon::identify_items(){
    bool first_item_identified = true;
    cout << "As you rest, you look over the items you have gathered more closely." << endl;
    for(vector<Treasure*>::iterator it = loot.begin(); it != loot.end(); ++it){
        if(!(*it)->identified && PC.attribute_chk("INT") > LOOT_IDENTIFY_DC){
            if(first_item_identified) cout << "You are able to identify the following:" << endl;
            first_item_identified = false;
            cout << "\t- Your " << (*it)->get_description();
            if((*it)->type=="art"){
                if((*it)->quantity == 1) cout << " is";
                else cout << " are";
            }
            else if((*it)->quantity == 1)
                cout << " is a " << (*it)->name;
            else
                cout << " are " << (*it)->name << "s";
            cout << " worth " << (*it)->value << "gp";
            if((*it)->quantity > 1) cout << " each";
            cout << "." << endl;
            (*it)->identified = true;
        }
    }

    for(vector<Treasure*>::iterator it = potions.begin(); it != potions.end(); ++it){
        if(!(*it)->identified && PC.skill_check("ARCANA") > POTION_IDENTIFY_DC){
            if(first_item_identified) cout << "Looking over the items you have gathered in more detail, you are able to identify the following:" << endl;
            first_item_identified = false;
            cout << "\t- Your " << (*it)->get_description();
            if((*it)->quantity == 1)
                cout << " is a potion of ";
            else
                cout << " are potions of ";
            cout << (*it)->name << "." << endl;
            (*it)->identified = true;
        }
    }

    for(vector<Treasure*>::iterator it = scrolls.begin(); it != scrolls.end(); ++it){
        if(!(*it)->identified && PC.skill_check("ARCANA") > SCROLL_IDENTIFY_DC){
            if(first_item_identified) cout << "Looking over the items you have gathered in more detail, you are able to identify the following:" << endl;
            first_item_identified = false;
            cout << "\t- " << (*it)->quantity << " of your scrolls";
            if((*it)->quantity == 1)
                cout << " is a scroll of ";
            else
                cout << " are scrolls of ";
            cout << (*it)->name << "." << endl;
            (*it)->identified = true;
        }
    }


    if(first_item_identified) cout << "Unfortunately, you are unable to identify anything." << endl;
    cout << endl;
}


void Dungeon::rest(){
    if(cur_room->has_monsters())
        cout << "You can't rest when there's a " << cur_room->get_active_monster() << " in the room!" << endl;
    else {
        PC.short_rest();
        identify_items();
    }
}


void Dungeon::look(){
    string input;
    cin >> input;
    if(input == "at") cin >> input;

    //room description
    if(input == "room" || input == "around"){
        cur_room->printFullDescription();

    //looking at a monster
    ///TODO add multi-monster support
    } else if(cur_room->get_monster(input) != NULL) {
        if(cur_room->has_monsters())
           cout << "He looks like a bad guy. You should kill him. What did you expect?" << endl << endl;
        else
            cout << "He looks like a bad guy. A dead bad guy. Good thing you killed him." << endl << endl;

    //looking at your weapons
    } else if(input == PC.melee_weapon || input == PC.ranged_weapon){
        cout << "It's a little rusty, but it's served you well over the years." << endl << endl;

    } else {
        ///TODO check if it's an item in inventory, give description on it
        cout << "I'm not smart enough to know what a \"" << input << "\" is... sorry. Try another command?" << endl << endl;
    }
}

//returns true iff fighting happened
bool Dungeon::combat(string input, bool is_ranged)
{
    if(cur_room->get_monster(input) != NULL){
        if(is_ranged){
            if(PC.in_melee){
                ///TODO FIXME multi-monster support (make sure the right monster gets the AOO
                ///add Character* in_melee_with to the Character class? (maybe even an array?)
                cout << "As you step away from the " << cur_room->get_active_monster() << ", it turns to attack you!" << endl;
                cur_room->get_monster(input)->melee_attack(&PC);
            }
            PC.ranged_attack(cur_room->get_monster(input));
        } else {
            PC.melee_attack(cur_room->get_monster(input));
        }
        return true;
    } else {
        cout << "You can't attack a \"" << input << "\"." << endl;
        return false;
    }

}

//returns true iff fighting happened
bool Dungeon::combat(){
    string monster_name;
    cin >> monster_name;

    cout << "Melee or ranged?" << endl;
    cout << ">> ";
    string input;
    cin >> input;
    cout << endl;
    if(input == "ranged")
        return combat(monster_name, true);
    else
        return combat(monster_name, false);

}

//returns false iff no action was taken
bool Dungeon::searching(){
    string input;
    cin >> input;


    //search a single monster
    ///TODO multi-monster support
    if(cur_room->get_monster(input) != NULL){
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
    } else if(input == "room" || input == "area"){
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

void Dungeon::drink_potion(int index){
    potions.at(index)->quantity--;
    if(PC.cur_hp == PC.max_hp)
        cout << "Nothing appears to happen." << endl << endl;
    else
        PC.heal(potions.at(index)->healing_amount());

    if(potions.at(index)->quantity == 0){
        delete potions.at(index);
        potions.erase(potions.begin()+index);
    }
}

//returns true iff a spell was cast
bool Dungeon::cast_spell(){
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
            cout << "\t(" << idx << "): " << (*it)->get_description() << endl;
            ++idx;
        }
    }
    if(flag){
        cout << "You can't cast a scroll you haven't identified. You can attempt to identify them while you rest." << endl;
        return false;
    }

    cout << endl << ">> ";
    char input;
    cin >> input;

    for(vector<Treasure*>::iterator it = scrolls.begin(); it != scrolls.end(); ++it){
        char idx = 'A';
        if((*it)->identified){
            if(idx == input){
                PC.cast((*it)->spell, cur_room->get_active_monster_char());
                (*it)->quantity--;
                if((*it)->quantity == 0){
                    delete *it;
                    scrolls.erase(it);
                }
                return true;
            }
            ++idx;
        }
    }
    return false;

}

//returns true iff you drank a potion
bool Dungeon::drink_potion(){
    if(potions.empty()){
        cout << "You have no potions to drink!" << endl;
        return false;
    }
    string input;
    if(potions.size() == 1){
        if(potions.front()->identified) cout << "Drink the " << potions.front()->name << " potion? [y/n] (" << potions.front()->quantity << " in inventory)" << endl;
        else cout << "Drink the unidentified " << potions.front()->description << " potion? [y/n] (" << potions.front()->quantity << " in inventory)" << endl;


        cin >> input;
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
            cout << endl << ">> ";
            cin >> inputChar;
        } while((unsigned)(inputChar - 'A') > potions.size());

        drink_potion(inputChar-'A');
        return true;
    }
}


bool Dungeon::getCommand() {
    cout << ">> ";
    string input;
    cin >> input;
    cout << endl;
    if(input == "exit"){
        return false;
    } else if(input == "special"){
        PC.special_action();
        return true;
    } else if (input == "rest"){
        rest();
        return true;
    } else if(input == "look" || input == "examine" || input == "inspect"){
        look();
        return true;
    } else if(input == "status"){
        PC.print_status();
        return true;

    //combat
    } else if(input == "melee"){
        cin >> input; //get monster name
        if(input == "attack") cin >> input;
        if(!combat(input, false)) return true;
    } else if(input == "ranged"){
        cin >> input; //get monster name
        if(input == "attack") cin >> input;
        if(!combat(input, true)) return true;
    } else if(input == "attack" || input == "fight"){
        if(!combat()) return true;

    } else if(input == "search" || input == "loot"){
        searching();
    } else if(input == "inventory"){
        print_inventory();
        return true;
    } else if(input == "drink" || input == "potion"){
        if(input == "drink") cin >> input;
        if(!drink_potion()) return true;

    } else if(input == "identify"){
        cin >> input;
        if(loot.empty() && potions.empty()) cout << "Sorry, not sure what you're referring to there." << endl << endl;
        else cout << "Inspecting things in that much detail takes a while. Maybe you should rest for a bit?" << endl << endl;
        return true;
    } else if(input == "cast" || input == "read"){
        if(input == "read") cin >> input; //if they type 'read scroll'
        if(!cast_spell()) return true;

    //switch rooms
    } else if(input == "open" || input == "go"){
        parse_open_door();
        return true;
    } else {
        parse_open_door(input);
        return true;
    }

    //moneter's turn to attack
    if(cur_room->has_monsters()){
    ///TODO robustness here
    ///TODO FIXME multiple monster support
        cout << "The " << cur_room->get_active_monster() << " turns to attack you!" << endl;
        cur_room->get_active_monster_char()->melee_attack(&PC);
    }

    if(!PC.is_alive()) return false;
    return true;
}

void Dungeon::run() {
    cur_room->printInitialDescription(0);
    while (in_dungeon && getCommand()){
        check_completion();
    }
    cout << "Thanks for playing!" << endl;
}
