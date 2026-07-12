#ifndef GAME2048_HPP
#define GAME2048_HPP

#include <stdlib.h>

class Game2048 {
public:
    Game2048();
    void reset();
    bool moveLeft();
    bool moveRight();
    bool moveUp();
    bool moveDown();
    int getCell(int row, int col);
    int getScore();
    bool isGameOver();

    // THÊM MỚI: 2 hàm Setter phục vụ tính năng Undo
    void setCell(int row, int col, int val);
    void setScore(int s);

private:
    int board[4][4];
    int score;
    void addRandomTile();
    bool slideLeft();
    void rotateRight();
};

#endif