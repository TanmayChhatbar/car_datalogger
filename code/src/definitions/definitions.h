// TFT
TFT_eSPI tft = TFT_eSPI(); // Invoke custom library
unsigned long scr_del = 0, scr_up = 0;
bool up = 0;

// IMU
#ifdef ENABLE_MPU9250
#include "sensor.h"
extern MPU9250 IMU;
#endif

// SD
SPIClass sdSPI(VSPI);
#define IP5306_ADDR         0X75
#define IP5306_REG_SYS_CTL0 0x00
unsigned long timer_stamp = 0;
int filenum = 1;
unsigned long last_save = 0;
bool record = 1;
File file;

// buttons
uint8_t state = 0;
Button2 *pBtns = nullptr;
uint8_t g_btns[] =  BUTTONS_MAP;
char buff[512];
Ticker btnscanT;

// GPS
#define GPSSerial Serial2
Adafruit_GPS GPS(&GPSSerial);
#define GPSECHO false
String gps_data = "";
bool clear_gps = 0;
unsigned long tm = 0;
bool lastfix = 0;

////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////

void pindump() {
    Serial.printf("Current select %s version\n", BOARD_VRESION);
    Serial.printf("TFT_MISO:%d\n", TFT_MISO);
    Serial.printf("TFT_MOSI:%d\n", TFT_MOSI);
    Serial.printf("TFT_SCLK:%d\n", TFT_SCLK);
    Serial.printf("TFT_CS:%d\n", TFT_CS);
    Serial.printf("TFT_DC:%d\n", TFT_DC);
    Serial.printf("TFT_RST:%d\n", TFT_RST);
    Serial.printf("TFT_BL:%d\n", TFT_BL);
    Serial.printf("SD_MISO:%d\n", SD_MISO);
    Serial.printf("SD_MOSI:%d\n", SD_MOSI);
    Serial.printf("SD_SCLK:%d\n", SD_SCLK);
    Serial.printf("SD_CS:%d\n", SD_CS);
    Serial.printf("I2C_SDA:%d\n", I2C_SDA);
    Serial.printf("I2C_SCL:%d\n", I2C_SCL);
    Serial.printf("SPEAKER_PWD:%d\n", SPEAKER_PWD);
    Serial.printf("SPEAKER_OUT:%d\n", SPEAKER_OUT);
    Serial.printf("ADC_IN:%d\n", ADC_IN);
    Serial.printf("BUTTON_1:%d\n", BUTTON_1);
    Serial.printf("BUTTON_2:%d\n", BUTTON_2);
    Serial.printf("BUTTON_3:%d\n", BUTTON_3);
    Serial.printf("GPS TX:%d\n", TX2);
    Serial.printf("GPS RX:%d\n", RX2);
#ifdef BUTTON_4
    Serial.printf("BUTTON_4:%d\n", BUTTON_4);
#endif
}