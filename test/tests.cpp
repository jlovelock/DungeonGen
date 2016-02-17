
#include <dungeon.h>
#include <defines.h>
#include <tests.h>

using namespace std;

ifstream testfile_input;


Tests::Tests(){
    test_mode = true;
}

Tests::~Tests(){
    test_mode = false;
    testfile_input.close();
}

void Tests::init_input_file(string test_name){
    string filename = "test\\" + test_name + ".txt";
    testfile_input.open(filename.c_str());
    if(!testfile_input){
        cout << "Cannot open file \"" << test_name << ".txt\"" << endl;
        exit(EXIT_FAILURE);
    }
}
