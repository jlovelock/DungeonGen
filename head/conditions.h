
#include <string>

class Character;

class Condition {

    std::string _name;
    int rounds_remaining;
    bool autopass;
    int DC;
    bool test_each_turn;
    std::string check_type; //attribute, skill, or save
    std::string check_stat;
    ///add something about when to test out? should default be end or start of turn?
    // maybe switch test_each_turn from a bool to a #define'd int (never, EOT, start of turn, etc -- customizable)

public:
    Condition(std::string n, int dur, std::string check="", int dc=0, bool tet=false);
    Condition(Condition*);
    ~Condition();
    std::string name(){ return _name; }
    bool advance(Character*, bool quiet=false);
    bool test(Character*, bool);
    void set_DC(int);
    std::string time_remaining();
};
