#include <Arduino.h>
#include <TaskScheduler.h>

#define _TASK_SLEEP_ON_IDLE_RUN


const int rgbPins[] = {9, 10, 11};
const int buttonPin = 3;

void aperturaCoperchio();
void prova() {
  Serial.println("prova");
}
Scheduler runner;
Task TaskCoperchio(1000*TASK_MILLISECOND, TASK_FOREVER, &aperturaCoperchio);
Task TaskProva(TASK_MILLISECOND, TASK_FOREVER, &prova);

void schedulerSetup() {
  runner.init();
  runner.addTask(TaskCoperchio);
  runner.addTask(TaskProva);
  TaskProva.enable();
  TaskCoperchio.enable();
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

  delay(30*TASK_MILLISECOND);
}

// ================== APERTURA COPERCHIO ==================
void aperturaCoperchio() {
  if (digitalRead(buttonPin)==LOW) {
    while(digitalRead(buttonPin)==LOW){
        ledFade("red");
    }

    // reset ledFade
    // per non far rimanere il led accesso anche fuori dal while precedente
    brightness = 0;
    fadeAmount = 5;
    ledFade("red");
  }
  delay(10*TASK_MILLISECOND);
}
