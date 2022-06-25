
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// OLED Screen size
#define SCR_WIDTH 128
#define SCR_HEIGHT 32


#define OLED_RESET -1

Adafruit_SSD1306 display(SCR_WIDTH, SCR_HEIGHT, &Wire, OLED_RESET);

float Vin = 3.3;     // [V in]        
float Rt = 10000;    // Resistor t [ohm] [10k]
float R0 = 10000;    // value of rct in T0 [ohm]
float T0 = 298.15;   // use T0 in Kelvin [K]
float Vout = 0.0;    // Vout in A0 
float Rout = 0.0;    // Rout in A0
// use the datasheet to get this data.
float T1 = 273.15;      // [K] in datasheet 0º C
float T2 = 373.15;      // [K] in datasheet 100° C
float RT1 = 4050;   // [ohms]  resistence in T1
float RT2 = 549;    // [ohms]   resistence in T2
float Bta = 4050;
float beta = 0.0;    // initial parameters [K]
float Rinf = 0.0;    // initial parameters [ohm]   
float TempK = 0.0;   // variable output
float TempC = 0.0;   // variable output

// ugly way to calc average of takes
float takes[] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
// only for display "Init..."
int init_takes = 0;

void setup() {
  
  // begin adafruit
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  

  display.display();
  delay(2000);

  display.clearDisplay();

  display.display();

  // setting default values
  beta= Bta;
  Rinf = R0*exp(-beta / T0);
  write_text("Starting...");
}


void write_text(String text){
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println(text);
  display.display(); 
}

// prints a line avoiding screen clearing
void write_text_no_clear(String text){
  display.println(text);
  display.display(); 
}

// Sense temp
void sense_temp(){
  // analog read
  float VRT = analogRead(PA0);
  
  Vout = Vin*((float)(VRT) /1024.0); // calc for ntc
  Rout = (Rt * Vout/(Vin-Vout));

  TempK = (beta / log(Rout / Rinf)); // calc for temperature
  TempC = TempK - 273.15;

  // takes an average (x takes)
  float average = 0.0;
  int arr_length = sizeof(takes) / sizeof(takes[0]);

  // prepare queue
  for(int n = 0; n < arr_length; n++){    
    if(n < arr_length - 1){
      takes[n] = takes[n + 1];  
    }    
  }
  //set last take to last index
  takes[arr_length - 1] = TempC;

  // make an average
  for(int a = 0; a < arr_length; a++){
    average += takes[a];
  }
  
  float n_takes = (float)arr_length;
  average = average / n_takes;  

  String text_output = String(average) + String(char(167)) + "C";
  write_text(text_output);
  if(init_takes < arr_length + 1){
    write_text_no_clear("Init...");
    init_takes++;
  } 

  // ToDo: use (millis() - lastupdate > interval) to avoid blocking the main thread
  delay(1000);
}

void loop() {
  
  sense_temp();
  
}
