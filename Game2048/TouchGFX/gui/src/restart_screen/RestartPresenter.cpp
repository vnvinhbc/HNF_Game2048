#include <gui/restart_screen/RestartView.hpp>
#include <gui/restart_screen/RestartPresenter.hpp>

RestartPresenter::RestartPresenter(RestartView& v)
    : view(v)
{

}

void RestartPresenter::activate()
{

}

void RestartPresenter::deactivate()
{

}

// Thêm đoạn này vào cuối file
int RestartPresenter::getScore() {
    return model->getScore();
}

int RestartPresenter::getBestScore() {
    return model->getBestScore();
}

void RestartPresenter::resetGame() {
    model->resetGame();
}

void RestartPresenter::requestResetGame()
{
    model->resetGame(); // Đây chính là hàm reset() bạn đã viết sẵn trong Model hôm trước!
}
