#include <SegmentDisplay_CD4511B.h>

#define MAX_NUM 10

#define A 8
#define B 9
#define C 10
#define D 11

#define DIG_4 2
#define DIG_3 3
#define DIG_2 4
#define DIG_1 5

#define MAX 20
#define MIN 2
#define WAIT_TIME 250
#define MAX_TIME 2000

SegmentDisplay_CD4511B dis(A , B, C, D, 1, 5);
unsigned int Time;

byte dig[4];

int pl = 1;
int count;
int number = 4;
int mult = 0;

bool done = false;

byte input[4] = {0, 0, 0, 0};
byte i = 0;
int kombinacije[20];
int kombinacija;
byte count_komb = 0;
bool start = false;

byte dobitnici = 0;

bool finish = false;

void upali_cifru(int cifra) {
  ugasiSve();

  switch (cifra) {
    case 1: digitalWrite(DIG_1, LOW); break;
    case 2: digitalWrite(DIG_2, LOW); break;
    case 3: digitalWrite(DIG_3, LOW); break;
    case 4: digitalWrite(DIG_4, LOW); break;
    default: Serial.println("GRESKA - CIFRA");
  }
}

void ugasiSve() {
  digitalWrite(DIG_1, HIGH);
  digitalWrite(DIG_2, HIGH);
  digitalWrite(DIG_3, HIGH);
  digitalWrite(DIG_4, HIGH);
}

void izracunaj() {
  if (input[3] == 0 && input[2] == 0 && input[1] == 0 && input[0] == 0) kombinacije[count_komb] = 0;
  else if (input[3] == 0 && input[2] == 0 && input[1] == 0  && i == 1) kombinacije[count_komb] = input[0];
  else if (input[3] == 0 && input[2] == 0 && i == 2) kombinacije[count_komb] = input[1] + input[0] * 10;
  else if (input[3] == 0 && i == 3) kombinacije[count_komb] = input[2] + input[1] * 10 + input[0] * 100;
  else    kombinacije[count_komb] = input[0] * 1000 + input[1] * 100 + input[2] * 10 + input[3];
}

void hardtest(){
  number = 0;
  dig[0] = dig[1] = dig[2] = dig[3] = 1;
  while(millis() - Time < 2000){
    display_num();
  }
  number = 4;
  Time = millis();
}

void setup() {
  Serial.begin(9600);

  pinMode(DIG_1, OUTPUT);
  pinMode(DIG_2, OUTPUT);
  pinMode(DIG_3, OUTPUT);
  pinMode(DIG_4, OUTPUT);

  Time = millis();
  hardtest();
  randomSeed(analogRead(5));
  count = random(6);
}

void loop() {
  display_num();

  if (start) {
    if (mult * WAIT_TIME < MAX_TIME) {
      if (millis() - Time > WAIT_TIME) {
        if (!done) {
          mult++;
          randomSeed(analogRead(count));
          count += pl;
          if (count > 5 || count < 0) pl *= -1;
          dig[number] = random(MAX_NUM);
          Time = millis();
        }
      }
    }
    else {
      mult = 0;
      dig[number] = random(MAX_NUM);
      if (number > 0) number--;
      else done = true;
    }
    if (done) {
      kombinacija = dig[3] * 1000 + dig[2] * 100  + dig[1] * 10  + dig[0];
      for (i = 0; i < count_komb; i++) {
        if (kombinacije[i] == kombinacija)  dobitnici++;
      }
      if (dobitnici > 0) {
        Serial.print("Ima ");
        Serial.print(dobitnici);
        Serial.println("srecnih dobitnika");
      }
      else {
        Serial.print("Na zalost ovaj put nema srecnih dobitnika. Vise srece drugi put");
      }
      Serial.print("Dobitna kombinacija: ");
      Serial.println(kombinacija);
      finish = true;
      start = false;
    }
  }
  else if (!finish) {
    while (Serial.available()) {
      char ch = Serial.read();
      if (ch == '\n')
      {
        izracunaj();
        Serial.print(count_komb + 1);
        Serial.print(". ");
        Serial.println(kombinacije[count_komb]);
        i = 0;
        count_komb++;
        if (count_komb == 20) {
          number--;
          start = true;
        }
        for (int k = 0; k < 4; k++) {
          input[k] = 0;
        }
      }
      else
      {
        if (ch == 's' && count_komb > 1) {
          number--;
          start = true;
        }
        else {
          input[i] = ch - '0';
          i++;
        }
      }
    }
  }
}
