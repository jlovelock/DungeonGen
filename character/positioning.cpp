
#include <iostream>
#include <math.h>
#include <spells.h>
#include <character.h>

using namespace std;

void Character::set_position(int x, int y){
    xPos = x;
    yPos = y;
}

void Character::move_position(int x, int y){
    xPos += x;
    yPos += y;
}

pair<int,int> Character::get_position(){
    return make_pair(xPos, yPos);
}


int Character::distance_to(Character* target){
    return sqrt(pow(xPos-target->xPos, 2)+pow(yPos-target->yPos, 2));
}

bool Character::within_range(Character* target, Spell* spell){
    if(distance_to(target) <= spell->range) return true;
    else return false;
}
