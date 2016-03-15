
#include <monster.h>

using namespace std;


bool Monster::search_monster(bool print_err_msgs){
    if(is_alive()){
        if(print_err_msgs) cout << "You can't do that while it's still alive!" << endl;
        return false;
    } else if (searched){
        if(print_err_msgs) cout << "You've already done that." << endl;
        return false;
    } else {
        searched = true;
        return true;
    }
}





bool Monster::has(string feature){
    for(vector<string>::iterator it = features.begin(); it != features.end(); ++it){
        if((*it) == feature) return true;
    }
    return false;
}
