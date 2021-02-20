#include <String.h>

#define RED_LED 6
#define GREEN_LED 7

unsigned long Time;

int intervals[4] = {500, 1000, 1500, 2000};
int interval = 0;
enum job_list {RED, GREEN, NONE} job = NONE;

String command = "";
bool com = false;
bool state = false;

void hard_test() {
  digitalWrite(GREEN_LED, HIGH);
  digitalWrite(RED_LED, HIGH);
  Serial.println(analogRead(A0));
  delay(2000);
}

void reset() {
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(RED_LED, LOW);
}

void setup() {
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);

  Serial.begin(9600);

  hard_test();
  reset();

  Time = millis();
}

void loop() {
  if (Serial.available()) {
    command = Serial.readString();
    command.trim();
    com = true;
  }

  if (com) {
    if (command == "UVECAJ" && interval < 3) interval++;
    else if (command == "SMANJI" && interval > 0) interval--;
    else if (command == "ZELENA") job = GREEN;
    else if (command == "CRVENA") job = RED;
    com = false;
  }

  if (job == GREEN) {
    digitalWrite(RED_LED, LOW);
    digitalWrite(GREEN_LED, state);
  }
  if (job == RED) {
    digitalWrite(RED_LED, LOW);
    digitalWrite(GREEN_LED, state);
  }

  if (millis() - Time > intervals[interval]) {
    Time = millis();
    state = !state;
    if (job != NONE) {
      Serial.print("Interval: ");
      Serial.println(intervals[interval] / 1000.0);
      Serial.print("Dioda: ");
      if (job == GREEN) Serial.println("ZELENA");
      if (job == RED) Serial.println("CRVENA");
      Serial.print("Stanje: ");
      if (state == true) Serial.println("UPALJENA");
      if (state == false) Serial.println("UGASENA");
    }
  }
}
