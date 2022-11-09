#ifndef MASTERMIND_H
#define MASTERMIND_H

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
#include <list>
#include <algorithm>
using namespace std;

class Mastermind 
{
private:
    bool allowDuplicates;
    int codeLength;
    int turns;
    set<string> colors = {"red", "green", "blue", "yellow", "brown", "orange", "black", "white"};;
    map<int, string> answer;
    list<string> currentGuess;
    stack<pair<int, int>> guessResults;
    queue<list<string>> guesses;

public:
    Mastermind();  // Default constructor
    ~Mastermind(); // Destructor
    void setDuplicates(bool);
    void setCodeLength(int);
    void setTurns(int);
    map<int, string> genAnswer();  // Generate answer
    bool validGuess(list<string>); // Check for validity of guesses
    pair<int, int> checkGuess(list<string>);
    bool checkWin();
    void preGame();
    void gameLoop();
    void outputVisuals();
};

Mastermind::Mastermind()
{
    this->allowDuplicates = false;
    this->codeLength = 4;
    this->turns = 10;
}

Mastermind::~Mastermind()
{
    answer.clear();
    colors.clear();

    stack<pair<int, int>> emptyStack;
    swap(this->guessResults, emptyStack);

    queue<list<string>> emptyQueue;
    swap(this->guesses, emptyQueue);
}

void Mastermind::setCodeLength(int length)
{
    this->codeLength = length;
}

void Mastermind::setDuplicates(bool duplicates)
{
    this->allowDuplicates = duplicates;
}

void Mastermind::setTurns(int turns)
{
    this->turns = turns;
}

map<int, string> Mastermind::genAnswer()
{
    if (allowDuplicates)
    { // if duplicates was chosen, generate random answer without catching repetitions
        for (int i = 0; i < this->codeLength; i++)
        {
            auto it = this->colors.cbegin();
            int colorIndex = rand() % this->colors.size();
            advance(it, colorIndex);
            answer.insert({i, *it});
        }
    }
    else
    { // otherwise if duplicates was not chosen, generate random answer that catches repetition
        for (int i = 0; i < this->codeLength; i++)
        {                                                  // loop for length
            int colorIndex = rand() % this->colors.size(); // generate a number for index
            auto it = this->colors.cbegin();
            advance(it, colorIndex);
            while (answer.find(i)->second == *it)
            { // keep generating number for index the number is already in answer
                colorIndex = rand() % this->colors.size();
                it = this->colors.cbegin();
                advance(it, colorIndex);
            }
            answer.insert({i, *it});
        }
    }

    return this->answer;
}

bool Mastermind::validGuess(list<string> guess)
{ // validGuess is a simple function that determines guess validity
    if (guess.size() != this->codeLength)
    { // if the player inputted a guess that is not the correct length it is wrong
        return false;
    }


    for (auto itr = guess.begin(); itr != guess.end(); ++itr)
    { // if the guess is the correct length, check if colors inputted are valid
        if (colors.count(*itr) == 1)
        {
            continue; // the color is in colors array, so continue the loop
        }
        else
        {
            return false; // the color is not in the colors set, check is not valid
        }
    }
    return true; // if we pass both of these checks, we have ourselves a valid guess from the player
}

pair<int, int> Mastermind::checkGuess(list<string> guess)
{
    int red = 0;   // red meaning correct position + color
    int white = 0; // white meaning correct color

    int i = 0;// index
    for (auto itr = guess.begin(); itr != guess.end(); ++itr) // check for red and white
    {

        if (answer.at(i) == *itr) // if position match and colors match, add to red
        {
            red++;
        }
        else
        {
            for (auto it = answer.begin(); it != answer.end(); it++)
            {
                if (it->second == *itr)
                {
                    white++; // else if colors match, or guess exists in answer vector, add to white
                }
            }
        }
        i++;
    }

    return {red, white}; // return pair with red and white values
}

bool Mastermind::checkWin()
{
    if (this->guessResults.top().first == 4)
    { // if there are 4 red(all positions and colors match) game is won
        return true;
    }
    else
    {
        return false; // else the game is not won
    }
}

void Mastermind::preGame()
{
    cout << "Allow duplicates: "; // choose duplicates
    cout << endl
         << "[1] True" << endl
         << "[0] False" << endl;
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
    if (input == 1)
    {
        setDuplicates(true);
    }

    int length;
    cout << endl
         << "Code Length(4,6,8): " << endl; // choose code length
    cin >> length;
    while (length != 4 && length != 6 && length != 8)
    {
        cin.clear();
        cin.ignore(10000, '\n');
        cout << endl
             << "Invalid input. Try Again." << endl;
        cin >> length;
    }
    setCodeLength(length);

    int turns;
    cout << endl
         << "Amount of Turns(Default 10): " << endl; // choose number of turns
    cin >> turns;
    while (turns <= 0)
    {
        cin.clear();
        cin.ignore(10000, '\n');
        cout << endl
             << "Invalid input. Try Again." << endl;
        cin >> turns;
    }
    setTurns(turns);
}

void Mastermind::gameLoop()
{
    this->answer = genAnswer();
    bool win;
    cin.ignore(10000, '\n');
    for (int i = 0; i < this->turns; i++)
    {
        currentGuess.clear();
        while (!validGuess(this->currentGuess))
        {
            if (i != 0)
            {
                outputVisuals();
            }
            cout << endl
                 << "Colors: Red, Green, Blue, Yellow, Brown, Orange, Black, White " << endl
                 << "Guess: ";
            string color;
           
            getline(cin, color);
            string delimiter = " ";
            size_t pos = 0;
            string token;
            while ((pos = color.find(delimiter)) != std::string::npos) {
                token = color.substr(0, pos);
                transform(token.begin(), token.end(), token.begin(), ::tolower);
                this->currentGuess.push_back(token);
                color.erase(0, pos + delimiter.length());
            }
            transform(color.begin(), color.end(), color.begin(), ::tolower);
            this->currentGuess.push_back(color);

            if (!validGuess(this->currentGuess))
            {
                currentGuess.clear();
                cout << endl << "Invalid input. Try Again." << endl;
            }
        }

        guesses.push(currentGuess);
        guessResults.push(checkGuess(this->currentGuess));

        win = checkWin();
        if (win)
        {
            break;
        }
    }

    outputVisuals();
    cout << endl;
    if (win) // if you win
        cout << "You win!";
    else // if you lost
        cout << "You lost.";
    cout << endl
         << endl
         << "Answer: "; // output answer

    for (auto it = answer.begin(); it != answer.end(); it++)
    {
        cout << it->second << " ";
    }
    cout << endl;
}

void Mastermind::outputVisuals()
{
    queue<list<string>> copyGuesses = this->guesses;
    stack<pair<int, int>> copyResults = this->guessResults;
    cout << endl
         << "Turns Remaining: " << turns - this->guesses.size() << endl
         << endl
         << setw(codeLength) << left << "Guesses" << setw(5);
    cout << " | [Red : White]" << endl;
    while (!copyGuesses.empty())
    {
        cout << endl;
        list<string> guessList = copyGuesses.front();
        for (auto it = guessList.begin(); it != guessList.end(); it++)
        {
            cout << *it << " ";
        }
        cout << " | [" << copyResults.top().first << " : " << copyResults.top().second << "]";
        copyGuesses.pop();
        copyResults.pop();
    }
    cout << endl;
};
#endif