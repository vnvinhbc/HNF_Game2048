#ifndef STARTGAMEPRESENTER_HPP
#define STARTGAMEPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class StartGameView;

class StartGamePresenter : public touchgfx::Presenter, public ModelListener
{
public:
    StartGamePresenter(StartGameView& v);

    virtual void activate();

    virtual void deactivate();

    virtual ~StartGamePresenter() {}

    void startNewGame();

private:
    StartGamePresenter();

    StartGameView& view;
};

#endif // STARTGAMEPRESENTER_HPP
