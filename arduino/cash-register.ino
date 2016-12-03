#include <avr/sleep.h>

const int OBSTACLE_SENSOR = 2;
const int OBSTACLE_BATTERY = 5;
const int BUZZER = 3;
const int BUZZER_BATTERY = 6;
const int SOUND_DURATION = 40;
const int RANDOM_ERROR_COUNT = 20;
int lastObstacleValue;

/**
 * Sleep mode
 */
void sleep() {
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  
  noInterrupts();
  sleep_enable();
  attachInterrupt(digitalPinToInterrupt(OBSTACLE_SENSOR), onObstacleChanged, CHANGE);
  interrupts();
  
  sleep_cpu();
}

/**
 * Wake up
 */
void wake() {
  sleep_disable();
  detachInterrupt(digitalPinToInterrupt(OBSTACLE_SENSOR));
}

/**
 * Obstacle sensor is changed
 */
void onObstacleChanged() {
  wake();
}

/**
 * Setup
 */
void setup() {
  pinMode(BUZZER, OUTPUT);
  pinMode(BUZZER_BATTERY, OUTPUT);
  pinMode(OBSTACLE_SENSOR, INPUT);
  pinMode(OBSTACLE_BATTERY, OUTPUT);

  lastObstacleValue = HIGH;
}

/**
 * Main loop
 */
void loop() {
  digitalWrite(OBSTACLE_BATTERY, 255);
  digitalWrite(BUZZER_BATTERY, 255);
  int obstacleValue = digitalRead(OBSTACLE_SENSOR);
  
  if (lastObstacleValue == HIGH && obstacleValue == LOW) {
      int randNumber = random(RANDOM_ERROR_COUNT);
      if (randNumber == 0) {
          soundError();
      } else {
          soundOk();
      }
  }
  lastObstacleValue = obstacleValue;

  sleep();
}

/**
 * Make OK sound
 */
void soundOk() {
    int i;
    for (i=0; i<SOUND_DURATION; i++) {
        digitalWrite(BUZZER, HIGH);
        delay(1);
        digitalWrite(BUZZER, LOW);
        delay(1);
    }
}

/**
 * Make error sound
 */
void soundError() {
    int i;
    for (i=0; i<SOUND_DURATION/2; i++) {
        analogWrite(BUZZER, 50);
        delay(1);
        analogWrite(BUZZER, 255);
        delay(1);
        analogWrite(BUZZER, 0);
        delay(1);
    }

    delay(100);

    for (i=0; i<SOUND_DURATION/2; i++) {
        analogWrite(BUZZER, 50);
        delay(1);
        analogWrite(BUZZER, 255);
        delay(1);
        analogWrite(BUZZER, 0);
        delay(1);
    }
}
