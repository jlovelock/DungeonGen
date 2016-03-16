
#include <treasure.h>
#include <character.h>
using namespace std;

Potion::Potion(std::string name, std::string _rarity, int num) : Treasure("potion", num, magic_item_cost(_rarity)) {
    consumable = true;
    is_healing = (name == "healing");
    rarity = _rarity;
    _name = name;
    _description = name;

    if(is_healing){
        rarity == "common" ? _description = "red, glimmering" : _description = "deep red, glimmering";

        if(rarity == "uncommon")        _name = "greater healing";
        else if(rarity == "rare")       _name = "superior healing";
        else if(rarity == "very rare")  _name = "supreme healing";
    }
}

Potion::~Potion(){}

string Potion::get_description(){
    string tmp = "";
    if(!identified) tmp += "unidentified ";
    tmp += _description;
    tmp += " potion";
    if(quantity != 1) tmp += "s";
    if(identified){
        tmp +=" of ";
        tmp += name();
    }
    return tmp;
}


void Potion::use(Character* user){
    quantity--;

    ///@TODO add non-healing potions
    if(is_healing){
        if(!user->is_injured())
            cout << "Nothing appears to happen." << endl << endl;
        else
            user->heal(healing_amount());
    }
}

//For healing potions. Returns the number of hp
int Potion::healing_amount(){
    if(!is_healing) return 0;

    int n;
    if(rarity == "common")
        n=2;
    else if(rarity == "uncommon")
        n = 4;
    else if(rarity == "rare")
        n = 8;
    else if(rarity == "very rare")
        n = 10;
    else
        n = 0;

    int total = 0;
    for(int i = 0; i < n; i++){
        total += d4()+1;
        if(rarity == "very rare") total++;
    }
    return total;
}
