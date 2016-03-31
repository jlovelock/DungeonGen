
#include <limits.h>
#include <pc.h>
#include <monster.h>
#include <inventory.h>

using namespace std;


bool PlayerChar::search_monster(Monster* m, bool print_err_msgs){
    if(m->is_alive()){
        if(print_err_msgs) cout << "You can't do that while it's still alive!" << endl;
        return false;
    } else if (m->was_looted()){
        if(print_err_msgs) cout << "You've already done that." << endl;
        return false;
    } else {
        inventory->transfer(m->inventory, m->full_name());
        m->mark_as_looted();
        return true;
    }
}


void PlayerChar::short_rest(){
    if(hit_dice > 0){
        cout << "You may spend any number of your remaining hit dice; each will restore 1d" << hit_die_size << "+" << attribute_mods["CON"] << " hp." << endl;
        cout << " You have " << hit_dice << " hit ";
        if(hit_dice == 1) cout << "die "; else cout << "dice ";
        cout << "remaining. How many would you like to spend?" << endl;

        int num;
        while(true){
            string tmp;
            read(tmp);
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

    int n_changes = 0;
    cout << "You rest here for a full hour." << endl;

    if(second_wind_available == false){
        second_wind_available = true;
        cout << "You have regained the use of your Second Wind class feature." << endl;
        n_changes++;
    }

    for(int i = 0; i < 600; i++){
        end_turn(true);
    }
    cout << endl;
}

int PlayerChar::next_levelup(){
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

void PlayerChar::levelup(){
    level++;
    cout << "You have reached level " << level << "!" << endl;
    cout << "Wow, didn't expect you to make it this far -- I haven't actually programmed level 2 yet, but stay tuned =]" << endl << endl;
}


void PlayerChar::special_action(){
    //sanity check -- this function is just for PCs
    //if(is_monster) return;

    cout << "Enter one of the following special action options, or cancel to return to the previous menu." << endl;
    if(second_wind_available){
        cout << "\t- secondwind [bonus action]: regain 1d10+" << level << " hitpoints as a bonus action" << endl;
    }

    cout << endl;
    string input;
    do {
        read(input);

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
            unrecognized_input << "(in special menu: ) " << input << endl;
        }
    } while (true);

}
