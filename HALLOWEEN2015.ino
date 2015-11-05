 
#include <SoftwareSerial.h>

#include <Adafruit_NeoPixel.h>
#include <Wire.h>
#include "rgb_lcd.h"

rgb_lcd lcd;

const int colorR = 90;
const int colorG = 0;
const int colorB = 90;

#ifdef __AVR__
  #include <avr/power.h>
#endif


#define NUM_LEDS  75
#define NEOPIXEL 13
#define NUM_TRACKS 8
#define PIR_MOTION_SENSOR 2//Use pin 2 to receive the signal from the module 
#define TRACK_TIME 3000 //appx 3 seconds

//ChainableLED leds(7, 8, NUM_LEDS);
SoftwareSerial mp3(4, 5);//modify this with the connector you are using.
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, NEOPIXEL, NEO_GRB + NEO_KHZ800);

void setup()
{
    mp3.begin(9600);
  Serial.begin(9600); 
  delay(100);
        if (true == SetPlayMode(0x00))
        Serial.println("Set The Play Mode to 0x01, Single Loop Mode.");
        else
        Serial.println("Playmode Set Error");
        SetVolume(0x0E);
        //PauseOnOffCurrentMusic();
        pinMode(PIR_MOTION_SENSOR, INPUT);
        //SetMusicPlay(00,01);
        strip.begin();
        strip.show(); // Initialize all pixels to 'off'
        // set up the LCD's number of columns and rows:
    lcd.begin(16, 2);
    
    lcd.setRGB(colorR, colorG, colorB);
    
    // Print a message to the LCD.
    lcd.write("Happy Halloween!");
    lcd.setCursor(1,1);
    lcd.write("      DTC");
    

    delay(1000);
}



void loop()
{
  
  if (checkRun()){
  }
  else
    noMotion();
    //lcd.setRGB(90,40,0);
    //delay(500);
}

boolean checkRun()
{
    if(isPeopleDetected()){//if it detects the moving people?
    onMotion();
    }
}


//SOMEONE MOVED
void onMotion()
{

  nextTrack();
  int playTime = 0;

  //White Theater Chase of LEDs
  while (playTime < TRACK_TIME){
      theaterChase(strip.Color(127, 127, 127), 100); 
      playTime = TRACK_TIME;
  } 
  
}


//NO ONE MOVED, so just rainbow a while. 
void noMotion()
{

  rainbowCycle(100);
  //colorWipe(strip.Color(255, 0, 0), 0);
  //colorWipe(strip.Color(0, 255, 0), 0);
  //colorWipe(strip.Color(0, 0, 255), 0);
  
}

/***************************************************************/
/*Function: Detect whether anyone moves in it's detecting range*/
/*Return:-boolean, ture is someone detected.*/
boolean isPeopleDetected()
{
  int sensorValue = digitalRead(PIR_MOTION_SENSOR);
  if(sensorValue == HIGH)//if the sensor value is HIGH?
  {
    return true;//yes,return ture
  }
  else
  {
    return false;//no,return false
  }
}
//-------------------------------------------------------

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;
  
  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    //lcd.setPWM(Wheel((j) & 255), 100);
    checkRun();
    strip.show();
    delay(wait);
  }
    //lcd.clear();
    //lcd.write("Happy Halloween DTC!");
}

//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait) {
  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, c);    //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < 3; q++) {
      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

//--------------------------------------------------------
//Set the music index to play, the index is decided by the input sequence
//of the music;
//hbyte: the high byte of the index;
//lbyte: the low byte of the index;
boolean SetMusicPlay(uint8_t hbyte,uint8_t lbyte)
{
  mp3.write(0x7E);
  mp3.write(0x04);
  mp3.write(0xA0);
  mp3.write(hbyte);
  mp3.write(lbyte);
  mp3.write(0x7E);
        delay(10);
   while(mp3.available())
  {
  if (0xA0==mp3.read())
  return true;
  else 
  return false;
        }
}

// Pause on/off  the current music
boolean nextTrack(void)
{
 mp3.write(0x7E);
 mp3.write(0x02);
 mp3.write(0xA5);
 mp3.write(0x7E);
 delay(10);
 while(mp3.available())
 {
 if (0xA5==mp3.read())
 return true;
 else 
 return false;
 }
}

// Pause on/off  the current music
boolean PauseOnOffCurrentMusic(void)
{
 mp3.write(0x7E);
 mp3.write(0x02);
 mp3.write(0xA3);
 mp3.write(0x7E);
 delay(10);
 while(mp3.available())
 {
 if (0xA3==mp3.read())
 return true;
 else 
 return false;
 }
}

//Set the volume, the range is 0x00 to 0x1F
boolean SetVolume(uint8_t volume)
{
 mp3.write(0x7E);
 mp3.write(0x03);
 mp3.write(0xA7);
 mp3.write(volume);
 mp3.write(0x7E);
 delay(10);
 while(mp3.available())
 {
 if (0xA7==mp3.read())
 return true;
 else 
 return false;
 }
}

boolean SetPlayMode(uint8_t playmode)
{
  if (((playmode==0x00)|(playmode==0x01)|(playmode==0x02)|(playmode==0x03))==false)
  {
    Serial.println("PlayMode Parameter Error! ");
    return false;
  }
   mp3.write(0x7E);
 mp3.write(0x03);
 mp3.write(0xA9);
 mp3.write(playmode);
 mp3.write(0x7E);
 delay(10);
 while(mp3.available())
 {
 if (0xA9==mp3.read())
 return true;
 else 
 return false;
 }
}
