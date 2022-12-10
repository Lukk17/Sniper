#include "header.h"
#include <map>

//  extern means it is defined in other cpp file
extern int redLedPin;
extern int greenLedPin;
extern int sensorPin;

extern unsigned long lastNotificationTime;
extern unsigned long currentTime;
extern unsigned int websocketInterval;

extern String changeLedPin;
extern IPAddress IP;

int getPinState(int pin);

void switchState(int pin, const int state);
void changeState(int pin, const int state);

void turnOff(int pin);
void turnOn(int pin);

void setupLedPins();
void setupSensorAsInput();

void setupWifi();
void setupServer();

void initWebSocket();
void startServer();
void notifyClients(String notification);
