#ifndef RANKINGSCREENPRESENTER_HPP
#define RANKINGSCREENPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class RankingScreenView;

class RankingScreenPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    RankingScreenPresenter(RankingScreenView& v);

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

    virtual ~RankingScreenPresenter() {}

    int getRankScore(int index);

private:
    RankingScreenPresenter();

    RankingScreenView& view;
};

#endif // RANKINGSCREENPRESENTER_HPP
