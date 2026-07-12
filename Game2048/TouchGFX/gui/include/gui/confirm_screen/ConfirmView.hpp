#ifndef CONFIRMVIEW_HPP
#define CONFIRMVIEW_HPP

#include <gui_generated/confirm_screen/ConfirmViewBase.hpp>
#include <gui/confirm_screen/ConfirmPresenter.hpp>

class ConfirmView : public ConfirmViewBase
{
public:
    ConfirmView();
    virtual ~ConfirmView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
protected:
};

#endif // CONFIRMVIEW_HPP
