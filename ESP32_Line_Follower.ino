/*
  ESP32 Two-Sensor Line Following Robot
  -------------------------------------
  Controller : ESP32 DevKit V1
  Sensors    : 2 x HW-201 digital IR modules
  Motor driver: DRV8833

  Tested sensor logic for this build:
    L=0, R=0 -> forward
    L=0, R=1 -> correct right
    L=1, R=0 -> correct left
    L=1, R=1 -> stop (safe default)

  Motor PWM values below are the latest calibration reported working
  on this particular robot. Recalibrate if you change motors, battery,
  chassis, sensor position or track.

  Uses the Arduino-ESP32 LEDC API: ledcAttach() and ledcWrite().
*/

#define LEFT_IR_PIN   35
#define RIGHT_IR_PIN  34

#define AIN1_PIN      18
#define AIN2_PWM_PIN  19
#define BIN1_PWM_PIN  21
#define BIN2_PIN      22
#define NSLEEP_PIN    23

const uint32_t PWM_FREQUENCY = 1000;
const uint8_t PWM_RESOLUTION = 8;  // duty range: 0 to 255

// Calibrated forward speeds for this robot.
const uint8_t LEFT_FORWARD_PWM  = 130;
const uint8_t RIGHT_FORWARD_PWM = 72;

// Speed used for the active wheel during a correction.
const uint8_t CORRECTION_PWM = 120;

const uint16_t LOOP_DELAY_US = 200;

bool pwmReady = false;

void stopMotors() {
  if (pwmReady) {
    ledcWrite(AIN2_PWM_PIN, 0);
    ledcWrite(BIN1_PWM_PIN, 0);
  }
  digitalWrite(AIN1_PIN, LOW);
  digitalWrite(BIN2_PIN, LOW);
}

void moveForward() {
  // Verified forward polarity for this build:
  // Left channel: AIN1 LOW, AIN2 PWM
  // Right channel: BIN1 PWM, BIN2 LOW
  digitalWrite(AIN1_PIN, LOW);
  digitalWrite(BIN2_PIN, LOW);
  ledcWrite(AIN2_PWM_PIN, LEFT_FORWARD_PWM);
  ledcWrite(BIN1_PWM_PIN, RIGHT_FORWARD_PWM);
}

void correctLeft() {
  // Slow/stop the left wheel and keep the right wheel moving.
  digitalWrite(AIN1_PIN, LOW);
  ledcWrite(AIN2_PWM_PIN, 0);

  digitalWrite(BIN2_PIN, LOW);
  ledcWrite(BIN1_PWM_PIN, CORRECTION_PWM);
}

void correctRight() {
  // Keep the left wheel moving and slow/stop the right wheel.
  digitalWrite(AIN1_PIN, LOW);
  ledcWrite(AIN2_PWM_PIN, LEFT_FORWARD_PWM);

  digitalWrite(BIN2_PIN, LOW);
  ledcWrite(BIN1_PWM_PIN, 0);
}

void setup() {
  Serial.begin(115200);

  pinMode(LEFT_IR_PIN, INPUT);
  pinMode(RIGHT_IR_PIN, INPUT);

  pinMode(AIN1_PIN, OUTPUT);
  pinMode(BIN2_PIN, OUTPUT);
  pinMode(NSLEEP_PIN, OUTPUT);

  digitalWrite(NSLEEP_PIN, HIGH);  // Enable DRV8833

  const bool leftPwmAttached =
      ledcAttach(AIN2_PWM_PIN, PWM_FREQUENCY, PWM_RESOLUTION);
  const bool rightPwmAttached =
      ledcAttach(BIN1_PWM_PIN, PWM_FREQUENCY, PWM_RESOLUTION);

  pwmReady = leftPwmAttached && rightPwmAttached;

  if (!pwmReady) {
    Serial.println("ERROR: PWM setup failed. Check ESP32 Arduino core version.");
    stopMotors();
    while (true) {
      delay(1000);
    }
  }

  stopMotors();
  Serial.println("ESP32 line follower ready.");
}

void loop() {
  const int leftValue = digitalRead(LEFT_IR_PIN);
  const int rightValue = digitalRead(RIGHT_IR_PIN);

  if (leftValue == LOW && rightValue == LOW) {
    moveForward();
  } else if (leftValue == LOW && rightValue == HIGH) {
    correctRight();
  } else if (leftValue == HIGH && rightValue == LOW) {
    correctLeft();
  } else {
    // Both sensors HIGH: stop rather than making an unverified guess.
    stopMotors();
  }

  delayMicroseconds(LOOP_DELAY_US);
}
