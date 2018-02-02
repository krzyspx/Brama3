#include <Arduino.h>
#include <BlynkSimpleEsp8266.h>


int termesp = 0;
int seresp = 0;

void Blynkwrite(String str2);

BLYNK_WRITE(V32) { //on/off serial esp to terminal
  if (param.asInt())termesp = 1; else termesp = 0;
}
BLYNK_WRITE(V33) { //on/off serial esp to serial
  if (param.asInt())seresp = 1; else seresp = 0;
}

// ....................................... recive string from serial
bool  stringComplete = false;
String inputString = "";
byte licznikodbioru = 0;
int startrecive = 0;

void printterm31() {
  terminal31.println("od  " + (inputString));
  terminal31.flush();
}

void recivestrfromserial () {
  if (stringComplete) {
  if(seresp){    Serial.println("od  " + (inputString));  }
  if(termesp){ 
    printterm31();
  }
    Blynkwrite(inputString);
    inputString = "";
    stringComplete = false;
    licznikodbioru = 0;
    startrecive = 0;
  }
}

void myserialEvent() { // string z Serial
  if (Serial.available()) {
//    licznikodbioru++;
    char inChar = (char)Serial.read();
    if (startrecive == 0) {
      if ((inChar == 'S') || (inChar == 'I') || (inChar == 'L') || (inChar == 'C') || (inChar == 'N') || (inChar == 'F')|| (inChar == 'O'))  startrecive = 1;
    }
    if (startrecive == 1) {
      if ((inChar == '\r')||(inChar == '\n'))  {
        stringComplete = true;
        inChar = '\0';
        inputString += inChar;
        startrecive = 0;
      } else  inputString += inChar;
    }
 //   if (licznikodbioru > 50) inputString = "";
  }
}



