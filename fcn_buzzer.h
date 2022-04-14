void playSound(void) {
    if (SPEAKER_OUT > 0) {
        if (SPEAKER_PWD > 0) {
            digitalWrite(SPEAKER_PWD, HIGH);
            delay(50);
        }
        ledcWriteTone(CHANNEL_0, 1000);
        delay(50);
        ledcWriteTone(CHANNEL_0, 0);
        if (SPEAKER_PWD > 0) {
            delay(50);
            digitalWrite(SPEAKER_PWD, LOW);
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////

void buzzer_test() {
    if (SPEAKER_OUT > 0) {
        if (SPEAKER_PWD > 0) {
            pinMode(SPEAKER_PWD, OUTPUT);
        }
        ledcSetup(CHANNEL_0, 1000, 8);
        ledcAttachPin(SPEAKER_OUT, CHANNEL_0);
    }
}
