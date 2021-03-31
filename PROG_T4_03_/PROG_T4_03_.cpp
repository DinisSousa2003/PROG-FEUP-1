#include <iostream>
#include <string>
using namespace std; // makes all the code more readable


void menuGameState(int Inst, string &gamestate){
    if (Inst == 0) gamestate = "exit";
    if (Inst == 1) gamestate = "rules";
    if (Inst == 2) gamestate = "play";
}

//we can redo this function for strings too...
void checkInput(int &variable){
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
void showRules() {
    // inserir as regras deste jogo 
    cout << "Rules: \n\n" << "I do not know what to say my friend!" << endl; 
    int smth; 
    checkInput(smth); // just to prevent infinite loop, to erase later.
}

void play() {
    int N_MAZE;
    cout << "Pick game Maze. Press 0 to return to the menu." << endl;
    checkInput(N_MAZE);     //need to check input
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

        else if (game_state == "rules") { showRules(); }

        else if (game_state == "play") { play(); }

        else if (game_state == "exit") { exit(running); }


        //readInst(Inst);
    }
}

