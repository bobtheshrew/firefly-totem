#include <FastLED.h>

//TODO:
//topper 100mm sphere 4each level, each level rotated 45deg
//tube cross section: .82in .85in withslot .44in slot width
//solder leads to strips
//cut tubes in 1/4's
//pin for main tube / pin holes
//glue in topper tubes
//print caps
//wire in 12v board power supply: power batt->20to12v->12to5v
//putty for switch hole
//paint power box black / stain grey? x2? use rest of stain?

#define NUM_MODES        20
#define LED_PIN     7
#define NUM_COLUMN_LEDS  64 //0-63
#define NUM_STAR_LEDS    16 //64-79
#define NUM_LEDS         80 //64+16
#define NUM_FIREFLIES    4
#define MODE_SHOW_MILLIS 20000  //number of millis to show a mode

CRGB leds[NUM_LEDS];

CRGB black = CRGB::Black;
CRGB green = CRGB::Green;
CRGB blue = CRGB::Blue;
CRGB yellow = CRGB::Yellow;
CRGB white = CRGB::NavajoWhite;
CRGB orange = CRGB::OrangeRed;
CRGB purples[] = {CRGB::Indigo, CRGB::Violet, CRGB::Purple};
CRGB chartreuse = CRGB::Chartreuse;

int minIdx = 0;
int maxIdx = NUM_LEDS - 1;
int r[NUM_FIREFLIES];
int modes[NUM_MODES];
int r2 = 0;
static uint8_t hue = 0;
unsigned long doneMillis = 0; //var for mode end times

struct Particle {
  int idx;
  int color;
  bool moving;
};
//////////////////////////////////////////////////////////////////////////////////////////////////////
//                                          SETUP                                                   //
//////////////////////////////////////////////////////////////////////////////////////////////////////
void setup() {
  //1 second delay to ensure strip is powered up before initial launch
  delay(1000);
  Serial.begin(9600);
  Serial.print("Starting setup...");
  Serial.println();

  // if analog input pin 0 is unconnected, random analog
  // noise will cause the call to randomSeed() to generate
  // different seed numbers each time the sketch runs.
  // randomSeed() will then shuffle the random function.
  int random_seed = analogRead(0) + analogRead(1) + analogRead(2) + analogRead(3) + analogRead(4) + analogRead(5) + analogRead(6) + analogRead(7);
  Serial.print(random_seed);
  randomSeed(random_seed);

  // lights Initialization
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
  leds[10] = CRGB(255, 0, 0);
  FastLED.show();
  delay(200);
  leds[20] = CRGB(0, 255, 0);
  FastLED.show();
  delay(200);
  leds[30] = CRGB(0, 0, 255);
  FastLED.show();
  delay(200);
  leds[NUM_LEDS - 1] = CRGB::White;
  FastLED.show();
  delay(200);
  fadeToBlack();

  //initialize modes
  for (int i = 0; i < NUM_MODES; i++) {
    modes[i] = i;
  }

  //shuffle the mode order
  for (int i = 0; i < NUM_MODES; i++) {
    int n = random(0, NUM_MODES);  // Integer from 0 to NUM_MODES-1
    int temp = modes[n];
    modes[n] =  modes[i];
    modes[i] = temp;
  }
  Serial.print("Setup complete.");
  Serial.println();
}//end init

//////////////////////////////////////////////////////////////////////////////////////////////////////
//                                        LOOP                                                      //
//////////////////////////////////////////////////////////////////////////////////////////////////////
void loop() {
  //DEBUG - Direct Calls
  christmasCrackles();
  fallingLeaves();

  for (int i = 0; i < NUM_MODES; i++) {
    switch (modes[i]) {
      case 0:
        starfield();
        break;
      case 1:
        fireFlies();
        break;
      case 2:
        yellowSparkle();
        break;
      case 3:
        blueStreaks();
        break;
      case 4:
        rainbowCrackle();
        break;
      case 5:
        rainbowChasers();
        break;
      case 6:
        pingPongWhite();
        break;
      case 7:
        reverseRainbowChasers();
        break;
      case 8:
        pingPongRainbow();
        break;
      case 9:
        rotatingGradient();
        break;
      case 10:
        longRainbowPulses();
        break;
      case 11:
        redYellowChasers();
        break;
      case 12:
        justCrackles();
        break;
      case 13:
        blueAndVioletChasers();
        break;
      case 14:
        tvStatic();
        break;
      case 15:
        halloween();
        break;
      case 16:
        daisy();
        break;
      case 17:
        fallingLeaves();
        break;
      case 18:
        americanCrackles();
        break;
      case 19:
        christmasCrackles();
        break;
      default:
        // statements
        break;
    }//end switch
  }//end for
}//end loop



/////////
//MODES//
/////////

///////////////////////
// American Crackles //
///////////////////////
void americanCrackles() {
  threeCrackles(CRGB::Red, CRGB::White, CRGB::Blue);
}

////////////////////////
// Christmas Crackles //
////////////////////////
void christmasCrackles() {
  threeCrackles(CRGB::Red, CRGB::White, CRGB::Green);
}

////////////////////
// Falling Leaves //
////////////////////
void fallingLeavesInit(Particle *leaves, int NUM_LEAVES) {
  //initialize leaves
  for (int j = 0; j < NUM_LEAVES; j++) {
    int r = random(0, 100);
    //start leaves: placedat top, random state, not moving
    //leaves[j] = {64 + (j), r, false};
    //start leaves: randomly placed near bottom, black, moving
    r = random(0, 32);
    leaves[j] = {r, 99, true};
  }
}

void fallingLeaves() {
  int i = 0;
  //leaves
  int NUM_LEAVES = 16;
  Particle leaves[NUM_LEAVES];

  // maple leaf progression
  CRGB leafProgression[100];
  fill_gradient (leafProgression, 100, CHSV(96, 255, 196), CHSV(0, 255, 128), CHSV(32, 255, 64), TGradientDirectionCode::SHORTEST_HUES);
  leafProgression[99] = CRGB::Black;

  fallingLeavesInit(leaves, NUM_LEAVES);

  int NUM_FALLS = 3;
  int CNT_FALLS = 0;
  int leavesOnGround = 0;
  while (NUM_FALLS >= CNT_FALLS)
  {
    //start frame with black canvas
    fill_solid (leds, NUM_LEDS, black);

    //update all the leaves this frame
    for (int f = 0; f < NUM_LEAVES; f++) {

      //age a leaf, start falling if brown
      if ((leaves[f].color < 98)) {
        leaves[f].color++;
      } else {
        leaves[f].moving = true;
      }

      //fall a bit if needed, every other
      if (leaves[f].idx > 0 && leaves[f].moving  && (f % 2 == i % 2)) {
        leaves[f].idx--;
      }

      //reset leaf if at the bottom
      if (leaves[f].idx == 0) {
        //reset a leaf
        leavesOnGround++;
        if (NUM_FALLS > CNT_FALLS) {  //skip last reset
          leaves[f] = {64 + (f), 0, false};          
        }
      }

      //count complete falls
      if (leavesOnGround == NUM_LEAVES)
      {
        leavesOnGround = 0;
        //reinitilize leaves
        //fallingLeavesInit(leaves, NUM_LEAVES);
        CNT_FALLS++;
      }

      //add to display
      //skip over other leaves in the top of the tree
      //if it's not (odd, more than 64, and moving) unless it's black
      if ((!( (leaves[f].idx % 2 != 0) && (leaves[f].idx > 64) && leaves[f].moving ))  || (leds[leaves[f].idx] == CRGB(0, 0, 0))) {
        leds[leaves[f].idx] = leafProgression[leaves[f].color];
      }
    }//end for


    delay(90);
    FastLED.show();
    i++;
  }//end while
  fadeToBlack();
}


///////////
// daisy //
///////////
void daisy() {
  doneMillis = millis() + MODE_SHOW_MILLIS;
  while (doneMillis > millis())
  {
    //NUM_COLUMN_LEDS  // 64
    //NUM_STAR_LEDS    // 16
    for (int i = 0; i < NUM_LEDS; i++) {
      //iluminate the star
      if (i < NUM_COLUMN_LEDS) {
        leds[i] = green;
      } else if (i < (NUM_COLUMN_LEDS + 2)) {
        leds[i] = yellow;
      } else {
        leds[i] = white;
      }
      sparkle();
      FastLED.show();
      delay(32);
      fadeAFrame();
    }//end for
  }//end while
  sparkleToBlack();
}

///////////////
// halloween //
///////////////
//orange column, purple stars
void halloween() {
  doneMillis = millis() + MODE_SHOW_MILLIS;
  while (doneMillis > millis())
  {
    for (int i = 1; i < NUM_LEDS; i++)
    {
      //iluminate the star
      if (i < NUM_COLUMN_LEDS) {
        leds[i] = orange;
      } else {
        leds[i] = purples[i % 3];
      }
      sparkle();
      delay(36);
      FastLED.show();
      fadeAFrame();
    }//end for

    for (int i = 1; i < NUM_LEDS; i++)
    {
      //iluminate the star
      if (i < NUM_COLUMN_LEDS) {
        leds[i] = purples[i % 3];
      } else {
        leds[i] = orange;
      }
      sparkle();
      delay(36);
      FastLED.show();
      fadeAFrame();
    }//end for
  }//end while
  sparkleToBlack();
}//end halloween

///////////////
// starfield //
///////////////
void starfield() {
  doneMillis = millis() + MODE_SHOW_MILLIS;
  while (doneMillis > millis())
  {
    static int i = 0;
    //NUM_COLUMN_LEDS  // 64
    //NUM_STAR_LEDS    // 16

    //TODO
    //for (int j = 0; j < 8; j++){
    //}
    int star = max(min(NUM_COLUMN_LEDS + (i % NUM_STAR_LEDS), NUM_LEDS - 1), 0);
    int star2 = max(min(NUM_COLUMN_LEDS + ((i + (NUM_STAR_LEDS / 2)) % NUM_STAR_LEDS), NUM_LEDS - 1), 0);

    int column = max(min(NUM_COLUMN_LEDS - (i % NUM_COLUMN_LEDS), NUM_LEDS - 1), 0);
    int column2 = max(min(NUM_COLUMN_LEDS - ((i + (NUM_COLUMN_LEDS * 1 / 2)) % NUM_COLUMN_LEDS), NUM_LEDS - 1), 0);
    int column3 = max(min(NUM_COLUMN_LEDS - ((i + (NUM_COLUMN_LEDS * 3 / 4)) % NUM_COLUMN_LEDS), NUM_LEDS - 1), 0);
    int column4 = max(min(NUM_COLUMN_LEDS - ((i + (NUM_COLUMN_LEDS * 1 / 4)) % NUM_COLUMN_LEDS), NUM_LEDS - 1), 0);

    leds[star] = CHSV(0, 0, 255);
    leds[star2] = CHSV(0, 0, 255);

    leds[column] = CHSV(0, 0, 255);
    leds[column2] = CHSV(0, 0, 255);
    leds[column3] = CHSV(0, 0, 255);
    leds[column4] = CHSV(0, 0, 255);

    fadeAFrameFast();
    delay(50);
    FastLED.show();
    i++;
  }
  fadeToBlack();
}

///////////////
// Fireflies //
///////////////
void fireFlies()
{
  //init firefly locaitons
  for (int j = 0; j < 4; j++)
  {
    r[j] = random(minIdx, maxIdx);
  }

  doneMillis = millis() + (MODE_SHOW_MILLIS);
  while (doneMillis > millis())
  {
    static CRGB fireflyColor = chartreuse;
    static int location = 0;
    static int brightness = 0;
    static int i = 0;
    //TODO EVEN / ODD Fade different rates
    //TODO Double Pluse
    //TODO try Chartreuse
    //Particles sin wave, sub-sin wave?
    //Don't dim 100%
    //Lum ramp up also sin wave?
    //Wave function? Fourier series

    //update each firefly
    for (int j = 0; j < 4; j++)
    {
      //adjust color
      fireflyColor = chartreuse;
      brightness = wave(i, 0, 255, 1, j * 90);

      //TODO: if (brightness%10) add or subtract location? within range

      leds[r[j]] =  fireflyColor.nscale8(brightness);

      //jump to next location
      //TODO: 2 cycles?
      if (brightness < 1)
      {
        r[j] = random(minIdx, maxIdx);
      }
    }//end for

    delay(1);
    FastLED.show();
    i++;
  }// end while
  fadeToBlack();
}//end for

///////////////////
// yellowSparkle //
///////////////////
void yellowSparkle() {
  doneMillis = millis() + MODE_SHOW_MILLIS;
  while (doneMillis > millis())
  {
    for (int i = 2; i < NUM_LEDS; i++) {
      //iluminate the star w/ 1 pixel streamer
      leds[i - 2] = CRGB(255, 165, 0);
      leds[i - 1] = CRGB(255, 200, 128);
      leds[i] = white;
      sparkle();
      delay(18);
      FastLED.show();
      fadeAFrame();
    }
    sparkleToBlack();
  }
  fadeToBlack();
}

////////////////
//blue streaks//
////////////////
void blueStreaks() {
  doneMillis = millis() + MODE_SHOW_MILLIS;
  while (doneMillis > millis())
  {
    for (int i = 0; i < NUM_LEDS; i++)
    {
      //iluminate the star
      leds[i] = CRGB(0, 0, 255);
      delay(18);
      FastLED.show();
      fadeAFrame();
    }
    fadeToBlack();
  }
  fadeToBlack();
}

///////////////////
//rainbow crackle//
///////////////////
void rainbowCrackle() {
  doneMillis = millis() + MODE_SHOW_MILLIS;
  while (doneMillis > millis())
  {
    for (int i = 1; i < NUM_LEDS; i++) {
      //iluminate the star
      hue = hue + 16;
      leds[i] = CHSV(hue, 255, 255);
      crackle();
      delay(20);
      FastLED.show();
      fadeAFrame();
    }
    sparkleToBlack();
  }
  fadeToBlack();
}

/////////////////////
// Rainbow Chasers //
/////////////////////
void rainbowChasers() {
  doneMillis = millis() + MODE_SHOW_MILLIS;
  while (doneMillis > millis())
  {
    static int i = 0;
    fadeAFrameFast();
    for (int j = 0; j < 8; j++) {
      leds[(i + (10 * j)) % NUM_LEDS] = CHSV(32 * j, 255, 255);
    }
    //sparkle();
    delay(36);
    FastLED.show();
    i++;
  }
  fadeToBlack();
}

///////////////////
//white ping pong//
///////////////////
void pingPongWhite() {
  doneMillis = millis() + MODE_SHOW_MILLIS;
  while (doneMillis > millis())
  {
    static int i = 0;
    float deg = (i + 90);
    float rad = deg * PI / 180;
    int sinI = int(((NUM_LEDS) / 2) + (sin(rad) * ((NUM_LEDS) / 2)) );
    i++;
    sinI = max(min(sinI, NUM_LEDS - 1), 0);
    leds[sinI] = CRGB(255, 255, 255); //CHSV(i%255, 255 , 255);
    delay(5);
    FastLED.show();
    fadeAll(128);
  }
  fadeToBlack();
}

/////////////////////
//Rainbow ping pong//
/////////////////////
void pingPongRainbow() {
  doneMillis = millis() + MODE_SHOW_MILLIS;
  while (doneMillis > millis())
  {
    static int i = 0;
    float deg = (i + 270);
    float rad = deg * PI / 180;
    int sinI = int(((NUM_LEDS) / 2) + (sin(rad) * ((NUM_LEDS) / 2)) );
    i++;
    sinI = max(min(sinI, NUM_LEDS - 1), 0);
    leds[sinI] = CHSV(i % 255, 255 , 255);
    delay(5);
    FastLED.show();
    fadeAll(240);
  }
  fadeToBlack();
}

///////////////////////////
//Reverse Rainbow Chasers//
///////////////////////////
void reverseRainbowChasers() {
  doneMillis = millis() + 15000;
  while (doneMillis > millis())
  {
    static int i = 0;
    i++;
    fadeAFrameFast();
    for (int j = 0; j < 8; j++) {
      int location = max(min((NUM_LEDS - 1) - ((i + (10 * j)) % NUM_LEDS), NUM_LEDS - 1), 0);
      leds[location] = CHSV(32 * j, 255, 255);
    }
    //sparkle();
    delay(36);
    FastLED.show();
  }
  fadeToBlack();
}

/////////////////////
//Rotating Gradient//
/////////////////////

//0 is a color
//NUM_LEDS-1 is a color
//Other leds in between
//is each channel 0's color + ((i/NUM_LEDS)*(difference))
void rotatingGradient() {
  leds[0] = CRGB(255, 0, 0);
  leds[NUM_LEDS - 1] = CRGB(0, 0, 255);

  int r;
  int g;
  int b;

  int h;
  int s;
  int v;
  float ratio;

  int startH = 0;
  int endH = 255;

  for (int j = 0; j < 750; j++) {
    startH++; // =        min(startH + 1,255);

    float deg = (j * 2) % 360;
    float rad = deg * PI / 180;
    endH = startH + (sin(rad) * 64);

    CHSV startCHSV = CHSV((startH), 255, 255);
    CHSV endCHSV = CHSV((endH), 255, 255);

    leds[0] = startCHSV;
    leds[NUM_LEDS - 1] = endCHSV;

    for (int i = 1; i < (NUM_LEDS - 1); i++)
    {

      ratio = float(i) / float(NUM_LEDS);
      r = int( leds[0].r + ( ratio * (leds[NUM_LEDS - 1].r - leds[0].r) )   );
      g = int( leds[0].g + ( ratio * (leds[NUM_LEDS - 1].g - leds[0].g) )   );
      b = int( leds[0].b + ( ratio * (leds[NUM_LEDS - 1].b - leds[0].b) )   );

      //h = int( leds[0].h + ( ratio * (leds[NUM_LEDS-1].h-leds[0].h) )   );
      //s = int( leds[0].s + ( ratio * (leds[NUM_LEDS-1].s-leds[0].s) )   );
      //v = int( leds[0].v + ( ratio * (leds[NUM_LEDS-1].v-leds[0].v) )   );

      leds[i] = CRGB(r, g, b);
      //leds[i]= CHSV(h,s,v);

      //fill_gradient(leds,0,startCHSV,NUM_LEDS-1,endCHSV,FORWARD_HUES);
      fill_gradient(leds, 0, startCHSV, NUM_LEDS - 1, endCHSV, SHORTEST_HUES);
    }
    FastLED.show();
    //delay(60);
  }
  FastLED.show();
  fadeToBlack();
}

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
  fadeToBlack();
*/

/////////////////////////
// Long Rainbow Pulses //
/////////////////////////
void longRainbowPulses() {
  doneMillis = millis() + MODE_SHOW_MILLIS;
  while (doneMillis > millis())
  {
    fadeAFrameFast();
    for (int j = 0; j < 8; j++) {
      for (int ii = 0; ii < (NUM_LEDS); ii++)
      {
        leds[ii] = CHSV(32 * j, 255, 255);
        FastLED.show();  //show after each pixel
        fadeAll(250);
        delay(25);
      }
    }
  }
  fadeToBlack();
}

////////////////////////
// Red Yellow Chasers //
////////////////////////
void redYellowChasers() {
  doneMillis = millis() + MODE_SHOW_MILLIS;
  while (doneMillis > millis())
  {
    static int i = 0;
    fadeAFrameFast();
    for (int j = 0; j < 8; j++) {
      leds[(i + (11 * j)) % NUM_LEDS] = CHSV(50 * (j % 2), 255, 255);
    }
    //sparkle();
    delay(25);
    FastLED.show();
    i++;
  }
  fadeToBlack();
}

///////////////////
// Just crackles //
///////////////////
void justCrackles() {
  doneMillis = millis() + MODE_SHOW_MILLIS;
  while (doneMillis > millis())
  {
    fadeAFrame();
    //random sparkles
    r2 = random(minIdx, maxIdx);
    leds[r2] = white;
    delay(36);
    FastLED.show();
  }
  fadeToBlack();
}

/////////////////////////////
// Blue and Violet Chasers //
/////////////////////////////
void blueAndVioletChasers() {
  twoChasers (CRGB::Blue, CRGB::DarkViolet);
  return;
  doneMillis = millis() + MODE_SHOW_MILLIS;
  while (doneMillis > millis())
  {
    static int i = 0;
    leds[(i) % NUM_LEDS] = CRGB::Blue;
    leds[(i + 44) % NUM_LEDS] = CRGB::DarkViolet;
    delay(36);
    FastLED.show();
    fadeAll(200);
    i++;
  }
  fadeToBlack();
}



//TV Static
void tvStatic() {
  int red = 0;
  int green = 0;
  int blue = 0;

  doneMillis = millis() + MODE_SHOW_MILLIS;
  while (doneMillis > millis())
  {
    for (int i = 1; i < (NUM_LEDS); i = i + 2)
    {
      red = leds[i].r;
      green = leds[i].g;
      blue = leds[i].b;

      switch (random(0, 3)) {
        case 0:
          red = random(0, 128);
          break;
        case 1:
          green = random(0, 128);
          break;
        case 2:
          blue = random(0, 128);
          break;
      }

      switch (random(0, 3)) {
        case 0:
          red = 0;
          break;
        case 1:
          green = 0;
          break;
        case 2:
          blue = 0;
          break;
      }

      leds[i] = CRGB(red, green, blue); //CHSV(random(0,256), 255, random(0,256));

      //      red = leds[i].r;
      //      green = leds[i].g;
      //      blue = leds[i].b;

      //FastLED.show();
      //delay(10);
    }
    delay(100);
    FastLED.show();
  }
  fadeToBlack();
}


/////////////
//UTILITIES//
/////////////

////////////////////
// Three Crackles //
////////////////////
void threeCrackles(CRGB color1, CRGB color2, CRGB color3) {
  int i = 0;
  CRGB colors[] = {color1, color2, color3};
  doneMillis = millis() + MODE_SHOW_MILLIS;
  while (doneMillis > millis())
  {
    fadeAFrame();
    //random sparkles
    r2 = random(0, NUM_LEDS);
    leds[r2] = colors[i % 3];
    delay(36);
    FastLED.show();
    i++;
  }
  fadeToBlack();
}

//////////////////
// colorSparkle //
//////////////////
void colorSparkle(CHSV color) {
  for (int i = 1; i < NUM_LEDS; i++) {
    //iluminate the star w/ 1 pixel streamer
    if (i < NUM_COLUMN_LEDS + 1) {
      leds[i - 1] = CRGB(255, 128, 0);
      leds[i] = white;
    } else {
      //leds[i-1] = color;
      leds[i] = color;
    }
    sparkle();
    delay(18);
    FastLED.show();
    fadeAFrame();
  }
  sparkleToBlack();
}

///////////////////
// randomSparkle //
///////////////////
void randomSparkle() {
  doneMillis = millis() + MODE_SHOW_MILLIS;
  while (doneMillis > millis())
  {
    int random_hue = random(0, 265);
    Serial.print(random_hue);
    Serial.println();
    CHSV color = CHSV(random_hue, 255, 255);
    colorSparkle(color);
    //Serial.print(color.r);
    //Serial.print(color.g);
    //Serial.print(color.b);


  }
}

void twoChasers(CRGB color1, CRGB color2) {
  doneMillis = millis() + MODE_SHOW_MILLIS;
  while (doneMillis > millis())
  {
    static int i = 0;
    leds[(i) % NUM_LEDS] = color1;
    leds[(i + 44) % NUM_LEDS] = color2;
    delay(36);
    FastLED.show();
    fadeAll(200);
    i++;
  }
  fadeToBlack();
}





//////////
// WAVE //
//////////
//i(degrees 0-359, floor, max, amp <1 slows >1 speeds, offset
int wave(int i, int lo, int hi, float amp, int offset)
{
  float deg = ((i * amp) + 270 + offset);
  float rad = deg * PI / 180;     //convert to radians + 720 so i=0 => sin(rad)=0
  int dif = hi - lo;              //differnce to get range
  // sin(rad) will bounce between -1 and 1
  // multiply by diff/2 shoud bouce between -(diff/2) and (diff/2)
  // add (diff/2) should bounce between 0 and diff

  int sinI = int((dif / 2) + (sin(rad) * (dif / 2)) );
  // shift range back up
  sinI += lo;
  //safety rails
  sinI = min(sinI, hi);
  sinI = max(sinI, lo);
  return sinI;
}

//maximize brightness of random pixles that are not black
void sparkle()
{
  //random sparkles
  r2 = random(minIdx, maxIdx);
  if (leds[r2].getAverageLight() != 0)
  {
    leds[r2].maximizeBrightness();
  }
}

//random pixles that are not black max to white
void crackle()
{
  //random white crackles
  r2 = random(minIdx, maxIdx);
  if (leds[r2].getAverageLight() != 0)
  {
    leds[r2] = white;
  }
}

//fade everyone 1/2 way down
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

//fade everone to black
void sparkleToBlack()
{
  //fade everything to 0 before looping
  for (int i = 2; i < NUM_LEDS; i++) {
    //fade everyone
    for (int i = 0; i < NUM_LEDS; i++) {
      //leds[i] = leds[i].subtractFromRGB(16);
      leds[i] = leds[i].nscale8(225);

    }
    sparkle();
    delay(18);
    FastLED.show();
  }
  fadeToBlack();
}

//Color Chart https://github.com/FastLED/FastLED/wiki/Pixel-reference#chsv
// EOF
