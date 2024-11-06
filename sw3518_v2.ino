#include <Arduino.h>
#include <h1_SW35xx.h>
using namespace h1_SW35xx;
#include  <Adafruit_ST7735.h>
#include  <Adafruit_GFX.h>
#include  <SPI.h>
/////////////////////// chân kết nối spi với màn hình( cần thay đổi tùy theo từng chip)
#define TFT_CS     7
#define TFT_RST    3                     
#define TFT_DC     2
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS,  TFT_DC, TFT_RST);
#define TFT_SCLK 4   
#define TFT_MOSI 6  
#include <Fonts/Org_01.h>


float vin_V  = 0.0;
float vout_V = 0.0;
float usba   = 0.0;
float usbc   = 0.0;
float power_c = 0.0;
float power_a = 0.0;
SW35xx sw(Wire);

char vout_str[10];  // Chuỗi chứa giá trị hiển thị
char vin_str[10];
char usba_str[10];
char usbc_str[10];
char power_c_str[10];
char power_a_str[10];

const char *fastChargeType2String(SW35xx::fastChargeType_t fastChargeType) {
  switch (fastChargeType) {
  case SW35xx::NOT_FAST_CHARGE:
    return "none";
    break;
  case SW35xx::QC2:
    return "QC2.0";
    break;
  case SW35xx::QC3:
    return "QC3.0";
    break;
  case SW35xx::FCP:
    return "FCP";
    break;
  case SW35xx::SCP:
    return "SCP";
    break;
  case SW35xx::PD_FIX:
    return "PD_Fix";
    break;
  case SW35xx::PD_PPS:
    return "PD_PPS";
    break;
  case SW35xx::MTKPE1:
    return "MTKPE1.1";
    break;
  case SW35xx::MTKPE2:
    return "MTKPE2.0";
    break;
  case SW35xx::LVDC:
    return "LVDC";
    break;
  case SW35xx::SFCP:
    return "SFCP";
    break;
  case SW35xx::AFC:
    return "AFC";
    break;
  case SW35xx::VOOC:
    return "VOOC";
    break;
  default:
    return "unknown";
    break;
  }
}

void setup() {
  Serial.begin(115200);
  Wire.begin(8, 9); // chân dữ liệu i2c(thay đổi tùy theo mạch), 8 là SDA / 9 là SCL đối với esp32c3 supermini
  sw.setMaxCurrent5A();
  tft.initR(INITR_144GREENTAB); // attach với màn hình tương ứng, ở đây đang dùng màn hình TFT 1.44inch ST7735
  tft.fillScreen(ST7735_BLACK); // Cài đặt màu nền
  tft.setFont(&Org_01);// cài đặt font chữ trong thư viện adafruit gfx
  tft.setRotation(3);  // xoay màn hình, để dọc thì 2 hoặc 4 còn ngang thì 1 hoặc 3
  ///////////////////////////////////////////tạo khung viền
  tft.drawRect(1, 42, 62, 52, ST7735_WHITE);
  tft.fillRect(1, 28, 62, 14, ST7735_WHITE);
  tft.drawRect(64, 42, 63, 52, ST7735_WHITE);
  tft.fillRect(64, 28, 63, 14, ST7735_WHITE);
  /////////////////////////////////////////// hiển thị tên cổng usb và chuẩn sạc
  printText("USB-A", ST7735_BLACK,4,39,2);
  printText("USB-C", ST7735_BLACK,67,39,2);
  printText("TYPE:", ST7735_GREEN,1,106,2);
  
}
void printText(char *text, uint16_t color, int x, int y,int textSize)
{
  tft.setCursor(x, y);
  tft.setTextColor(color);
  tft.setTextSize(textSize);
  tft.setTextWrap(true);
  tft.print(text);
}

void loop() {
  sw.readStatus();
  Serial.println("=======================================");
  
  // đổi đơn vị từ mV sang V// từ mA sang A
   vin_V  = sw.vin_mV / 1000.0;
   vout_V = sw.vout_mV / 1000.0;
   usba   = sw.iout_usba_mA/1000.0;
   usbc   = sw.iout_usbc_mA/1000.0;
   power_c = vout_V * usbc;
   power_a = vout_V * usba;
  //////////////////////////////////////////////
  //mở dòng này nến cần debug ra serial//
  //////////////////////////////////////////////
 /* Serial.printf("Current input voltage: %.2fV\n", vin_V);
  Serial.printf("Current output voltage: %.2fV\n", vout_V);
  Serial.printf("Current USB-C current:%dmA\r\n", sw.iout_usbc_mA);
  Serial.printf("Current USB-A current:%dmA\r\n", sw.iout_usba_mA);
  Serial.printf("Current fast charge type:%s\n", fastChargeType2String(sw.fastChargeType));
  if (sw.fastChargeType == SW35xx::PD_FIX || sw.fastChargeType == SW35xx::PD_PPS)
    Serial.printf("Current PD version:%d\n", sw.PDVersion);
  
  Serial.println("=======================================");
  Serial.println("");
  */
  // Hiển thị điện áp ra trên màn hình TFT
  
  dtostrf(vout_V, 4, 2, vout_str);  // Chuyển giá trị thành chuỗi với 2 chữ số thập phân
  dtostrf(vin_V, 4, 2, vin_str);
  dtostrf(usba , 4, 2, usba_str);
  dtostrf(usbc , 4, 2, usbc_str);
  dtostrf(power_c , 4, 2, power_c_str);
  dtostrf(power_a , 4, 2, power_a_str);
  //////////////////////////////////////////
  //cổng USB-A//
  tft.fillRect(4, 45, 52, 11, ST7735_BLACK);
  tft.setCursor(4, 53);
  tft.setTextColor(ST7735_YELLOW);
  tft.setTextSize(2);
  tft.setTextWrap(true);
  tft.printf("%sV", vout_str);  // Hiển thị giá trị vôn
  //////////////////////////////////////////
  tft.fillRect(4, 62, 52, 11, ST7735_BLACK);
  tft.setCursor(4, 70);
  tft.setTextColor(ST7735_CYAN);
  tft.setTextSize(2);
  tft.setTextWrap(true);
  tft.printf("%sA", usba_str);  // Hiển thị giá trị dòng điện
  //////////////////////////////////////////
  tft.fillRect(4, 80, 54, 11, ST7735_BLACK);
  tft.setCursor(4, 88);
  tft.setTextColor(ST7735_MAGENTA);
  tft.setTextSize(2);
  tft.setTextWrap(true);
  tft.printf("%sW", power_a_str);  // Hiển thị giá trị công suất
  //////////////////////////////////////////
  //Cổng USB-C//
  tft.fillRect(67, 45, 52, 11, ST7735_BLACK);
  tft.setCursor(67, 53);
  tft.setTextColor(ST7735_YELLOW);
  tft.setTextSize(2);
  tft.setTextWrap(true);
  tft.printf("%sV", vout_str);  // Hiển thị giá trị vôn
  //////////////////////////////////////////
  tft.fillRect(67, 62, 52, 11, ST7735_BLACK);
  tft.setCursor(67, 70);
  tft.setTextColor(ST7735_CYAN);
  tft.setTextSize(2);
  tft.setTextWrap(true);
  tft.printf("%sA", usbc_str);  // Hiển thị giá trị dòng điện
  //////////////////////////////////////////
  tft.fillRect(67, 80, 54, 11, ST7735_BLACK);
  tft.setCursor(67, 88);
  tft.setTextColor(ST7735_MAGENTA);
  tft.setTextSize(2);
  tft.setTextWrap(true);
  tft.printf("%sW", power_c_str);  // Hiển thị giá trị giá trị công suất
  /////////////////////////////////////////
  tft.fillRect(1, 111, 85, 14, ST7735_RED);
  tft.setCursor(1, 120);
  tft.setTextColor(ST7735_WHITE);
  tft.setTextSize(2);
  tft.setTextWrap(true);
  tft.printf(fastChargeType2String(sw.fastChargeType));  // Hiển thị chuẩn sạc đang nhận
  delay(300);
}

