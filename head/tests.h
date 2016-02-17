
#ifndef TESTS_H
#define TESTS_H

#include <string>

class Tests {
private:
    void init_input_file(std::string test_name);

public:
    Tests();
    ~Tests();
    void unbar_door();
    void shift_room_basic();


};

#endif // TESTS_H
