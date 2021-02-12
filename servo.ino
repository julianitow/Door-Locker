#include <Servo.h>
#include <Keypad.h>
#include "rgb_lcd.h"

Servo servo;

int currentAngle = 0;
const int lockAngle = 180;
const int unlockAngle = 90;

rgb_lcd lcd;

int colorR = 255;
int colorG = 020;
int colorB = 147;


int axeX = A0; // signal de l'axe X sur entrée A0
int axeY = A1; // signal de l'axe Y sur entrée A1

const byte ROWS = 4; //four rows
const byte COLS = 4; //three columns
char keys[ROWS][COLS] = {
  {'1', '2', '3', '4'},
  {'5', '6', '7', '8'},
  {'9', 'a', 'b', 'c'},
  {'d', 'e', 'f', 'g'}
};
byte rowPins[ROWS] = {9, 8, 7, 6}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {5, 4, 3, 2}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

String password = "222";
String inputPassword = "";

void setup() {
  Serial.begin(9600);
  servo.attach(10);
  servo.write(0);

  lcd.begin(16, 2);
  lcd.setRGB(colorR, colorG, colorB);
  lcd.print("Enter passcode:");
}

void loop() {
  Serial.println("LOOP BEGIN");
  //currentAngle = servo.read();
  int joystickValue = readJoystick();
  switch (joystickValue) {
    case 1 :
      Serial.println("Joystick à GAUCHE");
      lcd.print("Joystick à GAUCHE");
      Serial.println("pass");
      break;
    case 2 :
      lcd.print("Joystick à DROITE");
      unlock();
      break;
    case 3 :
      lcd.print("Joystick en HAUT");
      lock();
      break;
    case 4 :
      lcd.print("Joystick en BAS");
      break;
    default :
      break;
  }
  char key = keypad.getKey();
  if (key) {
    Serial.println(key);
    if (key == '1') {
      inputPassword = "";
      lock();
    } else {
      inputPassword.concat(key);
    }
  }
  if (password.equals(inputPassword)) {
    unlock();
    inputPassword = "";
    //inputPassword = "ACCESS GRANTED !";
  } else {
    lcd.setCursor(0, 1);
    lcd.print(inputPassword);
  }
  delay(500);
  Serial.println("LOOP END");
}

int readJoystick() {
  float X, Y;
  X = analogRead (axeX) * (5.0 / 1023.0);
  Y = analogRead (axeY) * (5.0 / 1023.0);

  if (X == 0) {
    return 1; // Joystick à gauche
  } else if (X == 5) {
    return 2; // Joystick à droite
  } else if (Y == 0) {
    return 3; // Joystick en haut
  } else if (Y == 5) {
    return 4; // Joystick en bas
  }

  return 0;
}

void readKp4x4() { /* function readKp4x4 */
  //// Read button states from keypad

  /*  if (customKey) {
      if(customKey == '7'){
        Serial.println(customKey);
        lcd.setCursor(0, 0);
        lcd.print(customKey);
      }
    }
    delay(500);*/
}

void lock() {
  if (currentAngle != lockAngle) {
    servo.write(0);
    delay(500);
  }
  servo.write(lockAngle);
  lcd.setCursor(0, 0);
  lcd.clear();
  lcd.setRGB(255, 0, 0);
  lcd.print("LOCKED !");
  lcd.setCursor(0, 1);
  lcd.print("Passcode ?");
  Serial.println("LOCKED !");
}

void unlock() {
  if (currentAngle != unlockAngle) {
    servo.write(0);
    delay(500);
  }
  servo.write(unlockAngle);
  lcd.setCursor(0, 0);
  lcd.setRGB(0, 255, 0);
  lcd.clear();
  lcd.print("ACCESS GRANTED !");
  Serial.println("ACCESS GRANTED !");
}
