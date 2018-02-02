/*
 * v31 - terminal esp
 * v32 ter on/off
 * v33 serial on/off
 * 
 * v35 setup wifi
 * 
 */

#include "setupwifi.h"
#include "transmit.h"

#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>


#include <Timers.h> //  my favorite timer 
Timers <4> akcja; // 4  wątki

WidgetLED led1(V30); // vLED ESP indicator for test
//WidgetTerminal terminal31(V31); //ESP terminal  for test


WidgetLED led00(V0); // vLEDs for NANO   - declaration for all vLEDs

WidgetTerminal terminal29(V29); //terminal for NANO


BLYNK_WRITE_DEFAULT() // send info form APP to NANO
{
  String strtosend = "";
  int pin = request.pin;      // Which exactly pin is handled?
  strtosend += 'V' + String(pin) + ':';
  for (auto i = param.begin(); i < param.end(); ++i) { //only 1 param now
    strtosend += i.asString();
  }
  Serial.println(strtosend);
}

void Blynkwrite(String str2) {//  command from NANO to APP

  String istr = str2.substring(1, str2.indexOf(':')); // nr vPin
  byte pin2 = istr.toInt();
  String  strdata2 = str2.substring(str2.indexOf(':') + 1); // if nr vPin typ int
  int  data2 = strdata2.toInt();
  char chardata2[10];
  int dl =  strdata2.length() + 1;
  strdata2.toCharArray(chardata2, dl);

  if (str2.charAt(0) == 'L') led00.setVPin(pin2); led00.setValue(data2); led00.setVPin(0);  //send LEDs data to APP
  if (str2.charAt(0) == 'S')  Blynk.virtualWrite(pin2, strdata2); //send str to APP
  if (str2.charAt(0) == 'I')  Blynk.virtualWrite(pin2, data2); //send int to APP
  if (str2.charAt(0) == 'C')  Blynk.setProperty(pin2, "color", chardata2);  // send color
  if (str2.charAt(0) == 'N')  Blynk.setProperty(pin2, "onLabel", chardata2); //send button label to APP
  if (str2.charAt(0) == 'F')  Blynk.setProperty(pin2, "offLabel", chardata2); //send button label to APP
  if (str2.charAt(0) == 'O')  order(pin2, strdata2); //send button label to APP
}

void order( byte pin4, String str4) {
  int  data = str4.toInt();
  if (pin4 == 1) Blynk.syncAll(); //order nr 1 - wyslać dane początkowe z APP
  if (pin4 == 2) Blynk.notify(str4);
  if (pin4 == 3) Blynk.email("Subject: email from BLYNK", str4);
  if (pin4 == 4) terminal29.flush();
  if (pin4 == 10)  akcja.updateInterval(1, data);
}

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  akcja.attach(0, 3000, timer1sek); //  1 sek
  akcja.attach(1, 3000, blinklednano);

  Serial.begin(115200);
  WifiBlynk(); //logowanie do wifi i blynk
  Serial.println(F(__FILE__));  //BLYNK .3.10 Arduino IDE.cc 1.6.12

//  EEPROM.begin(512); //do wifi setup
} //end setup

void loop()  //>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> loop
{
  akcja.process(); //
  Blynk.run();
  myserialEvent();
  recivestrfromserial();
}
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>  end loop

void timer1sek() {
  blinkvLed();
  // blinkLedNANO();
  //    Serial.println("ECHO ESP");
}

bool wsk3 = 0;
void  blinkvLed() {
  wsk3 = !wsk3;
  if (wsk3)  led1.off(); else  led1.on();
}
bool wsk2 = 0;
void  blinklednano() {
  wsk2 = !wsk2;
  String strtosend = 'V' + String(0) + ':' + String(wsk2);
  Serial.println(strtosend);
  digitalWrite(LED_BUILTIN, wsk2);
}



