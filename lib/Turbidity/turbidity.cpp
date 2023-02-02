#include <Arduino.h>
#include <turbidity.h>

void Turbidity_Setup(int sensorPin)
{
    pinMode(sensorPin, INPUT);
}
float Turbidity_ReadSensor(int sensorPin)
{
    int val = analogRead(sensorPin);
    Serial.println(val);
    float turbidity = map(val, 0, 2800, 5, 1); 
    String text = (String)turbidity;
    //Serial.println("Turbidity Value : "+val);
    return val;
}