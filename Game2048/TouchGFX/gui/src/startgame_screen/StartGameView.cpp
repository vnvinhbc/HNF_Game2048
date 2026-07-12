#include <gui/startgame_screen/StartGameView.hpp>

void StartGameView::startGameBtnClicked()
{
    presenter->startNewGame();
    application().gotoGameScreenScreenNoTransition();
}

StartGameView::StartGameView()
{

}

void StartGameView::setupScreen()
{
    StartGameViewBase::setupScreen();
}

void StartGameView::tearDownScreen()
{
    StartGameViewBase::tearDownScreen();
}
