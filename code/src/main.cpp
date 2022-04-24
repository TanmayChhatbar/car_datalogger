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
// #define PIN_DUMP         // dump all pins used at setup
// #define STARTUP_REC      // uncomment to start recording on startup by default
// #define LOG_SERIAL_ONLY  // comment to log to SD
// #define LIGHTMODE        // switch to light UI
// #define ENABLE_SS           // top button for screenshot to serial

// Helper files
#include "definitions.h"
#include "fcn_buzzer.h"
#include "fcn_tft.h"
#include "fcn_imu.h"
#include "fcn_gps.h"
#include "fcn_SD.h"
#include "fcn_buttons.h"
// #include "fcn_wifi.h"

/*
  TOCHECK
  file.flush() - fix potential data loss if power cut, effects on write speed

  TODO
    animation
      screenshot for the animation
      
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
    
    WiFi
      autobackup

  data processing
  https://www.youtube.com/watch?v=hJG08iWlres
  https://x-io.co.uk/open-source-imu-and-ahrs-algorithms/
  https://www.mathworks.com/help/fusion/ref/insfilterasync.html
*/

////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////

void setup() {
  const String vs = "v2.8";
  Serial.println(vs);
  button_init();
  btnscanT.attach_ms(30, button_loop);
#ifdef STARTUP_REC
  record = 0;
#else
  record = 1;
#endif
  state = 1;
  
  Serial.begin(115200);
  delay(50);
#ifdef PIN_DUMP
  pindump();                      // Pin out Dump
#endif
  tft_init();                     //
  disp(vs, TXT_SECONDARY, 134, 119, 1);
  spisd_test();                   // check if sd card connected and working
  buzzer_test();                  // buzzer test, but doesnt do anything

  imu_connect();

  gps_test();

  buttonmarkers(record);

  // if (state == 3){
  //   backuptowifi();
  // }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////

void loop() {
#ifdef ENABLE_SS
  if (state == 3){
    state = 0;
    screenshot();
  }
#endif

  // backlit toggle
  if (state == 2){
    state = 0;
    if (digitalRead(TFT_BL))
      digitalWrite(TFT_BL, LOW);
    else digitalWrite(TFT_BL, HIGH);
  }
  
  gps_update_screen();

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
      record = 0;
      file.close();
      tft.setTextSize(2);
      disp("Logging", TXT_NEUTRAL, 4, tft.height() / 2, 0);
      disp("Paused", TXT_NEUTRAL, 4, tft.height() / 2 + 24, 1);
      tft.setTextSize(1);
      buttonmarkers(record);
      tft.fillRect(146, 2, 15, 15, TXT_BACKGROUND);
    }
  }
  
  // if not recording, but need to start recording
  else if (state == 1) {
    state = 0;
    record = 1;
    file = new_file_open();          // get filenum
    tft.setTextSize(2);
    disp("Logging", TXT_POSITIVE, 4, tft.height() / 2, 0);
#ifdef LOG_SERIAL_ONLY
    disp("to Serial", TXT_POSITIVE, 4, tft.height() / 2 + 24, 1);
#else
    disp("-> " + String(filenum) + ".txt", TXT_POSITIVE, 4, tft.height() / 2 + 24, 1);
#endif
    tft.setTextSize(1);
    buttonmarkers(record);
    timer_stamp = millis();
  }
}
