#include <Arduino.h>
#pragma once

#include "lib7segmentfont.h"

#define COMMON_ANODE true
#define COMMON_CATHODE false

#if !(defined(NUMBERS_ONLY) || defined(ALPHANUMERIC_ONLY))
    #define _CHAR_SEARCH_STRING_LENGTH_ 49
    const char PROGMEM _CHAR_SEARCH_STRING_[] = " -.0123456789ABCDEFGHIJLNOPRSTUY�����������������";
#elif (!defined NUMBERS_ONLY) && (defined ALPHANUMERIC_ONLY)
    #define _CHAR_SEARCH_STRING_LENGTH_ 32
    const char PROGMEM _CHAR_SEARCH_STRING_[] = " -.0123456789ABCDEFGHIJLNOPRSTUY";
#elif (defined NUMBERS_ONLY) && !(defined ALPHANUMERIC_ONLY)
    #define _CHAR_SEARCH_STRING_LENGTH_ 19
    const char PROGMEM _CHAR_SEARCH_STRING_[] =  " -.0123456789ABCDEF";
#endif


class sevenSegmentScreenShifted
{
private:
    PGM_P charSearchString = _CHAR_SEARCH_STRING_;
    byte charSearchStringLength = _CHAR_SEARCH_STRING_LENGTH_;
    byte latchPin;
    byte dataPin; 
    byte clockPin;
    byte numDigits;
    bool commonPin;
    byte* displaySegmentBytes;
public:
    sevenSegmentScreenShifted(byte latchPin, byte dataPin, byte clockPin, byte numDigits, bool common);
    void clear(void);
    void setText(char* text);
    ~sevenSegmentScreenShifted();
};

sevenSegmentScreenShifted::sevenSegmentScreenShifted(byte latchPin, byte dataPin, byte clockPin, byte numDigits, bool common = COMMON_ANODE)
{
    this->latchPin  =  latchPin;
    this->dataPin   =   dataPin;
    this->clockPin  =  clockPin;
    this->numDigits = numDigits;
    this->commonPin =    common;

    this->displaySegmentBytes = new byte[this->numDigits];
}

sevenSegmentScreenShifted::~sevenSegmentScreenShifted()
{
    delete[] this->displaySegmentBytes;
}

// We just fill all the digits with charSearchString[0] (i.e. blank space " ")
// just making all bits inverted if we use a common cathode display
void sevenSegmentScreenShifted::clear(void)
{
    digitalWrite(this->latchPin, LOW);
    
    if (this->commonPin == COMMON_ANODE)
        for (byte i = 0; i < this->numDigits; i++)
            shiftOut(this->dataPin, this->clockPin, LSBFIRST, pgm_read_byte(&numberSegmentsFont[0]));
    else    
        for (byte i = 0; i < this->numDigits; i++)
            shiftOut(this->dataPin, this->clockPin, LSBFIRST, ~pgm_read_byte(&numberSegmentsFont[0]));

    digitalWrite(this->latchPin, HIGH);
}

// Here we parse the input text and set it to the shift register
// just making all bits inverted if we use a common cathode display
void sevenSegmentScreenShifted::setText(char* text)
{
    byte i = 0;
    byte digitNum = 0;

    // text should not be empty or start with ".". If it does, that's definetly an user's mistake,
    // so we clear the screen and exit.
    if((text[0] == '.') || (text[0] == '\0'))
    {
        this->clear();
        return;
    }

    // Make this->displaySegmentBytes[] contain all bits to manipulate individual segments
    do
    { 
        this->displaySegmentBytes[digitNum] = pgm_read_byte(&numberSegmentsFont[0]); // Set digit to blank space in case there is an unprintable char
        for (byte j = 0; j < this->charSearchStringLength; j++)
        {            
            if (text[i] == '.')
            {
                bitClear(this->displaySegmentBytes[digitNum - 1], 0); // That's only a dot. So we apply it to previous character. 
                
                digitNum++; // Since it'a dot, we'll have to iterate tis symbol one more time
                continue; 
            }

            byte segByte = pgm_read_byte(&this->charSearchString[j]); 
            
            if (segByte == (byte)text[i])
            { 
                this->displaySegmentBytes[digitNum] = pgm_read_byte(&numberSegmentsFont[j]);                
            }
        }

        digitNum++;
        i++;
    } while ((text[i] != '\0') || (digitNum < this->numDigits));

    // Shift out this->displaySegmentBytes[] to SN74HC165N registers to make our screen glow!
    // Again, we are making all bits inverted if we use a common cathode display
    for (byte j = 0; j < this->numDigits; j++)
    {
        digitalWrite(this->latchPin, LOW);
            
        if (this->commonPin == COMMON_ANODE)
            for (byte i = 0; i < this->numDigits; i++)
                shiftOut(this->dataPin, this->clockPin, LSBFIRST, this->displaySegmentBytes[j]);
        else    
            for (byte i = 0; i < this->numDigits; i++)
                shiftOut(this->dataPin, this->clockPin, LSBFIRST, ~this->displaySegmentBytes[j]);

        digitalWrite(this->latchPin, HIGH);
    }
}