#include <stdlib.h>
#include "pitches.h"

// LED Driver Pins
#define LED_RED 9
#define LED_GREEN 10
#define LED_BLUE 11

// Color definitions, using 0x0RGB
#define RED 0x00FF
#define GREEN 0x0F0F
#define BLUE 0x0FF0
#define WHITE 0x0000
#define YELLOW 0x000F
#define GREY 0x0888
#define BLACK 0x0FFF
#define PURPLE 0x00F0
#define LIGHTGREEN 0x0808
#define LIGHTBLUE 0x0880
#define LIGHTRED 0x0088

// Timing intervals
#define FADE_INTERVAL_MS 10  //milliseconds 

// Piezo pin definitions
#define PIEZO 6

#define DEBUG 1

// notes in the melody:
int melody[] = {
  // NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4
  NOTE_F2, NOTE_FS2, NOTE_E2, NOTE_F2, NOTE_F2, NOTE_FS2, NOTE_E2, NOTE_F2, NOTE_F2, NOTE_FS2, NOTE_E2, NOTE_F2, NOTE_F2, NOTE_FS2, NOTE_E2, NOTE_F2, 
};

// note durations: 4 = quarter note, 8 = eighth note, etc.:
int noteDurations[] = {
  2, 2, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5
};

void Play_Note(int note, int duration) {
      // to calculate the note duration, take one second
    // divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000 / duration;
    tone(PIEZO, note, noteDuration);
    delay(noteDuration);
    // stop the tone playing:
    noTone(PIEZO);
    int pauseBetweenNotes = noteDuration * 0.25;
    delay(pauseBetweenNotes);

    //delay(pauseBetweenNotes);
}

// Set LED intensities. "color" is 4 bits each for RGB. Duplicate to get a full byte
void LED_Display(unsigned int color, unsigned int fade_color, boolean fade) {
  unsigned int red = (color >> 8) & 0xF;
  red |= (red << 4);
  unsigned int green = (color >> 4) & 0xF;
  green |= (green << 4);
  unsigned int blue = color & 0xF;
  blue |= (blue << 4);
  #if defined(DEBUG)
  Serial.print("Red: ");
  Serial.println(red,HEX);
  Serial.print("Green: ");
  Serial.println(green,HEX);
  Serial.print("Blue: ");
  Serial.println(blue,HEX);
  #endif
  if (fade) {
    unsigned int fade_red = (fade_color >> 8) & 0xF;
    fade_red |= (fade_red << 4);
    unsigned int fade_green = (fade_color >> 4) & 0xF;
    fade_green |= (fade_green << 4);
    unsigned int fade_blue = fade_color & 0xF;
    fade_blue |= (fade_blue << 4);
 
    int red_inc = 0;
    if (fade_red > red) red_inc = -1;
    else if (fade_red < red) red_inc = 1;
    int green_inc = 0;
    if (fade_green > green) green_inc = -1;
    else if (fade_green < green) green_inc = 1;
    int blue_inc = 0;
    if (fade_blue > blue) blue_inc = -1;
    else if (fade_blue < blue) blue_inc = 1;
    
    for (int i=0; i<255; i++) {
      if (fade_red != red) fade_red += red_inc;
      if (fade_green != green) fade_green += green_inc;
      if (fade_blue != blue) fade_blue += blue_inc;\
      analogWrite(LED_RED,fade_red);
      analogWrite(LED_GREEN, fade_green);
      analogWrite(LED_BLUE, fade_blue);
      delay(FADE_INTERVAL_MS);
    }
  }
  analogWrite(LED_RED,red);
  analogWrite(LED_GREEN, green);
  analogWrite(LED_BLUE, blue);
}

void setup() {

  // Configure the LEDs and turn them off
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);
  pinMode(PIEZO, OUTPUT);
  LED_Display(BLACK,BLACK,false);

  // start the serial library:
  #if defined(DEBUG)
  Serial.begin(9600);
  #endif  
  
  randomSeed(analogRead(0));

  

}

void loop() {
  
  float photo_voltage = 5.0;
  
  while (photo_voltage > 3.0) {
    // read the input on analog pin 0:
    int sensorValue = analogRead(A0);
    // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
    photo_voltage = sensorValue * (5.0 / 1023.0);
    // print out the value you read:
    Serial.println(photo_voltage);
    delay(1000);
  }

  LED_Display(WHITE,WHITE,false);
  delay(200);
  LED_Display(BLACK,BLACK,false);
  delay(200);
  LED_Display(RED,RED,false);
  delay(200);
  LED_Display(GREEN,GREEN,false);
  delay(200);
  LED_Display(BLUE,BLUE,false);
  delay(200);
    
  for (int i=0; i<16; i++) {
    Play_Note(melody[i], noteDurations[i]);
    LED_Display(random(0,0xFFF),BLACK,false);
  }  
  
  for (int i=20; i < 2000; i=i+10) {
    Play_Note(i,50);
    LED_Display(random(0,0xFFF),BLACK,false);
    // delay(10);
  }  
  for (int i=2000; i > 0; i=i-10) {
    Play_Note(i,50);
    LED_Display(random(0,0xFFF),BLACK,false);
    // delay(10);
  }  
  for (int i=1; i < 100; i=i+1) {
    tone(PIEZO, random(100,1000));
    LED_Display(random(0,0xFFF),BLACK,false);
    delay(10);
  }  
 noTone(PIEZO);
 
 LED_Display(BLACK,BLACK,false);
  
}


