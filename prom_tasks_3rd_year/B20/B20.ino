#include <EEPROM.h>

#define ADDR_COUNT 500
#define MAX_COUNT  100
#define LENGHT     5

byte COUNT = 0;

int i = 0;
bool check = false;
bool read = false;
bool save = false;

char input[10];
byte local[5];

void restart(){
  save = false;
  check = false;
  read = false;
  for(int k = 0; k < 10; k++){
    if(k < 5) local[k] = NULL;
    input[k] = NULL;
  }
  i = 0;
}

void setup() {
  Serial.begin(9600);
  
  COUNT = EEPROM.read(ADDR_COUNT);
  //COUNT = 0;
  Serial.println("Telefonski imenik\n");
}

void loop() {
  while (Serial.available()) {
    char ch = Serial.read();
    if (ch == '\n')
    {
      check = true;
    }
    else
    {
      input[i] = ch;
      i++;
    }
  }

  if(check  && !save && !read){
    if(input[1] > 96 ? (input[0] == 'u' || input[1] == 'p') : 
                       (input[0] + 32 == 'u' || input[1] + 32 == 'p')) {
      Serial.print("Unesite broj: ");
      save = true;
    }
    else if(input[1] > 96 ? (input[1] == 'i' || input[1] == 's') : 
                            (input[1] + 32 == 'i' || input[1] + 32 == 's')) {
      COUNT > 0 ? Serial.println("\nEvo ti brojevi") : Serial.println("\nNema sacuvanih brojeva!");
      read = true;
    }
    else  Serial.println("ERROR! Pogresan unos!");
    check = false;
    i = 0;
  }

  if(save && check){
    Serial.print(input);
    for(int j = 0; j < LENGHT; j++){
      local[j] = ((input[2*j] - '0') * 10) + (input[2*j + 1] - '0');
      EEPROM.write((COUNT * LENGHT) + j, local[j]);
    }
    COUNT++;
    EEPROM.write(ADDR_COUNT, COUNT);
    
    restart();
  }

  if(read){
    int j = 0;
    while(j < COUNT){
      char string[10] = {j + 1 + '0', '.', ' ', 'b', 'r', 'o', 'j', ':', ' '};
      Serial.print(string);
      for(int k = 0; k < LENGHT; k++){
        byte load = EEPROM.read((j * LENGHT) + k);
        if(load < 10) Serial.print('0');
        Serial.print(load);
      }
      Serial.println();
      j++;
    }
    restart();
  }
}
