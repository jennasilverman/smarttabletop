//black chord
// Led strip code sampled from built in FastLED example library code titled ColorPallette
//Load cell code sampled from built in HX711_ADC example library code titled  Read_1x_load_cell

//Set up LED Strip:
#include <FastLED.h>
#define LED_PIN     2
#define NUM_LEDS    150
#define BRIGHTNESS  64
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];
#define UPDATES_PER_SECOND 100
CRGBPalette16 currentPalette;
TBlendType    currentBlending;
extern CRGBPalette16 myRedWhiteBluePalette;
extern const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM;

//Set up Load cells:
#include <HX711_ADC.h>
#if defined(ESP8266)|| defined(ESP32) || defined(AVR)
#include <EEPROM.h>
#endif
//pins:
const int HX711_dout = 4; //mcu > HX711 dout pin
const int HX711_sck = 5; //mcu > HX711 sck pin
//HX711 constructor:
HX711_ADC LoadCell(HX711_dout, HX711_sck);
const int calVal_eepromAdress = 0;
unsigned long t = 0;

void setup() {
  // put your setup code here, to run once:

  //LED Sample code's setup
 //delay( 300 ); // power-up safety delay
    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
    FastLED.setBrightness(  BRIGHTNESS );

  //Load Cell Sample code setup
   Serial.begin(57600); //delay(10);
  Serial.println();
  Serial.println("Starting...");

  LoadCell.begin();
  //LoadCell.setReverseOutput(); //uncomment to turn a negative output value to positive
  float calibrationValue; // calibration value (see example file "Calibration.ino")
  calibrationValue = 696.0; // uncomment this if you want to set the calibration value in the sketch
#if defined(ESP8266)|| defined(ESP32)
  //EEPROM.begin(512); // uncomment this if you use ESP8266/ESP32 and want to fetch the calibration value from eeprom
#endif
  //EEPROM.get(calVal_eepromAdress, calibrationValue); // uncomment this if you want to fetch the calibration value from eeprom

  unsigned long stabilizingtime = 2000; // preciscion right after power-up can be improved by adding a few seconds of stabilizing time
  boolean _tare = true; //set this to false if you don't want tare to be performed in the next step
  LoadCell.start(stabilizingtime, _tare);
  if (LoadCell.getTareTimeoutFlag()) {
    Serial.println("Timeout, check MCU>HX711 wiring and pin designations");
    while (1);
  }
  else {
    LoadCell.setCalFactor(calibrationValue); // set calibration value (float)
    Serial.println("Startup is complete");
  }
}

void loop() {
  // put your main code here, to run repeatedly:
//Load Cell Read
static boolean newDataReady = 0;
 const int serialPrintInterval = 0; //increase value to slow down serial print activity
  // check for new data/start next conversion:
  if (LoadCell.update()) newDataReady = true;
/*
  // get smoothed value from the dataset:
  if (newDataReady) {
    if (millis() > t + serialPrintInterval) {
      float i = LoadCell.getData();
      Serial.print("Load_cell output val: ");
      Serial.println(i);
      newDataReady = 0;
      t = millis();
    }
  }
   if (Serial.available() > 0) {
    char inByte = Serial.read();
    if (inByte == 't') LoadCell.tareNoDelay();
  }
*/

  // check if last tare operation is complete:
  if (LoadCell.getTareStatus() == true) {
    Serial.println("Tare complete");
  }
int  loadData = LoadCell.getData();
   Serial.print("Load_cell output val: ");
      Serial.println(loadData);
 //Led Loop
 if(loadData < 15)
 
 {
  int i;
for (i=0; i < 34; i++)
{
   leds[i] = CRGB::Black;
  FastLED.show();
}
 }
else if (loadData < 230)
{
   int i;
for (i=0; i < 34; i++)
{
   leds[i] = CRGB::Red;
  FastLED.show();
}
 }
 
else if (loadData < 270)
{
   int i;
for (i=0; i < 34; i++)
{
   leds[i] = CRGB::Yellow;
  FastLED.show();
}
 }

else
{
   int i;
for (i=0; i < 34; i++)
{
   leds[i] = CRGB::Green;
  FastLED.show();
}
 }
}
