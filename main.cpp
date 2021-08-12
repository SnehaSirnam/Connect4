#include <iostream>
#include <string>
#include "Player.h"

using namespace std;

int row = 6;
int column = 7;
char board[6][7];
int gamePathLength = 0;

//Function to print the statistics of the game play
void printStats(Player p) {
    cout << "Total nodes for " << p.name << " is " << p.totalnodes << endl;
    cout << "Total nodes expanded for " << p.name << " is " << p.totalNodesExpanded << endl;
    cout << "Total time spent for " << p.name << " is " << p.totalTime << "ms" << endl;
    cout << "Total game path length " << gamePathLength << endl;
    cout << "Memory Used by : " << p.name << " is " << p.getTotalMemoryUsedByPlayerInKb(p.totalnodes) << "kb" << endl;
}

//Function to reset the board
void resetBoard() {
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < column; j++) {
            board[i][j] = '*';
        }
    }
}

//Function to display the board
void displayBoard() {
    for (auto &i : board) {
        cout << "|";
        for (char &j : i) {
            if (j != 'X' && j != 'O') j = '*';
            cout << " " << j << " ";
        }
        cout << "|" << endl;
    }
    cout << "______________________________________________" << endl << endl;
}

//display the types of comparision
void displaySet(){
    cout << endl << "Following is the comparition types: " << endl << endl;
    cout << "1: MAX(MinMax A-B) vs MIN(Alpha-Beta Search)" << endl;
    cout << "2: MAX(MinMax A-B) vs MIN(MinMax A-B)" << endl;
    cout << "3: MAX(Alpha-Beta Search) vs MIN(Alpha-Beta Search)" << endl;
}

// Checks if the board is completed
bool checkIfCompleteBoard() {
    int fullboard = 0;
    for (int i = 0; i < column; i++) {
        if (board[0][i] != '*') fullboard = fullboard + 1;

    }
    if (fullboard == column) return true;
    return false;
}

// Function to check Win
bool checkWin(Player p) {
    if (p.isFour(board)) {
        displayBoard();
        cout << p.name << " Wins!" << endl;
        return true;
    }
    return false;
}

// Function to print the intermediate steps
void printIntermediateSteps(int index, Player p, Player p2)
{
    if(index % 4 == 0)
    {
        displayBoard();
        cout << "Nodes for " << p.name << " in this step " << p.nodes << endl;
        cout << "Nodes expanded for " << p.name << " in this step " << p.nodesExpanded << endl;
        cout << "Time spent by " << p.name << " in this step " << p.stepTime << "ms" << endl;
        cout << "Memory Used by : " << p.name << " in this step " << p.getTotalMemoryUsedByPlayerInKb(p.nodes) << "kb" << endl<< endl;

        cout << "Nodes for " << p2.name << " in this step " << p2.nodes << endl;
        cout << "Nodes expanded for " << p2.name << " in this step " << p2.nodesExpanded << endl;
        cout << "Time spent by " << p2.name << " in this step " << p2.stepTime << "ms" << endl;
        cout << "Memory Used by : " << p2.name << " in this step " << p2.getTotalMemoryUsedByPlayerInKb(p.nodes) << "kb" << endl<< endl;
        cout << "______________________________________________" << endl << endl;
    }
}

// Evaluation function 1 to estimate the goodness of the position
void firstSetEvaluation(int evalFunctionForMaxOfMinMaxAB, int evalFunctionForMinOfAlphaBeta) {
    gamePathLength = 0;
    Player ai(1, 'X', 'O', "Max(minMaxAB)"), ai2(1, 'O', 'X', "Min(AlphaBetaSearch)");
    bool isWin = false;
    pair<double, double> res;
    cout << "Max(minMaxAB) playing as " << ai.myTag << " using EV#" << evalFunctionForMaxOfMinMaxAB
         << " vs Min(AlphaBetaSearch) playing as " << ai2.myTag << " using EV#" << evalFunctionForMinOfAlphaBeta
         << endl;
    cout << "Initial Board" << endl;
    displayBoard();
    cout << "______________________________________________" << endl << endl;
    do {
        gamePathLength++;
        ai.aiMoveMinMax(board, 6, evalFunctionForMaxOfMinMaxAB);
        isWin = checkWin(ai);
        displayBoard();
        if (isWin) {
            resetBoard();
            printStats(ai);
            printStats(ai2);
            break;
        }

        ai2.aiMoveAlphaBeta(board, 6, evalFunctionForMinOfAlphaBeta);
        isWin = checkWin(ai2);
        displayBoard();
        if (isWin) {
            resetBoard();
            printStats(ai);
            printStats(ai2);
            break;
        }
        if (checkIfCompleteBoard()) {
            displayBoard();
            cout << "It's a DRAW! The board is full" << endl;
            resetBoard();
            printStats(ai);
            printStats(ai2);
            break;
        }
        printIntermediateSteps(gamePathLength, ai, ai2);
    } while (true);
    cout << "______________________________________________" << endl << endl;
}

// Evaluation function 2 to measure the goodness of the game
void secondSetEvaluation(int evalFunctionForMaxOfMinMaxAB, int evalFunctionForMinOfMinMaxAB) {
    gamePathLength = 0;
    resetBoard();
    Player ai(1, 'X', 'O', "Max(minMaxAB)"), ai2(1, 'O', 'X', "Min(minMaxAB)");
    bool isWin = false;
    pair<double, double> res;
    cout << "Max(minMaxAB) playing as " << ai.myTag << " using EV#" << evalFunctionForMaxOfMinMaxAB
         << " vs Min(minMaxAB) playing as " << ai2.myTag << " using EV#" << evalFunctionForMinOfMinMaxAB << endl;
    cout << "Initial Board" << endl;
    displayBoard();
    cout << "______________________________________________" << endl << endl;
    do {
        gamePathLength++;
        ai.aiMoveMinMax(board, 6, evalFunctionForMaxOfMinMaxAB);
        isWin = checkWin(ai);
        displayBoard();
        if (isWin) {
            resetBoard();
            printStats(ai);
            printStats(ai2);
            break;
        }

        ai2.aiMoveMinMax(board, 6, evalFunctionForMinOfMinMaxAB);
        isWin = checkWin(ai2);
        displayBoard();
        if (isWin) {
            resetBoard();
            printStats(ai);
            printStats(ai2);
            break;
        }
        if (checkIfCompleteBoard()) {
            displayBoard();
            cout << "It's a DRAW! The board is full" << endl;
            resetBoard();
            printStats(ai);
            printStats(ai2);
            break;
        }
        printIntermediateSteps(gamePathLength, ai, ai2);
    } while (true);
    cout << "______________________________________________" << endl << endl;
}

// Evaluation function 3 to measure the goodness of the game
void thirdSetEvaluation(int evalFunctionForMaxAlphaBeta, int evalFunctionForMinAlphaBeta) {
    gamePathLength = 0;
    resetBoard();
    Player ai(1, 'X', 'O', "Max(AlphaBetaSearch)"), ai2(1, 'O', 'X', "Min(AlphaBetaSearch)");
    bool isWin = false;
    pair<double, double> res;
    cout << "Max(AlphaBetaSearch) playing as " << ai.myTag << " using EV#" << evalFunctionForMaxAlphaBeta
         << " vs Min(AlphaBetaSearch) playing as " << ai2.myTag << " using EV#" << evalFunctionForMinAlphaBeta << endl;
    cout << "Initial Board" << endl;
    displayBoard();
    cout << "______________________________________________" << endl << endl;
    do {
        gamePathLength++;
        ai.aiMoveAlphaBetaForMax(board, 6, evalFunctionForMaxAlphaBeta);
        isWin = checkWin(ai);
        displayBoard();
        if (isWin) {
            resetBoard();
            printStats(ai);
            printStats(ai2);
            break;
        }

        ai2.aiMoveAlphaBeta(board, 6, evalFunctionForMinAlphaBeta);
        isWin = checkWin(ai2);
        displayBoard();
        if (isWin) {
            resetBoard();
            printStats(ai);
            printStats(ai2);
            break;
        }
        if (checkIfCompleteBoard()) {
            displayBoard();
            cout << "It's a DRAW! The board is full" << endl;
            resetBoard();
            printStats(ai);
            printStats(ai2);
            break;
        }
        printIntermediateSteps(gamePathLength, ai, ai2);
    } while (true);
    cout << "______________________________________________" << endl << endl;
}

//Driver code
int main() {

    int MaxEvalNum;
    int MinEvalNum;
    int set_num;
    displaySet();
    cout << "Enter the comparison type number (1, 2, or 3): ";
    cin >> set_num;
    cout << endl;
    cout << "enter Max player evaluation number (1, 2, or 3): ";
    cin >> MaxEvalNum;
    cout << "enter Min player evaluation number (1, 2, or 3): ";
    cin >> MinEvalNum;
    cout << endl;
    if(set_num==1)
        firstSetEvaluation(MaxEvalNum,MinEvalNum);

    else if(set_num==2)
        secondSetEvaluation(MaxEvalNum,MinEvalNum);
    else
        thirdSetEvaluation(MaxEvalNum,MinEvalNum);
    return 0;

    return 0;
}

