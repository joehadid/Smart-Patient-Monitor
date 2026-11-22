#include <Wire.h>
#include <Adafruit_MLX90614.h>
#include "MAX30100_PulseOximeter.h"

Adafruit_MLX90614 mlx = Adafruit_MLX90614();
PulseOximeter pox;

const int greenLed = 6;
const int redLed = 7;
const int buzzer = 8;

uint32_t tsLastReport = 0;

void onBeatDetected() {
}

void setup() {
    Serial.begin(9600);
    
    pinMode(greenLed, OUTPUT);
    pinMode(redLed, OUTPUT);
    pinMode(buzzer, OUTPUT);

    mlx.begin();
    pox.begin();
    pox.setOnBeatDetectedCallback(onBeatDetected);

    digitalWrite(greenLed, HIGH);
    digitalWrite(redLed, LOW);
    noTone(buzzer);
}

void loop() {
    pox.update();

    if (millis() - tsLastReport > 1500) {
        float temp = mlx.readObjectTempC();
        float bpm = pox.getHeartRate();
        int spo2 = pox.getSpO2();

        Serial.print("{\"temp\":");
        Serial.print(temp, 1);
        Serial.print(",\"bpm\":");
        Serial.print((int)bpm);
        Serial.print(",\"spo2\":");
        Serial.print(spo2);
        Serial.println("}");

        if (temp > 37.5 || (spo2 > 50 && spo2 < 94)) {
            digitalWrite(redLed, HIGH);
            digitalWrite(greenLed, LOW);
            tone(buzzer, 1000, 300);
        } else {
            digitalWrite(redLed, LOW);
            digitalWrite(greenLed, HIGH);
            noTone(buzzer);
        }

        tsLastReport = millis();
    }
}
