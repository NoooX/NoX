#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <string.h>
#include "Adafruit_WS2801.h"
#include "SPI.h"
#include <EEPROM.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

uint8_t dataPin  = 5;    // Yellow wire on Adafruit Pixels
uint8_t clockPin = 4;    // Green wire on Adafruit Pixels
int counter_mode_step = 0;
Adafruit_WS2801 strip = Adafruit_WS2801(38, dataPin, clockPin);
int val = 0;
int doubleColor = 0;
const char* ssid = "BUBBLES";
const char* passphrase = "BUBBLES";
String st;
String content;

ESP8266WebServer server(80);


void handleRoot() {
Serial.println("handle root..");
val = 0;
String green = server.arg(0); // read RGB arguments
String red = server.arg(1);
String blue = server.arg(2);

Serial.println(255 - red.toInt() / 4); //
Serial.println(255 - green.toInt() / 4);
Serial.println(255 - blue.toInt() / 4);

if(doubleColor == 1){
  colorWipeFirst(Color(255 - red.toInt() / 4, 255 - green.toInt() / 4, 255 - blue.toInt() / 4), 20);
  doubleColor = 2;
  Serial.println("First");
}
else if (doubleColor == 2){
  colorWipeSecond(Color(255 - red.toInt() / 4, 255 - green.toInt() / 4, 255 - blue.toInt() / 4), 20);
  doubleColor = 0;
  Serial.println("Second");
}
else 
  colorWipe(Color(255 - red.toInt() / 4, 255 - green.toInt() / 4, 255 - blue.toInt() / 4), 20);

  String webpage = ""
    "<!DOCTYPE html><html><head><title>RGB control</title><meta name='mobile-web-app-capable' content='yes' />"
"<meta name='viewport' content='width=device-width' /></head><body style='margin: 0px; padding: 0px;'>"
"<script src='http://ajax.googleapis.com/ajax/libs/jquery/2.1.3/jquery.min.js'></script>\n"
    "<body>\n"
" <select id='List'>"
  " <option>Rainbow</option>"
  " <option>RainbowCycle</option>"
  " <option>ChaseRainbow</option>"
  " <option>2_Color</option>"
" </select>"

"<button id='Start'>ClickMe</button>\n"




" <script>"
    "$( 'button#Start' ).click(function () {\n"
    "$.get('/' + $( '#List' ).val(), function( data ) {\n"
    "$( '.result' ).html( data );\n"
    "});\n"
    "});\n"
" </script>"
   
"<canvas id='colorspace'></canvas></body>"
"<script type='text/javascript'>"
"(function () {"
" var canvas = document.getElementById('colorspace');"
" var ctx = canvas.getContext('2d');"
" function drawCanvas() {"
" var colours = ctx.createLinearGradient(0, 0, window.innerWidth, 0);"
" for(var i=0; i <= 360; i+=10) {"
" colours.addColorStop(i/360, 'hsl(' + i + ', 100%, 50%)');"
" }"
" ctx.fillStyle = colours;"
" ctx.fillRect(0, 0, window.innerWidth, window.innerHeight);"
" var luminance = ctx.createLinearGradient(0, 0, 0, ctx.canvas.height);"
" luminance.addColorStop(0, '#ffffff');"
" luminance.addColorStop(0.05, '#ffffff');"
" luminance.addColorStop(0.05, 'rgba(0,0,0,0)');"
" luminance.addColorStop(0.95, '#000000');"
" luminance.addColorStop(1, '#000000');"
" ctx.fillStyle = luminance;"
" ctx.fillRect(0, 0, ctx.canvas.width, ctx.canvas.height);"
" }"
" var eventLocked = false;"
" function handleEvent(clientX, clientY) {"
" if(eventLocked) {"
" return;"
" }"
" function colourCorrect(v) {"
" return Math.round(1023-(v*v)/64);"
" }"
" var data = ctx.getImageData(clientX, clientY, 1, 1).data;"
" var params = ["
" 'r=' + colourCorrect(data[0]),"
" 'g=' + colourCorrect(data[1]),"
" 'b=' + colourCorrect(data[2])"
" ].join('&');"
" var req = new XMLHttpRequest();"
" req.open('POST', '?' + params, true);"
" req.send();"
" eventLocked = true;"
" req.onreadystatechange = function() {"
" if(req.readyState == 4) {"
" eventLocked = false;"
" }"
" }"
" }"
" canvas.addEventListener('click', function(event) {"
" handleEvent(event.clientX, event.clientY, true);"
" }, false);"
" canvas.addEventListener('touchmove', function(event){"
" handleEvent(event.touches[0].clientX, event.touches[0].clientY);"
"}, false);"
" function resizeCanvas() {"
" canvas.width = window.innerWidth;"
" canvas.height = window.innerHeight;"
" drawCanvas();"
" }"
" window.addEventListener('resize', resizeCanvas, false);"
" resizeCanvas();"
" drawCanvas();"
" document.ontouchmove = function(e) {e.preventDefault()};"
" })();"
"</script></html>";


server.send(200, "text/html", webpage);
  
}


void setup(void){
  pinMode(2, OUTPUT);
  randomSeed(10);
  strip.begin();
  strip.show();

  //server.on("/", handleRoot);
  
    server.on("/RainbowCycle", [](){
    val = 2;
    Serial.println("RainbowCycle");
    rainbowCycle(20);
  });
  
  server.on("/Rainbow", [](){
    val = 3;
    Serial.println("Rainbow");
    rainbow(20);  
  });
    server.on("/ChaseRainbow", [](){
    val = 4;
    Serial.println("ChaseRainbow");
    theaterChaseRainbow(50);
  });
  
    server.on("/2_Color", [](){
     Serial.println("2_Color");
     doubleColor = 1;
  });

  server.begin();
  Serial.println("HTTP server started");
  
  Serial.begin(115200);
  EEPROM.begin(512);
  delay(10);
  Serial.println();
  Serial.println();
  Serial.println("Startup");
  // read eeprom for ssid and pass
  Serial.println("Reading EEPROM ssid");
  String esid;
  for (int i = 0; i < 32; ++i)
    {
      esid += char(EEPROM.read(i));
    }
  Serial.print("SSID: ");
  Serial.println(esid);
  Serial.println("Reading EEPROM pass");
  String epass = "";
  for (int i = 32; i < 96; ++i)
    {
      epass += char(EEPROM.read(i));
    }
  Serial.print("PASS: ");
  Serial.println(epass);  
  if ( esid.length() > 1 ) {
      // test esid 
      //WiFi.mode(WIFI_AP_STA);
      //WiFi.disconnect();
      //delay(100);
      WiFi.begin(esid.c_str(), epass.c_str());
      WiFi.config(IPAddress(192,168,8,187), IPAddress(192,168,8,1), IPAddress(255,255,255,0),IPAddress(192,168,8,1));
      if (testWifi()) {
        launchWeb(0);
        return;
      }
  }
  setupAP();
/*  
  WiFi.begin(ssid, password);
  WiFi.config(IPAddress(192,168,1,187), IPAddress(192,168,1,1), IPAddress(255,255,255,0),IPAddress(192,168,1,1));
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }
  */
  #if defined (__AVR_ATtiny85__)
    if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
  #endif

  

}
void loop(void){

  server.handleClient();
  switch (val){
        case 1: 
        {
          float mult = random(4, 14);
          mult = mult / 10;
          colorWipe(Color(42*mult,156*mult,0), 0);
          delay(random(200, 500));
          break;
        }
        case 2:
        {
           rainbowCycle(20);
          break;
        }
        case 3:
        {
           rainbow(20);
           break;
        }
        case 4:
        {
           theaterChaseRainbow(50);;
           break;
        }
        default:
        break;
      }
}

uint32_t Color(byte g, byte r, byte b){
  uint32_t c;
  c = r;
  c <<= 8;
  c |= g;
  c <<= 8;
  c |= b;
  return c;
}

void colorWipe(uint32_t c, uint8_t wait) {
  int i;
  
  for (i=0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
      strip.show();
      delay(wait);
  }
}

void colorWipeFirst(uint32_t c, uint8_t wait) {
  int i;
  
  for (i=0; i < strip.numPixels()/2; i++) {
      strip.setPixelColor(i, c);
      strip.show();
      delay(wait);
  }
}

void colorWipeSecond(uint32_t c, uint8_t wait) {
  int i;
  
  for (i=strip.numPixels()/2; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
      strip.show();
      delay(wait);
  }
}

//void rainbow(uint8_t wait) {
//  uint16_t i, j;
//
//  for(j=0; j<256; j++) {
//    for(i=0; i<strip.numPixels(); i++) {
//      strip.setPixelColor(i, Wheel((i+j) % 255));
//      server.handleClient();
//      if (val == 0)
//        return;
//    }
//    strip.show();
//    delay(wait);
//  }
//}

void rainbow(uint8_t wait) {
  uint32_t color = Wheel(counter_mode_step);
  for(uint16_t i=0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, color);
    server.handleClient();
    if (val == 0)
      return;
  }
  strip.show();

  counter_mode_step = (counter_mode_step + 1) % 256;

  delay(wait);
}
uint32_t Wheel(byte WheelPos)
{
  if (WheelPos < 85) {
   return Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if (WheelPos < 170) {
   WheelPos -= 85;
   return Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
   WheelPos -= 170; 
   return Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) % 255));
      server.handleClient();
      if (val == 0)
      return;
    }
    strip.show();
    delay(wait);
  }
}

void theaterChaseRainbow(uint8_t wait) {
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
        server.handleClient();
        if (val == 0)
           return;
      }
      strip.show();

      delay(wait);

      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

void theaterChase(uint32_t c, uint8_t wait) {
  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, c);    //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

bool testWifi(void) {
  int c = 0;
  Serial.println("Waiting for Wifi to connect");  
  while ( c < 20 ) {
    if (WiFi.status() == WL_CONNECTED) { return true; } 
    delay(500);
    Serial.print(WiFi.status());    
    c++;
  }
  Serial.println("");
  Serial.println("Connect timed out, opening AP");
  return false;
} 


void setupAP(void) {
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
  int n = WiFi.scanNetworks();
  Serial.println("scan done");
  if (n == 0)
    Serial.println("no networks found");
  else
  {
    Serial.print(n);
    Serial.println(" networks found");
    for (int i = 0; i < n; ++i)
     {
      // Print SSID and RSSI for each network found
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));
      Serial.print(")");
      Serial.println((WiFi.encryptionType(i) == ENC_TYPE_NONE)?" ":"*");
      delay(10);
     }
  }
  Serial.println(""); 
  st = "<ol>";
  for (int i = 0; i < n; ++i)
    {
      // Print SSID and RSSI for each network found
      st += "<li>";
      st += WiFi.SSID(i);
      st += " (";
      st += WiFi.RSSI(i);
      st += ")";
      st += (WiFi.encryptionType(i) == ENC_TYPE_NONE)?" ":"*";
      st += "</li>";
    }
  st += "</ol>";
  delay(100);
  WiFi.softAP(ssid, passphrase, 6);
  Serial.println("softap");
  launchWeb(1);
  Serial.println("over");
}

void launchWeb(int webtype) {
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("Local IP: ");
  Serial.println(WiFi.localIP());
  Serial.print("SoftAP IP: ");
  Serial.println(WiFi.softAPIP());
  createWebServer(webtype);
  // Start the server
  server.begin();
  Serial.println("Server started"); 
  colorWipe(Color(0, 255, 0), 50);
   Serial.println("Start Blue");
}

void createWebServer(int webtype)
{

  if ( webtype == 1 ) {
    server.on("/", []() {
        IPAddress ip = WiFi.softAPIP();
        String ipStr = String(ip[0]) + '.' + String(ip[1]) + '.' + String(ip[2]) + '.' + String(ip[3]);
        content = "<!DOCTYPE HTML>\r\n<html>Hello from ESP8266 at ";
        content += ipStr;
        content += "<p>";
        content += st;
        content += "</p><form method='get' action='setting'><label>SSID: </label><input name='ssid' length=32><input name='pass' length=64><input type='submit'></form>";
        content += "</html>";
        server.send(200, "text/html", content);  
    });
    server.on("/setting", []() {
        String qsid = server.arg("ssid");
        String qpass = server.arg("pass");
        if (qsid.length() > 0 && qpass.length() > 0) {
          Serial.println("clearing eeprom");
          for (int i = 0; i < 96; ++i) { EEPROM.write(i, 0); }
          Serial.println(qsid);
          Serial.println("");
          Serial.println(qpass);
          Serial.println("");

          Serial.println("writing eeprom ssid:");
          for (int i = 0; i < qsid.length(); ++i)
            {
              EEPROM.write(i, qsid[i]);
              Serial.print("Wrote: ");
              Serial.println(qsid[i]); 
            }
          Serial.println("writing eeprom pass:"); 
          for (int i = 0; i < qpass.length(); ++i)
            {
              EEPROM.write(32+i, qpass[i]);
              Serial.print("Wrote: ");
              Serial.println(qpass[i]); 
            }    
          EEPROM.commit();
          content = "<!DOCTYPE HTML>\r\n<html>";
          content += "<p>saved to eeprom... reset to boot into new wifi</p></html>";
        } else {
          content = "Error";
          Serial.println("Sending 404");
        }
        server.send(200, "text/html", content);
    });
  } else if (webtype == 0) {
      Serial.println("Web = 0");
      server.on("/", handleRoot);      
      server.on("/cleareeprom", []() {
      content = "<!DOCTYPE HTML>\r\n<html>";
      content += "<p>Clearing the EEPROM</p></html>";
      server.send(200, "text/html", content);
      Serial.println("clearing eeprom");
      for (int i = 0; i < 96; ++i) { EEPROM.write(i, 0); }
      EEPROM.commit();
    });
  }
}
