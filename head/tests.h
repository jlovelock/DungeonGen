
#ifndef TESTS_H
#define TESTS_H

#include <string>
#include <game.h>

class Tests {
private:
    Game* g;
    std::string cur_test_name;

    void start_test(std::string test_name);
    void end_test(std::string test_name);
    void init_input_file(std::string test_name);

    // Utility functions
    void go(std::string direction);
    void go(std::string direction, int x, int y);
    void add_room(std::string wall, int x, int y);
    void add_room(std::string wall, int x, int y, int doors_to_add[]);
    Door* add_door(std::string wall);
    void jump_to_rm(int rm);
    void print_map(){ g->dungeon->print_map(g->PC->get_position()); }

public:
    Tests();
    ~Tests();
    void unbar_door();

    void shift_room_basic();
    void doubly_locked_wall_shift();
    void shrink_room_basic();
    void shrink_room_advanced();
    void shrink_room_sequential();

    void merge_doors_basic();
    void extra_door_new();
    void extra_door_adj();


};

#endif // TESTS_H
