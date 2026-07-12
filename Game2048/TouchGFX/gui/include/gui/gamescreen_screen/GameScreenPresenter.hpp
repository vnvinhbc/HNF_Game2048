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

    virtual void activate();
    virtual void deactivate();
    virtual ~GameScreenPresenter() {}

    void gameOverTriggered() override;

    void simulateJoystick(uint8_t dir);


    void joystickMoved(uint8_t direction) override;

    int getTileValue(int row, int col);
    int getScore();

private:
    GameScreenPresenter();

    GameScreenView& view;
};

#endif // GAMESCREENPRESENTER_HPP
