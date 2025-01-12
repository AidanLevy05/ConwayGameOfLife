#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <thread>
#include <chrono>
#include <random>
#include <omp.h>
using namespace std;

void clear() { system("clear"); }
void getBoardSize(int&);
void getNumTrials(int&);
void getUpdateSpeed(int&, int);
void preset(int&);
void progressBar(int, int);
void populateBoard(vector<vector<int>>&);
void printBoard(vector<vector<int>>&);
void updateBoard(vector<vector<int>>&);
int numAliveNeighbors(vector<vector<int>>&, int, int);
bool hasNeighbors(int, int, int);

int main() {

    int boardSize = -1;
    int numTrials;
    int ms;
    int presetChoice = -1;
    getBoardSize(boardSize);
    getNumTrials(numTrials);
    getUpdateSpeed(ms, numTrials);
    preset(presetChoice);
    vector<vector<int>> board(boardSize, vector<int>(boardSize, 0));

    if (presetChoice == 1) {
        int midR, midC;
        midR = midC = boardSize/2;

        board[midR][midC] = 1;
        board[midR+1][midC] = 1;
        board[midR-1][midC] = 1;
        board[midR][midC-1] = 1;
        board[midR+1][midC+1] =1;
    }
    else if (presetChoice == 2)
        populateBoard(board);

    clear();

    for (int i = 0; i <= numTrials; i++) {
        this_thread::sleep_for(chrono::milliseconds(ms));
        printBoard(board);
        progressBar(i, numTrials);
        if (i < numTrials) 
            updateBoard(board);
    }

    return 0;
}

void getBoardSize(int& size) {
    do {
        cout << "Please enter the size of the board! [10-100]" << endl;
        cin >> size;
    } while (size < 10 || size > 100);
}

void getNumTrials(int& trials) {
    do {
        cout << "Please enter the number of trials! [0-1000]" << endl;
        cin >> trials;
    } while (trials < 0 || trials > 1000);
}

void getUpdateSpeed(int& ms, int numTrials) {
    int option = -1;
    do {
        cout << "Please select an update speed:" << endl;
        cout << "1. Fast" << endl;
        cout << "2. Medium" << endl;
        cout << "3. Slow" << endl;
        cin >> option;
    } while (option < 1 || option > 3);

    if (option == 1) ms = 1000/numTrials;
    else if (option == 2) ms = 2500/numTrials;
    else ms = 5000/numTrials;
}

void progressBar(int current, int max) {
    int barWidth = 20;
    double progress = (1.0*current) / max;
    int perFinished = round(progress*barWidth);
    int perUnfinished = barWidth-perFinished;

    if (current == max) perFinished = barWidth;

    cout << "[";
    for (int i = 0; i < perFinished; i++) 
        cout << "#";
    for (int i = 0; i < perUnfinished; i++)
        cout << "-";
    cout << "] " << int(progress*100) << "%" << endl;
}

void preset(int& c) {
    do {
        cout << "Please choose a preset choice:" << endl;
        cout << "1. Preset #1" << endl;
        cout << "2. Random" << endl;
        cin >> c;
    } while (c < 1 || c > 2);
}

void populateBoard(vector<vector<int>>& board) {

    // rand() is not safe for multithreading
    // as it leads to race conditions.

    #pragma omp parallel 
    {
        random_device rd;
        mt19937 gen(rd() + omp_get_thread_num());
        uniform_int_distribution<int> dist(0, 1);

        #pragma omp for
        for (size_t i = 0; i < board.size(); i++) {
            for (size_t j = 0; j < board[i].size(); j++) {
                board[i][j] = dist(gen);
            }
        }
    }
}

void printBoard(vector<vector<int>>& board) {
    clear();
    for (size_t i = 0; i < board.size(); i++) {
        for (size_t j = 0; j < board[i].size(); j++) {
            if (board[i][j] == 1) cout << " █ ";
            else cout << " . ";
        }
        cout << endl;
    }
}

void updateBoard(vector<vector<int>>& board) {

    vector<vector<int>> temp = board;

    for (size_t i = 0; i < board.size(); i++) {
        for (size_t j = 0; j < board[i].size(); j++) {
            
            // only call numAliveNeigbors if the 
            // current cell has neighbors that
            // are accessible
            if (hasNeighbors(i, j, board.size())) {
                int numAlive = numAliveNeighbors(board, i, j);

                if (board[i][j] == 1 && (numAlive<2 || numAlive>3))
                    temp[i][j] = 0;
                else if (board[i][j] == 0 && numAlive == 3)
                    temp[i][j] = 1; 

                
            } else {
                
            }

        }
    }

    board = temp;
}

int numAliveNeighbors(vector<vector<int>>& board, int row, int col) {
    int counter = 0;
    int boardSize = board.size();

    for (int i = -1; i <=1; i++) {
        for (int j = -1; j<= 1; j++) {
            if (i==0 && j==0) continue;
            int ni = row+i;
            int nj = col+j;
            if (ni >= 0 && ni < boardSize && nj >= 0 && nj < boardSize)
                counter += board[ni][nj];
        }
    }

    return counter;

}

bool hasNeighbors(int row, int col, int boardSize) {
    return !(row < 1 || col < 1 || row >= boardSize-1 || col >= boardSize-1);
}
