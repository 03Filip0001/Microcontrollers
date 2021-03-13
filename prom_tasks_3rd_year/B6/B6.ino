#include <Bounce2.h>
#include <String.h>
#define MAX_PER 100
#define TIME 500

String command;

int led = 11;
int per = 0;
int PER;

int DECR = 2;
int INCR = 3;

bool manual = false;
int state = 1;

unsigned long time;

Bounce dec = Bounce();
Bounce inc = Bounce();

void setup() {
  Serial.begin(9600);
  
  pinMode(led, OUTPUT);
  pinMode(DECR, INPUT_PULLUP);
  pinMode(INCR, INPUT_PULLUP);

  dec.attach(DECR);
  inc.attach(INCR);
  dec.interval(15);
  inc.interval(15);

  time = millis();
}

void loop() {

  if(Serial.available()){
  command = Serial.readString();
  command.trim();
  command.toUpperCase();


  if(command.equals("MANUAL")){
    manual = true;
  }
  if(command.equals("AUTO")){
    manual = false;
  }
  }
  
  if(!manual){
    if(millis() - time > TIME)
    {
      if(per <= 100 && per >= 0)
        per += 10 * state;
      if(per == 100 || per == 0) state *= -1;

      time = millis();
    }
  }else{
    inc.update();
    dec.update();

    if(inc.fell() && per < 100) per += 10;
    if(dec.fell() && per > 0) per -= 10;
  }

  PER = map(per, 0, 100, 0, 255);
  Serial.println(per);
  Serial.println(PER);
  analogWrite(led, PER);
}
