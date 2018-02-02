/*******************************************************************
  V1 - vLED OK -
  V2 - VLED przełącznik
  V29 - Terminal
  V6 - show temp DS
  V10 - on/off menu pilot
  V11 - menu pilot
  V12 - button on/off
  v13 - gate open/close
  V15 - pompa on
  V16 timer pompy
  V17 - temp piec
  V18 - temp--
  v19 - temp++
  
  V34 -
  A5 - przełącznik
  13 - DS18b20
  11 transoptor
  #ifndef
#define
********************************************************************/

#include "klaw.h"
#include "transmit.h"
#include "dallas.h"
#include "pompa.h"
#include "piec_co.h"

#include <OneWire.h>
#include <DallasTemperature.h>
#include <RCSwitch.h>

#include <Timers.h> //  my favorite timer 
Timers <6> akcja; //

#define led_green 7  // przyłączenia do fizycznych portów NANO
#define led_red   8 // 
#define przelacznik A5
#define resetesp  4
#define czasresetesp 60
#define pilotbrama  11



//zmienne globalne

int sendnotifi = 0; //wyślij noti
int sendemail = 0;  // wyślij email

//>>>>>>>>>>>>>>>>>>> my NANO Programs

void setupstart() { //początkowe ustawienia widgetow
  BlynkvirtualWrite_col(30, _GREEN);
  BlynkvirtualWrite_col(1, BLYNK_GREEN);
  BlynkvirtualWrite_col(2, _RED);
  BlynkvirtualWrite_col(10, _BLUE);
}
 void setupstart1()
{ 
  BlynkvirtualWrite_col(11, _BLUE);
  BlynkvirtualWrite_col(12, _MAGENTA);
  BlynkvirtualWrite_col(15, _YELLOW);
    BlynkvirtualWrite_col(18, _BLUE);
      BlynkvirtualWrite_col(19, _RED);
}
void setupstart2()
{
  BlynkvirtualWrite_Off(10, "PILOT");
  BlynkvirtualWrite_On(10, "PILOT");
  BlynkvirtualWrite_On(12, "k1");
  BlynkvirtualWrite_Off(12, "k1");
}
void setupstart3()
{
  BlynkvirtualWrite_On(15, "POMPA");
  BlynkvirtualWrite_Off(15, "POMPA");
    BlynkvirtualWrite_Off(18, "DOWN");
      BlynkvirtualWrite_Off(19, "UP");
}
void setupstart4()
{
   wyslij_V11();
}

int ESPonline = czasresetesp; //licznik watchdog dla esp
byte flagasetup1 = 0; //czy wysłać dane początkowe z APP
void Program_vPin0(String strvPin) {
  digitalWrite(led_red, intfromstr(strvPin));  //miganie led red przez ESP
  ESPonline = czasresetesp;
  if (flagasetup1 == 0) {
    flagasetup1 = 1;
    BlynkvirtualWrite_order(1, "a");
  }
}
void Program_vPin10(String strvPin) {
  sendkodpilot(jakikod(menupilot, intfromstr(strvPin)), impuls_pilot); // wysyła kod z pozycji menupilot i on/off zależne od ppolot
  //    BlynkvirtualWrite_order(2, "to ja blynk");
  //   akcja.updateInterval(3, 500);
}
void Program_vPin11(String strvPin) {
  menupilot = intfromstr(strvPin);
}
void Program_vPin12(String strvPin) {
  sendkodpilot(jakikod(1, intfromstr(strvPin)), impuls_pilot);  //wysyła kod z 1 pozycji menu on lub off
  //  BlynkvirtualWrite_order(10, "500");
  //  akcja.updateInterval(3, 1500);
}
void Program_vPin13(String strvPin) {//pilot brama
  sterujbrama(intfromstr(strvPin)); // zmiana stanu pin 11 - naciskanie pilota bramy
}
void Program_vPin15(String strvPin) {
  pomparunprzycisk(strvPin);
}
void Program_vPin16(String strvPin) {
  timerpompa(strvPin);
}
void Program_vPin29(String strvPin) {}

void Program_vPin34(String strvPin) {
  if (intfromstr(strvPin)) termesp = 1; else termesp = 0;
}

int intfromstr(String str) {
  String    strdata3 = str.substring(str.indexOf(':') + 1);  // if nr vPin typ int
  return strdata3.toInt();
}


//>>>>>>>>>>>>>>>>>>>>>>>> gate

void infgateopen() { //czujnik otwarcia bramy
  // BlynkvirtualWrite_col(8, _RED);
  // BlynkvirtualWrite_str(8, "BRAMAA OPEN");
  BlynkvirtualWrite_Off(13, "BR OPEN");
  BlynkvirtualWrite_col(13, _RED);

}
void infgateclose() {
  // BlynkvirtualWrite_col(8, _GREEN);
  // BlynkvirtualWrite_str(8, "BRAMAA CLOSE");
  BlynkvirtualWrite_Off(13, "BR CLOSE");
  BlynkvirtualWrite_col(13, _GREEN);
}

void  sterujbrama(int stanvPin) //steroanie pilotem bramy
{
  if (stanvPin) {
    digitalWrite(pilotbrama, HIGH);
    akcja.updateInterval(3, 300); //led green miga szybko gdy naciśnięty VPin bramy
  } else {
    digitalWrite(pilotbrama, LOW);
    akcja.updateInterval(3, 5000);
  }
}
//ssssssssssssssssssssssssssssssssssssssssssss
void setup()
{
  pinMode(led_red, OUTPUT); //test
  pinMode(led_green, OUTPUT); //test
  pinMode(przelacznik, INPUT_PULLUP);
  digitalWrite(led_green, HIGH);
  digitalWrite(led_red, HIGH);
  pinMode(pilotbrama, OUTPUT);
  digitalWrite(pilotbrama, LOW);

  Serial.begin(115200);
  delay(100);
  Serial.println(F(__FILE__));  //BLYNK .3.10 Arduino IDE.cc 1.6.12
  setupstart();
  delay(500);
  setupstart1();
  delay(500);
  setupstart2();
  delay(500);
  setupstart3();
  delay(500);
 setupstart4();
  delay(500);

  akcja.attach(0, 2000, timer1sek); //  5 sek
  akcja.attach(1, 10000, licznikobiegu); // pętla dłuższa 10 s licznik grzania
  akcja.attach(2, 20000, notemail); //by nie wysyłać za częto emaili i noti
  akcja.attach(3, 5000, timerled); //
   akcja.attach(4, 5000, pieconoff);

  setrcswitch(); //setup for rcswitch
  setdallas(); //setup for ds18b20

  flagasetup1 = 0; //flaga do jednokrotnego wysłania aktualnych stanów vPinów z APP
}

void loop()
{
  akcja.process(); // timer
  recivestrfromserial(); //dekodowanie danych z serial
  kod_run(); //analiza odbioru 433 MHz
}

void notemail() {// wysyłanie emaili i noti
  if (sendnotifi) {
    sendnotifi = 0;
    BlynkvirtualWrite_order(2, "noti od BLYNKa");
  }
  if (sendemail) {
    sendemail = 0;
    BlynkvirtualWrite_order(3, "Email od BLYNKa");
  }
}

void timer1sek() { //różne rzeczy wykonywane cyklicznie
  testprzelacznik();
  watchdogESP();  //watchdog for ESP
  sensorT();    //dallas
  miganiegrzanie(); //miganie v12 kiedy piec grzeje
  oldrecivecod = 0; // by nie wysyłać za często powtarzających się kodów
}

void timerled() {// osobny timer dla leda green
  blinkLedgreen(); // blink led on NANO
    blinkvLed2(); //blink vLed from NANO
}

void watchdogESP () { //watchdog dla esp
  ESPonline--;
 //   Serial.println("reset esp za " + String(ESPonline));
  if (ESPonline == 0) { //reset esp
    digitalWrite(resetesp, LOW);
    pinMode(przelacznik, OUTPUT);
    Serial.println(F("Reset ESP"));
  }
  if (ESPonline == -1) {
    pinMode(przelacznik, INPUT);
    Serial.println(F("END Reset ESP"));
    ESPonline = czasresetesp;
  }
}
int oldprzelacznik = 0;
void testprzelacznik() { //sprawdzanie stany przełącznika
  if (oldprzelacznik != digitalRead(przelacznik))
  {
    oldprzelacznik = digitalRead(przelacznik);
    if (!digitalRead(przelacznik)) {
      BlynkvirtualWrite_led(2, 255);
    }
    else {
      BlynkvirtualWrite_led(2, 0);
    }
  }
}

bool flagaled = 0; //blink NANO led NANO
void blinkLedgreen() {
  flagaled = !flagaled;
  digitalWrite(led_green, flagaled);
}
bool flagaled2 = 0;
void blinkvLed2() {
  flagaled2 = !flagaled2;
  if (flagaled2)  {
    BlynkvirtualWrite_led(1, 255); //blink NANO in APP
  }
  else   {
    BlynkvirtualWrite_led(1, 0);
  }
}


