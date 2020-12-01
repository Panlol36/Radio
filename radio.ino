#include <radio.h>
#include <si4703.h>
#include <Wire.h>
// nastavení propojovacích pinů

#include <LiquidCrystal_I2C.h> 

#define resetPin 2
#define SDIO A4
#define SCLK A5

// proměnné pro běh programu
char rdsBuffer[10];
char rdsBuffer2[10];
int  frekvence;


// enkoder
#define pinCLK 3
#define pinDT 4
#define pinSW 5

//enkoder II
#define pinCLK2 8
#define pinDT2 7
#define pinSW2 6

// proměnné pro uložení pozice a stavů pro určení směru
// a stavu tlačítka
int stavPred;
int stavCLK;
int stavSW;

int stavPred2;
int stavCLK2;
int stavSW2;

unsigned long lastRefresh;
unsigned long lastRefresh2;
LiquidCrystal_I2C lcd(0x27, 16, 2);
SI4703 radio;

void setup() {
    // Initiate the LCD:
  lcd.begin();
  lcd.backlight();

  // zahájení komunikace s modulem
  radio.init();

  radio.setVolume(8);
  radio.setBandFrequency(RADIO_BAND_FM, 9650);

 // enkoder
 Serial.begin(9600);
  // nastavení propojovacích pinů jako vstupních
  pinMode(pinCLK, INPUT);
  pinMode(pinDT, INPUT);
  // nastavení propojovacího pinu pro tlačítko
  // jako vstupní s pull up odporem
  pinMode(pinSW, INPUT_PULLUP);
  // načtení aktuálního stavu pinu CLK pro porovnávání
  stavPred = digitalRead(pinCLK);   


 // enkoder
   // nastavení propojovacích pinů jako vstupních
  pinMode(pinCLK2, INPUT);
  pinMode(pinDT2, INPUT);
  // nastavení propojovacího pinu pro tlačítko
  // jako vstupní s pull up odporem
  pinMode(pinSW2, INPUT_PULLUP);
  // načtení aktuálního stavu pinu CLK pro porovnávání
  stavPred = digitalRead(pinCLK2);  

}

void loop() 
{
char buffer[12];


radio.formatFrequency(buffer, sizeof(buffer));
int volume = radio.getVolume();
int frekvence = radio.getFrequency();


checkEncoder(volume);
checkEncoder(frekvence);

if(millis()-lastRefresh > 100)
{
  lastRefresh = millis();
  zobrazInfo(buffer, volume);
}


}
void zobrazInfo(char *frekvence, int hlasitost) {
  // vytištění informací z proměnných
  lcd.setCursor(0,0);
  lcd.print(frekvence);
  lcd.setCursor(0,1);
  lcd.print("Hlasitost: "); 
  //if (hlasitost < 10)
  //{
    lcd.print(hlasitost);
    lcd.print(" ");
    lcd.print("Stisknuto tlacitko enkoderu!");
    
  //}
  /*//else
  {
    lcd.print(hlasitost);
  }*/
}

void checkEncoder(int hlasitost) 
{
    stavCLK = digitalRead(pinCLK);
  if (stavCLK != stavPred) 
  {
      // pokud stav pinu DT neodpovídá stavu pinu CLK,
      // byl pin CLK změněn jako první a rotace byla
      // po směru hodin, tedy vpravo
      if (digitalRead(pinDT) != stavCLK) 
      {
        // vytištění zprávy o směru rotace a přičtení
        // hodnoty 1 u počítadla pozice enkodéru
    
        hlasitost ++;
        
        if (hlasitost > 15) 
          hlasitost = 15;

          radio.setVolume(hlasitost);
      }
      else 
      {
        // vytištění zprávy o směru rotace a odečtení
        // hodnoty 1 u počítadla pozice enkodéru
        
        hlasitost --;  
        if (hlasitost < 0) 
        hlasitost = 0;

        radio.setVolume(hlasitost);

         

      }  
  }
stavPred = stavCLK;
}
void checkEncoder(char *frekvence){
  // načtení stavu pinu SW - tlačítko
  stavSW = digitalRead(pinSW);
  // v případě stisknutí vytiskni informaci
  // po sériové lince
  if (stavSW == '0') {
int frekvence = radio.seekUp();
else  (stavSW == '1') {
int frekvence = radio.seekDown();
     
}
}
}
