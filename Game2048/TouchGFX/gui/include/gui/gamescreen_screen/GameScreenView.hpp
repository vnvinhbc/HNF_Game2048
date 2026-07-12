#ifndef GAMESCREENVIEW_HPP
#define GAMESCREENVIEW_HPP

#include <gui_generated/gamescreen_screen/GameScreenViewBase.hpp>
#include <gui/gamescreen_screen/GameScreenPresenter.hpp>

class GameScreenView : public GameScreenViewBase
{
public:
    GameScreenView();
    virtual ~GameScreenView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();

    virtual void updateBoard();

    virtual void handleKeyEvent(uint8_t key);

protected:

    touchgfx::Box* tileBoxes[4][4];
    touchgfx::TextAreaWithOneWildcard* tileTexts[4][4];
    touchgfx::Unicode::UnicodeChar tileBuffers[4][4][6];
    colortype getColorForValue(int value);
};

#endif // GAMESCREENVIEW_HPP
