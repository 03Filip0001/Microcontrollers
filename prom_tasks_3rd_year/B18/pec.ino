#include <Bounce2.h>
#include <EEPROM.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 8
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

Bounce petlja1 = Bounce();
Bounce petlja2 = Bounce();

float minni_temp = 0;
float maxi_temp = 1;

float Celsius = 0;

unsigned long Time;

int led_crvena = 7;
int led_zuta = 6;
int led_zelena = 5;

int btn_start = 3;
int btn2 = 2;
bool BUTTONstate;

bool check = false;
int i = 0;
char input[3];

void setup() {

  pinMode(led_crvena, OUTPUT);
  pinMode(led_zuta, OUTPUT);
  pinMode(led_zelena, OUTPUT);
  pinMode(btn_start, INPUT);
  pinMode(btn2, INPUT);

  sensors.begin();
  Serial.begin(9600);
  //24 - 27

  petlja1.attach(btn_start);
  petlja1.interval(10);
  Time = millis();

  petlja2.attach(btn2);
  petlja2.interval(10);

  minni_temp = EEPROM.read(24);
  maxi_temp = EEPROM.read(27);
}

void loop() {

  

  while (Serial.available()) {
    char ch = Serial.read();
    if (ch == '\n')
    {
      if (input[0] == '-') {
        minni_temp = (input[1] - '0') * 10 + (input[2] - '0');
      }
      else {
        maxi_temp = (input[1] - '0') * 10 + (input[2] - '0');
      }
      EEPROM.write(24, minni_temp);
      EEPROM.write(27, maxi_temp);

      Serial.print("Minimalna temperatura: \n");
      Serial.print(minni_temp);
      Serial.print("Maksimalna temperatura: \n");
      Serial.print(maxi_temp);
    }
    else
    {
      input[i] = ch;
      i++;
    }
  }
  i = 0;



  petlja1.update();
  petlja2.update();

  sensors.requestTemperatures();

  Celsius = sensors.getTempCByIndex(0);

  if (millis() - Time > 2000) {
    Serial.print(Celsius);
    Serial.print(" C\n");
    Time = millis();
  }

  if (petlja1.fell())
  {
    BUTTONstate = true;
  }
  if (petlja2.fell())
  {
    BUTTONstate = false;
  }

  if (BUTTONstate) {
    digitalWrite(led_zelena, HIGH);
  }
  else {
    digitalWrite(led_zelena, LOW);
  }

  if (Celsius < minni_temp && BUTTONstate == true) digitalWrite(led_crvena, HIGH);
  else digitalWrite(led_crvena, LOW);

  if (Celsius > maxi_temp && BUTTONstate == true) digitalWrite(led_zuta, HIGH);
  else digitalWrite(led_zuta, LOW);
}
