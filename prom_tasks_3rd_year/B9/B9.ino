#include <Bounce2.h>
#include <LiquidCrystal_I2C.h>
#include <String.h>

#define MAX_TIME 10 * 60 * 1000

#define START_BTN 6
#define RESET_BTN 7

LiquidCrystal_I2C lcd(0x27, 16, 4);
Bounce start = Bounce();
Bounce restart = Bounce();

bool start_counting = false;
unsigned long start_time, stop_time;

void reset(){
  start_time = stop_time = 0;
  start_counting = false;
}

void printTime(unsigned long time1, unsigned long time2) {
  unsigned long time3 = time2 - time1;
  int min = time3 / 1000 / 60;
  int sec = time3 / 1000 - min * 60;
  int mil = (time3 % 1000) / 10;  
  String s_time = String(min) + ":" + String(sec) + ":" + String(mil);
  lcd.print(s_time);
  Serial.println(s_time);
  //Serial.println(time3);
}

void setup() {
  pinMode(START_BTN, INPUT_PULLUP);
  pinMode(RESET_BTN, INPUT_PULLUP);

  start.attach(START_BTN);
  start.interval(10);

  restart.attach(RESET_BTN);
  restart.interval(10);

  lcd.begin();
  lcd.backlight();

  Serial.begin(9600);

  reset();
}

void loop() {
  start.update();
  restart.update();

  if(restart.fell() || millis() - start_time > MAX_TIME){
    reset();
  }

  if(start.fell()) {
    if(!start_counting){
      start_counting = true;
      start_time = millis();
    }
    else {
      start_counting = false;
      stop_time = millis();
    }
  }
  
  printTime(start_time, start_counting ? millis() : stop_time);
}
