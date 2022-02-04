/*
  Morse.h - Library for flashing Morse code.
  Created by David A. Mellis, November 2, 2007.
  Released into the public domain.
*/

#ifndef Lock_h
#define Lock_h
#include "Arduino.h"

class Lock {
  public:
    bool readLock();
    void readUnlock();
    bool writeLock();
    bool writeLock(unsigned long timeoutMs);
    void writeLockInfinite();
    void writeUnlock();
  private:
    byte _reads = 0;
    bool _writing = 0;
};

#endif