#include "LedControl.h"

#define LEVO 2
#define DOLE 3
#define GORE 4
#define DESNO 5
#define SS 6
#define CLK 13
#define MOSI 11

#define GS 250

LedControl matrix = LedControl(MOSI, CLK, SS, 1);

byte dots[8][8] = {
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0}
};

short snake[7][2] = {
  {0, 0},
  {0, 0},
  {0, 0},
  {0, 0},
  {0, 0},
  {0, 0},
  {0, 0}
};

bool start = false;
bool restart = false;
bool endGame = false;
bool pressed = false;
bool levo, dole, gore, desno;
bool *p[4] = {&levo, &dole, &gore, &desno};

unsigned long Time;
short rowSnake, colomSnake;
byte rowFood, colomFood;

bool makeFood = true;
byte count = 0;
byte leng = 1;

void draw() {
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      matrix.setLed(0, j, i, dots[i][j]);
    }
  }
}

void newDirection(int way) {
  for (int k = 0; k < 4; k++) {
    if (k != way) *(p[k]) = false;
  }
}

void newPos() {

  updateTail();

  if (levo)  {
    if (colomSnake - 1 < 0) {
      if (dots[7][rowSnake] == 2) {
        leng++;
      }
      else if (dots[7][rowSnake] == 1) {
        endGame = true;
      }
    }
    else {
      if (dots[colomSnake - 1][rowSnake] == 2) {
        leng++;
      }
      else if (dots[colomSnake - 1][rowSnake] == 1) {
        endGame = true;
      }
    }
    dots[colomSnake][rowSnake] = 0;
    colomSnake--;
  }
  if (dole)  {
    if (rowSnake + 1 > 7) {
      if (dots[colomSnake][0] == 2) {
        leng++;
      }
      else if (dots[colomSnake][0] == 1) {
        endGame = true;
      }
    }
    else {
      if (dots[colomSnake][rowSnake + 1] == 2) {
        leng++;
      }
      else if (dots[colomSnake][rowSnake + 1] == 1) {
        endGame = true;
      }
    }
    dots[colomSnake][rowSnake] = 0;
    rowSnake++;
  }
  if (gore)  {
    if (rowSnake - 1 < 0) {
      if (dots[colomSnake][7] == 2) {
        leng++;
      }
      else if (dots[colomSnake][7] == 1) {
        endGame = true;
      }
    }
    else {
      if (dots[colomSnake][rowSnake - 1] == 2) {
        leng++;
      }
      else if (dots[colomSnake][rowSnake - 1] == 1) {
        endGame = true;
      }
    }
    dots[colomSnake][rowSnake] = 0;
    rowSnake--;
  }
  if (desno) {
    if (colomSnake + 1 > 7) {
      if (dots[0][rowSnake] == 2) {
        leng++;
      }
      else if (dots[0][rowSnake] == 1) {
        endGame = true;
      }
    }
    else {
      if (dots[colomSnake + 1][rowSnake] == 2) {
        leng++;
      }
      else if (dots[colomSnake + 1][rowSnake] == 1) {
        endGame = true;
      }
    }
    dots[colomSnake][rowSnake] = 0;
    colomSnake++;
  }

  if (leng == 6 || endGame) {
    restart = true;
  }

  if (!restart) {
    correctPos();

    snake[0][0] = colomSnake;
    snake[0][1] = rowSnake;
    updateField();
  }
}

void correctPos() {
  if (rowSnake  < 0)  rowSnake = 7;
  if (rowSnake > 7)   rowSnake = 0;
  if (colomSnake < 0) colomSnake = 7;
  if (colomSnake > 7) colomSnake = 0;
}

void updateTail() {
  switch (leng) {
    case 6:
      snake[6][0] = snake[5][0];
      snake[6][1] = snake[5][1];
    case 5:
      snake[5][0] = snake[4][0];
      snake[5][1] = snake[4][1];
    case 4:
      snake[4][0] = snake[3][0];
      snake[4][1] = snake[3][1];
    case 3:
      snake[3][0] = snake[2][0];
      snake[3][1] = snake[2][1];
    case 2:
      snake[2][0] = snake[1][0];
      snake[2][1] = snake[1][1];
    case 1:
      snake[1][0] = snake[0][0];
      snake[1][1] = snake[0][1];
    case 0:
      break;
  }
}

void updateField() {
  // leng = 2
  //3 puta
  for (int i = 0; i <= leng; i++) {
    if (i == leng && start) dots[snake[i][0]][snake[i][1]] = 0;
    else                       dots[snake[i][0]][snake[i][1]] = 1;
  }
}

void newFood() {
  do {
    rowFood = random(8);
    colomFood = random(8);
    if (!dots[colomFood][rowFood]) {
      dots[colomFood][rowFood] = 2;
      count++;
    }
  } while (count < 5);
  makeFood = false;
}

void restartGame(){
  levo = dole = gore = desno = false;
  rowSnake = colomSnake = 0;
  start = false;
  pressed = false;
  makeFood = true;
  endGame = false;
  leng = 1;
  count = 0;

  for(int i = 0; i < 7;i++){
    snake[i][0] = snake[i][1] = 0;
  }

  for(int i = 0; i < 8; i++){
    for(int j = 0; j < 8; j++){
      dots[i][j] = 0;
    }
  }
  
  restart = false;
}

void matrixPower(){
  matrix.shutdown(0, false);
  matrix.setIntensity(0, 5);
  matrix.clearDisplay(0);
}

void checkHard(){
  matrixPower();

  while(digitalRead(LEVO) == HIGH && digitalRead(DOLE) == HIGH && digitalRead(GORE) == HIGH && digitalRead(DESNO) == HIGH){
  }
  matrix.setLed(0, random(8), random(8), 1);
}

void setup() {
  randomSeed(analogRead(0));

  pinMode(LEVO, INPUT_PULLUP);
  pinMode(DOLE, INPUT_PULLUP);
  pinMode(GORE, INPUT_PULLUP);
  pinMode(DESNO, INPUT_PULLUP);

  checkHard();
  delay(1500);

  matrixPower();

  restartGame();
  draw();
  
  Time = millis();
}

void loop() {
  if (digitalRead(LEVO) == LOW && (!pressed || levo) && !desno) {
    start = true;
    levo = true;
    pressed = true;
    newDirection(0);
  }

  if (digitalRead(DOLE) == LOW && (!pressed || dole) && !gore) {
    start = true;
    dole = true;
    pressed = true;
    newDirection(1);
  }

  if (digitalRead(GORE) == LOW && (!pressed || gore) && !dole) {
    start = true;
    gore = true;
    pressed = true;
    newDirection(2);
  }

  if (digitalRead(DESNO) == LOW && (!pressed || desno) && !levo) {
    start = true;
    desno = true;
    pressed = true;
    newDirection(3);
  }

  if (digitalRead(LEVO) && digitalRead(DOLE) && digitalRead(GORE) && digitalRead(DESNO)) pressed = false;

  if (millis() - Time > GS) {
    newPos();
    if (start && makeFood) {
      newFood();
    }
    if(restart) restartGame();
    draw();
    Time = millis();
  }
}
