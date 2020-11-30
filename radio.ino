// připojení potřebných knihoven
#include <Si4703_Breakout.h>
#include <Wire.h>
// nastavení propojovacích pinů

#include <LiquidCrystal_I2C.h> 
LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 16, 2); 

#define resetPin 2
#define SDIO A4
#define SCLK A5
Si4703_Breakout radio(resetPin, SDIO, SCLK);
// proměnné pro běh programu
int frekvence;
int hlasitost;
char rdsBuffer[10];

// enkoder
int pinCLK = 3;
int pinDT  = 4;
int pinSW  = 5;

// proměnné pro uložení pozice a stavů pro určení směru
// a stavu tlačítka
int poziceEnkod = 0;
int stavPred;
int stavCLK;
int stavSW;


void setup() {
    // Initiate the LCD:
  lcd.init();
  lcd.backlight();
  lcd.setCursor(1,0);
  lcd.print("Frekvence");
  lcd.setCursor(1,1);
  lcd.print("Hlasitost");
  // zahájení komunikace s modulem
  radio.powerOn();


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


}

void loop() {
stavCLK = digitalRead(pinCLK);
if (stavCLK != stavPred) {
    // pokud stav pinu DT neodpovídá stavu pinu CLK,
    // byl pin CLK změněn jako první a rotace byla
    // po směru hodin, tedy vpravo
    if (digitalRead(pinDT) != stavCLK) {
      // vytištění zprávy o směru rotace a přičtení
      // hodnoty 1 u počítadla pozice enkodéru
      
      
      
      hlasitost ++;
      radio.setVolume(hlasitost);
    if (hlasitost == 16) hlasitost = 15;
      
  
     }
    else {
      // vytištění zprávy o směru rotace a odečtení
      // hodnoty 1 u počítadla pozice enkodéru
      
      hlasitost --;
      radio.setVolume(hlasitost);
    if (hlasitost < 0) hlasitost = 0;
        }
      lcd.setCursor(1,1);
      lcd.print("Hlasitost:");
      lcd.println(hlasitost);
      Serial.print("Hlasitost:");
   
  
}
stavPred = stavCLK;
}
void zobrazInfo() {
  // vytištění informací z proměnných
  lcd.print("Frekvence:"); lcd.print(frekvence);
  lcd.print("Hlasitost:"); lcd.print(hlasitost);
}
