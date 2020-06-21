/*!
 * @file LCDbarGraph.cpp
 *
 * @author Martin Vanbrabant
 */

#include "LCDbarGraph.h"

bool LCDbarGraph::_charsCreated = false;

#if 1
// classic vertical bars 7 pixels high
const uint8_t LCDbarGraph::_vbar0[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
const uint8_t LCDbarGraph::_vbar1[8] = {0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x00};
const uint8_t LCDbarGraph::_vbar2[8] = {0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x00};
const uint8_t LCDbarGraph::_vbar3[8] = {0x1c, 0x1c, 0x1c, 0x1c, 0x1c, 0x1c, 0x1c, 0x00};
const uint8_t LCDbarGraph::_vbar4[8] = {0x1e, 0x1e, 0x1e, 0x1e, 0x1e, 0x1e, 0x1e, 0x00};
const uint8_t LCDbarGraph::_vbar5[8] = {0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x00};
#endif
#if 0
// vertical bars 5 pixels high + fixed dotted lower line
const uint8_t LCDbarGraph::_vbar0[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x15, 0x00};
const uint8_t LCDbarGraph::_vbar1[8] = {0x10, 0x10, 0x10, 0x10, 0x10, 0x00, 0x15, 0x00};
const uint8_t LCDbarGraph::_vbar2[8] = {0x18, 0x18, 0x18, 0x18, 0x18, 0x00, 0x15, 0x00};
const uint8_t LCDbarGraph::_vbar3[8] = {0x1c, 0x1c, 0x1c, 0x1c, 0x1c, 0x00, 0x15, 0x00};
const uint8_t LCDbarGraph::_vbar4[8] = {0x1e, 0x1e, 0x1e, 0x1e, 0x1e, 0x00, 0x15, 0x00};
const uint8_t LCDbarGraph::_vbar5[8] = {0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x00, 0x15, 0x00};
#endif
#if 0
// horizontal bars 5 pixels high + fixed dotted lower line
const uint8_t LCDbarGraph::_vbar0[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x15, 0x00};
const uint8_t LCDbarGraph::_vbar1[8] = {0x00, 0x00, 0x00, 0x00, 0x1f, 0x00, 0x15, 0x00};
const uint8_t LCDbarGraph::_vbar2[8] = {0x00, 0x00, 0x00, 0x1f, 0x1f, 0x00, 0x15, 0x00};
const uint8_t LCDbarGraph::_vbar3[8] = {0x00, 0x00, 0x1f, 0x1f, 0x1f, 0x00, 0x15, 0x00};
const uint8_t LCDbarGraph::_vbar4[8] = {0x00, 0x1f, 0x1f, 0x1f, 0x1f, 0x00, 0x15, 0x00};
const uint8_t LCDbarGraph::_vbar5[8] = {0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x00, 0x15, 0x00};
#endif
#if 0
// diagonal bars 5 pixels high + fixed dotted lower line
const uint8_t LCDbarGraph::_vbar0[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x15, 0x00};
const uint8_t LCDbarGraph::_vbar1[8] = {0x00, 0x00, 0x10, 0x18, 0x1c, 0x00, 0x15, 0x00};
const uint8_t LCDbarGraph::_vbar2[8] = {0x00, 0x10, 0x18, 0x1c, 0x1e, 0x00, 0x15, 0x00};
const uint8_t LCDbarGraph::_vbar3[8] = {0x10, 0x18, 0x1c, 0x1e, 0x1f, 0x00, 0x15, 0x00};
const uint8_t LCDbarGraph::_vbar4[8] = {0x18, 0x1c, 0x1e, 0x1f, 0x1f, 0x00, 0x15, 0x00};
const uint8_t LCDbarGraph::_vbar5[8] = {0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x00, 0x15, 0x00};
#endif

LCDbarGraph::LCDbarGraph(hd44780 &lcd, uint8_t x, uint8_t y, uint8_t width, int minimum, int maximum) :
  _lcd(lcd),
  _x(x),
  _y(y),
  _width(width),
  _minimum(minimum),
  _maximum(maximum),
  _prevPixels(0),
  _prevFullChars(0),
  _prevUsedChars(0),
  _initialised(false)
{
}

void LCDbarGraph::createChars(void) {
  _lcd.createChar(0, _vbar0);
  _lcd.createChar(1, _vbar1);
  _lcd.createChar(2, _vbar2);
  _lcd.createChar(3, _vbar3);
  _lcd.createChar(4, _vbar4);
  _lcd.createChar(5, _vbar5);
  _charsCreated = true;
}

void LCDbarGraph::begin(void) {
  if (!_charsCreated) {
    createChars();
  }
  _lcd.setCursor(_x, _y);
  for (byte i = 0 ; i < _width ; i++) {
    _lcd.write(0);
  }
  _initialised = true;
}

void LCDbarGraph::display(int value) {
  if (!_initialised) {
    begin();
  }
  // limit and base the input
  int v;
  if (value > _maximum) {
    v = _maximum - _minimum;
  } else if (value < _minimum) {
    v = 0;
  } else {
    v = value - _minimum;
  }
  // how many pixel positions?
  int pixels = ((long)v * (_width * 5)) / (_maximum - _minimum);
  if (pixels != _prevPixels) {
    // how many full character positions?
    byte fullChars = pixels / 5;
    // how many pixel postions after the full characters?
    byte remainingPixels = pixels - 5 * (int)fullChars;
    if (fullChars > _prevFullChars) {
      // add full characters
      _lcd.setCursor(_x + _prevFullChars, _y);
      for (byte i = _prevFullChars ; i < fullChars ; i++) {
        _lcd.write(5);
      }
    } else {
      // beyond kept full characters
      _lcd.setCursor(_x + fullChars, _y);
    }
    byte usedChars = fullChars;
    if (remainingPixels > 0) {
      // partial character
      _lcd.write(remainingPixels);
      usedChars++;
    }
    for (byte i = usedChars ; i < _prevUsedChars ; i++) {
      // erase remainder of previous bar
      _lcd.write(0);
    }
    _prevPixels = pixels;
    _prevFullChars = fullChars;
    _prevUsedChars = usedChars;
  }
}
