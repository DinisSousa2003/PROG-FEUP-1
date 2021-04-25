#include <iostream>
#include <string>
#include <fstream> //handle files
#include <stdlib.h>
#include <vector> //vectors
#include <iomanip>


using namespace std; // makes all the code more readable

void outMap(vector <vector<char>> map, int n_lines, int n_colums);



void help() {  //outputs possible actions
    cout << "Movement: \n\n" << "NW: Q\t\tN: W\t\tNE: E\n"
    "W: A\t\tSTAY: S\t\tE: D\n"
    "SW: Z\t\tS: X\t\tSE: C\n" << endl;

    cout << "Other commands: \n\n" << "0: Return to menu\n"
    "h: Quick help\n"
    "CRTL-Z (or CRTL-D on Linux): End game\n" << endl;
}

bool switch_pos(char& start, char& end) {//switch char position on map
    bool dead = true;

    //The if is used to remove the players 's' movement, as no action is taken

    if (!(tolower(end) == tolower(start) && tolower(start) == 'h')){
        //FOR HUMAN
        if (tolower(start) == 'h') {
            if (end == '*') { end = tolower(start); }
            else if (tolower(end) == 'r') { end = tolower(start); }
            else if (tolower(end) == 'h') {end = tolower(end); }
            else { end = start; dead = false; }
        }
        //FOR ROBOT
        else if (tolower(start) == 'r') {
            if (end == '*') { end = tolower(start); }
            else if (end == 'H') { end = tolower(end); }//will be h
            else if (tolower(end) == 'r') { end = tolower(end); } //will be r
            else { end = start; dead = false; }
        }
        start = ' ';
    }
    
    return dead;
}
char find_him(vector <int> player_pos, vector <int> R_pos){
    char action;
    if (player_pos[0] == R_pos[0]) { //check if they are in the same line
       if (player_pos[1] < R_pos[1]) { action = 'a'; }
       else { action = 'd'; }
    }
    else if (player_pos[1] == R_pos[1]) { //check if they are in the same colum
            if (player_pos[0] > R_pos[0]) { action= 'x'; }
            else { action = 'w'; }
    }
    else if (player_pos[0] > R_pos[0]) {
            if (player_pos[1] < R_pos[1]) { action= 'z'; }
            else { action = 'c'; }
    }
    else {
            if (player_pos[1] < R_pos[1]) { action = 'q'; }
            else { action = 'e'; }
    }
    return action;
}

void checkErrors(int correct) {
    if (correct == 0){}
    else if (correct == 1) { cout << "chilla boy u'll die" << endl; }  //text in case certain death
    else if (correct == 2){cout << "Insert a valid input this time morron" << endl; } //in case the user is schtoopid
}
bool you_lose(vector <vector< char>> map, vector <int> player_pos){ //LOSE CONDITION
    if (map[player_pos[0]][player_pos[1]] == 'h') {
        outMap(map, map.size(), map[0].size());
        return true;
    }
    return false;
}
bool you_win(vector <bool> life) {
    bool win = true;

    //CHECK IF ALL ROBOTS ARE DEAD
    for (int ID = 0; ID < life.size(); ID++) {
        if (life[ID] == true) {
            win = false;
        }
    }
    return win;
}

void actionCheck(vector <vector<char>> map, char action,  bool &game, string &state, bool &move, int &correct ,int &l, int &c) { //check if player can move
    correct = 1;
    move = true;
    switch (tolower(action)) {
    case 'q': 
        l = -1; c = -1;
        break;

    case 'w':
        l = -1; c = 0;
        break;

    case 'e':
        l = -1; c = 1;
        break;

    case 'a':
        l = 0; c = -1;
        break;
    case 's':
        l = 0; c = 0;
        break;

    case 'd':
        l = 0; c = 1;
        break;

    case 'z':
        l = 1; c = -1;
        break;

    case 'x':
        l = 1; c = 0;
        break;

    case 'c':
        l = 1; c = 1;
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
    default: //invalid input
        move = false; //change needed
        correct = 2;
        break;
    }
    
}

void remove_copy(vector <bool> &life, vector <vector<int>> pos, int ID) {
    for (int c = 0; c < pos.size(); c++) {
        if (c != ID && pos[ID] == pos[c]) {
            life[c] = life[ID];
        }
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

void readMap(vector <vector<char>> map, int n_lines,int n_colums,vector <int> &player_pos, vector <vector<int>>& robots_pos) {     

    for (int l = 0; l < n_lines; l++) {
        for (int c = 0; c < n_colums; c++) { 
            if (tolower(map[l][c]) == 'h') { player_pos = { l , c }; }               // take player position
            else if (tolower(map[l][c]) == 'r') { robots_pos.push_back( { l, c }); } // take robots position
        }
    }
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

//we can redo this function for strings too...
void checkInput(int &variable){  //maybe redo this function with a loop instead recursion
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
    
    int new_l, new_c;
    bool move; //checks if he can moves
    int correct; //
    
    readMap(map, map.size(), map[0].size(), player_pos, robots_pos);
    vector <vector<int>> R_pos = robots_pos;  // ordered pair (lines, colum)
    vector <bool> RLivesMatter;

    for (int c = 0; c < robots_pos.size(); c++) { RLivesMatter.push_back(true); }
    
    while (game) {
        //readMap(map, map.size(), map[0].size(), player_pos, robots_pos);
        outMap(map, map.size(), map[0].size());

        //game play start here
        cout << "Take action: (press 'h' to get help)  ";  // 'h' doenst work right now

        move = false;

        //check for input
        while (!move) {
            cin >> action;
            cin.ignore(10000, '\n');
            if (action.length() > 1) {
                correct = 2;
                move = false;
            }
            else {
                action_char = action.at(0);
                actionCheck(map, action_char, game, state, move, correct, new_l, new_c);
            }

            if (!move) {
                //readMap(map, map.size(), map[0].size(), player_pos, robots_pos);
                outMap(map, map.size(), map[0].size());
                checkErrors(correct);
            }
        }
        if (!game) break; //checks if it is to continue 

        switch_pos(map[player_pos[0]][player_pos[1]], map[player_pos[0] + new_l][player_pos[1] + new_c]);  //switch position between player(H) and the new position
        readMap(map, map.size(), map[0].size(), player_pos, robots_pos); //update player_pos
        
        //finished player movement

        //ROBOTS MOVEMENT
        for (int ID = 0; ID < RLivesMatter.size(); ID++) {
            /*cout << "  {"<<R_pos[ID][0] << " , " << R_pos[ID][0] << "}  " << endl;*/
            if (RLivesMatter[ID]) {
                actionCheck(map, find_him(player_pos, R_pos[ID]), game, state, move, correct, new_l, new_c);
                if (switch_pos(map[R_pos[ID][0]][R_pos[ID][1]], map[R_pos[ID][0] + new_l][R_pos[ID][1] + new_c])) {
                    RLivesMatter[ID] = false;
                }
                R_pos[ID][0] += new_l;
                R_pos[ID][1] += new_c;
                if (!RLivesMatter[ID]) {
                    remove_copy(RLivesMatter, R_pos, ID);
                }
            }
        }
        
        readMap(map, map.size(), map[0].size(), player_pos, robots_pos); //update player_pos
        
      
        //LOSING CONDITION
        if (you_lose(map, player_pos)){
            cout << "YOU LOST, SHAME ON YOU!!!" << endl;
            state == "menu";
            return;
        }

        //SAVE ON FILE RECORD AND SHOW WIN MESSAGE
        if (you_win(RLivesMatter)){
            cout << "YOU WON, YOU ARE BLOODY AMAZING MY GUY!" << endl;
            //leaderboard();
            state == "menu";
            return;
        }  
    }
}
void exit(bool &running) {
    running = false;
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

