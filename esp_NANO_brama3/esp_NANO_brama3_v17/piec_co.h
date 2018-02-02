#define piechistereza 2
int temppiec = 170; //początkowa temparatura zadana

void Program_vPin18(String strvPin);
void Program_vPin19(String strvPin);
void wyslij_V11();

bool piecgrzanie = 0; //flaga grzania on/off
bool oldpiecgrzanie = 0;
void pieconoff()
{
  if (oldtemp < (temppiec - piechistereza))
  {
    piecgrzanie = 1;//włącz grzanie
    if (oldpiecgrzanie != piecgrzanie)
    {
      oldpiecgrzanie = piecgrzanie;
      sendkodpilot(piecon, impuls_piec);
      // BlynkvirtualWrite_col(17, _BLUE);
      Serial.println("piec on");
    }
  }
  if (oldtemp > (temppiec + piechistereza))
  {
    piecgrzanie = 0;//wyłącz grzanie
    if (oldpiecgrzanie != piecgrzanie)
    {
      oldpiecgrzanie = piecgrzanie;
      sendkodpilot(piecoff, impuls_piec);
      BlynkvirtualWrite_col(17, _RED);
      Serial.println("piec off");
    }
  }
}
bool flagagrzanie = 0;
void miganiegrzanie()
{
  flagagrzanie = !flagagrzanie;
  if (piecgrzanie)
  {
    if (flagagrzanie)
    {
      BlynkvirtualWrite_col(17, _RED);
    } else
    {
      BlynkvirtualWrite_col(17, _BLUE);
    }
  }
}

void Program_vPin19(String strvPin) //zwiekszanie setup temperatury co 0,5 oC
{
  int  klaw = intfromstr(strvPin);
  if (klaw)  //sprawdzanie zmiany stanu przycisku  vPIN V10 załączenie pompy
  {
    temppiec = temppiec + 5;
    if (temppiec > 260)
    {
      temppiec = 260;
    }
    wyslij_V11();
  }
}

void Program_vPin18(String strvPin) //zmniejszanie setup temperatury co 0,5 oC
{
  int  klaw = intfromstr(strvPin);
  if (klaw) //sprawdzanie zmiany stanu przycisku  vPIN V10 załączenie pompy
  {
    temppiec = temppiec - 5;
    if (temppiec < 130)
    {
      temppiec = 130;
    }
    wyslij_V11();
  }
}

int oldtemppiec = 0;
void wyslij_V11() { //wysylanie ttemp do APP

  int temperature = (temppiec / 10);
  int dziesietne = (temppiec % 10);
  String str = String(temperature) + "." + String(dziesietne) + " ℃"; // oszczedzam 3 kB pamieci
  Serial.println("set piec" + str);
  if (oldtemppiec != temppiec) {
    oldtemppiec = temppiec;
    BlynkvirtualWrite_str(17, str);
  }
}

