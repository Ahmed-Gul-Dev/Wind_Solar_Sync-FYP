                                                                                                                                                                                                                                                                                                                                                     /*
   SOLAR CURRENT PIN # A1
   Wind CURRENT PIN # A2
   Sync CURRENT Pin # A0

   SOLAR Voltage PIN # A6
   Wind Voltage PIN # A7
   SYNC Voltage PIN # A3

*/

# include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd1(0x23, 16, 2); // Initiliaze LCD
LiquidCrystal_I2C lcd2(0x25, 16, 2); // Initiliaze LCD
LiquidCrystal_I2C lcd3(0x26, 16, 2); // Initiliaze LCD
LiquidCrystal_I2C lcd4(0x27, 16, 2); // Initiliaze LCD

#define Wind_Volt_Pin A7
#define Solar_Volt_Pin A6
#define Sync_Volt_Pin A3
#define Solar_Cur_Pin A0
#define Wind_Cur_Pin A2
#define Sync_Cur_Pin A1
#define R1 4
#define R2 5

/**********Voltage Measuring Variables************/

double Wind_Voltage;
double Solar_Voltage;
double bat_Voltage;
double syn_Voltage;


double Wind_Current;
double Solar_Current;
double bat_Current;
double syn_Current;


void setup()
{
  Serial.begin(9600);                    // Initialize Serial Monitor
  lcd1.init();                     // LCD Dimenssions Set 20x4
  lcd2.init();                     // LCD Dimenssions Set 20x4
  lcd3.init();                     // LCD Dimenssions Set 20x4
  lcd4.init();                     // LCD Dimenssions Set 20x4
  lcd1.backlight();
  lcd2.backlight();
  lcd3.backlight();
  lcd4.backlight();
  pinMode(Solar_Volt_Pin, INPUT);
  pinMode(Wind_Volt_Pin, INPUT);
  pinMode(Sync_Volt_Pin, INPUT);
  pinMode(Solar_Cur_Pin, INPUT);
  pinMode(Wind_Cur_Pin, INPUT);
  pinMode(Sync_Cur_Pin, INPUT);
  pinMode(R1, OUTPUT);
  pinMode(R2, OUTPUT);
  digitalWrite(R1,HIGH);
  digitalWrite(R2,HIGH);

  lcd1.clear();
  lcd1.setCursor(0, 0);
  lcd1.print("WIND PARAMETERS");
  lcd1.setCursor(0, 1);
  lcd1.print(" LOADING ");
  
  lcd2.clear();
  lcd2.setCursor(0, 0);
  lcd2.print("SOLAR PARAMETERS");
  lcd2.setCursor(0, 1);
  lcd2.print(" LOADING ");
  
  lcd3.clear();
  lcd3.setCursor(0, 0);
  lcd3.print("SYNC PARAMETERS");
  lcd3.setCursor(0, 1);
  lcd3.print(" LOADING ");
  
  lcd4.clear();
  lcd4.setCursor(0, 0);
  lcd4.print("BAT ");
  lcd4.print("PARAMETERS");

  delay(1000);
}

void loop()
{

    Wind_Voltage = Voltage(Wind_Volt_Pin) - 1;
    Solar_Voltage = Voltage(Solar_Volt_Pin);
    bat_Voltage = Voltage(Sync_Volt_Pin);
    syn_Voltage = bat_Voltage;
    Solar_Current = Current(Solar_Cur_Pin) * -1;
    Wind_Current = Current(Wind_Cur_Pin) * -1;
    bat_Current = Current(Sync_Cur_Pin) * -1;
    syn_Current = bat_Current + Wind_Current + Solar_Current;
  
    LCD_Update();
    delay(1000);
}


float value=0.0;
double Voltage(int pin)
{
  uint16_t Raw = 0; // 16-bit unsigned integer
  double Volt;
  for (int i = 0; i < 20; i++)
  {
    Raw = (Raw + analogRead(pin));
    delay(1);
  }

  Raw = Raw / 20;
//  Volt = ((Raw / 4.045) / 10);

  //  vOUT = (value * 5.0) / 1024.0;    // where R2 = 7500 , R1 = 30000
  //  vIN = vOUT / (R2 / (R1 + R2));    // R2 / R1 + R2 = 0.2  , 5 / 1024 = 0.0049
// OR SIMPLIFY AS
    value = (Raw * 0.0049);    // where R2 = 7500 , R1 = 30000
    Volt = (value / 0.2) + 0.2;    // R2 / R1 + R2 = 0.2  , 5 / 1024 = 0.0049

  return Volt;
}

double Current(int pin)
{
  double Voltage = 0;
  double Current = 0;
  // Voltage is Sensed 1000 Times for precision
  for (int i = 0; i < 25; i++)
  {
    Voltage = (Voltage + (.0049 * analogRead(pin))); // (5 V / 1024 (Analog) = 0.0049) which converter Measured analog input voltage to 5 V Range
    delay(2);
  }
  Voltage = ( Voltage / 25);
  Current = ((Voltage - 2.5) / 0.066); // Sensed voltage is converter to current   here offset volt = 2.49 because at 0A , its gives 2.49v and 66 is factor for 30A ACS712 sesnor
  return Current;
}

void LCD_Update()
{
//  lcd1.print("WIND PARAMETERS");
  lcd1.setCursor(0, 1);
  lcd1.print("V:"+String(Wind_Voltage,1)+"V  I:"+String(Wind_Current,1)+"A ");
  
  
//  lcd2.print("SOLAR PARAMETERS");
  lcd2.setCursor(0, 1);
  lcd2.print("V:"+String(Solar_Voltage,1)+"V  I:"+String(Solar_Current,1)+"A ");
  
//  lcd3.print("SYNC PARAMETERS");
  lcd3.setCursor(0, 1);  
  lcd3.print("V:"+String(syn_Voltage,1)+"V  I:"+String(syn_Current,1)+"A ");
  
//  lcd4.print("BAT ");
  lcd4.setCursor(0, 1);
  lcd4.print("V:"+String(bat_Voltage,1)+"V  I:"+String(bat_Current,1)+"A ");
}
