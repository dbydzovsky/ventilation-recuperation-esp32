#include "Arduino.h"
#include "Lock.h"


boolean Lock::readLock() {
  this->_reads++;
  if (this->_writing) {
    this->_reads--;
    return false;
  }
  return true;
}

void Lock::readUnlock() {
  this->_reads--;
}

void Lock::writeUnlock() {
  this->_writing = false;
}

boolean Lock::writeLock() {
  if (!this->_writing) {
    this->_writing = true;
    while (this->_reads > 0) {
      delay(1);
    }
    return true;
  }
  return false;
}

void Lock::writeLockInfinite() {
  while(!this->writeLock()) {}
}

boolean Lock::writeLock(unsigned long timeoutMs) {
  
  unsigned long started = millis();
  while (millis() - started < timeoutMs) {
    if (this->writeLock()) {
      return true;
    }
    delay(1);
  }
  return false;
}