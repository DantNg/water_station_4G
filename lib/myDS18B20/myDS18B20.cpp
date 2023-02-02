#include <Arduino.h>
#include <myDS18B20.h>

int temp_Register; //2 Byte nhiet do

#define DS18B20_SKIP_ROM        0xCC //CMD gửi request đến tất cả slave
#define DS18B20_CONVERT_T       0x44 // CMD chuyển đổi nhiệt độ
#define DS18B20_READ_SCRATCH_PAD 0xBE //Đọc data từ slave


void DS18B20::begin(uint8_t Pin)
{
  sensorPin = Pin;
}
/*
Reset cảm biến
Return 0: OK ; 1:FAIL
*/
unsigned char DS18B20::Reset()
{
    unsigned char result;
    pinMode(sensorPin, OUTPUT); // ds18b20_DDR = 1;
    digitalWrite(sensorPin, 0); // ds18b20_PORT = 0;
    delayMicroseconds(480);

    digitalWrite(sensorPin, 1); // ds18b20_PORT = 1;
    delayMicroseconds(70);

    pinMode(sensorPin, INPUT_PULLUP);
    result = digitalRead(sensorPin);
    delayMicroseconds(410);

    return result;
}
// Ghi 1 bit vào cảm biến
void DS18B20::Write_Bit(unsigned char bit)
{
    if (bit == 1)
    {
        pinMode(sensorPin, OUTPUT); 
        digitalWrite(sensorPin, 0); 
        delayMicroseconds(6);
        digitalWrite(sensorPin, 1); 
        delayMicroseconds(64);
    }
    else if (bit == 0)
    {
        pinMode(sensorPin, OUTPUT);
        digitalWrite(sensorPin, 0);
        delayMicroseconds(60);
        digitalWrite(sensorPin, 1);
        delayMicroseconds(10);
    }
}
//Đọc 1 bit từ cảm biến
unsigned char DS18B20::Read_Bit()
{
    unsigned char result;
    pinMode(sensorPin, OUTPUT);
    digitalWrite(sensorPin, 0);
    delayMicroseconds(6);
    digitalWrite(sensorPin, 1);
    delayMicroseconds(9);
    pinMode(sensorPin, INPUT_PULLUP);
    result = digitalRead(sensorPin);
    delayMicroseconds(55);

    return result;
}
// Ghi 1 byte
void DS18B20::Write_Byte(unsigned char bit)
{
    unsigned char i = 8;
    while (i--)
    {
        Write_Bit((bit & 0x01));
        bit >>= 1;
    }
}
//Đọc 1 byte
unsigned char DS18B20::Read_Byte()
{
    unsigned char i = 8, result = 0;
    while (i--)
    {
        result >>= 1;
        result |= Read_Bit() << 7;
    }
    return result;
}

float DS18B20::readTemperature()
{
    float temp; 
    // Connect Slave
    while (Reset())
        ;

    Write_Byte(DS18B20_SKIP_ROM);
    Write_Byte(DS18B20_CONVERT_T);
    delay(94); //Đợi 94ms để chuyển đổi nhiệt độ
    // Connect Slave
    while (Reset())
        ;
    Write_Byte(DS18B20_SKIP_ROM);
    Write_Byte(DS18B20_READ_SCRATCH_PAD);

    temp_Register = Read_Byte();
    temp_Register = (Read_Byte() << 8) | temp_Register;

    temp = (temp_Register >> 4) + (temp_Register & 0x000F) * 1.0f / 16; // Tính giá trị nhiệt độ thực từ temp_Register

    return temp;
}
