#include <gui/gamescreen_screen/GameScreenView.hpp>
#include <gui/gamescreen_screen/GameScreenPresenter.hpp>

GameScreenPresenter::GameScreenPresenter(GameScreenView& v)
    : view(v)
{

}

void GameScreenPresenter::activate()
{

}

void GameScreenPresenter::deactivate()
{

}


// Khi View mới khởi tạo (hàm setupScreen), nó sẽ gọi hàm này để xin số liệu
void GameScreenPresenter::requestBoardUpdate()
{
    view.updateBoardUI();
}

// Khi Model báo bảng số đã thay đổi, báo View vẽ lại ngay
void GameScreenPresenter::boardUpdated()
{
    view.updateBoardUI();
}

// Khi Model báo Game Over, báo View hiện thông báo/chuyển màn hình
void GameScreenPresenter::gameHasEnded()
{
    view.showGameOver();
}

// Cầu nối lấy dữ liệu (View -> Presenter -> Model)
int GameScreenPresenter::getCell(int r, int c)
{
    return model->getCell(r, c);
}

int GameScreenPresenter::getScore()
{
    return model->getScore();
}

void GameScreenPresenter::requestUndo()
{
    if (model->undo()) // Nếu Model đồng ý cho lùi
    {
        view.updateBoardUI(); // Vẽ lại bàn cờ cũ
        view.updateUndoCount(); // Cập nhật lại con số 3,2,1,0
    }
}

int GameScreenPresenter::getUndoCount()
{
    return model->getUndoCount();
}
