int i = 0, k = 0, j = 0;
char string[256];
char string2[256];
bool back = false;
bool palindrom = false;

int green = 3;
int red = 2;

float freqGreen = 0.5;
float freqRed = 1;

void reset() {
  for (i; i >= 0; i--) {
    string[i] = string2[i] = NULL;
  }

  palindrom = false;
  back = false;
  i = 0;
}

void setup() {
  Serial.begin(9600);
  pinMode(green, OUTPUT);
  pinMode(red, OUTPUT);

  
  Serial.println("Palindrom");
  digitalWrite(green, HIGH);
  delay(1500);
  digitalWrite(green, LOW);
}

void loop() {
  while (Serial.available()) {
    char ch = Serial.read();
    if (ch == '\n')
    {
      back = true;
    }
    else
    {
      string[i] = ch;
      i++;
    }
  }

  if (back) {
    string[i] = '\0';

    k = 0;

    for (j = 0; j < i; j++) {
      while (string[j] == ' ') j++;
      if (string[j] < 91 && string[j] > 64) string[j] = string[j] + 32;
      string2[k] = string[j];
      k++;
    }

    int temp = k - 1;

    for (k = k - 1; k >= 0; k--) {

      palindrom = true;
      if (!(string2[k] == string2[temp - k])) {
        palindrom = false;
        break;
      }
    }

    if (palindrom) {
      Serial.println("Rec je palindrom");
      for (k = 0; k  < 5; k++) {
        digitalWrite(green, HIGH);
        delay(1 / freqGreen / 2 * 1000);
        digitalWrite(green, LOW);
        delay(1 / freqGreen / 2 * 1000);
      }
    }
    else {
      Serial.println("Rec nije palindrom");
      for (k = 0; k  < 5; k++) {
        digitalWrite(red, HIGH);
        delay(1 / freqRed / 2 * 1000);
        digitalWrite(red, LOW);
        delay(1 / freqRed / 2 * 1000);
      }
    }

    reset();
  }
}
