#ifndef RANKINGSCREENVIEW_HPP
#define RANKINGSCREENVIEW_HPP

#include <gui_generated/rankingscreen_screen/RankingScreenViewBase.hpp>
#include <gui/rankingscreen_screen/RankingScreenPresenter.hpp>

class RankingScreenView : public RankingScreenViewBase
{
public:
    RankingScreenView();
    virtual ~RankingScreenView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
protected:
};

#endif // RANKINGSCREENVIEW_HPP
