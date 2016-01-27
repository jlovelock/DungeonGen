#include <contents.h>
#include <defines.h>

void Contents::setHazard(){
    int x = d20();
    if(x < 4) hazard = "brown mold";
    else if(x < 9) hazard = "green slime";
    else if(x < 11) hazard = "shrieker";
    else if(x < 16) hazard = "spiderwebs";
    else if(x < 18) hazard = "violet fungus";
    else hazard = "yellow mold";
}

Contents::Contents(){

    //defaults
    hasTreasure=false;
    hasMonster=false;
    hasAlly=false;
    hasHazard=false;
    hasObstacle=false;
    hasTrap=false;
    hasTrick=false;

    //overwrite appropriate section(s)
    int x = d100();
    if(x<9){
        hasMonster = true; ///TODO distinguish between dominant / nondominant monster species
    } else if(x < 16){
        hasMonster = true;
        hasTreasure = true;
    } else if(x < 28){
        hasAlly = true;
    } else if(x < 34){
        hasAlly = true;
        hasTreasure = true;
    } else if(x < 43){
        hasMonster = true;
    } else if(x < 51){
        hasMonster = true;
        hasTreasure = true;
    } else if(x < 59){
        hasHazard = true;
        setHazard();
        hasTreasure = true; ///TODO less treasure than other treasure rooms
    } else if(x < 64){
        hasObstacle = true;
    } else if(x < 74){
        hasTrap = true;
    } else if(x < 77){
        hasTrap = true;
        hasTreasure = true;
    } else if(x < 81){
        hasTrick = true;
    } else if(x < 89){
    } else if(x < 94){
        hasHazard = true;
        setHazard();
    } else {
        hasTreasure = true;
    }
}
