#define FASTLED_INTERNAL 3
#include <FastLED.h>



//Código muy poco generico y demasiado especìfico para ciertas medidas, si veis que un condicional o un bucle tiene valores aleatorios es por eso.





int sensorAnalogPin = A4;
int dif=0;
int readd;


#define LED_PIN     9
#define NUM_LEDS    95 //NÚMERO DE LEDS EN LA TIRA
#define BRIGHTNESS  45 // BRILLO
#define LED_TYPE    WS2811 //CONTROLADOR DE LA TIRA
#define COLOR_ORDER GRB
#define nOpciones 3
#define inicio 10


extern CRGBPalette16 myRedWhiteBluePalette;
extern const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM;
CRGB leds[NUM_LEDS];

//Elegir los colores (CRGB)
CRGB color=CRGB::MediumOrchid;
CRGB colortop=CRGB::HotPink;

                              
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
  //elegir paleta de colores para el "modo estático"

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
  //Revisar estado del botón
  if (digitalRead(buttonPin)==LOW){
    opc++;
    delay(250);
    if (opc> nOpciones - 1) opc=0; 
    }
  //Serial.println(opc);
    if (opc==0){
    //Cambiar determina si se deben volver a encender los leds de más abajo además de apagar todos los leds para que se vuelva a usar bien(se entiende mejor si vemos cuando se niega más abajo)
    if(cambiar){
      for(int h=0;h<NUM_LEDS;h++){
        leds[h]=CRGB::Black;
      }
        for(int p=0;p<inicio;p++){
      leds[p]=color;
       }
       cambiar=false;
       FastLED.show();
      }
    int abss=readd - average;
    dif=abs(abss);

    hacer=dif*3;
    //control para que no haya violación de segmento si debemos de iluminar más leds de los que hay en la tira(solo llega hasta la mitad de la tira)
    if (hacer >38){
      hacer=38;
      }
    //Bucle para recorrer todos los leds y encenderlos
    //el recorrido para encender los leds es desde las dos puntas hasta el centro por eso el (i++,z--)
    for (int i=inicio,z=94;i<(hacer+inicio);i++,z--){
      //condicional para iluminar los de arriba de diferente color
        if(i>40){
          leds[(i)]=colortop;
          leds[(z)]=colortop;
          if(hacer==38){
           leds[48]=colortop;
           leds[49]=colortop;
           leds[50]=colortop;
           leds[51]=colortop;
           leds[52]=colortop;
           leds[53]=colortop;
           leds[54]=colortop;
           leds[55]=colortop;
           leds[56]=colortop;

          }
      }
      else{
        leds[(i)]=color;
        leds[(z)]=color;
        }          
      }
      //Mostrar los leds una vez cambiados
        FastLED.show();

      //Apagar los leds==Decir que su color sea negro
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
      //Este condicional evita en una pequeña parte el parpadeo ya que si la diferencia a la media actual es igual que la anterior no apagará los leds ya que se deben iluminar hasta el mismo sitio
      if(dif!=difa) FastLED.show();
      difa=dif;
  }else{
    //Esta es la parte del código para que sea de un color estático o en este caso usando una paleta de colores que trae la librería 
    static uint8_t startIndex = 0;
    startIndex = startIndex + 1; /* motion speed */
    
    FillLEDsFromPaletteColors( startIndex,opc);
    
    FastLED.show();
    FastLED.delay(1000 / 100);
    if (!cambiar) cambiar=true;
    }
    }


void FillLEDsFromPaletteColors( uint8_t colorIndex,int opc)
{
    uint8_t brightness = 255;

    if(opc==1){
      currentPalette = RainbowColors_p;
      currentBlending = LINEARBLEND;
      //Serial.println(currentBlending);
    }
    if(opc==2){
    currentPalette = CRGBPalette16(
                                   CRGB::DarkMagenta, CRGB::DarkMagenta,  CRGB::DarkViolet,  CRGB::DarkMagenta,
                                   CRGB::DarkMagenta, CRGB::DarkViolet, CRGB::DarkViolet,  CRGB::DarkMagenta,
                                   CRGB::DarkMagenta,  CRGB::DarkViolet,  CRGB::DarkMagenta,  CRGB::DarkMagenta,
                                   CRGB::DarkViolet, CRGB::DarkViolet, CRGB::DarkMagenta,  CRGB::DarkMagenta );
    currentBlending = LINEARBLEND;
    //Serial.println(currentBlending);
    }

    
    /*
     HACER TU PROPIA OPCIÓN:
     -Cambiar el #define nOpciones al numero de opciones correcto
     
     -Rellenar la siguiente plantilla:
        if(opc==<Número de la opción>){
           currentPalette = CRGBPalette16(
                                     <Color 1>, <Color 2>,  <Color 3>,  <Color 4>,
                                     <Color 5>, <Color 6>,  <Color 7>,  <Color 8>,
                                     <Color 9>, <Color 10>, <Color 11>, <Color 12>,
                                     <Color 13>,<Color 14>, <Color 15>,  <Color 16> 
                                     );
      currentBlending = <LINEARBLEND/NOBLEND>;
      }
      
    
    
    */

    
    
    for( int i = 0; i < NUM_LEDS; i++) {
        leds[i] = ColorFromPalette( currentPalette, colorIndex, brightness, currentBlending);
        colorIndex += 3;
    }
}
