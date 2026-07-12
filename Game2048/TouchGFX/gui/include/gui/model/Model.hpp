#ifndef MODEL_HPP
#define MODEL_HPP

#include <stdint.h>
#include <stdlib.h>

class ModelListener;

class Model
{
public:

	enum GameState {
	        STATE_START_SCREEN,
	        STATE_PLAYING,
	        STATE_GAME_OVER
	    };

    Model();

    void bind(ModelListener* listener) { modelListener = listener; }
    void tick();

    void initGame();
    bool isGameOver();
    void setGameState(GameState state) { currentState = state; }
    GameState getGameState() { return currentState; }
    void spawnRandomTwo();

    bool moveLeft();
    bool moveRight();
    bool moveUp();
    bool moveDown();

    int getTileValue(int row, int col) { return board[row][col]; }
    int getScore() { return score; }

    void simulateJoystick(uint8_t dir);

protected:
    ModelListener* modelListener;

private:
    int board[4][4];
    int score;
    GameState currentState;
    bool slideAndMergeRow(int* line);
};

#endif // MODEL_HPP
