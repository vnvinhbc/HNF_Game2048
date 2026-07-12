#ifndef HOWTOPLAYVIEW_HPP
#define HOWTOPLAYVIEW_HPP

#include <gui_generated/howtoplay_screen/HowToPlayViewBase.hpp>
#include <gui/howtoplay_screen/HowToPlayPresenter.hpp>

class HowToPlayView : public HowToPlayViewBase
{
public:
    HowToPlayView();
    virtual ~HowToPlayView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
protected:
};

#endif // HOWTOPLAYVIEW_HPP
