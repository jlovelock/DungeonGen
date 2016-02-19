#include <iostream>
#include <room.h>

using namespace std;

void Room::generatePassage(Door* d){
    //cout << "--this should be a passage, but it's a room for now :) bear with me!--" << endl;

    ///TODO: add passage support here!
    generateChamber("STRONGHOLD", d);
    return;

    int entrance = d->firstWall;

    int x = d20();
    cout << "generating passage with x = " << x << endl;
    xDim = 0; yDim = 0;
    int num_doors = 0;
    while(true){
        cout << "\tloop: xDim = " << xDim << ", yDim = " << yDim << endl;
        if(x < 3) {
            if(entrance == EAST || entrance == WEST)
                xDim+=30;
            else
                yDim += 30;
            continue;
        } else if(x == 3) {
            if(entrance == EAST || entrance == WEST)
                xDim += 30;
            else
                yDim +=30;
            doors[num_doors++] = new Door(this, right(entrance),false,false);
            continue;
        } else if(x==4) {
            if(entrance == EAST || entrance == WEST)
                xDim += 30;
            else
                yDim +=30;
            doors[num_doors++] = new Door(this, left(entrance),false,false);
            continue;
        } else if(x==5) {
            if(entrance == EAST || entrance == WEST)
                xDim += 20;
            else
                yDim +=20;
            doors[num_doors] = new Door(this, opposite(entrance),false,false);
            return;
        } else if(x < 8) {
            if(entrance == EAST || entrance == WEST)
                xDim += 30;
            else
                yDim +=30;
            doors[num_doors++] = new Door(this, right(entrance),true,false);
            continue;
        } else if(x < 10) {
            if(entrance == EAST || entrance == WEST)
                xDim += 30;
            else
                yDim +=30;
            doors[num_doors++] = new Door(this, left(entrance),true,false);
            continue;
        } else if(x==10){
            if(entrance == EAST || entrance == WEST)
                xDim += 20;
            else
                yDim +=20;
            if(d10()==10)
                addSecretDoor(num_doors++, entrance);
            return;
        } else if(x<13) {
            if(entrance == EAST || entrance == WEST)
                xDim += 20;
            else
                yDim +=20;
            doors[num_doors] = new Door(this, left(entrance),true,false);
            return;
        } else if(x < 15) {
            if(entrance == EAST || entrance == WEST)
                xDim += 20;
            else
                yDim +=20;
            doors[num_doors] = new Door(this, right(entrance),true,false);
            return;
        } else { ///TODO add x=20 case with stairs
            doors[num_doors] = new Door(this, opposite(entrance),false,false);
            return;
        }

        x = d20();

    }
}
