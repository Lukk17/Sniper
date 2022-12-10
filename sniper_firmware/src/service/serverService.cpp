#include "service/serverHeader.h"

void initialNotification();
void handleWebSocketMessage(void *arg, uint8_t *data, size_t len);
void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len);

int webServerPort = 80;

// Create AsyncWebServer object on port 80
AsyncWebServer server(webServerPort);

// Create a WebSocket object
AsyncWebSocket ws("/ws");

unsigned long lastNotificationTime = 0;
unsigned int websocketInterval = 400;

String message = "";
String updateInterval = "updateInterval ";
String updateStats = "updateStats";

void setupServer()
{
  // Web Server Root URL
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(LittleFS, "/index.html", "text/html"); });

  server.serveStatic("/", LittleFS, "/");
}

void initWebSocket()
{
  ws.onEvent(onEvent);
  server.addHandler(&ws);
  Serial.println("WebSocket initialized.");
  Serial.println("WebSocket interval: " + String(websocketInterval));
}

void startServer()
{
  server.begin();
}

void notifyClients(String notification)
{
  Serial.println("Notifying clients..");
  ws.textAll(notification);
}

void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len)
{
  switch (type)
  {
  case WS_EVT_CONNECT:
    Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
    initialNotification();
    break;

  case WS_EVT_DISCONNECT:
    Serial.printf("WebSocket client #%u disconnected\n", client->id());
    break;

  case WS_EVT_DATA:
    handleWebSocketMessage(arg, data, len);
    break;

  case WS_EVT_PONG:
  case WS_EVT_ERROR:
    break;
  }
}

void initialNotification()
{
  StaticJsonDocument<256> infoJson;

  infoJson["SSID"] = WiFi.softAPSSID();
  infoJson["WiFi pass"] = WiFi.softAPPSK();
  infoJson["AP IP address"] = IP.toString();
  infoJson["WebServer port"] = webServerPort;
  infoJson["Websocket interval"] = websocketInterval;
  infoJson["Last notification time[s]"] = labs(millis() - lastNotificationTime) / 1000;
  infoJson["Red LED pin"] = redLedPin;
  infoJson["Green LED pin"] = greenLedPin;
  infoJson["Sensor pin"] = sensorPin;

  String info;
  serializeJson(infoJson, info);
  
  notifyClients("Welcome: " + info);
}

void handleWebSocketMessage(void *arg, uint8_t *data, size_t len)
{
  AwsFrameInfo *info = (AwsFrameInfo *)arg;
  if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT)
  {
    data[len] = 0;
    message = (char *)data;
    Serial.println("Socket message: " + message);

    if (message.startsWith(updateInterval))
    {
      message.replace(updateInterval, "");
      int newInterval = abs(message.toInt());

      websocketInterval = newInterval;
      Serial.println("WebSocket interval changed to: " + String(websocketInterval) + "ms.");
    }

    else if (message.startsWith(updateStats))
    {
      initialNotification();
      Serial.println("Initial stats notification sent.");
    }

    else if (message.startsWith(changeLedPin))
    {
      int newLedPin = getPinNumberFromMessage(String(message));

      int state = getPinState(redLedPin);
      turnOff(redLedPin);

      redLedPin = newLedPin;
      setPinAsOutput(redLedPin);

      changeState(redLedPin, state);

      Serial.println("LED pin changed to: " + String(redLedPin) + ".");
    }
  }
}
