// receive MIDI on serial port and light up corresponding LED
// by Rubén Mena & David Sattelmaier / May 2022

// Adafruit NeoPixel library
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

// Which pin on the Arduino is connected to the NeoPixels?
#define PIN        7 

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS 96 // Popular NeoPixel ring size 
#define OFFSET 41 // tells us the value of the first Note's noteByte (use hairless to get this info)

// When setting up the NeoPixel library, we tell it how many pixels,
// and which pin to use to send signals. Note that for older NeoPixel
// strips you might need to change the third parameter -- see the
// strandtest example for more information on possible values.
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

// global vars
byte commandByte;
byte noteByte;
byte velocityByte;
byte noteOn = 152;
byte noteOff = 128;  //???
byte maxled = NUMPIXELS-1;
bool invert = true; //lights up LEDs if true => from maxled to 0 | if false => from 0 to maxled 

void setup() {

  Serial.begin(9600);
  
  // These lines are specifically to support the Adafruit Trinket 5V 16 MHz.
  // Any other board, you can remove this part (but no harm leaving it):
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif
  // END of Trinket-specific code.

  pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
  pixels.clear(); // Set all pixel colors to 'off'
  
}

void checkMIDI(){
  do{
    if (Serial.available()){

      commandByte = Serial.read();//read first byte
      noteByte = Serial.read();//read next byte
      velocityByte = Serial.read();//read final byte

      // when key released, then velocity equals 0
      if (velocityByte > 0) {
        if (invert)
          pixels.setPixelColor(maxled-(noteByte-OFFSET), pixels.Color(noteByte, 20, velocityByte));
        else
          pixels.setPixelColor(noteByte-OFFSET, pixels.Color(noteByte, 20, velocityByte));
        pixels.show();
      }
      else {
         if (invert)
           pixels.setPixelColor(maxled-(noteByte-OFFSET), pixels.Color(0, 0, 0));
         else
           pixels.setPixelColor(noteByte-OFFSET, pixels.Color(0, 0, 0));
        pixels.show();
      }
    }
  }
  while (Serial.available() > 2);//when at least three bytes available
}

// Main loop
void loop() { 
  checkMIDI();
  delay(50);
}
