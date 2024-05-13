#include <Arduino.h>
#include <TaskScheduler.h>

#define _TASK_SLEEP_ON_IDLE_RUN

const int rgbPins[] = {9, 10, 11};
const int buttonPin = 13;

// ================== FADE LED RGB ==================
void setColor(int redValue, int greenValue, int blueValue) {
  analogWrite(rgbPins[0], redValue);
  analogWrite(rgbPins[1], greenValue);
  analogWrite(rgbPins[2], blueValue);
}

int brightness = 0;
int fadeAmount = 5;
void ledFade(String Color) {
  if (Color == "red") {
    setColor(brightness, 0, 0);
  } else if (Color == "green") {
    setColor(0, brightness, 0);
  } else {
    setColor(0,0,0);
  }

  brightness = brightness + fadeAmount;

  if (brightness <= 0 || brightness >= 255) {
    fadeAmount = -fadeAmount;
  }
}

// ================== TASK SETUP ==================
void aperturaCoperchio();
void ledFadeRed() {
  ledFade("red");
}
Scheduler runner;
Task TaskLedRed(30*TASK_MILLISECOND, TASK_FOREVER, &ledFadeRed);
Task TaskAperturaCoperchio(1000*TASK_MILLISECOND, TASK_FOREVER, &aperturaCoperchio);

// ================== APERTURA COPERCHIO ==================
void aperturaCoperchio() {
  if (digitalRead(buttonPin)==LOW) {
    TaskLedRed.enable();
  } else {
    TaskLedRed.disable();

    // reset ledFade
    // per non far rimanere il led accesso anche fuori dal while precedente
    brightness = 0;
    fadeAmount = 5;
    ledFade("red");
  }
}

// ================== SETUP ==================
void schedulerSetup() {
  runner.init();

  runner.addTask(TaskAperturaCoperchio);
  runner.addTask(TaskLedRed);
  
  TaskAperturaCoperchio.enable();
}

void setup() {
  Serial.begin(115200);

  pinMode(rgbPins[0], OUTPUT);  //red
  pinMode(rgbPins[1], OUTPUT);  //green
  pinMode(rgbPins[2], OUTPUT);  //blue

  pinMode(buttonPin, INPUT_PULLUP);  //bottone apertura coperchio

  schedulerSetup();
}

void loop() {
  runner.execute();
}
