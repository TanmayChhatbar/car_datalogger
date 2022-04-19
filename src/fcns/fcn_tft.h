
#ifndef LIGHTMODE
#define TXT_PRIMARY TFT_WHITE
#define TXT_BACKGROUND TFT_BLACK
#define TXT_POSITIVE TFT_GREEN
#define TXT_NEGATIVE TFT_ORANGE
#define TXT_NEUTRAL TFT_ORANGE
#define TXT_SECONDARY TFT_WHITE
#else
#define TXT_PRIMARY TFT_BLACK
#define TXT_BACKGROUND TFT_WHITE
#define TXT_POSITIVE TFT_DARKGREEN
#define TXT_NEGATIVE TFT_RED
#define TXT_NEUTRAL TFT_ORANGE
#define TXT_SECONDARY TFT_DARKGREY
#endif

void tft_init() {
  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TXT_BACKGROUND);
  tft.setTextSize(1);
  tft.setTextFont(1);
  tft.setTextColor(TXT_PRIMARY);
  tft.setCursor(0, 0);

  if (TFT_BL > 0) {
    pinMode(TFT_BL, OUTPUT);
    digitalWrite(TFT_BL, HIGH);
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////

void is_recording() {
#define dx 146
#define dy 2
  scr_del = millis() - scr_up;
  if (scr_del < 1000) {
    if (up == 1) {
      up = 0;
      // tft.setTextColor(TFT_BLACK, TFT_BLACK);
      // tft.drawString("o", dx, dy);
      // tft.fillRect(dx + 2, dy + 6, 6, 6, TFT_BLACK);
      tft.fillCircle(152, 10, 4, TFT_RED);
}
  }
  else {
    if (up == 0) {
      up = 1;
      // tft.setTextColor(TFT_RED, TFT_BLACK);
      // tft.drawString("o", dx, dy);
      // tft.fillRect(dx + 2, dy + 6, 6, 6, TFT_RED);
      tft.fillCircle(152, 10, 4, TXT_BACKGROUND);
    }
    if (scr_del > 2000)
      scr_up = millis();
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////

void disp(String msg, int col, int locx, int locy, bool nextline) {
  if (nextline)
    Serial.println(msg);
  else {
    Serial.print(msg);
    Serial.print(' ');
  }
  tft.setTextDatum(TL_DATUM); // Set datum to bottom centre
  tft.setTextColor(col, TXT_BACKGROUND);
  tft.fillRect(locx, locy, 120, 20, TXT_BACKGROUND);
  tft.drawString(msg, locx, locy);
  playSound();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////

void buttonmarkers(bool record) {
  // size of the markers
#define xh 5
#define xh2 3
#define yh 3
  int xp, yp;   // location

  // backlit
  xp = 148;
  yp = 72;
  tft.fillCircle(xp, yp, 2, TXT_SECONDARY);
  tft.drawLine(xp, yp - yh-1, xp, yp + yh+1, TXT_SECONDARY);
  tft.drawLine(xp - yh-1, yp, xp + yh+1, yp, TXT_SECONDARY);
  tft.drawLine(xp + yh, yp - yh, xp - yh, yp + yh, TXT_SECONDARY);
  tft.drawLine(xp - yh, yp - yh, xp + yh, yp + yh, TXT_SECONDARY);

  // play/pause button
  xp = 150, yp = 108;
  tft.fillRect(xp - xh, yp - yh, xh + 2, 2 * yh + 2, TXT_BACKGROUND);            // clear space
  
  if (record == 0) {
    tft.fillTriangle(xp, yp, xp - xh, yp - yh, xp - xh, yp + yh, TXT_SECONDARY);    // play button
  }
  else {
    tft.drawLine(xp - xh,       yp - yh, xp - xh,       yp + yh, TXT_SECONDARY);    // pause button
    tft.drawLine(xp - xh + xh2, yp - yh, xp - xh + xh2, yp + yh, TXT_SECONDARY);
  }
  xp = 160, yp = 108; tft.fillTriangle(xp, yp, xp - xh, yp - yh, xp - xh, yp + yh, TXT_SECONDARY);  // button markers
  xp = 160, yp = 72;  tft.fillTriangle(xp, yp, xp - xh, yp - yh, xp - xh, yp + yh, TXT_SECONDARY);  // button markers
  xp = 160, yp = 36;  tft.fillTriangle(xp, yp, xp - xh, yp - yh, xp - xh, yp + yh, TXT_SECONDARY);  // button markers
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////

void gps_update_screen() {
    // update GPS status on screen
  if (!GPS.fix and lastfix == 1){  
    disp("GPS: NO FIX", TXT_NEUTRAL, 4, 28, 1);
    Serial.println("NO FIX");
    lastfix = 0;
  }
  else if (GPS.fix and lastfix == 0) {
    Serial.println("OK");
    disp("GPS: OK", TXT_POSITIVE, 4, 28, 1);
    lastfix = 1;
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef ENABLE_SS
void screenshot(){
  for (int j = 0; j < 128; j++)
    for (int i = 0; i < 160; i++){
      Serial.print(j);
      Serial.print(',');
      Serial.print(i);
      Serial.print(',');
      Serial.println(tft.readPixel(j, i));
    }
  delay(6000);
}
#endif
