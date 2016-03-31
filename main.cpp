#include <iostream>
#include <cstdlib>
#include <time.h>

#include <game.h>
#include <tests.h>

using namespace std;

bool test_mode;

void init(){
    srand(time(NULL));
    test_mode = false;
}

int main(){
    init();
//    run_all_tests();

    while(1){
        Game* g = new Game;
        g->run();
        delete g;
    }

    return 0;
}
