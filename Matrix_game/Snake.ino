#include "LedControl.h"
#include <EEPROM.h>
#include <LiquidCrystal.h>
#define SIZE    8
int avanceDelay = 50;
LedControl lc = LedControl(12, 11, 10, 1);//matrix


//joystick
const int pinSW = 9;
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
int switchValue = 0;


// lcd
const int RS = 13;
const int enable = 6;
const int d4 = 5;
const int d5 = 4;
const int d6 = 3;
const int d7 = 2;
int lcdPower = 8;
LiquidCrystal lcd(RS, enable, d4, d5, d6, d7);


// menu 
int digit = 0;
int startingLevelValue = 1;
int prevLevel = 1;
int valMenu = 0;
int sec = 2;
int score = 0;
int highScore = 0;
unsigned long lastPrint = 0;
unsigned long lastTime;
int nr = 0;
int p = 1;
int nrPrint = 0;
int setNr = 0;
int changeHigh = 0;


// snake
int snake[SIZE*SIZE][2];
int length;
int food[2], adv[2];
bool isGameOver = false;
long currentTime;
long prevAdvance;
bool isInGame = false;
int lives = 2;
int savedHighScore = 0;
bool matrix[8][8] = {
  {0, 0, 0, 0, 0, 0, 0, 0,},
  {0, 0, 1, 0, 0, 1, 0, 0,},
  {0, 0, 1, 0, 0, 1, 0, 0,},
  {0, 0, 1, 0, 0, 1, 0, 0,},
  {0, 1, 0, 0, 0, 0, 1, 0,},
  {0, 0, 1, 0, 0, 1, 0, 0,},
  {0, 0, 0, 1, 1, 0, 0, 0,},
  {0, 0, 0, 0, 0, 0, 0, 0,}
};
bool matrixSad[8][8] = {
  {0, 0, 0, 0, 0, 0, 0, 0,},
  {0, 0, 1, 0, 0, 1, 0, 0,},
  {0, 0, 1, 0, 0, 1, 0, 0,},
  {0, 0, 1, 0, 0, 1, 0, 0,},
  {0, 0, 0, 0, 0, 0, 0, 0,},
  {0, 0, 0, 1, 1, 0, 0, 0,},
  {0, 0, 1, 0, 0, 1, 0, 0,},
  {0, 0, 0, 0, 0, 0, 0, 0,}
};


// snake function
void initGame() {
  //the starting position and the first ball are initialized;
  prevAdvance = currentTime = 0;
  int half = SIZE / 2;
  length = 2;

  for (int i = 0; i < length; i++) {
      snake[i][0] = half - 1;
      snake[i][1] = half + i;
  }

  food[0] = half + 1;
  food[1] = half - 1;

  adv[0] = 0;
  adv[1] = -1;
}

void show() {
  for (int i = 0; i < length; i++){
    lc.setLed(0, snake[i][0], snake[i][1], 1);
  }
  lc.setLed(0, food[0], food[1], 1);
}

void clearScreen(){
  for (int x = 0; x < SIZE; x++){
    for (int y = 0; y < SIZE; y++){
      lc.setLed(0, x, y, 0);
    }
  }
}

// moves the snake forward,returns true if the game is over;
bool advance(){
  int head[2] = {snake[0][0] + adv[0], snake[0][1] + adv[1]};

  if (head[0] < 0 || head[0] >= SIZE){
    delay(1000);// is used to wait a second when the snake hits the walls;
    
    if (lives == 0) 
    showGameOverMessage();
    
    return true;
  }

  if (head[1] < 0 || head[1] >= SIZE){
    delay(1000); 
    
    if (lives == 0) 
    showGameOverMessage();
    
    return true;
  }

  for (int i = 0; i < length; i++){
      
      if (snake[i][0] == head[0] && snake[i][1] == head[1]){
        delay(1000);
        
        if (lives == 0) 
        showGameOverMessage();
          
        return true;
      }
  }

  bool grow = (head[0] == food[0] && head[1] == food[1]);
  if (grow){
      length++; 
      score = score + 50;
       
       if (score > highScore){
          highScore = score;
       }
       
       if(highScore > savedHighScore){
        EEPROM.put(0,highScore);
        changeHigh = 1;
       }
      
      if ( score % 100 == 0){
        startingLevelValue++;
      }
     
      randomSeed(currentTime);    
      food[0] = random(SIZE);
      food[1] = random(SIZE);
  }

  for (int i = length - 1; i >= 0; i--){
      snake[i + 1][0] = snake[i][0];
      snake[i + 1][1] = snake[i][1];
  }
  snake[0][0] += adv[0];
  snake[0][1] += adv[1];
  
  return false;
}

void readControls(){
   yValue = analogRead(pinY);
   xValue = analogRead(pinX);
   
   if (xValue < minThreshold && joyMovedX == false){
        adv[0] = 1;
        adv[1] = 0;
        joyMovedX = true;
    
     }
   
   if (xValue > maxThreshold && joyMovedX == false){
        adv[0] = -1;
        adv[1] = 0;
        joyMovedX = true;
      }
   
   if (xValue >= minThreshold && xValue <=maxThreshold){
         joyMovedX = false;
      }
   
   if (yValue < minThreshold && joyMovedY== false){
         adv[0] = 0;
         adv[1] = -1;
        joyMovedY = true;
    
     }
   
   if (yValue > maxThreshold && joyMovedY == false){
      adv[0] = 0;
      adv[1] = 1;
     joyMovedY = true;
     }

   
   if (yValue >= minThreshold && yValue <=maxThreshold){
    joyMovedY = false;
    }

}
void restart(){  
  initGame();
  isGameOver = false;
}

void restartGame(){
  restart();
  score = 0;
  lives = 2;
  
}

// menu function
void displayArrow(byte digit){
  if (digit == 0) {
    lcd.setCursor(7,0);
    lcd.print(" ");
    lcd.setCursor(0,1);
    lcd.print(" ");
    lcd.setCursor(10,1);
    lcd.print(" ");
    lcd.setCursor(1,0);
    lcd.print(">");
  }

  if (digit == 1){
    lcd.setCursor(1,0);
    lcd.print(" ");
    lcd.setCursor(0,1);
    lcd.print(" ");
    lcd.setCursor(10,1);
    lcd.print(" ");
    lcd.setCursor(7,0);
    lcd.print(">");
  }
 
 if (digit == 2){
    lcd.setCursor(1,0);
    lcd.print(" ");
    lcd.setCursor(7,0);
    lcd.print(" ");
    lcd.setCursor(10,1);
    lcd.print(" ");
    lcd.setCursor(0,1);
    lcd.print(">");
 }
  if (digit == 3){
    lcd.setCursor(1,0);
    lcd.print(" ");
    lcd.setCursor(7,0);
    lcd.print(" ");
    lcd.setCursor(0,1);
    lcd.print(" ");
    lcd.setCursor(10,1);
    lcd.print(">");
 }
}
void mainMenu(){
  lcd.setCursor(2, 0);
  lcd.print("Play");
  lcd.setCursor(8, 0);
  lcd.print("HighS");
  lcd.setCursor(1, 1);
  lcd.print("Settings");
  lcd.setCursor(11, 1);
  lcd.print("Info");
  
  
}
void finishGame(){
  if( nrPrint == 1) {
      if (millis() - lastPrint >600) {
         lcd.clear();
       }
       lcd.setCursor(0, 0);
       lcd.print("Congratulations");
       lcd.setCursor(0, 1);
       lcd.print("on finishing ");
       delay(2000);
       lcd.clear();
       lcd.setCursor(0, 0);
       lcd.print("the game");
       delay(2000);
       nrPrint = 0;
       
  }
  
}
void  displaySubMenu(byte digit){
  // the menu during the game 
  if (digit == 0){
      if (millis() - lastPrint >600){
         lcd.clear();
      }
      lcd.setCursor(0, 0);
      lcd.print("Lives:");
      lcd.setCursor(6,0);
      lcd.print(lives);
      lcd.setCursor(8, 0);
      lcd.print("Level");
      lcd.setCursor(14, 0);
      lcd.print(startingLevelValue);
      lcd.setCursor(2,1);
      lcd.print("Score:");
      lcd.setCursor(10,1);
      lcd.print(score);
      
      if(lives == 0){
         valMenu = 3;
        
       }
        isInGame = true;
        lastPrint = millis();
   }   
   // high score menu
   if (digit == 1){
     
     if (millis() - lastPrint >600){
         lcd.clear();
      }
     lcd.setCursor(1,0);
     lcd.print("HighScore:");
     lcd.setCursor(11,0);
     EEPROM.get(0, savedHighScore);
     lcd.print(savedHighScore);
     
     if (swState != lastSwState) {
      
      if(swState == LOW) {
         valMenu = 0;
         nr=0;
        }
     }
    lastSwState = swState;
    lastPrint = millis();
   }
  //settings menu; ( setNr = 0 is for starting level and setNr = 1 is for light intensity); 
  if (digit == 2) {
    
    if (millis() - lastPrint >600) {
         lcd.clear();
      }
     yValue = analogRead(pinY);
     xValue = analogRead(pinX);
     
     if (xValue < minThreshold && joyMovedX == false) {
        if (setNr < 1) {
            setNr++;
        } 
        else {
           setNr = 0;
        }
        joyMovedX = true;
    
     }
     
     if (xValue > maxThreshold && joyMovedX == false) {
        if(setNr > 0) {
           setNr--;
        } 
        else {
          setNr = 1;
        }
        joyMovedX = true;
      }

     if (xValue >= minThreshold && xValue <=maxThreshold) {
         joyMovedX = false;
      }
     
     if (setNr == 0) {
      lcd.setCursor(0,1);
      lcd.print(" ");
      lcd.setCursor(0,0);
      lcd.print(">");
     }
     
     if (setNr == 1) {
      lcd.setCursor(0,0);
      lcd.print(" ");
      lcd.setCursor(0,1);
      lcd.print(">");
      
     }
      
     lcd.setCursor(1,0);
     lcd.print("StartingLevel");
     lcd.setCursor(1,1);
     lcd.print("LightMatrix");
     
     if (yValue < minThreshold && joyMovedY== false) {
        
        if (setNr == 0) {
          if (startingLevelValue<9) {
             startingLevelValue++;
             avanceDelay = 50 - startingLevelValue*5;
             prevLevel = startingLevelValue;
           }
        }
        else {
          if(lcdPower < 15){
          lcdPower++;
          lc.setIntensity(0, lcdPower);
          }
         }
        joyMovedY = true;
    
     }
     if (yValue > maxThreshold && joyMovedY == false) {
        if (setNr == 0){
          if(startingLevelValue != 0){
             startingLevelValue--;
             avanceDelay = 50 - startingLevelValue*5;
             prevLevel = startingLevelValue;
           }
         }
        else {
           if (lcdPower > 0) {
              lcdPower--;
              lc.setIntensity(0, lcdPower);
            }
         }
        joyMovedY = true;
     }

    if (yValue >= minThreshold && yValue <=maxThreshold){
       joyMovedY = false;
    }
    
    lcd.setCursor(14,1);
    lcd.print(lcdPower);
   
    if (lcdPower < 10){
      lcd.setCursor(15,1);
      lcd.print(" ");
    }
    
    lcd.setCursor(15,0);
    lcd.print(startingLevelValue);
    
    if (swState != lastSwState){
      if(swState == LOW) {
         valMenu = 0;
         nr=0;
        }
     }
    lastSwState = swState;
    lastPrint = millis();
    }
    //info menu;
   if (digit == 3) {
      if( nrPrint == 1) {
         if (millis() - lastPrint >600){
            lcd.clear();
         }
         lcd.setCursor(1,0);
         lcd.print("Creater name :");
         lcd.setCursor(1,1);
         lcd.print("Madalina Versin");
         delay(2000);
         lcd.clear();
         lcd.setCursor(1,0);
         lcd.print("Github Link: ");
         delay(2000);
         lcd.clear();
         lcd.setCursor(1,0);
         lcd.print("github.com/");
         lcd.setCursor(1,1);
         lcd.print("MadalinaVersin ");
         delay(2000);
         lcd.clear();
         lcd.setCursor(1,0);
         lcd.print(" Game name:");
         lcd.setCursor(1,1);
         lcd.print("Snake");
         delay(2000);
         nrPrint = 0;
   }
     
   if (swState != lastSwState) {
     if(swState == LOW) {
         valMenu = 0;
         nr=0;
        }
      }
     lastSwState = swState;
     lastPrint = millis();
 }
}
void showMessage()
{
  lcd.setCursor(0,0);
  lcd.print("Congratulations");
  lcd.setCursor(0,1);
  lcd.print("score is highS") ;
  changeHigh = 0;
}
      

void setup() {
  pinMode(pinSW,INPUT_PULLUP);
  pinMode(pinX, INPUT);
  pinMode(pinY, INPUT);
  lcd.begin(16, 2);

  lc.shutdown(0, false);
  lc.setIntensity(0, lcdPower);
  Serial.begin(9600);
  EEPROM.get(0, savedHighScore);

  initGame();
  show();
}





void loop() {
  if ( prevLevel != startingLevelValue){
    prevLevel = startingLevelValue;
    avanceDelay = 50 - startingLevelValue*5;
   
  }
  
  if (!isInGame and valMenu == 0){
    for (int row =0; row < 8; row++) {
      for(int col = 0; col < 8; col++) {
        lc.setLed(0,row,col,matrix[row][col]);
      }
    }
  }
  
  if (isInGame) { 
    
    if (!isGameOver and startingLevelValue!=10) {
       clearScreen();
       show();

       if (currentTime - prevAdvance > avanceDelay) {
         isGameOver = advance();
         prevAdvance = currentTime;    
       }
     } 
  else {
    if (lives == 0) {
    
        for (int row =0; row < 8; row++) {
           for(int col = 0; col < 8; col++) {
             lc.setLed(0,row,col,matrixSad[row][col]); 
          }
        }
        if(changeHigh == 1){
           showMessage();
         }
        valMenu = 3;
        isInGame = false;  
    }
    else {
      lives --;
      restart();  
    } 
  }
  // level 10 is the end of the game;
  if (startingLevelValue == 10) {
        finishGame();
  }
  readControls();
  currentTime++;
  }
  
  
  swState = digitalRead(pinSW);
  xValue = analogRead(pinX);
  // if valMenu == 0, we are in the main menu;
  if (valMenu == 0) {
     if (xValue < minThreshold && joyMovedX == false) {
        if (digit < 3) {
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
          valMenu = 1;
          lastTime = millis();
          //Serial.println(lastTime);
        }
      }
    lastSwState = swState;
    if (nr == 0) {
      nr++;
      lcd.clear();
      }
    mainMenu();
    displayArrow(digit);
    nrPrint = 1;
  }
  // if valMenu == 1, we are in the secondary menu;
  if (valMenu == 1){
    displaySubMenu(digit);
  }
  // if valMenu == 3 ,it wait for press the button to switch from the sub menu to the main menu;
  if (valMenu == 3){
    
    if (swState != lastSwState) {
      if(swState == LOW) {
        restartGame();
         valMenu = 0;
         nr=0;
        }
     }
    lastSwState = swState;
  }
 }  




const  bool gameOverMessage[8][90] = {
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,1,1,0,0,0,1,1,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,1,1,1,1,0,0,0,1,1,0,0,1,1,0,0,1,1,1,1,1,1,0,0,1,1,1,1,1,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,1,0,1,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,0,0,0,0,1,1,0,0,1,1,0,0,1,1,1,1,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,1,1,0,0,1,1,1,1,1,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,0,0,0,0,1,1,0,0,1,1,0,0,1,1,1,1,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,1,1,1,1,0,0,1,1,0,1,0,1,1,0,0,1,1,1,1,1,0,0,0,0,0,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,1,1,1,0,0,0,1,1,1,1,1,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,1,1,0,1,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,0,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,0,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,1,1,0,0,1,1,0,0,0,1,1,1,1,0,0,0,1,1,0,0,0,0,0,0,1,1,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,1,1,0,0,1,1,0,0,1,1,0,0,0,1,1,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,1,1,0,0,0,0,1,1,1,1,1,1,0,0,1,1,0,0,1,1,0,0,0,1,1,0,0,0,0,0,0,0,0,0}
}; 
void showGameOverMessage() {
  for (int d = 0; d < 90 - 7; d++) {
    for (int col = 0; col < 8; col++) {
      delay(5);
      for (int row = 0; row < 8; row++) {
        lc.setLed(0, row, col, gameOverMessage[row][col + d]);
      }
    }
  }
}
