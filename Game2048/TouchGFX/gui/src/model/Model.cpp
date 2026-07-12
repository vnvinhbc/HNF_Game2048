#include <gui/model/Model.hpp>
#include <gui/model/ModelListener.hpp>

#ifndef SIMULATOR
#include "cmsis_os2.h"
extern "C" {
    extern osMessageQueueId_t joyQueueHandle;
}
#endif

Model::Model() : modelListener(0), score(0), currentState(STATE_START_SCREEN)
{

}

void Model::tick()
{
#ifndef SIMULATOR
    uint8_t joystick_dir = 0;
    while (osMessageQueueGet(joyQueueHandle, &joystick_dir, 0, 0) == osOK)
    {
        if (joystick_dir != 0 && modelListener != 0)
        {
            bool hasChanged = false;

            // 1(UP), 2(DOWN), 3(LEFT), 4(RIGHT)
            if (joystick_dir == 1) hasChanged = moveUp();
            else if (joystick_dir == 2) hasChanged = moveDown();
            else if (joystick_dir == 3) hasChanged = moveLeft();
            else if (joystick_dir == 4) hasChanged = moveRight();

            if (hasChanged)
            {
                spawnRandomTwo();
                modelListener->joystickMoved(joystick_dir);
            }
        }
    }
#endif
}

// ==========================================
// THUẬT TOÁN CORE GAME 2048
// ==========================================

void Model::initGame()
{
    score = 0;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) board[i][j] = 0;
    }
    spawnRandomTwo();
    spawnRandomTwo();
    currentState = STATE_PLAYING;
}

void Model::spawnRandomTwo()
{
    int emptyRows[16];
    int emptyCols[16];
    int emptyCount = 0;

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (board[i][j] == 0) {
                emptyRows[emptyCount] = i;
                emptyCols[emptyCount] = j;
                emptyCount++;
            }
        }
    }

    if (emptyCount > 0) {
        int randomIndex = rand() % emptyCount;
        int r = emptyRows[randomIndex];
        int c = emptyCols[randomIndex];
        board[r][c] = ((rand() % 10) == 0) ? 4 : 2;
    }
}

bool Model::isGameOver()
{
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (board[i][j] == 0) return false;
        }
    }

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] == board[i][j + 1]) return false;
        }
    }

    for (int j = 0; j < 4; j++) {
        for (int i = 0; i < 3; i++) {
            if (board[i][j] == board[i + 1][j]) return false;
        }
    }

    return true;
}

bool Model::slideAndMergeRow(int* line)
{
    bool changed = false;
    int writeIdx = 0;
    for (int i = 0; i < 4; i++) {
        if (line[i] != 0) {
            if (i != writeIdx) {
                line[writeIdx] = line[i];
                line[i] = 0;
                changed = true;
            }
            writeIdx++;
        }
    }

    for (int i = 0; i < 3; i++) {
        if (line[i] != 0 && line[i] == line[i+1]) {
            line[i] *= 2;
            score += line[i];
            line[i+1] = 0;
            changed = true;
        }
    }

    writeIdx = 0;
    for (int i = 0; i < 4; i++) {
        if (line[i] != 0) {
            if (i != writeIdx) {
                line[writeIdx] = line[i];
                line[i] = 0;
            }
            writeIdx++;
        }
    }
    return changed;
}

bool Model::moveLeft()
{
    bool changed = false;
    for (int i = 0; i < 4; i++) {
        changed |= slideAndMergeRow(board[i]);
    }
    return changed;
}

bool Model::moveRight()
{
    bool changed = false;
    for (int i = 0; i < 4; i++) {
        int reversed[4] = {board[i][3], board[i][2], board[i][1], board[i][0]};
        if (slideAndMergeRow(reversed)) {
            board[i][0] = reversed[3];
            board[i][1] = reversed[2];
            board[i][2] = reversed[1];
            board[i][3] = reversed[0];
            changed = true;
        }
    }
    return changed;
}

bool Model::moveUp()
{
    bool changed = false;
    for (int j = 0; j < 4; j++) {
        int col[4] = {board[0][j], board[1][j], board[2][j], board[3][j]};
        if (slideAndMergeRow(col)) {
            board[0][j] = col[0];
            board[1][j] = col[1];
            board[2][j] = col[2];
            board[3][j] = col[3];
            changed = true;
        }
    }
    return changed;
}

bool Model::moveDown()
{
    bool changed = false;
    for (int j = 0; j < 4; j++) {
        int col[4] = {board[3][j], board[2][j], board[1][j], board[0][j]};
        if (slideAndMergeRow(col)) {
            board[0][j] = col[3];
            board[1][j] = col[2];
            board[2][j] = col[1];
            board[3][j] = col[0];
            changed = true;
        }
    }
    return changed;
}

void Model::simulateJoystick(uint8_t dir)
{
    if (currentState != STATE_PLAYING) return;

    bool hasChanged = false;
    if (dir == 1) hasChanged = moveUp();
    else if (dir == 2) hasChanged = moveDown();
    else if (dir == 3) hasChanged = moveLeft();
    else if (dir == 4) hasChanged = moveRight();

    if (hasChanged)
    {
        spawnRandomTwo();

        if (isGameOver())
        {
            currentState = STATE_GAME_OVER;

            if (modelListener != 0) {
                modelListener->gameOverTriggered();
            }
            return;
        }

        if (modelListener != 0) {
            modelListener->joystickMoved(dir);
        }
    }
}
