#include <SPI.h>
#include <LoRa.h>
#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "";
const char* password = "";

void setup() {
  
  Serial.begin(9600);
  while (!Serial);

  delay(4000);
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
 
  Serial.println("Connected to the WiFi network");
  
  
  Serial.println("LoRa Receiver");
  LoRa.setPins(18,14,26);
  
  if (!LoRa.begin(868E6)) {//Es la frequencia de europa
    Serial.println("Starting LoRa failed!");
    while (1);
  }
}

void loop() {
  //RECIVING A PACKET
  String packet="";
  // try to parse packet
  int packetSize = LoRa.parsePacket();//Mira si le ha llegado algo
  if (packetSize) { //Si le ha llegado...
    // received a packet
    Serial.print("Received packet: \n");

    // read packet
    while (LoRa.available()) {
      packet = LoRa.readString();
    }
    
    // print the packet
    Serial.println(packet);
    
    //SEND POST REQUEST
        if ((WiFi.status() == WL_CONNECTED)) { //Check the current connection status 
      HTTPClient http;
   
      http.begin("http://jaimesarrion.freemyip.com:10000/mediciones"); //Specify the URL
      http.addHeader("Content-Type", "text/plain");
      int httpResponse = http.POST(packet);     //Make the request
   
      if (httpResponse > 0) { //Check for the returning code
          String response = http.getString();
          Serial.println(httpResponse);
          Serial.println(response);
      }
      else {
        Serial.println("Error on HTTP request");
      }
      http.end(); //Free the resources
    }
    delay(1000);
  }


  
}
