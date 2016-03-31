
#include <dungeon.h>
using namespace std;

void Dungeon::read_preferences(){
    ifstream sfile("save/settings.conf");

    if(!sfile.is_open()){
        cout << "Missing file settings.conf. Exiting." << endl;
        exit(EXIT_FAILURE);
    }

	char c;

    // Dungeon type: STRONGHOLD[0]
    sfile.get(c);
    switch(c){
        case '0':
            dtype = "STRONGHOLD";
            break;
        default:
            cout << "Corrupt settings file detected [dtype=" << c << "]. Exiting." << endl;
            exit(EXIT_FAILURE);
    }

    // Monsters enabled: false[0], true[1]
    sfile.get(c);
    switch(c){
        case '0':
            m_ena = false; break;
        case '1':
            m_ena = true; break;
        default:
            cout << "Corrupt settings file detected [m_ena=" << c << "]. Exiting." << endl;
            exit(EXIT_FAILURE);
    }

    //treasure_enabled: RANDOM[0], ALWAYS[1], or NEVER[2]
    sfile.get(c);
    switch(c){
        case '0':
            t_ena = "RANDOM"; break;
        case '1':
            t_ena = "ALWAYS"; break;
        case '2':
            t_ena = "NEVER"; break;
        default:
            cout << "Corrupt settings file detected [t_ena=" << c << "]. Exiting." << endl;
            exit(EXIT_FAILURE);
    }

    //magic items: RANDOM[0], ALWAYS[1], or NEVER[2]
    sfile.get(c);
    switch(c){
        case '0':
            mag_it = "RANDOM"; break;
        case '1':
            mag_it = "ALWAYS"; break;
        case '2':
            mag_it = "NEVER"; break;
        default:
            cout << "Corrupt settings file detected [mag_it=" << c << "]. Exiting." << endl;
            exit(EXIT_FAILURE);
    }

	sfile.close();
}

void Dungeon::write_preferences(){
    ofstream sfile("save/settings.conf");

    if(!sfile.is_open()){
        cout << "Missing file settings.conf. Exiting." << endl;
        exit(EXIT_FAILURE);
    }

    // Dungeon type: STRONGHOLD[0]
    if(dtype == "STRONGHOLD"){
        sfile.put('0');
    }

    // Monsters enabled: false[0], true[1]
    if(!m_ena)
        sfile.put('0');
    else
        sfile.put('1');

    //treasure_enabled: RANDOM[0], ALWAYS[1], or NEVER[2]
    if(t_ena == "RANDOM")
        sfile.put('0');
    else if(t_ena == "ALWAYS")
        sfile.put('1');
    else
        sfile.put('2');

    //magic items enabled: RANDOM[0], ALWAYS[1], or NEVER[2]
    if(mag_it == "RANDOM")
        sfile.put('0');
    else if(mag_it == "ALWAYS")
        sfile.put('1');
    else
        sfile.put('2');

    sfile.close();
}


void Dungeon::edit_preferences(){
    cout << "Your current settings:" << endl;
    cout << "\tDungeon type: " << dtype << endl;
    cout << "\tMonster spawning: " << (m_ena ? "ON" : "OFF") << endl;
    cout << "\tTreasure spawning: " << t_ena << endl;
    cout << "\tMagic items: " << mag_it << endl;
    cout << endl;

    cout << "Enter the setting you'd like to change, or 'back' to return to the previous menu." << endl;
    string input;
    read(input);

    if(contains(input, "back")) return;

    if(contains(input, "dungeon") || contains(input, "type")){
        cout << "Sorry, only the STRONGHOLD dungeon type is supported at this time." << endl;
    } else if(contains(input, "monster")){
        cout << "Monsters ON or OFF? " << endl;
        read(input);
        if(contains(input, "off")){
            m_ena = false;
            cout << "Monster spawning set to OFF." << endl;
        } else if(contains(input, "on")){
            m_ena = true;
            cout << "Monster spawning set to ON." << endl;
        } else {
            cout << "Error, unrecognized input." << endl;
            unrecognized_input << "(in settings menu: ) " << input << endl;
        }
    } else if(contains(input, "treasure")){
        cout << "Treasure spawning: RANDOM, ALWAYS, or NEVER?" << endl;
        read(input);
        if(contains(input, "random")){
            t_ena = "RANDOM";
            cout << "Treasure will spawn randomly." << endl;
        } else if(contains(input, "always")){
            t_ena = "ALWAYS";
            cout << "There will be treasure in every room." << endl;
        } else if(contains(input, "never")){
            t_ena = "NEVER";
            cout << "Treasure will never spawn." << endl;
        } else {
            cout << "Input not recognized." << endl;
            unrecognized_input << "(in treasure settings: ) " << input << endl;
        }
    } else if(contains(input, "magic") || contains(input, "items")){
        cout << "Treasure hoards containing magic items: RANDOM, ALWAYS, or NEVER?" << endl;
        read(input);
        if(contains(input, "random")){
            mag_it = "RANDOM";
            cout << "Magic items will randomly appear in treasure hoards." << endl;
        } else if(contains(input, "always")){
            mag_it = "ALWAYS";
            cout << "There will be a magic item in every treasure hoard." << endl;
        } else if(contains(input, "never")){
            mag_it = "NEVER";
            cout << "Magic items will never spawn." << endl;
        } else {
            cout << "Input not recognized." << endl;
            unrecognized_input << "(in magic item settings: ) " << input << endl;
        }
    } else cout << "Input not recognized." << endl;
    cout << endl;
    write_preferences();
    edit_preferences();
}
