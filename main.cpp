#include <iostream>
#include <cstdlib>
#include <time.h>

#include <dungeon.h>
#include <tests.h>

using namespace std;


int main(){
    srand(time(NULL));

//    Tests t;
//    t.unbar_door();

    Dungeon d;
    d.run();

    return 0;
}
