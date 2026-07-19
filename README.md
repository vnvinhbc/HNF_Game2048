# BÁO CÁO PROJECT

## GAME 2048 TRÊN KIT STM32F429I-DISCOVERY

---

## GIỚI THIỆU

**Đề bài:** *Xây dựng trò chơi 2048 trên kit STM32F429I-DISCOVERY, sử dụng TouchGFX để thiết kế giao diện và joystick analog để thực hiện thao tác dồn số.*

**Mục tiêu của dự án:**

- Vận dụng kiến thức lập trình vi điều khiển STM32F4.
- Đọc tín hiệu analog từ joystick bằng ADC và DMA.
- Xây dựng giao diện đồ họa nhúng bằng TouchGFX.
- Tổ chức chương trình theo mô hình Model–View–Presenter.
- Cài đặt thuật toán dồn, gộp ô và tính điểm của trò chơi 2048.
- Kết hợp FreeRTOS, TouchGFX và các ngoại vi trên kit thành một hệ thống hoàn chỉnh.

**Sản phẩm hoàn thành:**

1. Trò chơi 2048 với bàn cờ 4×4, gồm các ô số 2, 4, 8, 16, ..., 2048 và các giá trị lớn hơn.
2. Điều khiển dồn số theo bốn hướng lên, xuống, trái, phải bằng joystick analog.
3. Hai ô số được sinh khi bắt đầu ván; sau mỗi nước đi hợp lệ, hệ thống sinh thêm một ô mới.
4. Hai ô có cùng giá trị được gộp theo đúng quy tắc 2048 và cập nhật điểm số.
5. Tự động phát hiện Game Over khi bàn cờ đầy và không còn nước đi hợp lệ.
6. Hiển thị màu nền khác nhau theo giá trị của từng ô số.
7. Hỗ trợ Undo một bước, tối đa 3 lượt trong mỗi ván.
8. Hiển thị điểm hiện tại, Best Score và bảng xếp hạng Top 5.
9. Gồm 6 giao diện màn hình:
   - Start Game.
   - Game Screen.
   - Confirm.
   - Restart.
   - How To Play.
   - Ranking Screen.
10. Chạy trên màn hình cảm ứng tích hợp của STM32F429I-DISCOVERY.

**Repository:** <https://github.com/vnvinhbc/HNF_Game2048>

**Ảnh chụp minh họa:**

<p align="center">
  <img src="start_screen.jpeg" width="24%" alt="Start Screen" />
  <img src="how_to_play.jpeg" width="24%" alt="How to Play" />
  <img src="game_screen.png" width="24%" alt="Game Screen" />
  <img src="restart_screen.jpeg" width="24%" alt="Restart Screen" />
</p>
---

## TÁC GIẢ

- **Tên nhóm:** Human Not Found
- **Thành viên trong nhóm:**

   | STT | Họ tên | MSSV |
   |---:|---|---|
   | 1 | Bùi Cao Vinh | 20225430 |
   | 2 | Nguyễn An Khang | 20225342 |
   | 3 | Nguyễn Thị Ngọc Anh | 20225252 |
   | 4 | Nguyễn Trí Linh | 20225355 |
   | 5 | Lê Hồng Sơn | 20225389 |

---

## MÔI TRƯỜNG HOẠT ĐỘNG

### Phần cứng

- **CPU / Dev Kit:** STM32F429I-DISCOVERY.
- **Vi điều khiển:** STM32F429ZIT6, lõi ARM Cortex-M4F.
- **Tần số hệ thống:** 180 MHz.
- **Màn hình:** LCD TFT 2,4 inch tích hợp trên kit.
- **Độ phân giải giao diện:** 240×320 pixel, định dạng màu RGB565.
- **Cảm ứng:** STMPE811, giao tiếp qua I2C3.
- **Điều khiển LCD:** ILI9341; hệ thống sử dụng LTDC, DMA2D và SDRAM để hiển thị TouchGFX.
- **Thiết bị điều khiển:** Joystick analog hai trục X, Y.

### Phần mềm và thư viện

- **STM32CubeIDE**: biên dịch, nạp chương trình và debug.
- **STM32CubeMX**: cấu hình MCU thông qua file `STM32F429I_DISCO_REV_D01.ioc`.
- **STM32Cube FW_F4 V1.28.3**.
- **TouchGFX 4.26.1**: thiết kế và triển khai giao diện.
- **FreeRTOS + CMSIS-RTOS V2**: quản lý các task của hệ thống.
- **STM32 HAL Driver**: điều khiển ADC, DMA, LTDC, DMA2D, I2C, SPI và SDRAM.
- **Ngôn ngữ C**: phần khởi tạo phần cứng và FreeRTOS.
- **Ngôn ngữ C++**: thuật toán Game 2048 và các lớp TouchGFX.

---

## SƠ ĐỒ SCHEMATIC

### Kết nối joystick với STM32F429I-DISCOVERY

| STM32F429I-DISCOVERY | Joystick | Chức năng |
|---|---|---|
| PA0 / ADC1_IN0 | Trục X | Đọc chuyển động trái/phải |
| PA5 / ADC1_IN5 | Trục Y | Đọc chuyển động lên/xuống |
| 3.3 V | VCC | Cấp nguồn cho joystick |
| GND | GND | Mass chung |

### Thông số ADC và DMA

- ADC sử dụng: **ADC1**.
- Độ phân giải: **12 bit**, giá trị từ 0 đến 4095.
- Scan Conversion Mode: **Enable**.
- Continuous Conversion Mode: **Enable**.
- Số phép chuyển đổi: **2**.
- Rank 1: `ADC_CHANNEL_0` – PA0 – trục X.
- Rank 2: `ADC_CHANNEL_5` – PA5 – trục Y.
- Sampling Time: `ADC_SAMPLETIME_480CYCLES`.
- DMA: truyền từ ngoại vi vào bộ nhớ.
- Kiểu dữ liệu DMA: half-word 16 bit.
- Chế độ DMA: **Circular**.

Dữ liệu joystick được DMA cập nhật liên tục vào buffer:

```c
volatile uint16_t joystick_data[2] = {2048, 2048};
```

Trong đó:

```text
joystick_data[0] = giá trị trục X từ PA0
joystick_data[1] = giá trị trục Y từ PA5
```

### Sơ đồ khối kết nối

```text
       Joystick analog
       ┌───────────────┐
       │ X          Y  │
       └─┬──────────┬──┘
         │          │
         ▼          ▼
 PA0 / ADC1_IN0   PA5 / ADC1_IN5
         │          │
         └────┬─────┘
              ▼
       ADC1 Scan 2 kênh
              │
              ▼
        DMA Circular
              │
              ▼
     joystick_data[2]
              │
              ▼
      TouchGFX Model::tick()
              │
              ▼
       Thuật toán Game 2048
              │
              ▼
     Giao diện LCD TouchGFX
```

---

## TÍCH HỢP HỆ THỐNG

### Phần cứng

- **STM32F429I-DISCOVERY** đóng vai trò bộ xử lý trung tâm, thực hiện:
  - Đọc hai trục joystick qua ADC1.
  - Dùng DMA để truyền dữ liệu ADC trực tiếp vào RAM.
  - Xử lý logic Game 2048.
  - Điều khiển màn hình LCD và cảm ứng.
  - Chạy FreeRTOS và TouchGFX.
- **Joystick analog** cung cấp hai điện áp tương ứng vị trí trục X và Y.
- **Màn hình cảm ứng** hiển thị toàn bộ giao diện, các ô số, điểm và các nút chức năng.
- **SDRAM ngoài** được sử dụng làm vùng nhớ phục vụ framebuffer TouchGFX.

### Phần mềm

- **TouchGFX** xây dựng 6 màn hình, quản lý widget và chuyển màn hình.
- **FreeRTOS** chạy task giao diện `GUI_Task` và `defaultTask`.
- **STM32 HAL** cấu hình và vận hành ADC, DMA, LTDC, DMA2D, I2C3, SPI5 và FMC/SDRAM.
- **Game2048** là lớp xử lý thuật toán độc lập với giao diện.
- **Model–View–Presenter** được sử dụng để tách logic, dữ liệu và hiển thị.

### Kiến trúc phần mềm

```text
Core/Src/main.c
    │
    ├── Khởi tạo ADC1 + DMA Circular
    ├── Khởi tạo LTDC, DMA2D, SDRAM, TouchGFX
    └── Khởi động FreeRTOS
             │
             ▼
TouchGFX Model::tick()
    │
    ├── Đọc joystick_data[0], joystick_data[1]
    ├── Xác định hướng di chuyển
    ├── Lưu trạng thái Undo
    └── Gọi lớp Game2048
             │
             ▼
Game2048
    │
    ├── Dồn và gộp ô
    ├── Tính điểm
    ├── Sinh ô mới
    └── Kiểm tra Game Over
             │
             ▼
Presenter
    │
    └── Thông báo dữ liệu mới cho View
             │
             ▼
View
    │
    ├── Cập nhật 16 ô số
    ├── Cập nhật màu nền
    ├── Cập nhật điểm và Undo
    └── Chuyển màn hình
```

### Các màn hình của hệ thống

| Màn hình | Chức năng |
|---|---|
| `StartGame` | Màn hình chính; cho phép bắt đầu chơi, xem hướng dẫn và bảng xếp hạng |
| `GameScreen` | Hiển thị bàn cờ 4×4, điểm số, số lượt Undo và nút kết thúc ván |
| `Confirm` | Xác nhận quay lại trò chơi hoặc kết thúc ván |
| `Restart` | Hiển thị điểm cuối, Best Score; cho phép chơi lại hoặc về màn hình chính |
| `HowToPlay` | Hiển thị luật chơi và hướng dẫn sử dụng joystick |
| `RankingScreen` | Hiển thị 5 điểm cao nhất trong phiên chạy hiện tại |

---

## NGUYÊN LÝ HOẠT ĐỘNG

1. Khi cấp nguồn, chương trình khởi tạo các ngoại vi và TouchGFX.
2. ADC1 liên tục quét PA0 và PA5.
3. DMA ghi hai giá trị mới nhất vào `joystick_data[2]` mà không yêu cầu CPU đọc từng mẫu.
4. TouchGFX gọi `Model::tick()` định kỳ.
5. `Model::tick()` so sánh giá trị joystick với các ngưỡng:

| Điều kiện | Hành động |
|---|---|
| `X < 500` | Dồn trái |
| `X > 3500` | Dồn phải |
| `Y < 500` | Dồn lên |
| `Y > 3500` | Dồn xuống |
| X và Y trong khoảng 500–3500 | Joystick trở về vùng trung tâm |

6. Biến `joystickReady` bảo đảm một lần giữ joystick chỉ tạo một nước đi.
7. Trước một nước đi hợp lệ, Model lưu lại bàn cờ và điểm để phục vụ Undo.
8. Lớp `Game2048` dồn, gộp ô, tính điểm và sinh ô mới.
9. Presenter thông báo cho View cập nhật giao diện.
10. Khi không còn nước đi, chương trình chuyển sang màn hình Restart.

---

## ĐẶC TẢ HÀM

### 1. Khởi động ADC1 với DMA Circular

File: `Game2048/Core/Src/main.c`

```c
volatile uint16_t joystick_data[2] = {2048, 2048};

/* Sau khi khởi tạo ADC1 và DMA */
hdma_adc1.Init.Mode = DMA_CIRCULAR;
HAL_DMA_Init(&hdma_adc1);

HAL_ADC_Start_DMA(
    &hadc1,
    (uint32_t*)joystick_data,
    2
);

/* Chương trình chỉ đọc giá trị mới nhất nên không dùng callback DMA */
__HAL_DMA_DISABLE_IT(
    hadc1.DMA_Handle,
    DMA_IT_TC | DMA_IT_HT | DMA_IT_TE
);
```

**Chức năng:**

- Khởi động ADC1 quét liên tục hai kênh.
- DMA tự động ghi dữ liệu vào `joystick_data`.
- Chế độ circular cho phép quá trình cập nhật lặp vô hạn.
- CPU không phải gọi `HAL_ADC_PollForConversion()` như code mẫu cũ.

---

### 2. Task mặc định của FreeRTOS

File: `Game2048/Core/Src/main.c`

```c
void StartDefaultTask(void *argument)
{
    for (;;)
    {
        __HAL_ADC_CLEAR_FLAG(&hadc1, ADC_FLAG_OVR);
        osDelay(100);
    }
}
```

**Chức năng:**

- Định kỳ xóa cờ ADC Overrun.
- Dùng `osDelay(100)` để task chuyển sang trạng thái chờ, tránh chiếm CPU.
- Việc đọc joystick không được thực hiện bằng polling trong task; dữ liệu do DMA cập nhật.

---

### 3. Khởi tạo một ván chơi mới

File: `Game2048/TouchGFX/gui/src/model/Game2048.cpp`

```cpp
Game2048::Game2048()
{
    reset();
}

void Game2048::reset()
{
    score = 0;

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            board[i][j] = 0;
        }
    }

    addRandomTile();
    addRandomTile();
}
```

**Chức năng:**

- Đưa điểm số về 0.
- Xóa toàn bộ ma trận 4×4.
- Sinh hai ô ban đầu.

---

### 4. Sinh ô số ngẫu nhiên

```cpp
void Game2048::addRandomTile()
{
    int emptyCells[16][2];
    int count = 0;

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (board[i][j] == 0)
            {
                emptyCells[count][0] = i;
                emptyCells[count][1] = j;
                count++;
            }
        }
    }

    if (count > 0)
    {
        int index = rand() % count;
        board[emptyCells[index][0]][emptyCells[index][1]] =
            (rand() % 10 < 9) ? 2 : 4;
    }
}
```

**Chức năng:**

- Tạo danh sách tất cả các ô đang trống.
- Chọn ngẫu nhiên một vị trí trống.
- Sinh số 2 với xác suất 90% và số 4 với xác suất 10%.

---

### 5. Xử lý joystick và điều khiển nước đi

File: `Game2048/TouchGFX/gui/src/model/Model.cpp`

```cpp
void Model::tick()
{
    uint16_t x_val = joystick_data[0];
    uint16_t y_val = joystick_data[1];

    if (x_val < 500 || x_val > 3500 ||
        y_val < 500 || y_val > 3500)
    {
        if (joystickReady)
        {
            int tempBoard[4][4];
            int tempScore = game.getScore();

            for (int i = 0; i < 4; i++)
            {
                for (int j = 0; j < 4; j++)
                {
                    tempBoard[i][j] = game.getCell(i, j);
                }
            }

            bool moved = false;

            if (x_val < 500)
                moved = game.moveLeft();
            else if (x_val > 3500)
                moved = game.moveRight();
            else if (y_val < 500)
                moved = game.moveUp();
            else if (y_val > 3500)
                moved = game.moveDown();

            if (moved)
            {
                if (undoCount > 0)
                {
                    for (int i = 0; i < 4; i++)
                    {
                        for (int j = 0; j < 4; j++)
                        {
                            previousBoard[i][j] = tempBoard[i][j];
                        }
                    }

                    previousScore = tempScore;
                    canUndo = true;
                }

                if (game.getScore() > bestScore)
                {
                    bestScore = game.getScore();
                }

                if (modelListener != 0)
                {
                    modelListener->boardUpdated();
                }

                if (game.isGameOver() && modelListener != 0)
                {
                    modelListener->gameHasEnded();
                }
            }

            joystickReady = false;
        }
    }
    else
    {
        joystickReady = true;
    }
}
```

**Chức năng:**

- Đọc trực tiếp hai giá trị được DMA cập nhật.
- Xác định hướng di chuyển từ ngưỡng ADC.
- Lưu trạng thái cũ trước khi di chuyển.
- Chỉ cập nhật Undo khi bàn cờ thực sự thay đổi.
- Cập nhật Best Score.
- Thông báo cho View vẽ lại bàn cờ.
- Kiểm tra điều kiện Game Over.

---

### 6. Thuật toán dồn và gộp ô sang trái

File: `Game2048/TouchGFX/gui/src/model/Game2048.cpp`

```cpp
bool Game2048::slideLeft()
{
    bool moved = false;

    for (int i = 0; i < 4; i++)
    {
        int writePos = 0;
        int lastMergedPos = -1;

        for (int j = 0; j < 4; j++)
        {
            if (board[i][j] != 0)
            {
                if (writePos > 0 &&
                    board[i][writePos - 1] == board[i][j] &&
                    lastMergedPos != writePos - 1)
                {
                    board[i][writePos - 1] *= 2;
                    score += board[i][writePos - 1];
                    board[i][j] = 0;
                    lastMergedPos = writePos - 1;
                    moved = true;
                }
                else
                {
                    if (writePos != j)
                    {
                        board[i][writePos] = board[i][j];
                        board[i][j] = 0;
                        moved = true;
                    }
                    writePos++;
                }
            }
        }
    }

    return moved;
}
```

**Nguyên tắc:**

- Bỏ qua các ô có giá trị 0.
- Dồn các ô khác 0 về bên trái.
- Hai ô bằng nhau được gộp thành một ô có giá trị gấp đôi.
- Điểm tăng bằng giá trị của ô mới.
- `lastMergedPos` bảo đảm mỗi ô chỉ được gộp một lần trong một nước đi.

Ví dụ:

```text
[2, 2, 2, 2] → [4, 4, 0, 0]
[2, 2, 4, 0] → [4, 4, 0, 0]
[4, 0, 4, 4] → [8, 4, 0, 0]
```

---

### 7. Xử lý bốn hướng di chuyển

```cpp
bool Game2048::moveLeft()
{
    bool moved = slideLeft();
    if (moved) addRandomTile();
    return moved;
}

bool Game2048::moveRight()
{
    rotateRight();
    rotateRight();
    bool moved = slideLeft();
    rotateRight();
    rotateRight();
    if (moved) addRandomTile();
    return moved;
}

bool Game2048::moveUp()
{
    rotateRight();
    rotateRight();
    rotateRight();
    bool moved = slideLeft();
    rotateRight();
    if (moved) addRandomTile();
    return moved;
}

bool Game2048::moveDown()
{
    rotateRight();
    bool moved = slideLeft();
    rotateRight();
    rotateRight();
    rotateRight();
    if (moved) addRandomTile();
    return moved;
}
```

**Chức năng:**

- Thuật toán chỉ viết trực tiếp cho hướng trái.
- Các hướng còn lại được chuyển đổi bằng cách xoay ma trận, gọi `slideLeft()` rồi xoay trở lại.
- Chỉ sinh ô mới nếu nước đi làm bàn cờ thay đổi.

---

### 8. Kiểm tra điều kiện kết thúc game

```cpp
bool Game2048::isGameOver()
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (board[i][j] == 0)
                return false;
        }
    }

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (j < 3 && board[i][j] == board[i][j + 1])
                return false;

            if (i < 3 && board[i][j] == board[i + 1][j])
                return false;
        }
    }

    return true;
}
```

Game kết thúc khi đồng thời thỏa mãn:

- Không còn ô trống.
- Không còn hai ô kề ngang bằng nhau.
- Không còn hai ô kề dọc bằng nhau.

---

### 9. Cập nhật bàn cờ và điểm số trên TouchGFX

File: `Game2048/TouchGFX/gui/src/gamescreen_screen/GameScreenView.cpp`

```cpp
void GameScreenView::updateBoardUI()
{
    Unicode::snprintf(
        textArea1Buffer,
        TEXTAREA1_SIZE,
        "%d",
        presenter->getScore()
    );
    textArea1.invalidate();

    touchgfx::TextAreaWithOneWildcard* gridUI[4][4] = {
        {&textArea2, &textArea2_1, &textArea2_2, &textArea2_3},
        {&textArea3, &textArea3_1, &textArea3_2, &textArea3_3},
        {&textArea4, &textArea4_1, &textArea4_2, &textArea4_3},
        {&textArea5, &textArea5_1, &textArea5_2, &textArea5_3}
    };

    touchgfx::Unicode::UnicodeChar* gridBuffer[4][4] = {
        {textArea2Buffer, textArea2_1Buffer,
         textArea2_2Buffer, textArea2_3Buffer},
        {textArea3Buffer, textArea3_1Buffer,
         textArea3_2Buffer, textArea3_3Buffer},
        {textArea4Buffer, textArea4_1Buffer,
         textArea4_2Buffer, textArea4_3Buffer},
        {textArea5Buffer, textArea5_1Buffer,
         textArea5_2Buffer, textArea5_3Buffer}
    };

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            int val = presenter->getCell(i, j);

            if (val == 0)
                Unicode::strncpy(gridBuffer[i][j], "", 1);
            else
                Unicode::snprintf(gridBuffer[i][j], 5, "%d", val);

            gridUI[i][j]->invalidate();
        }
    }
}
```

**Chức năng:**

- Lấy điểm và dữ liệu 16 ô qua Presenter.
- Chuyển giá trị số thành chuỗi Unicode.
- Ô trống được hiển thị bằng chuỗi rỗng.
- Gọi `invalidate()` để TouchGFX vẽ lại widget.
- Trong code đầy đủ, mỗi ô còn được gọi `setTileColor()` để đổi màu nền.

---

### 10. Đổi màu ô số

```cpp
void GameScreenView::setTileColor(
    touchgfx::Box& boxWidget,
    int tileValue
)
{
    uint32_t color = 0;

    switch (tileValue)
    {
        case 0:    color = touchgfx::Color::getColorFromRGB(205, 193, 180); break;
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
        default:   color = touchgfx::Color::getColorFromRGB(60, 58, 50);    break;
    }

    boxWidget.setColor(color);
    boxWidget.invalidate();
}
```

---

### 11. Chức năng Undo

```cpp
bool Model::undo()
{
    if (canUndo && undoCount > 0)
    {
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                game.setCell(i, j, previousBoard[i][j]);
            }
        }

        game.setScore(previousScore);
        canUndo = false;
        undoCount--;
        return true;
    }

    return false;
}
```

**Quy tắc Undo hiện tại:**

- Mỗi ván có tối đa 3 lượt Undo.
- Chỉ lưu một trạng thái gần nhất.
- Chỉ cho Undo sau một nước đi hợp lệ.
- Không thể nhấn Undo hai lần liên tiếp mà chưa thực hiện nước đi mới.

---

### 12. Lưu bảng xếp hạng Top 5

```cpp
void Model::saveScoreToRanking(int currentScore)
{
    for (int i = 0; i < 5; i++)
    {
        if (currentScore > topScores[i])
        {
            for (int j = 4; j > i; j--)
            {
                topScores[j] = topScores[j - 1];
            }

            topScores[i] = currentScore;
            break;
        }
    }
}
```

**Chức năng:**

- Tìm vị trí phù hợp của điểm mới.
- Dịch các điểm thấp hơn xuống một bậc.
- Chèn điểm mới để giữ danh sách giảm dần.
- Bảng xếp hạng hiện được lưu trong RAM nên bị xóa khi mất nguồn hoặc reset kit.

---

### 13. Hiển thị màn hình kết thúc

File: `Game2048/TouchGFX/gui/src/restart_screen/RestartView.cpp`

```cpp
void RestartView::setupScreen()
{
    RestartViewBase::setupScreen();

    Unicode::snprintf(
        textArea3Buffer,
        TEXTAREA3_SIZE,
        "%d",
        presenter->getScore()
    );
    textArea3.invalidate();

    Unicode::snprintf(
        textArea3_1Buffer,
        TEXTAREA3_1_SIZE,
        "%d",
        presenter->getBestScore()
    );
    textArea3_1.invalidate();
}
```

Màn hình Restart hiển thị:

- Điểm của ván vừa chơi.
- Best Score trong phiên hiện tại.
- Nút chơi lại.
- Nút trở về màn hình chính.

---

## CẤU TRÚC THƯ MỤC QUAN TRỌNG

```text
HNF_Game2048/
├── README.md
└── Game2048/
    ├── Core/
    │   ├── Inc/
    │   └── Src/
    │       └── main.c
    ├── Drivers/
    ├── Middlewares/
    │   └── Third_Party/FreeRTOS/
    ├── TouchGFX/
    │   ├── Game2048.touchgfx
    │   ├── assets/
    │   └── gui/
    │       ├── include/gui/
    │       │   ├── model/
    │       │   │   ├── Game2048.hpp
    │       │   │   └── Model.hpp
    │       │   └── ...
    │       └── src/
    │           ├── model/
    │           │   ├── Game2048.cpp
    │           │   └── Model.cpp
    │           ├── gamescreen_screen/
    │           ├── startgame_screen/
    │           ├── restart_screen/
    │           ├── confirm_screen/
    │           ├── howtoplay_screen/
    │           └── rankingscreen_screen/
    ├── STM32CubeIDE/
    └── STM32F429I_DISCO_REV_D01.ioc
```

---

## HƯỚNG DẪN BIÊN DỊCH VÀ CHẠY

1. Clone repository:

```bash
git clone https://github.com/vnvinhbc/HNF_Game2048.git
```

2. Mở STM32CubeIDE.
3. Chọn **File → Import → Existing Projects into Workspace**.
4. Trỏ đến thư mục:

```text
HNF_Game2048/Game2048/STM32CubeIDE
```

5. Kiểm tra kết nối joystick theo bảng schematic.
6. Kết nối kit STM32F429I-DISCOVERY với máy tính qua cổng ST-LINK USB.
7. Build project.
8. Chọn Run hoặc Debug để nạp chương trình.
9. Dùng màn hình cảm ứng để bắt đầu ván và joystick để dồn số.

Khi cần sửa giao diện:

1. Mở `Game2048/TouchGFX/Game2048.touchgfx` bằng TouchGFX Designer.
2. Chỉnh sửa giao diện.
3. Generate Code.
4. Quay lại STM32CubeIDE để build lại project.

---

## KẾT QUẢ

Theo kết quả chạy thực tế của nhóm, hệ thống đã:

- Khởi động ổn định trên STM32F429I-DISCOVERY.
- Đọc được hai trục joystick bằng ADC1 và DMA.
- Nhận đúng bốn hướng di chuyển.
- Dồn và gộp ô đúng quy tắc của Game 2048.
- Không sinh ô mới khi người chơi thực hiện một hướng không làm thay đổi bàn cờ.
- Cập nhật điểm số và màu ô trên LCD.
- Phát hiện Game Over và chuyển màn hình kết thúc.
- Thực hiện được Undo, Best Score và Top 5.
- Chuyển đổi ổn định giữa 6 màn hình TouchGFX.

**Video demo:** <https://drive.google.com/file/d/1v3uHPwpLZKU4o1PDe8_FHmnRv-jm5PPc/view?usp=sharing>

---

## HẠN CHẾ HIỆN TẠI

1. Chưa có màn hình hoặc trạng thái chiến thắng riêng khi tạo được ô 2048.
2. Best Score và Top 5 chỉ được lưu trong RAM, mất khi reset hoặc mất nguồn.
3. Hàm `rand()` chưa được khởi tạo seed bằng nguồn biến đổi nên chuỗi vị trí có thể lặp sau mỗi lần khởi động.
4. Ngưỡng joystick 500 và 3500 được đặt cố định, chưa có chức năng tự hiệu chuẩn.
5. Undo chỉ lưu một trạng thái gần nhất, không phải lịch sử nhiều bước.
6. Hàm `saveState()` được khai báo trong `Model.hpp` nhưng chưa được triển khai và sử dụng.
7. Biến `isScoreSaved` đã được khai báo nhưng cơ chế lưu bảng xếp hạng hiện chủ yếu thực hiện trong `resetGame()`.
8. Chưa lưu dữ liệu người chơi vào Flash/EEPROM ngoài.

---

## HƯỚNG PHÁT TRIỂN

- Thêm màn hình Win khi đạt ô 2048 và cho phép lựa chọn tiếp tục chơi.
- Lưu Best Score và Top 5 vào Flash hoặc EEPROM để không mất khi tắt nguồn.
- Dùng timer, RTC hoặc nhiễu ADC để tạo seed ngẫu nhiên tốt hơn.
- Xây dựng chức năng hiệu chuẩn joystick tự động.
- Lưu nhiều trạng thái để Undo nhiều bước liên tiếp.
- Thêm animation khi ô di chuyển, sinh mới và gộp ô.
- Bổ sung âm thanh khi di chuyển, gộp số và Game Over.
- Thêm lựa chọn điều khiển bằng gyroscope tích hợp trên kit.
- Tối ưu tên widget và xóa các comment tạm trong mã nguồn trước khi phát hành.

---

## KẾT LUẬN

Dự án đã xây dựng thành công trò chơi 2048 trên nền tảng STM32F429I-DISCOVERY. Hệ thống kết hợp được nhiều nội dung quan trọng của môn Hệ nhúng, bao gồm ADC, DMA, FreeRTOS, giao diện TouchGFX, lập trình C/C++ và tổ chức phần mềm theo mô hình MVP. Joystick được đọc liên tục bằng ADC1 kết hợp DMA circular, giúp giảm tải CPU và cung cấp thao tác điều khiển ổn định. Thuật toán game được tách thành lớp riêng, thuận lợi cho kiểm thử, bảo trì và mở rộng trong tương lai.
