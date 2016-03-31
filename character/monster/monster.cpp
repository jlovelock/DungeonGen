
#include <monster.h>

using namespace std;


Monster::~Monster(){

}


bool Monster::has(string feature){
    for(auto it = features.begin(); it != features.end(); ++it){
        if((*it) == feature) return true;
    }
    return false;
}
