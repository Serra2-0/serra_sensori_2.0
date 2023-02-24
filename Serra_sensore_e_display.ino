#include <DHT.h>  //libreria per il sensore di temperatura e umidità

//Costanti
#define DHTPIN1 13       //Pin a cui è connesso il sensore
#define DHTTYPE1 DHT22   //Tipo di sensore che stiamo utilizzando (DHT22)
DHT dht(DHTPIN1, DHTTYPE1); //Inizializza oggetto chiamato "dht", parametri: pin a cui è connesso il sensore, tipo di dht 11/22

#include <LiquidCrystal_I2C.h>
#define SH_LD 2
#define LED_PIN 3
#define CP_CK 4
#define Q 5
#define P_su 6
#define P_giu 7 
#define P_enter 8
#define P_menu 9
#define P_scheda 10

#define JOYSTICK_X A0
#define JOYSTICK_Y A1
#define JOYSTICK_BUTTON 2

LiquidCrystal_I2C lcd(0x3f, 20, 4);
 

//Variabili
float hum;  //Variabile in cui verrà inserita la % di umidità
float temp; //Variabile in cui verrà inserita la temperatura
int batteria; //Variabile in cui verrà inserita la percentuale di batteria 
float Volt_batteria; //dichiarazione della variabile Tensione 
float Lux;
float Volt_pannello;
int scheda;
int sel1;
int sel2;
int i;
int Puls[5]={P_scheda, P_su, P_giu, P_enter, P_menu};


int y = 0;
int statolcd = 0;
int x = 0;
int button = 0;


void setup()
{
  Serial.begin(9600);
  dht.begin();
  lcd.init();
  lcd.backlight();
  analogWrite(LED_PIN, 50);
  pinMode(LED_PIN, OUTPUT);
  pinMode(SH_LD, OUTPUT);
  pinMode(CP_CK, OUTPUT);
  pinMode(Q, INPUT);
  scheda=0;
  y=0;
  sel1=0;
  sel2=0;
}

void loop()
{
  if(scheda==0)
  {
    Numero_scheda();
    if(sel1==0)
    {
     lcd.setCursor(0, y);
     lcd.print(">");
     lcd.setCursor(1, 0);
     lcd.print("Temperatura");
     lcd.setCursor(1, 1);
     lcd.print("Umidita");
     lcd.setCursor(1, 2);
     lcd.print("Luminosità");
     su();
     giu();
     PISO_Reg();     
     if(digitalRead(P_enter))
      sel1=1;
    }
     if(sel1==1)
     selezione_prima_scheda();
  }

  if(scheda==1) 
  {  
    Numero_scheda();
    if(sel2==0)
    {
     lcd.setCursor(0, y);
     lcd.print(">");
     lcd.setCursor(1, 0);
     lcd.print("Batteria");
     lcd.setCursor(1, 1);
     lcd.print("Volt_pannello");
     su();
     giu();
     PISO_Reg();
     if(digitalRead(P_enter))
       sel2=1;
    }
    if(sel2==1)
    selezione_seconda_scheda();
  }
}

void Numero_scheda()
{
  PISO_Reg();  
  if(digitalRead(P_scheda))
    {
      scheda=scheda++;
      if(scheda==2)
      scheda=0;
    }
}

void selezione_prima_scheda()  
{
  if(y==0)
  {
    lcd.clear();
    Temperatura();
    delay(100);
  }
  if(y==1)
  {
    lcd.clear();
    Umidita();
    delay(100);
  }
  if(y==2)
  {
    lcd.clear();
    Luminosita();
    delay(100);
  }
}

void selezione_seconda_scheda()
{
  if(y==0)
  {
    lcd.clear();
    Controllo_batteria();
    delay(100);
  }
  if(y==1)
  {
    lcd.clear();
    Pannello();
    delay(100);
  }
}

void su()
{
  PISO_Reg();
  if(digitalRead(P_su))
  {
    lcd.clear();
    y=y-1;
    if(y<0)
      y=1;
    delay(100);
  }  
}  
  
void giu()
{
  PISO_Reg();
  if(digitalRead(P_giu))
  {
    lcd.clear();
    y=y+1;
    if(y>2)
      y=0;  
    delay(100);
  }
}


void Temperatura()
{
  temp= dht.readTemperature();
  Serial.print(" %, Temp: ");
  Serial.print(temp);
  Serial.println(" Celsius");
  lcd.setCursor(0, 0);
  lcd.print("temperatura:");
  lcd.setCursor(12, 1);
  lcd.print(temp);
  PISO_Reg();
  if(digitalRead(P_menu))
    {
    lcd.clear();
    sel1 = 0;
    delay(100);
    }
}

void Umidita()
{
  hum = dht.readHumidity();
  Serial.print("Umidità: ");
  Serial.print(hum);
  lcd.clear();
  lcd.setCursor(0, 1);
  lcd.print("umidita:");
  lcd.setCursor(8, 0);
  lcd.print(hum);
  PISO_Reg();
    if(digitalRead(P_menu))
    {
    lcd.clear();
    sel1 = 0;
    delay(100);
    }
}

void Luminosita()
{
  Lux=analogRead(A2);
  Lux=map(Lux,0,1023,0,100);
  lcd.setCursor(0, 1);
  lcd.print("Luminosità:");
  lcd.print(Lux);
  PISO_Reg();
  if(digitalRead(P_menu))
  {
    lcd.clear();
    sel1 = 0;
    delay(100);
  }
}

void Controllo_batteria()
{
  Volt_batteria=analogRead(A0);
  Volt_batteria=map(Volt_batteria,0,1023,0,100);
  lcd.setCursor(0, 3);
  lcd.print("Batteria: ");   //stampa su display la stringa "Tensione"
  lcd.print(Volt_batteria);       //stampa su display il valore di tensione
  PISO_Reg();
  Serial.print(Volt_batteria);
  if(digitalRead(P_menu))
  {
    lcd.clear();
    sel2 = 0;
    delay(100);
  }
}

void Pannello()
{
  Volt_pannello=analogRead(A1);
  Volt_pannello=map(Volt_pannello,0,1023,0,100);
  lcd.setCursor(0, 3);
  lcd.print("Pannello: ");   //stampa su display la stringa "Tensione"
  lcd.print(Volt_pannello);       //stampa su display il valore di tensione
  PISO_Reg();
  Serial.print(Volt_pannello);
  if(digitalRead(P_menu))
  {
    lcd.clear();
    sel2 = 0;
    delay(100);
  }
}

void PISO_Reg()
{
  digitalWrite(SH_LD, LOW);
  digitalWrite(SH_LD, HIGH);
  delay(10);
  for(i=0;i<4;i++)
  {
    digitalWrite(CP_CK, LOW);  
    digitalWrite(CP_CK, HIGH);  
    Puls[i]=digitalRead(Q);
  }
}
