#include <iostream>
#include <string>
#include <fstream> //handle files
#include <stdlib.h>
#include <vector> //vectors


using namespace std; // makes all the code more readable

void help() {  //outputs possible actions
    cout << "insert possible actions here (we'll do it later)\n"; //we do it later 
}

void switch_pos(char &a, char &b) {//switch char position on map
    int temp;
    temp = a;
    a = b;
    b = temp;
}

void checkErrors(int correct) {
    if (correct == 0){}
    else if (correct == 1) { cout << "chilla boy u'll die" << endl; }  //text in case certain death
    else if (correct == 2){cout << "Insert a valid input this time morron" << endl; } //in case the user is schtoopid
}

void actionCheck(vector <vector<char>> map, char action, vector<int> pos, bool &game, string &state, bool &move, int &correct ,int &l, int &c) { //check if player can move
    correct = 1;
    move = false;
    switch (tolower(action)) {
    case 'q': 
        if (tolower(map[pos[0] - 1][pos[1] - 1]) != 'r' && map[pos[0] - 1][pos[1] - 1] != '*') { move = true; l = -1; c = -1; }
        break;

    case 'w':
        if (tolower(map[pos[0] - 1][pos[1]]) != 'r' && map[pos[0] - 1][pos[1]] != '*') { move = true; l = -1; c = 0;}
        break;

    case 'e':
        if (tolower(map[pos[0] - 1][pos[1] + 1]) != 'r' && map[pos[0] - 1][pos[1]+1] != '*') { move = true; l = -1; c = 1;}
        break;

    case 'a':
        if (tolower(map[pos[0]][pos[1] - 1]) != 'r' && map[pos[0]][pos[1] - 1] != '*') { move = true; l = 0; c = -1;}
        break;
    case 's':
        if (tolower(map[pos[0]][pos[1]]) != 'r' && map[pos[0]][pos[1]] != '*') {   move = true; l = 0; c = 0;}
        break;

    case 'd':
        if (tolower(map[pos[0]][pos[1] + 1]) != 'r' && map[pos[0]][pos[1] + 1] != '*') { move = true; l = 0; c = 1; }
        break;

    case 'z':
        if (tolower(map[pos[0] + 1][pos[1] - 1]) != 'r' && map[pos[0] + 1][pos[1] - 1] != '*') { move = true; l = 1; c = -1;}
        break;

    case 'x':
        if (tolower(map[pos[0] + 1][pos[1]]) != 'r' && map[pos[0] + 1][pos[1]] != '*') { move = true; l = 1; c = 0; }
        break;

    case 'c':
        if (tolower(map[pos[0] + 1][pos[1] + 1]) != 'r' && map[pos[0] + 1][pos[1] + 1] != '*') { move = true; l = 1; c = 1; }
        break;

    case '0':
        game = false;
        move = true;
        state = "menu";
        //correct = 0;
        break;
    case 'h':
        help();
        move = false;
        correct = 0;
        break;
    default:
        move = false; //change needed
        correct = 2;
        break;
    }
}

void readMap(vector <vector<char>> map, int n_lines,int n_colums,vector <int> &player_pos, vector <vector<int>>& robots_pos) {     //outputs the map on the buffer

    for (int l = 0; l < n_lines; l++) {
        for (int c = 0; c < n_colums; c++) { 
            cout << map[l][c];
            if (tolower(map[l][c]) == 'h') { player_pos = { l , c }; }               // take player position
            else if (tolower(map[l][c]) == 'r') { robots_pos.push_back( { l, c }); } // take robots position
        }
        cout << endl;
    }
}

vector<vector<char>> importMap(int num_map){
    ifstream inStream;
   
    string file_name = "MAPS/MAZE_" + to_string(num_map) + ".txt";
    inStream.open(file_name);

    //NO MAP WITH THIS NUMBER
    if(inStream.fail()){                    //need a loop to check input
        cerr << "There is no map with that number." << endl;
        exit(1);
    }
    //WHEN EVERYTHING IS OKAY!
    else{
        //SIZE OF MAP //first line on map.txt
        int height, lenght;
        char k;
        inStream >> height >> k >> lenght;

        //initialized vector
        vector<vector<char>> map_vec(height, vector<char> (lenght, ' '));
        string lines;
        getline(inStream, lines);
        for(int line = 0; line < height; line++){
            getline(inStream, lines);
            for(int colum = 0; colum < lenght; colum++){
                
                map_vec[line][colum] = lines.at(colum);
            }
        }
        return map_vec;
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
    //system("CLS"); //clears the user's view
    cout << "Menu \nPlease choose an option" << endl;
    cout << "1) Rules \n2) Play \n0) Exit" << endl;
    checkInput(inst);   //need to check input
    cout << endl;
}


/* Options function ################################################ */
void showRules(string& state) {
    //system("CLS"); //clears the user's view
    int back = 1;

    // inserir as regras deste jogo 
    cout << "Rules: \n\n" << "I do not know what to say my friend!" << endl;

    while (back != 0) {
        cout << "(Press '0' to exit rules)" << endl;  //maybe not "0" but works for now
        checkInput(back); // check input
        if (back == 0) { state = "menu"; }
    }
}

void play(string& state) {
    //system("CLS"); //clears the user's view
    int N_MAZE;
    cout << "Pick game Maze. Press 0 to return to the menu." << endl;
    checkInput(N_MAZE);     //need to check input
    if (N_MAZE == 0) { state = "menu"; return ; } //return to menu

    vector<vector<char>> map = importMap(N_MAZE);
    vector<vector<int>> robots_pos;
    vector <int> player_pos;

    //play loop
    bool game = true;
    
    char action;
    bool done; //cin loop
    int new_l, new_c;
    bool move; //checks if he can moves
    int correct; //

    while (game) { 
        //system("CLS"); //clears the user's view
        readMap(map, map.size(), map[0].size(), player_pos, robots_pos);

        //game play start here
        cout << "Take action: (press 'h' to get help)  ";  // 'h' doenst work right now

        done = false;

        //check for input
        while (!done){
            cin >> action;
            actionCheck(map, action, player_pos,game,state , move,correct, new_l, new_c );
            
            if (move) { done = true; }
            else{
                //system("CLS"); //clears the user's view
                readMap(map, map.size(), map[0].size(), player_pos, robots_pos);
                checkErrors(correct);
            }
        }
    if (!game) break; //checks if it is to continue 
     
    switch_pos(map[player_pos[0]][player_pos[1]], map[player_pos[0] + new_l][player_pos[1] + new_c]);  //switch position between player(H) and the new position
    //finished player movement

        
        
        
    }
    
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

