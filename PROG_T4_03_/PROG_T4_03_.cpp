#include <iostream>
#include <string>
#include <fstream> //handle files
#include <stdlib.h>
#include <vector> //vectors
#include <iomanip>
#include <cmath> //pow and sqrt


using namespace std; // makes all the code more readable

void help() {  //outputs possible actions
    cout << "Movement: \n\n" << "NW: Q\t\tN: W\t\tNE: E\n"
    "W: A\t\tSTAY: S\t\tE: D\n"
    "SW: Z\t\tS: X\t\tSE: C\n" << endl;

    cout << "Other commands: \n\n" << "0: Return to menu\n"
    "h: Quick help\n"
    "CRTL-Z (or CRTL-D on Linux): End game\n" << endl;
}

void switch_pos(char &a, char &b) {//switch char position on map
    int temp;
    temp = a;
    a = b;
    b = temp;
}

void checkErrors(int correct) {
    if (correct == 0){}
    else if (correct == 1) { cout << "U cannot go against the dead robots, choose anotha" << endl; }  //text in case certain death
    else if (correct == 2){cout << "Insert a valid input this time morron" << endl; } //in case the user is schtoopid
    else if (correct == 3){cout << "You dead u nerd" << endl;}
}

void actionCheck(vector <vector<char>> map, char action, vector<int> pos, bool &game, string &state, bool &move, int &correct ,int &l, int &c) { //check if player can move
    correct = 0;
    move = false;
    switch (tolower(action)) {
    case 'q': 
        if (tolower(map[pos[0] - 1][pos[1] - 1]) != 'r' && map[pos[0] - 1][pos[1] - 1] != '*') { move = true; l = -1; c = -1;}
        else if((map[pos[0] - 1][pos[1] - 1]) == 'r') {move = false; correct = 1;}
        else { move = true; game = false; state = "menu"; correct = 3; }
        break;

    case 'w':
        if (tolower(map[pos[0] - 1][pos[1]]) != 'r' && map[pos[0] - 1][pos[1]] != '*') { move = true; l = -1; c = 0; }
        else if((map[pos[0] - 1][pos[1]]) == 'r') {move = false; correct = 1;}
        else {move = true; game = false; state = "menu"; correct = 3 ;}
        break;

    case 'e':
        if (tolower(map[pos[0] - 1][pos[1] + 1]) != 'r' && map[pos[0] - 1][pos[1]+1] != '*') { move = true; l = -1; c = 1;}
        else if((map[pos[0] - 1][pos[1] + 1]) == 'r') {move = false; correct = 1;}
        else {move = true; game = false; state = "menu"; correct = 3 ;}
        break;

    case 'a':
        if (tolower(map[pos[0]][pos[1] - 1]) != 'r' && map[pos[0]][pos[1] - 1] != '*') { move = true; l = 0; c = -1;}
        else if((map[pos[0]][pos[1] - 1]) == 'r') {move = false; correct = 1;}
        else {move = true; game = false; state = "menu"; correct = 3; }
        break;
    case 's':
        if (tolower(map[pos[0]][pos[1]]) != 'r' && map[pos[0]][pos[1]] != '*') {   move = true; l = 0; c = 0;}
        else if((map[pos[0]][pos[1]]) == 'r') {move = false; correct = 1;}
        else { move = true; game = false; state = "menu"; correct = 3; }
        break;

    case 'd':
        if (tolower(map[pos[0]][pos[1] + 1]) != 'r' && map[pos[0]][pos[1] + 1] != '*') { move = true; l = 0; c = 1; }
        else if((map[pos[0]][pos[1] + 1]) == 'r') {move = false; correct = 1;}
        else { move = true; game = false; state = "menu"; correct = 3; }
        break;

    case 'z':
        if (tolower(map[pos[0] + 1][pos[1] - 1]) != 'r' && map[pos[0] + 1][pos[1] - 1] != '*') { move = true; l = 1; c = -1;}
        else if((map[pos[0] + 1][pos[1] - 1]) == 'r') {move = false; correct = 1;}
        else { move = true; game = false; state = "menu"; correct = 3; }
        break;

    case 'x':
        if (tolower(map[pos[0] + 1][pos[1]]) != 'r' && map[pos[0] + 1][pos[1]] != '*') { move = true; l = 1; c = 0; }
        else if((map[pos[0] + 1][pos[1]]) == 'r') {move = false; correct = 1;}
        else { move = true; game = false; state = "menu"; correct = 3; }
        break;

    case 'c':
        if (tolower(map[pos[0] + 1][pos[1] + 1]) != 'r' && map[pos[0] + 1][pos[1] + 1] != '*') { move = true; l = 1; c = 1; }
        else if((map[pos[0] + 1][pos[1] + 1]) == 'r') {move = false; correct = 1;}
        else { move = true; game = false; state = "menu"; correct = 3; }
        break;

    case '0':
        game = false;
        move = true;
        state = "menu";
        correct = 0;
        break;
    case 'h':
        help();
        move = false;
        correct = 0;
        break;
    default:
        move = false; 
        correct = 2;
        break;
    }
}

void outMap(vector <vector<char>> map, int n_lines, int n_colums) { //outputs the map on the buffer
    for (int l = 0; l < n_lines; l++) {
        for (int c = 0; c < n_colums; c++) {
            cout << map[l][c];
        }
        cout << endl;
    }
}

void readMap(vector <vector<char>> map, int n_lines,int n_colums,vector <int> &player_pos, vector <vector<int>>& robots_pos) {     //only reads the map

    for (int l = 0; l < n_lines; l++) {
        for (int c = 0; c < n_colums; c++) { 
            //cout << map[l][c];
            if (tolower(map[l][c]) == 'h') { player_pos = { l , c }; }               // take player position
            else if (tolower(map[l][c]) == 'r') { robots_pos.push_back( { l, c }); } // take robots position
        }
        //cout << endl;
    }
    //outMap(map, n_lines, n_colums);
}


void pad_str(string &num_map, int spaces_to_fill = 0, char filling = '0'){ 
    
    if (spaces_to_fill > num_map.length())
      num_map.insert(num_map.begin(), spaces_to_fill - num_map.length(), filling);
}

vector<vector<char>> importMap(int num_map){
    ifstream inStream;

    string str_num_map = to_string(num_map);
    pad_str(str_num_map, 2, '0');
   
    string file_name = "MAPS/MAZE_" + str_num_map + ".txt";
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

void checkInput(int &variable){ 
    cin >> variable;
    if (cin.fail()){
        if (cin.eof()){
            cin.clear(); //clears error flags
            cin.ignore(10000, '\n'); //clears buffer
            cout << "FATAL ERROR AHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH";
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
    cout << "1) Rules \n2) Play \n0) Exit" << endl << endl;
    checkInput(inst);   //need to check input
    cout << endl;
}


/* Options function ################################################ */
void showRules(string& state) {
    //system("CLS"); //clears the user's view
    int back = 1;

    // inserir as regras deste jogo 
    cout << "Rules: \n\n" << "The player is placed in a maze made up of high-voltage fences and posts. There are also some interceptor robots that \n"
"will try to destroy the player. If the player touches the maze or any of these robots, that is the end of the game (and\n"
"the player!). The robots are also destroyed when they touch the fences/posts or when they collide with each other.\n"
"Every time the player moves in any direction (horizontally, vertically, or diagonally) to a contiguous cell, each robot\n"
"moves one cell closer to the new player's location, in whichever direction is the shortest path. The robots have no\n"
"vision sensors but they have an accurate odour sensor that allows them to follow the player!\n"
"There is one hope: make the robots hit the maze or each other. If all of them are destroyed, the player wins.\n " << endl;

    cout << "Movement: \n\n" << "NW: Q\t\tN: W\t\tNE: E\n"
    "W: A\t\tSTAY: S\t\tE: D\n"
    "SW: Z\t\tS: X\t\tSE: C\n" << endl;

    cout << "Useful information: \n\n" << "You can leave the game at any given moment by pressing CTRL-Z (or CTRL-D if you are on Linux)\n" << endl;

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
    
    string action;
    char action_char;
    bool done; //cin loop
    int new_l, new_c;
    bool move; //checks if he can moves
    int correct; 

    readMap(map, map.size(), map[0].size(), player_pos, robots_pos);
    vector <vector<int>> R_pos = robots_pos;  // ordered pair (lines, colum)
    vector <bool> RLivesMatter;
    for (int c = 0; c < robots_pos.size(); c++){RLivesMatter.push_back(true);}

    while (game) { 
        readMap(map, map.size(), map[0].size(), player_pos, robots_pos);
        outMap(map, map.size(), map[0].size());

        //game play start here
        cout << "Take action: (press 'h' to get help)  ";  // 'h' doenst work right now

        done = false;

        //check for input
        while (!done){
            cin >> action;
            cin.ignore(10000, '\n');
            if (action.length() > 1){
                correct = 2;
                move = false;
            }
            
            else{
            action_char = action.at(0);
            actionCheck(map, action_char, player_pos, game, state , move, correct, new_l, new_c );
            }
            
            if (move) { done = true; }
            else{
                //readMap(map, map.size(), map[0].size(), player_pos, robots_pos);
                outMap(map, map.size(), map[0].size());
            }
            checkErrors(correct);
        }
    if (!game) break; //checks if it is to continue 
 
    switch_pos(map[player_pos[0]][player_pos[1]], map[player_pos[0] + new_l][player_pos[1] + new_c]);  //switch position between player(H) and the new position
    //finished player movement
    
    for (int ID = 0; ID < RLivesMatter.size(); ID++) {
        if (player_pos[0] == R_pos[ID][0]) { //check if they are in the same line
            if (player_pos[1] < R_pos[ID][1]) { action_char = 'a'; }
            else { action_char = 'd'; }
        }  

        else if (player_pos[1] == R_pos[ID][1]) { //check if they are in the same colum
            if (player_pos[0] > R_pos[ID][0]) { action_char = 'x'; }
            else { action_char = 'w'; }
        }  
        else if (player_pos[0] > R_pos[ID][0]){
            if (player_pos[1] < R_pos[ID][1]){ action_char = 'q';}
            else {action_char = 'e';}
        }
        else{
            if (player_pos[1] < R_pos[ID][1]){ action_char = 'z';}
            else {action_char = 'c';}
        }

        actionCheck(map, action_char, R_pos[ID], game, state, move, correct, new_l, new_c);

    }
    }
    
    //play

}

void exit(bool &running) {
    running = false;
}

// void readInst(string& inst) {           // funcion that reads intruction for the nex iteration of the loop
//     cout << "Next instruction" << endl; // standart text for read instruction, we must change that
//     cin >> inst;                        //need to check input
//     cout << endl;
// }


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

