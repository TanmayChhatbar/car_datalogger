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

void gps_timestamp() {
  gps_data = "Date: ";
  gps_data = gps_data + "20" + String(GPS.year, DEC) + '/';
  gps_data = gps_data + String(GPS.month, DEC) + '/';
  gps_data = gps_data + String(GPS.day, DEC);
  gps_data = gps_data + "\nTime: ";

  // gps_data = "Time: ";
  if (GPS.hour < 10) { gps_data = gps_data + '0'; }
  gps_data = gps_data + String(GPS.hour, DEC) + ':';
  if (GPS.minute < 10) { gps_data = gps_data + '0'; }
  gps_data = gps_data + String(GPS.minute, DEC) + ':';
  if (GPS.seconds < 10) { gps_data = gps_data + '0'; }
  gps_data = gps_data + String(GPS.seconds, DEC) + '.';
  if (GPS.milliseconds < 10)
     gps_data = gps_data + "00";
  else if (GPS.milliseconds > 9 && GPS.milliseconds < 100)
     gps_data = gps_data + "0";
  gps_data = gps_data + String(GPS.milliseconds);  
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////

int setup_GPS() {
  //  return  condition
  //  0       all ok
  //  1       not ok
  //  2       no fix

  unsigned long gps_up = millis();      
  while(!GPS.begin(9600))
    if (millis() - gps_up > 5000)
      return 1;

  // uncomment this line to turn on RMC (recommended minimum) and GGA (fix data) including altitude
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  
setup_start:
  gps_up = millis();
  GPS.fix = 0;
  while(!GPS.fix){
    // check for timeout
    if (millis() - gps_up > 5000){
      Serial.println(GPS.satellites);
      return 2;
    }
    // try to read and parse GPS data until fixed
    GPS.read();
    if (GPS.newNMEAreceived())
      GPS.parse(GPS.lastNMEA());
  }
  // increase baud rate, TOCHECK
  // GPS.sendCommand(PMTK_SET_BAUD_57600);
  // GPS.begin(57600);
  // GPS.sendCommand(PMTK_SET_NMEA_UPDATE_5HZ);

  // if fix is found
  if(GPS.year == 0)
    goto setup_start;
  gps_timestamp();
  return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool clear_gps = 0;
unsigned long tm = 0;
bool get_GPS() {
  GPS.read();

  if (GPS.newNMEAreceived()) {
    if (!GPS.parse(GPS.lastNMEA()))
      return 0; 
    else {
      gps_data = "GPS:"; // filter based on this
      if (GPS.fix) {
        gps_data = gps_data + String(GPS.latitude, 4) + String(GPS.lat) + ',';
        gps_data = gps_data + String(GPS.longitude, 4) + String(GPS.lon) + ',';
        gps_data = gps_data + String(GPS.altitude, 4) + ',';
        gps_data = gps_data + String(GPS.speed) + ',';        
        gps_data = gps_data + String(GPS.angle) + ',';  // knots
        gps_data = gps_data + String(GPS.satellites) + '\n';
        // Serial.println(gps_data);
        return 1;
      }
    }
  }
  return 0; 
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////

void gps_test() {
y:
  tft.setTextDatum(TL_DATUM); // Set datum to bottom centre
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  String str = "GPS: ";
  tft.drawString(str, 4, 28);
  Serial.print(str);
  
  int gps_state = setup_GPS();
  if (gps_state == 0) {
    str = "GPS: OK";
    Serial.println("OK");
    tft.drawString(str, 4, 28);
  }
  else if (gps_state == 2) {
    tft.setTextColor(TFT_PURPLE, TFT_BLACK);
    str = "GPS: NO FIX";
    tft.drawString(str, 4, 28);
    Serial.println("NO FIX");
    goto y;
  }
  else {
    tft.setTextColor(TFT_RED, TFT_BLACK);
    str = "GPS: NOT OK";
    tft.drawString(str, 4, 28);
    Serial.println("NOT OK");
    delay(500);
    goto y;
  }
}
