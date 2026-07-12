#include <gui/model/Game2048.hpp>

Game2048::Game2048() { reset(); }

void Game2048::reset() {
    score = 0;
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            board[i][j] = 0;
    addRandomTile();
    addRandomTile();
}

void Game2048::addRandomTile() {
    int emptyCells[16][2];
    int count = 0;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (board[i][j] == 0) {
                emptyCells[count][0] = i;
                emptyCells[count][1] = j;
                count++;
            }
        }
    }
    if (count > 0) {
        int index = rand() % count;
        board[emptyCells[index][0]][emptyCells[index][1]] = (rand() % 10 < 9) ? 2 : 4; 
    }
}

bool Game2048::slideLeft() {
    bool moved = false;
    for (int i = 0; i < 4; i++) {
        int writePos = 0, lastMergedPos = -1;
        for (int j = 0; j < 4; j++) {
            if (board[i][j] != 0) {
                if (writePos > 0 && board[i][writePos - 1] == board[i][j] && lastMergedPos != writePos - 1) {
                    board[i][writePos - 1] *= 2;
                    score += board[i][writePos - 1];
                    board[i][j] = 0;
                    lastMergedPos = writePos - 1;
                    moved = true;
                } else {
                    if (writePos != j) {
                        board[i][writePos] = board[i][j];
                        board[i][j] = 0;
                        moved = true;
                    }
                    writePos++;
                }
            }
        }
    }
    return moved;
}

void Game2048::rotateRight() {
    int temp[4][4];
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            temp[j][3 - i] = board[i][j];
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            board[i][j] = temp[i][j];
}

bool Game2048::moveLeft()  { bool moved = slideLeft(); if(moved) addRandomTile(); return moved; }
bool Game2048::moveRight() { rotateRight(); rotateRight(); bool moved = slideLeft(); rotateRight(); rotateRight(); if(moved) addRandomTile(); return moved; }
bool Game2048::moveUp()    { rotateRight(); rotateRight(); rotateRight(); bool moved = slideLeft(); rotateRight(); if(moved) addRandomTile(); return moved; }
bool Game2048::moveDown()  { rotateRight(); bool moved = slideLeft(); rotateRight(); rotateRight(); rotateRight(); if(moved) addRandomTile(); return moved; }

int Game2048::getCell(int row, int col) { return board[row][col]; }
int Game2048::getScore() { return score; }

bool Game2048::isGameOver() {
    for(int i=0; i<4; i++) for(int j=0; j<4; j++) if(board[i][j] == 0) return false;
    for(int i=0; i<4; i++) {
        for(int j=0; j<4; j++) {
            if(j < 3 && board[i][j] == board[i][j+1]) return false;
            if(i < 3 && board[i][j] == board[i+1][j]) return false;
        }
    }
    return true;
}

// THÊM MỚI: 2 HÀM SETTER CHO TÍNH NĂNG UNDO
// =======================================================
void Game2048::setCell(int row, int col, int val) {
    board[row][col] = val;
}

void Game2048::setScore(int s) {
    score = s;
}