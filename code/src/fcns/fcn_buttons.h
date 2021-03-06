// with GPS side as top
// pos - state
// top - 3
// mid - 2
// bot - 1

void button_handle(uint8_t gpio) {

  switch (gpio) {
#ifdef BUTTON_1
    case BUTTON_1: {
        state = 1;
      }
      break;
#endif

#ifdef BUTTON_2
    case BUTTON_2: {
        state = 2;
      }
      break;
#endif

#ifdef BUTTON_3
    case BUTTON_3: {
        state = 3;
      }
      break;
#endif

#ifdef BUTTON_4
    case BUTTON_4: {
        state = 4;
      }
      break;
#endif
    default:
      break;
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////

void button_callback(Button2 &b) {
  for (int i = 0; i < sizeof(g_btns) / sizeof(g_btns[0]); ++i) {
    if (pBtns[i] == b) {
      // Serial.printf("btn: %u press\n", pBtns[i].getAttachPin());
      button_handle(pBtns[i].getAttachPin());
      // Serial.println(state);
    }
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////

void button_init() {
  uint8_t args = sizeof(g_btns) / sizeof(g_btns[0]);
  pBtns = new Button2 [args];
  for (int i = 0; i < args; ++i) {
    pBtns[i] = Button2(g_btns[i]);
    pBtns[i].setPressedHandler(button_callback);
  }
  pBtns[1].setLongClickHandler([](Button2 & b) {
    int x = tft.width() / 2 ;
    int y = tft.height() / 2 - 30;
    int r = digitalRead(TFT_BL);
    tft.setTextColor(TFT_GREEN, TFT_BLACK);
    tft.setTextDatum(MC_DATUM);
    tft.fillScreen(TFT_BLACK);

    tft.setTextColor(TFT_GREEN, TFT_BLACK);
    tft.drawString("Press again to wake up", x - 20, y + 30);
#ifdef ENABLE_MPU9250
    IMU.setSleepEnabled(true);
#endif

#ifndef ST7735_SLPIN
#define ST7735_SLPIN   0x10
#define ST7735_DISPOFF 0x28
#endif

    delay(300);
    tft.writecommand(ST7735_SLPIN);
    tft.writecommand(ST7735_DISPOFF);
    digitalWrite(TFT_BL, !r);
    delay(100);
    // esp_sleep_enable_ext0_wakeup((gpio_num_t )BUTTON_1, LOW);
    esp_sleep_enable_ext1_wakeup(((uint64_t)(((uint64_t)1) << BUTTON_1)), ESP_EXT1_WAKEUP_ALL_LOW);
    esp_deep_sleep_start();
  });
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////

void button_loop() {
  for (int i = 0; i < sizeof(g_btns) / sizeof(g_btns[0]); ++i) {
    pBtns[i].loop();
  }
}
