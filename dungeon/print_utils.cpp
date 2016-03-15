
#include <iostream>
#include <dungeon.h>
#include <monster.h>

using namespace std;

void Dungeon::print_map(pair<int,int> POI){
    int grid[GRID_SIZE][GRID_SIZE];

    const int DOOR = 1;
    const int WALL = 2;
    const int HERE = 3;
    const int MONSTER = 4;

    for(int i = 0; i < GRID_SIZE; i++){
        for(int j = 0; j < GRID_SIZE; j++){
            grid[i][j] = 0;
        }
    }

    //for(Room* r = first_room; r != NULL; r = r->next){
    for(vector<Room*>::iterator it = rooms.begin(); it != rooms.end(); ++it){
        for(int i = 0; i <= (*it)->xDim; i+=5){
            grid[((*it)->westEdge+i)/5][((*it)->southEdge)/5] = WALL;
            grid[((*it)->westEdge+i)/5][((*it)->northEdge)/5] = WALL;
        }
        for(int i = 0; i <= (*it)->yDim; i+=5){
            grid[((*it)->westEdge)/5][((*it)->southEdge+i)/5] = WALL;
            grid[((*it)->eastEdge)/5][((*it)->southEdge+i)/5] = WALL;
        }
        for(int d = 0; d < MAX_DOORS && (*it)->doors[d] != NULL; d++){
            grid[(*it)->doors[d]->xPos/5][(*it)->doors[d]->yPos/5] = DOOR;
        }
        for(int i = 0; i < MAX_MONSTERS && (*it)->monsters[i] != NULL; i++){
            if((*it)->monsters[i]->is_alive())
                grid[(*it)->monsters[i]->xPos/5][(*it)->monsters[i]->yPos/5] = MONSTER;
        }
    }

//    int xMid = cur_room->westEdge+cur_room->xDim/2;
//    int yMid = cur_room->southEdge+cur_room->yDim/2;
//    grid[xMid/5][yMid/5] = HERE;

    grid[POI.first/5][POI.second/5] = HERE;

    for(int i = GRID_SIZE-1; i >= 0; i--){
        for(int j = 0; j < GRID_SIZE; j++){
            if(grid[j][i] == WALL) cout << "#";
            else if(grid[j][i] == DOOR) cout << " ";
            else if(grid[j][i] == HERE) cout << "*";
            else if(grid[j][i] == MONSTER) cout << "M";
            else cout << " ";
        }
        cout << endl;
    }
    cout << endl;

}
