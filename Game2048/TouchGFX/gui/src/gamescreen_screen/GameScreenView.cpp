#include <gui/gamescreen_screen/GameScreenView.hpp>
#include <touchgfx/Color.hpp>

GameScreenView::GameScreenView()
{

}

void GameScreenView::tearDownScreen()
{
    GameScreenViewBase::tearDownScreen();
}

void GameScreenView::setupScreen()
{
    GameScreenViewBase::setupScreen();
    // Yêu cầu Presenter lấy dữ liệu mới nhất đổ ra khi vừa vào màn hình này
    presenter->requestBoardUpdate();
}

// Hàm này được gọi từ Presenter mỗi khi board thay đổi
void GameScreenView::updateBoardUI()
{
    // Cập nhật Điểm số
    Unicode::snprintf(textArea1Buffer, TEXTAREA1_SIZE, "%d", presenter->getScore());
    textArea1.invalidate();

    // Mảng 1: Trỏ đến các Widget để gọi lệnh vẽ lại màn hình (invalidate)
    touchgfx::TextAreaWithOneWildcard* gridUI[4][4] = {
        {&textArea2, &textArea2_1, &textArea2_2, &textArea2_3},
        {&textArea3, &textArea3_1, &textArea3_2, &textArea3_3},
        {&textArea4, &textArea4_1, &textArea4_2, &textArea4_3},
        {&textArea5, &textArea5_1, &textArea5_2, &textArea5_3}
    };

    // Mảng 2: Trỏ đến các biến Buffer để GHI dữ liệu vào (Fix lỗi invalid conversion)
    touchgfx::Unicode::UnicodeChar* gridBuffer[4][4] = {
        {textArea2Buffer, textArea2_1Buffer, textArea2_2Buffer, textArea2_3Buffer},
        {textArea3Buffer, textArea3_1Buffer, textArea3_2Buffer, textArea3_3Buffer},
        {textArea4Buffer, textArea4_1Buffer, textArea4_2Buffer, textArea4_3Buffer},
        {textArea5Buffer, textArea5_1Buffer, textArea5_2Buffer, textArea5_3Buffer}
    };

    // Mảng 3: THÊM MỚI - Trỏ đến 16 Box nền để đổi màu
    // YÊU CẦU: Bạn hãy sửa tên box1, box2... dưới đây thành ĐÚNG TÊN Box nền của bạn trong TouchGFX
    touchgfx::Box* gridBoxes[4][4] = {
        {&box2_1_2, &box2, &box2_2, &box2_3},       // Hàng 1
        {&box2_1_1, &box2_1, &box2_1_3, &box2_1_4},       // Hàng 2
        {&box2_1_5, &box2_1_6, &box2_1_7, &box2_1_8},    // Hàng 3
        {&box2_1_9, &box2_1_10, &box2_1_11, &box2_1_12}    // Hàng 4
    };

    // Vòng lặp cập nhật 16 ô
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            int val = presenter->getCell(i, j);
            if (val == 0) {
                // Nếu giá trị = 0, ghi chuỗi rỗng vào Buffer
                Unicode::strncpy(gridBuffer[i][j], "", 1);
            } else {
                // Nếu có số, ghi số đó vào Buffer (tối đa 5 ký tự)
                Unicode::snprintf(gridBuffer[i][j], 5, "%d", val);
            }

            // 1. Ra lệnh vẽ lại ô CHỮ trên LCD
            gridUI[i][j]->invalidate();

            // 2. THÊM MỚI - Gọi hàm đổi màu cho BOX NỀN tương ứng
            // Dấu * dùng để giải mã con trỏ (dereference) truyền vào hàm
            setTileColor(*(gridBoxes[i][j]), val);
        }
    }
}

// Hàm này gọi khi thua game
void GameScreenView::showGameOver()
{
    application().gotoRestartScreenCoverTransitionEast();
}

void GameScreenView::setTileColor(touchgfx::Box& boxWidget, int tileValue)
{
    uint32_t color = 0;

    // Xét giá trị của ô số để gán màu tương ứng
    switch (tileValue)
    {
        case 0:    color = touchgfx::Color::getColorFromRGB(205, 193, 180); break; // Ô trống
        case 2:    color = touchgfx::Color::getColorFromRGB(238, 228, 218); break;
        case 4:    color = touchgfx::Color::getColorFromRGB(237, 224, 200); break;
        case 8:    color = touchgfx::Color::getColorFromRGB(242, 177, 121); break;
        case 16:   color = touchgfx::Color::getColorFromRGB(245, 149, 99);  break;
        case 32:   color = touchgfx::Color::getColorFromRGB(246, 124, 95);  break;
        case 64:   color = touchgfx::Color::getColorFromRGB(246, 94, 59);   break;
        case 128:  color = touchgfx::Color::getColorFromRGB(237, 207, 114); break;
        case 256:  color = touchgfx::Color::getColorFromRGB(237, 204, 97);  break;
        case 512:  color = touchgfx::Color::getColorFromRGB(237, 200, 80);  break;
        case 1024: color = touchgfx::Color::getColorFromRGB(237, 197, 63);  break;
        case 2048: color = touchgfx::Color::getColorFromRGB(237, 194, 46);  break;
        default:   color = touchgfx::Color::getColorFromRGB(60, 58, 50);    break; // Các số > 2048
    }

    // Cập nhật màu cho cái Box nền
    boxWidget.setColor(color);

    // Yêu cầu TouchGFX vẽ lại cái Box này lên màn hình LCD
    boxWidget.invalidate();
}

void GameScreenView::undoBtnClicked()
{
    // Ra lệnh cho Presenter lùi bước
    presenter->requestUndo();
}

void GameScreenView::updateUndoCount()
{
    // Cập nhật giá trị hiển thị dưới icon Undo
    // Lưu ý: Nếu bạn đặt tên text khác 'txtUndoCount' thì nhớ sửa lại tên biến Buffer cho khớp
    Unicode::snprintf(txtUndoCountBuffer, TXTUNDOCOUNT_SIZE, "%d", presenter->getUndoCount());
    txtUndoCount.invalidate();
}
