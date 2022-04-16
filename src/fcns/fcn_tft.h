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
  tft.setTextColor(col, TFT_BLACK);
  tft.fillRect(locx, locy, 120, 20, TFT_BLACK);
  tft.drawString(msg, locx, locy);
  playSound();
}
