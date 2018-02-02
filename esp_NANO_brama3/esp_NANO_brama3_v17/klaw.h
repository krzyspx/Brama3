#include <Arduino.h>
#include <RCSwitch.h>
RCSwitch mySwitch = RCSwitch(); //nie wiem co to - pewnie nadanie nazwy własnej procedurze

void pomparunpir();
void infgateopen();
void infgateclose();
void BlynkvirtualWrite_str (byte vPin1, String str1);

#define BLYNK_GREEN     "#23C48E"
#define BLYNK_BLUE      "#04C0F8"
#define BLYNK_YELLOW    "#ED9D00"
#define BLYNK_RED       "#D3435C"
#define BLYNK_DARK_BLUE "#5F7CD8"

#define _RED        "#FF0000"
#define _GREEN      "#00FF00"
#define _MAGENTA    "#FF00FF"
#define _YELLOW     "#FFFF00"
#define _BLACK      "#000000"
#define _WHITE      "#FFFFFF"
#define _WBLUE      "#00FFFF"
#define _PINK       "#FFB3B3"
#define _OLIVE      "#808000"
#define _BRAU       "#FF8000"
#define _karmin     "#FF0080"
#define _SEE        "#00FF80"
#define _fiolet     "#8000FF"
#define _BLUE       "#0080FF"

#define impuls_pilot 320  // dł impulsu dla pilota ELRO
#define impuls_piec 210

#define k1on 5522769
#define k1off 5522772
#define k2on 5525841
#define k2off 5525844
#define k3on 5526609
#define k3off 5526612
#define k4on 5526801
#define k4off 5526804

#define sal1on 17745
#define sal1off 17748
#define sal2on 20817
#define sal2off 20820
#define sal3on 21585
#define sal3off 21588
#define sal4on 21777
#define sal4off 21780

#define ogrodAon 263505
#define ogrodAoff 263508
#define ogrodBon 266577
#define ogrodBoff 266580
#define ogrodCon 267345
#define ogrodCoff 267348
#define ogrodDon 267537
#define ogrodDoff 267540

#define gateopen 3150041 // 400
#define gateclose 13564377 // 400
#define pirpompa 14013756 // delay  400

#define piecon 5592331 //5592332 delay 210
#define piecoff 5592323 //5592332 delay 210

#define pompaoff 4478228 //zmienic
#define pompaon  4478225

int menupilot;   // wybrana pozycja z menu

unsigned long jakikod(int mp, int pp) // tabela kodów wybieranych przez mp i pp dla menu rozwijanego
{
  unsigned long x;
  if (pp)  {
    switch (mp) { // Switch on Elro - wybór kodu pilota w zależności od ustawienia w menu rozwijanym
      case 1: x = k1on;  break;
      case 2: x = k2on;  break;
      case 3: x = k3on;  break;
      case 4: x = k4on;  break;

      case 5: x = ogrodAon;  break;
      case 6: x = ogrodBon;  break;
      case 7: x = ogrodCon;  break;
      case 8: x = ogrodDon;  break;

      case 9: x = sal1on;  break;
      case 10: x = sal2on;  break;
      case 11: x = sal3on;  break;
      case 12: x = sal4on;  break;

      default: ;
    }
  } else  {
    switch (mp)  { // Switch on Elro - wybór kodu pilota w zależności od ustawienia w menu rozwijanym
      case 1: x = k1off;  break;
      case 2: x = k2off;  break;
      case 3: x = k3off;  break;
      case 4: x = k4off;  break;

      case 5: x = ogrodAoff;  break;
      case 6: x = ogrodBoff;  break;
      case 7: x = ogrodCoff;  break;
      case 8: x = ogrodDoff;  break;

      case 9: x = sal1off;  break;
      case 10: x = sal2off;  break;
      case 11: x = sal3off;  break;
      case 12: x = sal4off;  break;

      default: ;
    }
  }
  return x;
}

void sendkodpilot (unsigned long codeA, int len) // wysłanie kodu przez nadajnik 433MHz
{
  mySwitch.setPulseLength(len);
  mySwitch.setProtocol(1);
  mySwitch.setRepeatTransmit(4);
  mySwitch.send(codeA, 24);
}

void setrcswitch() { //ustawienia początkowe
  mySwitch.enableReceive(0);  // Receiver on inerrupt 0 => that is pin #2
  mySwitch.enableTransmit(3); // Transmiter pin D3
}

bool termesp = 0; //przełącznik wyświetlania odebranych danych na vTerminalu
unsigned long oldrecivecod = 0; // kod odebrany z 433MHz
void  kod_run() // procedyra obsługi odebranego z 433MHz
{
  unsigned long recivecod = 0; // kod odebrany z 433MHz
  if (mySwitch.available()) {

    String ss = " * " + String(mySwitch.getReceivedValue()) + " - " + String(mySwitch.getReceivedDelay()) + " * " ;

    if (termesp) {
      BlynkvirtualWrite_str(29, ss);
    }
    recivecod = mySwitch.getReceivedValue();

    if (oldrecivecod != recivecod) { //zabezpieczenie przed wielkorotnym wysyłaniem do APP przy powtórzeniach kodu
      oldrecivecod = recivecod;
      if ( pirpompa == mySwitch.getReceivedValue()) pomparunpir();
      if ( gateopen == mySwitch.getReceivedValue()) infgateopen();
      if ( gateclose == mySwitch.getReceivedValue()) infgateclose();
    }
    mySwitch.resetAvailable();
  }
}

