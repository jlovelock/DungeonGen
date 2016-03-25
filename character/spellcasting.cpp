
#include <math.h>

#include <character.h>
#include <spells.h>
#include <pc.h>

using namespace std;

int Character::saving_throw(Spell* spell){
    return save(spell->save_stat);
}

int Character::spell_attack(){
    return attribute_chk(casting_stat)+prof;
}

int Character::spell_save_DC(){
    return 8+prof+attribute_mods[casting_stat];
}

/** TODO: END THIS SPELL'S ONGOING EFFECTS !! **/
void Character::drop_concentration(){
    if(concentrating_on != NULL){
        cout << "You lose concentration on your " << concentrating_on->name() << " spell." << endl;
        delete concentrating_on;
        concentrating_on = NULL;
    }
}
