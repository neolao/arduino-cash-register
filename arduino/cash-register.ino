const int obstaclePin = 7;
const int buzzerPin = 6;
const int soundDuration = 40;
const int randomError = 20;
int lastObstacleValue;

/**
 * Setup the board
 */
void setup() {
    // initialize serial communication:
    //Serial.begin(9600);

    pinMode(buzzerPin, OUTPUT);
    pinMode(obstaclePin, INPUT);

    lastObstacleValue = HIGH;
}

/**
 * Main loop
 */
void loop() {
    int obstacleValue = digitalRead(obstaclePin);
  
    if (lastObstacleValue == HIGH && obstacleValue == LOW) {
        int randNumber = random(randomError);
        if (randNumber == 0) {
            soundError();
        } else {
            soundOk();
        }
    }
    lastObstacleValue = obstacleValue;
  
    delay(50);
}

/**
 * Make OK sound
 */
void soundOk() {
    int i;
    for (i=0; i<soundDuration; i++) {
        digitalWrite(buzzerPin, HIGH);
        delay(1);
        digitalWrite(buzzerPin, LOW);
        delay(1);
    }
}

/**
 * Make error sound
 */
void soundError() {
    int i;
    for (i=0; i<soundDuration; i++) {
        analogWrite(buzzerPin, 50);
        delay(1);
        analogWrite(buzzerPin, 255);
        delay(1);
        analogWrite(buzzerPin, 0);
        delay(1);
    }
}
