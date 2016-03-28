
#include <dungeon.h>
#include <defines.h>
#include <tests.h>
#include <pc.h>

using namespace std;

ifstream testfile_input;                    //input: commands the "user" enters
ofstream output_log;                        //output: print statements logged
streambuf *coutbuf = std::cout.rdbuf();     //save default output buffer
unsigned num_tests_run;


Tests::Tests(){
    test_mode = true;
    output_log.open("test/test_output_log.txt");
    if(!output_log.is_open()){
        cout << "Could not open output log. Exiting." << endl;
        exit(EXIT_FAILURE);
    }
    num_tests_run = 0;
}

Tests::~Tests(){
    test_mode = false;
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

    string filename = "test/instructions/" + test_name + ".txt";
    testfile_input.open(filename.c_str());
    if(!testfile_input.is_open()){
        cout << "## NOTE: using default instruction file." << endl;
        testfile_input.open("test/instructions/default.txt");
    }
    if(!testfile_input.is_open()){
        cout << "ERROR: could not open default instruction file. Exiting." << endl;
        exit(EXIT_FAILURE);
    }
    ++num_tests_run;
}

void Tests::start_test(string test_name){
    cout.rdbuf(output_log.rdbuf());  //redirect cout to temp/test output log
    cur_test_name = test_name;
    init_input_file(test_name);

    g = new Game();
    g->dungeon->m_ena = false;

    for(int i = 0; i < MAX_DOORS; i++){
        if(g->dungeon->starting_room()->doors[i])
            g->dungeon->starting_room()->doors[i]->set_defaults();
    }
}

void Tests::end_test(string test_name){
    cout.rdbuf(coutbuf);   //reset cout
    cur_test_name = "N/A";
    if(testfile_input) testfile_input.close();
    delete g;
}

/*
 * Adds a room to the first unattached door along the specified wall
 * If no door exists, it spawns one
 * NOTE: does not move you through said door!
 */
void Tests::add_room(string wall, int x, int y){
    int arr[MAX_DOORS] = {};
    add_room(wall, x, y, arr);
}

void Tests::add_room(string wall, int x, int y, int doors_to_add[]){
    Door* d = g->cur_room->get_door_on_wall(wall, true);

    if(!d) d = add_door(wall);

    cout << "## Adding room " << wall << " with dimensions (" << x << "," << y << ")." << endl;
    Room* next_rm = new Room(d, x, y);
    g->dungeon->rooms.push_back(next_rm);
    next_rm->doors[0] = d;
    d->second = next_rm;

    for(int i = 0; i < MAX_DOORS && doors_to_add[i] != 0; i++){
        next_rm->add_door(doors_to_add[i]);
    }

    g->dungeon->adjust_room_position(next_rm);

    cout << "## Room added: ID=" << next_rm->id << ", location=(" << next_rm->location() << ")" << endl << endl;
//    cout << "## Doors:" << endl;
//    for(int i = 0; i < MAX_DOORS; i++){
//        if(next_rm->doors[i]){
//            cout << "\t- Index " << i << " at (" << next_rm->doors[i]->xPos << "," << next_rm->doors[i]->yPos << ")" << endl;
//        }
//    }
}


void Tests::jump_to_rm(int rm){
    cout << "## Jumping to room " << rm << "." << endl;
    g->cur_room = g->dungeon->rooms.at(rm);
}

/*
 * Adds a door to the centre of wall in the current room.
 */
Door* Tests::add_door(string wall){
    cout << "## Adding door along " << wall << " wall." << endl;
    int wnum;
    if(wall == "north") wnum = NORTH;
    else if(wall == "south") wnum = SOUTH;
    else if(wall == "east") wnum = EAST;
    else if(wall == "west") wnum = WEST;
    else {
        cout << "During " << cur_test_name << ": Cannot add door on \"" << wall << "\" wall. Exiting." << endl;
    }
    Door* d = g->cur_room->add_door(wnum);
    d->set_defaults();

    cout << "## Door added: ID=" << d->id << ", position=(" << d->xPos << "," << d->yPos << ")." << endl << endl;
    return d;
}

/*
 * Moves you through a door along <direction> wall.
 * Door must exist.
 */
void Tests::go(string direction){
    Room* original = g->cur_room;
    cout << "## Opening door in " << direction << " direction." << endl;
    g->parse_open_door(direction);
    if(original->id != g->cur_room->id)
        cout << "## " << direction << " door opened." << endl << endl;
    else
        cout << "## FAILURE: " << direction << " door was NOT opened." << endl << endl;
}

/*
 * Moves you through a door along <direction> wall.
 * The room behind this door must not exist: it will be created with dimensions (x, y).
 * The door itself may or may not exist.
 */
void Tests::go(string direction, int x, int y){
    add_room(direction, x, y);
    go(direction);
}

void Tests::print_map(){
    g->dungeon->print_map(g->PC->get_position());
}

void run_all_tests(){

    for(int i = 0; i < TEST_REPETITION; i++){
        Tests* t = new Tests();
        t->unbar_door();

        t->shift_room_basic();
        t->doubly_locked_wall_shift();

        t->shrink_room_basic();
        t->shrink_room_advanced();
        t->shrink_room_sequential();

        t->extra_door_adj();
    //...add more tests here

        delete t;
    }

    cout << "All " << num_tests_run << "/" << num_tests_run << " tests completed successfully!" << endl;
}
