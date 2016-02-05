#ifndef DEFINES_H
#define DEFINES_H

#include <vector>

int d4();
int d6();
int d8();
int d10();
int d12();
int d20();
int d100();
int opposite(int wall);
int left(int wall);
int right(int wall);


static const char* skargs[] = {
    "ATHLETICS",
    "ACROBATICS",
    "SLEIGHT OF HAND",
    "THIEVES TOOLS",
    "ARCANA",
    "HISTORY",
    "INVESTIGATION",
    "NATURE",
    "RELIGION",
    "ANIMAL HANDLING",
    "INSIGHT",
    "MEDICINE",
    "PERCEPTION",
    "SURVIVAL",
    "DECEPTION",
    "INTIMIDATION",
    "PERFORMANCE",
    "PERSUASION"
  };

static const std::vector<std::string> skills_list(skargs, skargs+sizeof(skargs)/sizeof(skargs[0]));

static bool is_skill(std::string s){
    for(unsigned i = 0; i < skills_list.size(); i++){
        if(skills_list[i] == s) return true;
    }
    return false;
}

#define NORTH 100
#define SOUTH 200
#define EAST 300
#define WEST 400
#define MAX_DOORS 7
#define MAX_MONSTERS 5

///TODO SET THESE
#define LOCKPICK_DC 0
#define SECRET_DOOR_DC 0
#define FIND_TREASURE_DC 0

#define LOOT_IDENTIFY_DC 15
#define POTION_IDENTIFY_DC 15
#define SCROLL_IDENTIFY_DC 15

#define COMMON 123
#define UNCOMMON 234
#define RARE 345
#define VERY_RARE 456

#define STR_STAT 101
#define DEX_STAT 202
#define CON_STAT 303
#define INT_STAT 404
#define WIS_STAT 505
#define CHA_STAT 606

#define ACTION 222
#define BONUS_ACTION 333
#define REACTION 444

#endif // DEFINES_H