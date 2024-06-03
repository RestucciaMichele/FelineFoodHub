#include <Arduino.h>
#include <TaskScheduler.h>
#include <Stepper.h>
#include <HX711.h>  //https://github.com/bogde/HX711 non è una libreria che si trova dalle risorse di Arduino IDE

#define _TASK_SLEEP_ON_IDLE_RUN
#define _TASK_STATUS_REQUEST

// ======= PIN =======
const int echoPin = 2;    // Echo pin for ultrasonic sensor
const int triggerPin = 3;  // Trigger pin for ultrasonic sensor
const int LOADCELL_SCK_PIN = 4;
const int LOADCELL_DOUT_PIN = 5;  
const int rgbPins[] = {6, 7, 8}; // red, green, blue
const int motorPin1 = 9;  // IN1 on the ULN2003 driver
const int motorPin2 = 10;  // IN3 on the ULN2003 driver
const int motorPin3 = 11;  // IN2 on the ULN2003 driver
const int motorPin4 = 12;  // IN4 on the ULN2003 driver
const int buttonPin = 13;

// ======= VARIABILI GLOBALI =======
const int intervalloControlloCiotola = 40; // secondi
const int intervalloCoperchio = 1; // secondi

const int distanzaContenitoreVuoto = 17; // cm
float distanzaDalCibo;

const int giriVite = 20;  // 20 per un giro completo

const int pesoCiboMinimo = 15;
const int pesoCiboMassimo = 60;
const float calibrationLoadCell = 2076.89;
float calibrationDrift = calibrationLoadCell;
unsigned long oldCalibrationTime = millis();
float pesoAttuale;

// ======= MOTORE STEPPER =======
const int stepsPerRevolution = 2048;  // 1 giro = 2048 passi
Stepper stepper = Stepper(stepsPerRevolution, motorPin1, motorPin3, motorPin2, motorPin4);
void stepperMove(int step) {
  stepper.setSpeed(14);
  stepper.step(step);
}

void moveForward() {
  stepperMove(200);
  stepperMove(-100);
}

// ======= BILANCIA, LOAD CELL =======
HX711 scale;
void misuraPeso() {
  scale.power_up();
  
  if (millis() > oldCalibrationTime) { // controllo overflow millis()
    calibrationDrift += ((millis()-oldCalibrationTime)/300000) * 0.001; // ogni 5 minuti = 300000 millis, aggiorno la calibrazione
  } // nel caso in cui millis() si resettasse, si perderebbe solo un aggiornamento della calibrazione
  oldCalibrationTime = millis();

  scale.set_scale(calibrationDrift);
  pesoAttuale = scale.get_units(10);

  scale.power_down();
}

// ======= DISTANZA ULTRASUONI =======
long durata;
void misurazioneDistanza() {
  digitalWrite(triggerPin, LOW);  // gli impulsi LOW servono per pulire il segnale per la ricezione del vero impulso HIGH di 10 microsecondi
  delayMicroseconds(5);           // delay obbligatori per il corretto funzionamento del sensore
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);

  durata = pulseIn(echoPin, HIGH);
  
  distanzaDalCibo = (durata/2.0 * 0.0343); // cm
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
  } else if (Color == "blue") {
    setColor(0, 0, brightness);
  } else {
    setColor(0,0,0);
  }

  brightness = brightness + fadeAmount;

  if (brightness <= 0 || brightness >= 255) {
    fadeAmount = -fadeAmount;
  }
}

// ================== TASK SETUP ==================
void controlloAperturaCoperchio();
void controlloCiotola();
void ledFadeRed() {ledFade("red");}
void ledFadeBlue() {ledFade("blue");}
void Motore();
Scheduler runner;
Task TaskLedRed(30*TASK_MILLISECOND, TASK_FOREVER, &ledFadeRed);
Task TaskLedBlue(30*TASK_MILLISECOND, TASK_FOREVER, &ledFadeBlue);
Task TaskControlloAperturaCoperchio(intervalloCoperchio*TASK_SECOND, TASK_FOREVER, &controlloAperturaCoperchio);
Task TaskAttivazioneMotore(TASK_MILLISECOND, giriVite, &Motore);
Task TaskControlloCiotola(intervalloControlloCiotola*TASK_SECOND, TASK_FOREVER, &controlloCiotola);

// ================== APERTURA COPERCHIO ==================
void resetLed() { // funzione chiamata tramite la funzione controlloAperturaCoperchio, schedulata ogni secondo. In questo modo il led non rimane acceso dall'ultima chiamata di ledFade
  if (!TaskLedRed.isEnabled() && !TaskLedBlue.isEnabled()) {
    brightness = 0;
    fadeAmount = 5;
    ledFade("red"); // va bene qualsiasi colore
  }
}

void controlloAperturaCoperchio() {
  if (digitalRead(buttonPin)==LOW) {  // coperchio aperto -> disabilito tutti i task
    Serial.println("COPERCHIO APERTO. Disabilitazione task ...");
    TaskAttivazioneMotore.disable();
    TaskControlloCiotola.disable();
    TaskLedBlue.disable();
    TaskLedRed.enableIfNot();
  } else {            // coperchio chiuso -> disabilito task led e abilito i gli altri task
    TaskLedRed.disable();
    resetLed();       //controllo funziona per tutte le chiamate di led (anche per colori diversi), quindi basta questo
    TaskControlloCiotola.enableIfNot();
  }
}

// ================== CONTROLLO BILANCIA E MOTORE CIOTOLA==================
void Motore() {
  setColor(0,255,0);
  moveForward();  
  setColor(0,0,0);
}

bool ricaricato = false;
void controlloCiotola() {
  Serial.print("controllo quantità scorta di cibo ==> ");
  misurazioneDistanza();
  Serial.println(distanzaDalCibo);
  if (distanzaDalCibo >= distanzaContenitoreVuoto) {
    Serial.println("SCORTA INSUFFICIENTE. ricarica di cibo non possibile ...");
    TaskLedBlue.enableIfNot(); 
  } else {
    TaskLedBlue.disable();
    Serial.print("SCORTA SUFFICIENTE. controllo quantità cibo nella ciotola ==> ");
    misuraPeso();
    Serial.println(pesoAttuale);
    if (pesoAttuale < pesoCiboMinimo || ricaricato == true) {
      Serial.println("CIBO INSUFFICIENTE. Attivazione motore ...");
      TaskAttivazioneMotore.restart();
      ricaricato = true;
      if (pesoAttuale >= pesoCiboMassimo) {
        Serial.println("CIBO SUFFICIENTE. Motore non attivato");
        ricaricato = false;
        TaskAttivazioneMotore.disable();
      } 
    } else {
      Serial.println("CIBO SUFFICIENTE. Motore non attivato");
    }
  }
}

// ================== SETUP ==================
void schedulerSetup() {
  runner.init();

  runner.addTask(TaskLedRed);
  runner.addTask(TaskLedBlue);
  runner.addTask(TaskControlloAperturaCoperchio);
  runner.addTask(TaskAttivazioneMotore);
  runner.addTask(TaskControlloCiotola);
  
  TaskControlloAperturaCoperchio.enable(); 
  TaskControlloCiotola.enable();
}

void loadCellSetup() {
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  scale.set_scale();
  scale.tare();
  scale.power_down();
}

void initialSetup() {
  Serial.println("Inizializzazione dispositivo FelineFoodHub ...");
  delay(2000);
  Serial.println("appoggiare sopra la bilancia una ciotola vuota per la calibrazione e taratura ...");
  delay(2000); Serial.println("...");
  delay(2000); Serial.println("...");
  delay(2000); Serial.println("...");
  Serial.println("Inizio Calibrazione ...");
  loadCellSetup();
  Serial.println("Calibrazione completata. Bilancia pronta per l'uso ...");
  schedulerSetup();
}

void setup() {
  Serial.begin(115200);

  pinMode(rgbPins[0], OUTPUT);  //red
  pinMode(rgbPins[1], OUTPUT);  //green
  pinMode(rgbPins[2], OUTPUT);  //blue
  pinMode(buttonPin, INPUT_PULLUP);  //bottone apertura coperchio
  pinMode(triggerPin, OUTPUT);  //ultrasonic
  pinMode(echoPin, INPUT);      //ultrasonic

  initialSetup();
}

void loop() {
  runner.execute();
}
