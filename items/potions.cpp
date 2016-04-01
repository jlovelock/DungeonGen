
#include <treasure.h>
#include <character.h>
using namespace std;

Potion::Potion(std::string name, std::string _rarity, int num) : Treasure("potion", num, magic_item_cost(_rarity)) {
    consumable = true;
    is_healing = (name == "healing");
    rarity = _rarity;
    _name = name;
    _description = name;
    _weight = 0.5; //PHB p150

    if(is_healing){
        rarity == "common" ? _description = "red, glimmering" : _description = "deep red, glimmering";

        if(rarity == "uncommon")        _name = "greater healing";
        else if(rarity == "rare")       _name = "superior healing";
        else if(rarity == "very rare")  _name = "supreme healing";
    }
}

Potion::Potion(Potion* o) : Treasure(o){
    is_healing = o->is_healing;
}

Potion::~Potion(){}

Object* Potion::clone(){
    return new Potion(this);
}

void Potion::identify(){
    cout << "Your " << get_description();
    if(quantity == 1)
        cout << " is a potion of " << name() << "." << endl;
    else
        cout << " are potions of " << name() << "." << endl;
    identified = true;
}

string Potion::get_description(bool plural){
    string tmp = "";
    if(!identified) tmp += "unidentified ";
    tmp += _description;
    tmp += " potion";
    if(quantity != 1 && plural) tmp += "s";
    if(identified){
        tmp +=" of ";
        tmp += name();
    }
    return tmp;
}


void Potion::use(Character* user, Character* target){
    cout << "You drink the " << get_description() << "." << endl;
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
