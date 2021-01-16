#include <WiFi.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>

#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BMP280 bmp; // I2C

// Replace with your network credentials
const char* ssid     = "xxxx";
const char* password = "xxxx";

const char* server = "api.thingspeak.com";
String apiKey = "XXXX";

const char* wl_status_to_string(wl_status_t status) {
  switch (status) {
    case WL_NO_SHIELD: return "WL_NO_SHIELD";
    case WL_IDLE_STATUS: return "WL_IDLE_STATUS";
    case WL_NO_SSID_AVAIL: return "WL_NO_SSID_AVAIL";
    case WL_SCAN_COMPLETED: return "WL_SCAN_COMPLETED";
    case WL_CONNECTED: return "WiFi connected";
    case WL_CONNECT_FAILED: return "WL_CONNECT_FAILED";
    case WL_CONNECTION_LOST: return "WL_CONNECTION_LOST";
    case WL_DISCONNECTED: return "WL_DISCONNECTED";
  }
}

void setup() {

  Serial.begin(115200);
  bool status;
  delay(1000); //Take some time to open up the Serial Monitor


  if (!bmp.begin(0x76)) {
    Serial.println("Could not find a valid BMP280 sensor, check wiring!");
    while (1);
  }

  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

 
  
 
}

void loop(){

  double Temp = bmp.readTemperature();
  double Pressure = bmp.readPressure();
 // double TempTest = 25;
 // double PressureTest = 1000;
  
  Serial.println(Temp);        
  Serial.println(Pressure);
  
  // write data to thingspeak
 WiFiClient client;

  const int httpPort = 80;

  if (!client.connect(server, httpPort)) {
    Serial.println("connection failed");
    return;
  }
  else
  {
    String data_to_send = apiKey;
    data_to_send += "&field1=";
    data_to_send += String(Temp);
    data_to_send += "&field2=";
    data_to_send += String(Pressure);
    data_to_send += "\r\n\r\n";

    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: " + apiKey + "\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(data_to_send.length());
    client.print("\n\n");
    client.print(data_to_send);

    
  }

  
  delay(15000);
  
}
