/*
  Morse.h - Library for flashing Morse code.
  Created by David A. Mellis, November 2, 2007.
  Released into the public domain.
*/

#ifndef DewPoint_h
#define DewPoint_h
#include "Arduino.h"

class DewPoint {
  public:
    DewPoint();
    /**
     * @brief 
     * 
     * @param humidity Can be NAN
     * @param temperature Can be NAN
     */
    void compute(float humidity, float temperature);
    /**
     * @brief Get the Dew Point object
     * 
     * @return float dewPoint or NAN if not computed.
     */
    float getDewPoint();
  private:
    /**
     * @brief For free space reasons, value is stored in short, but converted to float when needed
     * 
     */
    short _dewPoint = -20000;
};

#endif