// This class is the core of the project
#include <iostream>
#include <stdlib.h>
#include <string>
#include <vector>
#include <climits>
#include <utility>
#include <chrono>

using namespace std;
using namespace std::chrono;

//Player class with data members and functions
class Player {
public:
    int searchOrder[7];
    int rows, cols;
    int playChoice;
    char myTag, userTag;
    int maxdepth, nodes = 0, totalnodes = 0, nodesExpanded = 0, totalNodesExpanded = 0;
    double stepTime = 0.0, totalTime = 0.0;
    string name;

    Player(int depth, char playerTag, char oppTag, string name);

    void computeEval(int &myTagCount, int &opponentTagCount, int &starcount, int
    &evaluation, int &bestevaluation, pair<int, int> &);

    bool checkIfCompleteBoard(char board[6][7], int column);

    bool isValidMove(char board[6][7], int column);

    char opposite(char player);

    vector<pair<int, int> > getValidMoves(char board[6][7]);

    void makeMove(char board[6][7], int slot);

    pair<int, int> alphaBetaSearch(char board[6][7], int depth, char player, int alpha, int
    beta, int evalFunc);

    pair<int, int> alphaBetaSearchForMax(char board[6][7], int depth, char player, int alpha, int
    beta, int evalFunc);

    pair<int, vector<pair<int, int>>> minMaxAB(char board[6][7], int depth, int position, char
    player, int usethresh, int passthresh, int evalFunc);

    pair<int, int> max_value(char board[6][7], int depth, char player, int alpha, int beta, int evalFunc);

    pair<int, int> min_value(char board[6][7], int depth, char player, int alpha, int beta, int evalFunc);

    int evaluateFunction1(char board[6][7], char player);

    int getAndComputeEval(char board[6][7], char player, int evalFunc);

    void aiMoveAlphaBeta(char board[6][7], int depth, int evalFunc);

    void aiMoveAlphaBetaForMax(char board[6][7], int depth, int evalFunc);

    void aiMoveMinMax(char board[6][7], int depth, int evalFunc);

    int evaluateFunction2(char board[6][7]);

    void checkSpaceBelow(char currentBoard[6][7]);

    bool isFour(char currentBoard[6][7]);

    double getTotalMemoryUsedByPlayerInKb(int nodeCount);

    int computeScoreForEval3(char gameBoard[6][7], char player, int col, int row);

    int evaluateFunction3(char board[6][7], char player);
};

//The below method is the parameterized constructor for the player class
Player::Player(int depth, char playerTag, char oppTag, string pname) {
    rows = 6, cols = 7;
    //int order[7] = {3, 2, 4, 1, 5, 0, 6};
    int order[7] = {0, 1, 2, 3, 4, 5, 6};
    myTag = playerTag, userTag = oppTag;
    name = pname;
    for (int i = 0; i < 7; i++) searchOrder[i] = order[i];
    maxdepth = depth;
    nodes = 0;
    totalnodes = 0;
}

// This function is used for getting the valid moves
vector<pair<int, int> > Player::getValidMoves(char board[6][7]) {
    vector<pair<int, int>> moves;

    for (auto col : searchOrder) {
        for (int row = 5; row >= 0; row--) {
            if (board[row][col] == '*') {
                moves.push_back(make_pair(row, col));
                break;
            }
        }
    }
    return moves;
}

// This function is used to make a move
void Player::makeMove(char board[6][7], int column) {
    playChoice = column;
    checkSpaceBelow(board);
}

// This function is used to check if all the 4 consecutive squares
bool Player::isFour(char currentBoard[6][7]) {
    char tagPosition = myTag;
    bool win = false;
    int counts = 0;
    for (int i = 5; i >= 0; i--) // hori
    {
        for (int j = 0; j < 4; j++) {
            if (currentBoard[i][j] == tagPosition && currentBoard[i][j + 1] == tagPosition &&
                currentBoard[i][j + 2] == tagPosition && currentBoard[i][j + 3] == tagPosition) {
                return true;
            }

        }
    }
    for (int i = 5; i >= 3; i--) // vertical
    {
        for (int j = 0; j < 7; j++) {
            if (currentBoard[i][j] == tagPosition && currentBoard[i - 1][j] == tagPosition &&
                currentBoard[i - 2][j] == tagPosition && currentBoard[i - 3][j] == tagPosition) {
                return true;
            }

        }
    }
    for (int i = 5; i >= 3; i--) // r - l
    {
        for (int j = 3; j < 7; j++) {
            if (currentBoard[i][j] == tagPosition && currentBoard[i - 1][j - 1] == tagPosition &&
                currentBoard[i - 2][j - 2] == tagPosition && currentBoard[i - 3][j - 3] == tagPosition)
                return true;
        }
    }
    for (int i = 5; i >= 3; i--) // l - r
    {
        for (int j = 0; j < 4; j++) {
            if (currentBoard[i][j] == tagPosition &&
                currentBoard[i - 1][j + 1] == tagPosition &&
                currentBoard[i - 2][j + 2] == tagPosition &&
                currentBoard[i - 3][j + 3] == tagPosition)
                return true;
        }
    }
    return false;

}

/*
 * The heuristic is determined by counting all potential spaces for making a row (ie all 4-size spaces that doesn't contain any enemy), and then subtracting the
number of potential spaces for the enemy. Seems to work quite well even at low depth trees will not check diagonals (game's too clever already) */
int Player::evaluateFunction2(char board[6][7]) {
    int result = 0;
    int i, j;

    for (i = 0; i < 6; i++)
        for (j = 0; j <= 7 - 4; j++) {
            if (board[i][j] != myTag && board[i][j + 1] != myTag && board[i][j + 2] != myTag &&
                board[i][j + 3] != myTag)
                result--;
            if (board[i][j] != userTag && board[i][j + 1] != userTag && board[i][j + 2] != userTag &&
                board[i][j + 3] != userTag)
                result++;
        }

    for (i = 0; i <= 6 - 4; i++)
        for (j = 0; j < 7; j++) {
            if (board[i][j] != myTag && board[i + 1][j] != myTag && board[i + 2][j] != myTag &&
                board[i + 3][j] != myTag)
                result--;
            if (board[i][j] != userTag && board[i + 1][j] != userTag && board[i + 2][j] != userTag &&
                board[i + 3][j] != userTag)
                result++;
        }

    return result;
}

//The below function is the MIN-MAX algorithm code with depth check, Move gen in algo is the getValidMoves function
pair<int, vector<pair<int, int>>> Player::minMaxAB(char board[6][7], int position, int depth, char
player, int usethresh, int passthresh, int evalFunc) {
    nodes++;
    vector<pair<int, int> > successors = getValidMoves(board);

    if (depth == maxdepth || successors.size() == 0) {
        vector<pair<int, int>> empty;
        if (player == 'X') {
            return make_pair(-getAndComputeEval(board, player, evalFunc), empty);
        } else {
            return make_pair(getAndComputeEval(board, player, evalFunc), empty);
        }

    }

    vector<pair<int, int>> bestpath;
    int nval;
    for (int i = 0; i < successors.size(); i++) {

        if (isValidMove(board, successors[i].second)) {
            board[successors[i].first][successors[i].second] = player;

            pair<int, vector<pair<int, int>>> value = minMaxAB(board, i, depth + 1, opposite(player),
                                                               -passthresh, -usethresh, evalFunc);

            board[successors[i].first][successors[i].second] = '*';

            nval = -value.first;
            if (nval > passthresh) {
                passthresh = nval;
                bestpath = value.second;
                bestpath.push_back(make_pair(successors[i].second, passthresh));
            }
            if (passthresh >= usethresh) {
                return make_pair(passthresh, bestpath);
            }
        }

    }

    return make_pair(passthresh, bestpath);
}

// Function for alpha beta search algorithm
pair<int, int> Player::alphaBetaSearch(char board[6][7], int depth, char player, int
alpha, int beta, int evalFunc) {
    int val, move_value;
    pair<int, int> bestmove;
    bestmove = min_value(board, depth, player, alpha, beta, evalFunc);
    return bestmove;
}

pair<int, int> Player::alphaBetaSearchForMax(char board[6][7], int depth, char player, int
alpha, int beta, int evalFunc) {
    int val, move_value;
    pair<int, int> bestmove;
    bestmove = max_value(board, depth, player, alpha, beta, evalFunc);
    return bestmove;
}

pair<int, int> Player::max_value(char board[6][7], int depth, char player, int alpha, int beta, int evalFunc) {
    nodes++;

    vector<pair<int, int> > moves = getValidMoves(board);
    if (depth == 0 || moves.size() == 0) {

        if (player == 'X') {
            return make_pair(-getAndComputeEval(board, player, evalFunc), -1);
        } else {
            return make_pair(getAndComputeEval(board, player, evalFunc), -1);
        }

    }

    int val = INT_MIN, column = -1;
    pair<int, int> value;
    for (int i = 0; i < moves.size(); i++) {

        if (isValidMove(board, moves[i].second)) {
            board[moves[i].first][moves[i].second] = myTag;
            value = min_value(board, depth - 1, opposite(player), alpha, beta, evalFunc);
            val = max(val, value.first);
            if (value.first == val) column = moves[i].second;

            board[moves[i].first][moves[i].second] = '*';

            if (val >= beta) {
                nodesExpanded++;
                return make_pair(val, column);
            }
            alpha = max(alpha, val);
            if (alpha > val) column = moves[i].second;
        }

    }

    return make_pair(val, column);
}

pair<int, int> Player::min_value(char board[6][7], int depth, char player, int alpha, int
beta, int evalFunc) {
    nodes++;

    vector<pair<int, int> > moves = getValidMoves(board);
    if (depth == 0 || moves.size() == 0) {
        if (player == 'X') {
            return make_pair(-getAndComputeEval(board, player, evalFunc), -1);
        } else {
            return make_pair(getAndComputeEval(board, player, evalFunc), -1);
        }
    }

    int val = INT_MAX, column = -1;
    pair<int, int> value;
    for (int i = 0; i < moves.size(); i++) {

        if (isValidMove(board, moves[i].second)) {
            board[moves[i].first][moves[i].second] = userTag;
            value = max_value(board, depth - 1, opposite(player), alpha, beta, evalFunc);

            val = min(val, value.first);
            if (value.first == val) column = moves[i].second;

            board[moves[i].first][moves[i].second] = '*';
            if (val <= alpha) {
                return make_pair(val, column);
            }
            beta = min(beta, val);
            if (beta < val) column = moves[i].second;

        }

    }
    return make_pair(val, column);
}

void Player::aiMoveAlphaBeta(char board[6][7], int depth, int evalFunc) {
    nodes = 0;
    nodesExpanded = 0;
    stepTime = 0.0;
    auto start = high_resolution_clock::now();
    pair<int, int> slot;
    slot = alphaBetaSearch(board, depth, myTag, INT_MAX, INT_MIN, evalFunc);
    totalnodes += nodes;
    totalNodesExpanded += nodesExpanded;
    makeMove(board, slot.second);
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    stepTime = static_cast<double>(duration.count() / 1000.0);
    totalTime += stepTime;
}

void Player::aiMoveAlphaBetaForMax(char board[6][7], int depth, int evalFunc) {
    nodes = 0;
    nodesExpanded = 0;
    stepTime = 0.0;
    auto start = high_resolution_clock::now();
    pair<int, int> slot;
    slot = alphaBetaSearch(board, depth, myTag, INT_MIN, INT_MAX, evalFunc);
    totalnodes += nodes;
    totalNodesExpanded += nodesExpanded;
    makeMove(board, slot.second);
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    stepTime = static_cast<double>(duration.count() / 1000.0);
    totalTime += stepTime;
}

void Player::aiMoveMinMax(char board[6][7], int depth, int evalFunc) {
    maxdepth = depth;
    nodes = 0;
    nodesExpanded = 0;
    stepTime = 0.0;
    auto start = high_resolution_clock::now();
    pair<int, vector<pair<int, int>>> slot = minMaxAB(board, 0, 0, myTag, INT_MAX, INT_MIN, evalFunc);
    nodesExpanded = slot.second.size();
    totalnodes += nodes;
    totalNodesExpanded += nodesExpanded;
    int col = slot.second.back().first;
    makeMove(board, col);
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    stepTime = static_cast<double>(duration.count() / 1000.0);
    totalTime += stepTime;
}

void Player::checkSpaceBelow(char currentBoard[6][7]) {
    int depth = 5;
    bool playhere = false;
    do {
        if (currentBoard[depth][playChoice] != 'O' && currentBoard[depth][playChoice] != 'X') {
            playhere = true;
            currentBoard[depth][playChoice] = myTag;
        } else depth--;
    } while (!playhere);
}

char Player::opposite(char player) {
    if (player == myTag) return userTag;
    else return myTag;
}

// Evaluation function 1 to estimate  the goodness of a position
int Player::evaluateFunction1(char board[6][7], char player) {
    int starcount = 0, myTagCount = 0, opponentTagCount = 0, evaluation = 0, bestevaluation = 0;
    double danger = 0.0, totaldanger = 0, goodness = 0.0;
    int linecount = 0;
    bool tilebelow = false;
    pair<int, int> bestposition;

    for (int i = 5; i >= 0; i--) {
        for (int j = 0; j < 7; j++) {

            for (int z = 0; z < 4; z++) {
                if (j + z >= 0 && j + z <= cols) {
                    if (i < 5 && board[i + 1][j + z] != '*') tilebelow = true;
                    else if (i == 5) tilebelow = true;

                    if (board[i][j + z] == player) myTagCount++;
                    if (board[i][j + z] == opposite(player)) opponentTagCount++;
                    if (board[i][j + z] == '*') starcount++;
                    tilebelow = false;
                }
            }

            if (myTagCount > 0 && opponentTagCount > 0) {
            }
            if (myTagCount >= 2) {
                linecount++;
                double GOOD_FACTOR = myTagCount / 4.0;
                double goodFac = GOOD_FACTOR / ((4.0 - 1) / (4.0 - 1.0));
                goodness += (static_cast<double> (myTagCount)) * myTagCount * goodFac;


            }
            if (opponentTagCount >= 2) {
                linecount++;
                double DANGER_FACTOR = opponentTagCount / 4.0;
                double maxAllowPerc = (4.0 - 1.0) / 4.0;
                double percWon = opponentTagCount / 4.0;
                danger = (percWon * DANGER_FACTOR / maxAllowPerc);
                totaldanger = totaldanger + danger;
            }

            computeEval(myTagCount, opponentTagCount, starcount, evaluation, bestevaluation,
                        bestposition);
            tilebelow = false;

            for (int z = 0; z < 4; z++) {
                if (i - z >= 0 && i - z <= rows) {
                    if (board[i - z][j] == player) myTagCount++;
                    if (board[i - z][j] == opposite(player)) opponentTagCount++;
                    if (board[i - z][j] == '*') starcount++;

                }
            }
            if (myTagCount > 0 && opponentTagCount > 0) {
            }
            if (myTagCount >= 2) {

                linecount++;
                double GOOD_FACTOR = myTagCount / 4.0;
                double goodFac = GOOD_FACTOR / ((4.0 - 1) / (4.0 - 1.0));
                goodness += (static_cast<double> (myTagCount)) * myTagCount * goodFac;

            }
            if (opponentTagCount >= 2) {
                linecount++;
                double DANGER_FACTOR = opponentTagCount / 4.0;
                double maxAllowPerc = (4.0 - 1.0) / 4.0;
                double percWon = opponentTagCount / 4.0;
                danger = (percWon * DANGER_FACTOR / maxAllowPerc);
                totaldanger = totaldanger + danger;
            }
            computeEval(myTagCount, opponentTagCount, starcount, evaluation, bestevaluation,
                        bestposition);
            tilebelow = false;
            for (int z = 0; z < 4; z++) {

                if (i - z >= 0 && i - z <= rows && j + z >= 0 && j + z <= cols) {
                    if (board[i - z][j + z] == player) myTagCount++;
                    if (board[i - z][j + z] == opposite(player)) opponentTagCount++;
                    if (board[i - z][j + z] == '*') starcount++;
                    tilebelow = false;
                    if (z == 3) {
                        linecount++;
                        double DANGER_FACTOR = opponentTagCount / 4;
                        double maxAllowPerc = (4 - 1) / 4;
                        double percWon = opponentTagCount / 4;
                        danger += percWon * (DANGER_FACTOR / maxAllowPerc * 1.0);
                        double GOOD_FACTOR = myTagCount / 4;
                        double goodFac = GOOD_FACTOR / ((4 - 1) / (4 - 1));
                        goodness += myTagCount * myTagCount * goodFac * 1.0;
                    }
                }
            }
            computeEval(myTagCount, opponentTagCount, starcount, evaluation, bestevaluation,
                        bestposition);
            tilebelow = false;

            if (myTagCount > 0 && opponentTagCount > 0) {
            }
            if (myTagCount >= 2) {

                linecount++;
                double GOOD_FACTOR = myTagCount / 4.0;
                double goodFac = GOOD_FACTOR / ((4.0 - 1) / (4.0 - 1.0));
                goodness += (static_cast<double> (myTagCount)) * myTagCount * goodFac;

            }
            if (opponentTagCount >= 2) {
                linecount++;
                double DANGER_FACTOR = opponentTagCount / 4.0;
                double maxAllowPerc = (4.0 - 1.0) / 4.0;
                double percWon = opponentTagCount / 4.0;
                danger = (percWon * DANGER_FACTOR / maxAllowPerc);
                totaldanger = totaldanger + danger;
            }

            for (int z = 0; z < 4; z++) {

                if (i - z >= 0 && i - z <= rows && j - z >= 0 && j - z <= cols) {
                    if (i - z < 5 && board[i - z + 1][j - z] != '*') tilebelow = true;
                    else if (i - z == 5) tilebelow = true;
                    if (board[i - z][j - z] == player) myTagCount++;
                    if (board[i - z][j - z] == opposite(player)) opponentTagCount++;
                    if (board[i - z][j - z] == '*') starcount++;
                    tilebelow = false;
                }
            }
            computeEval(myTagCount, opponentTagCount, starcount, evaluation, bestevaluation,
                        bestposition);
            tilebelow = false;

            if (myTagCount > 0 && opponentTagCount > 0) {
            }
            if (myTagCount >= 2) {

                linecount++;
                double GOOD_FACTOR = myTagCount / 4.0;
                double goodFac = GOOD_FACTOR / ((4.0 - 1) / (4.0 - 1.0));
                goodness += (static_cast<double> (myTagCount)) * myTagCount * goodFac;

            }
            if (opponentTagCount >= 2) {
                linecount++;
                double DANGER_FACTOR = opponentTagCount / 4.0;
                double maxAllowPerc = (4.0 - 1.0) / 4.0;
                double percWon = opponentTagCount / 4.0;
                danger = (percWon * DANGER_FACTOR / maxAllowPerc);
                totaldanger = totaldanger + danger;
            }

        }

    }

    return max(danger, goodness);
}

int Player::getAndComputeEval(char board[6][7], char player, int evalFunc) {
    if (evalFunc == 1) {
        return evaluateFunction1(board, player);
    } else if (evalFunc == 2) {
        return evaluateFunction2(board);
    } else {
        return evaluateFunction3(board, player);
    }
}

void Player::computeEval(int &myTagCount, int &opponentTagCount, int &starcount,
                         int &evaluation, int &bestevaluation, pair<int, int> &bestposition) {
    if (myTagCount > 0 && opponentTagCount > 0) {
        evaluation = 0;

    } else if (myTagCount == 4) { //
        evaluation = 10000;
    } else if (opponentTagCount == 4) { //
        evaluation = -10000;
    } else if (myTagCount == 3 && starcount == 1) {
        evaluation = 400;
    } else if (opponentTagCount == 3 && starcount == 1) { //
        evaluation = -400;
    } else if (myTagCount == 2 && starcount == 2) {
        evaluation = 10;
    } else if (opponentTagCount == 2 && starcount == 2) { //
        evaluation = -10;
    } else if (myTagCount == 1 && starcount == 3) {
        evaluation = 1;
    } else if (opponentTagCount == 1 && starcount == 3) { //
        evaluation = -1;
    }
    if (abs(bestevaluation) < abs(evaluation)) {

        bestevaluation = evaluation;
    }
    evaluation = 0;
    myTagCount = 0, opponentTagCount = 0, starcount = 0;
}

bool Player::checkIfCompleteBoard(char board[6][7], int column) {
    if (board[0][column] != '*') return true;
    else return false;
}

bool Player::isValidMove(char board[6][7], int column) {
    if (column >= cols || column < 0 || checkIfCompleteBoard(board, column))
        return false;
    else {
        if (checkIfCompleteBoard(board, column))
            return false;
        else
            return true;
    }
}

double Player::getTotalMemoryUsedByPlayerInKb(int nodeCount) {
    int sizeOfEachNode = 8;
    return sizeOfEachNode * nodeCount / 1024.0;
}

int Player::computeScoreForEval3(char gameBoard[6][7], char player, int col, int row) {
    int total = 0;
    // 1. horizontal
    if (col < 5)
        if (gameBoard[row][col + 1] == player && gameBoard[row][col + 2] == player && gameBoard[row][col + 3] == player)
            total++;
    if (col < 6 && col > 0)
        if (gameBoard[row][col + 1] == player && gameBoard[row][col + 2] == player && gameBoard[row][col - 1] == player)
            total++;
    if (col < 7 && col > 1)
        if (gameBoard[row][col + 1] == player && gameBoard[row][col - 1] == player && gameBoard[row][col - 2] == player)
            total++;
    if (col > 2)
        if (gameBoard[row][col - 3] == player && gameBoard[row][col - 1] == player && gameBoard[row][col - 2] == player)
            total++;

    // 2. vertical
    if (row < 3)
        if (gameBoard[row + 1][col] == player && gameBoard[row + 2][col] == player && gameBoard[row + 3][col] == player)
            total++;

    // 3. up-left -- down-right diag
    if (row < 3 && col < 5)
        if (gameBoard[row + 1][col + 1] == player && gameBoard[row + 2][col + 2] == player &&
            gameBoard[row + 3][col + 3] == player)
            total++;
    if (row < 4 && row > 0 && col < 6 && col > 0)
        if (gameBoard[row + 1][col + 1] == player && gameBoard[row + 2][col + 2] == player &&
            gameBoard[row - 1][col - 1] == player)
            total++;
    if (row < 5 && row > 1 && col < 7 && col > 1)
        if (gameBoard[row + 1][col + 1] == player && gameBoard[row - 2][col - 2] == player &&
            gameBoard[row - 1][col - 1] == player)
            total++;
    if (row > 2 && col > 2)
        if (gameBoard[row - 3][col - 3] == player && gameBoard[row - 2][col - 2] == player &&
            gameBoard[row - 1][col - 1] == player)
            total++;
    // 4. up-right -- down-left diag
    if (row > 2 && col < 5)
        if (gameBoard[row - 1][col + 1] == player && gameBoard[row - 2][col + 2] == player &&
            gameBoard[row - 3][col + 3] == player)
            total++;
    if (row < 5 && row > 1 && col < 6 && col > 0)
        if (gameBoard[row + 1][col - 1] == player && gameBoard[row - 2][col + 2] == player &&
            gameBoard[row - 1][col + 1] == player)
            total++;
    if (row < 4 && row > 0 && col < 7 && col > 1)
        if (gameBoard[row - 1][col + 1] == player && gameBoard[row + 2][col - 2] == player &&
            gameBoard[row + 1][col - 1] == player)
            total++;
    if (row < 3 && col > 2)
        if (gameBoard[row + 3][col - 3] == player && gameBoard[row + 2][col - 2] == player &&
            gameBoard[row + 1][col - 1] == player)
            total++;

    return total;
}

// Evaluation Function 3 to estimate the value or goodness of a position
int Player::evaluateFunction3(char board[6][7], char player) {
    int nextRow;
    int score = 0;
    for (int i = 0; i < 7; ++i) { // linearly go from left to right to eval all
        for (int row = 5; row >= 0; row--) {
            if (board[row][i] == '*') {
                nextRow = row;
                break;
            }
        }
        if (nextRow < 1)
            continue; // because the threat shouldnot be the immediate step. If it is less than 1, it is the last step can take
        for (int j = 0; j < nextRow; ++j)
            score += computeScoreForEval3(board, player, i, j);
    }

    return score;
}
