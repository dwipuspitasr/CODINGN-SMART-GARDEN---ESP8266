#define BLYNK_TEMPLATE_ID "TMPL6mgJGO4xD"
#define BLYNK_TEMPLATE_NAME "Dwi Puspita sari"
#define BLYNK_AUTH_TOKEN "8o8raPglkRZTMORZbTZjcrCtPCWfscqQ"

#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "den";  // type your wifi name
char pass[] = "dwikps123";  // type your wifi password

int GasSensor = A0;
int sensorThreshold = 100;
#define Buzzer D4
#define DHTPIN D5           // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11       // DHT 11
#define sensor D7
#define waterPump D3

bool Relay = 0;
DHT dht(DHTPIN, DHTTYPE);
BlynkTimer timer;

void setup(){
  Serial.begin(115200);
  pinMode(waterPump, OUTPUT);
  pinMode(sensor, INPUT);
  pinMode(GasSensor, INPUT);
  pinMode(Buzzer, OUTPUT);
  digitalWrite(waterPump, HIGH);
  Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
  dht.begin();
  timer.setInterval(500L, sendSensor);
  timer.setInterval(500L, soilMoistureSensor);
}

BLYNK_WRITE(V0) {
  Relay = param.asInt();
  digitalWrite(waterPump, Relay == 1 ? LOW : HIGH);
}

void loop(){
  Blynk.run();
  timer.run();
}

void sendSensor(){
  int gasValue = analogRead(GasSensor);
  Blynk.virtualWrite(V4, gasValue);
  Serial.print("Gas Value: ");
  Serial.println(gasValue);

//PENGATURAN GAS 
    if(gasValue>=650)
    {
    digitalWrite(Buzzer, HIGH);
    Serial.println("WARNING! Gas is High");
    delay(400);
  }
  else
  {
    digitalWrite(Buzzer, LOW);
    Blynk.logEvent("gas_alert","Gas is High");
    Serial.println("Gas Normal");
    delay(400);
  }
//PENGATURAN DHT   
  float h = dht.readHumidity();
  float t = dht.readTemperature(); // or dht.readTemperature(true) for Fahrenheit
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  Blynk.virtualWrite(V1, t);
  Blynk.virtualWrite(V2, h);
  
  Serial.print("Suhu : ");
  Serial.print(t);
  Serial.print(" || Kelembapan : ");
  Serial.println(h);

  if(t > 30){
    Blynk.logEvent("notifikasi","Suhu diatas 30 Derajat celcius");
  }
}

//PENGATURAN SENSOR TANAH
void soilMoistureSensor() {
  int sensorValue = analogRead(sensor);
  String status;
  
  Serial.print("soilMoisture: ");
  Serial.println(sensorValue);

  if(sensorValue <= 100) {
    status = "Tanah Lembab";
    Serial.println("Tanah Lembab");
  } else {
    status = "Tanah Kering";
    Serial.println("Tanah Kering");
  }

  Blynk.virtualWrite(V3, status);
}
