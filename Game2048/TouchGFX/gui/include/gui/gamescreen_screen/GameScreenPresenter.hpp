#ifndef GAMESCREENPRESENTER_HPP
#define GAMESCREENPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class GameScreenView;

class GameScreenPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    GameScreenPresenter(GameScreenView& v);

    /**
     * The activate function is called automatically when this screen is "switched in"
     * (ie. made active). Initialization logic can be placed here.
     */
    virtual void activate();

    /**
     * The deactivate function is called automatically when this screen is "switched out"
     * (ie. made inactive). Teardown functionality can be placed here.
     */
    virtual void deactivate();

    virtual ~GameScreenPresenter() {}


    virtual void boardUpdated();
    virtual void gameHasEnded();

        // ====================================================
        // 2. Các hàm trung gian để View gọi lấy dữ liệu từ Model
        // ====================================================
        void requestBoardUpdate();
        int getCell(int r, int c);
        int getScore();
        void requestUndo();
        int getUndoCount();

private:
    GameScreenPresenter();

    GameScreenView& view;
};

#endif // GAMESCREENPRESENTER_HPP
