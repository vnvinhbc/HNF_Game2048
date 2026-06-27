#ifndef RESTARTVIEW_HPP
#define RESTARTVIEW_HPP

#include <gui_generated/restart_screen/RestartViewBase.hpp>
#include <gui/restart_screen/RestartPresenter.hpp>

class RestartView : public RestartViewBase
{
public:
    RestartView();
    virtual ~RestartView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
protected:
};

#endif // RESTARTVIEW_HPP
