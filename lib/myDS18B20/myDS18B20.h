/*
*authors:Nguyen Dat
*Created:31/10/2022
*/
#ifndef MyDS18B20_h
#define MyDS18B200_h
class DS18B20
{
private:
    unsigned char bit;
    uint8_t sensorPin;
    
public:
    DS18B20(uint8_t Pin){begin(Pin);};
    void begin(uint8_t Pin);
    unsigned char Reset(void);
    void Write_Bit(unsigned char bit);
    unsigned char Read_Bit(void);
    void Write_Byte(unsigned char bit);
    unsigned char Read_Byte(void);
    float readTemperature(void);
};

#endif
