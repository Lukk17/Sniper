#include "header.h"

int ledPin = 12;    // pin D6
int sensorPin = A0; // pin A0

String message = "";
String updateInterval = "updateInterval ";
String updateStats = "updateStats";
String changeLedPin = "changeLedPin ";

int webServerPort = 80;

IPAddress IP;

unsigned long lastNotificationTime = 0;
unsigned long currentTime;
unsigned int websocketInterval = 400;

// Set your access point network credentials
const char *ssid = "ESP";
// password have to be 8+ lenght
// if not SSID will be cahnged to Fairy(something) and connection will be hanging on "Obtaining IP.."
const char *password = "12345678";

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

// Create AsyncWebServer object on port 80
AsyncWebServer server(webServerPort);

// Create a WebSocket object
AsyncWebSocket ws("/ws");

void notifyClients(String notification)
{
  Serial.println("Notifying clients..");
  ws.textAll(notification);
}

void initialNotification()
{
  StaticJsonDocument<512> infoJson;

  infoJson["SSID"] = WiFi.softAPSSID();
  infoJson["WiFi pass"] = WiFi.softAPPSK();
  infoJson["AP IP address"] = IP.toString();
  infoJson["WebServer port"] = webServerPort;
  infoJson["Websocket interval"] = websocketInterval;
  infoJson["Last notification time[s]"] = labs(millis() - lastNotificationTime) / 1000;
  infoJson["LED pin"] = ledPin;
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
      Serial.println("Stats notification sent.");
    }

    else if (message.startsWith(changeLedPin))
    {
      message.replace(changeLedPin, "");
      int newLedPin = abs(message.toInt());

      ledPin = newLedPin;
      pinMode(ledPin, OUTPUT);
      Serial.println("LED pin changed to: " + String(ledPin) + ".");
    }
  }
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

void initWebSocket()
{
  ws.onEvent(onEvent);
  server.addHandler(&ws);
  Serial.println("WebSocket initialized.");
  Serial.println("WebSocket interval: " + String(websocketInterval));
}

void setup()
{
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  pinMode(sensorPin, INPUT);

  Serial.println("\n");
  Serial.println("LED pin: " + String(ledPin));
  Serial.println("Sensor pin: " + String(sensorPin));
  // Serial port for debugging purposes
  Serial.println();

  // Setting the ESP as an access point
  Serial.println("Setting AP..");

  // Remove the password parameter, if you want the AP (Access Point) to be open
  WiFi.softAP(ssid, password);

  Serial.println("SSID: " + WiFi.softAPSSID());
  Serial.println("PSW: " + WiFi.softAPPSK());

  Serial.println();

  IP = WiFi.softAPIP();
  Serial.println("AP IP address: " + IP.toString());
  Serial.println();

  initFS();
  initWebSocket();

  // Web Server Root URL
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(LittleFS, "/index.html", "text/html"); });

  server.serveStatic("/", LittleFS, "/");

  // Start server
  server.begin();
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
      notifyClients(JSON.stringify("Sensor value: " + String(value)));
      lastNotificationTime = currentTime;
    }

    Serial.println("Sensor value: " + String(value));
    digitalWrite(ledPin, HIGH);
    delay(50);
    digitalWrite(ledPin, LOW);
  }
}
