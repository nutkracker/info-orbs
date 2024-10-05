// main.cpp

#include "core/wifiWidget.h"
#include "widgetSet.h"
#include "screenManager.h"
#include "widgets/clockWidget.h"
#include "widgets/weatherWidget.h"
#include "widgets/webDataWidget.h"
#include <Arduino.h>
#include <Button.h>
#include <globalTime.h>
#include <config.h>
#include <widgets/stockWidget.h>
#include <widgets/mqttWidget.h>   // MQTT
#include <widgets/XmasWidget.h>   // Xmas

TFT_eSPI tft = TFT_eSPI();

// Button states
bool lastButtonOKState = HIGH;
bool lastButtonLeftState = HIGH;
bool lastButtonRightState = HIGH;

// Initialize buttons
Button buttonOK(BUTTON_OK);
Button buttonLeft(BUTTON_LEFT);
Button buttonRight(BUTTON_RIGHT);

// Initialize global time
GlobalTime *globalTime;

// Additional variables
String connectingString{""};

WifiWidget *wifiWidget{ nullptr };

int connectionTimer{0};
const int connectionTimeout{10000};
bool isConnected{true};

// Function to handle JPEG rendering callbacks
bool tft_output(int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t *bitmap) {
    if (y >= tft.height())
        return 0;
    tft.pushImage(x, y, w, h, bitmap);
    return 1;
}

ScreenManager* sm;
WidgetSet* widgetSet;

// Instantiate MQTTWidget pointer globally if needed
MQTTWidget* mqttWidgetInstance = nullptr;

void setup() {

  // Initialize buttons
  buttonLeft.begin();
  buttonOK.begin();
  buttonRight.begin();

  // Initialize serial communication
  Serial.begin(115200);
  Serial.println();
  Serial.println("Starting up...");

  // Initialize ScreenManager and WidgetSet
  sm = new ScreenManager(tft);
  sm->selectAllScreens();
  sm->getDisplay().fillScreen(TFT_WHITE);
  sm->reset();
  widgetSet = new WidgetSet(sm);

  // Set JPEG rendering callback
  TJpgDec.setSwapBytes(true); // JPEG rendering setup
  TJpgDec.setCallback(tft_output);

#ifdef GC9A01_DRIVER
  Serial.println("GC9A01 Driver");
#endif
#ifdef ILI9341_DRIVER
  Serial.println("ILI9341 Driver");
#endif
#if HARDWARE == WOKWI
  Serial.println("Wokwi Build");
#endif

  // Initialize busy pin
  pinMode(BUSY_PIN, OUTPUT);
  Serial.println("Connecting to: " + String(WIFI_SSID));

  // Initialize Wi-Fi widget
  wifiWidget = new WifiWidget(*sm);
  wifiWidget->setup();

  // Initialize global time
  globalTime = GlobalTime::getInstance();

  // Add widgets to the WidgetSet
  widgetSet->add(new ClockWidget(*sm));
//  widgetSet->add(new StockWidget(*sm)); // Uncomment if using StockWidget
  widgetSet->add(new WeatherWidget(*sm)); // Uncomment if using WeatherWidget

#ifdef WEB_DATA_WIDGET_URL
  widgetSet->add(new WebDataWidget(*sm, WEB_DATA_WIDGET_URL));
#endif

#ifdef MQTT_WIDGET_HOST
  // Instantiate MQTTWidget with host and port from config.h
  mqttWidgetInstance = new MQTTWidget(*sm, MQTT_WIDGET_HOST, MQTT_WIDGET_PORT);
  widgetSet->add(mqttWidgetInstance);
#endif

#ifdef MQTT_WIDGET_HOST
  widgetSet->add(new XmasWidget(*sm));
#endif

}

void loop() {
  if (!wifiWidget->isConnected()) {
    // If not connected to Wi-Fi, update and draw the Wi-Fi widget
    wifiWidget->update();
    wifiWidget->draw();
    widgetSet->setClearScreensOnDrawCurrent(); // Clear screen after Wi-Fi widget
    delay(100);
  } else {
    // If connected to Wi-Fi
    if (!widgetSet->initialUpdateDone()) {
      widgetSet->initializeAllWidgetsData();
    }
    // Update global time
    globalTime->updateTime();

    // Handle button presses
    if (buttonLeft.pressed()) {
      Serial.println("Left button pressed");
      widgetSet->prev();
    }
    if (buttonOK.pressed()) {
      Serial.println("OK button pressed");
      widgetSet->changeMode();
    }
    if (buttonRight.pressed()) {
      Serial.println("Right button pressed");
      widgetSet->next();
    }

    // Update and draw the current widget
    widgetSet->updateCurrent();
    widgetSet->drawCurrent();
  }
}
