#include "service/sensorHeader.h"

int sensorPin = A0; // pin A0

void setupSensorAsInput()
{
    pinMode(sensorPin, INPUT);
}