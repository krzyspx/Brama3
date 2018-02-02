#include <Arduino.h>
// matryca programów
void Program_vPin0(String strvPin);
void Program_vPin10(String strvPin);
void Program_vPin11(String strvPin);
void Program_vPin12(String strvPin);
void Program_vPin13(String strvPin);
//void Program_vPin14(String strvPin);
void Program_vPin15(String strvPin);
void Program_vPin16(String strvPin);
//void Program_vPin17(String strvPin);
void Program_vPin18(String strvPin);
void Program_vPin19(String strvPin);
void Program_vPin29(String strvPin);
void Program_vPin34(String strvPin);

// equivalent BLYNK_WRITE(VPin)   vPin from APP to NANO  

void Blynkwrite(String str2) {

  String istr = str2.substring(1, str2.indexOf(':')); // nr vPin
  byte pin2 = istr.toInt();
  String  strdata2 = str2.substring(str2.indexOf(':') + 1); // if nr vPin typ int
  int  data2 = strdata2.toInt();

  if (str2.charAt(0) == 'V') {

    switch (pin2 + 1) { // Switch to progrma for recived vPin
      case 1: Program_vPin0(strdata2);  break;
      
  //    case 10: Program_vPin9(strdata2);  break;
      case 11: Program_vPin10(strdata2);  break;
      case 12: Program_vPin11(strdata2);  break;
      case 13: Program_vPin12(strdata2);  break;
      case 14: Program_vPin13(strdata2);  break;
   //   case 15: Program_vPin14(strdata2);  break;
      case 16: Program_vPin15(strdata2);  break;
      case 17: Program_vPin16(strdata2);  break;
   //   case 18: Program_vPin17(strdata2);  break;
       case 19: Program_vPin18(strdata2);  break;
      case 20: Program_vPin19(strdata2);  break;  
   
      // etc....
      case 30: Program_vPin29(strdata2);  break;
            case 35: Program_vPin34(strdata2);  break;
      default: ;
    }
  }
}

// equivalent   Blynk.virtualWrite(VPin, Data);

void sendstrtoserial (String toserial) {
  Serial.println(toserial);
}
void BlynkvirtualWrite(char Ch, byte vPin, String str) { // send to APP
  String  str1 = Ch + String(vPin) + ':' + str; // string to send 
  sendstrtoserial(str1);
}
void BlynkvirtualWrite_str (byte vPin1, String str1) { // send str to APP
  BlynkvirtualWrite('S', vPin1, str1);
}
void BlynkvirtualWrite_int (byte vPin2, int z)  { // send int to APP
  BlynkvirtualWrite('I', vPin2, String(z));
}
void BlynkvirtualWrite_led(byte vPin3, byte Value) { // send LEDs data to APP
BlynkvirtualWrite('L', vPin3, String(Value));
}
void BlynkvirtualWrite_col(byte vPin4, String col) { // send color data to APP
BlynkvirtualWrite('C', vPin4, col);
}
void BlynkvirtualWrite_On(byte vPin5, String ON) { // send onLabels data to APP
BlynkvirtualWrite('N', vPin5, ON);
}
void BlynkvirtualWrite_Off(byte vPin6, String OFF) { // send offLabel data to APP
 BlynkvirtualWrite('F', vPin6, OFF);
}
void BlynkvirtualWrite_order(byte vPin7, String str7) { // send order to esp
 BlynkvirtualWrite('O', vPin7, str7);
}

// ....................................... recive string from serial

bool  stringComplete = false;
String inputString = "";
byte licznikodbioru = 0;
int startrecive = 0;

void recivestrfromserial () {
  if (stringComplete) {
      if(termesp){ 
    Serial.println("od  " + String(inputString)); //test
  }
    Blynkwrite(inputString);
    inputString = "";
    stringComplete = false;
    licznikodbioru = 0;
    startrecive = 0;
  }
}

void serialEvent() { //odbiór string z Serial
  if (Serial.available()) {
    licznikodbioru++;
    char inChar = (char)Serial.read();
    if (startrecive == 0) {
      if ((inChar == 'V'))  startrecive = 1;
    }
    if (startrecive == 1) {
      if (inChar == '\r')  {
        stringComplete = true;
        inChar = '\0';
        inputString += inChar;
        startrecive = 0;
      } else  {inputString += inChar;}
    }
    if (licznikodbioru > 60) {inputString = "";}
  }
}


