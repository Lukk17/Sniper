// declaration of functions and variables
#include "sniperHeader.h"

unsigned long currentTime;

int baud = 115200;

// Initialize LittleFS
void initFS()
{
  if (!LittleFS.begin())
  {
    Serial.println("An error has occurred while mounting LittleFS");
  }
  else
  {
    Serial.println("LittleFS mounted successfully");
  }
}

void setup()
{
  Serial.begin(baud);
  initFS();

  setupSensorAsInput();

  setupLedPins();
  setupWifi();

  initWebSocket();
  setupServer();

  startServer();
  Serial.println("Server started.");
}

void loop()
{
  currentTime = millis();
  delay(50);

  int value = analogRead(sensorPin);
  if (value < 1020)
  {
    if (labs(currentTime - lastNotificationTime) > websocketInterval)
    {
      String body;
      StaticJsonDocument<256> infoJson;

      infoJson["Sensor value"] = String(value);

      serializeJson(infoJson, body);

      notifyClients(body);
      lastNotificationTime = currentTime;
    }

    Serial.println("Sensor value: " + String(value));

    switchState(redLedPin, getPinState(redLedPin));
    switchState(greenLedPin, getPinState(greenLedPin));
  }
}
