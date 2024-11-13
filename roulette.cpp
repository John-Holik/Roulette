#include <iostream>
#include <fstream>
#include <string>
#include <random>

using namespace std;

// Function to get player's name and balance from file
int getPlayerInfo(string& name) {
    ifstream inputFile("players.txt");
    string savedName;
    int savedBalance;
    while (inputFile >> savedName >> savedBalance) {
        if (savedName == name) {
            cout << "Welcome back " << name << "!" << endl;
            return savedBalance;
        }
    }
    cout << "Welcome " << name << "! Your balance is $200" << endl;
    return 200;
}

// Function to save player's balance to file
void savePlayerInfo(string name, int balance) {
    string strTemp;
    int savedBalance;
    bool playerExists = 0;
    ifstream myFile("players.txt");
    // Creates temporary file to transfer data to.
    ofstream fileout("fileout.txt");

    // While loop transfers data until the current player's save location is found. Updates current player's balance.
    while(myFile >> strTemp >> savedBalance)
    {
        if(strTemp == name)
        {
            strTemp = name.append(" ") += std::to_string(balance);
            playerExists = 1;
        }
        else
        {
            strTemp = strTemp.append(" ") += std::to_string(savedBalance);
        }
        strTemp += "\n";
        fileout << strTemp;
    }

    // Creates new data entry if the current player does not exist in the database.
    if (playerExists == 0)
    {
        fileout << name << " " << balance;
    }

    // Deletes old save file and replaces it with updated version.
    char oldname[] = "fileout.txt";
    char newname[] = "players.txt";
    myFile.close();
    fileout.close();
    remove(newname);
    if (rename(oldname,newname) != 0)
        cout << endl << "Error saving data" << endl;
}

// Main function
int main() {
    string playerName;
    int playerBalance = 200;
    int totalWinnings = 0, totalLosses = 0;
    cout << "Welcome to the Roulette game!" << endl;
    cout << "Please enter your name: ";
    cin >> playerName;
    playerBalance = getPlayerInfo(playerName);

    // Initialize random number generator
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distr(0, 36);

    cout << "You have $" << playerBalance << " in your balance." << endl;
    
    // Main game loop
    char playAgain = 'y';
    while (playAgain == 'y' && playerBalance > 0) {
        cout << "What would you like to bet on? Enter 'c' for color or 'n' for number: ";
        char betType;
        cin >> betType;
        while ((betType != 'c') && (betType != 'n'))
        {
            cout << "Unfortunately we do not offer betting of that type. Input 'c' to bet on color or 'n' to bet on number: ";
            cin >> betType;
        }
        int betAmount;
        cout << "Enter your bet amount (max $50): ";
        cin >> betAmount;
        if (cin.fail())
            {
                cin.clear();
                //cin.ignore(INT_MAX, '\n');
                cout << "You can only enter numbers" << endl;
            }
        while (betAmount > 50 || betAmount < 1) {
            cout << "Bet amount exceeds maximum or minimum bet. Enter a new amount (max $50, min $1): ";
            cin >> betAmount;
            // Checks if cin recieves an invalid input
            if (cin.fail())
            {
                cin.clear();
                //cin.ignore(INT_MAX, '\n');
                cout << "You can only enter numbers" << endl;
            }
        }
        if (betType == 'c') {
            cout << "Enter your color choice ('r' for red, 'b' for black): ";
            char colorChoice;
            cin >> colorChoice;
            while (colorChoice != 'r' && colorChoice != 'b') {
                cout << endl << "Invalid color choice, try again with 'r' for red, and 'b' for black: ";
                cin >> colorChoice;
            }
            if (distr(gen) % 2 == 0 && colorChoice == 'r') {
                cout << "Winning color: Red!" << endl;
                cout << "You win!" << endl;
                playerBalance += betAmount;
                totalWinnings += betAmount;
            } else if (distr(gen) % 2 == 1 && colorChoice == 'b') {
                cout << "Winning color: Black!" << endl;
                cout << "You win!" << endl;
                playerBalance += betAmount;
                totalWinnings += betAmount;
            } else {
                if (distr(gen) % 2 == 0){
                    cout << "Winning color: Red!" << endl;
                }
                else if (distr(gen) % 2 == 1){
                    cout << "Winning color: Black!" << endl;
                }
                cout << "You lose!" << endl;
                playerBalance -= betAmount;
                totalLosses += betAmount;
            }
        } else if (betType == 'n') {
            cout << "Enter your number choice (0-36): ";
            int numberChoice;
            cin >> numberChoice;
            if (distr(gen) == numberChoice) {
                cout << "Winning number: " << distr(gen) << "!" << endl;
                cout << "You win!" << endl;
                playerBalance += betAmount * 35;
                totalWinnings += betAmount * 35;
            } else {
                cout << "Winning number: " << distr(gen) << "!" << endl;
                cout << "You lose!" << endl;
                playerBalance -= betAmount;
                totalLosses += betAmount;
            }
        }
        cout << "You have $" << playerBalance << " in your balance." << endl;
        cout << "Play again? (y/n): ";
        cin >> playAgain;
    }
    if (playerBalance <= 0) {
                cout << "You are out of funds. Press 'r' to reset balance to $200 or any other key to quit: ";
        char resetBalance;
        cin >> resetBalance;
        if (resetBalance == 'r') {
            playerBalance = 200;
            cout << "Your balance has been reset to $200. Good luck!" << endl;
        }
    }
    savePlayerInfo(playerName, playerBalance);
    cout << "Thanks for playing!" << endl;
    cout << "Final Balance: $" << playerBalance << endl;
    cout << "Session winnings: $" << totalWinnings << endl;
    cout << "Session losses: $" << totalLosses << endl;
    return 0;
}