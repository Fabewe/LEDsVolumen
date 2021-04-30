#include <FastLED.h>

int sensorAnalogPin = A4;
int dif=0;
int readd;


#define LED_PIN     9
#define NUM_LEDS    95
#define BRIGHTNESS  45
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
#define inicio 10


extern CRGBPalette16 myRedWhiteBluePalette;
extern const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM;
CRGB leds[NUM_LEDS];
CRGB color=CRGB::Green;
                              
const int numReadings = 20;

int readings[numReadings];
int readIndex = 0;
int total = 0;
int average = 0;

int difa=0;
int hacer=0;
bool cambiar=true;
int opc=0;
int val;
int buttonPin = 2;
CRGBPalette16 currentPalette;
TBlendType    currentBlending;
void arcoiris(int vel=1);
void FillLEDsFromPaletteColors( uint8_t colorIndex);
void setup()
{
  //añadir leds y establecer pines
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(  BRIGHTNESS );
  pinMode(sensorAnalogPin,INPUT);
  pinMode(buttonPin,INPUT);
  currentPalette = RainbowColors_p;
  currentBlending = LINEARBLEND;

  //Serial.begin(9600);
  
  //primera media
  for (int thisReading = 0; thisReading < numReadings; thisReading++) {
    readings[thisReading] = 0;
  }
  for (int x=0;x<numReadings;x++){
   readd=analogRead(sensorAnalogPin);
    total = total - readings[readIndex];  
    readings[readIndex] = readd;  
  total = total + readings[readIndex];  
  readIndex = readIndex + 1;
  if (readIndex >= numReadings) {  
    readIndex = 0; }
   average = total / numReadings;
  }
  //leds inferiores [0-9]

    currentPalette = RainbowColors_p;
    currentBlending = LINEARBLEND;
    FastLED.show();

}

void loop(){
  //media
  readd=analogRead(sensorAnalogPin);  
  total = total - readings[readIndex];  
  readings[readIndex] = readd;  
  total = total + readings[readIndex];  
  readIndex = readIndex + 1;  
  if (readIndex >= numReadings) {  
    readIndex = 0; }  
  average = total / numReadings;
  
  if (digitalRead(buttonPin)==LOW){
    opc++;
    delay(250);
    if (opc>1) opc=0; 
    }
  //Serial.println(opc);
    if (opc==0){
    if(cambiar){
      for(int h=0;h<NUM_LEDS;h++){
        leds[h]=CRGB::Black;
      }
        for(int p=0;p<inicio;p++){
      leds[p]=CRGB::HotPink;
       }
       cambiar=false;
       FastLED.show();
      }
    int abss=readd - average;
    dif=abs(abss);

    hacer=dif*3;

    if (hacer >38){
      hacer=38;
      }
    for (int i=inicio,z=94;i<(hacer+inicio);i++,z--){
        if(i>40){
          leds[(i)]=0x800080;
          leds[(z)]=0x800080;
          if(hacer==38){
           leds[48]=0x800080;
           leds[49]=0x800080;
           leds[50]=0x800080;
           leds[51]=0x800080;
           leds[52]=0x800080;
           leds[53]=0x800080;
           leds[54]=0x800080;
           leds[55]=0x800080;
           leds[56]=0x800080;

          }
      }
      else{
        leds[(i)]=CRGB::HotPink;
        leds[(z)]=CRGB::HotPink;
        }          
      }
        FastLED.show();


      for (int j=hacer+inicio,x=94-hacer;j>=inicio;j--,x++){
        leds[j]=CRGB::Black;
        leds[x]=CRGB::Black;
        if(hacer==38){
          leds[48]=CRGB::Black;
          leds[49]=CRGB::Black;
          leds[50]=CRGB::Black;
          leds[51]=CRGB::Black;
          leds[52]=CRGB::Black;
          leds[53]=CRGB::Black;
          leds[54]=CRGB::Black;
          leds[55]=CRGB::Black;
          leds[56]=CRGB::Black;
          }
      }
      if(dif!=difa) FastLED.show();
      difa=dif;
  }else{
     //Serial.println(7);
      arcoiris();
    if (!cambiar) cambiar=true;
    }
    }

void arcoiris(int vel){
    if (vel<=0) vel=1;
    static uint8_t startIndex = 0;
    startIndex = startIndex + vel; /* motion speed */
    
    FillLEDsFromPaletteColors( startIndex);
    
    FastLED.show();
    FastLED.delay(1000 / 100);
}
void FillLEDsFromPaletteColors( uint8_t colorIndex)
{
    uint8_t brightness = 255;
    
    for( int i = 0; i < NUM_LEDS; i++) {
        leds[i] = ColorFromPalette( currentPalette, colorIndex, brightness, currentBlending);
        colorIndex += 3;
    }
}
  


    
 
