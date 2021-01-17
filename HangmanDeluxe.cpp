//Written by Kevin Chen and Joshua Li
//Created on January 16 at 9:40am PST
//This program has a handful of fun short games to play

#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <random>
#include <string>
#include <vector>

using namespace std;

//vector to store hangman pictographs
const vector<string> hangmanPics = {"\n  +---+\n      |\n      |\n      |\n     ===", "\n  +---+\n  O   |\n      |\n      |\n     ===", "\n  +---+\n  O   |\n  |   |\n      |\n     ===", "\n  +---+\n  O   |\n /|   |\n      |\n     ===", "\n  +---+\n  O   |\n /|\\  |\n      |\n     ===", "\n  +---+\n  O   |\n /|\\  |\n /    |\n     ===", "\n  +---+\n  O   |\n /|\\  |\n / \\  |\n     ==="};

//structure for saving team information
struct teamInfo_t {
    string teamName = "";
    int totalWins = 0;
};

//function to get the teams in the game
vector<teamInfo_t> getTeamNames() {

    int numberOfTeams = 0;
    while (true) {
        cout << "How many teams will be playing today? ";
        cin >> numberOfTeams;
        //check to make sure only integers are used
        if (cin.fail()) {
            cin.clear();
            cin.ignore();
            cout << "Enter a valid number! \n";
        } else {
            break;
        }
    }

    cout << "Very cool! We have got " << numberOfTeams << " playing today!\nNow how about we get some names?";
    //create vector to store teams
    vector<teamInfo_t> teamList = {};
    for (int i = 1; i <= numberOfTeams; i++) {
        //as teams are added ask for their names
        cout << "\nName for Team " << i << ": ";
        teamInfo_t newTeam;
        string teamInput = "";
        cin >> teamInput;
        newTeam.teamName = teamInput;
        teamList.push_back(newTeam);
    }

    cout << "So for review, today our competing teams will be ";
    //if only playing one team
    if (numberOfTeams == 1) {
        cout << "just you!";
    } else {
        //print for multiple teams 
        for (int i = 0; i < teamList.size(); i++) {
            if (i == (teamList.size() - 1)) {
                cout << "and " << teamList.at(teamList.size() - 1).teamName << "!";
            } else {
                cout << teamList.at(i).teamName << ", ";
            }
        }
    }
    return teamList;
}

//function to read the wordlist 
vector<string> readHangmanWords() {
    string fileName;
    cout << ("\n\nPlease give the file name that your word list is stored in:  ");
    cin >> fileName;
    ifstream wordlistFile(fileName);

    //check if the file exists, if it doesn't quit the program 
    if (wordlistFile.fail()) {
        cout << "There is no such file. ";
        exit(EXIT_FAILURE);
    }
    
    //get the wordlist and format it to be all upper case 
    vector<string> wordlist = {};
    string word;
    while (true) {
        getline(wordlistFile, word);
        string upper_word = "";
        for (char x : word) {
            x = toupper(x);
            upper_word += x;
        }
        wordlist.push_back(upper_word);
        if (wordlistFile.fail()) {
            break;
        }
    }

    wordlistFile.close();
    return wordlist;
}

//choosing the random word for the hangman 
string getRandomWord(const vector<string> &wordList) {

    return wordList.at(rand() % wordList.size());
}

//interpreting the inputs of the user 
char getGuess(vector<char> guesses) {
    char guess = ' ';

    while (true) {
        //prompt the user 
        cout << "\nGuess a letter: ";
        //capitalize the input 
        cin >> guess;
        guess = toupper(guess);
        
        //check if the user is going to guess the entire word 
        if (guess == '!') {
            cout << "You must now guess the whole word: ";
            return guess;
        //check if that letter has already been guessed 
        } else if (count(guesses.begin(), guesses.end(), guess)) {
            cout << "\nYou have already guessed that letter.";
        //check if the input is actually a letter 
        } else if (guess < 65 || guess > 132) {
            cout << "\nPlease enter a LETTER.";
        } else {
        //if it passses through everthing return it 
            return guess;
        }
    }
}

void displayHangman(vector<char> badGuesses, vector<char> guesses, string word, bool &gameDone) {

    //display the hangman picture corresponding to how many incorrect guesses have been made
    cout << hangmanPics.at(badGuesses.size());
    cout << "\nMissed guesses: ";

    //display all the incorrect guesses
    for (char guess : badGuesses) {
        cout << guess << ", ";
    }

    cout << '\n';

    //display the underlines to show how many letters, but replace them with the letter if it has been guessed
    for (char letter : word) {
        if (count(guesses.begin(), guesses.end(), letter)) {
            cout << letter << ' ';
        } else if (letter == ' ') {
            cout << ' ';
        } else {
            cout << "_ ";
        }
    }
}

bool hangmanRound(vector<string> &wordBank, vector<teamInfo_t> &teams) {

    //initiate a list for bad guesses and total guesses, including space so the player doesn't have to
    vector<char> badGuesses = {};
    vector<char> guesses = {' '};

    //pick a random word from the word bank
    string word = getRandomWord(wordBank);
    bool gameDone = false;

    for (int turn = (rand() % teams.size());; turn++) {
        //determine the team here
        int currentTeam = turn % teams.size();
        cout << "\nIt is " << teams.at(currentTeam).teamName << "'s turn\n\n";

        displayHangman(badGuesses, guesses, word, gameDone);

        //get the player's guess
        char guess = getGuess(guesses);
        guesses.push_back(guess);

        //! indicates that a player is ready to guess the whole word
        if (guess == '!') {
            string wholeWord = "";
            cin.ignore();
            //getline the answer (including spaces)
            getline(cin, wholeWord);
            string upper_word = "";
            for (char x : wholeWord) {
                x = toupper(x);
                upper_word += x;
            }
            //if the word is correct, the game is over
            if (upper_word == word) {
                cout << "Congratulations! The word was " << word << ".";
                cout << " \nThe winner is " << teams.at(currentTeam).teamName << "!\n";
                teams.at(currentTeam).totalWins++;
                return true;
            }
            //if the word is not correct, count it as a bad guess and continue
            else {
                cout << "\nNo, " << upper_word << " is not correct.\n";
            }
        }

        //if the guess is not in the word, add it to the list of bad guesses
        if (!(count(word.begin(), word.end(), guess))) {
            badGuesses.push_back(guess);
        }

        //check if the players have reached the max amount of guesses
        if (badGuesses.size() == hangmanPics.size()) {
            cout << "You are out of guesses! The word was " << word << ".";
            gameDone = true;
        }

        //check if all letters are present
        bool allLetters = true;
        for (char letter : word) {
            if (!count(guesses.begin(), guesses.end(), letter)) {
                allLetters = false;
                break;
            }
        }
        //if all letters are present, then the game is over
        if (allLetters) {
            gameDone = true;
            displayHangman(badGuesses, guesses, word, gameDone);
            cout << "\n\nCongratulations! The word was " << word;
            cout << " \nThe winner is " << teams.at(currentTeam).teamName << "!\n";
            teams.at(currentTeam).totalWins++;
        }
        //return if the game is done
        if (gameDone) {
            return true;
        }
    }
}

void playHangman() {

    cout << "Welcome to Hangman!\nYou may supply your own vocabulary by putting words (no punctuation) in a .txt file.\nIf you are ready to guess the whole word, press enter !";

    //initiate and fill the word bank
    vector<string> wordBank = {};
    wordBank = readHangmanWords();

    //get team names
    vector<teamInfo_t> teams = getTeamNames();

    //play hangman infinitely until the players stop
    while (hangmanRound(wordBank, teams)) {
        cout << "\nScoreboard\n----------\n";

        //display scores
        for (teamInfo_t team : teams) {
            cout << team.teamName << ": " << team.totalWins << "\n";
        }

        //prompt them to play again, quit if their answer doesn't start with y
        cout << "Type yes if you would like to play again: ";
        string answer = " ";
        cin >> answer;
        if (!(answer.at(0) == 'y' || answer.at(0) == 'Y')) {
            break;
        }

        //ask them to reselect a category
        wordBank = readHangmanWords();
    }
}

int main() {
    //seed the random and start the game
    srand(time(nullptr));
    playHangman();
}