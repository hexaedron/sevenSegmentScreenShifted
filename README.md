# sevenSegmentScreenShifted
A small Arduino library and font for 7-segment LED displays.

# What is this library for?
This library is usable when you connect some simple 7-segment digits in a row using the 74HC595 registers.

# How to install it?
Just put both .h files into your include dir

# How to use it?
First of all, connect all your digits to 74HC595 chips and connect 74HC595 chips together according to its datasheet. If you want to use the brighntess control, you need one more PWM pin in your Arduino.

Please see an example (please note it is in PlatformIO style, no setup() and loop(), but of course you can use them):

```cpp
#include "lib7SegmentScreenShifted.h"

#define CLOCK 10
#define DATA 8 
#define LATCH 9
#define PWM 6 

int main(int argc, char **argv) 
{
    init();

    // Init all the pins for output  
    pinMode(CLOCK, OUTPUT);
    pinMode(DATA, OUTPUT);
    pinMode(LATCH, OUTPUT); 
    pinMode(PWM, OUTPUT); 

    // Create a screen. 4 means 4 digits here
    sevenSegmentScreenShifted CA56_GWA(LATCH, DATA, CLOCK, PWM, 4, COMMON_ANODE);

    // That's obvious. We clear our screen
    CA56_GWA.clear();

    for(;;) 
    {
        // Set low brightness
        CA56_GWA.setBrightness(64);
        CA56_GWA.clear();

        // Set text
        CA56_GWA.setText("HI\0");
        delay(1000); 
        CA56_GWA.clear();   
        
        // Set text
        CA56_GWA.setText("GYUS\0");
        delay(1000); 
        CA56_GWA.clear();

        // Set text
        CA56_GWA.setText("LET.S\0");
        delay(1000);
        CA56_GWA.clear(); 

        // Set text
        CA56_GWA.setText("HAUE\0");
        delay(1000); 
        CA56_GWA.clear();

        // BEER have to be visible!
        CA56_GWA.setBrightness(255);

        // Set text
        CA56_GWA.setText("BEER\0");
        delay(2000); 
    }
```

If you don't want to use PWM, you should invoke a constructor in following way (no PWM pin in definition):
```cpp
sevenSegmentScreenShifted CA56_GWA(LATCH, DATA, CLOCK, 4, COMMON_ANODE);
```

If you want to print a dot, just pass the dot following the symbol you want to have one, see the examples:
```cpp
CA56_GWA.setText("12.34\0");
CA56_GWA.setText("1.2.3.4.\0");
CA56_GWA.setText("D.O.T.\0");
CA56_GWA.setText(" .A .B\0");
```

All the symbols you pass to `setText` have to be in uppercase. All the symbols which are not present in the font (`lib7segmentfont.h`), will be changed to a blank space. 

Bad examples (please don't do that):
```cpp
CA56_GWA.setText("");
CA56_GWA.setText("\0");
CA56_GWA.setText(".1\0");
```


# Useful defines

1. `COMMON_ANODE` and `COMMON_CATHODE`: use them when constructing a `sevenSegmentScreenShifted` object according to datasheet of your display. You can't mix digits of different types in a single display.
1. `ALPHANUMERIC_ONLY` or `NUMBERS_ONLY`: define **one of them** before the `lib7SegmentScreenShifted.h` to slightly reduce an amount of flash memory if you don't need Cyrillic letters or if you need numbers only.
1. `ENABLE_UNPRINTABLE_CHARS`: This is a low-level define. Use it only if you want to use the font (`lib7segmentfont.h`) separately. It makes the `numberSegmentsFont[]` contain blank spaces in places of unprintable chars instead of just skipping them.

# Notes for Cyrillic users
Remember to save all your files containing Cyrillic letters in Windows-1251 encoding!

Cyrillic examples:
```cpp
CA56_GWA.setText("ГДЕ\0");
CA56_GWA.setText("Н.Л.О.\0");
```