#include <Arduino.h>
#include <TaskScheduler.h>

#define _TASK_SLEEP_ON_IDLE_RUN

// ======= PIN =======
const int rgbPins[] = {9, 10, 11}; // red, green, blue
const int buttonPin = 13;
const int triggerPin = 14;
const int echoPin = 15;

// ======= VARIABILI GLOBALI =======
const int contenitoreVuoto = 10; // cm

// ======= MISURAZIONE DISTANZA ULTRASUONI =======
// non è un task perché viene fatta una chiamata alla volta
long durata;
float distanza;
void misurazioneDistanza() {
  digitalWrite(triggerPin, LOW);  // gli impulsi LOW servono per pulire la ricezione del vero impulso HIGH di 10 microsecondi
  delayMicroseconds(5);
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);

  durata = pulseIn(echoPin, HIGH);
  
  distanza = (durata/2.0 * 0.0343); // cm
  Serial.print("misurazioneDistanza: ");
  Serial.print(distanza);
  Serial.print("cm");
  Serial.println();
}

// ================= FADE LED RGB =================
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
  } else if (Color == "yellow") {
    setColor(brightness, brightness, 0);
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
void controlloContenitore();
void ledFadeRed() {
  ledFade("red");
}
void ledFadeYellow() {
  ledFade("yellow");
}
Scheduler runner;
Task TaskLedRed(30*TASK_MILLISECOND, TASK_FOREVER, &ledFadeRed);
Task TaskLedYellow(30*TASK_MILLISECOND, TASK_FOREVER, &ledFadeYellow);
Task TaskAperturaCoperchio(1000*TASK_MILLISECOND, TASK_FOREVER, &aperturaCoperchio);
Task TaskControlloContenitore(15*TASK_SECOND, TASK_FOREVER, &controlloContenitore);

// ================== CONTROLLO CONTENITORE ==================
void controlloContenitore() {
  Serial.print("controlloContenitore ");
  Serial.println(millis());

  misurazioneDistanza();
  if (distanza >= contenitoreVuoto) {
    Serial.println("contenitore vuoto");
    TaskLedYellow.enable();
  } else {
    TaskLedYellow.disable();
  }
}

// ================== APERTURA COPERCHIO ==================
void resetLed() {
  if (!TaskLedRed.isEnabled() && !TaskLedYellow.isEnabled()) {
    Serial.print("IF resetLed red :");
    Serial.print(TaskLedRed.isEnabled());
    Serial.print(" yellow :");
    Serial.println(TaskLedYellow.isEnabled());
    // reset ledFade
    // in questo modo non rimane accesso dall'ultima chiamata di ledFade
    brightness = 0;
    fadeAmount = 5;
    ledFade("red"); // va bene qualsiasi colore
  } else {
    Serial.print("ELSE resetLed red :");
    Serial.print(TaskLedRed.isEnabled());
    Serial.print(" yellow :");
    Serial.println(TaskLedYellow.isEnabled());
  }
}

void aperturaCoperchio() {
  if (digitalRead(buttonPin)==LOW) {  // coperchio aperto -> disabilito tutti i task
    TaskControlloContenitore.disable();
    TaskLedYellow.disable();
    TaskLedRed.enableIfNot();
  } else {  // coperchio chiuso -> disabilito task led e abilito i gli altri task
    TaskLedRed.disable();
    TaskControlloContenitore.enableIfNot();
    resetLed();
  }
}

// ================== SETUP ==================
void schedulerSetup() {
  runner.init();

  runner.addTask(TaskLedRed);
  runner.addTask(TaskLedYellow);
  runner.addTask(TaskAperturaCoperchio);
  runner.addTask(TaskControlloContenitore);
  
  TaskAperturaCoperchio.enable();
}

void setup() {
  Serial.begin(115200);

  pinMode(rgbPins[0], OUTPUT);  //red
  pinMode(rgbPins[1], OUTPUT);  //green
  pinMode(rgbPins[2], OUTPUT);  //blue
  pinMode(buttonPin, INPUT_PULLUP);  //bottone apertura coperchio
  pinMode(triggerPin, OUTPUT);  //ultrasonic
  pinMode(echoPin, INPUT);      //ultrasonic

  schedulerSetup();
}

void loop() {
  runner.execute();
}
