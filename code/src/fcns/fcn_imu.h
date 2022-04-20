void imu_connect() {
imu_start:
  if (I2C_SDA > 0) {
    Wire.begin(I2C_SDA, I2C_SCL);
    String str = "IMU:";
    disp(str, TXT_POSITIVE, 4, 16, 1);

    if (setupMPU9250() == 0) {
      str = "IMU: OK";
      disp(str, TXT_POSITIVE, 4, 16, 1);
    }
    else {
      str = "IMU: NOT OK";
      disp(str, TXT_NEGATIVE, 4, 16, 1);
      Serial.println("IMU error");
      delay(500);
      goto imu_start;
    }
  }
}