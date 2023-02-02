#include <Arduino.h>
#include <SoftwareSerial.h>
#include <tds_sensor.h>
#include <turbidity.h>
#include <Sim.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <TinyGPS++.h>

#define TdsSensorPin 34
#define TurbidityPin 35
#define PhSensorPin 32
#define DS18B20Pin 4
#define LAT 16.464568
#define LNG 107.607577
SoftwareSerial  GPS_Serial;
TinyGPSPlus gps;
OneWire oneWire(DS18B20Pin);
DallasTemperature sensors(&oneWire);
LiquidCrystal_I2C lcd(0x3F, 20, 4); 

float TdsValue, phValue, turbidityValue, temperatureValue, latitude, longtitude;
float PHRead(int sensorPin)
{
  float Value = analogRead(PhSensorPin);
  //Serial.print(Value);
  //Serial.print(" | ");
  float voltage = Value * (3.33 / 4095.0);
  float ph = (3.33 * voltage);
  //Serial.println(ph);
  return ph;
}
float TemperatureRead()
{
  sensors.requestTemperatures();
  return sensors.getTempCByIndex(0);
  // Serial.println(sensors.getTempCByIndex(0)); // vì 1 ic nên dùng 0
}
void displayValue()
{
 
  turbidityValue = Turbidity_ReadSensor(TurbidityPin);
  phValue = PHRead(PhSensorPin);
  temperatureValue = TemperatureRead();
  TdsValue = TDS_ReadSensor(temperatureValue);
  Serial.println("-------------------------------");
  Serial.println("PH :" + String(phValue));
  Serial.println("TDS :" + String(TdsValue));
  Serial.println("Turbidity :" + String(turbidityValue));
  Serial.println("Temperature :" + String(temperatureValue));
  Serial.println("GPS : " + String(latitude) + "," + String(longtitude));
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("PH :" + String(phValue));
  lcd.setCursor(11, 0);
  lcd.print("TDS :" + String(TdsValue));
  lcd.setCursor(0, 1);
  lcd.print("Turbidity :" + String(turbidityValue));
  lcd.setCursor(0, 2);
  lcd.print("Temperature :" + String(temperatureValue));
  lcd.setCursor(0, 3);

  lcd.print("GPS : " + String(LAT) + "," + String(LNG));
  sendRequest(phValue, TdsValue, turbidityValue, temperatureValue, LAT, LNG);
  // if (latitude == 0 && longtitude == 0)
  // {
  //   sendRequest(phValue, TdsValue, turbidityValue, temperatureValue, LAT, LNG);
  // }
  // else
  //   sendRequest(phValue, TdsValue, turbidityValue, temperatureValue, latitude, longtitude);
}
void displayGPSInfo()
{
  if (gps.location.isValid())
  {
    latitude = gps.location.lat();
    longtitude = gps.location.lng();
    lcd.setCursor(0, 3);
    lcd.print("GPS : " + String(latitude) + "," + String(longtitude));
    Serial.print(gps.location.lat(), 6);
    Serial.print(F(","));
    Serial.print(gps.location.lng(), 6);
  }
  else
  {
    Serial.print(F("INVALID"));
  }

  Serial.println();
}
void setup()
{
  Serial.begin(115200);
  GPS_Serial.begin(9600, SWSERIAL_8N1, 13,12, false);
  sensors.begin();

  TDS_Setup(TdsSensorPin);
  Turbidity_Setup(TurbidityPin);

  delay(3000);
  initSim();
  delay(3000);
  lcd.init();
}
void loop()
{

  while (GPS_Serial.available() > 0)
    if (gps.encode(GPS_Serial.read()))
      displayGPSInfo();

  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    Serial.println(F("No GPS detected: check wiring."));
  }
  displayValue();
}
