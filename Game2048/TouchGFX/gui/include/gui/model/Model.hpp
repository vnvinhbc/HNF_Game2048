#ifndef MODEL_HPP
#define MODEL_HPP

#include "Game2048.hpp"

class ModelListener;

class Model
{
public:
    Model();

    void bind(ModelListener* listener)
    {
        modelListener = listener;
    }

    void tick();

    // CHỈ KHAI BÁO HÀM Ở ĐÂY (bỏ chữ Model:: đi)
    void resetGame();
    int getCell(int r, int c);
    int getScore();
    int getBestScore();

    void saveState();        // Hàm lưu quá khứ
    bool undo();             // Hàm thực hiện lùi bước
    int getUndoCount();      // Hàm lấy số lần lùi còn lại

    void saveScoreToRanking(int currentScore); // Hàm chèn điểm vào Top 5
    int getRankScore(int index);               // Hàm lấy điểm ra theo Top (0 đến 4)
protected:
    ModelListener* modelListener;

private:
    // BẮT BUỘC PHẢI KHAI BÁO CÁC BIẾN NÀY ĐỂ BÊN .CPP CÓ THỂ GỌI
    Game2048 game;
    int bestScore;
    bool joystickReady; // Đưa biến này vào trong class cho gọn gàng và an toàn

    int previousBoard[4][4]; // Mảng nhớ bàn cờ ván trước
    int previousScore;       // Mảng nhớ điểm ván trước
    int undoCount;           // Số lượt undo còn lại (3)
    bool canUndo;            // Cờ cho phép undo (tránh lùi 2 lần liên tiếp)

    int topScores[5];    // Mảng lưu 5 điểm cao nhất
    bool isScoreSaved;   // Cờ chặn việc chèn điểm liên tục khi game over
};

#endif // MODEL_HPP
