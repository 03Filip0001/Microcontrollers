#include <Bounce2.h>
#include <SegmentDisplay_CD4511B.h>

#define A 11
#define B 10
#define C 9
#define D 8

#define red 12
#define green 13

#define btn1 2
#define btn2 3
#define btn3 4

byte slots = 9;
int wait = 500;
unsigned int Time;
byte check = 0;

SegmentDisplay_CD4511B dis(A , B, C, D, 1, 5);
Bounce petlja1 = Bounce();
Bounce petlja2 = Bounce();
Bounce ulazak = Bounce();

bool step1 = false;
bool step2 = false;
bool liftUp = false;
bool go = false;

bool h1 = false;
bool h2 = false;
bool h3 = false;

void reset() {
  step1 = step2 = liftUp = false;
  check = 0;
  go = true;

  digitalWrite(green, LOW);
  digitalWrite(red, HIGH);
}

void ulaz() {
  digitalWrite(green, HIGH);
  digitalWrite(red, LOW);
}

void hard_test() {
  while (check < 4) {
    read_btn();

    if (step1) {
      digitalWrite(red, HIGH);
      if (!h1) check++;
      h1 = true;
    }
    else digitalWrite(red, LOW);
    if (step2) {
      digitalWrite(green, HIGH);
      if (!h2) check++;
      h2 = true;
    }
    else digitalWrite(green, LOW);
    if (liftUp) {
      dis.showNumber(5);
      if (!h3) check++;
      h3 = true;
    }
    else dis.showNumber(0);

    //go = !step1 || !step2 || !liftUp;
    if (!step1 && !step2 && !liftUp && check == 3) check++;
  }
}

void read_btn() {
  ulazak.update();
  petlja1.update();
  petlja2.update();

  step1 = !petlja1.read();
  step2 = !petlja2.read();
  liftUp = !ulazak.read();
}

void setup() {
  Serial.begin(9600);

  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);

  pinMode(btn1, INPUT_PULLUP);
  pinMode(btn2, INPUT_PULLUP);
  pinMode(btn3, INPUT_PULLUP);

  petlja1.attach(btn3);
  petlja1.interval(25);

  petlja2.attach(btn1);
  petlja2.interval(25);

  ulazak.attach(btn2);
  ulazak.interval(25);

  hard_test();

  delay(1500);
  reset();
}

void loop() {
  read_btn();
  Serial.println(check);

  if (!liftUp) {
    go = true;
  }

  if (step1 && !step2 && !liftUp && check == 0) check++; // 1
  if (step1 && !step2 && liftUp && check == 1)  {
    if (slots > 0) {
      ulaz();
      go = false;
      check++; // 2
    }
  }
  if (step1 && step2 && !liftUp && check == 2)  check++; // 3
  if (!step1 && step2 && !liftUp && check == 3) check++; // 4
  if (!step1 && !step2 && !liftUp && check == 4) {      // 5
    reset();
    slots--;
  }

  if (((step1 && step2) || !step1) && check == 1) reset();
  if ((!step1 || liftUp) && check == 2 && go) reset();
  if ((liftUp || !step2) && check == 3) reset();
  if ((step1 || liftUp) && check == 4) reset();

  dis.showNumber(slots);
}
