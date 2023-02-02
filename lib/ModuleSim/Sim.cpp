#include <Arduino.h>
#include <Sim.h>
void updateSerial()
{
  delay(1000);
  while (Serial.available())
  {
    Serial2.write(Serial.read());//Forward what Serial received to Software Serial Port
  }
  while (Serial2.available())
  {
    Serial.write(Serial2.read());//Forward what Software Serial received to Serial Port
  }
}
void initSim()
{
   Serial2.begin(115200);
  Serial2.println("AT");
  updateSerial();
  Serial.println();

  Serial2.println("AT+CREG?");
  updateSerial();
}

void sendRequest(float field1, float field2,float field3,float field4,double field5,double field6)
{
  
  //Serial2.write("AT+HTTPPARA=\"URL\",\"https://api.thingspeak.com/update?api_key=KYLRGGMQC67FH5D2&field1=20&field2=250&field3=81&field4=30\r\n");
  updateSerial();
  Serial2.print("AT+HTTPINIT\r\n");
  updateSerial();
  String http_str = "AT+HTTPPARA=\"URL\",\"https://api.thingspeak.com/update?api_key=KYLRGGMQC67FH5D2&field1="+String(field1)+"&field2="+String(field2)+"&field3="+String(field3)+"&field4="+String(field4)+"&field5="+String(field5)+"&field6="+String(field6)+"\"\r\n";
  Serial2.print(http_str);
  updateSerial();
  delay(1000);
  Serial2.print("AT+HTTPACTION=0\r\n");
  updateSerial();
  //Serial2.print("AT+HTTPTERM\r\n");
  updateSerial();
}