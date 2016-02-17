
#include <dungeon.h>
#include <defines.h>
#include <tests.h>

using namespace std;

ifstream testfile_input;                    //input: commands the "user" enters
ofstream output_log;                        //output: print statements logged
streambuf *coutbuf = std::cout.rdbuf();     //save default output buffer
unsigned num_tests_run;


Tests::Tests(){
    test_mode = true;
    output_log.open("test/test_output_log.txt");
    cout.rdbuf(output_log.rdbuf());  //redirect cout to temp/test output log
    num_tests_run = 0;
}

Tests::~Tests(){
    test_mode = false;
    cout.rdbuf(coutbuf);   //reset cout
    testfile_input.close();
    output_log.close();
}

void Tests::init_input_file(string test_name){

    cout << endl << endl << endl << endl
         << "-------------------------------------------------------------------------------" << endl
         << "|" << endl << "|" << endl
         << "|\tTEST #" << num_tests_run << ": " << test_name << endl
         << "|" << endl << "|" << endl
         << "-------------------------------------------------------------------------------" << endl
         << endl << endl << endl << endl;

    if(testfile_input) testfile_input.close();
    string filename = "test/instructions/" + test_name + ".txt";
    testfile_input.open(filename.c_str());
    if(!testfile_input){
        cout << "Cannot open file \"" << test_name << ".txt\"" << endl;
        exit(EXIT_FAILURE);
    }
    ++num_tests_run;
}

void run_all_tests(){

    Tests* t = new Tests();
    t->unbar_door();
    t->shift_room_basic();
    //...add more tests here
    delete t;

    cout << "All " << num_tests_run << "/" << num_tests_run << " tests completed successfully!" << endl;
}
