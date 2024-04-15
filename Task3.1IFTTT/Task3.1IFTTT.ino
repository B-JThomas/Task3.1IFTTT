#include <BH1750.h>
#include <Wire.h>
#include <WiFiNINA.h>

BH1750 lightMeter;

const int DirectSunlight = 30000;
bool flag = false;

// WiFi credentials
// WiFi credentials
char ssid[] = "WIFI NAME";
char password[] = "WIFI PASSWORD";

// IFTTT Webhooks API key and event name
char apiKey[] = "APIKEY";
char eventName[] = "EVENTNAME";

WiFiClient client;

void setup() {
  Serial.begin(9600);

  // Connect to Wi-Fi
  while (WiFi.begin(ssid, password) != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  Wire.begin();
  lightMeter.begin();

  Serial.println(F("BH1750 Test begin"));
}

void loop() {
  float lux = lightMeter.readLightLevel();
  Serial.print("Light: ");
  Serial.print(lux);
  Serial.println(" lx");
  if(lux > DirectSunlight){
    if(!flag){
      //Send Allert that sun is comming
      sendIFTTTEvent(eventName);
    }
    flag = true;
  }else{
    if(flag){
      //Send Allert that sun is leaving
      sendIFTTTEvent(eventName);
    }
    flag = false;
  }
  delay(1000);
}

// Function to send a POST request to IFTTT Webhooks
void sendIFTTTEvent(char* eventName) {
  if (client.connect("maker.ifttt.com", 80)) {
    client.print(String("POST /trigger/") + eventName + "/with/key/" + apiKey + " HTTP/1.1\r\n" +
                 "Host: maker.ifttt.com\r\n" +
                 "Content-Type: application/json\r\n" +
                 "Connection: close\r\n\r\n");
    client.stop();
  }
}

//sunChange
