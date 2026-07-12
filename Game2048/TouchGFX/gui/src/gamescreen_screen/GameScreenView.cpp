#include <gui/gamescreen_screen/GameScreenView.hpp>
#include <touchgfx/Color.hpp>

GameScreenView::GameScreenView()
{
}

void GameScreenView::setupScreen()
{
    GameScreenViewBase::setupScreen();

    tileBoxes[0][0] = &box2_1_2;
    tileBoxes[0][1] = &box2;
    tileBoxes[0][2] = &box2_2;
    tileBoxes[0][3] = &box2_3;

    tileBoxes[1][0] = &box2_1_1;
    tileBoxes[1][1] = &box2_1;
    tileBoxes[1][2] = &box2_1_3;
    tileBoxes[1][3] = &box2_1_4;

    tileBoxes[2][0] = &box2_1_5;
    tileBoxes[2][1] = &box2_1_6;
    tileBoxes[2][2] = &box2_1_7;
    tileBoxes[2][3] = &box2_1_8;

    tileBoxes[3][0] = &box2_1_9;
    tileBoxes[3][1] = &box2_1_10;
    tileBoxes[3][2] = &box2_1_11;
    tileBoxes[3][3] = &box2_1_12;

    tileTexts[0][0] = &textArea2;
    tileTexts[0][1] = &textArea2_1;
    tileTexts[0][2] = &textArea2_2;
    tileTexts[0][3] = &textArea2_3;

    tileTexts[1][0] = &textArea3;
    tileTexts[1][1] = &textArea3_1;
    tileTexts[1][2] = &textArea3_2;
    tileTexts[1][3] = &textArea3_3;

    tileTexts[2][0] = &textArea4;
    tileTexts[2][1] = &textArea4_1;
    tileTexts[2][2] = &textArea4_2;
    tileTexts[2][3] = &textArea4_3;

    tileTexts[3][0] = &textArea5;
    tileTexts[3][1] = &textArea5_1;
    tileTexts[3][2] = &textArea5_2;
    tileTexts[3][3] = &textArea5_3;
}

void GameScreenView::tearDownScreen()
{
    GameScreenViewBase::tearDownScreen();
}

colortype GameScreenView::getColorForValue(int value)
{
    switch (value)
    {
        case 0:    return touchgfx::Color::getColorFromRGB(205, 193, 180); // Màu nền trống
        case 2:    return touchgfx::Color::getColorFromRGB(238, 228, 218);
        case 4:    return touchgfx::Color::getColorFromRGB(237, 224, 200);
        case 8:    return touchgfx::Color::getColorFromRGB(242, 177, 121);
        case 16:   return touchgfx::Color::getColorFromRGB(245, 149, 99);
        case 32:   return touchgfx::Color::getColorFromRGB(246, 124, 95);
        case 64:   return touchgfx::Color::getColorFromRGB(246, 94, 59);
        case 128:  return touchgfx::Color::getColorFromRGB(237, 207, 114);
        case 256:  return touchgfx::Color::getColorFromRGB(237, 204, 97);
        case 512:  return touchgfx::Color::getColorFromRGB(237, 200, 80);
        case 1024: return touchgfx::Color::getColorFromRGB(237, 197, 63);
        case 2048: return touchgfx::Color::getColorFromRGB(237, 194, 46);
        default:   return touchgfx::Color::getColorFromRGB(60, 58, 50); // Màu đen cho số khủng
    }
}

void GameScreenView::updateBoard()
{
    for (int r = 0; r < 4; r++)
    {
        for (int c = 0; c < 4; c++)
        {
            // 1. Lấy dữ liệu toán học
            int val = presenter->getTileValue(r, c);

            // 2. CẬP NHẬT MÀU SẮC (Cho Box)
            colortype newColor = getColorForValue(val);
            tileBoxes[r][c]->setColor(newColor);
            tileBoxes[r][c]->invalidate();

            // 3. CẬP NHẬT CON SỐ (Cho Text Area)
            if (val == 0)
            {
                // Nếu ô trống (0): Nạp chuỗi rỗng vào Buffer
                touchgfx::Unicode::strncpy(tileBuffers[r][c], "", 1);
            }
            else
            {
                // Nếu có số: Ép kiểu int thành chuỗi Unicode và nạp vào Buffer
                touchgfx::Unicode::snprintf(tileBuffers[r][c], 6, "%d", val);
            }

            // Ép Text Area sử dụng Buffer mới của chúng ta
            tileTexts[r][c]->setWildcard(tileBuffers[r][c]);

            // CHỐT AN TOÀN: Báo cho Text Area quét lại đồ họa
            tileTexts[r][c]->invalidate();
        }
    }
}

// Hàm này tự động bắt phím khi bạn gõ trên bàn phím PC
void GameScreenView::handleKeyEvent(uint8_t key)
{
    // Bắn tín hiệu ép Presenter truyền lệnh xuống Model
    // 'w'=119 (Lên), 's'=115 (Xuống), 'a'=97 (Trái), 'd'=100 (Phải)
    if (key == 'w') presenter->simulateJoystick(1);
    else if (key == 's') presenter->simulateJoystick(2);
    else if (key == 'a') presenter->simulateJoystick(3);
    else if (key == 'd') presenter->simulateJoystick(4);
}
