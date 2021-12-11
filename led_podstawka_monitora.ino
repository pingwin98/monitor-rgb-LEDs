/*
  projekt: oswietlenie led do podstawki monitora
  autor: Karol
  rev: 0.1.2

  TODO:
  -add ability to change palettes (button or so)
  -add abillity to change patterns (brightening and dimming <-> chain LED switching <-> linear changes)
  -BT module
  -Add oalettes better than current one
  -what is blending??
*/
#include <FastLED.h>

#define LED_PIN     3
#define NUM_LEDS    6
#define BRIGHTNESS  200
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
#define DELAY_TIME 5
CRGB leds[NUM_LEDS];

CRGBPalette16 currentPalette;
TBlendType    currentBlending;

//----paletts

void Setup1Pallete(){
//ustawienie pierwszej palety  RGB
CRGB red = CHSV(HUE_RED,255,255);
CRGB green = CHSV(HUE_GREEN,255,255);
CRGB blue = CHSV(HUE_BLUE,255,255);

currentPalette = CRGBPalette16(red, green, blue,
                              red, green, blue,
                              red, green, blue,
                              red, green, blue,
                              red, green, blue, green);
}

void Setup2Pallete(){
//ustawienie drugiej palety 
CRGB orange = CHSV(HUE_ORANGE,255,255);
CRGB purple = CHSV(HUE_PURPLE,255,255);
CRGB aqua = CHSV(HUE_AQUA,255,255);

currentPalette = CRGBPalette16(orange, purple, aqua,
                                orange, purple, aqua,
                                orange, purple, aqua,
                                orange, purple, aqua,
                                orange, purple, aqua, purple);
} 

void Setup3Pallete(){
//ustawienie trzeciej palety 
CRGB yellow = CHSV(HUE_YELLOW,255,255);
CRGB pink = CHSV(HUE_PINK,255,255);
CRGB aqua = CHSV(HUE_AQUA,255,255);

currentPalette = CRGBPalette16(yellow, pink, aqua,
                              yellow, pink, aqua,
                              yellow, pink, aqua,
                              yellow, pink, aqua,
                              yellow, pink, aqua, pink);
}

void ChangePallete(uint8_t index){
  if(index == 0){
    Setup1Pallete();
  }
  else if(index == 1){
    Setup2Pallete();
  }
  else if(index == 2){
    Setup3Pallete();
  }
  else {
    currentPalette = CRGBPalette16(CRGB::White,CRGB::DarkSeaGreen,CRGB::BlueViolet); // zmienić na 16 pozycji
  }
}


//-------------
//---global
uint8_t index = 0;
uint8_t pallete_index = 1;
//-------------
void setup() {
  // put your setup code here, to run once:
  delay( 3000 );
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(  BRIGHTNESS );
  Serial.begin(9600); // for debugging

  ChangePallete(pallete_index);
  //currentPalette = Tropical_Colors_gp;
  currentBlending = NOBLEND;

}

void loop() {
  // put your main code here, to run repeatedly:
  FillLEDsFromPalleteColors(index); 

  FastLED.show();
  for(int i = 0; i <255;i++){
    FastLED.setBrightness(i);
    FastLED.show();
    FastLED.delay(DELAY_TIME);
  }
  for(int i = 0; i <255;i++){
    FastLED.setBrightness(255-i);
    FastLED.show();
    FastLED.delay(DELAY_TIME);
  }
  
  index = index + 16; 

}


void FillLEDsFromPalleteColors(uint8_t colorIndex) 
{
    Serial.println(colorIndex);
    for( int i = 0; i < NUM_LEDS; ++i) {
        leds[i] = ColorFromPalette( currentPalette, colorIndex, BRIGHTNESS, currentBlending);
    }
}

