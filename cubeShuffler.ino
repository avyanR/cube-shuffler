#include <Adafruit_PWMServoDriver.h>
#include <ESP8266TrueRandom.h>
#include <List.hpp>

Adafruit_PWMServoDriver board1 = Adafruit_PWMServoDriver(0x40);

#define BUTTON_PIN D0

#define SHUFFLE_MOVES 16

#define MOTOR_LEFT_ROTATOR 0
#define MOTOR_LEFT_HOLDER 1
#define MOTOR_RIGHT_ROTATOR 2
#define MOTOR_RIGHT_HOLDER 3

#define SERVOMIN 74
#define SERVOMAX 556
#define ANGLEMIN 0
#define ANGLEMAX 205

#define LEFT_ROTATOR_0 9
#define LEFT_ROTATOR_90 99
#define LEFT_ROTATOR_180 185

#define RIGHT_ROTATOR_0 20
#define RIGHT_ROTATOR_90 112
#define RIGHT_ROTATOR_180 198

#define LEFT_HOLDER_HOLD 100
#define LEFT_HOLDER_LETGO 110
#define LEFT_HOLDER_ROTATE 140

#define RIGHT_HOLDER_HOLD 104
#define RIGHT_HOLDER_LETGO 114
#define RIGHT_HOLDER_ROTATE 140

List<int> randomMoves;

int angleToPulse(int ang) {
  int pulse = map(ang, ANGLEMIN, ANGLEMAX, SERVOMIN, SERVOMAX);
  // Serial.print("Angle: "); Serial.print(ang);
  // Serial.print(" pulse: "); Serial.println(pulse);
  return pulse;
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  board1.begin();
  board1.setPWMFreq(50);
  // delay(5192);
  pinMode(BUTTON_PIN, INPUT_PULLDOWN_16);
  initMotors();
}

void rotateMotor(int numMotor, int rotationAngle) {
  // Serial.print("Motor: ");
  // Serial.print(numMotor);
  // Serial.print(" | Angle: ");
  // Serial.print(rotationAngle);
  // Serial.print(" | Pulse: ");
  // Serial.println(angleToPulse(rotationAngle));
  board1.setPWM(numMotor, 0, angleToPulse(rotationAngle));
  delay(512);
}

void leftRotator(int rotationAngle){
  rotateMotor(MOTOR_LEFT_ROTATOR, rotationAngle);
}

void leftHolder(int rotationAngle){
  rotateMotor(MOTOR_LEFT_HOLDER, rotationAngle);
}

void rightRotator(int rotationAngle){
  rotateMotor(MOTOR_RIGHT_ROTATOR, rotationAngle);
}

void rightHolder(int rotationAngle){
  rotateMotor(MOTOR_RIGHT_HOLDER, rotationAngle);
}

void initMotors(){
  leftRotator(LEFT_ROTATOR_90);
  rightRotator(RIGHT_ROTATOR_90);
  letgo();
}

void hold(){
  leftHolder(LEFT_HOLDER_HOLD);
  rightHolder(RIGHT_HOLDER_HOLD);
}

void letgo(){
  leftHolder(LEFT_HOLDER_LETGO);
  rightHolder(RIGHT_HOLDER_LETGO);
}

void openToRotateLeft(){
  leftHolder(LEFT_HOLDER_ROTATE);
}

void openToRotateRight(){
  rightHolder(RIGHT_HOLDER_ROTATE);
}

void leftMoveClockwise(){
  Serial.println("leftMoveClockwise");
  leftRotator(LEFT_ROTATOR_0);
  leftHolder(LEFT_HOLDER_ROTATE);
  leftRotator(LEFT_ROTATOR_90);
  leftHolder(LEFT_HOLDER_HOLD);
}

void leftMoveAntiClockwise(){
  Serial.println("leftMoveAntiClockwise");
  leftRotator(LEFT_ROTATOR_180);
  leftHolder(LEFT_HOLDER_ROTATE);
  leftRotator(LEFT_ROTATOR_90);
  leftHolder(LEFT_HOLDER_HOLD);
}

void rightMoveClockwise(){
  Serial.println("rightMoveClockwise");
  rightRotator(RIGHT_ROTATOR_180);
  rightHolder(RIGHT_HOLDER_ROTATE);
  rightRotator(RIGHT_ROTATOR_90);
  rightHolder(RIGHT_HOLDER_HOLD);
}

void rightMoveAntiClockwise(){
  Serial.println("rightMoveAntiClockwise");
  rightRotator(RIGHT_ROTATOR_0);
  rightHolder(RIGHT_HOLDER_ROTATE);
  rightRotator(RIGHT_ROTATOR_90);
  rightHolder(RIGHT_HOLDER_HOLD);
}

void leftFaceClockwise(){
  Serial.println("leftFaceClockwise");
  rightHolder(RIGHT_HOLDER_ROTATE);
  leftRotator(LEFT_ROTATOR_0);
  rightHolder(RIGHT_HOLDER_HOLD);
  leftHolder(LEFT_HOLDER_ROTATE);
  leftRotator(LEFT_ROTATOR_90);
  leftHolder(LEFT_HOLDER_HOLD);
}

void leftFaceAntiClockwise(){
  Serial.println("leftFaceAntiClockwise");
  rightHolder(RIGHT_HOLDER_ROTATE);
  leftRotator(LEFT_ROTATOR_180);
  rightHolder(RIGHT_HOLDER_HOLD);
  leftHolder(LEFT_HOLDER_ROTATE);
  leftRotator(LEFT_ROTATOR_90);
  leftHolder(LEFT_HOLDER_HOLD);
}

void rightFaceClockwise(){
  Serial.println("rightFaceClockwise");
  leftHolder(LEFT_HOLDER_ROTATE);
  rightRotator(RIGHT_ROTATOR_180);
  leftHolder(LEFT_HOLDER_HOLD);
  rightHolder(RIGHT_HOLDER_ROTATE);
  rightRotator(RIGHT_ROTATOR_90);
  rightHolder(RIGHT_HOLDER_HOLD);
}

void rightFaceAntiClockwise(){
  Serial.println("rightFaceAntiClockwise");
  leftHolder(LEFT_HOLDER_ROTATE);
  rightRotator(RIGHT_ROTATOR_0);
  leftHolder(LEFT_HOLDER_HOLD);
  rightHolder(RIGHT_HOLDER_ROTATE);
  rightRotator(RIGHT_ROTATOR_90);
  rightHolder(RIGHT_HOLDER_HOLD);
}

void reseat(){
  board1.setPWM(MOTOR_LEFT_HOLDER, 0, angleToPulse(LEFT_HOLDER_LETGO));
  board1.setPWM(MOTOR_RIGHT_HOLDER, 0, angleToPulse(RIGHT_HOLDER_LETGO));
  delay(256);
  board1.setPWM(MOTOR_LEFT_HOLDER, 0, angleToPulse(LEFT_HOLDER_HOLD));
  board1.setPWM(MOTOR_RIGHT_HOLDER, 0, angleToPulse(RIGHT_HOLDER_HOLD));
  delay(256);
}

void shuffle(int numMoves){
  for (int i=0; i<numMoves; i++){
    bool faceMove = move(ESP8266TrueRandom.random(0,8));
    if (faceMove == true){
      i--;
    }
  }
}

int last = -1;

void generateMoves(){
  for (int i=0; i<SHUFFLE_MOVES; i++){
    int move = ESP8266TrueRandom.random(0,8);
    Serial.print("Adding: ");
    Serial.println(move);
    randomMoves.add(move);
    if (i > 0){
      int current = move;
      Serial.print("Last: ");
      Serial.print(last);
      Serial.print(" | Current: ");
      Serial.println(current);
      if ((last % 2 == 0 && current == last + 1) || (last % 2 == 1 && current == last - 1)){
        Serial.print("Removing: ");
        Serial.println(randomMoves.get(randomMoves.getSize()-1));
        randomMoves.removeLast();
        i--;
      } else if (move > 3){
        i--;
      }
    }
    last = move;
  }
}

bool move(int moveNum){
  switch (moveNum){
    case 0:
      leftMoveClockwise();
      break;
    case 1:
      leftMoveAntiClockwise();
      break;
    case 2:
      rightMoveClockwise();
      break;
    case 3:
      rightMoveAntiClockwise();
      break;
    case 4:
      leftFaceClockwise();
      break;
    case 5:
      leftFaceAntiClockwise();
      break;
    case 6:
      rightFaceClockwise();
      break;
    case 7:
      rightFaceAntiClockwise();
      break;
  }
  reseat();
  if (moveNum < 4) {
    Serial.println("Normal Move");
    return false;
  } else {
    Serial.println("Face Move");
    return true;
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  int buttonState = LOW;
  buttonState = digitalRead(BUTTON_PIN);
  if (buttonState == HIGH){
    Serial.println("Button Pressed");
    if (randomMoves.getSize() == 0){
      Serial.println("Generating moves....");
      generateMoves();
    }
    hold();
    for (int i=0; i<randomMoves.getSize(); i++){
      move(randomMoves.get(i));
      Serial.println(randomMoves.get(i));
    }
    letgo();
  }
  // for (int i=0; i<8; i++){
  //   move(ESP8266TrueRandom.random(0,8));
  // }
}