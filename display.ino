#include "display.h"

#define MAX_VAL 906

//Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire);

void drawSplash() {
  display.clearDisplay();
  display.setTextSize(2); // Draw 2X-scale text
  display.setTextColor(WHITE);
  display.setCursor(1, 0);
  display.println(F("Unity\n Materials"));

  display.drawLine(5, 32, 123, 32, WHITE);

  display.setCursor(0, 40);
  display.println(F("ppump 0.1"));
  display.display();
  delay(3000);

  display.fillRect(0, 0, display.width(), display.height(), WHITE);
  display.display();
  delay(10);
  display.clearDisplay();
  display.display();
}

void drawMenu(int value, int state, float total = 0, bool pumping = false) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);

  display.setCursor(1, 0);
  display.print("SSID: ");
  display.println(WiFi.SSID());
  display.print("IP: ");
  display.println(WiFi.localIP());

  int sel = 0;

  if (state == 0) {           // base menu
    sel = value / (MAX_VAL / 3.3);

    display.setCursor(20, 17);
    display.setTextSize(2);
    display.println("Manual");
    display.setCursor(20, 32);
    display.println("Program");
    display.setCursor(20, 47);
    display.println("Remote");

    display.setCursor(2, 15 * sel + 17);
    display.print(">");

  } else if (state == 1) {     // manual control
    //display.setCursor(1, 0);
    display.print("Total pumped: ");
    display.print(total);
    display.println(" mL");
    //display.println(F("Total pumped: 0 mL"));

    display.setTextSize(2);
    display.setCursor(1, 30);
    display.print((float)value/MAX_VAL*60, 1);
    display.setTextSize(1);
    display.println(" mL/min");

    if(pumping){
      display.fillTriangle(100, 30, 120, 43, 100, 55, WHITE);
    }else{
      display.fillRect(100, 30, 10, 25, WHITE);
      display.fillRect(115, 30, 10, 25, WHITE); 
    }

    //display.setTextSize(1);
    //display.setCursor(1, 50);
    //display.print("Long press button to return.");
  } else if (state == 2) {     // program selection
    display.println("Program mode");
    display.setTextSize(2);
    display.print((float)value/MAX_VAL*60, 1);
    display.setCursor(0,40);
    display.setTextSize(1);
    display.println("mL");
    display.println("Click to pump");

    if(pumping){
      display.fillTriangle(85, 20, 110, 33, 85, 45, WHITE);
    }else{
      display.fillRect(85, 20, 10, 25, WHITE);
      display.fillRect(100, 20, 10, 25, WHITE); 
    }
    //display.setCursor(1, 50);
    //display.print("Long press button to return.");
  } else if (state == 3) {     // Remote mode
    display.setCursor(20, 17);
    display.setTextSize(2);
    display.println("Remote Mode");

    //display.setCursor(1, 50);
    //display.print("Long press button to return.");
  }

  display.display();
}

void drawStatus(float rate, float total, bool paused) {
  char* stotal, srate;
  rate = rate / 9.10;
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);

  display.setCursor(1, 0);
  display.print("Total pumped: ");
  display.print(total);
  display.println(" mL");
  //display.println(F("Total pumped: 0 mL"));

  display.setTextSize(2);
  display.setCursor(1, 30);
  display.print(rate);
  //Serial.print("rate str: \"");
  //sprintf(srate, "Rate: %0.0f", rate);
  display.println(" mL/min");

  display.display();
}
