#include <Wire.h>
#include <Adafruit_BMP280.h>

Adafruit_BMP280 bmp; 

void setup() {
  Serial.begin(9600);
  Serial.println(F("BMP280 test"));

  if (!bmp.begin(0x76)) {
    Serial.println(F("Could not find a valid BMP280 sensor, check wiring or "
                      "try a different address!"));
    while (1) delay(10);
  }
}

void loop() {
    Serial.print(F("Temperatura: "));
    Serial.print(bmp.readTemperature());
    Serial.println(" *C");

    float pres = bmp.readPressure();
    Serial.print(F("Pritisak: "));
    Serial.print(pres);
    Serial.println(" Pa");

    pres = pres / 100 + 17;
    Serial.print(F("Visina: "));
    Serial.print(bmp.readAltitude(pres));
    Serial.println(" m");

    Serial.println();
    delay(2000);
}
