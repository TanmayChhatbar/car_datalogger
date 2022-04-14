void spisd_test() {
x:
  disp("SD Card: ", TFT_GREEN, 4, 4);
  if (SD_CS >  0) {
    sdSPI.begin(SD_SCLK, SD_MISO, SD_MOSI, SD_CS);
    if (!SD.begin(SD_CS, sdSPI)) {
      disp("SD Card: Mount fail", TFT_RED, 4, 4);
      delay(2000);
      goto x;
    }
    else {
      uint32_t cardSize = SD.cardSize() / (1024 * 1024);
      String str = "SD Card: " + String(cardSize) + "MB";
      disp(str, TFT_GREEN, 4, 4);
    }
    delay(50);
  }
}
