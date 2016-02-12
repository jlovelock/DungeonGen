#ifndef CONTENTS_H
#define CONTENTS_H

#include <string>

#define PRESSURE_PLATE 22;
#define MOVE_THROUGH 33;
#define TOUCH 44;
#define OPEN 55;
#define LOOK_AT 66;
#define MOVE 77;

class Trap {
    int severity;
    int trigger;
    std::string description;
    int save; // or some equivalent on how to avoid it?
};

class Contents {

    bool hasTreasure, hasMonster, hasAlly, hasHazard, hasObstacle,
          hasTrap, hasTrick;
    Contents();
    std::string hazard;

    void setHazard();

};


#endif // CONTENTS_H
