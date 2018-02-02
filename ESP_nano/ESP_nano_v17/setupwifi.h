// set wifi OK

#include <BlynkSimpleEsp8266.h>
WidgetTerminal terminal31(V31); //ESP terminal  for test

#include <EEPROM.h>

void savewifi();
void settypewifi(byte set);
void printread();
void printdef();
void printset();
void getdata();

char ssid[] = "000000000000000000";
char pass[] = "kkkkkkkkkkkkkkkkk";
char auth[] = "1a6625144444444444444444444ace226c";


char t_auth[] = "77778888999911112222333344445555"; // declare same 32 char
char t_ssid[] = "same AP name 123456789"; // declare
char t_pass[] = "same AP password 123456789"; // declare
byte setp;

//eeprom adr
#define adrauth 200
#define adrssid 250
#define adrpass 300
#define adrflagawifi 1
const byte defwifi = 3;
const byte  setwifi = 6;

String      y_ssid = "";
String      y_pass = "";
String      y_auth = "";
int wpis = 0;
int dl_i = 0;
int dl_j = 0;
int dl_k = 0;

BLYNK_WRITE(V31)
{
  String  coscos = param.asStr(); // string from terminal
  switch (wpis)
  {
    case 1: {
        y_auth = coscos;  //from terminal
        dl_i = y_auth.length() + 1;  //change string to char[]
        y_auth.toCharArray(t_auth, dl_i); //change string to char[]
      } break;

    case 2: {
        y_ssid = coscos;
        dl_j = y_ssid.length() + 1;
        y_ssid.toCharArray(t_ssid, dl_j);
      } break;

    case 3: {
        y_pass = coscos;
        dl_k = y_pass.length() + 1;
        y_pass.toCharArray(t_pass, dl_k);
      } break;

    case 5: { // save new  login wifi data

        String b = "y";
        if (coscos == b) savewifi();
        b = "n";
        if (coscos == b) {
          terminal31.println(F("-"));
          terminal31.println(F("NOT SAVED"));
          terminal31.flush();
        }
      } break;
    case 6: { // seve mode next login to wifi
        String b = "def";
        if (coscos == b) {
          settypewifi(defwifi);
          terminal31.println(F("next login to def wifi"));
          terminal31.flush();
        }
        b = "set";
        if (coscos == b) {
          settypewifi(setwifi);
          terminal31.println(F("next login to set wifi"));
          terminal31.flush();
        }
      } break;
    default: ;
  }
}

BLYNK_WRITE(V35) //button SETUP
{
  if (param.asInt()) {
    wpis++;
    if ((wpis > 9) || (wpis < 1))  wpis = 1;

    switch (wpis)
    {
      case 1: {
          terminal31.println(F("print your auth: "));
          terminal31.flush();
        } break;

      case 2: {
          terminal31.println(F("print your ssid: "));
          terminal31.flush();
        } break;

      case 3: {
          terminal31.println(F("print your pass: "));
          terminal31.flush();
        } break;

      case 4: {
          wpis = 5;
          terminal31.println(F("-"));
          terminal31.print(F("your auth: "));
          terminal31.println(t_auth);
          terminal31.print(F("your ssid: "));
          terminal31.println(t_ssid);
          terminal31.print(F("your pass: "));
          terminal31.println(t_pass);
          terminal31.println(F("data wifi OK ? y/n"));
          terminal31.flush();
        } break;
      case 5: { } break;
      case 6: {
          terminal31.println(F("login to ? def/set "));
          terminal31.flush();
        } break;

      case 7:    printdef(); break;
      case 8:    printset(); break;
      case 9:    printread(); break;

      default: ;
    }
  }
}

void printdef() { // print def wifi data
  terminal31.println(" ");
  terminal31.println(F("**** DEF ****"));
  terminal31.println(auth);
  terminal31.println(ssid);
  terminal31.println(pass);
  terminal31.println("**************");
  terminal31.flush();
}

void printset() { // print set wifi data
  terminal31.println(" ");
  terminal31.println(F("**** SET ****"));
  terminal31.println(t_auth);
  terminal31.println(t_ssid);
  terminal31.println(t_pass);
  terminal31.println(F("*************"));
  terminal31.flush();
}

void savewifi()
{
  EEPROM.begin(512);
  EEPROM.put(adrauth, t_auth);
  EEPROM.put(adrssid, t_ssid);
  EEPROM.put(adrpass, t_pass);
  //  EEPROM.commit();
  EEPROM.end();
  terminal31.println(F("-"));
  terminal31.println(F("save new WiFi data"));
  terminal31.flush();
}

void settypewifi(byte set) { // save mode login flag
  EEPROM.begin(512);
  int addr = adrflagawifi;
  EEPROM.put(addr, set);
  //  EEPROM.commit();
  EEPROM.end();
}

void printread() { // print login data from EEPROM

  getdata();
  terminal31.println(F("-"));
  terminal31.println(F("**** FROM EE ****"));
  terminal31.println(t_auth);
  terminal31.println(t_ssid);
  terminal31.println(t_pass);
  terminal31.println(setp);
  terminal31.println(F("*****************"));
  terminal31.flush();
}
void getdata() {
  EEPROM.begin(512);
  EEPROM.get(adrauth, t_auth);
  EEPROM.get(adrssid, t_ssid);
  EEPROM.get(adrpass, t_pass);
  EEPROM.get(adrflagawifi, setp);
  EEPROM.commit();
  EEPROM.end();
}

void WifiBlynk() { //logowanie do wifi i blynk nie wstrzymujące wejścia do pętli głównej
  getdata();
  if (setp == setwifi) {
    Serial.println(F("set wifi login "));
    WiFi.mode(WIFI_STA);
    WiFi.begin(t_ssid, t_pass);
    //    Blynk.config(t_auth);
    Blynk.config(auth); //ten sam auth
    WiFi.status();
    Blynk.connect();

  }  else
  {
    Serial.println(F("def wifi login "));
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, pass);
    Blynk.config(auth);
    WiFi.status();
    Blynk.connect();
  }
  Serial.print("Connecting to ");    Serial.println(WiFi.SSID());
  Serial.print("IP address: ");  Serial.println(WiFi.localIP());
}

