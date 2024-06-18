# RadarScanAndCam

## A. Giới thiệu

Các tính năng chính của dự án bao gồm:

- Xác định mục tiêu bằng cảm biến siêu âm
- Hiển thị mục tiêu trên màn hình OLED
- Chụp ảnh khi phát hiện mục tiêu ở khoảng cách gần
- Điều khiển servo để quét mục tiêu trong phạm vi 180 độ

## B. Hướng dẫn sử dụng

### Các bước sử dụng:

1. **Cấp nguồn cho ESP32**: Kết nối ESP32 với nguồn điện 5V.
2. **Khởi động hệ thống**: ESP32 sẽ tự khởi động và bắt đầu quét radar.
3. **Quan sát màn hình OLED**: Màn hình sẽ hiển thị vị trí mục tiêu.
4. **Chụp ảnh**: Khi phát hiện mục tiêu ESP32 sẽ tự động chụp ảnh mục tiêu.

### Hình ảnh  và video demo:

![image](https://github.com/loct2205/RadarScanAndCam/assets/75266172/229fb532-73c6-4b50-ae5b-05ecb30c12bb)

Link video: [Video demo](https://drive.google.com/file/d/1oqdByZ9TCyQgGIg9pIIQ2jjrJ_XlKjTF/view?usp=drive_link)

## C. Danh sách linh kiện (Bill of Material)

| Tên linh kiện         | Số lượng | Link của linh kiện                                                                                                                                                     |
|-----------------------|----------|-----------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| ESP-32 ESP-WROOM-32S       | 1        | [Link mua hàng](https://banlinhkien.com/kit-wifi-esp32-espwroom32s-p6649289.html) |
| Ultrasonic Distance Sensor | 1        | [Link mua hàng](https://banlinhkien.com/cam-bien-sieu-am-srf04-p6646905.html)     |
| Servo SG90S                | 2        | [Link mua hàng](https://banlinhkien.com/dong-co-servo-mg90s-p6648776.html)       |
| Màn oled 1.3               | 1        | [Link mua hàng](https://banlinhkien.com/man-hinh-oled-1.3-128x64-i2c-xanh-oled13i2cb-p12496195.html)             |
| ESP32 Cam                  | 1        | [Link mua hàng](https://shopee.vn/B%E1%BB%99-Camera-Kh%C3%B4ng-D%C3%A2y-OV2640-ESP32-DVP-24pin-66-120-160-%C4%90%E1%BB%99-850nm-2MP-2.4Ghz-8MB-PSRAM-Bluetooth-i.934626898.14797112018?sp_atk=f5515306-88ae-4891-9512-86503e8579cc&xptdk=f5515306-88ae-4891-9512-86503e8579cc)           |

## D. Sơ đồ sơ đồ mạch (Tham khảo)

![Sơ đồ mạch](https://github.com/loct2205/RadarScanAndCam/assets/75266172/f608e31b-5aa4-4b00-a624-e16ea2814bfd)

## E. Thiết kế phần mềm

- Servo quét từ 0 đến 180 độ và ngược lại.
- Sử dụng cảm biến siêu âm SR-04 để đo khoảng cách với giá trị trả về được đo bằng công thức lấy thời gian phát hiện ra vật nhân với tốc độ âm thanh trong không khí.
- Hiển thị thông tin: Hiển thị khoảng cách và góc quét trên màn hình OLED.
- Gửi tín hiệu: Khi phát hiện vật thể gửi tín hiệu đến ESP32 CAM để chụp ảnh.
- ESP32 CAM gửi ảnh về laptop qua kết nối bluetooth

## F. Tác giả

- Trần Lộc             
- Trần Văn Thế            
- Nguyễn Thành Trung      

  
