#ifndef DEFINES_H
#define DEFINES_H

#include <vector>
#include <string>
#include <fstream>

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
bool contains(std::string to_search, std::string keyword);
void convert_to_uppercase(std::string& input);
void convert_to_lowercase(std::string& input);
bool is_vowel(char);
bool in_list(std::string, std::string[]);
int num_digits(int);
void run_all_tests();

extern bool test_mode;
extern std::ifstream testfile_input;
extern unsigned num_tests_run;

extern std::ofstream unrecognized_input;

int scroll_save_DC(int lvl);

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

//there has got to be a better way to do this...
static const std::vector<std::string> skills_list(skargs, skargs+sizeof(skargs)/sizeof(skargs[0]));

bool is_skill(std::string s);

void read(std::string& input);

std::string to_string(int dir); //converts NORTH to "north", eg

#define GRID_SIZE 30
#define TEST_REPETITION 10
#define DEBUG (0 || test_mode)

#define NORTH 100
#define SOUTH 200
#define EAST 300
#define WEST 400
#define MAX_DOORS 7
#define MAX_MONSTERS 5

//check_time for conditions
#define CASTER_END_TURN 101010
#define CASTER_START_TURN 202020
#define TARGET_START_TURN 303030
#define TARGET_END_TURN 404040

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

//#define STR_STAT 101
//#define DEX_STAT 202
//#define CON_STAT 303
//#define INT_STAT 404
//#define WIS_STAT 505
//#define CHA_STAT 606

#define ACTION 222
#define BONUS_ACTION 333
#define REACTION 444

#define START_X 50
#define START_Y 50

#endif // DEFINES_H
