#include <iostream>
#include <cstdlib>
#include <time.h>

#include <dungeon.h>
#include <tests.h>

using namespace std;

bool test_mode = false;

void init(){
    srand(time(NULL));
    test_mode = false;
}

int main(){
    init();
    run_all_tests();

    while(1){
        Dungeon d;
        d.run();
    }

    return 0;
}
