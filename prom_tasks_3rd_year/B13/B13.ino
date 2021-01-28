#define NAPRED 2
#define NAZAD 3
#define LEVO 4
#define DESNO 5
#define LED 7
#define POT 9
#define BUZZ 10

#define MOTOR_A_EN 5
#define MOTOR_B_EN 6

#define MOTOR_A_IN_1 8
#define MOTOR_A_IN_2 11
#define MOTOR_B_IN_1 12
#define MOTOR_B_IN_2 13

#define PERIOD 0,5

bool napred = false;
bool nazad = false;
bool levo = false;
bool desno = false;

unsigned long Time;
bool buzz = true;

void updateBTN() {
  if (digitalRead(NAPRED) == HIGH) napred = true;
  if (digitalRead(NAZAD) == HIGH) nazad = true;
  if (digitalRead(LEVO) == HIGH) levo = true;
  if (digitalRead(DESNO) == HIGH) desno = true;
}

void set_BTN_Direction() {
  if (napred && nazad) {
    napred = nazad = false;
  }
  if (levo && desno) {
    levo = desno = false;
  }
}

void set_motor(bool a1, bool a2, bool b1, bool b2) {
  digitalWrite(MOTOR_A_IN_1, a1 ? HIGH : LOW);
  digitalWrite(MOTOR_A_IN_2, a2 ? HIGH : LOW);
  digitalWrite(MOTOR_B_IN_1, b1 ? HIGH : LOW);
  digitalWrite(MOTOR_B_IN_2, b2 ? HIGH : LOW);
}

void set_Motor_Direction() {
  if (levo || desno) {
    set_motor(1, 0, 1, 0);
  }
  if (napred) {
    set_motor(1, 0, 1, 0);
  }
  if (nazad) {
    set_motor(0, 1, 0, 1);
    if(millis() - Time > PERIOD * 1000 / 2){
      if(buzz){
        tone(BUZZ, 450);
        Time = millis();
      }
      else{
        noTone(BUZZ);
        Time = millis();
      }
      buzz = !buzz;
    }
  }
}

int get_Speed() {
  int temp = analogRead(POT);
  return temp = map(temp, 0, 1024, 0, 175);
}

void set_Motor_Speed() {
  analogWrite(MOTOR_A_EN, get_Speed());
  analogWrite(MOTOR_B_EN, get_Speed());
  if(levo) analogWrite(MOTOR_B_EN, 255);
  if(desno) analogWrite(MOTOR_A_EN, 255);
}

void set_BUZZ() {
  
}

void set_Effects() {
  if(napred) digitalWrite(LED, HIGH);
  else digitalWrite(LED, LOW);

  if(nazad) set_BUZZ();
}

void setup() {
  Serial.begin(9600);

  pinMode(NAPRED, INPUT_PULLUP);
  pinMode(NAZAD, INPUT_PULLUP);
  pinMode(LEVO, INPUT_PULLUP);
  pinMode(DESNO, INPUT_PULLUP);

  pinMode(LED, OUTPUT);
  pinMode(BUZZ, OUTPUT);

  pinMode(MOTOR_A_EN, OUTPUT);
  pinMode(MOTOR_B_EN, OUTPUT);

  pinMode(MOTOR_A_IN_1, OUTPUT);
  pinMode(MOTOR_A_IN_2, OUTPUT);
  pinMode(MOTOR_B_IN_1, OUTPUT);
  pinMode(MOTOR_B_IN_2, OUTPUT);

  Time = millis();
}

void loop() {
  updateBTN();
  set_BTN_Direction();
  set_Motor_Direction();
  set_Motor_Speed();
  set_Effects();
}
