//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   pompa
#define czasobiegu  18  // ilość sek włączenia pompy x 10

int intfromstr(String str);
void pompazal();


int obieg = -6; //licznik czasu włączenia pompy -6 bo 6 razy powarza wyłączenie pompy
byte oldtimeronoff = 1;
void timerpompa(String str) //timer z APP do pompy obiegowej załącza pompe z czujnika PiR tylko w okrelonych porach
{
  byte timeroff = intfromstr(str);
  if (oldtimeronoff == timeroff) {}
  else {
    obieg = czasobiegu; // 2 razy na dobę załączanie pompy dla celów sanitarnych
    oldtimeronoff = timeroff;
    if (timeroff == 0) BlynkvirtualWrite_col(16, BLYNK_RED); else BlynkvirtualWrite_col(16, BLYNK_GREEN);
  }
}

void licznikobiegu() //odmierzanie czasu włączenia pompy
{
  obieg--;
  if (obieg < -5) {
    obieg = -6;
  }
  else {
    if (obieg < 0) {
      sendkodpilot(pompaoff, impuls_pilot);
      BlynkvirtualWrite_col(15 , _BRAU );
      BlynkvirtualWrite_Off(15 , "POMPA");
    }  else {
      pompazal();
    }
  }
}

void pompazal() //wysyłanie kodu do załączenia pompy
{
  sendkodpilot(pompaon, impuls_pilot);
  String xString = String(obieg);
  BlynkvirtualWrite_col(15 , _YELLOW);
  BlynkvirtualWrite_Off(15, xString);
}
void pomparunprzycisk(String str) { //zał pompy vprzyciskiem w APP
  if (intfromstr(str)) {
    obieg = czasobiegu;
    pompazal();
  }
}

void pomparunpir() { //sprawdzenie czy mozna włączyć pompę w odpowiednim przedziale czasu
  if (oldtimeronoff == 1) {
    obieg = czasobiegu;
    pompazal();
  }
}

