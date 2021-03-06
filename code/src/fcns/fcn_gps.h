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

void gps_test() {
  String str = "GPS: ";
  disp(str, TXT_POSITIVE, 4, 28, 1);
y:  
  int gps_state = setup_GPS();
  if (gps_state == 0 or gps_state == 2) {
    str = "GPS: OK";
    disp(str, TXT_POSITIVE, 4, 28, 1);
  }
  else if (gps_state == 2) {
    str = "GPS: NO FIX";
    disp(str, TXT_NEUTRAL, 4, 28, 1);
    Serial.println("NO FIX");
    goto y;
  }
  else {
    str = "GPS: NOT OK";
    disp(str, TXT_NEGATIVE, 4, 28, 1);
    Serial.println("NOT OK");
    delay(500);
    goto y;
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////

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
        return 1;
      }
    }
  }
  return 0; 
}
