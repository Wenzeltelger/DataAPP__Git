#include <WiFi.h>
#include <FirebaseESP32.h>
#include <HCSR04.h>

//#include "DHT.h"

//Ultrasónico
byte Trigger = 2;   //Pin digital 2 para el Trigger del sensor
byte Echo = 4;   //Pin digital 3 para el Echo del sensor

//Direccion de internet
#define ssid ""
//contraseña de internet
#define pass ""

#define led1 33
#define led2 32

#define led3 19

//URL de Firebase
#define URL ""
//Secreto de Firebase
#define secreto ""

FirebaseData myFirebaseData;
FirebaseJson myJson;
FirebaseJsonData myJsonData;

String estados, estado1, estado2, estado3, estado4, estados2;

int intervalo = 1000;
unsigned long actual = 0, ultimo = 0;

//float T, H;
boolean estado5;

//DHT dht(4, DHT11);

int Sensor = 35;


void setup() {

  Serial.begin(115200);
  //dht.begin();
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);

//Ultrasonico
  HCSR04.begin(Trigger, Echo);

  
  Serial.print("Conectando a la red: ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);
  while(WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(50);
  }
  Firebase.begin(URL, secreto);
  Firebase.reconnectWiFi(true);
  Serial.println("Conectado con exito");
}

void loop() {
  
  actual = millis();
  if(actual - ultimo >= intervalo) {
    ultimo = actual;    
    
  double* distances = HCSR04.measureDistanceCm();
  int humedad = analogRead(Sensor);

    //T = dht.readTemperature();
    //H = dht.readHumidity();
    
float T = distances[0];
    
    Firebase.set(myFirebaseData, "/LECTURAS/temp", T);  

    Firebase.set(myFirebaseData, "/LECTURAS2/hum", humedad);  


    if( T <= 30){
      estado5 = 1;
    Firebase.set(myFirebaseData, "/ALARMA/Distancia", estado5);  
  Serial.println(estado3);
    }else{
      estado5 = 0;
      Firebase.set(myFirebaseData, "/ALARMA/Distancia", estado5);  
  Serial.println(estado3);

    }
    
    //Firebase.set(myFirebaseData, "/LECTURAS/hum", H);
  }

  

  Firebase.get(myFirebaseData, "/BOTONLED1");
  estados = myFirebaseData.jsonString();
  myJson.setJsonData(estados);
  myJson.get(myJsonData, "/Valor");
  estado1 = myJsonData.stringValue;

  Firebase.get(myFirebaseData, "/BOTONLED2");
  estados = myFirebaseData.jsonString();
  myJson.setJsonData(estados);
  myJson.get(myJsonData, "/Valor");
  estado2 = myJsonData.stringValue;

  if(estado1 == "true"){
    digitalWrite(led1, 1);
    digitalWrite(led3, 1);
  }else{
    digitalWrite(led1, 0);
    digitalWrite(led3, 0);
  }

    if(estado2 == "true"){
    digitalWrite(led2, 1);
  }else{
    digitalWrite(led2, 0);
  }

}
