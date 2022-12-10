#include <iostream>
#include <iomanip>
using namespace std;

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <sstream>
#include <set>
#include <map>
#include <list>
#include <stack>
#include <queue>

#include "Mastermind.h"

int main(){
    Mastermind mastermind;

    cout << "Welcome to Mastermind!" << endl;
    cout << endl << "Do you want to choose your rules?" << endl << endl;
    cout << "[0] - No, [1] - Yes" << endl;
    
    int input;
    cin >> input;
    while (input != 1 && input != 0)
    {
        cin.clear();
        cin.ignore(10000, '\n');
        cout << endl
             << "Invalid input. Try Again." << endl;
        cin >> input;
    }
    if(input == 1){
        mastermind.preGame();
    }else{
        mastermind.answer= mastermind.genAnswer();
    }
        
    mastermind.gameLoop();

    return 0; 
}

