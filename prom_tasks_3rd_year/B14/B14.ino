#include <Bounce2.h>
#include <Servo.h>

#define START 8
#define RESET 2
#define LEVO 3
#define NAPRED 4
#define DESNO 5

#define SERVO_L 180
#define SERVO_N 90
#define SERVO_D 0

#define TIME 3000

Servo servo;
Bounce B_START = Bounce();
Bounce B_RESET = Bounce();
Bounce B_LEVO = Bounce();
Bounce B_NAPRED = Bounce();
Bounce B_DESNO = Bounce();

short gps[10] = {NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL};

bool reset = true;
bool start = false;

short count = 0;
short station = 0;

unsigned long Time;

void updateBTN(){
  B_START.update();
  B_RESET.update();
  B_LEVO.update();
  B_NAPRED.update();
  B_DESNO.update();
}

void readBTN(){
  if(B_START.fell() && count >= 2){
    start = true;
    reset = false;
  }
  if(B_RESET.fell()){
    reset = true;
    start = false;
    count = 0;
    for(int i = 0; i < 10; i++){
      gps[i] = NULL;
    }
  }
  if(B_LEVO.fell()){
    gps[count] = SERVO_L;
    count++;
  }
  if(B_NAPRED.fell()){
    gps[count] = SERVO_N;
    count++;
  }
  if(B_DESNO.fell()){
    gps[count] = SERVO_D;
    count++;
  }
}

void reset_gps(){
  count = 0;
  station = 0;
  Time = millis();
  while(reset && !start){
    updateBTN();
    readBTN();
    if(count == 10) start = true;
  }
  if(start) reset = false;
}

void checkSR(){
  if(B_START.fell() && count >= 2){
    station = 0;
    start = true;
    reset = false;
  }
  if(B_RESET.fell()){
    reset = true;
    start = false;    
  }
}

void drive_gps(){
  while (station < count && !reset){
    updateBTN();
    checkSR();
    if(millis() - Time > TIME && !reset){
      servo.write(gps[station]);
      station++;
      Time = millis();
    }
  }
  start = false;
}

void hard(){
  bool go = false;
  while(!go){
    updateBTN();
    if(B_START.fell() || B_RESET.fell()  || B_LEVO.fell() || B_NAPRED.fell() || B_DESNO.fell()){
      go = true; 
    }
  }
  servo.write(SERVO_N);
}

void setup() {
  pinMode(START, INPUT_PULLUP);
  pinMode(RESET, INPUT_PULLUP);
  pinMode(LEVO, INPUT_PULLUP);
  pinMode(NAPRED, INPUT_PULLUP);
  pinMode(DESNO, INPUT_PULLUP);

  B_START.attach(START);
  B_START.interval(10);
  
  B_RESET.attach(RESET);
  B_RESET.interval(10);

  B_LEVO.attach(LEVO);
  B_LEVO.interval(10);

  B_NAPRED.attach(NAPRED);
  B_NAPRED.interval(10);

  B_DESNO.attach(DESNO);
  B_DESNO.interval(10);
  
  servo.attach(7);

  servo.write(0);
  hard();
  delay(1000);
}

void loop() {
  updateBTN();
  checkSR();
  if(reset) reset_gps();
  if(start) drive_gps();
}
