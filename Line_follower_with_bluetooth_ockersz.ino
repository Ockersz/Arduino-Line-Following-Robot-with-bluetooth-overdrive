// Motor A
#define enA 9  //PWM pin
#define in1 8
#define in2 7

// Motor B
#define enB 3  //PWM pin
#define in3 4
#define in4 5

#define FAR_LEFT_SENSOR A0
#define LEFT_SENSOR A1
#define MIDDLE_SENSOR A2
#define RIGHT_SENSOR A3
#define FAR_RIGHT_SENSOR A5

int const rightSpeed = 70;
int const leftSpeed = 70;

bool bluetoothMode = false;

unsigned long crossSectionTime = 0;

int const LINE_THRESHOLD = 500;

void setup() {
  // put your setup code here, to run once:
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);

  Serial.begin(9600);
  Serial1.begin(9600);

  attachInterrupt(digitalPinToInterrupt(19), handleBluetoothMessage, CHANGE);
}

void loop() {

  while (!bluetoothMode) {
    if (millis() - crossSectionTime > 5000) {

      stop();
      delay(5000);
      crossSectionTime = millis();

    } else {

      int leftSensorValue = analogRead(LEFT_SENSOR);
      int middleSensorValue = analogRead(MIDDLE_SENSOR);
      int rightSensorValue = analogRead(RIGHT_SENSOR);


      if (leftSensorValue > LINE_THRESHOLD && middleSensorValue < LINE_THRESHOLD && rightSensorValue > LINE_THRESHOLD) {
        // Middle sensor detects the line
        forward();
      } else if (leftSensorValue < LINE_THRESHOLD && middleSensorValue > LINE_THRESHOLD && rightSensorValue > LINE_THRESHOLD) {
        // Left sensor detects the line
        left();
      } else if (leftSensorValue < LINE_THRESHOLD && middleSensorValue < LINE_THRESHOLD && rightSensorValue > LINE_THRESHOLD) {
        //middle and left sensor detected
        left();
      } else if (leftSensorValue > LINE_THRESHOLD && middleSensorValue > LINE_THRESHOLD && rightSensorValue < LINE_THRESHOLD) {
        // Right sensor detects the line
        right();
      } else if (leftSensorValue > LINE_THRESHOLD && middleSensorValue < LINE_THRESHOLD && rightSensorValue < LINE_THRESHOLD) {
        //middle and right sensor detected
        right();
      } else if (leftSensorValue < LINE_THRESHOLD && middleSensorValue < LINE_THRESHOLD && rightSensorValue < LINE_THRESHOLD) {
        // cross-section detected
        stop();
      }

      
    }
  }
}



void handleBluetoothMessage() {
  // read incoming message from Bluetooth module
  bluetoothMode = true;
  while (Serial1.available() && bluetoothMode) {
    char command = Serial1.read();
    switch (command) {
      case 'F':
        forward();
        break;
      case 'B':
        backward();
        break;
      case 'L':
        left();
        break;
      case 'R':
        right();
        break;
      case 'S':
        stop();
        bluetoothMode = false;
        break;
      case 'W':
        bluetoothMode = false;
        break;
      default:
        stop();
        bluetoothMode = false;
        break;
    }
  }
}


// Functions for motor control
void forward() {
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  analogWrite(enA, leftSpeed);
  analogWrite(enB, rightSpeed);
}

void backward() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  analogWrite(enA, leftSpeed);
  analogWrite(enB, rightSpeed);
}

void left() {

  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  analogWrite(enA, leftSpeed);
  analogWrite(enB, rightSpeed);
}


void right() {
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  analogWrite(enA, leftSpeed);
  analogWrite(enB, rightSpeed);
}


void stop() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  analogWrite(enA, 0);
  analogWrite(enB, 0);
}
