
#include <character.h>
#include <weapon.h>

using namespace std;


int Character::AC(){
    int modifiers = 0;
    if((main_hand && main_hand->name() == "shield") || (off_hand && off_hand->name() == "shield"))
        modifiers +=2;

    ///@TODO they should only get this bonus when wearing armor
    if(has_fighting_style("defense"))
        modifiers +=1;

    return _AC;
}


bool Character::in_melee_with(Character* opponent){
    return     xPos - opponent->xPos <= 5
            && xPos - opponent->xPos >= -5
            && yPos - opponent->yPos <= 5
            && yPos - opponent->yPos >= -5;
}

int Character::adjusted_speed(){
    int modifiers = 0;
    if(is("buffed by longstrider")) modifiers += 10;
    if(is_heavily_encumbered()) modifiers -= 20;
    else if(is_encumbered()) modifiers -= 10;

    return speed + modifiers;
}

///@TODO dash actions
void Character::close_with(Character* opponent){
    int dX = opponent->xPos - xPos;
    if(dX > 0) dX -= 5;
    else if(dX < 0) dX += 5;

    int dY = opponent->yPos - yPos;
    if(dY > 0) dY -= 5;
    else if(dY < 0) dY += 5;

    float percentage = (float) adjusted_speed() / distance_to(opponent);
    if(percentage < 1.0){
        dX *= percentage;
        dY *= percentage;
    }

    xPos += dX;
    yPos += dY;

    if(!in_melee_with(opponent)){
        if(!is_monster) cout << "You are too far away to close this turn, but you move as much as you can." << endl;
        else cout << "He charges towards you, but can't close in time to attack yet." << endl;
    }
}




//void Character::melee_attack(Character* opponent){
//    Weapon* w = weapon_select(opponent);
//    if(in_melee == false){
//        if(is_monster) cout << "The " << PC_class << " charges you from across the room with his " << w.name() << " drawn." << endl;
//        else cout << "You charge forward to meet the " << opponent->PC_class << " with your " << w.name() << " drawn." << endl;
//    }
//    attack(opponent, attack_roll(w), damage(w));
//    in_melee = true;
//    opponent->in_melee = true; ///TODO FIXME if you attack at range first and melee second, it doesn't set your flag to "in melee", even though this line should have...
//}
//
//void Character::ranged_attack(Character* opponent){
//    Weapon w = weapon_select(opponent);
//    if(!is_monster) cout << "You draw your " << w.name() << " and prepare to strike." << endl;
//    else cout << "The " << PC_class << " draws a " << w.name() << " and readies a shot." << endl;
//    attack(opponent, attack_roll(w), damage(w));
//    in_melee = false;
//    opponent->in_melee = false;
//}

void Character::adjust_for_resistances(int& dmg, string dtype){
    if(dmg <= 0) return;

    if(is("protected by blade ward") && (dtype == "piercing" || dtype == "bludgeoning" || dtype == "slashing")){
        if(is_monster) cout << "His blade ward deflects part of your blow." << endl;
        else cout << "Your blade ward deflects part of his blow." << endl;
        dmg /= 2;
    }

    if((race == "Hill dwarf" || race == "Mountain dwarf") && dtype == "poison"){
        if(!is_monster) cout << "Your dwarven resilience helps you partially resist the poison." << endl;
        dmg /= 2;
    }
}

void Character::take_damage(int dmg){
    ///TODO make this whole section more flavourful!
    if(is_monster){
        if(cur_hp > dmg){
            cur_hp -= dmg;
            if((float)cur_hp/max_hp > 0.5) cout << "He still looks ready to fight." << endl << endl;
            else if((float)cur_hp/max_hp > 0.25) cout << "He's not in very good shape." << endl << endl;
            else cout << "He's barely alive at this point." << endl << endl;

            if(concentrating()){
                int DC = dmg / 2;
                if (DC < 10) DC = 10;
                if(save("CON") >= DC) cout << "Somehow, he manages to keep concentrating on his spell." << endl;
                else {
                    cout << "The blow distracts him, and he loses concentration on his spell." << endl;
                    drop_concentration();
                }
            }

        } else {
            cur_hp = 0;
            cout << "He collapses on the ground, dead." << endl;
            drop_concentration();
        }
        return;
    }

    if(temp_hp > 0){
        if(temp_hp >= dmg){
            temp_hp -= dmg;
            cout << "Fortunately, your temporary hitpoints absorb the entire blow. (You have " << temp_hp << " temp hp remaining)." << endl;
        } else {
            cout << "Fortunately, your temporary hitpoints absorb " << temp_hp << " points of damage, but ";
            dmg -= temp_hp;
            temp_hp = 0;
            cout << dmg << " points of damage are still dealt directly to you. " << endl;
            cur_hp -= dmg;

            if(cur_hp > 0){
                cout << "You have " << cur_hp << "/" << max_hp << " hitpoints remaining." << endl << endl;
            } else {
                ///TODO implement death saves, stabilizing, etc
                cur_hp = 0;
                cout << "You have died." << endl << endl;
                return;
            }
        }

            if(concentrating()){
                int DC = dmg / 2;
                if (DC < 10) DC = 10;
                if(save("CON") >= DC) cout << "Thankfully, you manage to keep concentrating on your spell." << endl;
                else {
                    cout << "Reeling from the blow, you lose concentration on your spell." << endl;
                    drop_concentration();
                }
            }

        return;
    } else {
        if(dmg < cur_hp){
            cur_hp -= dmg;
            cout << "You have " << cur_hp << "/" << max_hp << " hitpoints remaining." << endl << endl;

            if(concentrating()){
                int DC = dmg / 2;
                if (DC < 10) DC = 10;
                if(save("CON") >= DC) cout << "Thankfully, you manage to keep concentrating on your spell." << endl;
                else {
                    cout << "Reeling from the blow, you lose concentration on your spell." << endl;
                    drop_concentration();
                }
            }
        } else { ///TODO same as above
            cur_hp = 0;
            cout << "You have died." << endl << endl;
        }
    }
}


int Character::attack_roll(Weapon* w){
   return w->attack_roll(this);
}

int Character::damage(Weapon* w, string mode){
    return w->damage_roll(this, mode);
}
