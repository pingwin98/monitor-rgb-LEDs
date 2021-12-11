/*
  projekt: oswietlenie led do podstawki monitora
  autor: Karol
  rev: 0.1

  TODO:
  -create git
  -add ability to change palettes (button or so)
  -add abillity to change patterns (brightening and dimming <-> chain LED switching)
  -BT module
  -Add more palletes, better than current one
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

// Gradient palette "Tropical_Colors_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/ggr/tn/Tropical_Colors.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 116 bytes of program space.
DEFINE_GRADIENT_PALETTE( Tropical_Colors_gp ) {
    0,   1,  4,  1,
   14,   1, 10,  1,
   21,   1, 19,  0,
   35,   1, 42,  1,
   49,   3, 75,  1,
   59,  56,103,  1,
   70, 208,135,  4,
   84, 217, 54,  2,
   98, 224, 10,  1,
  130, 117, 15,  3,
  141,  48, 20,  6,
  155,  42, 18, 77,
  170,  36, 17,255,
  197,  55, 97,242,
  210,  80,255,228,
  220,  47,219, 77,
  225,  24,187, 10,
  228,  24,180,  9,
  230,  24,171,  8,
  232,  23,161,  6,
  235,  23,149,  5,
  237,  22,136,  4,
  240,  22,124,  2,
  242,  21,112,  1,
  245,  21,100,  1,
  247,  20, 88,  1,
  250,  19, 77,  1,
  252,  19, 66,  1,
  255,  18, 55,  0};

//-------------
//---global
uint8_t index = 0;
//-------------
void setup() {
  // put your setup code here, to run once:
  delay( 3000 );
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(  BRIGHTNESS );

  currentPalette = Tropical_Colors_gp;
  currentBlending = NOBLEND;

}

void loop() {
  // put your main code here, to run repeatedly:
  index =random(0,116);

  FillLEDsFromPalleteColors(index); //dopisać funkcje wypełniająca ledy kolorem o danym indeksie z palety


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
  
}


void FillLEDsFromPalleteColors(uint8_t colorIndex) 
{
    
    for( int i = 0; i < NUM_LEDS; ++i) {
        leds[i] = ColorFromPalette( currentPalette, colorIndex, BRIGHTNESS, currentBlending);
    }
}

