#include "sniperHeader.h"

// Set your access point network credentials
const char *ssid = "Sniper";
// password have to be 8+ lenght
// if not SSID will be cahnged to Fairy(something) and connection will be hanging on "Obtaining IP.."
const char *password = "bulletproof";

IPAddress IP;

void setupWifi()
{
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
}