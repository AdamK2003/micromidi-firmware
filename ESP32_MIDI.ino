#include <Arduino.h>
#include <BLEMidi.h>

//#include <Adafruit_GFX.h>
#include <SEMU_SSD1331.h>
#include <SPI.h>



#include <RotaryEncoder.h>






// OLED display pins
#define sclk 14
#define mosi 13
#define cs   18
#define rst  16
#define dc   17

// colors for OLED
#define BLACK           0x0000
#define BLUE            0x001F
#define RED             0xF800
#define GREEN           0x07E0
#define CYAN            0x07FF
#define MAGENTA         0xF81F
#define YELLOW          0xFFE0
#define WHITE           0xFFFF

int mainColor = WHITE;
int accentColor = YELLOW;


// row and column counts for the *electrical* matrix for the keys
#define ROWS 4
#define COLS 4

//rotary encoder pins
#define RE_DT 2
#define RE_CL 4
#define RE_BTN 32


bool connectedBT = false;


#include "graphics.h"
#include "menus.h"
#include "keymaps.h"
#include "modifiers.h"



struct Button {
  bool pressed;
  bool changed;
  String buttonName;
  const int matrixPos[2];
};

struct Matrix {
  const uint8_t pins[2][4];
  int matrix[ROWS][COLS];
};

Button a1 = {false, false, "a1", {0, 0}};
Button a2 = {false, false, "Cs4", {0, 1}};
Button a3 = {false, false, "Ds4", {0, 2}};
Button a4 = {false, false, "a4", {0, 3}};
Button a5 = {false, false, "Fs4", {1, 0}};
Button a6 = {false, false, "Gs4", {1, 1}};
Button a7 = {false, false, "As4", {1, 2}};
Button a8 = {false, false, "a8", {1, 3}};
Button b1 = {false, false, "C4", {2, 0}};
Button b2 = {false, false, "D4", {2, 1}};
Button b3 = {false, false, "E4", {2, 2}};
Button b4 = {false, false, "F4", {2, 3}};
Button b5 = {false, false, "G4", {3, 0}};
Button b6 = {false, false, "A4", {3, 1}};
Button b7 = {false, false, "B4", {3, 2}};
Button b8 = {false, false, "C5", {3, 3}};

Button Buttons[16] = {a1, a2, a3, a4, a5, a6, a7, a8, b1, b2, b3, b4, b5, b6, b7, b8};

Matrix matrix = {
  { { 19, 21, 22, 23 }, { 27, 26, 25, 33 } }, // matrix pins, voltage will be applied to 1st array, output will be read from 2nd array
  { { 0, 1, 2, 3 }, { 4, 5, 6, 7 }, { 8, 9, 10, 11 }, { 12, 13, 14, 15 } } // button rows/cols
};

/*
   Matrix looks like this:

  [0,0] [0,1] [0,2] [0,3] [1,0] [1,1] [1,2] [1,3]
    [2,0] [2,1] [2,2] [2,3] [3,0] [3,1] [3,2] [3,3]
*/

















void connectedCallback() {
  Serial.println("Connected!");
  bluetoothIcon(0, 0, BLUE);
  connectedBT = true;
}

void disconnectedCallback() {
  Serial.println("Disconnected");
  bluetoothIcon(0, 0, RED);
  connectedBT = false;
}




void matrixInit() {
  for (int i = 0; i < ROWS; i++) {
    pinMode(matrix.pins[0][i], OUTPUT);
  }

  for (int i = 0; i < COLS; i++) {
    pinMode(matrix.pins[1][i], INPUT_PULLDOWN);
  }
  pinMode(matrix.pins[1][0], INPUT_PULLDOWN);
}


RotaryEncoder encoder = RotaryEncoder(RE_CL, RE_DT, RotaryEncoder::LatchMode::FOUR3);

bool encoderChange = false;
static int encoderPos = 0;

IRAM_ATTR void checkPosition()
{
  encoder.tick(); // just call tick() to check the state.
  encoderChange = true;
}


bool encoderButton = false;
unsigned long lastEncoderPress = 0;

IRAM_ATTR void checkEncoderButton() {
  if(millis() - lastEncoderPress >= 200) {
    encoderButton = true;
    lastEncoderPress = millis();
  }
}


//#include "menus.h"

void setup() {
  Serial.begin(115200);
  display.begin();
  


  display.clearWindow();

  matrixInit();




  attachInterrupt(digitalPinToInterrupt(RE_DT), checkPosition, CHANGE);
  attachInterrupt(digitalPinToInterrupt(RE_CL), checkPosition, CHANGE);

  pinMode(RE_BTN, INPUT_PULLUP);

  attachInterrupt(RE_BTN, checkEncoderButton, FALLING);



  

  Serial.println("Initializing bluetooth");
  
  BLEMidiServer.begin("ESP32 MIDI");
  BLEMidiServer.setOnConnectCallback(connectedCallback);
  BLEMidiServer.setOnDisconnectCallback(disconnectedCallback);
  
  Serial.println("Waiting for connections...");



  

  bluetoothIcon(0, 0, WHITE);

  initOctaves(35, 62, mainColor);
  updateOctaves(0, 0, mainColor, accentColor);

  initSemis(47, 59, mainColor);
  updateSemis(0, 0, mainColor, accentColor);

  
  
  UIbase.select();
  
  changeKeymap(0);


  keymapSelector.enable();
  keymapSelector.sel();
  


  BLEMidiServer.setOnConnectCallback(connectedCallback);
  BLEMidiServer.setOnDisconnectCallback(disconnectedCallback);
  //BLEMidiServer.enableDebugging();
}





bool matrixChange = false;
unsigned long lastScan;

bool matrixScan() {
  lastScan = millis();
  bool anyChanges = false;
  for (int i = 0; i < ROWS; i++) {

    digitalWrite(matrix.pins[0][i], HIGH);
    for (int j = 0; j < COLS; j++) {
      bool currState = digitalRead(matrix.pins[1][j]);

      //if(j = 0) Serial.print(state);

      bool prevState = Buttons[matrix.matrix[i][j]].pressed;
      if (currState != prevState) {

        Buttons[matrix.matrix[i][j]].pressed = currState;
        Buttons[matrix.matrix[i][j]].changed = true;
        anyChanges = true;
      }
    }

    digitalWrite(matrix.pins[0][i], LOW);
    //Serial.println("");

  }

  return anyChanges;
}







const int baseNote = 60; // C4
int octaveOffset = 0;
int semiOffset = 0;
int channel = 0;


int pos = 0;

int (*changeCallbacks[OPTION_COUNT])(int) = {&changeKeymap, &changeScale};


void loop() {
  if (millis() >= lastScan + 3) matrixChange = matrixScan();


  if(encoderButton) {
    mainMenu.toggleSelection();
    encoderButton = false;
  }

  if(encoderChange) {

    int newPos = encoder.getPosition();

    if (pos != newPos) {
      if(mainMenu.dir(newPos - pos)) {
        changeCallbacks[mainMenu.pos](newPos - pos);
      }
      pos = newPos;
    }

    encoderChange = false;
    
    
  }
  

  if (connectedBT) {
  
    

    if (matrixChange) {
      Keymap keymap = keymaps[currentKeymap];

      switch (keymap.typeNum) {
        
        case 0: // base
        // base keymap
          for (int i = 0; i < 16; i++) {
    
            if (Buttons[i].changed) {
              if (Buttons[i].pressed) {
    
                if (keymap.keys[i].action == "oct") {
    
                  if (!Buttons[3].pressed  && -4 <= (octaveOffset + keymap.keys[i].value) && (octaveOffset + keymap.keys[i].value) <= 4) { // octaves
    
                    updateOctaves(octaveOffset, octaveOffset + keymap.keys[i].value, mainColor, accentColor);
    
                    for (int j = 0; j < 16; j++) {
                      if (Buttons[j].pressed && keymap.keys[j].action == "note") BLEMidiServer.noteOff(channel, baseNote + octaveOffset * 12 + semiOffset + keymap.keys[j].value, 127);
                    }
    
                    octaveOffset += keymap.keys[i].value;
    
                  } else if (Buttons[3].pressed && -11 <= (semiOffset + keymap.keys[i].value) && (semiOffset + keymap.keys[i].value) <= 11) { // modifier pressed, semis
    
    
                    for (int j = 0; j < 16; j++) {
                      if (Buttons[j].pressed && keymap.keys[j].action == "note") BLEMidiServer.noteOff(channel, baseNote + octaveOffset * 12 + semiOffset + keymap.keys[j].value, 127);
                    }
    
                    updateSemis(semiOffset, semiOffset + keymap.keys[i].value, mainColor, accentColor);
     
                    semiOffset += keymap.keys[i].value;
                  }
    
                } else if (keymap.keys[i].action == "note") {
                  BLEMidiServer.noteOn(channel, baseNote + octaveOffset * 12 + semiOffset + (keymap.keys[i].value), 127);
                }
    
              } else if (!Buttons[i].pressed) {
                if (keymap.keys[i].action == "note") {
                  BLEMidiServer.noteOff(channel, baseNote + octaveOffset * 12 + semiOffset + (keymap.keys[i].value), 127);
                }
              }
              Buttons[i].changed = false;
            }
    
          }
          break;
          // /base keymap

        case 1: // 2oct

          for (int i = 0; i < 16; i++) {
    
            if (Buttons[i].changed) {
              if (Buttons[i].pressed) {
    
                if (keymap.keys[i].action == "note") {
                  BLEMidiServer.noteOn(channel, baseNote + octaveOffset * 12 + semiOffset + scales[currentScale].offsets[i%8] + (keymap.keys[i].value), 127);
                }
    
              } else if (!Buttons[i].pressed) {
                
                if (keymap.keys[i].action == "note") {
                  BLEMidiServer.noteOff(channel, baseNote + octaveOffset * 12 + semiOffset + scales[currentScale].offsets[i%8] + (keymap.keys[i].value), 127);
                }
                
              }
              Buttons[i].changed = false;
            }
    
          }
          break;
          // /2oct
          

      } // end of switch-case statement
      
      matrixChange = false;
    }

  }


}
