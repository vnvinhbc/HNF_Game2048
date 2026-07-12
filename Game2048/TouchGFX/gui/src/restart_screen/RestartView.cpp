#include <gui/restart_screen/RestartView.hpp>
#include <gui/common/FrontendApplication.hpp>

RestartView::RestartView()
{
}

void RestartView::restartBtnClicked()
{
    presenter->startNewGame();
    application().gotoGameScreenScreenNoTransition();
}

void RestartView::setupScreen()
{
    RestartViewBase::setupScreen();
}

void RestartView::tearDownScreen()
{
    RestartViewBase::tearDownScreen();
}
