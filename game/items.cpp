
#include <game.h>
#include <weapon.h>
#include <inventory.h>
#include <pc.h>

using namespace std;

//TODO check if item is already equipped, being careful of items with duplicate names (ie equipping a second offhand shortsword is OK)
bool Game::equip_item(string input){
    Object* o = PC->inventory->get_item(input);
    if(!o){
        cout << "That item is not in your inventory!" << endl;
        return false;
    }
    return PC->equip(o);
}

//returns true iff you drank a potion
bool Game::drink_potion(string input){

    vector<Potion*> potions = PC->inventory->potion_vector();

    if(potions.empty()){
        cout << "You have no potions to drink!" << endl;
        return false;
    }
    Potion* p;

    if(potions.size() == 1){
        p = potions.front();
        cout << "Drink the " << p->get_description() << "? [y/n] (" << p->get_quantity() << " in inventory)" << endl;

        read(input);
        if(input == "y"){
            p->use(PC);
            if(p->depleted()) PC->inventory->remove(p);
            return true;
        } else {
            cout << "Alright, what do you want to do instead?" << endl;
            return false;
        }

    } else {
        p = (Potion*) PC->inventory->get_item(input);
        while(!p) {
            cout << "Which potion?" << endl;
            for(auto it = potions.begin(); it != potions.end(); ++it){
                cout << "\t- " << (*it)->get_description() << endl;
            }
            cout << endl;
            read(input);
            p = (Potion*)PC->inventory->get_item(input);
        }

        p->use(PC);
        if(p->depleted()) PC->inventory->remove(p);
        return true;
    }
}


Scroll* Game::select_scroll(){
    vector<Scroll*> scrolls = PC->inventory->scroll_vector();
    bool flag = true;
    Scroll* s;
    do {
        for(auto it = scrolls.begin(); it != scrolls.end(); ++it){
            if((*it)->is_identified()){
                if(flag){
                    cout << "Choose which scroll you would like to cast, or enter 'cancel' to exit this menu." << endl;
                    flag = false;
                }
                cout << "\t- " << (*it)->name() << endl;
            }
        }
        if(flag){
            cout << "You can't cast a scroll you haven't identified. You can attempt to identify them while you rest." << endl;
            return NULL;
        }

        cout << endl;
        string input;
        read(input);
        s = (Scroll*) PC->inventory->get_item(input);
    } while (!s);
    return s;
}

//returns true iff a spell was cast
bool Game::cast_spell(string input){
    vector<Scroll*> scrolls = PC->inventory->scroll_vector();

    if(scrolls.empty()){
        cout << "You have no scrolls from which to cast." << endl << endl;
        return false;
    }

    Scroll* to_cast = (Scroll*) PC->inventory->get_item(input);

    if(!to_cast) to_cast = select_scroll();
    if(!to_cast) return false;

    if(!to_cast->is_identified()){
        cout << "You can't cast a scroll you haven't identified. You can attempt to identify them while you rest." << endl;
        return false;
    }

    if(to_cast->targets_enemy() && !cur_room->has_monsters()){
        cout << "There are no enemies in the room to cast that at." << endl << endl;
        return false;
    }

    to_cast->use(PC, cur_room->get_active_monster_char());
    if(to_cast->depleted()) PC->inventory->remove(to_cast);
    return true;
}


