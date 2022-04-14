void tft_init() {
  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);
  tft.setTextSize(1);
  tft.setTextFont(1);
  tft.setTextColor(TFT_WHITE);
  tft.setCursor(0, 0);

  if (TFT_BL > 0) {
    pinMode(TFT_BL, OUTPUT);
    digitalWrite(TFT_BL, HIGH);
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////

void disp(String msg, int col, int locx, int locy) {
  Serial.println(msg);
  tft.setTextDatum(TL_DATUM); // Set datum to bottom centre
  tft.setTextColor(col, TFT_BLACK);
  tft.fillRect(locx, locy, 120, 20, TFT_BLACK);
  tft.drawString(msg, locx, locy);
  playSound();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////

unsigned long timer = 0;
void write2SD(File file, bool has_GPS) {
  String dl = ",";
  String tp = String((float)(millis() - timer) / 1000.0, 3);
  //  Serial.println(tp);
  String buf = tp + dl + String(IMU.ax) + dl + String(IMU.ay) + dl + String(IMU.az) + dl + String(IMU.gx) + \
               dl + String(IMU.gy) + dl + String(IMU.gz) + dl + String(IMU.mx) + dl + String(IMU.my) + dl + String(IMU.mz) + "\n";
  if (has_GPS)
    buf = buf + gps_data;  
  file.print(buf);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////

int filenum = 1;
File new_file_open() {
  // create new file
  String filename = "/" + String(filenum) + ".txt";
  while (SD.exists(filename)) {
    filenum++;
    filename = "/" + String(filenum) + ".txt";
  }

  // open file
  File f = SD.open(filename, FILE_WRITE);
  
  // populate it with headers
  gps_timestamp();
  if (f) {
    // time of log start
    Serial.println(gps_data);
    f.println(gps_data);
    
    // format for data
    f.println(F("latitude(2)°latitude(2.4)dir,longitude(2)°longitude(2.4)dir,alt,speed,angle,sat"));
    f.println(F("time,ax,ay,az,gx,gy,gz,mx,my,mz"));
  }
  return f;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////

unsigned long scr_del = 0, scr_up = 0;
bool up = 0;
void is_recording() {
#define dx 146
#define dy 2
  scr_del = millis() - scr_up;
  if (scr_del < 1000) {
    if (up == 1) {
      up = 0;
      tft.setTextColor(TFT_BLACK, TFT_BLACK);
      tft.drawString("o", dx, dy);
      tft.fillRect(dx + 2, dy + 6, 6, 6, TFT_BLACK);
    }
  }
  else {
    if (up == 0) {
      tft.setTextColor(TFT_RED, TFT_BLACK);
      tft.drawString("o", dx, dy);
      tft.fillRect(dx + 2, dy + 6, 6, 6, TFT_RED);
      up = 1;
    }
    if (scr_del > 2000)
      scr_up = millis();
  }
}
