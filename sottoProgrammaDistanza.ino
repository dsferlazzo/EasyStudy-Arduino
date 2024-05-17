//NOTA: POSSIBILE MIGLIORAMENTO PER IL CODICE, PER GESTIRE I LED (SE DA IMPLEMENTARE) UTILIZZARE UNA FUNZIONE CHE INVERTE LO STATO DEI DUE LED
#include <HCSR04.h> //LIBRERIA PER GESTIRE I SENSORI DI DISTANZA

const int triggerPin1 = 12;  //DEFINISCO I PIN DEI DUE SENSORI DI DISTANZA
const int echoPin1 = 13;
const int triggerPin2 = 6;
const int echoPin2 = 5;

const int distanceThreshold = 50; //DISTANZA SOTTO IL QUALE VIENE CONSIDERATO IL PASSAGGIO DI UNA PERSONA, IN cm

UltraSonicDistanceSensor distanceSensor1(triggerPin1, echoPin1);  //CREO GLI OGGETTI DEI SENSORI DI DISTANZA
UltraSonicDistanceSensor distanceSensor2(triggerPin2, echoPin2);

bool currentSensorState1 = false; //CONTENENTE LO STATO DEI SENSORI
bool currentSensorState2 = false; //IF true => UNA PERSONA E' APPENA PASSATA; IF false => NON E' PASSATO NESSUNO; INIZIALIZZATI A false

// Timing configuration
const unsigned long triggerDelay = 2000;  //UNA PERSONA HA 2s PER PASSARE TRA I DUE SENSORI (VALORE ALTO PER DEBUGGING)

// Variables to keep the state
int count = 0;
unsigned long lastTrigger1 = 0;
unsigned long lastTrigger2 = 0;
int airQualityThreshold = 300;


void setup() {
  Serial.begin(9600);
}

void loop() {
  handleDistanceSensor();
  handlePeopleCounter(currentSensorState1, currentSensorState2);
  delay(100);
}

void handleDistanceSensor(){  //FUNZIONE CHE MODIFICA LO STATO DEI DUE SENSORI DI DISTANZA
  float distance1 = distanceSensor1.measureDistanceCm();  //RICEVO LE DISTANZE DEI DUE SENSORI
  //Serial.print("Distanza1: ");
  //Serial.println(distance1);  //DEBUGGING
  float distance2 = distanceSensor2.measureDistanceCm();
  //Serial.println(distance2);
  // Serial.print("Distanza2: ");
  //Serial.println(distance2);  //DEBUGGING

  if((distance1 <= distanceThreshold) && (distance1!=-1))  //COMPARO LE DISTANZE CON IL VALORE DI THRESHOLD, MODIFICANDO DI CONSEGUENZA IL SENSORSTATE
    currentSensorState1 = true;
  else currentSensorState1 = false;
  if((distance2 <= distanceThreshold) && (distance2!=-1))
    currentSensorState2 = true;
  else currentSensorState2 = false;

  
  //STAMPO SU CONSOLE LO STATO DEGLI ARRAY, DEBUGGING
  /*
  Serial.print("Sensore1: ");
  Serial.print(currentSensorState1);
  Serial.print("   Sensore2: ");
  Serial.println(currentSensorState2);
  */
}


void handlePeopleCounter(int currentState1, int currentState2) {
  unsigned long currentTime = millis();
  
  if (currentState1 == true) {  //DA RICONTROLLARE, LOGICA
    lastTrigger1 = currentTime;
    //Serial.print("Trigger1: ");
    //Serial.println(lastTrigger1);
    //digitalWrite(greenLED, LOW);
    //digitalWrite(redLED, HIGH); // Stop others from entering
  }
  if (currentState2 == true) {  //DA RICONTROLLARE, LOGICA
    lastTrigger2 = currentTime;
    //Serial.print("Trigger2: ");
    //Serial.println(lastTrigger2);
    //digitalWrite(greenLED, LOW);
    //digitalWrite(redLED, HIGH); // Stop others from entering
  }

  if (lastTrigger1 != 0 && lastTrigger2 != 0) {   //SE I DUE TIMER SONO DIVERSI DA 0, QUINDI SE ENTRAMBI I SENSORI SONO STATI ATTRAVERSATI DOPO L'ULTIMA ENTRATA/USCITA
    if (lastTrigger1 < lastTrigger2 && (lastTrigger2 - lastTrigger1) <= triggerDelay) {   //DETERMINO IL VERSO AND CONTROLLO DI STARE NEL TIMER
      count++;
      Serial.print("Entering: ");
      Serial.println(count);
      lastTrigger1 = 0;
      lastTrigger2 = 0;
      //digitalWrite(redLED, LOW);
      //digitalWrite(greenLED, HIGH); // Allow next person to enter
    }
    else if (lastTrigger2 < lastTrigger1 && (lastTrigger1 - lastTrigger2) <= triggerDelay) {    //DETERMINO IL VERSO AND CONTROLLO DI STARE NEL TIMER
      count--;
      Serial.print("Exiting: ");
      Serial.println(count);
      lastTrigger1 = 0;
      lastTrigger2 = 0;
      //digitalWrite(redLED, LOW);
      //digitalWrite(greenLED, HIGH); // Allow next person to enter
    }
  }
}

