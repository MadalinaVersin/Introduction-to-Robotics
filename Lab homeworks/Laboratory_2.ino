const int redPin = 9;
const int greenPin = 10;
const int bluePin = 11;
const int podPinr = A0;
const int podPing = A1;
const int podPinb = A2; 
void setup() {
  // put your setup code here, to run once:
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  pinMode(podPinr, INPUT);
  pinMode(podPing, INPUT);
  pinMode(podPinb, INPUT);
  Serial.begin(9600); 

}

void loop() {
  // put your main code here, to run repeatedly:
  int podValr = analogRead(podPinr);
  podValr = map(podValr,0,1023,0,255);

  int podValg = analogRead(podPing);
  podValg = map(podValg, 0, 1023, 0, 255);

  int podValb = analogRead(podPinb);
  podValb = map(podValb, 0, 1023, 0, 255);
  analogWrite(redPin, podValr);
  analogWrite(greenPin, podValg);
  analogWrite(bluePin, podValb);
 
}
