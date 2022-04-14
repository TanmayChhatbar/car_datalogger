#include <Arduino.h>
#include "T10_V20.h"
#include <TFT_eSPI.h>
#include <SPI.h>
#include "WiFi.h"
#include <Wire.h>
#include <Ticker.h>
#include <Button2.h>
#include <SD.h>
#include "definitions.h"
#include "fcn_buzzer.h"
#include "out.h"
#include "fcn_SD.h"
#include "buttons.h"
#include "fcn_wifi.h"
#include "sensor2.h"

/*
  TOCHECK
  file.flush() - fix potential data loss if power cut, effects on write speed

  TODO
    SDFat library - file.sync() to preserve recent changes, like flush for sd library - FIGURE OUT HOW TO CHANGE SPI PINS
      https://forum.arduino.cc/t/sdfat-when-to-call-sync/272019
      https://forum.arduino.cc/t/explanation-of-sd-write-and-flush-please/369320/5
    
    make imu and GPS sensing interrupt based
    
    rtOS

    GPS - https://airu.coe.utah.edu/wp-content/uploads/sites/62/2017/09/adafruit-ultimate-gps.pdf
      what data is received
      connection stats on screen
      battery advantages
      timestamp from GPS
    
    IMU
      max refresh rate

    Design
      mount in car
      soft edges
      LCD cut in cap

  data processing
  https://www.youtube.com/watch?v=hJG08iWlres
  https://x-io.co.uk/open-source-imu-and-ahrs-algorithms/
  https://www.mathworks.com/help/fusion/ref/insfilterasync.html
*/

bool record = 1;
File file;

////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////

void setup() {
  const String vs = "v2.6";
  
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, RXP, TXP);
  delay(50);

  pindump();                      // Pin out Dump
  tft_init();                     //
  disp(vs, TFT_GREEN, 134, 119);
  spisd_test();                   // check if sd card connected and working
  buzzer_test();                  // buzzer test, but doesnt do anything
  button_init();

  imu_connect();

  gps_test();
  btnscanT.attach_ms(30, button_loop);

  record = 0;
  state = 1;
  buttonmarkers(record);
  tft.setTextSize(2);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////

unsigned long last_save = 0;
void loop() {
  // if in recording mode
  if (record == 1) {
    readMPU9250();
    write2SD(file, get_GPS());
    if (millis() - last_save > 3000) {
      last_save = millis();
      file.flush();
    }
    is_recording();

    // if need to stop recording
    if (state == 1) {
      state = 0;
      record = 0;
      file.close();
      disp("Recording", TFT_WHITE, 4, tft.height() / 2);
      disp("paused", TFT_WHITE, 4, tft.height() / 2 + 24);
      buttonmarkers(record);
      tft.fillRect(146, 2, 15, 15, TFT_BLACK);
      tft.setTextColor(TFT_BLACK, TFT_BLACK);
      tft.drawString(".", dx, dy);
    }
  }
  
  // if not recording, but need to start recording
  else if (state == 1) {
    state = 0;
    record = 1;
    file = new_file_open();          // get filenum
    String dsp = "Recording";
    disp(dsp, TFT_GREEN, 4, tft.height() / 2);
    dsp = "-> " + String(filenum) + ".txt";
    disp(dsp, TFT_GREEN, 4, tft.height() / 2 + 24);
    buttonmarkers(record);
    timer = millis();
  }
}
