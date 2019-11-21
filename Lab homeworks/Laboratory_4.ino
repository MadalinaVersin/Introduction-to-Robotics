const int pinSW = 1; // digital pin connected to switch output
const int pinX = A0; // A0 - analog pin connected to X output
const int pinY = A1; // A1 - analog pin connected to Y output

// declare all the segments pins
const int pinA = 12;
const int pinB = 8;
const int pinC = 5;
const int pinD = 3;
const int pinE = 2;
const int pinF = 11;
const int pinG = 6;
const int pinDP = 4;

const int pinD1 = 7;
const int pinD2 = 9;
const int pinD3 = 10;
const int pinD4 = 13;


const int segSize = 8;
const int noOfDigits = 10;
const int noOfDisplays = 4;


int dpState = HIGH;
int swState = LOW;
int lastSwState = LOW;
int switchValue;
int xValue = 0;
int yValue = 0;

bool joyMovedX = false;
bool joyMovedY = false;
int digit = 0;
int minThreshold= 400;
int maxThreshold= 600;

// store the pins in an array for easier access
int segments[segSize] = {
  pinA, pinB, pinC, pinD, pinE, pinF, pinG, pinDP
};
int currentDigit = 3;
int valueD1 = 0;
int valueD2 = 0;
int valueD3 = 0;
int valueD4 = 0;

int values[noOfDisplays] = {
 valueD1, valueD2, valueD3, valueD4  
};

int digits[noOfDisplays] = {
 pinD1, pinD2, pinD3, pinD4
};


byte digitMatrix[noOfDigits][segSize - 1] = {
// a  b  c  d  e  f  g
  {1, 1, 1, 1, 1, 1, 0}, // 0
  {0, 1, 1, 0, 0, 0, 0}, // 1
  {1, 1, 0, 1, 1, 0, 1}, // 2
  {1, 1, 1, 1, 0, 0, 1}, // 3
  {0, 1, 1, 0, 0, 1, 1}, // 4
  {1, 0, 1, 1, 0, 1, 1}, // 5
  {1, 0, 1, 1, 1, 1, 1}, // 6
  {1, 1, 1, 0, 0, 0, 0}, // 7
  {1, 1, 1, 1, 1, 1, 1}, // 8
  {1, 1, 1, 1, 0, 1, 1}  // 9
};

void displayNumber(byte digit, byte decimalPoint) {
  for (int i = 0; i < segSize - 1; i++) {
    digitalWrite(segments[i], digitMatrix[digit][i]);
    digitalWrite(segments[segSize - 1], decimalPoint);

  }
 }

void showDigit(int num) {
 for (int i = 0; i < noOfDisplays; i++) {
  digitalWrite(digits[i], HIGH);
 }
  digitalWrite(digits[num], LOW);
}


void setup() {
  // initialize all the pins
  for (int i = 0; i < segSize; i++) {
  pinMode(segments[i], OUTPUT);
  }
  for (int i = 0; i < noOfDisplays; i++){
  pinMode(digits[i], OUTPUT);
 }
  pinMode(pinSW, INPUT_PULLUP);
 
 Serial.begin(9600);
}

void loop() {
  swState = digitalRead(pinSW);
  if (swState !=  lastSwState) {
  if (swState == LOW) {
      dpState = !dpState;
   }
  }
  lastSwState = swState;

 if (dpState == HIGH){
  xValue = analogRead(pinX);
  if (xValue > maxThreshold && joyMovedX == false){
    if (currentDigit > 0) {
      currentDigit--;
      } 
    else {
      currentDigit = 3;
     }
    joyMovedX = true;
   }
 
  if (xValue < minThreshold && joyMovedX == false){
  if (currentDigit < 3) {
      currentDigit++;
  } else {
      currentDigit = 0;
  }
  joyMovedX = true;
  }

  if (xValue >= minThreshold && xValue <= maxThreshold) {
  joyMovedX = false;
  }
 }

 if (dpState == LOW){
  yValue = analogRead(pinY);
  
  if (yValue > maxThreshold && joyMovedY == false) {
    if (values[currentDigit] > 0) {
        values[currentDigit]--;
    } 
    else {
        values[currentDigit] = 9;
    }
    joyMovedY = true;
  }
   
  if (yValue < minThreshold && joyMovedY == false) {
    if (values[currentDigit] < 9) {
        values[currentDigit]++;
    } 
    else {
      values[currentDigit] = 0;
    }
    joyMovedY = true;
  }

  if (yValue >= minThreshold && yValue <= maxThreshold) {
  joyMovedY = false;
  }
 }

 for (int i = 0; i < noOfDisplays; i++) {
    showDigit(i);
    displayNumber(values[i], HIGH);
    if(i == currentDigit){
    digitalWrite(segments[segSize - 1], HIGH);
    }
    else 
      digitalWrite(segments[segSize - 1], LOW);
  delay(5);
 }
 
}
  
