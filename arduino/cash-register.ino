#include "LowPower.h"

const int OBSTACLE_SENSOR = 2;
const int OBSTACLE_BATTERY = 3;
const int LED = 5;
const int BUZZER = 9;
const int BUZZER_BATTERY = 6;
const int SOUND_DURATION = 40;
const int RANDOM_ERROR_COUNT = 20;
const int COUNT_BEFORE_SLEEP = 30;
int lastObstacleValue;

/**
 * Setup
 */
void setup() {
  pinMode(LED, OUTPUT);
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
  start:

  // Turn on batteries
  digitalWrite(OBSTACLE_BATTERY, 255);
  digitalWrite(BUZZER_BATTERY, 255);


  attachInterrupt(digitalPinToInterrupt(OBSTACLE_SENSOR), wake, LOW);

  int obstacleValue = HIGH;
  int count = COUNT_BEFORE_SLEEP;
  while (count > 0) {
    obstacleValue = digitalRead(OBSTACLE_SENSOR);
    if (lastObstacleValue == HIGH && obstacleValue == LOW) {
      detachInterrupt(digitalPinToInterrupt(OBSTACLE_SENSOR));
      
      int randNumber = random(RANDOM_ERROR_COUNT);
      if (randNumber == 0) {
          soundError();
      } else {
          soundOk();
      }
    }
    lastObstacleValue = obstacleValue;
    if (obstacleValue == LOW) {
      
      delay(500);
      goto start;
      break;
    }
    
    count--;
    LowPower.powerSave(SLEEP_2S, ADC_OFF, BOD_OFF, TIMER2_ON);
  }
  detachInterrupt(digitalPinToInterrupt(OBSTACLE_SENSOR));
  

  // Last light before sleep
  analogWrite(LED, 10);
  delay(2000);
  analogWrite(LED, 0);

  // Turn off batteries
  digitalWrite(OBSTACLE_BATTERY, 0);
  digitalWrite(BUZZER_BATTERY, 0);

  // Sleep and wait sensor battery
  attachInterrupt(digitalPinToInterrupt(OBSTACLE_SENSOR), wake, RISING);
  LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF);
  detachInterrupt(digitalPinToInterrupt(OBSTACLE_SENSOR));
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

/**
 * Wake up
 */
void wake() {
  
}
