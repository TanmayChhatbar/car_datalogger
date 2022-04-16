// Standard libraries
#include <Arduino.h>
#include "T10_V20.h"
#include <SPI.h>
#include <TFT_eSPI.h>
#include <Wire.h>
#include <Ticker.h>
#include <Button2.h>
#include <SD.h>
#include <Adafruit_GPS.h>

// Helper file settings
// #define PIN_DUMP  // dump all pins used
// #define STARTUP_REC // uncomment to start recording on startup by default
#define LOG_SERIAL_ONLY // comment to log to SD

// Helper files
#include "definitions.h"
#include "fcn_buzzer.h"
#include "fcn_imu.h"
#include "fcn_gps.h"
#include "fcn_tft.h"
#include "fcn_SD.h"
#include "fcn_buttons.h"

/*
  TOCHECK
  file.flush() - fix potential data loss if power cut, effects on write speed

  TODO
    SDFat library - file.sync() to preserve recent changes, like flush for sd library - FIGURE OUT HOW TO CHANGE SPI PINS
      https://forum.arduino.cc/t/sdfat-when-to-call-sync/272019
      https://forum.arduino.cc/t/explanation-of-sd-write-and-flush-please/369320/5
    
    make imu and GPS sensing interrupt based
    
    rtOS
      standardise refresh rate

    GPS - https://airu.coe.utah.edu/wp-content/uploads/sites/62/2017/09/adafruit-ultimate-gps.pdf
      connection stats on screen
      battery advantages
    
    IMU
      max refresh rate

    Design
      soft edges
      LCD cut in cap
    
    Screen
      backlight
    
    WiFi
      autobackup
    
    Tidy serial output


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
  const String vs = "v2.7";
  
  Serial.begin(115200);
  delay(50);
#ifdef PIN_DUMP
  pindump();                      // Pin out Dump
#endif
  tft_init();                     //
  disp(vs, TFT_GREEN, 134, 119, 1);
  spisd_test();                   // check if sd card connected and working
  buzzer_test();                  // buzzer test, but doesnt do anything
  button_init();

  imu_connect();

  gps_test();
  btnscanT.attach_ms(30, button_loop);

#ifdef STARTUP_REC
  record = 0;
#else
  record = 1;
#endif
  state = 1;

  buttonmarkers(record);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////

unsigned long last_save = 0;
bool lastfix = 0;
void loop() {
  // backlit control
  //TODO
  
  // update GPS status on screen
  if (!GPS.fix and lastfix == 1){  
    tft.setTextSize(1);
    tft.setTextColor(TFT_PURPLE, TFT_BLACK);
    tft.drawString("GPS: NO FIX", 4, 28);
    Serial.println("NO FIX");
    lastfix = 0;
  }
  else if (GPS.fix and lastfix == 0) {
    tft.setTextSize(1);
    tft.setTextColor(TFT_GREEN, TFT_BLACK);
    Serial.println("OK");
    tft.drawString("GPS: OK", 4, 28);
    lastfix = 1;
  }

  // if in recording mode
  if (record == 1) {
    readMPU9250();
    data_out(file, get_GPS());

    if (millis() - last_save > 3000) {
      last_save = millis();
      file.flush();
    }
    is_recording();

    // if need to stop recording
    if (state == 1) {
      state = 0;
      tft.setTextSize(2);
      record = 0;
      file.close();
      disp("Logging", TFT_WHITE, 4, tft.height() / 2, 0);
      disp("Paused", TFT_WHITE, 4, tft.height() / 2 + 24, 1);
      buttonmarkers(record);
      tft.fillRect(146, 2, 15, 15, TFT_BLACK);
      tft.setTextColor(TFT_BLACK, TFT_BLACK);
      tft.drawString(".", dx, dy);
    }
  }
  
  // if not recording, but need to start recording
  else if (state == 1) {
    state = 0;
    tft.setTextSize(2);
    record = 1;
    file = new_file_open();          // get filenum
    String dsp = "Logging";
    disp(dsp, TFT_GREEN, 4, tft.height() / 2, 0);
#ifndef LOG_SERIAL_ONLY
    dsp = "-> " + String(filenum) + ".txt";
#else
    dsp = "to Serial";
#endif
    disp(dsp, TFT_GREEN, 4, tft.height() / 2 + 24, 1);
    buttonmarkers(record);
    timer_stamp = millis();
  }
}
