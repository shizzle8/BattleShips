#include <iostream>
#include <stdlib.h>



using namespace std;
#define MAX_SHIPS 5


class Ship {
    private:
        // ship size
        int size;
        // counts the number of hits
        int hits;
        string type;
    public:
        Ship(int size, string type){
            this->size = size;
            this->type = type;
        }
        /**
            Checks if the ship has sunk (the hits must be the size of the ship to blow up)
        **/
        bool isSunk(){
            return (this->hits >= this->size);
        }
        /**
            Getter for size
        **/
        int getSize(){
            return this->size;
        }
        /**
            Getter for type
        **/
        string getType(){
            return this->type;
        }
        /**
            increments hit by once since ship has been hit.
        **/
        void hit(){
            // Preventing it to hit when the ship is blown up
            if(this->hits < this->size){
                this->hits = this->hits + 1;
            }
        }
};
class BattleshipSquare{
    private:
        Ship *ship;
        bool isFired;
    public:
        BattleshipSquare(){
            this->ship = NULL;
            this->isFired = false;
        }
        void reset(){
            this->isFired = false;
            this->ship = NULL;
        }
        bool addShip(Ship *ship){
            // checking if the ship is free
            if(this->ship == NULL){
                this->ship = ship;
                return true;
            }
            // Ship is alreay occupied please reset first
            else{
                return false;
            }
        }
        char getStatus(){


            if((this->ship == NULL) && (this->isFired == true)){
                return 'M';
            }
            else if (!(this->ship == NULL) && (this->isFired == false)){
                return 'S';
            }
            else if (!(this->ship == NULL) && (this->isFired == true)){
                return 'H';
            }
            else{
                return '-';
            }

        }
        /**
            Hitting the ship by calling the hit() function;
        **/
        void fire(){
            this->isFired = true;
            if(this->ship != NULL){
                this->ship->hit();
            }
        }
};



class Board {
    private:
        BattleshipSquare squares [8][8];


    public:

        Board(){   // board constructor
           for(int i = 0; i<8; i++){
               for(int j = 0; j<8; j++){
                   this->squares[i][j].addShip(NULL);
               }
           }
        }
    // this will just display the ships board
        void displayboard(){
            cout<< "   ";
            char alpha[8]={'A','B','C','D','E','F','G'};
            char alphaH = 'H';
            for(int x = 0; x<7;x++){

            cout<< alpha[x]<<"--";
            }
            cout<<alphaH;
            cout<< "\n";
            cout<<"  ";
            for(int y=0; y<8; y++){
            cout<< "---";

            }
            cout<< "\n";                                // This will print the gameboard final view, which will show the ships as char 'S'
            int t = 0;
            for(int i = 0; i<8;i++){
            cout << i+1 <<"| " ;
            for(int j = 0; j<8; j++){
                cout << this->squares[i][j].getStatus() << "  ";
            }
            cout << endl;
            }

    }
    int getSquareState(int x, int y){
        if(squares[y][x].getStatus() == 'M'){
            return 3;
        }
        else if(squares[y][x].getStatus() == 'H'){
            return 2;
        }
        else if(squares[y][x].getStatus() == 'S'){
            return 1;

        }
        /** Must be an error | empty so lets leave it as empty on the board**/
        else{
            return 0;

        }
    }
    bool validateShips(int x, int y, int direction, Ship * ship){
        bool isValid = false;
        /** If east and it go out of bounds **/
        if(getSquareState(x,y) == 1){
            return false;
        }
        if((direction == 2) && (x+(ship->getSize() - 1)<8)){
            /** Looping through the ship size and see if there is any collisions **/
            for(int i = 1; i<=ship->getSize(); i++){
                /** Checking if there is a ship already then return true **/
                if(!(getSquareState(x,y) == 1)){
                    isValid = true;
                }
                x = x + 1;
            }
        }
        /** If direction is west and dodesn't go out of bounds on the left **/
        else if(direction == 4 && (x+(ship->getSize() - 1)>=0)){
             /** Looping through the ship size and see if there is any collisions **/
            for(int i = 1; i<=ship->getSize(); i++){
                if(!(getSquareState(x,y) == 1)){
                    isValid = true;
                }
                x = x - 1;
            }
        }

        /** If direction is north we check it doesn't go out of bounds north **/
        else if(direction == 1 && (y-(ship->getSize()-1)>=0) ){
             /** Looping through the ship size and see if there is any collisions **/
             for (int i=1; i<=ship->getSize(); i++ ){
                if(!(getSquareState(x,y) == 1)){
                    isValid = true;
                }
                y=y-1;
             }
        }
                /** If direction is south we check it doesn't go out of bounds north **/
        else if(direction == 3 && (y+(ship->getSize()-1)<8) ){
             for (int i=1; i<=ship->getSize(); i++ ){
                 if(!(getSquareState(x,y) == 1)){
                    isValid = true;
                }
             }
             y = y + 1;
        }
        return isValid;

    }


    /**
        Places down a ship onto the board. Calls validation from above.
    **/
    bool placeShip (int x, int y, int direction, Ship * ship){
        /** If validation fails return false **/
        if(validateShips(x,y,direction, ship) == false){
            return false;
        }
        else{
            /** If direction is East place ships **/
            if(direction == 2){
                this->squares[y][x].addShip(ship);
                /** Loop through subsequent square **/
                for(int i = 1; i<ship->getSize(); i++){
                    /** Setting initial spot as the ship **/
                    x = x + 1;
                    this->squares[y][x].addShip(ship);
                }
                return true;

            }

            /** If directions is West place the ships **/
            else if(direction == 4){
                this->squares[y][x].addShip(ship);
                for(int i = 1; i<ship->getSize();i++){
                    x = x -1;
                    this->squares[y][x].addShip(ship);

                }
                return true;
            }
            /** If direction is North place the ships **/
            else if(direction == 1){
                this->squares[y][x].addShip(ship);
                for(int i = 1; i<ship->getSize();i++){
                     y = y -1;
                    this->squares[y][x].addShip(ship);
                }
                return true;
            }

            /** If direction is South place the ships **/
            else if(direction == 3){
                this->squares[y][x].addShip(ship);
                for(int i = 1; i<ship->getSize();i++){
                    y = y + 1;
                    this->squares[y][x].addShip(ship);
                }
                return true;
            }
            /** Unknown Error occured **/
            else{
                return false;
            }


        }
    }
    bool fireAtSquare (int x, int y){
        /** Checking if its a Ship that is not sunk or empty **/
        if((this->squares[y][x].getStatus() == 'S') || (this->squares[y][x].getStatus() == '-')){
            /** Firing at the ship **/
            this->squares[y][x].fire();
            /** Ship is not sunk so we can hit it. Return true **/
            return true;
        }
        /** Return false since the if statement is not reached **/
        return false;
    }
    void resetBoard(){
        /** Loop through board and replace with empty instances of BattleshipSquare **/
        for(int i = 0; i<8 ;i++){
            for(int j = 0; j<8;j++){
                BattleshipSquare empty;
                squares[j][i] = empty;
            }
        }
    }

};


// this player class just represents the players of the game allowing us to create multiple player objects
class Player {

    private:
        string name;
        Board board;
        Ship *ships[5];

    public:
        /** Constructor to add ships into this->ships **/
        Player(){
            string shipName[5] = {"Destroyer","Submarine","Cruiser","Battleship","Carrier"};
            int sizeShip[5] = {2,3,3,4,5};
            for(int i = 0; i<5; i++){
                Ship * ship = new Ship(sizeShip[i],shipName[i]);
                this->ships[i] = ship;
            }
        }
        // calls the board function to display the baord
        void displayBoard(){
            this->board.displayboard();
        }
        //assigns the name
        void setName(string name){
            this->name = name;
        };
        // retrieves the name and returns it
        string getName(){
            return this->name;
        }
        // this will fire at the board whilst checking if the board has been hit or missed
        bool fire(int x, int y){
           if(this->board.fireAtSquare(x,y)){
                return true;
           }
           else {return false;}
        }
        // calls the placeship function in board class
        bool placeShip(int x, int y, int direction, int idx){
            if(this->board.placeShip(x,y,direction,this->ships[idx])){
                return true;
            }else{
                return false;

            }
        }
        // this will check if the given player has won the game using the shipleft as an indicator to check if there are ships left
        bool checkLost(){
            /** Counter for deadd ships **/
            int deadShips = 0;
            /** Loop through all ships **/
            for(int i = 0; i<5; i++){
                /** If the ship is sunk increment counter **/
                if(this->ships[i]->isSunk()){
                    deadShips = deadShips + 1;
                }
            }
            /** If ships are sunk return true otherwise return false **/
            return (deadShips >= 5);

        }

        // displays the visual hitboard
        void displayHitBoard(){
                        cout<< "   ";
            char alpha[8]={'A','B','C','D','E','F','G'};
            char alphaH = 'H';
            for(int x = 0; x<7;x++){

            cout<< alpha[x]<<"--";
            }
            cout<<alphaH;
            cout<< "\n";
            cout<<"  ";
            for(int y=0; y<8; y++){
            cout<< "---";

            }
            cout<< "\n";                                // This will print the gameboard final view, which will show the ships as char 'S'
            int t = 0;
            for(int i = 0; i<8;i++){
                cout << i+1 <<"| " ;
                // using the statesquares we can determine whether its board is empty, has a ship, is hit, or is missed
                for(int j = 0; j<8; j++){
                    if (this->board.getSquareState(j,i)==0){
                        cout << '-' << "  ";
                    }
                    else if(this->board.getSquareState(j,i)==1){
                        cout << '-' << "  ";
                    }
                    else if(this->board.getSquareState(j,i)==2) {
                        cout << 'H' << "  ";
                    }
                    else if(this->board.getSquareState(j,i)==3) {
                        cout << 'M' << "  ";
                    }
            }
            cout << endl;


            }
        }
        // this is validate if the hitboard has not been hit twice on the same square
        bool hitValidate(int xcord, int ycord){
            if(this->board.getSquareState(xcord,ycord)==2||this->board.getSquareState(xcord,ycord)==3){
                return true;
            }
            else{return false;}

        }
        /** Getter for a specified ship **/
        Ship* getShip(int idx){
            return this->ships[idx];
        }

};

void menuArt();
void addShip (Player &player);
void show_menu(Player &player1, Player &player2);
void get_coordinatesFire(Player &player1, Player &player2);
void enterToContinue();

int main() {
    // Board b;
    // Player p;
    // b.displayboard();
   Player player1;  // creating my player objects
    Player player2;
    menuArt();
    show_menu(player1, player2); // allow the players to enter their names

    addShip(player1); // allows the players to enter their ships
    addShip(player2);
    get_coordinatesFire(player1,player2); // allows separate players to hit against each others boards

   }

// -----------FUNCTIONS------------------



void show_menu(Player &player1, Player &player2){            // this function will just display the main menu (if you want to start the game and the player objects name) passed through reference
    string inputMenu;
    string name;
    cout<< "     1. Start Game"<<endl;
    cout<< "     2. Exit Game \n"<< endl;
    while(true){
        cout<< "Enter menu selection (1-2): ";
        getline(cin, inputMenu);    // getline stores characters included spaces into the inputmenu variable this solves the spaces problem


         if (inputMenu=="2"){
            cout <<"\n";
            cout<<"Goodbye!"<<endl;
            exit(0);                  // this if statement will return true and exit if the condition of '2' is met
        }

        else if(inputMenu=="1"){         // this will exit the loop when the player inputs 1 and continue with the game menu
            cout << "\x1B[2J\x1B[H"<<endl; // To clear BASH terminal
            break;
        }

        else {
            cout<< "Invalid entry! Please try again."<<endl;  // if anything else is inputted then the loop restarts and a error is outputted
        }
    }
    cout<< "** Enter player1 name: ";               // This asks and stores the players name
    getline(cin,name);
    player1.setName(name);
    cout<<"\n";
    cout << "\x1B[2J\x1B[H"<<endl;
    cout<< "** Enter player2 name: ";               // This asks and stores the players name
    getline(cin,name);
    player2.setName(name);
    cout<<"\n";
    cout << "\x1B[2J\x1B[H"<<endl; // To clear BASH terminal


}


void menuArt(){
    cout<<"--------------------------------------------- \n";
    cout<<"         Battleships  \n";
    cout<<"         Main Menu \n";
    cout<<"---------------------------------------------\n";
}
// gets the players inputs to get ready to fire
void get_coordinatesFire(Player &player1, Player &player2){
    bool overLaphit1; // a boolean that returns true if a hit square is hit again
    string xcord;
    string ycord;
    int turn=1;             // declared 2 variables that stores a string and a character
    char xchord_char;       //   A string is used allowing multi character to be inserted without causing the loop error and the char stores in the first character array of the string
    while(true){// creating a for loop for 2 turns of firing
           Player *selectedPlayer; // because we are storing
           Player *otherPlayer;
           if(turn % 2 == 0){
            selectedPlayer = &player1; // storing the memory address from the parameters into the new variables
            otherPlayer = &player2;
           }else{
            selectedPlayer = &player2;
            otherPlayer = &player1;
           }


           if(player1.checkLost()){
            cout<<"\n";
            cout<< "Player "<<player2.getName()<< " you won!"<<endl;
            break;
           }
           if(player2.checkLost()){
            cout<<"\n";
            cout<< "Player "<<player1.getName()<< " you won!"<<endl;
            break;
           }
            cout<<"\n";
            cout<<"Turn: "<<turn<<endl;  // 17 is the number of squares being used so after 17 hits you win
            cout<<"Player "<< selectedPlayer->getName()<<" enter coordinates to fire \n \n";  // playerName function returns the name, since its the memory addresses passed through the parameters we must use ->
            otherPlayer->displayHitBoard();


        while(true){
            while (true){
                cout<<"\n";
                cout<< "Enter the x coordinate (A-H): ";
                getline(cin,xcord);
                xchord_char = toupper(xcord[0]);        // this will take the first character of the string
                int xcord_asc = int(xchord_char);    // this will convert the first character into a dec, for the ascii table

                if((xcord.length()!= 1) ||!((((97<=xcord_asc) && (xcord_asc<=104)) ||((65<=xcord_asc) && (xcord_asc<=72)))) ){ // this condition will check if the input length is not 1 and if the first character does not lie between the ascii character number
                                                                                                // this checks if is between both lower-case and upper-case a-h A-H
                    cout<< "Invalid entry! Please try again."<<endl;  // if the condition is met it will do the loop again with the invalid entry message
                }

                else {

                    break;  // when the player inputs a single character which lies between A-H the loop breaks and continues the game menu
                }
        }

                 // similarly we do this with the y cord
            char ychord_char;

            while (true){
                cout<< "Enter the y coordinate (1-8): ";
                getline(cin,ycord);
                ychord_char = ycord[0];

                int ycord_asc = int(ychord_char);      // this gets its ascii value

                if((ycord.length()!= 1) ||(!(((49<=ycord_asc) && (ycord_asc<=56)))) ){ // I use the ascii value and the length to determine if the input is valid if its between 1-8

                    cout<< "Invalid entry! Please try again."<<endl;  // if invalid the loop starts again
                }

                else {

                    break;
                }

            }

            char charX= toupper(xcord[0]);

            int xcord_asc = int(charX)-65;  // similarly as the gameboard function this function will just update the table

            char charY= ycord[0];           // when a square is hit or missed

            int ycord_asc = (charY - '0')-1;
            if(otherPlayer->hitValidate(xcord_asc,ycord_asc)==false){ // does the validation check
                    if(otherPlayer->fire(xcord_asc,ycord_asc)){ // the memory addresses passed through the parameters we must use ->
                        cout << "It's a hit!"<<endl;
                    }else{
                        cout << "It's a miss!";
                    }
                    break;
            }
            else{
                    cout<<"Cannot shoot the same square twice, try again!"<<endl;
            }

            }
            cout<<"\n";
            enterToContinue();
            turn++; // increase the turns once there has been a successful input from the user
        }
        cout<< "\n";

    }



void get_coordinates(string &xcord,string &ycord,int &direction){ // this is to get coordinates for the ship to place them
               // declared 2 variables that stores a string and a character
    char xchord_char;       //   A string is used allowing multi character to be inserted without causing the loop error and the char stores in the first character array of the string
    char xcord_char;       //   A string is used allowing multi character to be inserted without causing the loop error and the char stores in the first character array of the string
    char ycord_char;
    int xcord_asc;
    int ycord_asc;
    while (true){
        cout<< "Enter the x coordinate [A-H][1-8]: ";
        getline(cin,xcord);



            /** Checking if lenght is 2 **/
            if(xcord.length() == 2){

                xcord_char = xcord[0];
                ycord_char = xcord[1];
                xcord_asc = int(xcord_char);
                ycord_asc = int(ycord_char);
                xcord = xcord;
                ycord = xcord;
                /** If within range of A-H **/
                if( !((((97<=xcord_asc) && (xcord_asc<=104)) || ((65<=xcord_asc) && (xcord_asc<=72))))  ){
                    cout<< "Invalid entry! Please try again."<<endl;  // if invalid the loop starts again
                }
                else if((!(((49<=ycord_asc) && (ycord_asc<=56)))) ){
                    cout<< "Invalid entry! Please try again."<<endl;  // if invalid the loop starts again
                 }
                else{
                    break;
                }
            }
                /** Invalid length of input **/
                else{
                    cout<< "Invalid entry! Please try again."<<endl;  // if invalid the loop starts again

                }
    }


       // string variable storing the player's direction
    char charDir; // will be used to store the first character
    char base [] = {'N','E','S','W'}; // char array that stores the compass direction
    string tempDirection;
    while (true){
        bool isValid = false; // checks first input is valid
        cout<< "Enter the direction (N/E/S/W): ";
        getline(cin,tempDirection);
        charDir= tempDirection[0]; // takes the first character of the string
        for (int i = 0; i<4;i++){
            if(base[i] == toupper(charDir)){  // the toupper will convert the character to a upper case whilst going through the array
                isValid = true; // when the character exists in the array isvalid is true
            }
        }
        if(isValid && tempDirection.length()==1){
            if(toupper(charDir) == 'N'){
                direction = 1;
            }
            else if(toupper(charDir) == 'E'){
                direction = 2;
            }
            else if(toupper(charDir) == 'S'){
                direction = 3;
            }
            else if(toupper(charDir) == 'W'){
                direction = 4;
            }
            break;  // which then breaks and exists when its true
        }
        else{
            cout << "Invalid entry! Please try again."<<endl; // otherwise invalid and loop starts again
        }



    }
  //  cout<< "You added a ship to x:"<< xcord << " and y:"<< ycord<<" Direction: "<<direction <<endl;  // this just outputs the users final decision
}



void addShip (Player &player){
  // addship for the given player object


    cout<<"--------------------------------------------- \n";
    cout<<"               Add Ship\n";
    cout<<"--------------------------------------------- \n";
    cout<<"           No  Name        squares\n";
    cout<<"--------------------------------------------- \n";    // the add ship grid art
    cout<<"           1   Carrier      5 \n";
    cout<<"           1   Battleship   4 \n";
    cout<<"           2   Cruiser      3 \n";
    cout<<"           1   Submarine    3 \n";
    cout<<"           1   Destroyer    2 \n";
    cout<<"--------------------------------------------- \n";

    string shipName[5] = {"Destroyer","Submarine","Cruiser","Battleship","Carrier"};
    int sizeShip[5] = {2,3,3,4,5};
    bool overLapShip; // boolean to see if the user overlapped the ships or not
    string xcord;
    string ycord;
    int direction;

    for(int i=0;i<=4;i++){
        while(true){
        // loops 5 time allowing for 5  ships to be added
        cout<<player.getName()<<" add a "<<shipName[i]<< " size: "<< sizeShip[i]<<endl;
        get_coordinates(xcord,ycord,direction);
        // convert the data types to be used on the board
        char charX= toupper(xcord[0]);
        int xcord_asc = int(charX)-65;
        char charY= ycord[1];
        int ycord_asc = (charY - '0')-1;

        if(player.placeShip(xcord_asc,ycord_asc,direction,i)){
            break;
        }
        else{cout<< "Ships cannot overlap or go past the boundaries! Please try again." << endl;}
        }
        cout << "\x1B[2J\x1B[H"<<endl;
        // displays the ship board
        player.displayBoard();
        cout<<"\n";

    }
    cout<< "All ships are added!"<<endl;
    enterToContinue(); // clears console in bash

}

void enterToContinue(){     // this is just the press enter to continue function cin.clear doesnt work so i settled out for the clear bash console

    cout<<"Press enter to continue!";
    cin.ignore();

    cout << "\x1B[2J\x1B[H"<<endl; // clears console in bash

}

