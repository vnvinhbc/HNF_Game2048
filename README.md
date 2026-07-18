# BÁO CÁO PROJECT
## GIỚI THIỆU (Chưa sửa)

__Đề bài__: _Game 2048 trên kit STM32F429 Discovery sử dụng TouchGFX thiết kế giao diện và joystick để điều khiển._

__Sản phẩm:__
1. Game gồm 3 giao diện màn hình: Start, Main Game và Restart
2. Lưới 4x4 với các ô số (2, 4, 8,.., 2048)
3. Xử lý điều khiển ô số bằng joystick
4. Hiển thị điểm số và xử lý điều kiện thắng/thua
- Ảnh chụp minh họa: (Chưa thêm)
<img  />

---
## TÁC GIẢ

- Tên nhóm: Human Not Found
- Thành viên trong nhóm
  |STT|Họ tên|MSSV|Công việc|
  |--:|--|--|--|
  |1|Bùi Cao Vinh|20225430|Null|
  |2|Nguyễn An Khang|20225342|Null|
  |3|Nguyễn Thị Ngọc Anh|20225252|Null|
  |4|Nguyễn Trí Linh|20225355|Null|
  |5|Lê Hồng Sơn|20225389|Null|
---
## MÔI TRƯỜNG HOẠT ĐỘNG (Chưa sửa)

- CPU / Dev Kit: **STM32F429I-DISCOVERY**
- Các module được sử dụng:
  - Bộ joystick analog 2 trục X, Y
  - TouchGFX framework
  - Màn hình LCD tích hợp sẵn trên STM32F429I-DISCO
- Hệ điều hành: FreeRTOS, quản lý đa nhiệm cho TouchGFX và đọc joystick.
---
## SƠ ĐỒ SCHEMATIC (Chưa sửa)

_Cách nối dây, kết nối giữa các linh kiện_ 
|STM32F429|Joystick|
|--|--|
|PC3 (ADC1_IN13)|Trục X|
|PA5 (ADC2_IN5)|Trục Y|
|3V|VCC|
|GND|GND|



### TÍCH HỢP HỆ THỐNG (Chưa Sửa)

- __Phần cứng__:
  - **STM32F429I-DISCO**: đóng vai trò là board xử lý trung tâm, đọc giá trị ADC từ joystick thông qua ngắt polling, xử lý logic game 2048, hiển thị.
  - **Màn hình cảm ứng**: hiển thị toàn bộ giao diện người dùng (bao gồm màn hình game và score).
  - **Joystick**: điều khiển di chuyển các ô số trong game theo 4 hướng lên, xuống, trái, phải.

- __Phần mềm__:
  - **TouchGFX**: thiết kế giao diện Start Screen, Game Screen, Restart Screen, xử lý sự kiện chạm nút Start game và nút Restart.
  - **FreeRTOS**: xử lý đa nhiệm, cho phép chạy song song giữa TouchGFX và luồng đọc Joystick.
  - **STM32Cube HAL**: quản lý phần cứng (ADC cho joystick, LTDC cho màn hình, I2C cho touch controller).
  - **Custom Game Logic**: xử lý thuật toán di chuyển 2048, gộp ô, tính điểm.

### ĐẶC TẢ HÀM (Chưa sửa)

- Một số hàm quan trọng:

  ```C
     /**
      *  Hàm task mặc định để đọc liên tục giá trị joystick trong FreeRTOS
      *  Dữ liệu lưu vào biến toàn cục JoytickX, JoytickY
      *  @param argument Tham số khởi tạo cho task (không sử dụng)
      */
     void StartDefaultTask(void *argument) {
         for(;;)
            {
	              HAL_ADC_Start(&hadc1);
	              HAL_ADC_Start(&hadc2);
	              HAL_ADC_PollForConversion(&hadc1,10);
	              HAL_ADC_PollForConversion(&hadc2,10);
	              JoytickX = HAL_ADC_GetValue(&hadc1);
	              JoytickY = HAL_ADC_GetValue(&hadc2);
            }
     }
  ```
  ```cpp
     /**
      *  Hàm khởi tạo màn hình game 2048
      *  Khởi tạo ma trận 4x4, đặt hai ô ngẫu nhiên với giá trị 2 hoặc 4, và hiển thị
      */
     void Screen2View::setupScreen() {
         Screen2ViewBase::setupScreen();
         point=0;
         for(int i=0;i<4;i++){
            for(int j=0;j<4;j++){
               arr[i][j]=0;
            }
         }
         int h = randInRange(seed, 0, 15);
         arr[h / 4][h % 4] = r[(randInRange(seed + 1, 0, 1))];
         h = randInRange(seed+2, 0, 15);
         arr[h / 4][h % 4] = r[(randInRange(seed + 3, 0, 1))];
         Show();
     }
  ```
  ```cpp
     /**
      *  Hàm xử lý sự kiện tick
      *  Kiểm tra hướng di chuyển từ joystick và gọi hành động tương ứng
      */
     void Screen2View::handleTickEvent() {
         Screen2ViewBase::handleTickEvent();
         if(JoytickX<150&&JoytickY<150&&JoytickX>100&&JoytickY>100) isStop=0;
         if(isStop==1) return;
         if(JoytickX>150&&JoytickY<150&&JoytickY>100){
             Right();
             isStop=1;
         } else if(JoytickX<100&&JoytickY<150&&JoytickY>100){
             Left();
             isStop=1;
         } else if(JoytickY>150&&JoytickX<150&&JoytickX>100){
             Bottom();
             isStop=1;
         } else if(JoytickY<100&&JoytickX<150&&JoytickX>100){
             Top();
             isStop=1;
         }
     }
  ```
  ```cpp
     /**
      *  Hàm thực hiện di chuyển ô sang trái và thêm ô ngẫu nhiên
      */
     void Screen2View::Left() {
         point += leftAction(arr,seed);
         Show();
     }
  ```
  Các hàm di chuyển ô sang phải, lên, xuống tương tự.
  ```cpp
     /**
      *  Hàm hiển thị board và điểm số (được cập nhật liên tục)
      */
     void Screen2View::Show() {
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                memset(buff[i][j], 0, sizeof(buff[i][j]));
                if (arr[i][j] == 0) {
                    Unicode::snprintf(buff[i][j], sizeof(buff[i][j]) / sizeof(Unicode::UnicodeChar), " ");
                } else if (arr[i][j] >= 1024) {
                    Unicode::snprintf(buff[i][j], sizeof(buff[i][j]) / sizeof(Unicode::UnicodeChar), "2e10");
                } else {
                    Unicode::snprintf(buff[i][j], sizeof(buff[i][j]) / sizeof(Unicode::UnicodeChar), "%d", arr[i][j]);
                }
                switch (i * 4 + j) {
                       case 0: tile_0_0.setWildcard(buff[i][j]); tile_0_0.invalidate(); break;
                       case 1: tile_0_1.setWildcard(buff[i][j]); tile_0_1.invalidate(); break;
                       case 2: tile_0_2.setWildcard(buff[i][j]); tile_0_2.invalidate(); break;
                       case 3: tile_0_3.setWildcard(buff[i][j]); tile_0_3.invalidate(); break;
                       case 4: tile_1_0.setWildcard(buff[i][j]); tile_1_0.invalidate(); break;
                       case 5: tile_1_1.setWildcard(buff[i][j]); tile_1_1.invalidate(); break;
                       case 6: tile_1_2.setWildcard(buff[i][j]); tile_1_2.invalidate(); break;
                       case 7: tile_1_3.setWildcard(buff[i][j]); tile_1_3.invalidate(); break;
                       case 8: tile_2_0.setWildcard(buff[i][j]); tile_2_0.invalidate(); break;
                       case 9: tile_2_1.setWildcard(buff[i][j]); tile_2_1.invalidate(); break;
                       case 10: tile_2_2.setWildcard(buff[i][j]); tile_2_2.invalidate(); break;
                       case 11: tile_2_3.setWildcard(buff[i][j]); tile_2_3.invalidate(); break;
                       case 12: tile_3_0.setWildcard(buff[i][j]); tile_3_0.invalidate(); break;
                       case 13: tile_3_1.setWildcard(buff[i][j]); tile_3_1.invalidate(); break;
                       case 14: tile_3_2.setWildcard(buff[i][j]); tile_3_2.invalidate(); break;
                       case 15: tile_3_3.setWildcard(buff[i][j]); tile_3_3.invalidate(); break;
                }
            }
        }
        Unicode::snprintf(score, SIZE, "%d", point);
        textArea1.setWildcard(score);
        textArea1.invalidate();
        if (isFinish(arr)) {
           application().gotoScreen3ScreenNoTransition();
        }
     }
  ```
  ```cpp
     /**
      *  Hàm làm mới game, đặt lại ma trận và điểm số
      */
     void Screen2View::refresh() {
         seed = (unsigned int)time(NULL);
         point=0;
         for(int i=0;i<4;i++){
            for(int j=0;j<4;j++){
               arr[i][j]=0;
            } 
         }
         int h = randInRange(seed, 0, 15);
         arr[h / 4][h % 4] = r[(randInRange(seed + 1, 0, 1))];
         h = randInRange(seed+2, 0, 15);
         arr[h / 4][h % 4] = r[(randInRange(seed + 3, 0, 1))];
         Show();
     }
  ```
  ```cpp
     /**
      *  Hàm trả về màu nền ô số tương ứng với giá trị
      *  @param v Giá trị tile 2, 4, 8, 16,..
      *  @return Màu nền kiểu touchgfx::colortype phù hợp với giá trị tile
      */
      touchgfx::colortype getTileBgColor(int v) {
           switch (v) {
                case 2:    return touchgfx::Color::getColorFromRGB(230, 230, 250);
                case 4:    return touchgfx::Color::getColorFromRGB(173, 216, 230);
                case 8:    return touchgfx::Color::getColorFromRGB(144, 238, 144);
                case 16:   return touchgfx::Color::getColorFromRGB(102, 51, 153);
                case 32:   return touchgfx::Color::getColorFromRGB(34, 139, 34);
                case 64:   return touchgfx::Color::getColorFromRGB(0, 104, 139);
                case 128:  return touchgfx::Color::getColorFromRGB(138, 43, 226);
                case 256:  return touchgfx::Color::getColorFromRGB(0, 191, 255);
                case 512:  return touchgfx::Color::getColorFromRGB(50, 205, 50);
                case 1024: return touchgfx::Color::getColorFromRGB(25, 25, 112);
                case 2048: return touchgfx::Color::getColorFromRGB(75, 0, 130);
                default:   return touchgfx::Color::getColorFromRGB(204, 192, 179);
           }
      }
  ```  
  ```cpp
     /**
      *  Hàm sinh số ngẫu nhiên dựa vào seed
      *  @param seed Giá trị seed cho random
      *  @param min Giá trị nhỏ nhất
      *  @param max Giá trị lớn nhất
      *  @return giá trị ngẫu nhiên trong khoảng
      */
     int randInRange(unsigned int seed, int min, int max) {
         static std::mt19937 rng(std::random_device{}());  // Chỉ khởi tạo 1 lần với seed từ random_device
         std::uniform_int_distribution<int> dist(min, max);     // Sinh số nguyên từ 1 đến 4 (bao gồm)
         return dist(rng);
     }
  ```
  ```cpp
     /**
      *  Hàm đẩy các ô về phía bên trái trong ma trận
      *  @param a[4][4] là ma trận 4x4 cần xử lý
      *  @return 1 nếu có di chuyển, 0 nếu không
      */
     int pushLeft(int a[4][4]) {
         int result = 0;
         int b[4], k;
         for (int i = 0; i < 4; i++) {
             k = 0;
             for (int j = 0; j < 4; j++) {
                 if (a[i][j] != 0) b[k++] = a[i][j];
             }
             for (int j = k; j < 4; j++) b[j] = 0;
             for (int j = 0; j < 4; j++) {
                 if (a[i][j] != b[j]) result = 1;
                 a[i][j] = b[j];
             }
          }
          return result;
     }
  ```
  ```cpp
     /**
      *  Hàm xử lý hành động di chuyển trái, gộp ô giống nhau, thêm ô ngẫu nhiên và cập nhật điểm số
      *  @param a[4][4] là ma trận 4x4 cần xử lý
      *  @param seed Giá trị seed cho sinh số ngẫu nhiên
      *  @return core là điểm số tăng thêm từ việc gộp ô
      */
     int leftAction(int a[4][4], unsigned int seed) {
         int core=0;
         int r1 = pushLeft(a);
         for (int i = 0; i < 4; i++) {
             for (int j = 0; j < 3; j++) {
                 if (a[i][j] == a[i][j + 1]) {
                     core+=2*a[i][j];
                     a[i][j] += a[i][j + 1];
                     a[i][j + 1] = 0;
                 }
             }
         }
         int r2 = pushLeft(a);
         if (r1 || r2) {
            int c[16], k = 0;
            for (int i = 0; i < 4; i++) {
                for (int j = 2; j < 4; j++) {
                    if (a[i][j] == 0) c[k++] = i * 4 + j;
                }
            }
            if (k > 0) {
                int h = c[randInRange(seed, 0, k - 1)];
                a[h / 4][h % 4] = r[(randInRange(seed + 1, 0, 7)>6?1:0)];
            }
         }
         return core;
     }
  ```
  Các hàm đẩy ô phải, lên, xuống và xử lý hành động di chuyển tương tự.
  ```cpp
     /**
      *  Hàm kiểm tra kết thúc game
      *  @param a[4][4] là ma trận 4x4 cần xử lý
      *  @return 1 nếu game kết thúc (hết nước đi), 0 nếu game chưa kết thúc (còn nước đi)
      */
     int isFinish(int a[4][4]) {
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                if (a[i][j] == 0) return 0;
            }
        }
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                if (j < 3 && a[i][j] == a[i][j + 1]) return 0;
                if (i < 3 && a[i][j] == a[i + 1][j]) return 0;
            }
        }
        return 1;
     }
  ```
  ```cpp
     /**
      *  Hàm khởi tạo màn hình restart game nếu thắng/thua hoặc bấm nút endgame
      *  Hiển thị số điểm cuối cùng của người chơi
      */
      void Screen3View::setupScreen()
      {
          Screen3ViewBase::setupScreen();
          Unicode::snprintf(score, SIZE, "%d", point);
          Score.setWildcard(score);
          Score.invalidate();
      }
  ```
### KẾT QUẢ 

- Video demo: (Chưa thêm)
  
### SỰ CỐ (Null)



  
