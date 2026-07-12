#include <gui/rankingscreen_screen/RankingScreenView.hpp>

RankingScreenView::RankingScreenView()
{

}

void RankingScreenView::setupScreen()
{
    RankingScreenViewBase::setupScreen();
    // 1. Lấy điểm Top 1 và in ra (Lưu ý: index từ 0 đến 4)
        Unicode::snprintf(txtRank1Buffer, TXTRANK1_SIZE, "%d", presenter->getRankScore(0));
        txtRank1.invalidate();

        // 2. Lấy điểm Top 2 và in ra
        Unicode::snprintf(txtRank2Buffer, TXTRANK2_SIZE, "%d", presenter->getRankScore(1));
        txtRank2.invalidate();

        // 3. Lấy điểm Top 3 và in ra
        Unicode::snprintf(txtRank3Buffer, TXTRANK3_SIZE, "%d", presenter->getRankScore(2));
        txtRank3.invalidate();

        // 4. Lấy điểm Top 4 và in ra
        Unicode::snprintf(txtRank4Buffer, TXTRANK4_SIZE, "%d", presenter->getRankScore(3));
        txtRank4.invalidate();

        // 5. Lấy điểm Top 5 và in ra
        Unicode::snprintf(txtRank5Buffer, TXTRANK5_SIZE, "%d", presenter->getRankScore(4));
        txtRank5.invalidate();
}

void RankingScreenView::tearDownScreen()
{
    RankingScreenViewBase::tearDownScreen();
}
