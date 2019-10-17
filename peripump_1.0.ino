#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <FS.h>

#include "display.h"

#define MAX_VAL 906

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire);

ESP8266WiFiMulti wifiMulti;
ESP8266WebServer server(80);

const int button = D4; // push button D4
const int dial = A0;   // rheostat
int sensorValue = 0;
// D1, D2, 5V and gnd => screen
const int motor = D3; // transistor pwm port

int menuState = 0;
float totalvol = 0;
int longpress = 0;
bool pumping = false;
int activeprog = -1;

String getContentType(String filename);
bool handleFileRead(String path);
float pump(float pumprate);

const int led = 13;

void(* resetFunc) (void) = 0; //declare reset function @ address 0

void handleRoot() {
  digitalWrite(led, 1);
  server.send(200, "text/plain", "hello from esp8266!");
  digitalWrite(led, 0);
}

void handleNotFound() {
  digitalWrite(led, 1);
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
  digitalWrite(led, 0);
}

void setup(void) {
  pinMode(led, OUTPUT);
  pinMode(button, INPUT_PULLUP);
  
  digitalWrite(led, 0);
  Serial.begin(115200);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  display.display();
  
  
  wifiMulti.addAP("XXX", "YYY"); //Wifi info

  Serial.println("Connecting...");
  while (wifiMulti.run() != WL_CONNECTED){
    delay(250);
    Serial.print('.');
  }
  
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(WiFi.SSID());
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  drawMenu(sensorValue, menuState, totalvol, pumping);
  
  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }

  SPIFFS.begin();

  server.onNotFound([](){
    if(!handleFileRead(server.uri()))
      server.send(404, "text/plain", "404: Not Found");
  });
  server.on("/", handleRoot);
  server.on("/inline", []() {
    server.send(200, "text/plain", "this works as well");
  });

  //server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loop(void) {
  server.handleClient();
  MDNS.update();

  sensorValue = analogRead(dial);
  
  if (digitalRead(button) == HIGH && longpress >= 10){
    //Serial.println("Box button released");
    if (menuState == 1 || menuState == 2 || menuState == 3){ // manual menu
      Serial.println("Longpress! Go back");
      menuState = 0;
      longpress = 0;
    }
  }else if(digitalRead(button) == LOW){   // Front button pressed
    //Serial.println("Pushed box button");
    longpress++;
    //Serial.println(longpress);
    
    if(menuState == 0){ // base menu
      if((float)sensorValue/MAX_VAL*3.3 <= 1){
        menuState = 1;
        Serial.println("pressed at menu state 1");
      }else if ((float)sensorValue/MAX_VAL*3.3 > 1 && (float)sensorValue/MAX_VAL*3.3 <= 2){
        menuState = 2;
        Serial.println("pressed at menu state 2");
      }else{
        menuState = 3;
        Serial.println("pressed at menu state 3");
      }
    }else if (menuState == 1 && longpress <= 1){ // manual menu
      pumping = !pumping;
      if(pumping)
        pump(sensorValue);
      else
        pump(0);
    }else if (menuState == 2 && longpress <= 1){ // program menu
        pumping = !pumping;
    }else if (menuState == 3){ // remote menu

    }
  }else if(digitalRead(button) == HIGH && longpress > 0){
    longpress=0;
  }
  
  
  //Serial.println(sensorValue);
  if(digitalRead(16) == LOW){
    Serial.println("Pushed Reset button");
    resetFunc();
  }

  display.clearDisplay();
  drawMenu(sensorValue, menuState, totalvol, pumping);
  //Serial.println(sensorValue);
  
  delay(100);
}


float pump(float pumprate){
  Serial.println("Pumping");
  if (pumprate = 0)
    analogWrite(motor, 0);
  else
    analogWrite(motor, pumprate);
}
