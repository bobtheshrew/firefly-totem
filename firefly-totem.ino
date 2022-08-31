#include <FastLED.h>

#define LED_PIN     7
#define NUM_LEDS    70//87

CRGB leds[NUM_LEDS];
CRGB black = CRGB::Black;
CRGB white = CRGB::NavajoWhite;

int min = 0;
int max = NUM_LEDS-1;
int r = 0;
int r2 = 0;
static uint8_t hue = 0;
unsigned long doneMillis = 0;

void setup() {
  Serial.begin(9600);
  Serial.print("Starting setup...");
  Serial.println();

  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
  //Initialization lights 
  leds[0] = CRGB(255, 0, 0);
  FastLED.show();
  delay(50);  
  leds[1] = CRGB(0, 255, 0);
  FastLED.show();
  delay(50);
  leds[2] = CRGB(0, 0, 255);
  FastLED.show();
  delay(50);
  leds[69] = CRGB::White;
  FastLED.show();
  delay(50);
  Serial.print("Setup complete.");
  Serial.println();
}

void loop() {
  //TODO rewite each routine as a function to return a frame? or accept a time to run?
  //TODO: Star field



  //yellow sparkle
  doneMillis = millis() + 15000;
  while (doneMillis > millis())
  {
    for (int i = 2; i < NUM_LEDS; i++) {
      //iluminate the star w/ 1 pixel streamer
      leds[i-2] = CRGB(255, 165, 0);          
      leds[i-1] = CRGB(255, 200, 128);    
      leds[i] = white;          
      sparkle();
      delay(18);
      FastLED.show();
      fadeAFrame();
    }
    fadeToBlack();
  }
  fadeToBlack();

  //red streaks
  doneMillis = millis() + 15000;
  while (doneMillis > millis())
  {
    for (int i = 0; i < NUM_LEDS; i++) 
    {
      //iluminate the star
      leds[i] = CRGB(255, 0, 0);          
      delay(18);
      FastLED.show();
      fadeAFrame();
      }
      fadeToBlack();
  }
  fadeToBlack();

  //rainbow crackle
  doneMillis = millis() + 15000;
  while (doneMillis > millis())
  {
    for (int i = 1; i < NUM_LEDS; i++) {
      //iluminate the star
      hue = hue + 16;
      leds[i] = CHSV(hue, 255, 255);
      crackle();
      delay(18);
      FastLED.show();
      fadeAFrame();
    }
    fadeToBlack();
  }
  fadeToBlack();

  //Reverse Chasers
  //for (int i = (NUM_LEDS*5); i > 0 ; i--)
  doneMillis = millis() + 15000;
  while (doneMillis > millis())
  {
    static int i = 0;
    i++;
    fadeAFrameFast();
    for (int j = 0; j < 8; j++){
      leds[(NUM_LEDS-1)-((i+(10*j))%NUM_LEDS)]=CHSV(32*j, 0, 255);
    }
    //sparkle();
    delay(36);
    FastLED.show();
  }
  fadeToBlack();

  //Rainbow ping pong
  doneMillis = millis() + 15000;
  while (doneMillis > millis())
  {
      static int i = 0;
      float deg = (i+270)%360;
      float rad = deg * PI / 180;
      int sinI = int((NUM_LEDS/2) + (sin(rad)*(NUM_LEDS/2)) );
      i++;
      leds[sinI]=CHSV(i%255, 255 , 255);          
      delay(5);
      FastLED.show();
      fadeAll(240);  
    }
  fadeToBlack();

  //Fire Flies
  doneMillis = millis() + 15000;
  while (doneMillis > millis())
  {
    static int i = 0;
    //TODO EVEN / ODD Fade different rates
    //TODO Double Pluse
    
    //random LEDs
    r = random(min, max);           
    if ((i%10)==0){
      leds[r]=CHSV(96, 255, 255);      
      leds[r2]=CHSV(96, 255, 255);      
      r2=r;
    }
    delay(50);
    FastLED.show();
    fadeAll(210);
    i++;
  }
  fadeToBlack();

  //gradient
  //0 is a color
  //NUM_LEDS-1 is a color
  //Other leds in between
  //is each channel 0's color + ((i/NUM_LEDS)*(difference))

  leds[0] = CRGB(255,0,0); 
  leds[NUM_LEDS-1] = CRGB(0,0,255); 
  
  int r;
  int g;
  int b;
  
  int h;
  int s;
  int v;
  float ratio;

  int startH=0;
  int endH=255;

  for (int j = 0; j < 750; j++){
    startH++; // =        min(startH + 1,255); 

    float deg = (j*2)%360;
    float rad = deg * PI / 180;
    endH = startH + (sin(rad)*64);
    
    CHSV startCHSV=CHSV((startH), 255, 255);  
    CHSV endCHSV=CHSV((endH), 255, 255);  
    
    leds[0] = startCHSV; 
    leds[NUM_LEDS-1] = endCHSV; 
  
    for (int i = 1; i < (NUM_LEDS-1); i++)
          {

          ratio=float(i)/float(NUM_LEDS);
          r = int( leds[0].r + ( ratio * (leds[NUM_LEDS-1].r-leds[0].r) )   );
          g = int( leds[0].g + ( ratio * (leds[NUM_LEDS-1].g-leds[0].g) )   );
          b = int( leds[0].b + ( ratio * (leds[NUM_LEDS-1].b-leds[0].b) )   );

          //h = int( leds[0].h + ( ratio * (leds[NUM_LEDS-1].h-leds[0].h) )   );
          //s = int( leds[0].s + ( ratio * (leds[NUM_LEDS-1].s-leds[0].s) )   );
          //v = int( leds[0].v + ( ratio * (leds[NUM_LEDS-1].v-leds[0].v) )   );
          
          leds[i]= CRGB(r,g,b);
          //leds[i]= CHSV(h,s,v);

          //fill_gradient(leds,0,startCHSV,NUM_LEDS-1,endCHSV,FORWARD_HUES);
          fill_gradient(leds,0,startCHSV,NUM_LEDS-1,endCHSV,SHORTEST_HUES);
          }
      FastLED.show();
      //delay(60);
    }
  FastLED.show();
  fadeToBlack();


 /* //teal bounce
  for (int i = 0; i < 2; i++)
  {
      fadeAFrameFast();
      for (int j = 0; j < 8; j++){
        for (int ii = 0; ii < (NUM_LEDS)*2; ii++)
        {
        leds[ii]=CHSV(32*j, 255, 255);
        fadeAll(250);
        FastLED.show();
        delay(25);
        }
      }
      //sparkle();
  }
*/
  fadeToBlack();
  
  //Long Rainbow Pulses
  doneMillis = millis() + 15000;
  while (doneMillis > millis())
  {
      fadeAFrameFast();
      for (int j = 0; j < 8; j++){
        for (int ii = 0; ii < (NUM_LEDS); ii++)
        {
        leds[ii]=CHSV(32*j, 255, 255);
        FastLED.show();  //show after each pixel
        fadeAll(250);
        delay(25);
        }
      }
  }
  fadeToBlack();

  
  //red yellow Chasers
  doneMillis = millis() + 15000;
  while (doneMillis > millis())
  {
    static int i = 0;
    fadeAFrameFast();
    for (int j = 0; j < 8; j++){
      leds[(i+(11*j))%NUM_LEDS]=CHSV(50*(j%2), 255, 255);
    }
    //sparkle();
    delay(18);
    FastLED.show();
    i++;
  }
  fadeToBlack();
    
  //Just crackles
  doneMillis = millis() + 15000;
  while (doneMillis > millis())
  {
    fadeAFrame();
    //random sparkles
    r = random(min, max);            
    leds[r]=white;
    delay(36);
    FastLED.show();
  }
  fadeToBlack();

  //Rainbow Chasers
  doneMillis = millis() + 15000;
  while (doneMillis > millis())
  {
    static int i=0;
    fadeAFrameFast();
    for (int j = 0; j < 8; j++){
      leds[(i+(11*j))%NUM_LEDS]=CHSV(32*j, 255, 255);
    }
    //sparkle();
    delay(36);
    FastLED.show();
    i++;
  }
  fadeToBlack();

  //Blue and Violet Chasers
  doneMillis = millis() + 15000;
  while (doneMillis > millis())
  {
    static int i=0;
    leds[(i)%NUM_LEDS]=CRGB::Blue;
    //leds[(i+25)%NUM_LEDS]=CRGB::DarkViolet;
    leds[(i+44)%NUM_LEDS]=CRGB::DarkViolet;
    //leds[(i+75)%NUM_LEDS]=CRGB::DarkViolet;
    delay(36);
    FastLED.show();
    fadeAll(200);
    i++;
  }
  fadeToBlack();



}



//maximize brightness of random pixles that are not black
void sparkle()
{
  //random sparkles
  r = random(min, max);            
  if(leds[r].getAverageLight()!=0)
  {
    leds[r].maximizeBrightness();
  }
}

//random pixles that are not black max to white
void crackle()
{
  //random sparkles
  r = random(min, max);            
  if(leds[r].getAverageLight()!=0)
  {
    leds[r]=white;
  }
}

//fade everyone 1 shade down
void fadeAFrameFast()
{
  for (int i = 0; i < NUM_LEDS; i++) {
    //scales all chanels down at the same time just decreasing luminocity
    leds[i] = leds[i].nscale8(128);
  }
}


//fade everyone 1 shade down
void fadeAFrame()
{
  for (int i = 0; i < NUM_LEDS; i++) {
    //simple subtract will bend color to initial highest chanel
    //leds[i] = leds[i].subtractFromRGB(16);
    //scales all chanels down at the same time just decreasing luminocity
    leds[i] = leds[i].nscale8(225);
  }
}

//fade everyone N/255ths shades down
void fadeAll(int N)
{
  for (int i = 0; i < NUM_LEDS; i++) {
    //scales all chanels down at the same time just decreasing luminocity
    leds[i] = leds[i].nscale8(N);
  }
}

//fade everone to black
void fadeToBlack()
  {
  //fade everything to 0 before looping
  for (int i = 2; i < NUM_LEDS; i++) {
    //fade everyone
    for (int i = 0; i < NUM_LEDS; i++) {
      //leds[i] = leds[i].subtractFromRGB(16);
      leds[i] = leds[i].nscale8(225);
    }
    delay(18);
    FastLED.show();
  }
}

//Color Chart https://github.com/FastLED/FastLED/wiki/Pixel-reference#chsv
// EOF
