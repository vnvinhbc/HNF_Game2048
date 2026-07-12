#include <gui/startgame_screen/StartGameView.hpp>
#include <gui/startgame_screen/StartGamePresenter.hpp>

StartGamePresenter::StartGamePresenter(StartGameView& v)
    : view(v)
{

}

void StartGamePresenter::startNewGame()
{
    model->initGame();
}

void StartGamePresenter::activate()
{

}

void StartGamePresenter::deactivate()
{

}
