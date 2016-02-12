#ifndef OBJECTS_H
#define OBJECTS_H

#include <string>

class Character;

class Object{
    public:
        Object(std::string name);
        Object(){} //TODO this shouldn't be necessary or ever called but it is =/
        std::string name(){ return _name; }
        bool is_two_handed(){ return _2h; }
        bool is_weapon(){ return _is_weapon; }
        bool is_equipped_to(Character*);

    protected:
        std::string _name;
        bool _2h;
        bool _is_weapon;
};

#endif // OBJECTS_H
