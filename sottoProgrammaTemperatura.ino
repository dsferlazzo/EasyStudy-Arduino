// SOTTO-PROGRAMMA CONTENENTE LA PARTE DI CONTROLLO DELLA TEMPERATURA
//
//RICONTROLLARE BENE I PIN, I COLLEGAMENTI, E REIMPOSTARE I PIN NEL PROGRAMMA

const int tempSensorPin = A2;
const int fanPin1 = 11;
const int fanPin2 = 10;

void setup()
{
  pinMode(fanPin1, OUTPUT);
  pinMode(fanPin2, OUTPUT);
  Serial.begin(9600);
  //Serial.println("Prova console");  //DEBUGGING
}

void loop()
{
  float temperature = readTemperature();
  handleTemperature(temperature);
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
  Serial.println(temperature);
  return temperature;
}
