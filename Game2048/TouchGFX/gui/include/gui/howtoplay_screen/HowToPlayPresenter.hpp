#ifndef HOWTOPLAYPRESENTER_HPP
#define HOWTOPLAYPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class HowToPlayView;

class HowToPlayPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    HowToPlayPresenter(HowToPlayView& v);

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

    virtual ~HowToPlayPresenter() {}

private:
    HowToPlayPresenter();

    HowToPlayView& view;
};

#endif // HOWTOPLAYPRESENTER_HPP
