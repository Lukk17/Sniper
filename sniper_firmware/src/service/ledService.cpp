#include "service/ledHeader.h"

int redLedPin = 12;   // pin D6
int greenLedPin = 13; // pin D7

String changeLedPin = "changeLedPin ";

std::map<String, int> pinMapping =
    {
        {"D1", 5},
        {"D2", 4},
        {"D5", 14},
        {"D6", 12},
        {"D7", 13},
};

void setupLedPins()
{
    redLedPin = 12;   // pin D6
    greenLedPin = 13; // pin D7
    sensorPin = A0;   // pin A0
    setPinAsOutput(redLedPin);
    turnOff(redLedPin);

    setPinAsOutput(greenLedPin);
    turnOn(greenLedPin);

    Serial.println("\n");
    Serial.println("Red LED pin: " + String(redLedPin));
    Serial.println("Green LED pin: " + String(greenLedPin));
    Serial.println("Sensor pin: " + String(sensorPin));
    // Serial port for debugging purposes
    Serial.println();
}

int getPinNumberFromMessage(String message)
{
    message.replace(changeLedPin, "");
    return abs(message.toInt());
}

int getPinState(int pin)
{
    return digitalRead(redLedPin);
}

void setPinAsOutput(int pin)
{
    pinMode(pin, OUTPUT);
}

void turnOff(int pin)
{
    digitalWrite(pin, LOW);
}

void turnOn(int pin)
{
    digitalWrite(pin, HIGH);
}

void switchState(int pin, const int state)
{

    if (state == HIGH)
    {
        changeState(pin, LOW);
    }
    else
    {
        changeState(pin, HIGH);
    }
}

void changeState(int pin, const int state)
{
    switch (state)
    {
    case LOW:
        turnOff(pin);
        break;

    case HIGH:
        turnOn(pin);
        break;

    default:
        break;
    }
}
