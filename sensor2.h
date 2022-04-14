void imu_connect() {
  if (I2C_SDA > 0) {
    Wire.begin(I2C_SDA, I2C_SCL);
    tft.setTextDatum(TL_DATUM); // Set datum to bottom centre
    tft.setTextColor(TFT_GREEN, TFT_BLACK);
    String str = "IMU:";
    tft.drawString(str, 4, 16);

    if (setupMPU9250() == 0) {
      str = "IMU: OK";
      tft.drawString(str, 4, 16);
    }
    else {
      tft.setTextColor(TFT_RED, TFT_BLACK);
      str = "IMU: NOT OK";
      tft.drawString(str, 4, 16);
      Serial.println("IMU error");
      while (1) {}
    }
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define RXP 19
#define TXP 18
bool setup_GPS() {
  unsigned long gps_up = millis();
  while (!Serial2.available())
    if (millis() - gps_up > 5000)
      return 1;
      
  Serial2.begin(9600, SERIAL_8N1, RXP, TXP);
  delay(50);
//  Serial2.println("$PMTK251,38400*27<CR><LF>");   // higher baud rate
//  Serial2.begin(38400, SERIAL_8N1, RXP, TXP);
//  delay(200);
  Serial2.println("$PMTK220,250*29<CR><LF>");     // 4Hz   http://www.hhhh.org/wiml/proj/nmeaxor.html

  return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool clear_gps = 0;
unsigned long tm = 0;
bool get_GPS() {
  // clear gps if read the last time
  if (clear_gps) {
    gps_data = "";
    clear_gps  = 0;
  }
  // collect all data
  while (Serial2.available()) {
    gps_data = gps_data + String(char(Serial2.read()));
    tm = millis();
  }
  // if all data from gps for this cycle collected, flag to be saved to SD
  if (millis() - tm > 2 and !gps_data.equals("\n")) {
    clear_gps = 1;
    tm = millis();
    return 1;
  }
  return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////

void gps_test() {
y:
  tft.setTextDatum(TL_DATUM); // Set datum to bottom centre
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  String str = "GPS:";
  tft.drawString(str, 4, 28);
  delay(100);
  if (setup_GPS() == 0) {
    str = "GPS: OK";
    tft.drawString(str, 4, 28);
  }
  else {
    tft.setTextColor(TFT_RED, TFT_BLACK);
    str = "GPS: NOT OK";
    //    tft.setTextColor(TFT_PURPLE, TFT_BLACK);
    //    str = "GPS: TODO";
    tft.drawString(str, 4, 28);
    Serial.println("GPS error");
    delay(1000);
    goto y;
  }
}
