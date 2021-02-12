#include <Servo.h>
#include <Keypad.h>
#include <LiquidCrystal.h>
#include "rgb_lcd.h"

// LCD pins <--> Arduino pins
const int RS = 11, EN = 12, D4 = 2, D5 = 3, D6 = 4, D7 = 5;
LiquidCrystal lcd(RS, EN, D4, D5, D6, D7);

rgb_lcd lcdrgb;
int colorR = 255;
int colorG = 020;
int colorB = 147;

Servo servo;

int currentAngle = 0;
const int lockAngle = 180;
const int unlockAngle = 90;

int axeX = A0; // signal de l'axe X sur entrée A0
int axeY = A1; // signal de l'axe Y sur entrée A1

const byte ROWS = 4; //four rows
const byte COLS = 4; //three columns
char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
byte rowPins[ROWS] = {22, 24, 26, 28}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {30, 32, 34, 36}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

String password = "777";
String inputPassword = "";

void setup() {
  Serial.begin(9600);
  servo.attach(10);
  servo.write(0);

  lcd.begin(16, 2);
  lcd.print("Enter passcode:");

  lcdrgb.begin(16, 2);
  lcdrgb.setRGB(colorR, colorG, colorB);
  lcdrgb.print("NIQUE TOI");
}

void loop() {
  int joystickValue = readJoystick();
  switch (joystickValue) {
    case 1 :
      Serial.println("GAUCHE");
      lcd.setCursor(0,1);
      lcd.print("GAUCHE");
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

  readKp4x4();

  if (password.equals(inputPassword)) {
    unlock();
    inputPassword = "";
    //inputPassword = "ACCESS GRANTED !";
  } else {
    //lcd.setCursor(0, 1);
    //lcd.print(inputPassword);
  }
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
  char key = keypad.getKey();
  if (key != NO_KEY) {
    Serial.println(key);
    if (key == 'g') {
      inputPassword = "";
      lock();
    } else if (key == '8') {
      inputPassword = "";
    } else if (key != 'g') {
      inputPassword.concat(key);
    }
  }
  keypadEvent(key);
}

void lock() {
  if (currentAngle != lockAngle) {
    servo.write(0);
    delay(500);
  }
  servo.write(lockAngle);
  lcd.setCursor(0, 0);
  lcd.clear();
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
  lcd.clear();
  lcd.print("ACCESS GRANTED !");
  Serial.println("ACCESS GRANTED !");
}

void keypadEvent (KeypadEvent key)
{
  switch (keypad.getState())
  {
    case PRESSED:
    if (key == '1')
    {
      Serial.println(key);
    }  
    if (key == '2')
    {
     Serial.println(key);         
    }
    if (key == '3')
    {
      int previousState= keypad.getState();
      while(key == '3')
      {
        key = keypad.getKey();       
        Serial.println(keypad.getState());

      }
  }
  }

}
