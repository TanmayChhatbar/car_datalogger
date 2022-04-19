void spisd_test() {
#ifdef LOG_SERIAL_ONLY
  String str = "SD Card: Serial only";
  disp(str, TXT_POSITIVE, 4, 4, 1);

#else
x:
  disp("SD Card: ", TXT_POSITIVE, 4, 4, 0);
  
  if (SD_CS >  0) {
    sdSPI.begin(SD_SCLK, SD_MISO, SD_MOSI, SD_CS);
    if (!SD.begin(SD_CS, sdSPI)) {
      disp("SD Card: Mount fail", TXT_NEGATIVE, 4, 4, 1);
      delay(2000);
      goto x;
    }
    else {
      uint32_t cardSize = SD.cardSize() / (1024 * 1024);
      String str = "SD Card: " + String(cardSize) + "MB";
      disp(str, TXT_POSITIVE, 4, 4, 1);
    }
    delay(50);
  }
#endif
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////

void data_out(File file, bool has_GPS) {
  String dl = ",";
  String tp = String((float)(millis() - timer_stamp) / 1000.0, 3);
  //  Serial.println(tp);
  String buf = tp + dl + String(IMU.ax) + dl + String(IMU.ay) + dl + String(IMU.az) + dl + String(IMU.gx) + \
               dl + String(IMU.gy) + dl + String(IMU.gz) + dl + String(IMU.mx) + dl + String(IMU.my) + dl + String(IMU.mz) + "\n";
  if (has_GPS)
    buf = buf + gps_data;  
#ifdef LOG_SERIAL_ONLY
  Serial.print(buf);
#else
  file.print(buf);
#endif
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////

File new_file_open() {
#ifndef LOG_SERIAL_ONLY
  // create new file
  String filename = "/" + String(filenum) + ".txt";
  while (SD.exists(filename)) {
    filenum++;
    filename = "/" + String(filenum) + ".txt";
  }

  // open file
  File f = SD.open(filename, FILE_WRITE);
#endif

  // populate it with headers
  gps_timestamp();
    // time of log start
    // format for data
#ifndef LOG_SERIAL_ONLY
  if (f) {
    f.println(gps_data);
    f.println(F("latitude(2)°latitude(2.4)dir,longitude(2)°longitude(2.4)dir,alt,speed,angle,sat"));
    f.println(F("time,ax,ay,az,gx,gy,gz,mx,my,mz"));
  }
  return f;
#else
  Serial.println(gps_data);
  Serial.println(F("latitude(2)°latitude(2.4)dir,longitude(2)°longitude(2.4)dir,alt,speed,angle,sat"));
  Serial.println(F("time,ax,ay,az,gx,gy,gz,mx,my,mz"));
  File f;
  return f;
#endif

}
