#include <iostream>
#include <cstdlib>
#include <time.h>

#include <dungeon.h>
#include <tests.h>

using namespace std;

bool test_mode = false;

int main(){
    srand(time(NULL));
    test_mode = false;

    Tests* t = new Tests();
    t->unbar_door();
    delete t;

    while(1){
        Dungeon d;
        d.run();
    }

    return 0;
}
