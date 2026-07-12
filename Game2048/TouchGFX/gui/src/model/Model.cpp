#include <gui/model/Model.hpp>
#include <gui/model/ModelListener.hpp>
#include <stdint.h>

// Đọc dữ liệu từ main.c
extern volatile uint16_t joystick_data[2];

// Khởi tạo thêm undoCount = 3 và canUndo = false
Model::Model() : modelListener(0), bestScore(0), joystickReady(true), undoCount(3), canUndo(false)
{
    // TouchGFX sẽ gọi hàm này 1 lần khi cấp nguồn
	for(int i=0; i<5; i++) topScores[i] = 0;
	isScoreSaved = false;
}

void Model::tick() {
    uint16_t x_val = joystick_data[0]; // Trục X
    uint16_t y_val = joystick_data[1]; // Trục Y

    if (x_val < 500 || x_val > 3500 || y_val < 500 || y_val > 3500) {
        if (joystickReady) {

            // 1. LƯU NHÁP QUÁ KHỨ (Lưu trước khi di chuyển)
            int tempBoard[4][4];
            int tempScore = game.getScore();
            for (int i = 0; i < 4; i++) {
                for (int j = 0; j < 4; j++) {
                    tempBoard[i][j] = game.getCell(i, j);
                }
            }

            // 2. THỰC HIỆN DI CHUYỂN
            bool moved = false;
            if (x_val < 500)       moved = game.moveLeft();
            else if (x_val > 3500) moved = game.moveRight();
            else if (y_val < 500)  moved = game.moveUp();
            else if (y_val > 3500) moved = game.moveDown();

            // 3. CHỈ KHI NÀO DI CHUYỂN THÀNH CÔNG THÌ MỚI CHỐT LƯU QUÁ KHỨ
            if (moved) {
                if (undoCount > 0) {
                    for (int i = 0; i < 4; i++) {
                        for (int j = 0; j < 4; j++) {
                            previousBoard[i][j] = tempBoard[i][j];
                        }
                    }
                    previousScore = tempScore;
                    canUndo = true; // Bật cờ cho phép người chơi ấn nút Undo
                }

                // Kiểm tra xem có phá kỷ lục không
                if (game.getScore() > bestScore) {
                    bestScore = game.getScore();
                }

                // Báo cho UI cập nhật
                if (modelListener != 0) {
                    modelListener->boardUpdated();
                }

                // Kiểm tra thua
                if (game.isGameOver()) {
                    if (modelListener != 0) modelListener->gameHasEnded();
                }
            }
            joystickReady = false;
        }
    } else {
        joystickReady = true;
    }
}

// =======================================================
// THÂN CÁC HÀM TỪ .HPP ĐƯỢC CHUYỂN SANG ĐÂY
// =======================================================
void Model::resetGame() {
	// THÊM MỚI CHỖ NÀY: Lưu điểm trước khi dọn dẹp bàn cờ (Chỉ lưu nếu có cày được điểm)
	    if (game.getScore() > 0) {
	        saveScoreToRanking(game.getScore());
	    }

    game.reset();
    undoCount = 3;   // Reset lại 3 lượt undo khi chơi ván mới
    canUndo = false;

    if(modelListener != 0) {
        modelListener->boardUpdated();
    }
    isScoreSaved = false;
}

int Model::getCell(int r, int c) {
    return game.getCell(r, c);
}

int Model::getScore() {
    return game.getScore();
}

int Model::getBestScore() {
    return bestScore;
}

// =======================================================
// THÊM MỚI: CÁC HÀM CHO TÍNH NĂNG UNDO
// =======================================================
bool Model::undo() {
    // Nếu được phép lùi và vẫn còn lượt
    if (canUndo && undoCount > 0) {
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                // Đẩy dữ liệu quá khứ ngược trở lại vào đối tượng game
                game.setCell(i, j, previousBoard[i][j]);
            }
        }
        game.setScore(previousScore);

        canUndo = false; // Đã lùi rồi thì không lùi 2 lần liên tiếp được
        undoCount--;     // Trừ đi 1 lượt
        return true;
    }
    return false;
}

int Model::getUndoCount() {
    return undoCount;
}

void Model::saveScoreToRanking(int currentScore)
{
    // Thuật toán: Tìm vị trí xứng đáng, chèn vào và đẩy các điểm thấp hơn xuống dưới
    for (int i = 0; i < 5; i++)
    {
        if (currentScore > topScores[i])
        {
            // Đẩy các điểm thấp hơn lùi xuống 1 bậc
            for (int j = 4; j > i; j--)
            {
                topScores[j] = topScores[j - 1];
            }
            // Chèn điểm mới vào vị trí top
            topScores[i] = currentScore;
            break; // Đã chèn xong thì thoát vòng lặp
        }
    }
}

int Model::getRankScore(int index)
{
    if (index >= 0 && index < 5) return topScores[index];
    return 0;
}
