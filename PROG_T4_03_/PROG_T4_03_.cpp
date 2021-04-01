#include <iostream>
#include <string>
#include <fstream> //handle files
#include <stdlib.h>
#include <vector> //vectors

using namespace std; // makes all the code more readable

void importMap(int num_map){
    ifstream inStream;
   
    string file_name = "MAZE_" + to_string(num_map) + ".txt";
    inStream.open(file_name);

    //NO MAP WITH THIS NUMBER
    if(inStream.fail()){
        cerr << "There is no map with that number." << endl;
        exit(1);
    }
    //WHEN EVERYTHING IS OKAY!
    else{
        //SIZE OF MAP
        int height, lenght;
        char k;
        inStream >> height >> k >> lenght;

        //initialized vector
        vector<vector<char>> map_vec(height, vector<char> (lenght, ' '));

        for(int l = 0; l < height; l++){
            for(int c = 0; c < lenght; c++){
                cout << "lol ";
            }
            cout << endl;
        }
    }
}

void menuGameState(int Inst, string &gamestate){
    if (Inst == 0) gamestate = "exit";
    if (Inst == 1) gamestate = "rules";
    if (Inst == 2) gamestate = "play";
}

//we can redo this function for strings too...
void checkInput(int &variable){  //maybe redo this function with a loop instead recursion
    cin >> variable;
    if (cin.fail()){
        if (cin.eof()){
            cin.clear(); //clears error flags
            cin.ignore(10000, '\n'); //clears buffer
            //decide what to do if it gets to the end of the file
        }
        else{
            cin.clear(); //clears error flags
            cin.ignore(10000, '\n'); //clears buffer
            cout << "You wrote something invalid, we were looking for an integer!" << endl;
            checkInput(variable); //if it's wrong, it asks again
        }
    }
}

void menu(int &inst) {        // menu function
    cout << "Menu \nPlease choose an option" << endl;
    cout << "1) Rules \n2) Play \n0) Exit" << endl;
    checkInput(inst);   //need to check input
    cout << endl;
}


/* Options function ################################################ */
void showRules(string& state) {
    system("CLS"); //clears the user's view
    int back = 0;

    // inserir as regras deste jogo 
    cout << "Rules: \n\n" << "I do not know what to say my friend!" << endl;

    while (back != 1) {
        cout << "(Press '1' to exit rules)" << endl;  //maybe not "1" but works for now
        checkInput(back); // check input
        if (back == 1) { state = "menu"; }
    }
}

void play(string& state) {
    int N_MAZE;
    cout << "Pick game Maze. Press 0 to return to the menu." << endl;
    checkInput(N_MAZE);     //need to check input
    if (N_MAZE == 0) { state = "menu"; } //return to menu
    else importMap(N_MAZE);
    //play

}

void exit(bool &running) {
    running = false;
}

void readInst(string& inst) {           // funcion that reads intruction for the nex iteration of the loop
    cout << "Next instruction" << endl; // standart text for read instruction, we must change that
    cin >> inst;                        //need to check input
    cout << endl;
}


int main()   //main function
{
    bool running = true;
    string game_state = "menu"; //the game starts on the menu
    int Inst;

    cout << "Welcome to this game names <inserir nome deste jogo>" << endl << endl; //need to check game's name

    while (running) { // game main loop
        
        if (game_state == "menu") { menu(Inst); menuGameState(Inst, game_state);}

        else if (game_state == "rules") { showRules(game_state); }

        else if (game_state == "play") { play(game_state); }

        else if (game_state == "exit") { exit(running); }


        //readInst(Inst);
    }//end main fuction
}