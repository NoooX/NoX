//#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <Adafruit_NeoPixel.h>
#include <EEPROM.h>
#include <TimeLib.h>
#include <WidgetRTC.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <RemoteDebug.h>

#define HOST_NAME "remotedebug-sample"
//#define BLYNK_PRINT Serial
//#define DEBUG 1
//#define DEBUG2
#define HOUR_PIN 4
#define MINUTE_PIN 5
#define SUN_PIN 17

Adafruit_NeoPixel strip = Adafruit_NeoPixel(15, HOUR_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip2 = Adafruit_NeoPixel(15, MINUTE_PIN, NEO_GRB + NEO_KHZ800);
BlynkTimer clockTimer;
BlynkTimer brighnessTimer;
WidgetRTC rtc;
RemoteDebug Debug;

const char* ssid = "XXXXXXX";
const char* password = "XXXXXXXX";
unsigned long previousMillis = 0;
uint32_t c = strip.Color(0, 0, 0,0);

bool AutoBrightness = true;
int hours = 0;
int minutes = 0;
uint8_t r = 0;
uint8_t g = 0;
uint8_t b = 0;
uint16_t interval = 100;
int arr[100] = {0};
unsigned long currentMillis2 = 0;
unsigned long previousMillis2 = 0;
bool ota_flag = false;

// Token
char auth[] = "b823867a3e9c4b6d868065dff1b16b8b";

void setup()
{
  // startWiFi();
  // Debug console
  Serial.begin(115200);
  EEPROM.begin(512);
  strip.begin();
  strip2.begin();
  strip.show(); // Initialize all pixels to 'off'
  strip2.show(); // Initialize all pixels to 'off'

  Blynk.begin(auth, ssid, password);

  
    String hostNameWifi = HOST_NAME;
    hostNameWifi.concat(".local");


    WiFi.hostname(hostNameWifi);

    if (MDNS.begin(HOST_NAME)) {
        Serial.print("* MDNS responder started. Hostname -> ");
        Serial.println(HOST_NAME);
    }

    MDNS.addService("telnet", "tcp", 23);

    Debug.begin(HOST_NAME); // Initiaze the telnet server

    Debug.setResetCmdEnabled(true); // Enable the reset command

    ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH)
      type = "sketch";
    else // U_SPIFFS
      type = "filesystem";

    // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
    Serial.println("Start updating " + type);
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  ArduinoOTA.begin();
  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  
  //Small animation
  sucessStart();

  // How often to sync 
  setSyncInterval(10 * 60); // Sync interval in seconds (10 minutes)

  getTime();
  ReadLight();
  updateTime(hours, minutes, c);

  // Get digital time every 10 seconds
  clockTimer.setInterval(10000L, getTime);
  // Get brightness every 5 seconds
  brighnessTimer.setInterval(10L, ReadLight);

  
}

void loop()
{
  unsigned long currentMillis = millis();
  if (ota_flag)
  {
   uint16_t time_elapsed = 0;
    while(time_elapsed < 15000){
      #ifdef DEBUG
      Serial.println("Waiting for OTA...");
      #endif
      strip.setPixelColor(0, 0, 100 ,0);
      strip2.setPixelColor(0, 0, 100, 0);
      strip.show();
      strip2.show();
      ArduinoOTA.handle();
      time_elapsed = millis();
      delay(10);
    }
    ota_flag = false;
  }
  Blynk.run();
  clockTimer.run();
  brighnessTimer.run();

  // Display digital clock every 5 seconds
  if(currentMillis - previousMillis >= interval)
  {
    previousMillis = currentMillis;
    updateTime(hours, minutes, c);
  }
Debug.handle();

yield();
}
