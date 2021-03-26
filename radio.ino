#include <radio.h>
#include <si4703.h>
#include <Wire.h>
#include <RDSParser.h>
// nastavení propojovacích pinů

#include <LiquidCrystal_I2C.h> 

#define resetPin 2
#define SDIO A4
#define SCLK A5

// enkoder
#define pinCLK 3
#define pinDT 4
#define pinSW 5

//enkoder II
#define pinCLK2 6
#define pinDT2 7
#define pinSW2 9

// rele
#define pinRele 10
// proměnné pro uložení pozice a stavů pro určení směru
// a stavu tlačítka
int stavPred;
int stavCLK;
int stavSW;

int stavPred2;
int stavCLK2;
int stavSW2;

long lastButtonPressed = 0;
unsigned long lastRefresh;
bool backlightState;

bool bluetooth = false;


LiquidCrystal_I2C lcd(0x27, 16, 2);
SI4703 radio;
RDSParser rds;

void setup() {
    // Initiate the LCD:
  lcd.begin();
  lcd.backlight();

  // zahájení komunikace s modulem
  radio.init();

  radio.setVolume(8);
  radio.setBandFrequency(RADIO_BAND_FM, 9650);
  radio.debugEnable();

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
  stavPred2 = digitalRead(pinCLK2);  

  radio.attachReceiveRDS(RDS_process);

  rds.attachServicenNameCallback(DisplayServiceName);
}

void loop() 
{
char buffer[14];


radio.formatFrequency(buffer, sizeof(buffer));
int volume = radio.getVolume();
int frekvence = radio.getFrequency();


checkEncoder(volume);
checkEncoder2(frekvence);

Serial.println(bluetooth);

if(millis()-lastRefresh > 100)
{
  lastRefresh = millis();
  zobrazInfo(buffer, volume);

}

radio.checkRDS();

}
void zobrazInfo(char *frekvence, int hlasitost) {
  // vytištění informací z proměnných
  lcd.setCursor(0, 0);
  lcd.clear();
  if(!bluetooth)
  {
  lcd.print(frekvence);
  lcd.setCursor(14,1);
  lcd.print(hlasitost);
  lcd.print(" ");
  }
  else
  {
    lcd.setCursor(3,0);
    lcd.print("BLUETOOTH");
  }
  
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

void checkEncoder2(char *frekvence) 
{
   
   stavCLK2 = digitalRead(pinCLK2);
 if (stavCLK2 != stavPred2){
   if (digitalRead(pinDT2) != stavCLK2)
   {
     frekvence += 10;
     if (frekvence > 12000)
       frekvence = 12000;
  radio.setFrequency(frekvence);
}
 else{
     frekvence -= 10;
     if (frekvence < 0)
     frekvence = 0;
     
radio.setFrequency(frekvence);
 }
   

 }
    
stavPred2 = stavCLK2;

/*stavSW2 = digitalRead(pinSW2);
if (stavSW2 == 0){
radio.seekDown();

}
stavSW = digitalRead(pinSW);
if (stavSW == 0){
radio.seekUp();
}*/

stavSW2 = digitalRead(pinSW2);
if (stavSW2 == 0)
{
  if(digitalRead(pinRele) == LOW)
  {
    digitalWrite(pinRele, HIGH);
    bluetooth = true;
  }
  else
  {
    digitalWrite(pinRele,LOW);
    bluetooth = false;
  }
}
stavSW = digitalRead(pinSW);
if (stavSW == 0)
{
  radio.seekUp();
}
}

  
 void switchBacklight()
{
  if (backlightState == true){                         //check to see state of backlight
    backlightState = false;                            //flip state of backlight
  }
  else if (backlightState == false){
    backlightState = true;
  }

  if (backlightState == true){                         //the bool argument for backlight state (true/false)
    lcd.setBacklight(HIGH);                            //the statement that actualy changes the backlight state
  }
  else if (backlightState == false){
    lcd.setBacklight(LOW);
  }
}

void DisplayServiceName(char *name)
{
  lcd.setCursor(0, 1);  
  lcd.print(name);
  Serial.print(name);
}

 // DisplayServiceName()

void RDS_process(uint16_t block1, uint16_t block2, uint16_t block3, uint16_t block4) {
  rds.processData(block1, block2, block3, block4);
}



  
  
  
