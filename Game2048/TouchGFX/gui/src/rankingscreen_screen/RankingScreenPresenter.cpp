#include <gui/rankingscreen_screen/RankingScreenView.hpp>
#include <gui/rankingscreen_screen/RankingScreenPresenter.hpp>

RankingScreenPresenter::RankingScreenPresenter(RankingScreenView& v)
    : view(v)
{

}

void RankingScreenPresenter::activate()
{

}

void RankingScreenPresenter::deactivate()
{

}

int RankingScreenPresenter::getRankScore(int index)
{
    return model->getRankScore(index);
}
