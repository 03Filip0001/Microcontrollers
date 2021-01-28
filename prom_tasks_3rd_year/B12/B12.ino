#define MOTOR_EN 9
#define MOTOR_IN_1 5
#define MOTOR_IN_2 6
#define CLOCK 3
#define DATA 2
#define TURN 20
#define CONST 2

#define POT 10

int counter = 0;
int currentState;
int previousState;

int TURNS = 0;
bool check = false;
bool start = false;
bool turns = false;
bool msg = false;
int pc = 0;

char komanda[6];
int i = -1;

bool reseti = false;
bool resetProgram = false;

void message(String msg) {
  Serial.print(msg);
}

void message(String msg, bool row) {
  Serial.print(msg);
  if (row) Serial.println();
}

void message(int msg, bool row) {
  Serial.print(msg);
  if (row) Serial.println();
}

char checkSerial(bool increase) {
  char ch;
  while (Serial.available()) {
    ch = Serial.read();
    if (ch == '\n')
    {
      check = true;
      reseti = true;
    }
    else
    {
      if (increase) i++;
      return ch;
    }
  }
  return 0;
}

void checkKomandu(bool change) {
  if (!msg) {
    message("Komanda: ");
    msg = true;
    check = false;
  }
  if (msg && !check) {
    int temp = checkSerial(1);
    if (temp != false) komanda[i] = temp;
  }
  if (check) {
    message(komanda, 1);
    if (komanda[2] > 96 ? (komanda[2] == 'a') :
        komanda[2] + 32 == 'a') start = true;
    else {
      start = false;
      resetProgram = true;
    }
    if (change) {
      pc++;
    }
    i = -1;
    msg = false;
    resetKomandu();
  }
}

void resetKomandu() {
  for(int j = 0; j < 6;j++){
    komanda[j] = NULL;
  }
}

void ugasiMotor() {
  digitalWrite(MOTOR_EN, LOW);
  if (resetProgram) {
    TURNS = 0;
    check = false;
    counter = 0;
    pc = 0;
    start = false;
    msg = false;
  }
  resetProgram = false;
}

void setup() {
  pinMode(DATA, INPUT);
  pinMode(CLOCK, INPUT);

  pinMode(MOTOR_EN, OUTPUT);
  pinMode(MOTOR_IN_1, OUTPUT);
  pinMode(MOTOR_IN_2, OUTPUT);

  pinMode(POT, INPUT);
  Serial.begin(9600);

  digitalWrite(MOTOR_EN, 127);
  delay(3000);
  ugasiMotor();

  previousState = digitalRead(CLOCK);
}

void loop() {
  if (start) {
    checkKomandu(false);
    if (reseti) {
      reseti = false;
      i = -1;
    }
    currentState = digitalRead(CLOCK);

    int Speed = analogRead(POT);
    Speed = map(Speed, 0, 1024,0,255);
    message(Speed,1);
    digitalWrite(MOTOR_EN, Speed);

    if (currentState != previousState) {
      if (digitalRead(DATA) == currentState) {
        //clockwise
        counter++;
      }/*else{
      counter--;
    }*/
      //Serial.println(komanda[3]);
    }

    if (counter < TURNS * TURN * CONST)
    {
      digitalWrite(MOTOR_IN_1, HIGH);
      digitalWrite(MOTOR_IN_2, LOW);
    }
    else {
      resetProgram = true;
      ugasiMotor();
      message("", 1);
      message("Namotavanje zice je uspesno zavrseno", 1);
    }

    previousState = currentState;
  }
  else {
    ugasiMotor();
    switch (pc) {
      case 0:
        if (!msg) {
          message("Unesite broj namotaja: ");
          msg = true;
          check = false;
        }
        if (msg && !check) {
          int temp = checkSerial(0);
          if (temp != false) TURNS = TURNS * 10 + (temp - '0');
        }
        if (check) {
          message(TURNS, 1);
          msg = false;
          if (TURNS < 0) {
            TURNS = 0;
            check = false;
            Serial.println("ERROR ! Unesi broj veci od 0 !");
          }
          else  pc++;
        }
        break;
      case 1:
        checkKomandu(false);
        break;
      default:
        message("ERROR ! switch");
    }
  }
}
