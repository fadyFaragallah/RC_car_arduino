// Constants
// Motors
const int ENR = 2; // Control speed of Right Motor
const int IN1 = 22; // Right Yellow
const int IN2 = 23; // Right Grey
const int ENL = 3; // Control speed of Left Motor
const int IN3 = 24; // Left Yellow
const int IN4 = 25; // Left Grey
// UltraSonic
const int triggerPin = 26;
const int echoPin = 27;
// LDR
const int LDRIn = A0;
const int LDROut = 4;
// Line Tracking
const int line_followerL = 28;
const int line_followerR = 29;
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
// Variables
char bluetoothChar;
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
// Functions
void setMove(int IN1D, int IN2D, int IN3D, int IN4D){
  digitalWrite(IN1, IN1D);
  digitalWrite(IN2, IN2D);
  digitalWrite(IN3, IN3D);
  digitalWrite(IN4, IN4D);
}
void setMotorSpeed(int MotorControlPin, char receivedChar){
  if(receivedChar == 'q'){
    analogWrite(MotorControlPin, 255);
  }
  else {
    analogWrite(MotorControlPin, ((int(receivedChar) - 48) * 255) / 10);
  }
}
long getDistance(){
  long duration, distance;
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  distance= duration*0.034/2;
  return distance;
}
void autoLight(){
  int in = analogRead(LDRIn);
  Serial.print("LDR Reads ");
  Serial.println(in);
  if(in > 300){
    int out = 0.51 * in - 153;
    analogWrite(LDROut, out);
    Serial.print("LDR Outs ");
    Serial.println(out);
  } else analogWrite(LDROut, 0);
}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
// Start
void setup() {
  // Setup Pins Mode
  pinMode(ENR, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENL, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  // UltraSonic
  pinMode(triggerPin, OUTPUT);
  pinMode(echoPin, INPUT);
  // LRD
  pinMode(LDRIn, INPUT);
  pinMode(LDROut, OUTPUT);
  // Line Tracking
  pinMode(line_followerL,INPUT);
  pinMode(line_followerR,INPUT);
  // Start Serial
  Serial.begin(9600);
  Serial3.begin(9600);
  // Set Max speed as default... Run Once! (first time ONLY)
  analogWrite(ENR, 255);
  analogWrite(ENL, 255);
  // setMove(0, 1, 1, 0);
  Serial.println(">>> START");
}

void loop() {
  /*
  for(int i = 0; i < 255; i++){
    analogWrite(LDROut, i);
    delay(100);
  }
  */
  autoLight();
  if(Serial3.available()){
    // Get received Char
    bluetoothChar = Serial3.read();
    Serial.print("Received Char is => ");
    Serial.println(bluetoothChar);
    // Movements
    if(bluetoothChar == 'F'){
      // Move forward
      setMove(0, 1, 0, 1);
    } else if(bluetoothChar == 'B'){
      // Move backward
      setMove(1, 0, 1, 0);
    } else if(bluetoothChar == 'L'){
      // Rotate Left
      setMove(0, 1, 1, 0);
    } else if(bluetoothChar == 'R'){
      // Rotate Right
      setMove(1, 0, 0, 1);
    } else if(bluetoothChar == 'J'){
      // Turn Back Right
      setMove(0, 0, 1, 0);
    } else if(bluetoothChar == 'H'){
      // Turn Back Left
      setMove(1, 0, 0, 0);
    } else if(bluetoothChar == 'I'){
      // Turn Forward Right
      setMove(0, 0, 0, 1);
    } else if(bluetoothChar == 'G'){
      // Turn Forward Left
      setMove(0, 1, 0, 0);
    } else if(bluetoothChar == 'S'){
      setMove(0, 0, 0, 0);
    } else if(bluetoothChar == 'V'){
      // Do nothing yet!
    } else if(bluetoothChar == 'U'){
      // line tracking mode
      serial.println("line tracking mode is on");
      while(bluetoothChar !='u')
      {
        if((!(digitalRead(line_followerL))) && (!(digitalRead(line_followerR))))
        {//move forward
        setMove(0,1,0,1);}
        if(((digitalRead(line_followerL))) && (!(digitalRead(line_followerR))))
        {//left sensor detects black so move left
        setMove(0,1,1,0);}
        if((!(digitalRead(line_followerL))) && ((digitalRead(line_followerR))))
        {//right sensor detects black so move right
        setMove(1,0,0,1);}
        if(((digitalRead(line_followerL))) && ((digitalRead(line_followerR))))
        {//both detecting black so stop
        setMove(0,0,0,0);}
      }
      serial.println("line tracking mode is off");
      setMove(0,0,0,0);
    } else if(bluetoothChar == 'u'){
      // Do nothing yet!
    } else if(bluetoothChar == 'W'){
      // Do nothing yet!
    } else if(bluetoothChar == 'w'){
      // Do nothing yet!
    } else if(bluetoothChar == 'X'){
      // Auto UltraSonic
      Serial.println(">>> Auto UltraSonic Mode is ACTIVATED");
      do{
        long distanceInCm = getDistance();
        if(distanceInCm < 30){
          setMotorSpeed(ENR, 'q');
          setMotorSpeed(ENL, 'q');
          // Rotate Right
          setMove(1, 0, 0, 1);
          delay(600);
        }
        else if(distanceInCm < 45){
          setMotorSpeed(ENR, '3');
          setMotorSpeed(ENL, '3');
        }
        else if(distanceInCm < 60){
          setMotorSpeed(ENR, '5');
          setMotorSpeed(ENL, '5');
        }
        else {
          setMotorSpeed(ENR, 'q');
          setMotorSpeed(ENL, 'q');
          // Move forward
          setMove(0, 1, 0, 1);
        }
        if(Serial3.available()){
          bluetoothChar = Serial3.read();
          Serial.print("Received Char is => ");
          Serial.println(bluetoothChar);
        }
      } while(bluetoothChar != 'x');
      // Stop Auto
      setMove(0, 0, 0, 0);
      Serial.println(">>> Auto UltraSonic Mode is DEACTIVATED");
    } else if(bluetoothChar == 'D'){
      // Stop Every thing
      setMove(0, 0, 0, 0);
    } else {
      setMotorSpeed(ENR, bluetoothChar);
      setMotorSpeed(ENL, bluetoothChar);
    }
  }
}
