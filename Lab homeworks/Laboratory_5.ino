#include <LiquidCrystal.h>
const int RS = 12;
const int enable = 11;
const int d4 = 5;
const int d5 = 4;
const int d6 = 3;
const int d7 = 2;

const int pinSW = 10;
const int pinX = A0;
const int pinY = A1;

int swState = HIGH;
int lastSwState = HIGH;
int xValue = 0;
int yValue = 0;
bool joyMovedX = false;
bool joyMovedY = false;

int minThreshold = 400;
int maxThreshold = 600;
int digit = 0;
int startingLevelValue = 0;
int valMenu = 0;
int switchValue = 0;
int sec = 2;
int score = 0;
unsigned long lastPrint = 0;
unsigned long lastTime;
int nr = 0;
int p=1;


LiquidCrystal lcd(RS, enable, d4, d5, d6, d7);

void displayArrow(byte digit) {
  if (digit == 0) {
    lcd.setCursor(7,1);
    lcd.print(" ");
    lcd.setCursor(0,1);
    lcd.print(" ");
    lcd.setCursor(2,0);
    lcd.print(">");
  }

  if (digit == 1) {
    lcd.setCursor(2,0);
    lcd.print(" ");
    lcd.setCursor(7,1);
    lcd.print(" ");
    lcd.setCursor(0,1);
    lcd.print(">");
  }
 
 if (digit == 2) {
  lcd.setCursor(0,1);
  lcd.print(" ");
  lcd.setCursor(2,0);
  lcd.print(" ");
  lcd.setCursor(7,1);
  lcd.print(">");
 }
}
void mainMenu() {
  lcd.setCursor(3, 0);
  lcd.print("Start Game:");
  lcd.setCursor(1, 1);
  lcd.print("HighS");
  lcd.setCursor(8, 1);
  lcd.print("Settings");
  
  
}


void  displaySubMenu(byte digit) {
  if (digit == 0) {
      if (millis() - lastPrint >600) {
         lcd.clear();
      }
      lcd.setCursor(0, 0);
      lcd.print("Lives:3");
      lcd.setCursor(8, 0);
      lcd.print("Level");
      lcd.setCursor(14, 0);
      lcd.print(startingLevelValue);
      
      if ((millis()-lastTime)/1000 == sec) {
         startingLevelValue = startingLevelValue+1;
         lcd.setCursor(14, 0);
         lcd.print(startingLevelValue);
         sec=sec+2;
        }
      lcd.setCursor(2,1);
      lcd.print("Score:");
      lcd.setCursor(10,1);
      score=3*startingLevelValue;
      lcd.print(score);
      if ((millis() - lastTime)/1000 == 10) {
         lcd.clear();
         lcd.setCursor(0, 0);
         lcd.print("Congratulations!");
         lcd.setCursor(0, 1);
         lcd.print("Press but to exit");
         startingLevelValue = 0;
         valMenu =3;
        } 
        lastPrint = millis();
      }   
   if (digit == 1) {
     if (millis() - lastPrint >600) {
         lcd.clear();
      }
     lcd.setCursor(1,0);
     lcd.print("HighScore:");
     lcd.setCursor(11,0);
     lcd.print(score);
     
     if (swState != lastSwState) {
      if(swState == LOW) {
         valMenu = 0;
         nr=0;
        }
     }
    lastSwState = swState;
    lastPrint = millis();
   }
  if (digit == 2) {
     yValue = analogRead(pinY);
     if (millis() - lastPrint >600) {
         lcd.clear();
      }
     lcd.setCursor(1,0);
     lcd.print("StartingLevel:");
     lcd.setCursor(1,1);
     if (yValue < minThreshold && joyMovedY== false) {
      if (startingLevelValue<94) {
          startingLevelValue++;
      }
      joyMovedY = true;
    
     }
     if (yValue > maxThreshold && joyMovedY == false) {
      if(startingLevelValue != 0)
     startingLevelValue--;
     joyMovedY = true;
     }

    if (yValue >= minThreshold && yValue <=maxThreshold) {
    joyMovedY = false;
    }
    lcd.print(startingLevelValue);
    if (startingLevelValue < 10) {
      lcd.setCursor(2,1);
      lcd.print(" ");  
    }
    
    if (swState != lastSwState) {
      if(swState == LOW) {
         valMenu = 0;
         nr=0;
        }
     }
    lastSwState = swState;
    }
    lastPrint = millis();
  }
      




void setup() {
  pinMode(pinSW,INPUT_PULLUP);
  lcd.begin(16, 2);
  Serial.begin(9600);

}

void loop() {
  swState = digitalRead(pinSW);
  xValue = analogRead(pinX);
  if (valMenu == 0) {
     if (xValue < minThreshold && joyMovedX == false) {
        if (digit < 2) {
            digit++;
        } 
        else {
           digit = 0;
        }
        joyMovedX = true;
    
     }
     if (xValue > maxThreshold && joyMovedX == false) {
        if(digit > 0) {
           digit--;
        } 
        else {
          digit = 2;
        }
        joyMovedX = true;
      }

     if (xValue >= minThreshold && xValue <=maxThreshold) {
         joyMovedX = false;
      }
   
     if (swState != lastSwState) {
        if (swState == LOW) { 
          valMenu =1;
          lastTime = millis();
          sec=2;
          Serial.println(lastTime);
        }
      }
    lastSwState = swState;
    if (nr == 0) {
      nr++;
      lcd.clear();
      }
    mainMenu();
    displayArrow(digit);
   // lastPrint = millis();
  }
  if (valMenu == 1){
    displaySubMenu(digit);
  }

  if (valMenu == 3){
    
    if (swState != lastSwState) {
      if(swState == LOW) {
         valMenu = 0;
         nr=0;
        }
     }
    lastSwState = swState;
  }
} 
  
