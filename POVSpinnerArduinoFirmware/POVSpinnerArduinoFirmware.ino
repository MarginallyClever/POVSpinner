// Persistence Of Vision spinning LED tutorial
// dan@marginallyclever.com (2019-01-01)
// 

// supported boards
#define RUMBA 1
#define RAMPS 2

// your chosen board
//#define BOARD_TYPE RUMBA
//#define BOARD_TYPE RAMPS

// sanity check
#if !defined(BOARD_TYPE)
#error You must choose a board type!
#endif

// pin assignments
#if BOARD_TYPE == RAMPS
#define MOTOR_0_STEP_PIN              (54)
#define MOTOR_0_DIR_PIN               (55)
#define MOTOR_0_ENABLE_PIN            (38)
#define DATA_PIN 60  // Y_STEP_PIN
#endif

#if BOARD_TYPE == RUMBA
#define MOTOR_0_STEP_PIN              (17)
#define MOTOR_0_DIR_PIN               (16)
#define MOTOR_0_ENABLE_PIN            (48)
#define DATA_PIN 54  // Y_STEP_PIN
#endif


#define NUM_LEDS             14

#define STEPS_PER_TURN      200  // 1.8 degree stepper motor

// delay between steps when the machine starts moving.
#define START_DELAY        1200  // us
// delay between steps at top speed.
// the limiting factor is strip.show(), not the motor.
#define MIN_DELAY           500  // us

#define SPACING              16.5  // mm.  distance between lights

#define NUM_LIGHTS            7  // on each side
#define DEGREES_PER_TURN    360

#define GRID_SIZE             7  // bitmap grid is 7x7
#define GRID_SPACING (SPACING*2) // mm.  grid size.


#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, DATA_PIN, NEO_GRB + NEO_KHZ800);


// colors to display on the letters in the message.
#define COLOR_LIST_LENGTH   7
uint32_t color_list[] = {
  strip.Color(  0,  0,255),
  strip.Color(  0,255,  0),
  strip.Color(255,  0,  0),
  strip.Color(  0,255,255),
  strip.Color(255,255,  0),
  strip.Color(255,  0,255),
  strip.Color(255,255,255),
};

  
///////////// LETTER BITMAPS ///////////////////
char grid_h[] = {  // GRID_SIZE*GRID_SIZE
0,0,0,0,0,0,0,
0,1,0,0,0,1,0,
0,1,0,0,0,1,0,
0,1,1,1,1,1,0,
0,1,0,0,0,1,0,
0,1,0,0,0,1,0,
0,0,0,0,0,0,0,
};

char grid_a[] = {  // GRID_SIZE*GRID_SIZE
0,0,0,0,0,0,0,
0,0,1,1,1,0,0,
0,1,0,0,0,1,0,
0,1,1,1,1,1,0,
0,1,0,0,0,1,0,
0,1,0,0,0,1,0,
0,0,0,0,0,0,0,
};

char grid_p[] = {  // GRID_SIZE*GRID_SIZE
0,0,0,0,0,0,0,
0,1,1,1,1,0,0,
0,1,0,0,0,1,0,
0,1,1,1,1,0,0,
0,1,0,0,0,0,0,
0,1,0,0,0,0,0,
0,0,0,0,0,0,0,
};

char grid_y[] = {  // GRID_SIZE*GRID_SIZE
0,0,0,0,0,0,0,
0,1,0,0,0,1,0,
0,1,0,0,0,1,0,
0,0,1,1,1,0,0,
0,0,0,1,0,0,0,
0,0,0,1,0,0,0,
0,0,0,0,0,0,0,
};

char grid__[] = {  // GRID_SIZE*GRID_SIZE
0,0,0,0,0,0,0,
0,0,0,0,0,0,0,
0,0,0,0,0,0,0,
0,0,0,0,0,0,0,
0,0,0,0,0,0,0,
0,0,0,0,0,0,0,
0,0,0,0,0,0,0,
};

char grid_n[] = {  // GRID_SIZE*GRID_SIZE
0,0,0,0,0,0,0,
0,1,0,0,0,1,0,
0,1,1,0,0,1,0,
0,1,0,1,0,1,0,
0,1,0,0,1,1,0,
0,1,0,0,0,1,0,
0,0,0,0,0,0,0,
};

char grid_e[] = {  // GRID_SIZE*GRID_SIZE
0,0,0,0,0,0,0,
0,1,1,1,1,1,0,
0,1,0,0,0,0,0,
0,1,1,1,0,0,0,
0,1,0,0,0,0,0,
0,1,1,1,1,1,0,
0,0,0,0,0,0,0,
};

char grid_w[] = {  // GRID_SIZE*GRID_SIZE
0,0,0,0,0,0,0,
0,1,0,0,0,1,0,
0,1,0,0,0,1,0,
0,1,0,1,0,1,0,
0,1,0,1,0,1,0,
0,0,1,0,1,0,0,
0,0,0,0,0,0,0,
};

char grid_r[] = {  // GRID_SIZE*GRID_SIZE
0,0,0,0,0,0,0,
0,1,1,1,1,0,0,
0,1,0,0,0,1,0,
0,1,1,1,1,0,0,
0,1,0,0,1,0,0,
0,1,0,0,0,1,0,
0,0,0,0,0,0,0,
};

char grid_2[] = {  // GRID_SIZE*GRID_SIZE
0,0,0,0,0,0,0,
0,0,1,1,1,0,0,
0,0,0,0,1,0,0,
0,0,1,1,1,0,0,
0,0,1,0,0,0,0,
0,0,1,1,1,0,0,
0,0,0,0,0,0,0,
};

char grid_0[] = {  // GRID_SIZE*GRID_SIZE
0,0,0,0,0,0,0,
0,0,1,1,1,0,0,
0,0,1,0,1,0,0,
0,0,1,0,1,0,0,
0,0,1,0,1,0,0,
0,0,1,1,1,0,0,
0,0,0,0,0,0,0,
};

char grid_1[] = {  // GRID_SIZE*GRID_SIZE
0,0,0,0,0,0,0,
0,0,0,1,0,0,0,
0,0,0,1,0,0,0,
0,0,0,1,0,0,0,
0,0,0,1,0,0,0,
0,0,0,1,0,0,0,
0,0,0,0,0,0,0,
};

char grid_9[] = {  // GRID_SIZE*GRID_SIZE
0,0,0,0,0,0,0,
0,0,1,1,1,0,0,
0,0,1,0,1,0,0,
0,0,1,1,1,0,0,
0,0,0,0,1,0,0,
0,0,1,1,1,0,0,
0,0,0,0,0,0,0,
};


#define GRID_LIST_LENGTH 20  // length of the message
// A list of pointers to each bitmap.
char *grid_list[] {
  grid_h,
  grid_a,
  grid_p,
  grid_p,
  grid_y,
  grid__,
  grid_n,
  grid_e,
  grid_w,
  grid__,
  grid_y,
  grid_e,
  grid_a,
  grid_r,
  grid__,
  grid_2,
  grid_0,
  grid_1,
  grid_9,
  grid__,
};

// which letter is being displayed now?
int grid_list_index;
// a pointer to the current bitmap.
char *grid;


// how long to delay between each step of the motor.
// this is used to accelerate at the start and achieve a higher top speed.
int newWait=START_DELAY;


// the bitmap floats invisibly in space.  This tool remembers where each bitmap
// pixel is floating
struct GridCell {
  float x,y;
  int index;
};
GridCell cells[GRID_SIZE*GRID_SIZE];

// a map between the angle-distance of a light and the x/y position of each bitmap pixel.
int angleLightCell[DEGREES_PER_TURN * NUM_LIGHTS];



void setup() {
  pinMode(MOTOR_0_STEP_PIN  ,OUTPUT);
  pinMode(MOTOR_0_DIR_PIN   ,OUTPUT);
  pinMode(MOTOR_0_ENABLE_PIN,OUTPUT);

  digitalWrite(MOTOR_0_DIR_PIN,HIGH);
  
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'

  grid_list_index=0;
  grid = grid_list[0];

  Serial.begin(57600);
  Serial.println("Calculating grid cells...");

  for(int y=0;y<GRID_SIZE;++y) {
    for(int x=0;x<GRID_SIZE;++x) {
      int index=y*GRID_SIZE+x;
      cells[index].index=index;
      
      float x0=((float)x-GRID_SIZE/2)*GRID_SPACING;
      float y0=((float)y-GRID_SIZE/2)*GRID_SPACING;
      cells[index].x=x0;
      cells[index].y=y0;
    }
  }
  
  Serial.println("Calculating angle/light/cell relationships...");
  for(int a=0;a<DEGREES_PER_TURN;++a) {
    float c = cos(radians(a));
    float s = sin(radians(a));
    for(int i=0;i<NUM_LIGHTS;++i) {
      // find the x,y of the current light
      float x = (float)NUM_LIGHTS/2.0 + -s*((float)i*SPACING+SPACING/2);
      float y = (float)NUM_LIGHTS/2.0 + c*((float)i*SPACING+SPACING/2);
      // match to the nearest grid square
      int nearest=-1;
      float nearestDistance=SPACING*SPACING*NUM_LIGHTS;
      
      for(int j=0;j<GRID_SIZE*GRID_SIZE;++j) {
        float dx = cells[j].x-x;
        float dy = cells[j].y-y;
        float distance = dx*dx+dy*dy; 
        if( nearestDistance > distance ) {
          nearestDistance = distance;
          nearest=j;
        }
      }
      angleLightCell[a*NUM_LIGHTS+i]=nearest;
    }  
  }
  Serial.println("------- READY -------");
}


void loop() {
  uint16_t i, j=0, dir=1,s=0,t2=0;
  int angle, cellID;
  int color_list_index=0;

  uint32_t c3=color_list[color_list_index];
  const float aInc = 360.0/(float)STEPS_PER_TURN;
  float aSum=0;
  int *offset;
  int *offset2;
  unsigned long tEnd;
  
  long nextLetterTime=millis()+500;

  // adjust brightness to taste.
  strip.setBrightness(64);
  
  do {
    tEnd = micros() + newWait;
    digitalWrite(MOTOR_0_STEP_PIN,LOW);

    // count rotation steps
    s++;
    aSum+=aInc;
    if(s>=STEPS_PER_TURN) {
      s=0;
      aSum=0;
    }
    angle = aSum;

    // use the lookup table to find which grid cell is closest to each light.
    // then use the grid cell to find the color of the current letter bitmap.
    offset  = angleLightCell + ((angle    )    )*NUM_LIGHTS;
    offset2 = angleLightCell + ((angle+180)%360)*NUM_LIGHTS;
    for(i=0; i<NUM_LIGHTS; i++) {
      strip.setPixelColor(NUM_LIGHTS+i  , grid[offset [i]]*c3);  // light one half of the strip
      strip.setPixelColor(NUM_LIGHTS-1-i, grid[offset2[i]]*c3);  // light the other half
    }
    strip.show();

    // wait a bit
    while(micros()<tEnd);
    
    // step the motor
    digitalWrite(MOTOR_0_STEP_PIN,HIGH);

    // accelerate
    if(newWait>MIN_DELAY) {
      if(t2++>2) {  // >2 controls the acceleration. >10 would be slower.
        newWait--;
        t2=0;
      }
    }

    // change to the next letter and next color?
    if(millis()>nextLetterTime) {
      nextLetterTime=millis()+400;

      grid_list_index = (grid_list_index+1)%GRID_LIST_LENGTH;
      grid = grid_list[grid_list_index];

      color_list_index = (color_list_index+1)%COLOR_LIST_LENGTH;
      c3=color_list[color_list_index];
    }
  } while(1);
}
