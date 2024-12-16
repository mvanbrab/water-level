/*!
 * @file LCDbarGraph.h
 *
 * A bar graph on a character-based LCD, controlled by an HD44780 chip.
 * 
 * Depends on the beautiful hd44780 library (https://github.com/duinoWitchery/hd44780) by Bill Perry,
 * which allows to make abstraction of how the HD44780 is interfaced to the processor. 
 *
 * Tested on a ESP32 DevKit C.
 *
 * @author Martin Vanbrabant
 */

#ifndef LCDBARGRAPH_H
#define LCDBARGRAPH_H

#include <hd44780.h>

/*! 
 *  Class responsible for one bar graph on the LCD
 */
class LCDbarGraph {
  public:
    /*!
     * Constructor
     * 
     * @param lcd reference to an initialised object controlling the LCD display
     * @param x x-position of the start of the bar (0-based, 0 = left)
     * @param y y-position of the start of the bar (0-based, 0 = top)
     * @param width number of character positions in the bar
     * @param minimum minimum value in the bar (left); negative values allowed
     * @param maximum maximum value in the bar (right); greater than minimum
     */
    LCDbarGraph(hd44780 &lcd, uint8_t x, uint8_t y, uint8_t width, int minimum, int maximum);
   /*!
     * Display a value in the bar
     *
     * @param value value to display; values outside the range [minimum, maximum] are clipped
     */
    void display(int value);
 
  private:
    hd44780  &_lcd;
    uint8_t  _x;
    uint8_t  _y;
    uint8_t  _width;
    int      _minimum;
    int      _maximum;
    int      _prevPixels;
    int      _prevFullChars;
    int      _prevUsedChars;
    bool     _initialised;
    
    static bool _charsCreated;
  
    static const uint8_t _vbar0[8];
    static const uint8_t _vbar1[8];
    static const uint8_t _vbar2[8];
    static const uint8_t _vbar3[8];
    static const uint8_t _vbar4[8];
    static const uint8_t _vbar5[8];
    
    void createChars(void);
    void begin(void);
};

#endif
