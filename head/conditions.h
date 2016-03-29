
#include <string>
#include <defines.h>

class Character;

class Condition {

    std::string _name;
    int rounds_remaining;
    bool autopass;
    int DC;
    bool test_each_turn;
    std::string check_type; //attribute, skill, or save
    std::string check_stat;
    int check_time; //see defines.h
    Character* target;
    bool active;
    Condition(Condition*, Character*);

public:
    Condition(std::string n, int dur, std::string check="", int dc=0, int time=TARGET_END_TURN);
    ~Condition();
    std::string name(){ return _name; }
    bool advance(bool quiet=false);
    bool test(bool);
    void set_DC(int);
    std::string time_remaining();
    bool is_active(){ return active; }
    void deactivate(){ active = false; }
    void apply(Character* target, Character* caster);
    bool updates_at(int time){ return check_time == time; }
    Character* get_target() { return target; }
};
