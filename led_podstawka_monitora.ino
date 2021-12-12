/*
  projekt: oswietlenie led do podstawki monitora
  autor: Karol
  rev: 0.1.2

  TODO:
  -add function to change pallete
  -add function to change pattern
  -add abillity to change patterns (brightening and dimming <-> chain LED switching <-> linear changes)
  -BT module
  -what is blending??
*/
#include <FastLED.h>

#define LED_PIN     3
#define NUM_LEDS    6
#define BRIGHTNESS  200
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
#define DELAY_TIME 5
#define CHAIN_TIME_MS 100
#define LINEAR_TIME_MS 25
#define BUTTON_PIN 2
#define DIMMING 0
#define CHAIN 1
#define LINEAR 2
#define DIMMING_COLOR_INCREMENT 16
#define CHAIN_COLOR_INCREMENT 16
#define LINEAR_COLOR_INCREMENT 1
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

void ChoosePalette(uint8_t index){
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
uint8_t pallete_index = 0;
volatile uint16_t counter = 0;
uint8_t colorIndexIncrement = DIMMING_COLOR_INCREMENT;
uint8_t pattern = DIMMING;

long debouncing_time = 15; //Debouncing Time in Milliseconds
volatile unsigned long last_micros;
volatile bool previusBtnState = 1;
//-------------
//----ISRs
ISR(TIMER1_COMPA_vect){
  //increment counter
  counter++;
}
void BUTTON_CHANGE (){
  if((long)(micros() - last_micros) >= debouncing_time * 1000) {
  if(previusBtnState == 1){ 
    counter = 0; 
    Serial.println("falling");
    previusBtnState = 0;
  }else{ 
    Serial.println("rising");
    if(counter <= 1000){
      ChangeCurrentPaletteIndex();
    }
    else{
      ChangeColorTransitionPattern();
    }
    previusBtnState = 1;
    counter = 0;
  }
  last_micros = micros();
  }

}
//----

void setup() {
  cli(); //stop interrupt
  delay( 3000 );
  pinMode(BUTTON_PIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), BUTTON_CHANGE, CHANGE);  
  //----setting up the timer1 (interrupt every 1ms)
  TCCR1A = 0; //clear register on startup (to run the timer OK every time)
  TCCR1B = 0;
  TCCR1B |= 0b011; // prescaler 64
  TCCR1B |= (1<<WGM12); //turn on CTC mode
  TCNT1 = 0; //initial timr1 value of 0
  OCR1A = 249; // to have 1000hz interrupt (16 000 000/(64*1000)-1)
  TIMSK1 |= (1<<OCIE1A); // turn on interrupt on compare Channel A
  
  //----
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(  BRIGHTNESS );
  Serial.begin(9600); // for debugging

  ChoosePalette(pallete_index);
  
  currentBlending = LINEARBLEND;

  sei(); //allow interrupts
}

void loop() {
  ChoosePalette(pallete_index);
  switch (pattern){
    case DIMMING:
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

      index = index + colorIndexIncrement;
      break;

    case CHAIN:
      FastLED.setBrightness(BRIGHTNESS);
      FillLEDsChainwise(index);
      index = index + colorIndexIncrement;
      break;

    case LINEAR:
      FastLED.setBrightness(BRIGHTNESS);
      FillLEDsFromPalleteColors(index);
      FastLED.show();
      FastLED.delay(LINEAR_TIME_MS);
      index = index + colorIndexIncrement;
      break;
  }

}


void FillLEDsFromPalleteColors(uint8_t colorIndex) 
{
    //Serial.println(colorIndex);
    for( int i = 0; i < NUM_LEDS; ++i) {
        leds[i] = ColorFromPalette( currentPalette, colorIndex, BRIGHTNESS, currentBlending);
    }
}

void FillLEDsChainwise(uint8_t colorIndex){
    for( int i = 0; i < NUM_LEDS; ++i) {
        leds[i] = ColorFromPalette( currentPalette, colorIndex, BRIGHTNESS, currentBlending);
        FastLED.show();
        FastLED.delay(CHAIN_TIME_MS);
    }

}

void ChangeCurrentPaletteIndex(){
  Serial.println("color change");
  if(pallete_index == 2){
    pallete_index = 0; 
  }
  else {
    pallete_index = pallete_index + 1;
  }
}

void ChangeColorTransitionPattern(){
  Serial.println("pattern change");
  if(pattern == DIMMING){
    pattern = CHAIN;
    colorIndexIncrement = CHAIN_COLOR_INCREMENT;
  }
  else if(pattern == CHAIN){
    pattern = LINEAR;
    colorIndexIncrement = LINEAR_COLOR_INCREMENT;
  }
  else {
    pattern = DIMMING;
    colorIndexIncrement = DIMMING_COLOR_INCREMENT;
  }

}
