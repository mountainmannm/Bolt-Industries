// Written Feb 2022 by Brian DiDonna. 

//This library is free software; you can redistribute it and/or
//modify it under the terms of the GNU Lesser General Public
//License as published by the Free Software Foundation version 3.0.
//This firmware is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
//Lesser General Public License for more details.
//
//You should have received a copy of the GNU Lesser General Public
//License along with this library; if not, write to the Free Software
//Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA

// This sketch is for the BMK! Bolt (Industries) Mechanical keyboard.
// The assignment of keyboard columns and rows to pins is defined below.
// The wiring has diodes pointed from column to row.
// We set the columns to INPUT_HIGH to pull them to 5V. When a row is
// also HIGH and the key for that row and column is pressed then there
// is no voltage difference and nothing happens. If the row is set LOW and
// the key is pressed then is pulls the column to LOW and we can read it out
// for the column. We look for keypresses by setting one row to LOW and
// all others to HIGH and looking for a column that is LOW. We scan the
// LOW row to read the whole keyboard.

// If flashed exactly as it is to your Pico, this firmware will function as a regular
// keyboard with the Function keys modified as shortcuts. 


// to purchase a keyboard kit, visit www.boltind.com

// ___ _   _ ____ _____  _    _     _        _  _____ ___ ___  _   _ 
//|_ _| \ | / ___|_   _|/ \  | |   | |      / \|_   _|_ _/ _ \| \ | |
// | ||  \| \___ \ | | / _ \ | |   | |     / _ \ | |  | | | | |  \| |
// | || |\  |___) || |/ ___ \| |___| |___ / ___ \| |  | | |_| | |\  |
//|___|_| \_|____/ |_/_/   \_\_____|_____/_/   \_\_| |___\___/|_| \_|
                                                                 

//TO  INSTALL  THIS FIRMWARE
//This firmware must be installed through the Arduino IDE.
//You must first install a third party board. The RP  2040 support in the Arduino IDE does
//not  include  support for the HID.h library that this sketch reqires. Fortunantly, there's a work around.
//You will need to install a third party board written by Earle F. Philhower, III from Github.
//You can read more about this third party board at: https://github.com/earlephilhower/arduino-pico

//In the Arduino IDE: go to File > preferances, and paste the following URL  under the additional boards manager URL's:
// https://github.com/earlephilhower/arduino-pico/releases/download/global/package_rp2040_index.json

//Now, navigate over to the boards manager. Go to: Tools > Board > Board Manager. Search for "Pico"
//You will see "Raspberry Pi Pico/RP2040." Install this, it's the thrid party board you just added.
//Now, you should be able to select your newly installed Raspberry Pi Pico driver and upload your sketch!

#include <Keyboard.h>
#include <limits.h>

// Uncomment this to switch the modifier keys used for macros and other OS dependent behavior
// When MACOSX is defined we send Command-c to copy instead of Control-c, etc.
// WIN32 works with Windows, Chrome, Android and Linux functions. 
// This is not true for the Unicode helper function starting on line 277. To change the Unicode helper from Linux/Android/Chrome to Windows, go to line 277.

//#define MACOSX
#define WIN32

// assign the collums to pins.
#define Col_0 0
#define Col_1 1
#define Col_2 2
#define Col_3 3
#define Col_4 4
#define Col_5 5
#define Col_6 6
#define Col_7 7
#define Col_8 8
#define Col_9 9
#define Col_10 10
#define Col_11 11
#define Col_12 12
#define Col_13 13
#define Col_14 14
#define Col_15 15
#define Col_16 16
#define Col_17 17

// assign the rows to pins.
#define Row_0 18
#define Row_2 19
#define Row_3 20
#define Row_4 21
#define Row_5 22
#define Row_6 26

// For the caps lock
#define LED_1 27
#define LED_2 28

// The pin numbers of rows 2 through 6 do not increase linearly. This vector
// lets us loop over those indice efficiently
const int rowIndices[ 5 ] = { Row_2, Row_3, Row_4, Row_5, Row_6 };

// These are the times before a key is repeated. There is an initial long time then after that a shorter time so they repeat faster
const int repeatDelayInitial = 300;
const int repeatDelayRepeat = 30;

//Adding a slight delay before sendding the key press helps with the modifier keys or key sequences. Without it, The
//keypresses tend to be missed.
const int preDelay = 10;

//The key needs to be pressed for at least this long to register. This could be set to zero but that might allow unintended keypresses
const int minimumKeypressDelay = 5;

// This is the mapping from row, column to key for rows 2 through 6. If you want to override a key, just enter the new key here.
// If you want to write your own special handling, enter a 0x00 here to skip the key loop then write your special handling elsewhere.
//                                     0     1     2     3     4     5     6     7     8     9    10    11    12    13    14    15    16    17

const char sKeysForRows[5][18] = { { '`',  '1',  '2',  '3',  '4',  '5',  '6',  '7',  '8',  '9',  '0',  '-',  '=', 0x00, 0xB2, 0xD1, 0xD2, 0xD3 },
                                  { 0xB3, 0x00,  'q',  'w',  'e',  'r',  't',  'y',  'u',  'i',  'o',  'p',  '[',  ']', '\\', 0xD4, 0xD5, 0xD6 },
                                  { 0x00, 0x00,  'a',  's',  'd',  'f',  'g',  'h',  'j',  'k',  'l',  ';', '\'', 0xB0, 0x00, 0x00, 0x00, 0x00 },
                                  { 0x00, 0x00,  'z',  'x',  'c',  'v',  'b',  'n',  'm',  ',',  '.',  '/', 0x00, 0x00, 0x00, 0x00, 0xDA, 0x00 },
                                  { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  ' ', 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xD8, 0xD9, 0xD7 }
                                };

// Not the most elegant solution, but just track whether a key is currently being held down using an array of bools.
bool sKeyPressed[5][18];

// This is the mapping from column to key for the top function keys (Rw_0). If you want to override a key, just enter the new key here.
// If you want to write your own special handling, enter a 0x00 here to skip the key loop then write your special handling elsewhere.
// If you want regular Function keys instead of shortcuts, you can comment out line 126 and un comment line 127.
//                                   0     1     2     3     4     5     6     7     8     9    10    11    12    13    14    15    16    17
const char sKeysForRowsFn[18] = { 0xB1, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
//const char sKeysForRowsFn[18] = { 0xB1, 0x00, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7, 0xC8, 0xC9, 0xCA, 0xCB, 0xCC, 0xCD,0x00, 0x00, 0x00, 0x00 };

// Here we track whether the function key was down or up on the last loop. We use this
// to prevent key repeat of function keys
bool sFnPressed[18];

// Here we keep track of the last time the various keys were pressed. This tracks rows 2 through 6. This is
// used to control the timing of key repeats
unsigned long sLastPress[5][18];
unsigned long sLastClear[5][18];

// Global flags for whether modifier keys are active
static bool sBoolShift = false;
static bool sBoolCtrl = false;
static bool sBoolAlt = false;
static bool sBoolGui = false;
static bool sBoolCapsLock = false;

// wrappers for keyboard actions that enforce a slight delay so Keyboard does not miss next action

void keyboardPress( char key )
{
  Keyboard.press( key );
  delay( preDelay );
}

void keyboardRelease( char key )
{
  Keyboard.release( key );
  delay( preDelay );
}

void keyboardWrite( char key )
{
  Keyboard.write( key );
  delay( preDelay );
}

void keyboardPrint( const char *string )
{
  Keyboard.print( string );
  delay( preDelay );
}

// wrapper to check column
bool columnPressed( int column ) 
{
  return digitalRead( column ) == LOW;
}

// RAII class to conditionally toggle modifier when class object goes in and out of scope.
// If the modifier is already pressed then we don't do anything.
// This class is templated on the variables we pass in to allow us to declare types associated with the keys -
// see the section below that defines these types with the "using" keyword
// Usually the object is deleted when it goes out of scope (e.g., you have a closing '}' or you finish the current
// iterationm of the for loop contianing this class instantiation.
template< bool* modifierBool, char key >
class ScopedModifier {
  public:
    ScopedModifier()
    {
      if ( !*modifierBool ) {
        keyboardPress(key); // press the Modifier key
      }
    }

    ~ScopedModifier()
    {
      if ( !*modifierBool ) {
        keyboardRelease(key); //release the Modifier key
      }
    }
};

// define the usual modifier classes
using ScopedControl = ScopedModifier< &sBoolCtrl, KEY_LEFT_CTRL >;
using ScopedAlt = ScopedModifier< &sBoolAlt, KEY_LEFT_ALT >;
using ScopedGui = ScopedModifier< &sBoolGui, KEY_LEFT_GUI >;
using ScopedShift = ScopedModifier< &sBoolShift, KEY_LEFT_SHIFT >;

// OS dependent modifier key - define MACOSX to toggle (on line42)
#ifdef MACOSX
using ScopedOsModifier = ScopedGui;
#else
using ScopedOsModifier = ScopedControl;
#endif

// Get the time delta in milliseconds between first and second, accounting for a possible
// integer overflow of the millis() function (this happens every 50 days or so)
// NOTE this will ALWAYS return a positive number
unsigned long timeDelta( unsigned long first, unsigned long second )
{
  if ( second >=  first ) {
    return second - first;
  } else {
    return ( ULONG_MAX - first ) + second + 1;
  }
}

// Helper funtion for keys that do not repeat
// Only accept these keys if the last time through they were not down.
// Also check that they are held down for a minimum period of time.
// Need to supply a pressState bool for tracking state
bool nonRepeatingKeyPress( int column, bool& pressState ) {
  if ( digitalRead( column ) == LOW ) {
    if ( !pressState ) {
      delay (minimumKeypressDelay);
      if ( columnPressed( column ) ) {
        pressState = true;
        return true;
      }
    }
  } else {
    pressState = false;
  }
  return false;
};

// Helper function to check whether a key has been pressed for minimumKeypressDelay
bool checkMinimumKeyPress( int column ) {
  if (columnPressed(column) ) {
    delay (minimumKeypressDelay);
    if ( columnPressed( column ) ) {
      return true;
    }
  }
  return false;
}

// Check if modifier keys are down and change state accordingly. Usually two keys
// send the same modifier, so check them both.
// Only toggle once. No key press delay
// User must supply state bool to track modifier state
void modifierFunc( int column1, int column2, bool& state, char key ) {
  auto columnState1 = digitalRead(column1);
  auto columnState2 = digitalRead(column2);
  if ( !state && ( columnState1 == LOW || columnState2 == LOW ) ) {
    keyboardPress(key);
    state = true;
  }
  if ( state && columnState1 == HIGH && columnState2 == HIGH ) {
    keyboardRelease(key);
    state = false;
  }
};


// Helper function to write a unicode key code (Linux, Chrome, Android OS only.) This is not application spacific and works just about anywhere 
void writeUnicode( const char* unicodeString )
{
  {
    ScopedControl sc;
    ScopedShift ss;
    keyboardWrite( 'u' );
  }
  keyboardPrint ( unicodeString );
  keyboardWrite (0xB0); //return
}


//void writeUnicode( const char* unicodeString ) // Windows only. This works in most applications, but not all. 
//{
//    keyboardPrint ( unicodeString );
//    ScopedAlt sa;
//    keyboardWrite ('x');
//}


// This function wraps the simple keyboardPress( key ) function and properly handles
// the caps lock key. The Keyboard library does not handle caps lock correctly, so we wrote our
// own. The caps lock key will only affect letters. Also, if the shift key is held down while
// caps lock is active, it will still type a lower case letter.
// We implement the caps lock by actually toggling the shift key, because sending an upper case
// character in the Keyboard library also will toggle shift. So we only ever
// send lower case characters to press or release.
void pressKeyboardRespectCapLock( char key )
{
  if ( !sBoolCapsLock || !isalpha( key ) ) {
    keyboardPress( key );
  } else {
    if ( sBoolShift ) {
      keyboardRelease(KEY_LEFT_SHIFT);
      keyboardPress( key );
      keyboardPress(KEY_LEFT_SHIFT);
    } else {
      keyboardPress(KEY_LEFT_SHIFT);
      keyboardPress( key );
      keyboardRelease(KEY_LEFT_SHIFT);
    }
  }
}

// RAII class to make a row active when the class object is created, then make the row inactive when the object is deleted.
// Usually the object is deleted when it goes out of scope (e.g., you have a closing '}' or you finish the current
// iterationm of the for loop contianing this class instantiation.
class ScopedRowActive
{
  public:
    ScopedRowActive( int rowIndex) : mRowIndex( rowIndex )
    {
      digitalWrite( mRowIndex, LOW);
    }

    ~ScopedRowActive()
    {
      digitalWrite( mRowIndex, HIGH);
    }

  private:
    int mRowIndex;
};


void setup() {
  unsigned long currentTimeMs = millis();

  for ( int j = 0; j < 18; ++j ) {
    sFnPressed[ j ] = false;
  }

  for ( int i = 0; i < 5; ++i ) {
    for ( int j = 0; j < 18; ++j ) {
      sLastPress[ i ][ j ] = currentTimeMs;
      sLastClear[ i ][ j ] = currentTimeMs;
      sKeyPressed[ i ][ j ] = false;
    }
  }

  // Set the column pins as inputs and pull them high
  for ( int j = 0; j < 18; ++j ) {
    pinMode(Col_0 + j, INPUT_PULLUP);
  }
  // Set the row pins as output and set them high
  pinMode (Row_0, OUTPUT);
  for ( int i = 0; i < 5; ++i ) {
    pinMode (rowIndices[ i ], OUTPUT);
    digitalWrite( rowIndices[ i ], HIGH);
  }

  // Set the LED pins as output and set them low
  pinMode (LED_1, OUTPUT);
  digitalWrite( LED_1, LOW);
  pinMode (LED_2, OUTPUT);
  digitalWrite( LED_2, LOW);

  // begin keyboard input
  Keyboard.begin();
}

void loop() {

  //timing loop goes through 10000 loops to get good averaging. Prints result to screen.
//  static unsigned long timingCount = 0;
//  static unsigned long timingTime = millis();
//  if ( timingCount % 10000 == 0  && timingCount > 0) {
//     unsigned long delta = timeDelta( timingTime, millis() );    
//     double rate = 1000.0 * 10000.0 / delta; //extra factor of 1000 is because delta is in milliseconds
//     char rateSting[ 100 ];
//     sprintf( rateSting, "%lf\n", rate );
//     keyboardPrint( rateSting );
//     timingTime = millis();
//  }
//  ++timingCount;



  // main loop over central part of keyboard (starting at numbers row and working down
  bool anyKeyDown = false;
  static bool clearAllLoaded = false;
  unsigned long currentTimeMs = millis();
  static unsigned long lastPressTime = 0;
  for ( int i = 0; i < 5; ++i ) {
    // activate row rowIndices[ i ]
    ScopedRowActive rowLow( rowIndices[ i ] );
    // Do a few special cases for certain rows (e.g., modifier keys)
    if ( i == 2 ) {
      // CAPS LOCK
      // The behavior of the keyboard library capslock is weird
      // we are just going to write our own
      static bool capsLockPressed = false;      
      if ( nonRepeatingKeyPress( Col_0, capsLockPressed ) ) {
        sBoolCapsLock = !sBoolCapsLock;
        auto ledState = sBoolCapsLock ? HIGH : LOW;
        digitalWrite( LED_1, ledState);
      }
    } else if ( i == 3 ) { 
      //SHIFT
      modifierFunc( Col_1, Col_13, sBoolShift, KEY_LEFT_SHIFT );
      anyKeyDown = anyKeyDown || sBoolShift;
    } else if ( i == 4 ) { 
      //Ctrl, ALT, Windows key
      modifierFunc( Col_0, Col_14, sBoolCtrl, KEY_LEFT_CTRL );
      modifierFunc( Col_3, Col_10, sBoolAlt, KEY_LEFT_ALT );
      modifierFunc( Col_1, Col_1, sBoolGui, KEY_LEFT_GUI );
      anyKeyDown = anyKeyDown || sBoolCtrl;
      anyKeyDown = anyKeyDown || sBoolAlt;
      anyKeyDown = anyKeyDown || sBoolGui;
      
      // remap menu and Fn key
      static bool sMenuKeyPressed = false;
      if ( nonRepeatingKeyPress( Col_13, sMenuKeyPressed ) ) {
        // add some macro for menu key
      }
      static bool sFunctionKeyPressed = false;
      if ( nonRepeatingKeyPress( Col_11, sFunctionKeyPressed ) ) {
        // add some macro for Fn key
      }
    }
    
    // loop over sKeysForRows array    
    for ( int j = 0; j < 18; ++j ) {
      if ( sKeysForRows[ i ][ j ] != 0x00 ) {
        unsigned long deltaPress = timeDelta( sLastPress[ i ][ j ], currentTimeMs );
        unsigned long deltaClear = timeDelta( sLastClear[ i ][ j ], currentTimeMs );
        if ( columnPressed(Col_0 + j) ) {
          int delayLocal = deltaClear > repeatDelayInitial ? repeatDelayRepeat : repeatDelayInitial;          
          if ( !sKeyPressed[ i ][ j ] ) {
            // Initial keypress
            pressKeyboardRespectCapLock(sKeysForRows[ i ][ j ]);
            sKeyPressed[ i ][ j ] = true;
            currentTimeMs = millis();
            sLastPress[ i ][ j ] = currentTimeMs;
          } else if ( deltaPress > delayLocal ) {
            // Manually implement key repeat by releasing and immediately pressing again
            keyboardRelease(sKeysForRows[ i ][ j ]);
            pressKeyboardRespectCapLock(sKeysForRows[ i ][ j ]);
            currentTimeMs = millis();
            sLastPress[ i ][ j ] = currentTimeMs;            
          }
        } else {
          if ( sKeyPressed[ i ][ j ] ){
            // Release key
            keyboardRelease(sKeysForRows[ i ][ j ]);
            sKeyPressed[ i ][ j ] = false;
          }
          currentTimeMs = millis();
          sLastClear[ i ][ j ] = currentTimeMs;
          // set sLastPress to some recent time just before sLastClear, so time deltas never get big
          if ( currentTimeMs > 10 ) {
            sLastPress[ i ][ j ] = currentTimeMs - 10;
          }
        }
      }
      anyKeyDown = anyKeyDown || sKeyPressed[ i ][ j ];
      if ( anyKeyDown ) {
        lastPressTime = currentTimeMs;
      }
    }
  }// after this brace rowIndices[ i ] is inactive

  clearAllLoaded = clearAllLoaded || anyKeyDown;
  // Safety in case keyrelease was missed for some reason and key seems to be stuck on.
  // Release all keys. Only do this once, one second after the last key was pressed.
  // This should not be an issue but do it just in case.
  if ( clearAllLoaded && !anyKeyDown && timeDelta( lastPressTime, currentTimeMs ) > 1000 ) {
    clearAllLoaded = false;
    Keyboard.releaseAll();
    delay( preDelay );
  }

  // Now do the function keys (row 0). These do not repeat
  {
    // activate row 0
    ScopedRowActive rowLow( Row_0 );

    for ( int j = 0; j < 18; ++j ) {
      if ( sKeysForRowsFn[ j ] != 0x00 ) {
        if ( nonRepeatingKeyPress( Col_0 + j, sFnPressed[ j ] ) ) {
          keyboardWrite( sKeysForRowsFn[ j ] );
        }
      }
    }

    // What good are the function keys nowadays? These can be easily converted to macro keys!
    // I've replaced most of the function keys with some handy shortcuts. If you want the F1-F12 keys instead of these shortcuts, 
    // see line 124 of this code. 


    // F1 (cut)
    if ( nonRepeatingKeyPress( Col_2, sFnPressed[ 2 ] ) ) {
      ScopedOsModifier sm;
      keyboardWrite( 'x' );
    }

    //F2 (Copy) 
    if ( nonRepeatingKeyPress( Col_3, sFnPressed[ 3 ] ) ) {
      ScopedOsModifier sm;
      keyboardWrite( 'c' );
    }

    // F3 (Paste)
    if ( nonRepeatingKeyPress( Col_4, sFnPressed[ 4 ] ) ) {
      ScopedOsModifier sm;
      keyboardWrite( 'v' );
    }

    //F4 (Undo)
    if ( nonRepeatingKeyPress( Col_5, sFnPressed[ 5 ] ) ) {
      ScopedOsModifier sm;
      keyboardWrite( 'z' );
    }

    // F5 (Redo)
    if ( nonRepeatingKeyPress( Col_7, sFnPressed[ 7 ] ) ) {
      ScopedOsModifier sm;
      keyboardWrite( 'y' );
    }

    // F6 (Search)
    if ( nonRepeatingKeyPress( Col_8, sFnPressed[ 8 ] ) ) {
      ScopedOsModifier sm;
      keyboardWrite( 'f' );
    }
    
    // F7 (Save)
    if ( nonRepeatingKeyPress( Col_9, sFnPressed[ 9 ] ) ) {
      ScopedOsModifier sm;
      keyboardWrite( 's' );
    }

        // F8 (Type my email address)
    if ( nonRepeatingKeyPress( Col_10, sFnPressed[ 10 ] ) ) {
       keyboardPrint( "email@address.com" );

    }

          
    //F9
    if ( nonRepeatingKeyPress( Col_11, sFnPressed[ 11 ] ) ) {

      //do something here.

    }
    
    //F10 Type my street address
    if ( nonRepeatingKeyPress( Col_12, sFnPressed[ 12 ] ) ) {
      keyboardPrint( "1234 Anywhere St" );
    }

     //F11 Type my street address
    if ( nonRepeatingKeyPress( Col_13, sFnPressed[ 13 ] ) ) {
        writeUnicode( "03f4" ); // types a Theta symbol
    }

     //F12 Type a Pi symbol
    if ( nonRepeatingKeyPress( Col_14, sFnPressed[ 14 ] ) ) {
       writeUnicode( "03c0" ); // types a Pi symbol
    }

    // Print Screen
   if ( checkMinimumKeyPress( Col_15 ) ) {
       //Instead of Scroll lock, this types an ohm symbol.
      writeUnicode( "03a9" );

      
//#ifdef MACOSX
//      // Mac whole screen capture
//      ScopedGui sg;
//      ScopedShift ss;
//      keyboardWrite( '3' );
//#elif defined WIN32
//      // Windows snipping tool
//      ScopedGui sg;
//      ScopedShift ss;
//      keyboardWrite( 's' );
//
//#endif


    }

    //Scrl:  
    if (checkMinimumKeyPress( Col_16 ) ) {
//#ifdef MACOSX
//      // Mac partial screen capture
//      ScopedGui sg;
//      ScopedShift ss;
//      keyboardWrite( '4' );
//#endif

      //Instead of pause, this types a micro symbol.
      writeUnicode( "03bc" );
    }

    //Pause: 
    if ( checkMinimumKeyPress( Col_17 ) ) {
//#ifdef MACOSX
//      // Mac screen lock
//      ScopedGui sg;
//      ScopedControl sc;
//      keyboardWrite( 'q' );
//#elif defined WIN32
//      // Windows screen lock
//      ScopedGui sg;
//      keyboardWrite( 'l' );      
//#endif

      //Instead of Print Screen this types a degree symbol
      writeUnicode( "00b0" );



    }

  } // after this brace row 0 is inactive

}
