
#include <iostream>
#include <dungeon.h>

using namespace std;

void Dungeon::print_map(){
    int grid[GRID_SIZE][GRID_SIZE];

    const int DOOR = 1;
    const int WALL = 2;
    const int HERE = 3;

    for(int i = 0; i < GRID_SIZE; i++){
        for(int j = 0; j < GRID_SIZE; j++){
            grid[i][j] = 0;
        }
    }

    for(Room* r = first_room; r != NULL; r = r->next){
        for(int i = 0; i <= r->xDim; i+=5){
            grid[(r->westEdge+i)/5][(r->southEdge)/5] = WALL;
            grid[(r->westEdge+i)/5][(r->northEdge)/5] = WALL;
        }
        for(int i = 0; i <= r->yDim; i+=5){
            grid[(r->westEdge)/5][(r->southEdge+i)/5] = WALL;
            grid[(r->eastEdge)/5][(r->southEdge+i)/5] = WALL;
        }
        for(int d = 0; d < MAX_DOORS && r->doors[d] != NULL; d++){
            grid[r->doors[d]->xPos/5][r->doors[d]->yPos/5] = DOOR;
        }
    }

    int xMid = cur_room->westEdge+cur_room->xDim/2;
    int yMid = cur_room->southEdge+cur_room->yDim/2;
    grid[xMid/5][yMid/5] = HERE;

    for(int i = GRID_SIZE-1; i >= 0; i--){
        for(int j = 0; j < GRID_SIZE; j++){
            if(grid[j][i] == WALL) cout << "#";
            else if(grid[j][i] == DOOR) cout << " ";
            else if(grid[j][i] == HERE) cout << "*";
            else cout << " ";
        }
        cout << endl;
    }
    cout << endl;

}


void Dungeon::print_inventory(){
    cout << "WEAPONS: " << endl;
    for(vector<Weapon*>::iterator it = PC.weapons.begin(); it != PC.weapons.end(); ++it){
        cout << "\t - " << (*it)->get_weapon_description(&PC);
        if((*it)->is_equipped_to(&PC))
            cout << " (equipped)";
        cout << endl;
    }
    cout << endl;

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
        cout << "MISC TREASURE:" << endl;
        for(vector<Treasure*>::iterator it = loot.begin(); it != loot.end(); ++it){
            cout << "\t- ";
            //if(!(*it)->identified) cout << "[?] ";
            cout << (*it)->quantity << " " << (*it)->get_description() << "." << endl;
        }
        cout << endl;
    }
    cout << endl;

    if(!PC.objects.empty()){
        cout << "OTHER ITEMS: " << endl;
        for(vector<Object*>::iterator it = PC.objects.begin(); it != PC.objects.end(); ++it){
            cout << "\t - " << (*it)->name();
            if((*it)->is_equipped_to(&PC))
                cout << " (equipped)";
            cout << endl;
        }
        cout << endl;
    }
}
