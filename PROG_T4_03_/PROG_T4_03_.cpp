#include <iostream>
#include <string.h>
using namespace std; // makes all the code more readable

void menu(string &inst) {        // menu function

    cout << "Menu \nPlease choose an option" << endl;
    cout << "1) Rules \n2) Play \n 0) Exit" << endl;
    cin >> inst;    //need to check input
    cout << endl;

}


/* Options function ################################################ */
void showRules() {
    // inserir as regras deste jogo 
    cout << "Rules: \n\n" << "Não sei o que dizer nas regras amigo Dinis!!" << endl; 
}

void play() {
    int N_MAZE;
    cout << "Pick game Maze" << endl;
    cin >> N_MAZE;                        //need to check input


}
void exit() {}
/* ################################################################# */


void readInst(string& inst) {           // funcion that reads intruction for the nex iteration of the loop
    cout << "Next instruction" << endl; // standart text for read instruction, we must change that
    cin >> inst;                        //need to check input
    cout << endl;
}


int main()   //main function
{
    bool running = true;
    string game_state = "menu", Inst;

    cout << "Welcome to this game names <inserir nome deste jogo>" << endl << endl; //need to check game's name

    while (running) { // game main loop
        
        if (game_state == "menu") { menu(Inst); }

        else if (game_state == "rules") { showRules(); }

        else if (game_state == "play") { play(); }

        else if (game_state == "exit") { exit(); }


        readInst(Inst);
    }
}

