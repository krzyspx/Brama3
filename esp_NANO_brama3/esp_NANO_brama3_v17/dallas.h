#include <OneWire.h>
#define ONE_WIRE_BUS 13 // 1-Wire bus Arduino pin
OneWire oneWire(ONE_WIRE_BUS);
#include <DallasTemperature.h>
DallasTemperature sensors(&oneWire); 
DeviceAddress tempDeviceAddress;

int oldtemp = 0;
void sensorT() { //wysylanie ttemp do APP
  int tempx10 = (((sensors.getTempCByIndex(0)) * 10) - 30); // x 10 i kalibracja
  
  sensors.setWaitForConversion(false);  // makes it async
  sensors.requestTemperatures(); // Send the command to get temperatures
  int temperature = (tempx10 / 10);
  int dziesietne = (tempx10 % 10);
  String str = String(temperature) + "." + String(dziesietne) + " ℃"; // oszczedzam 3 kB pamieci
//    Serial.println("realtemp" + str);
if (oldtemp != tempx10){
  oldtemp = tempx10;
  BlynkvirtualWrite_str(6, str);
}
}
void setdallas() //ustawienia początkowe dla Dallas
{
  sensors.begin();// Dallas
  sensors.getAddress(tempDeviceAddress, 0);
  sensors.setResolution(tempDeviceAddress, 12);
  sensors.setWaitForConversion(false);  // makes it async
  sensors.requestTemperatures(); // Send the command to get temperatures 
}

