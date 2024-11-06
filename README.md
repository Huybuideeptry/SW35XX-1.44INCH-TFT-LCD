#SW35XX VỚI MÀN HÌNH TFT 1.44 INCH

Đây là một project nhỏ mà mình mày mò được từ thư viện i2c của chip sw3518/3516, thêm vài dòng code để làm màu và mình đã hiện các thông số đọc được lên màn hình được tft mình sử dụng ở đây là 1.44inch dùng driver ST7735 nhưng nó khá đặc biệt so với các màn hình sử dụng cùng loại chip khác:)))


![image](https://github.com/user-attachments/assets/ac129c28-5fcb-4acf-804e-41d8cb1aafe0)



Ở đây mình sử dụng loại mạch này có 2 phiên bản sử dụng chip SW3518 VÀ SW3516, cả 2 đều hoạt động tốt với thư viện sw35xx

![image](https://github.com/user-attachments/assets/bb476986-bc3c-4cfa-a455-6f4fc496225e)


Để sử lí được tín hiệu i2c đọc từ chip tất nhiên ta cần phải có 1 con vi điều khiển để đọc và ghi dữ liệu lên màn hình. 

Hiện tại mình đang làm với cấu hình sử dụng chip Esp32C3-supermini để cho gọn nhất có thể vì mình đang thiết kế cái hộp cho nó mà tất nhiên là vì nó rất mạnh nữa( 160Mhz cho 1 con chip 32bit là quá ổn rồi).


![image](https://github.com/user-attachments/assets/33b791c4-2345-44de-9e52-49529054cae0)


Giờ đến phần quan trọng nhất là cách làm sao để 3 thứ trên hoạt động, dĩ nhiên cấp nguồn cho nó là chắc chắn rồi:))))

Về sơ đồ nối dây thì khá đơn giản nhưng cần có kĩ thuật hàn ổn mới được không cẩn thận là giống như mình chết mất 2 con mạch sạc tiếc luôn:((

Ta sẽ bắt đầu với chip sạc trước. Dưới đây là sơ đồ chân của nó (mình lấy SW3518 làm ví dụ những ae dùng SW3516 thì y chang không khác gì cả)


![image](https://github.com/user-attachments/assets/8c31e9bd-72f7-44e1-a440-b571f26ca51b)

Ta chỉ cần quan tâm 2 chân duy nhất là SCK và SDA, đó là 2 chân giúp ta giao tiếp và lấy dữ liệu để tính toán các thông số như điện áp usb A/C, dòng diện,chuẩn sạc,...

Mà khó 1 cái là loại mạch mà anh Thắng KST và ae hay dùng như trên thì NSX họ không có chân ra của 2 chân đó nên bắt buộc ta phải hàn dây vào và đây cũng là bước khó nhất vì chỉ cần ae không cẩn thận 1 tí hoặc
nhiệt độ quá cao là bay 2 chân này luôn hoặc tệ hơn là dính vào nhau và đương nhiên kết quả là chập chip và chết chip. Trên mạch thì nó nằm ở vị trí này


![z6004631812383_b591473098540c66c1cab017af1d2c35](https://github.com/user-attachments/assets/1dbc6ad6-af74-472b-8873-9484e6d53194)

Khá là khó nhìn phải không cũng may là đế hàn của mình có sẵn cái kính phóng đại nên cũng đỡ hơn chút. 

Như hình thì vị trí của 2 chân đó là ngoài cùng bên phải theo hướng mạch trong hình hoặc ae nhìn vào dấu chấm trên chip là xác định được. Khi hàn xong ae cần phải đo thông mạch 2 dây này nếu nó thông với nhau thì ae cần phải gỡ ra hàn lại ( đây là lý do mình bay mất con mạch đầu tiên:(((


Hàn được 2 chân này thì chúc mừng ae qua được bước khó nhất:)))). 

Hàn xong ta được 2 sợi dây ra với dây chân ngoài cùng sẽ là chân SCK tức là chân SCL ấy và chân tiếp theo là SDA.
Giờ đến phần kết nối giữa màn hình TFT với chip esp32c3 supermini hơi nhiều dây chút vì SPI mà nhưng nhiều dây mới nhanh được;)))



![image](https://github.com/user-attachments/assets/28fd40fc-176a-4b93-825c-7b52a277c0cf)


Giải thích 1 chút về các chân kết nối trên màn hình do 1 chân có thể có nhiều tên gọi khác nhau:

GND ----> GND

VCC ----> +5V

SCL ----> SCK/SCLK

SDA ----> SDA (MOSI)

RES ----> RST(RESET)

DC  ----> DC

CS  ----> CS (CHIP SELECT)

BLK ----> BACKLIGHT( đèn nền, chân này có thể nối thẳng vào dương nguồn 5v hoặc 3.3 đều được( hoặc ae có thể nối vào chân digital bất kì để điều khiển bật tắt cái đèn) vì là LCD mà không có đèn nền đâu có thấy gì đâu:))

Nối đúng các chân vào như hình

![image](https://github.com/user-attachments/assets/5c6efd56-2a07-4802-a532-da5dce172a32)


![image](https://github.com/user-attachments/assets/b33f4eda-5325-47b4-9c2d-d90822889509)

Quay lại với 2 sợi dây vừa hàn lên chip sạc, nếu ae dùng chip giống mình thì nối SCK vào chân 9 và SDA vào chân 8 theo datasheet hình trên. 

Để i2c có thể hoạt động ae cần ít nhất 1 sợi dây nữa nối chân GND nguồn vào của mạch sạc chung với GND nguồn của vi điều khiển thì bọn nó mới hiểu nhau được.

Vậy là cơ bản phần nối dây đã xong việc còn lại là nạp code và chờ kết quả, nếu ae hàn đúng hết nó sẽ hiện như này 

![image](https://github.com/user-attachments/assets/9984c0df-abbe-4414-ad3e-ae778ad515c8)

À quên ae phải cắm nguồn cho cả mạch sạc nó mới hiện thông số được nha còn cấp nguồn cho vi điều khiển không thì chỉ hiện mỗi giao diện thôi:))

Nếu ae muốn làm cấu hình giống mình thì có thể mua đồ vào link này, toàn bộ mình đều mua ở shop này :

https://shopee.vn/M%C3%A0n-H%C3%ACnh-LCD-TFT-0.96-1.3-1.44-1.8-inch-IPS-7P-SPI-HD-65K-ST7735-ST7789-IC-80*160-240*240-i.148048328.16949135690

https://shopee.vn/Esp32-c3-MINI-Ban-Ph%C3%A1t-Tri%E1%BB%83n-ESP32-SuperMini-Ban-Ph%C3%A1t-Tri%E1%BB%83n-ESP32-C3-Ban-Ph%C3%A1t-Tri%E1%BB%83n-WiFi-Bluetooth-Ban-M%E1%BB%9F-R%E1%BB%99ng-i.148048328.27251969369

https://shopee.vn/Sw3518-SW3516-QC4.0-QC3.0-USB-Type-C-%C4%90i%E1%BB%87n-tho%E1%BA%A1i-di-%C4%91%E1%BB%99ng-B%E1%BB%99-chuy%E1%BB%83n-%C4%91%E1%BB%95i-s%E1%BA%A1c-nhanh-6-35V-B%C6%B0%E1%BB%9Bc-xu%E1%BB%91ng-Buck-Boost-M%C3%B4-%C4%91un-i.148048328.25369679116

### HIỆN TẠI CODE MÌNH MỚI CHỈ NẠP ĐƯỢC TRÊN ESP32 VÀ ESP8266, CÁC LOẠI ARDUINO HIỆN CODE VẪN ĐANG LỖI PHẦN I2C NÊN SẼ CẬP NHẬT SAU MONG AE THÔNG CẢM Ạ.















