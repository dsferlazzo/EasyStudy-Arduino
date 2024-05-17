// SOTTO-PROGRAMMA CONTENENTE LA PARTE DI CONTROLLO DI TEMPERATURA E DI ARIA (ATTUALMENTE CON IMPLEMENTO DEL PULSANTE CHE ATTIVA IL SERVOMOTORE)
//
//RICONTROLLARE BENE I PIN, I COLLEGAMENTI, E REIMPOSTARE I PIN NEL PROGRAMMA
#include <Servo.h>

const int tempSensorPin = A2;
const int fanPin1 = 11;
const int fanPin2 = 10;
const int servoPin = 8;
const int airQualityButton = 9;   //TODO DA RICONTROLLARE IL PIN DEL PULSANTE

const int airQualityThreshold = 300;	//DA CERCARE ED INSERIRE THRESHOLD EFFETTIVO PER LA QUALITA DELL'ARIA	
Servo windowServo;
int servoState = 0;

int statoPrecedenteServo = 0; //0 FINESTRA CHIUSA, 1 FINESTRA APERTA
int buttonState = 0;  //CONTIENE LO STATO DEL PULSANTE ARIA, PER L'ATTIVAZIONE DEL SERVOMOTORE CON IL PULSANTE
void setup()
{
  pinMode(fanPin1, OUTPUT);
  pinMode(fanPin2, OUTPUT);
  
  
  windowServo.attach(servoPin);
  windowServo.write(0);	
  Serial.begin(9600);
  
}

void loop()
{
  float temperature = readTemperature();
  //delay(2000); //PER EVITARE IL GHOSTING
  //int airQuality = analogRead(airQualitySensor);    //INT O FLOAT? (INDIFFERENTE)
  
  handleTemperature(temperature);
  handleAirQualityButton();
  delay(1000);
}

void handleTemperature(float temperature) {
  if (temperature > 25.0) {
    digitalWrite(fanPin1, HIGH);
    digitalWrite(fanPin2, LOW);
  } else if (temperature < 22.0) {
    digitalWrite(fanPin1, LOW);
    digitalWrite(fanPin2, HIGH);
  } else {
    digitalWrite(fanPin1, LOW);
    digitalWrite(fanPin2, LOW);
  }
}

float readTemperature() {
  int reading = analogRead(tempSensorPin);
  float voltage = reading * 5.0 / 1023.0;
  float temperature = (voltage) * 100.0;	//DA RIMUOVERE -0.5 IN LM35
  Serial.print("Temperatura: ");
  Serial.println(temperature);
  return temperature;
}

void handleAirQuality(int airQuality) {
  if (airQuality > airQualityThreshold && statoPrecedenteServo == 0) {
    Serial.println("Apertura finestra");
    windowServo.write(0);
    statoPrecedenteServo = 1 - statoPrecedenteServo;
  } else if(airQuality < airQualityThreshold && statoPrecedenteServo == 1){
    windowServo.write(90); 
    statoPrecedenteServo = 1 - statoPrecedenteServo;
    Serial.println("Chiusura finestra");
  }
}

bool ifClick(){   //PER L'ATTIVAZIONE DEL SERVOMOTORE CON IL PULSANTE
  int prevState = buttonState;
  buttonState = digitalRead(airQualityButton);
  //Serial.println(buttonState);	//DEBUGGING
  if (buttonState > prevState){
    //Serial.println("Button click");	//DEBUGGING
    return true;
  }
  return false;
}

void handleAirQualityButton(){  //PER L'ATTIVAZIONE DEL SERVOMOTORE CON IL PULSANTE
  if(ifClick()){
    servoState = 90-servoState;
    windowServo.write(servoState);
  }
}