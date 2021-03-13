#include <Bounce2.h>
#define MIN_TIME 200
#define MAX_TIME 2000

int leds[8] = {13,12,11,10,9,8,7,6};
int pot = A0;
int DECR = 2;
int INCR = 3;

int i;

int length = 1;
int current = 0;
int s_down;

int TIME = 200;
unsigned long time;

Bounce dec = Bounce();
Bounce inc = Bounce();

void setup() {
  Serial.begin(9600);
  
  for(i = 0; i < 8; i++){
    pinMode(leds[i], OUTPUT);
  }

  pinMode(pot, INPUT);

  pinMode(DECR, INPUT_PULLUP);
  pinMode(INCR, INPUT_PULLUP);

  dec.attach(DECR);
  inc.attach(INCR);

  dec.interval(15);
  inc.interval(15);

  time = millis();
}

void loop() {
  dec.update();
  inc.update();

  if(dec.fell() && length > 1) {
    length--;
    s_down = current - length;
    if(s_down < 0) s_down += 8;
    digitalWrite(leds[s_down], LOW);
  }
  if(inc.fell() && length < 8) {
    length++;
    current++;
    if(current > 7) current = 0;
    digitalWrite(leds[current], HIGH);
  }

  Serial.println(length);
  TIME = map(analogRead(pot), 1023, 0, 200, 2000);

  if(millis() - time > TIME && length > 0){
    current++;
    s_down = current - length;
    if(s_down < 0) s_down += 8;
    if(current > 7) current = 0;

    digitalWrite(leds[current], HIGH);
    if(length != 8) digitalWrite(leds[s_down], LOW);
    time = millis();
  }

}
