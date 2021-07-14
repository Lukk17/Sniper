#include "header.h"

int LED = 12;    // pin D6
int sensor = A0; //pin A0

String message = "";

// Set your access point network credentials
const char *ssid = "ESP";
// password have to be 8+ lenght 
// if not SSID will be cahnged to Fairy(something) and connection will be hanging on "Obtaining IP.."
const char *password = "12345678";

// Initialize LittleFS
void initFS() {
  if (!LittleFS.begin()) {
    Serial.println("An error has occurred while mounting LittleFS");
  }
  else{
   Serial.println("LittleFS mounted successfully");
  }
}

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

// Create a WebSocket object
AsyncWebSocket ws("/ws");

void notifyClients(String sliderValues) {
  Serial.println("Notifying clients..");
  ws.textAll(sliderValues);
}

void handleWebSocketMessage(void *arg, uint8_t *data, size_t len) {
  AwsFrameInfo *info = (AwsFrameInfo*)arg;
  if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
    data[len] = 0;
    message = (char*)data;
    Serial.println("Socket message: " + message);
  }
}

void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {
  switch (type) {
    case WS_EVT_CONNECT:
      Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
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

void initWebSocket() {
  ws.onEvent(onEvent);
  server.addHandler(&ws);
  Serial.println("WebSocket initialized.");
}

void setup()
{
  Serial.begin(115200);
  pinMode(LED, OUTPUT);
  pinMode(sensor, INPUT);

  // Serial port for debugging purposes
  Serial.println();

  // Setting the ESP as an access point
  Serial.println("Setting AP..");

  // WiFi.mode(WIFI_AP);

  // Remove the password parameter, if you want the AP (Access Point) to be open
  WiFi.softAP(ssid, password);

  Serial.println("Credentials:");
  Serial.println("SSID: " + WiFi.softAPSSID());
  Serial.println("PSW: " + WiFi.softAPPSK());

  Serial.println();

  IPAddress IP = WiFi.softAPIP();
  Serial.println("AP IP address: " + IP.toString());
  Serial.println("Local IP address: " + WiFi.localIP().toString());
  Serial.println();

  initFS();
  initWebSocket();  

   // Web Server Root URL
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS, "/index.html", "text/html");
  });
  
  server.serveStatic("/", LittleFS, "/");

  // Start server
  server.begin();
}

void loop()
{
  WiFi.status();
  delay(50);

  int value = analogRead(sensor);
  if (value < 1020)
  {
    notifyClients(JSON.stringify("Sensor value: " + String(value)));

    Serial.println("sensor value:");
    Serial.println(value);
    digitalWrite(LED, HIGH);
    delay(100);
    digitalWrite(LED, LOW);
  }
}
