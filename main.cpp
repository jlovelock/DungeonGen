#include <iostream>
#include <room.h>
#include <cstdlib>
#include <time.h>
#include <dungeon.h>
#include <string>
#include <character.h>
#include <vector>

using namespace std;




int main(){
    srand(time(NULL));

    Dungeon d;
    d.run();

    return 0;
}
