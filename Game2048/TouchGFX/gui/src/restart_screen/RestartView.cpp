#include <gui/restart_screen/RestartView.hpp>

RestartView::RestartView()
{

}

void RestartView::tearDownScreen()
{
    RestartViewBase::tearDownScreen();
}

void RestartView::setupScreen()
{
    RestartViewBase::setupScreen();

    // =========================================================
    // [3. SỬA TÊN BIẾN]: TextArea hiển thị Điểm Số Của Ván Vừa Chơi
    // Chỗ số "0" to đùng dưới chữ SCORE. Giả sử tên nó là txtFinalScore
    // =========================================================
    Unicode::snprintf(textArea3Buffer, TEXTAREA3_SIZE, "%d", presenter->getScore());
    textArea3.invalidate();

    // =========================================================
    // [4. SỬA TÊN BIẾN]: TextArea hiển thị Kỷ Lục
    // Chỗ số "0" to đùng dưới chữ BEST SCORE. Giả sử tên nó là txtBestScore
    // =========================================================
    Unicode::snprintf(textArea3_1Buffer, TEXTAREA3_1_SIZE, "%d", presenter->getBestScore());
    textArea3_1.invalidate();
}

// Khi nút RESTART trên màn hình này được bấm (TouchGFX Designer cấu hình Interaction chuyển màn)
// bạn không cần code nút này, chỉ cần cài đặt Interaction trong Designer:
// Trigger: Button is clicked -> Action: Call new virtual function -> Name: restartGameClicked
void RestartView::restartGameClicked()
{
    presenter->resetGame();
    application().gotoGameScreenScreenWipeTransitionEast();
}

void RestartView::restartBtnClicked()
{
    presenter->requestResetGame();
}
