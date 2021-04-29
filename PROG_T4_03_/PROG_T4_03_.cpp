//---------------------IMPORT LIBRARIES---------------------------------------------------------------------------------------------

#include <iostream>
#include <string>
#include <fstream> //handle files
#include <stdlib.h>
#include <vector> //vectors
#include <iomanip>
#include <ctime>


using namespace std; // makes all the code more readable

//-------------------FUNCTION DECLARATION------------------------------------------------------------------------------------------

void actionCheck(vector <vector<char>> map, char action, bool& game, string& state, bool& move, int& correct, int& l, int& c); 
void check_eof();
void checkErrors(int correct);
void checkInput(int& variable);
void deadRobotCheck(char nextPos, int& correct, bool& move);
bool empty(string s);
void end(bool& running);
char find_him(vector <int> player_pos, vector <int> R_pos);
void help();
bool is_int(string s);
vector<vector<char>> importMap(int num_map, bool& mapGood);
void kill_stackedRobots(vector <bool>& life, vector <vector<int>> pos, int ID);
void leaderboard(string name, unsigned long int time, int N_MAZE);
void menu(int& inst);
void menuGameState(int Inst, string& gamestate);
void outMap(vector <vector<char>> map, int n_lines, int n_colums);
string pad_str(string num_map, int spaces_to_fill, char filling, bool reverse);
void play(string& state);
void readMap(vector <vector<char>> map, int n_lines, int n_colums, vector <int>& player_pos, vector <vector<int>>& robots_pos);
void showRules(string& state);
bool switch_pos(char& start, char& end);
bool you_lose(vector <vector< char>> map, vector <int> player_pos);
bool you_win(vector <bool> life);

//-----------------------CODE GOES HERE -----------------------------------------------------------------------------------------------

//-----------------------INPUT AND STRING MANIPULATION AND CONTROL---------------------------------------------------------------------

//check_eof() -> ENDS THE GAME IF CTRL-Z (WINDOWS) OR CTRL-D (LINUX)
void check_eof() {if (cin.eof()) {exit(1);}}

//pad_str() -> FILLS WITH A CERTAIN CHAR (filling) ON THE RIGHT OR LEFT (choose with reverse), TO A CERTAIN NUMBER OF CHARACTERS (spaces_to_fill)
string pad_str(string text, int spaces_to_fill = 0, char filling = '0', bool reverse = false){
    
    //reverse = False --> Stuff from left
    if (reverse == false) {
        if (spaces_to_fill > text.length()) { text.insert(text.begin(), spaces_to_fill - text.length(), filling);}
    }
    //reverse = True --> Stuff from right
    else {
        if (spaces_to_fill > text.length()) { text.append(spaces_to_fill - text.length(), filling); }
    }
    
    return text;
}

//is_int() -> RETURNS true IF THE INPUT IS AN INTEGER
//"1   ", "0", "     34     " - VALID (true)
//"1 2", "aaa" - invalid (false)
bool is_int(string s)
{
    int foo;
    //REMOVE SPACES FROM THE LEFT ("    34   " -> "    34")
    for (int i = 0; i < s.size(); i++)
        if (s.at(i) != ' ' && s.at(i) != '\t') { foo = i; break; }
    s = s.substr(foo, s.size() - foo);
    
    //REMOVE SPACES FROM THE RIGHT ("    34" -> "34")
    for (int i = s.size()-1; i >=0; i--) {
        if (s.at(i) != ' ' && s.at(i) != '\t') { foo = i; break; }
    }
    s = s.substr(0, foo +1);

    //CHECKS IF THE CHARACTERS ARE ALL DIGITS ("34" -> ALL DIGITS -> true)
    for (int i = 0; i < s.size(); i++) {
        if (!(isdigit(s.at(i)))) { return false; }
    }
    return true;
}

//empty() -> RETURNS true IF THE LINE IS ONLY MADE UP OF WHITE SPACES ('\t' or ' ')
bool empty(string s) {
    for (int i = 0; i < s.length(); i++)
        if (s.at(i) != ' ' && s.at(i)!= '\t') { return false; }
    return true;
}

//checkInput() -> USES THE FUNCTIONS empty() AND is_int() TO ASK THE USER FOR AN INTEGER
void checkInput(int& variable) {  
    string line;
    bool good = false;
    getline(cin, line);
    check_eof();
    while ((empty(line) || !is_int(line))) {
        if (!empty(line)) { cout << "You wrote something invalid! (We are looking for an integer)" << endl; } 
        getline(cin, line);
        check_eof();

    }
    variable = stoi(line);
}

//-------------------------------------------------------------------------------------------------------------------------------------
//---------------------------GAME FUNCTIONS--------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------

//---------------------------MOVEMENT--------------------------------------------------------------------------------------------------

//switch_pos -> USED TO MOVE THE CHARACTER (ROBOT OR PLAYER) TO THE NEXT PLACE, AND DEFINE WHAT THE NEXT STATE IS GOING TO BE
//RETURNS A BOOL (dead), true FOR DEAD and false FOR ALIVE

bool switch_pos(char& start, char& end) {
    bool dead = true;

    //The if is used to remove the players 's' movement, as no action is taken
    if (!(tolower(end) == tolower(start) && tolower(start) == 'h')){
        //FOR HUMAN
        if (tolower(start) == 'h') {
            //collides with fence '*' and dies
            if (end == '*') { end = tolower(start); } 
            //colides with robot and dies
            else if (tolower(end) == 'r') { end = tolower(start); }
            //dont know...
            //else if (tolower(end) == 'h') {end = tolower(end); }
            //normal move
            else { end = start; dead = false; }
        }
        //FOR ROBOT
        else if (tolower(start) == 'r') {
            //collides with fence '*' and dies
            if (end == '*') { end = tolower(start); }
            //collides with human, kills human
            else if (end == 'H') { end = tolower(end); }
            //collides with robot (dead/alive), dies and stacks
            else if (tolower(end) == 'r') { end = tolower(end); } 
            //normal move
            else { end = start; dead = false; }
        }
        start = ' ';
    }
    
    return dead;
}

//find_him -> USED FOR ROBOTS TO FIND THE HUMAN, AND CHASE HIM

char find_him(vector <int> player_pos, vector <int> R_pos){
    char action;

    //check if they are in the same line
    if (player_pos[0] == R_pos[0]) { 
       if (player_pos[1] < R_pos[1]) { action = 'a'; }
       else { action = 'd'; }
    }
    //check if they are in the same column
    else if (player_pos[1] == R_pos[1]) { 
            if (player_pos[0] > R_pos[0]) { action= 'x'; }
            else { action = 'w'; }
    }
    //if they are not in the same line or column, the movement is done diagonally
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

//actionCheck -> USED FOR HUMAN AND ROBOTS, DOES CHAR TO MOVEMENT AND RETURNS FLAGS WICH INDICATE IF THE MOVEMENT IS VALID (move)
//IN CASE IT ISN'T, THE FLAG CORRECT DECIDES THE ERROR MESSAGE (the error message is used only for the player, as our robots don't do mistakes).

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
        correct = 0;
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

//checkErrors -> PRINTS THE APPROPRIATE ERROR MESSAGE
void checkErrors(int correct) {
    if (correct == 0){}
    else if (correct == 1){cout << "You can't go against dead robots" << endl; }
    else if (correct == 2){cout << "Insert a valid input this time ('h' if you need help)" << endl; } 

}
//-----------------------------MAP-----------------------------------------------------------------------------------------------

//outMap() -> PRINTS MAP IN CONSOLE
void outMap(vector <vector<char>> map, int n_lines, int n_colums) { //outputs the map on the buffer
    for (int l = 0; l < n_lines; l++) {
        for (int c = 0; c < n_colums; c++) {
            cout << map[l][c];
        }
        cout << endl;
    }
}

//readMap() -> SAVES PLAYER POSITION AND THE VARIOUS ROBOTS POSITIONS
void readMap(vector <vector<char>> map, int n_lines,int n_colums,vector <int> &player_pos, vector <vector<int>>& robots_pos) {     

    for (int l = 0; l < n_lines; l++) {
        for (int c = 0; c < n_colums; c++) { 
            if (tolower(map[l][c]) == 'h') { player_pos = { l , c }; }               // take player position
            else if (tolower(map[l][c]) == 'r') { robots_pos.push_back( { l, c }); } // take robots position
        }
    }
}

//importMap() -> SEARCHES FOR MAP, IF IT EXISTS LOADS IT AND RETURNS IT, IF NOT FLAG mapGood = false
vector<vector<char>> importMap(int num_map, bool &mapGood){
    ifstream inStream;

    // 2 -> 02, 1 -> 01, etc...
    string str_num_map = pad_str(to_string(num_map) , 2, '0', false);
   
    string file_name = "MAZE_" + str_num_map + ".txt";
    inStream.open(file_name);

    //NO MAP WITH THIS NUMBER
    if(inStream.fail()){                    
        cout << "There is no map with that number." << endl;
        mapGood = false; //loop to check input
        inStream.close();
        return {}; //kill the fuction
    }
    //WHEN EVERYTHING IS OKAY!
    else{

        mapGood = true;

        //SIZE OF MAP first line on map.txt
        int height, lenght;
        char k;
        inStream >> height >> k >> lenght;

        //INITIALIZE VECTOR
        vector<vector<char>> map_vec(height, vector<char> (lenght, ' '));
        string lines;
        
        //LOAD MAP
        getline(inStream, lines);
        for(int line = 0; line < height; line++){
            getline(inStream, lines);
            for(int colum = 0; colum < lenght; colum++){
                
                map_vec[line][colum] = lines.at(colum);
            }
        }
        inStream.close();
        return map_vec;
    }
}

//---------------------------------------GAME LOGIC-----------------------------------------------------------------------------------

//you_lose() -> IF THE HUMAN IS IN THE DEAD STATE ('h'), YOU HAVE LOST
bool you_lose(vector <vector< char>> map, vector <int> player_pos){ //LOSE CONDITION
    if (map[player_pos[0]][player_pos[1]] == 'h') {
        outMap(map, map.size(), map[0].size());
        return true;
    }
    return false;
}

//you_win() -> IF ALL ROBOTS ARE DEAD, YOU WIN!!!!
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

//deadRobotCheck -> USED TO ACTIVATE FLAG FOR PLAYER MOVEMENT, AS IT CAN'T GO AGAINST DEAD ROBOTS
void deadRobotCheck(char nextPos, int &correct, bool &move){
    if (nextPos == 'r') {correct = 1; move = false; }
}

//kill_stackedRobots -> KILLS STACKED ROBOTS (WHEN TWO ROBOTS ARE IN THE SAME PLACE, THEY ARE DEAD)
void kill_stackedRobots(vector <bool> &life, vector <vector<int>> pos, int ID) {
    for (int c = 0; c < pos.size(); c++) {
        if (c != ID && pos[ID] == pos[c]) {
            life[c] = life[ID];
        }
    }
}

//-----------------------LEADERBORDS---------------------------------------------------------------------------------------------------

//USED FOR LEADERBOARDS
struct Player {
    string name;
    unsigned long int time;
};

//leaderboard() -> LOADS FILE OF WINNERS AND STORES ALL THE DATA
void leaderboard(string name_now, unsigned long int time_now, int N_MAZE) {
    //MAZE_XX_WINNERS.txt

    //1 -> 01 (etc...)
    string str_N_MAZE =  pad_str(to_string(N_MAZE) , 2, '0', false);
    
    str_N_MAZE = "MAZE_" + str_N_MAZE + "_WINNERS.txt";
    
    ifstream win_file_input(str_N_MAZE);

    if (win_file_input){
        //FILE EXIST, OPPENED SUCESSFULLY
        string lines;
        
        //GET RID OF FIST TWO LINES, AS THEY ARE USELESS
        getline(win_file_input, lines);
        getline(win_file_input, lines);

        vector<Player> player_vector;
        Player player;

        while(!win_file_input.eof()){

            getline(win_file_input, lines);

            player.name = lines.substr(0, 15);
            player.time = stoi(lines.substr(18, lines.size() - 18)); //reads to the end of the line

            player_vector.push_back(player);
        }

        //READ DONE
        win_file_input.close();

        //CURRENT PLAYER
        player.name = pad_str(name_now, 15, ' ', true);
        player.time = time_now;

        //INSERTED FLAG
        bool inserted = false;

        //Insert player in correct pos
        for (unsigned long int i = 0; i < player_vector.size(); i++){
            if (time_now < player_vector[i].time){
                player_vector.insert(player_vector.begin() + i, player);
                inserted = true;
                break;
            }
        }
        
        //Player is in last pos
        if (!inserted) {player_vector.push_back(player);}

        //WRITE TO FILE 
        ofstream win_file_output(str_N_MAZE);
        win_file_output << "Player          - Time";
        win_file_output << "\n----------------------";
        for (int c = 0; c < player_vector.size(); c++){
            win_file_output <<"\n"<< player_vector[c].name << " - " << pad_str(to_string(player_vector[c].time), 4, ' ', false);
        }
        win_file_output.close();

        //WRITE TO CONSOLE
        cout << "***********************" << endl;
        cout << "******Leaderboard******" << endl;
        cout << "***********************" << endl;
        cout << endl;
        cout << "!!MAZE " + pad_str(to_string(N_MAZE) , 2, '0', false) + "WINNERS!!" << endl;
        cout << "Player          - Time";
        cout << "\n---------------------";
        for (int c = 0; c < player_vector.size(); c++){
            cout <<"\n"<< player_vector[c].name << " - " << pad_str(to_string(player_vector[c].time), 4, ' ', false);
        }
        cout << "***********************" << endl;
    }
    else{
        //FILE DOES NOT EXIST, THEN IT'S THE RECORD
        //WRITE TO FILE
        ofstream win_file_output(str_N_MAZE);
        win_file_output << "Player          - Time";
        win_file_output << "\n---------------------";
        win_file_output << "\n"<< pad_str(name_now,15, ' ', true) << " - " << pad_str(to_string(time_now), 4, ' ', false);
        win_file_output.close();

        //WRITE TO CONSOLE
        cout << "-----------------------" << endl;
        cout << "------Leaderboard------" << endl;
        cout << "-----------------------" << endl;
        cout << endl;
        cout << "!!MAZE " + pad_str(to_string(N_MAZE) , 2, '0', false) + "WINNERS!!" << endl;
        cout << "Player          - Time";
        cout << "\n---------------------";
        cout << "\n"<< pad_str(name_now,15, ' ', true) << " - " << pad_str(to_string(time_now), 4, ' ', false);
        cout << "\n-----------------------" << endl;
    }
}


//--------------------------------------PLAY FUNCTION------------------------------------------------------------------------------

// play() -> THE JUICE OF OUR PROGRAM, IT IS DESCRIBED AS IT GOES
void play(string& state) {

    int N_MAZE;
    bool mapGood;
    vector<vector<char>> map;

    //IMPORT MAP, DO WHILE MAP IS NOT FOUND
    do{
        cout << "Pick game Maze. Press 0 to return to the menu." << endl;
        checkInput(N_MAZE);     //need to check input
        if (N_MAZE == 0) { state = "menu"; return; } //return to menu
        map = importMap(N_MAZE, mapGood);
    } while (!mapGood);

    
    
    vector<vector<int>> robots_pos;
    vector <int> player_pos;

    //PLAY LOOP
    bool game = true;
    
    string action;
    char action_char;
    
    int new_l, new_c;
    bool move; //checks if he can moves
    int correct; //flag for error message
    
    readMap(map, map.size(), map[0].size(), player_pos, robots_pos); //loads player and robots position
    vector <vector<int>> R_pos = robots_pos;  // ordered pair (lines, colum)
    vector <bool> RLivesMatter; //if robots are dead or alive

    //ALL ROBOTS START ALIVE
    for (int c = 0; c < robots_pos.size(); c++) { RLivesMatter.push_back(true); }

    //START TIMER
    unsigned long int T1 = time(NULL);
    
    while (game) {
        outMap(map, map.size(), map[0].size());

        //game play start here
        cout << "Take action: (press 'h' to get help)  "; 

        move = false;

        //check for input
        while (!move) {
            getline(cin, action);
            check_eof();

            //IF THE USER RIGHTS MORE THAN ONE CHAR, THE ACTION IS WRONG
            if (action.length() > 1) {
                correct = 2;
            }
            else {
                action_char = action.at(0);
                actionCheck(map, action_char, game, state, move, correct, new_l, new_c);
                if (move && correct == 1) {deadRobotCheck(map[player_pos[0] + new_l][player_pos[1] + new_c], correct, move);} //move = false, correct = 1
            }

            if (!move) {
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
            
            //if the robot is alive:
            if (RLivesMatter[ID]) {
                actionCheck(map, find_him(player_pos, R_pos[ID]), game, state, move, correct, new_l, new_c);

                //switch_pos returns true if the robot is dead, false if alive
                if (switch_pos(map[R_pos[ID][0]][R_pos[ID][1]], map[R_pos[ID][0] + new_l][R_pos[ID][1] + new_c])) {
                    RLivesMatter[ID] = false; //switch to dead
                }
                R_pos[ID][0] += new_l;
                R_pos[ID][1] += new_c;

                //if the robot died, you must kill the robots that he collided with
                if (!RLivesMatter[ID]) {
                    kill_stackedRobots(RLivesMatter, R_pos, ID);
                }
            }
        }
        
        //update player_pos
        readMap(map, map.size(), map[0].size(), player_pos, robots_pos); 
        
        //LOSING CONDITION
        if (you_lose(map, player_pos)){
            cout << "YOU LOST, SHAME ON YOU!!!" << endl;
            state == "menu";
            return;
        }

        //SAVE ON FILE RECORD AND SHOW WIN MESSAGE
        if (you_win(RLivesMatter)){
            
            //END TIMER, CALCULATE INTERVAL
            unsigned long int T2 = time(NULL);
            unsigned long int time = T2 - T1;

            outMap(map, map.size(), map[0].size());
            cout << "YOU WON, YOU ARE BLOODY AMAZING MY GUY!" << "  time: "<< time << endl;

            //INPUT NAME
            string name;
            char y_n_char = ' '; 
            bool name_done = false;
            while(!name_done){
                cout << "Qual is o teu name? (max. 15 chars)" << endl; //must change
                getline(cin, name);
                check_eof();

                //CHECK IF NAME SIZE <= 15
                if (name.size() > 15) {
                    cout << "Your name is to big, choose something smaller."<<endl;
                    continue;}
    
                cout << "\nO teu nome is " << name << ", right? (Y/N)" << endl;
                bool done = false;
                
                while (!done) {
                    cin >> y_n_char; 
                    check_eof();
                    cin.ignore(10000, '\n');
                    //ignore if more than one letter
                    cout << endl;
                    //check input
                    if (tolower(y_n_char) == 'y') { name_done = true; done = true; }
                    else if (tolower(y_n_char) == 'n') { name_done = false; done = true; }
                    else { cout << "Y/N"<<endl; done = false; } ;
                }
            }

            leaderboard(name, time, N_MAZE);
            state = "menu";
            return;
        }  
    }
}

//-----------------------------MENUS AND HELP----------------------------------------------------------------------------------------------

//help() -> SHOWS BASIC COMMANDS WHEN PLAYER PRESSES 'h' WHILE PLAYING
void help() {  //outputs possible actions
    cout << "Movement: \n\n" << "NW: Q\t\tN: W\t\tNE: E\n"
    "W: A\t\tSTAY: S\t\tE: D\n"
    "SW: Z\t\tS: X\t\tSE: C\n" << endl;

    cout << "Other commands: \n\n" << "0: Return to menu\n"
    "h: Quick help\n"
    "CRTL-Z (or CRTL-D on Linux): End game\n" << endl;
}

//menuGameState() -> DECYPHERS NUMBERS FROM MENU INTO GAMESTATE
void menuGameState(int Inst, string &gamestate){
    if (Inst == 0) gamestate = "end";
    if (Inst == 1) gamestate = "rules";
    if (Inst == 2) gamestate = "play";
}

//showRules() -> EXTENDED RULES, AVAILABLE ON THE MENU
void showRules(string& state) {
    
    int back = 1;

    //GAME RULES
    cout << "Rules: \n\n" << "The player ('H') is placed in a maze made up of high-voltage fences and posts ('*').\n"
"There are also some interceptor robots ('R') that will try to destroy the player.\n"
"If the player touches the maze or any of these robots, that is the end of the game, and the player ('h')!.\n"
"The robots are also destroyed when they touch the fences/posts or when they collide with each other. \n"
"Every time the player moves in any direction (horizontally, vertically, or diagonally) to a contiguous cell, each robot\n"
"moves one cell closer to the new player's location, in whichever direction is the shortest path.\n"
"The robots have no vision sensors but they have an accurate odour sensor that allows them to follow the player!\n"
"There is one hope: make the robots hit the maze or each other. If all of them are destroyed ('r'), the player wins.\n " << endl;

    cout << "Movement: \n\n" << "NW: Q\t\tN: W\t\tNE: E\n"
    "W: A\t\tSTAY: S\t\tE: D\n"
    "SW: Z\t\tS: X\t\tSE: C\n" << endl;

    cout << "Useful information: \n\n" << "You can leave the game at any given moment by pressing CTRL-Z (or CTRL-D if you are on Linux)\n" << endl;

    while (back != 0) {
        cout << "(Press '0' to exit rules)" << endl;
        checkInput(back); // check input
        if (back == 0) { state = "menu"; }
    }
}

//menu() -> PRINTS MENU AND CHECKS INSTRUCTION
void menu(int &inst) {
    cout << "\n\n------------------------" << endl;
    cout << "----------Menu----------" << endl;
    cout << "------------------------" << endl;
    cout << endl;
    cout << "Please choose an option:" << endl;

    cout << "1) Rules" << endl;
    cout << "2) Play" << endl;
    cout << "0) Exit" << endl;
    cout << endl;

    checkInput(inst);   
    //do it while it is not correct
    while (!(0 <= inst && inst <= 2)) {
        cout << "Invalid input: only '0' , '1' or '2' are accepted" << endl;
        checkInput(inst);   //need to check input
        cout << endl;
    }
}

//end() -> running = false, shuts down program, by ending main loop
void end(bool &running) {
    running = false;
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------


int main()   //main function
{
    bool running = true;
    string game_state = "menu"; //the game starts on the menu
    int Inst;
    cout << "Welcome, to the..." << endl;
    cout << "----------------------------------------------------" << endl;
    cout << "-----------------ROBOTS GAME------------------------" << endl; 
    cout << "----------------------------------------------------" << endl;
    cout << endl;
    
    while (running) { // game main loop
        
        if (game_state == "menu") { menu(Inst); menuGameState(Inst, game_state);}

        else if (game_state == "rules") { showRules(game_state); }

        else if (game_state == "play") { play(game_state); }

        else if (game_state == "end") { end(running); }

    }//end main fuction
}

