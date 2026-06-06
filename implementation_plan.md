# Update Smart Garden for ST7789 TFT and Provide Pump Wiring

Màn hình bạn mới mua là loại **TFT màu 1.54 inch (chip ST7789, giao tiếp SPI)** chứ không phải màn hình OLED I2C cũ. Việc này yêu cầu phải thay đổi thư viện lập trình, sửa lại toàn bộ giao diện hiển thị sang màn hình màu (độ phân giải 240x240) và đấu nối dây hoàn toàn mới. 

Ngoài ra, mình cũng sẽ cung cấp chi tiết sơ đồ cắm dây cho máy bơm (thông qua module Relay) đối với bo mạch ESP32 Thường (NodeMCU-32S).

## Open Questions
- Không có câu hỏi nào. Mọi thông tin từ hình ảnh bạn cung cấp đã quá rõ ràng.

## Proposed Changes

### Thay đổi cấu hình thư viện
Xóa thư viện `U8g2` cũ (dành cho OLED trắng đen) và thêm thư viện `Adafruit ST7735 and ST7789 Library` + `Adafruit GFX Library` vào file cấu hình.

#### [MODIFY] [platformio.ini](file:///c:/Users/MR%20ASUS/Downloads/Iot_3/platformio.ini)
- Gỡ bỏ `olikraus/U8g2@^2.35.19`
- Thêm `adafruit/Adafruit ST7735 and ST7789 Library` và `adafruit/Adafruit GFX Library`

### Viết lại giao diện và cấu hình chân cho TFT
File `main.cpp` sẽ được viết lại phần hiển thị (hàm `displayScreen`). Chúng ta sẽ tận dụng màn hình màu để làm cho giao diện đẹp hơn (ví dụ: Cảnh báo sẽ có màu Đỏ, Trạng thái bình thường màu Xanh).

#### [MODIFY] [main.cpp](file:///c:/Users/MR%20ASUS/Downloads/Iot_3/src/main.cpp)
- Khai báo các chân SPI mới cho màn hình TFT:
  - `TFT_CS = 5`
  - `TFT_RST = 4`
  - `TFT_DC = 2`
  *(Chân SCL nối 18, SDA nối 23)*
- Thay thế toàn bộ biến và hàm của `u8g2` bằng `Adafruit_ST7789 tft = Adafruit_ST7789(...)`
- Cập nhật lại UI hiển thị với chữ to hơn (`setTextSize(2)`) và phân loại màu sắc (`ST77XX_GREEN`, `ST77XX_RED`,...).

## Sơ đồ đấu dây thiết bị (ESP32 Thường)

### 1. Cách cắm màn hình TFT ST7789 mới:
- **GND** ➜ cắm vào **GND** của ESP32
- **VCC** ➜ cắm vào **3V3** của ESP32
- **SCL** ➜ cắm vào chân **P18** (chân Clock của SPI)
- **SDA** ➜ cắm vào chân **P23** (chân Data MOSI của SPI)
- **RES** (hoặc RST) ➜ cắm vào chân **P4**
- **DC** ➜ cắm vào chân **P2**
- **CS** ➜ cắm vào chân **P5**
- **BL** ➜ cắm vào **3V3** (để bật đèn nền luôn sáng)

### 2. Cách cắm Máy Bơm qua Module Relay (Relay 5V/3V):
Máy bơm thực tế là một thiết bị tiêu thụ dòng điện lớn, ESP32 không thể cắm trực tiếp được mà phải đi qua một cái **Module Relay (cục rơ-le màu xanh dương có 3 chân cắm nhỏ)**.
- **Trên Module Relay (bên 3 chân kim):**
  - Chân `VCC` (hoặc DC+) ➜ cắm vào chân **5V** (hoặc Vin) của ESP32
  - Chân `GND` (hoặc DC-) ➜ cắm vào **GND** của ESP32
  - Chân `IN` (hoặc Signal) ➜ cắm vào chân **P25** của ESP32
- **Trên Module Relay (bên Domino 3 cổng vặn ốc):**
  - Cắt 1 sợi dây điện của máy bơm làm đôi.
  - Vặn 2 đầu dây vừa cắt vào cổng `COM` (ở giữa) và cổng `NO` (thường mở) của Relay.
  - Sợi dây điện còn lại của máy bơm cắm thẳng vào nguồn. (Bạn có thể xem lại hình minh họa relay trên Google nếu chưa quen cắt dây).

---
**Bạn có đồng ý với kế hoạch thay đổi toàn bộ code màn hình này để mình tiến hành viết lại code luôn không?**
