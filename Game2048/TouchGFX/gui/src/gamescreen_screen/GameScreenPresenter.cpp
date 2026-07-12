#include <gui/gamescreen_screen/GameScreenView.hpp>
#include <gui/gamescreen_screen/GameScreenPresenter.hpp>
#include <gui/common/FrontendApplication.hpp>

GameScreenPresenter::GameScreenPresenter(GameScreenView& v)
    : view(v)
{
}

void GameScreenPresenter::activate()
{
    view.updateBoard();
}

void GameScreenPresenter::deactivate()
{
}
void GameScreenPresenter::joystickMoved(uint8_t direction)
{
    view.updateBoard();
}

int GameScreenPresenter::getTileValue(int row, int col)
{
    return model->getTileValue(row, col);
}

int GameScreenPresenter::getScore()
{
    return model->getScore();
}

void GameScreenPresenter::simulateJoystick(uint8_t dir)
{
    model->simulateJoystick(dir);
}

void GameScreenPresenter::gameOverTriggered()
{
    static_cast<FrontendApplication*>(touchgfx::Application::getInstance())->gotoRestartScreenNoTransition();
}
