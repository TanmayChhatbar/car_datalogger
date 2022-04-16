void imu_connect() {
imu_start:
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
      delay(500);
      goto imu_start;
    }
  }
}